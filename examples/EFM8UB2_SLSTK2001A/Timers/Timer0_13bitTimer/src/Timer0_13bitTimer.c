//-----------------------------------------------------------------------------
// Timer0_13bitTimer.c
//-----------------------------------------------------------------------------
//
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program presents an example of use of the Timer0 of the EFM8UB2's
// in 13-bit counter/timer mode. It uses the EFM8UB2DK as HW platform and blinks
// a LED. To do so it used an interrupt driven by the Timer0 which toggles the
// LED every 100msec.
//
// Pinout:
//
//    P1.6 -> LED
//
//    all other port pins unused
//
// How To Test:
//
// This code enables the Timer0 as a 13-bit timer that uses the system clock
// (SYSCLK, in this case the internal clock @12MHz/8) with a 1:48 prescaler
// to generate an interrupt every 100msec. (+/- 2%)
// The interrupt routine then toggles the LED. To execute the test please
// follow these steps:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8UB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8UB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) The LED should blink at <LED_TOGGLE_RATE>.
//
//
// Target:         EFM8UB2
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"

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
  // Disable the watchdog here
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   enter_DefaultMode_from_RESET();

   while (1);                          // Loop forever
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
