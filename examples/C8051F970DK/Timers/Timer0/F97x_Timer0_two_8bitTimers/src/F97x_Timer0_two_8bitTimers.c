//-----------------------------------------------------------------------------
// F97x_Timer0_two_8bitTimers.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use Timer0 as two 8-bit timers
//
// This program presents an example of use of the Timer0 of the C8051F97x in
// two 8-bit reload counter/timer mode. It uses the 'F970 TB as HW platform.
//
// It sets the two timers to interrupt every millisecond.
//
// When the timer low interrupts, a counter (low_counter) increments and upon
// reaching the value defined by LED15_TOGGLE_RATE toggles LED15
// When the timer high interrupts a counter (high_counter) increments and upon
// reaching the value defined by LED05_PERIOD_MS toggles LED05
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) LED05 and LED15 will blink.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 2 MAY 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK             20000000/8  // SYSCLK frequency in Hz

#define TIMER_PRESCALER            48  // Based on Timer0 CKCON settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

#if (TIMER_TICKS_PER_MS > 255)
  #error "TIMER_TICKS_PER_MS is over 255"
#endif

#define TIMER0_RELOAD      -TIMER_TICKS_PER_MS // Reload value for Timer2

#define TIMER0_RELOAD_LOW   TIMER0_RELOAD  // Reload value for Timer0 low byte
#define TIMER0_RELOAD_HIGH  TIMER0_RELOAD  // Reload value for Timer0 high byte

#define LED15_PERIOD_MS            200
#define LED05_PERIOD_MS            500


SI_SBIT(LED15, P1, 5);                     // LED='0' means ON
SI_SBIT(LED05, P0, 5);                     // LED='0' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(TIMER0_ISR, TIMER0_IRQn);
SI_INTERRUPT_PROTO(TIMER1_ISR, TIMER1_IRQn);

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
  SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
  PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   enter_DefaultMode_from_RESET();
   while (1);
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer0 interrupt and toggle the LED
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER0_ISR, TIMER0_IRQn)
{
   static unsigned int low_ctr = 0;  // Define counter variables

   TL0 = TIMER0_RELOAD_LOW;          // Reinitialize Timer0 Reload value

   if((low_ctr++) == LED15_PERIOD_MS)
   {
      low_ctr = 0;                   // Reset interrupt counter

      LED15 = !LED15;                // Toggle LED15
   }
}

//-----------------------------------------------------------------------------
// Timer1_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer0 interrupt and toggle the LED
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER1_ISR, TIMER1_IRQn)
{
   static unsigned int high_ctr = 0; // Define counter variables

   TH0 = TIMER0_RELOAD_HIGH;         // Reinitialize Timer1 Reload value

   if((high_ctr++) == LED05_PERIOD_MS)
   {
      high_ctr = 0;                 // Reset interrupt counter

      LED05 = !LED05;               // Toggle LED05
   }
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
