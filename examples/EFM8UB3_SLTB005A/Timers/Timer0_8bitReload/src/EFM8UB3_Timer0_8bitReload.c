//-----------------------------------------------------------------------------
// EFM8UB3_Timer0_8bitReload.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program uses Timer0 in 8-bit counter/timer with reload mode.
// It uses the Timer0 to create an interrupt at a certain rate and toggles
// the LED when the interrupt counter reaches the selected value.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 8
//   Timer0 - 10 Hz interrupt
//   P1.4   - LED green
//
//-----------------------------------------------------------------------------
// How To Test: EFM8UB3 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8UB3 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8UB3 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Check that the green LED is blinking.
//
// Target:         EFM8UB3
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 10 OCT 2014
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8UB3_Register_Enums.h>
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
// Main Routine
//-----------------------------------------------------------------------------
void main (void)
{
   enter_DefaultMode_from_RESET();

   IE_EA = 1;                          // Enable global interrupts

   while (1) {}
}
