//-----------------------------------------------------------------------------
// F41x_RTC_Suspend.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// The program demonstrates the SmaRTClock oscillator and power
// savings mode operation on the F410. The MCU will sleep in low power suspend
// mode with only the RTC running. When the MCU wakes up it will switch
// to a 24.5 MHz internal oscillator & turn on an LED. It will then switch
// to the 32kHz RTC clock and wait 10 ms. The 10 ms delay also uses T0
// and the RTC clock oscillator but does not use idle mode or interrupts.
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    J9:   VUNREG (9V adapter) or 5VEC3 (Debugger) 
//    J10:  +3VD/+3VD
//    J12:  +3VD/VREGIN
//          VREGIN_EN
//    J17:  5VEC3/VIO or +3VD/VIO
//          VIO_EN
//    J5:   P2.3/D5
// 4) Connect the USB Debug Adapter to J4. If VUNREG is selected on J9,
//    connect the 9V power adapter to P1.
// 5) Compile and download code to the C8051F410-TB development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in the
//    quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//    LED D5 should blink once a secoond.
//
// Target:         C8051F41x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.2 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 specific code with compiler agnostic code from 
//      compiler_defs.h.
//    - 21 JAN 2014
//
// Release 1.1 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
//
// Release 1.0
//    -Initial Revision (KAB)
//    -07 AUG 2006
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F410_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define RTC0CN    0x06                 // RTC address of RTC0CN register
#define RTC0XCN   0x07                 // RTC address of RTC0XCN register

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
   PCA0MD  &= ~0x40;                   // disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(RTC_ISR, 8);

void main (void);
void OSC_Init (void);
void PORT_Init (void);
void RTC_Init (void);
uint8_t RTC_Read (uint8_t);
void RTC_Write (uint8_t, uint8_t);
void RTC_SetBits (uint8_t, uint8_t);
void RTC_ClearBits (uint8_t, uint8_t);
void RTC_ClearCapture (void);
void RTC_ClearAlarm (void);
void RTC_BumpAlarm (uint8_t code *p);
void configSleep (void);
void configWake (void);
void delay (uint16_t);
                   // interrupt key word not permitted.

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P2, 3);                  // port pin used for LED

// RTC_Interval - little endian data added to alarm register = 1 second
uint8_t code RTC_Interval[6]= {0x00,0x00,0x01,0x00,0x00,0x00};

//-----------------------------------------------------------------------------
// main () Routine
//-----------------------------------------------------------------------------

void main (void)
{

   OSC_Init ();                        // initialize 24.5 MHz oscillator
   PORT_Init ();                       // initialize IO ports
   RTC_Init ();                        // initiralize RTC

   IE_EA = 1;                             // set global interrupt enable

   while (1)
   {
      configSleep ();                  // configure MCU for low power sleep
      OSCICN |= 0x20;                  // set suspend mode bit
      configWake ();                   // configure MCU for wake mode
      LED = 1;                         // turn on LED
      delay (164);                     // wait 50 ms
      LED = 0;                         // turn off LED
   }
}

//-----------------------------------------------------------------------------
// PORT_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
// P1.3 push-pull output LED
//-----------------------------------------------------------------------------

void Port_Init ()
{
// configure Port IO here
   P2MDOUT   = 0x08;                   // enable P2.3 output
   XBR1     |= 0x40;                   // enable crossbar
}

//-----------------------------------------------------------------------------
// OSC_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the internal oscilloator.
// Note that the OSCICN setting for 24.5MHz is different than F12x and F3xx
//
//-----------------------------------------------------------------------------

void OSC_Init ()
{
   OSCICN    =  0x87;                  // enable 24.5 MHz int16_t osc
}

//-----------------------------------------------------------------------------
// RTC_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will initialize the RTC. First it unlocks the RTC interface,
// enables the RTC, clears ALRMn and CAPTUREn bits. Then it sets up the RTC
// to operate using a 32.768khZ crystal. Lastly it enables the alarm and
// interrupt. This function uses the RTC primitive functions to access
// the internal RTC registers.
//
//-----------------------------------------------------------------------------

void RTC_Init (void)
{
   uint8_t i;

   RTC0KEY = 0xA5;                     // unlock the RTC interface
   RTC0KEY = 0xF1;

   RTC_Write (RTC0CN, 0x80);           // enable RTC

   RTC_ClearCapture ();                // clear CAPTUREn registers
   RTC_ClearAlarm ();                  // clear ALARMn registers

   RTC_SetBits (RTC0XCN, 0x40);        // set crystal mode

   OSCICN    =  0x80;                  // switch to 192 kHz int16_t osc
   for (i=0xFF;i!=0;i--);              // wait > 1 ms
   OSCICN    =  0x87;                  // switch back to 24.5 MHz int16_t osc

   // wait for clock valid
   while ((RTC_Read (RTC0XCN) & 0x10)== 0x00);

   RTC_SetBits (RTC0CN, 0x40);         // enable missing clock detector
   RTC_SetBits (RTC0XCN, 0x80);        // enable ACG to save current

   RTC_SetBits (RTC0CN, 0x02);         // transfer capture to clock
   RTC_SetBits (RTC0CN, 0x10);         // enable RTC run
   RTC_SetBits (RTC0CN, 0x08);         // enable RTC alarm

   EIE1 |= 0x02;                       // enable RTC interrupt
}

//-----------------------------------------------------------------------------
// configSleep ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the MCU for low power sleep disabling the crossbar,
// enabling weak pull-ups, and turning off MCU peripherals. The lowest possible
// sleep current can be obtained by turning off the Voltage monitor.
//
//-----------------------------------------------------------------------------

void configSleep (void)
{
   XBR1    &= ~0xC0;                   // disable crossbar, enabled weak pull ups

   // disable all peripherals for low power sleep

   SCON0   &= ~0x10;                   // disable UART0
   TCON    &= ~0x10;                   // disable T0
   TCON    &= ~0x40;                   // disable T1
   TMR2CN  &= ~0x04;                   // disable T2

   // disable any additional peripherals here

   VDM0CN  &= ~0x80;                   // disable voltage monitor

}

//-----------------------------------------------------------------------------
// configWake
//
// Return Value : None
// Parameters   : None
//
// This function will configure the MCU for wake mode operation.
// Enable additional peripherals as needed.
//-----------------------------------------------------------------------------

void configWake (void)
{
   VDM0CN  |= 0x80;                    // enable voltage monitor
   TCON    |= 0x10;                    // enable T0

   // enable any additional peripherals here

   XBR1    |= 0x40;                    // enable crossbar
}

//-----------------------------------------------------------------------------
// delay ()
//
// Return Value : None
// Parameters   : None
//
// This function will implement a delay using T0 and the RTC 32.768 kHz clock.
// A delay time of 32768 counts is equal to one second. This function modifies
// the system clock and exits with a 24.5 MHz clcok.
//-----------------------------------------------------------------------------

void delay (uint16_t time)

{
   TCON    &= ~0x30;                   // stop T0 & clear flag
   TMOD    &= ~0x0F;                   // clear T0 nibble of TMOD
   TMOD    |= 0x01;                    // T0 16 bit counter mode
   CKCON   |= 0x04;                    // T0 uses SYSCLK
   // set T0 using time
   time = -time;
   TL0      = (uint8_t)(time & 0x00FF);
   TH0      = (uint8_t)(time >> 8);
   //switch to low freq oscillator
   CLKSEL   = 0x03;                    // Switch to RTC clock
   TCON    |= 0x10;                    // enable T0
   while (!TCON_TF0);                       // wait for TOF
   TCON    &= ~0x30;                   // stop T0 & clear flag
   OSCICN    =  0x87;                  // enable 24.5 MHz int16_t osc
   CLKSEL    = 0x00;                   // use int16_t osc
}

//=============================================================================
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// T0ISR ()
//-----------------------------------------------------------------------------

SI_INTERRUPT(RTC_ISR, 8)
{
   RTC_ClearBits (RTC0CN, 0x0C);       // clear RTCAEN and ALARM bits
   RTC_BumpAlarm (RTC_Interval);       // Add RTC interval to ALARMn
   RTC_SetBits (RTC0CN, 0x08);         // enable RTC alarm
}

//=============================================================================
// RTC Primitive Fuctions - used for read, write, and bit twiddling
//-----------------------------------------------------------------------------
// RTC_Read ()
//-----------------------------------------------------------------------------
//
// Return Value : RTC0DAT
// Parameters   : 1) uint8_t reg - address of RTC register to read
//
// This function will read one byte from the specified RTC register.
// Using a register number greater that 0x0F is not permited,
//
//-----------------------------------------------------------------------------

#pragma DISABLE
uint8_t RTC_Read (uint8_t reg)
{
   reg &= 0x0F;                        // mask low nibble
   RTC0ADR  = reg;                     // pick register
   RTC0ADR |= 0x80;                    // set BUSY bit to read
   while ((RTC0ADR & 0x80) == 0x80);   // poll on the BUSY bit
   return RTC0DAT;                     // return value
}

//-----------------------------------------------------------------------------
// RTC_Write ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) uint8_t reg - address of RTC register to write
//                2) uint8_t value - the value to write to <reg>
//
// This function will Write one byte from the specified RTC register.
//
//-----------------------------------------------------------------------------

#pragma DISABLE
void RTC_Write (uint8_t reg, uint8_t value)
{
   reg &= 0x0F;                        // mask low nibble
   RTC0ADR  = reg;                     // pick register
   RTC0DAT = value;                    // write data
   while ((RTC0ADR & 0x80) == 0x80);   // poll on the BUSY bit
}

//-----------------------------------------------------------------------------
// RTC_SetBits ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) uint8_t reg - address of RTC register to set
//                2) uint8_t value - bitmask for the register
//
// This function will set bits in the specified RTC register using the
// supplied bit mask. This function can only be used with an RTC register.
// Using a register number greater that 0x0F is not permited,
//
//-----------------------------------------------------------------------------

#pragma DISABLE
void RTC_SetBits (uint8_t reg, uint8_t value)
{
   reg &= 0x0F;                        // mask low nibble
   RTC0ADR  = reg;                     // pick register
   RTC0ADR |= 0x80;                    // set BUSY bit to read
   while ((RTC0ADR & 0x80) == 0x80);   // poll on the BUSY bit
   value  |= RTC0DAT;                  // read RTC0DAT and OR to set bits
   RTC0ADR  = reg;                     // overwrite autoincrement value
   RTC0DAT = value;                    // write to RTC0DAT
   while ((RTC0ADR & 0x80) == 0x80);   // poll on the BUSY bit
}

//-----------------------------------------------------------------------------
// RTC_ClearBits ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) uint8_t reg - address of RTC register to clear
//                2) uint8_t value - bitmask for the register
//
// This function will clear bits in the specified RTC register using the
// supplied bit mask, ones will be cleared.
//
// Note that writing to the ALARMn or CAPTUREn registers will autoincrement
// RTC0ADDR. So the RTC0ADDR register is set twice.
//
//-----------------------------------------------------------------------------

#pragma DISABLE
void RTC_ClearBits (uint8_t reg, uint8_t value)
{
   reg &= 0x0F;                        // mask low nibble
   value =~value;                      // complement mask
   RTC0ADR  = reg;                     // pick register
   RTC0ADR |= 0x80;                    // set BUSY bit to read
   while ((RTC0ADR & 0x80) == 0x80);   // poll on the BUSY bit
   value  &= RTC0DAT;                  // read RTC0DAT and AND to clear bits
   RTC0ADR  = reg;                     // overwrite autoincrement value
   RTC0DAT = value;                    // write to RTC0DAT
   while ((RTC0ADR & 0x80) == 0x80);   // poll on the BUSY bit
}

//-----------------------------------------------------------------------------
// RTC_ClearCapture ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : none
//
// This function will clear all six CAPTURE registers of the RTC.
//
//-----------------------------------------------------------------------------

#pragma DISABLE
void RTC_ClearCapture (void)
{
   uint8_t n;

   for (n=0;n<6;n++)                   // n = 0-5 for CAPTURE0-5
   {
      RTC0ADR = n;                     // select CAPTUREn register
      RTC0DAT = 0x00;                  // clear data
      while ((RTC0ADR & 0x80) == 0x80);// poll on the BUSY bit
   }
}

//-----------------------------------------------------------------------------
// RTC_ClearAlarm ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : none
//
// This function will clear all six ALARM registers of the RTC.
//
//-----------------------------------------------------------------------------

#pragma DISABLE
void RTC_ClearAlarm (void)
{
   uint8_t n;

   for (n=8;n<14;n++)                  // n = 8-13 for ALARM0-5
   {
      RTC0ADR = n;                     // select ALARMn
      RTC0DAT = 0x00;                  // clear data
      while ((RTC0ADR & 0x80) == 0x80);// poll on the BUSY bit
   }
}

//-----------------------------------------------------------------------------
// RTC_BumpAlarm ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : pointer to 6 byte value stored in code flash memory
//
// This function will add the 48-bit value located by the supplied pointer
// to the 48-bit value in the ALARM registers. The new sum is calculated
// byte-by-byte and store back into the ALARM registers. This is more efficient
// that reading all six bytes, adding, and writing back.
//
// Note Autoread is not used. However the RTC0ADDR register is
// auto-incremented. So the RTC0ADR must be set a second time.
//
//-----------------------------------------------------------------------------

#pragma DISABLE
void RTC_BumpAlarm (uint8_t code *p)
{
   uint8_t n;
   uint16_t sum;

   sum = 0;

   for (n=8;n<14;n++)                  // n = 8-13 for ALARM0-5
   {
      RTC0ADR = n;                     // select ALARMn
      RTC0ADR |= 0x80;                 // set BUSY bit to initiate read
      while ((RTC0ADR & 0x80) == 0x80);// poll on the BUSY bit
      sum += RTC0DAT;                  // add to sum
      sum += *p++;                     // add constant value
      RTC0ADR = n;                     // select ALARMn again
      RTC0DAT = (uint8_t)(sum);  // write sum LSB
      while ((RTC0ADR & 0x80) == 0x80);// poll on the BUSY bit
      sum = sum>>8 ;                   // save remainder
   }
}

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------
