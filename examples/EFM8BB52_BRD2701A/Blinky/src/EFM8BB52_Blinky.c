//-----------------------------------------------------------------------------
// EFM8BB52_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program flashes the red/green/blue LED on the EFM8BB52 PK board about
// five times a second using the interrupt handler for Timer2.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 8
//   Timer2 - 5 Hz interrupt
//   P1.4   - LED0
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB52 Explorer Kit
//-----------------------------------------------------------------------------
// 1) Connect the EFM8BB52 Explorer Kit to a PC using a mini USB cable.
// 2) Compile and download code to the EFM8BB52 Explorer Kit board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 3) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 4) The LED0 should blink at approximately 5 Hz.
//
// Target:         EFM8BB52
// Tool chain:     Generic
//
// Release 0.1 (RN)
//    - Initial Revision
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB52_Register_Enums.h>
#include <InitDevice.h>

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

   while (1) {}                        // Spin forever
}
