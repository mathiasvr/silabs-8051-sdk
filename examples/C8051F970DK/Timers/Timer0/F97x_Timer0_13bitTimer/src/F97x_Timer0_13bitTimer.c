//-----------------------------------------------------------------------------
// F97x_Timer0_13bitTimer.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use Timer0 as a single 13-bit timer
//
// This program presents an example of use of the Timer0 of the C8051F97x's
// in 13-bit counter/timer mode. It uses the 'F970 TB as HW platform and blinks
// a LED. To do so it used an interrupt driven by the Timer0 which toggles the
// LED every 100msec.
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
// 5) LED05 will blink.
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

#define LED05_PERIOD_MS           100

#define TIMER_PRESCALER            48  // Based on Timer0 CKCON settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

 // Reload value for Timer0
#define TIMER0_RELOAD      -(TIMER_TICKS_PER_MS * LED05_PERIOD_MS)

#if (TIMER_TICKS_PER_MS * LED05_PERIOD_MS > 8191)
  #error "TIMER_TICKS_PER_MS * LED5_PERIOD_MS is over 8191"
#endif

#define TIMER0_RELOAD_LOW   (TIMER0_RELOAD & 0x00FF)  // Reload value for Timer0 low byte
#define TIMER0_RELOAD_HIGH  (TIMER0_RELOAD & 0x1F00) >> 8  // Reload value for Timer0 high byte


SI_SBIT(LED05, P0, 5);                     // LED='1' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(TIMER0_ISR, TIMER0_IRQn);

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
   TH0 = TIMER0_RELOAD_HIGH;           // Reinitialize Timer0 reload values
   TL0 = TIMER0_RELOAD_LOW;

   LED05 = !LED05;                     // Toggle LED5
 }

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
