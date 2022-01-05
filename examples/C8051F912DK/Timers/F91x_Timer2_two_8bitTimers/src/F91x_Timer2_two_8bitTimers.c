//-----------------------------------------------------------------------------
// F91x_Timer2_two_8bitTimers.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program presents an example of use of the Timer2 of the C8051F91x's in
// two 8-bit reload counter/timer mode. It uses the 'F912DK as HW platform.
//
// It sets Timer2 to be two 8-bit timers that interrupt every millisecond.
//
// When the Timer2 low interrupts a counter (low_counter) increments and upon
// reaching the value defined by LED_TOGGLE_RATE toggles the LED
// When the Timer2 high interrupts a counter (high_counter) increments and upon
// reaching the value defined by SIGNAL_TOGGLE_RATE toggles the SIGNAL pin
//
// Pinout:
//
//    P1.6 -> Yellow LED (toggled by TMR2L)
//    P1.5 -> Red LED    (toggled by TMR2H)
//
//    all other port pins unused
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J8:   P1.6/LED1.6
//             P1.5/LED1.5
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J4.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) If a different LED blink rates are required, change the following:
//       -> YELLOW_TOGGLE_RATE
//       -> RED_TOGGLE_RATE
// 5) Compile and download code to a 'F91x device on a C8051F91x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) The yellow LED will blink at YELLOW_TOGGLE_RATE, the red LED will blink
//    at RED_TOGGLE_RATE.
//
//
// Target:         C8051F90x-C8051F91x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 14 JAN 2014
//
// Release 1.2 
//    - Compiled and tested for Raisonance Toolchain (FB)
//    - 17 MAY 10
//
// Release 1.1
//    - Port to C8051F912 from C8051F930 (JH)
//    - Compile and test C8051F912_defs.h on 912 board (JH)
//    - 06 JULY 2009
//
// Release 1.0
//    -Initial Revision (FB)
//    -20 NOV 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             (20000000L/8L) // SYSCLK in Hz (20 MHz internal
                                          // low power oscillator / 8)
                                          // the low power oscillator has a
                                          // tolerance of +/- 10%
                                          // the precision oscillator has a 
                                          // tolerance of +/- 2%

#define TIMER_PRESCALER            12L    // Based on Timer2 CKCON and TMR2CN
                                          // settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  (SYSCLK/TIMER_PRESCALER/1000L)

// Note: TIMER_TICKS_PER_MS should not exceed 255 (0xFF) for the 8-bit timers

#define AUX1     TIMER_TICKS_PER_MS
#define AUX2     -AUX1

#define YELLOW_TOGGLE_RATE         100    // Yellow LED toggle rate in milliseconds
                                          // if YELLOW_TOGGLE_RATE = 1, the yellow LED will
                                          // be on for 1 millisecond and off for
                                          // 1 millisecond

#define RED_TOGGLE_RATE            30     // Red LED toggle rate in milliseconds
                                          // if RED_TOGGLE_RATE = 1, the
                                          // red LED will be on for 1 millisecond
                                          // and off for 1 millisecond

#define TIMER2_RELOAD_HIGH       AUX2     // Reload value for Timer2 high byte
#define TIMER2_RELOAD_LOW        AUX2     // Reload value for Timer2 low byte

SI_SBIT(YELLOW_LED, SFR_P1, 6);             // YELLOW_LED == 0 means ON
SI_SBIT(RED_LED, SFR_P1, 5);                // Red LED to be used also by Timer2

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Port_Init (void);                    // Port initialization routine
void Timer2_Init (void);                  // Timer2 initialization routine
SI_INTERRUPT_PROTO(TIMER2_ISR, TIMER2_IRQn);

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
   PCA0MD &= ~0x40;                       // Clear watchdog timer enable
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   Timer2_Init ();                        // Initialize the Timer2
   Port_Init ();                          // Init Ports

   IE_EA = 1;                                // Enable global interrupts

   while (1);                             // Loop forever
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// P1.6   digital   push-pull   Yellow LED
// P1.5   digital   push-pull   Red LED
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   XBR2 = 0x40;                           // Enable crossbar
   P1MDOUT = 0x60;                        // Set LEDs to be used as push-pull
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures Timer2 as two 8-bit reload timers with interrupt.
// Using the internal osc. at 24.5MHz/8 with a timer prescaler of 1:12
// reloading the TMR2H register with TIMER2_RELOAD_HIGH and TMR2L with
// TIMER2_RELOAD_HIGH.
//
// Note: The Timer2 uses a 1:12 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void Timer2_Init(void)
{
   CKCON &= ~0x30;                        // Timer2 uses SYSCLK/12
   TMR2CN &= ~0x01;

   TMR2RLH = (uint8_t) TIMER2_RELOAD_HIGH;     // Init Timer2 Reload High register
   TMR2RLL = (uint8_t) TIMER2_RELOAD_LOW;      // Init Timer2 Reload Low register

   TMR2H = (uint8_t) TIMER2_RELOAD_HIGH;       // Init Timer2 High register
   TMR2L = (uint8_t) TIMER2_RELOAD_LOW;        // Init Timer2 Low register

   TMR2CN = 0x2C;                         // Low byte interrupt enabled
                                          // Timer2 enabled
                                          // and Split-Mode enabled
   IE_ET2 = 1;                               // Timer2 interrupt enabled
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This function processes the Timer2 interrupt. It checks both counters for
// matches and, upon a match, toggles the LEDs.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
   static uint16_t low_counter=0;     // Define counter variables
   static uint16_t high_counter=0;

   if(TMR2CN_TF2L == 1)
   {
      if((low_counter++) == YELLOW_TOGGLE_RATE)
      {
         low_counter = 0;                 // Reset interrupt counter
         YELLOW_LED = !YELLOW_LED;        // Toggle the yellow LED
      }
      TMR2CN_TF2L = 0;
   }
   if(TMR2CN_TF2H == 1)
   {
      if((high_counter++) == RED_TOGGLE_RATE)
      {
         high_counter = 0;                // Reset interrupt counter
         RED_LED = !RED_LED;              // Toggle the Red LED
      }
      TMR2CN_TF2H = 0;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
