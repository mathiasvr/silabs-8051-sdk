//-----------------------------------------------------------------------------
// EFM8BB1_Timer2_two8bit.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This uses Timer2 in two 8-bit reload counter/timer mode.
//
// When the Timer2 low interrupts, a counter (low_counter) increments and
// toggles LED after reaching the value defined by LED_TOGGLE_RATE.
//
// When the Timer2 high interrupts, a counter (high_counter) increments and
// toggles P1_5 after reaching the value defined by P1_5_TOGGLE_RATE.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC / 8
//   Timer2 - 1 kHz interrupt
//   P1.4   - LED / Timer2 low interrupt output
//   P1.5   - Timer2 high interrupt output
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 LCK
//-----------------------------------------------------------------------------
// 1) Connect the EFM8BB1 LCK board to a PC using a micro USB cable.
// 2) Compile and download code to the EFM8BB1 LCK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 3) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 4) Check that the green and blue LEDs are blinking.
//
// Target:         EFM8BB1
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 10 OCT 2014
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB1_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

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

   while (1);                          // Loop forever
}
