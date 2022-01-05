//-----------------------------------------------------------------------------
// EFM8BB1_ExternalInterrupts.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This software shows the necessary configuration to use External Interrupt 0
// (/INT0 as an interrupt source.  The code  executes the initialization
// routines and then spins in an infinite while() loop.
// If the button is pressed, then the edge-triggered /INT0 input on P0.2 will
// cause an interrupt and toggle the LED.
//
// Resources:
//   SYSCLK - 80 kHz LFOSC / 128
//   P0.2   - Push button switch, /INT0 input
//   P1.4   - LED
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
// 4) Press the switch.  Every time a switch is pressed, one of
//    the LEDs should toggle.
//
// Target:         EFM8BB1
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 10 OCT 2014
//

//-----------------------------------------------------------------------------
// Include Files
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

   while(1);                           // Infinite while loop waiting for
                                       // an interrupt from /INT0 or /INT1
}
