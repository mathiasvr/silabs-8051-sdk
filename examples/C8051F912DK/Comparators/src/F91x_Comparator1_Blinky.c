//-----------------------------------------------------------------------------
// F91x_Comparator1_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This software shows the necessary configuration to use Comparator1 to detect
// when a switch is pressed and change the rate at which the LED is blinking.
// The code executes the initialization routines and then spins in an infinite
// while() loop, blinking the LED.  If the button on P0.2 (on the target board)
// is pressed, then the comparators postive input (CP1+) input will drop
// below the comparator's negative input (CP1-).  When this happens,
// Comparator1 will cause an interrupt to occur that will change the blinking
// rate of the LED.  For this example, CP1+ is connected to the switch and
// CP1- is connected to VDD/DC+ divided by 2.
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J8:   P1.5/LED1.5
//             P0.2/SW0.2
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J4.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F91x device on a C8051F91x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Verify the LED is blinking.
// 7) Press the P0.2 switch.
// 8) Verify that the LED is blinking at a different rate.
//
//
// Target:         C8051F90x-C8051F91x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 13 JAN 2014
//
// Release 1.0
//    -Initial Revision (TP)
//    -1 FEB 2012
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK      24500000           // SYSCLK frequency in Hz
#define COMPARATOR_SETTLE    10        // Settle time in us

#define LED_ON              0          // Macros to turn LED on and off
#define LED_OFF             1

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

SI_SBIT(RED_LED,    SFR_P1, 5);          // '0' means ON, '1' means OFF
SI_SBIT(SW2,        SFR_P0, 2);          // SW2 == 0 means switch pressed

uint16_t LEDBlinkRate = 1000;      // Blink time in milliseconds

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);           // Configure the system clock
void Port_Init (void);                 // Configure the Crossbar and GPIO
void Comparator1_Init (void);          // Configure Comparator1

void Timer0_Delay_us (uint16_t us);
void Timer0_Delay_ms (uint16_t ms);

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
   PCA0MD &= ~0x40;                    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{

   Oscillator_Init();                  // Initialize the system clock
   Port_Init ();                       // Initialize crossbar and GPIO
   Comparator1_Init();                 // Initialize Comparator1

   RED_LED = 0;

   IE_EA = 1;                             // Enable global interrupts

   SFRPAGE = LEGACY_PAGE;

   while(1)                            // Loop forever
   {
      RED_LED = !RED_LED;

      Timer0_Delay_ms (LEDBlinkRate);
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal precision
// oscillator at its maximum frequency and enables the missing clock
// detector.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   OSCICN |= 0x80;                     // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// Pinout:
//
// P0.2   analog    push-pull     Comparator1 + Input / Switch
//
// P1.5   digital   push-pull     LED
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P0SKIP = 0x04;                      // Skip P0.2 (CP1+)

   P0MDIN &= ~0x04;                    // Comparator1 input (P0.2)
                                       // is an analog inputs.
   P0     |= 0x04;                     // Set P0.2 latch to '1'

   P1MDOUT = 0x20;                     // LED is push-pull output

   // Configure Crossbar
   XBR2    = 0x40;                     // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Comparator1_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the comparator to pins P2.0 and P2.1 and enables
// the Comparator1 interrupt.
//-----------------------------------------------------------------------------
void Comparator1_Init (void)
{
   CPT1CN = 0x0F;                      // Positive and Negative Hysterisis =
                                       // 20 mV
   CPT1CN |= 0x80;                     // Comparator enabled
   CPT1MX = 0xD1;                      // VDD/DC+ divided by 2 = Negative Input
                                       // P0.2 = Positive Input

   Timer0_Delay_us (COMPARATOR_SETTLE); // Allow CP1 output to settle for 10 us

   CPT1MD = 0x10;                      // Enable Comparator1 falling-edge
                                       // interrupt
   EIE1 |= 0x40;                       // Enable the Comparator1 interrupt
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Comparator1 Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// This ISR will trigger on a Comparator1 falling edge, or whenever the switch
// on P0.2 is pressed.
//
SI_INTERRUPT(Comparator1_ISR, 13)
{
   EIE1 &= ~0x40;                      // Disable the Comparator1 interrupt
                                       // to effectively "debounce" the switch

   if ((CPT1CN & 0x40) == 0x00)        // If the output of CP1 is 0
   {                                   // (CP1+ < CP1-)
      // Vary the speed at which the LED blinks every time the switch is
      // pressed
      if (LEDBlinkRate == 1000)
      {
         LEDBlinkRate = 100;           // 100 ms = 10 Hz
      }
      else
      {
         LEDBlinkRate = 1000;          // 1000 ms = 1 Hz
      }
   }

   CPT1CN &= ~0x10;                    // Clear the falling-edge interrupt
                                       // flag

   EIE1 |= 0x40;                       // Re-enable the Comparator1 interrupt
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_Delay_us
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint16_t us - number of microseconds of delay
//                        range is full range of integer: 0 to 65335
//
// Configure Timer0 to delay <us> microseconds before returning.
//-----------------------------------------------------------------------------
void Timer0_Delay_us (uint16_t us)
{
   unsigned i;                         // Microsecond counter

   TCON  &= ~0x30;                     // STOP Timer0 and clear overflow flag
   TMOD  &= ~0x0f;                     // Configure Timer0 to 16-bit mode
   TMOD  |=  0x01;
   CKCON |=  0x04;                     // Timer0 counts SYSCLKs

   for (i = 0; i < us; i++)            // Count microseconds
   {
      TCON &= ~0x10;                   // STOP Timer0
      TH0 = (-SYSCLK/1000000) >> 8;    // Set Timer0 to overflow in 1us
      TL0 = -SYSCLK/1000000;
      TCON |= 0x10;                    // START Timer0
      while (TCON_TF0 == 0);                // Wait for overflow
      TCON &= ~0x20;                   // Clear overflow indicator
   }
}

//-----------------------------------------------------------------------------
// Timer0_Delay_ms
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint16_t ms - number of milliseconds of delay
//                        range is full range of integer: 0 to 65335
//
// Configure Timer0 to delay <ms> milliseconds before returning.
//
// Note: This routine modifies the SCA0-1 bits in CKCON, which will affect
// the timing of Timer1.
//-----------------------------------------------------------------------------
void Timer0_Delay_ms (uint16_t ms)
{
   unsigned i;                         // Millisecond counter

   TCON  &= ~0x30;                     // STOP Timer0 and clear overflow flag
   TMOD  &= ~0x0F;                     // Configure Timer0 to 16-bit mode
   TMOD  |=  0x01;

   CKCON &=  ~0x04;                    // Timer0 counts SYSCLK / 48
   CKCON |= 0x02;

   for (i = 0; i < ms; i++)            // Count milliseconds
   {
      TCON &= ~0x10;                   // STOP Timer0
      TH0 = (-SYSCLK/48/1000) >> 8;    // Set Timer0 to overflow in 1ms
      TL0 = -SYSCLK/48/1000;
      TCON |= 0x10;                    // START Timer0
      while (TCON_TF0 == 0);                // Wait for overflow
      TCON &= ~0x20;                   // Clear overflow indicator
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
