//-----------------------------------------------------------------------------
// F38x_LFO_Idle.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// The program demonstrates the Low Frequency internal oscillator and power
// savings mode operation on the F380. The MCU will sleep in low power idle
// mode with only the low frequency oscillator and timer T0 running. When 
// the MCU wakes up it will switch to a 24 MHz clock and turn on an LED
// and wait 100 ms. The 100 ms delay also uses T0 and the low frequency 
// oscillator but does not use idle mode or interrupts.
// 
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J19: Power - P1_PWR or SER_PWR
//    - J7: +3VD - VDD
//    - J12: P2.3 - P2.3_LED
// 2) Connect the USB Debug Adapter to J9. If P1_PWR power is selected on J19,
//    connect the 9V power adapter to P1.
// 3) Compile and download code to a 'F380 device on a C8051F380-TB development
//    board selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) P2.3_LED should blink once a second.
//
//
//
// Target:         C8051F380
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 code with compiler agnostic code
//    - 09 JAN 2014
//
// Release 1.0
//    -Initial Revision (KAB)
//    -07 AUG 2006
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F380_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SLEEP_TIME 10000               // sleep 1 second
#define BLINK_TIME  1000               // blink 100 ms

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
   PCA0MD  &= ~0x40;
}
 
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(Timer0_ISR, TIMER0_IRQn);

void main (void);
void OSC_Init (void);           
void PORT_Init (void);
void lowPowerSleep (uint16_t);
void configMCU_Sleep (void);
void configMCU_Wake (void);
void delay (uint16_t);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P2, 3);                      // Green LED='1' means ON

//-----------------------------------------------------------------------------
// main () Routine
//-----------------------------------------------------------------------------

void main (void)
{
   PCA0MD   = 0x00;

   OSC_Init();
   PORT_Init();
   
   IE_EA = 1;                             // set global interrupt enable
   
   while(1)
   {
      lowPowerSleep(SLEEP_TIME);
      LED = 1;
      delay(1000);
      LED = 0;
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
   P2MDOUT   = 0x08;                    // enable P2.3 output
   XBR1     |= 0x40;                    // enable crossbar
}
//-----------------------------------------------------------------------------
// OSC_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
//-----------------------------------------------------------------------------

void OSC_Init ()
{
   OSCICN    =  0x83;                  // enable 24.5 MHz int16_t osc
}

//-----------------------------------------------------------------------------
// lowPowerSleep ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will put the MCU into low power sleep mode using a T0
// interrupt to wake up the MCU. In sleep mode the MCU will be running off
// the internal low frequency oscillator running at 10 kHz with all of the
// peripherals disabled except T0. 
//-----------------------------------------------------------------------------

void lowPowerSleep (uint16_t time)
{   
   uint8_t restoreIE;
   uint8_t restoreEIE1;
   
   IE_EA = 0;                             // clear global interrupt enable
   
   // save interrupt state
   restoreIE = IE;                     // preserve IE register
   restoreEIE1 = EIE1;                 // preserve EIE1 register
   
   // disable all interrupts  
   IE       = 0x00;                   // disable interrupts
   EIE1     = 0x00;                   // disable extended interrupts
       
   configMCU_Sleep ();                  // configure for Sleep
   
   //init T0 to wake up MCU 

   TCON    &= ~0x30;                   // stop T0 & clear flag
   TMOD    &= ~0x0F;                   // clear T0 nibble of TMOD
   TMOD    |= 0x01;                    // T0 16 bit counter mode
   CKCON   |= 0x04;                    // T0 uses SYSCLK
   
   // set T0 using time
   time = -time;
   TL0      = (uint8_t)(time & 0x00FF);
   TH0      = (uint8_t)(time >> 8);  
   
   // Comment out this line to measure steady state idle current
   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   IE      |= 0x82;                    // enable T0 & global interrupts
   //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

   //switch to low freq oscillator
   OSCLCN   = 0x80;                    // LFO is 10 kHz
   CLKSEL   = 0x04;                    // Switch to LFO
   OSCICN   = 0x00;                    // Stop internal osc to save power                          

   TCON    |= 0x10;                    // enable T0
   
   // go to IDLE mode
   PCON    |= 0x01;                    // Idle mode
   PCON     = PCON;
   
   //--wake--//

   configMCU_Wake ();
   
   // restore MCU pin configuration to portInit state & restore LEDs
   EIE1     = restoreEIE1;             // restore ext interrupts
   IE       = restoreIE;               // restore interrupts
   IE_EA = 1;                             // set global interrupt enable
}

//-----------------------------------------------------------------------------
// configMCU_Sleep ()
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

void configMCU_Sleep (void)
{
   XBR1    &= ~0xC0;                  // disable crossbar, enabled weak pull ups

   // disable all peripherals for low power sleep
   
   SCON0   &= ~0x10;                   // disable UART0
   TCON    &= ~0x40;                   // disable T1
   TMR2CN  &= ~0x04;                   // disable T2

   // disable any additional peripherals here

   VDM0CN  &= ~0x80;                   // disable voltage monitor                                       
}

//-----------------------------------------------------------------------------
// configMCU_Wake ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will configure the MCU for wake mode operation.
// Enable additional peripherals as needed.
//-----------------------------------------------------------------------------

void configMCU_Wake (void)
{
   VDM0CN  |= 0x80;                    // enable voltage monitor

   // enable any additional peripherals here

   XBR1    |= 0x40;                    // enable crossbar
}

//-----------------------------------------------------------------------------
// delay ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : time - (10000 = 1 second)
//
// This function will implement a delay using T0 and the LFO 10 kHz clock.
// A delay time of 10000 counts is equal to one second. This function modifies
// the system clock and exits with a 24.5 MHz clock.
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
   OSCLCN   = 0x80;                    // LFO is 10 kHz
   CLKSEL   = 0x04;                    // Switch to LFO
   TCON    |= 0x10;                    // enable T0
   while (!TCON_TF0);                        // wait for TOF
   TCON    &= ~0x30;                   // stop T0 & clear flag
   OSCICN    =  0x83;                  // enable 24.5 MHz int16_t osc
   CLKSEL    = 0x00;                   // use int16_t osc
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------
// T0ISR ()
//-----------------------------------------------------------------------------

SI_INTERRUPT(Timer0_ISR, TIMER0_IRQn)
{   
   OSCICN    =  0x83;                  // enable 24.5 MHz int16_t osc
   while ((OSCICN & 0x40) == 0);       // wait for lock bit
   CLKSEL    = 0x00;                   // use int16_t osc
   TCON   &= ~0x10;                    // stop timer T0   
   IE     &= ~0x02;                    // disable T0 interrupt 
}

//-----------------------------------------------------------------------------
// END F38x_LFO_Sleep.c
//-----------------------------------------------------------------------------
