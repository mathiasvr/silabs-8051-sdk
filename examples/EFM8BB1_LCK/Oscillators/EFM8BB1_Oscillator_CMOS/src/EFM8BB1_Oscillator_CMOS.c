//-----------------------------------------------------------------------------
// EFM8BB1_Oscillator_CMOS.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the oscillator for use with an
// external CMOS clock.
//
//-----------------------------------------------------------------------------
// How To Test:
//-----------------------------------------------------------------------------
// 1) Ensure that a CMOS clock is connected to the EXTCLK pin (P0.3).
// 2) Place the power switch in the AEM position and power the board through
//    the DBG USB connector.
// 3) Compile and download code to the EFM8BB1 board by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
// 5) Press PB0 to switch to the external oscillator. The divided precision
//    oscillator (24.5 MHz / 8) will be output on P0.7 while PB0 is not pressed.
// 6) Measure the frequency output on P0.7.
//
//
// Target:         EFM8BB1
// Command Line:   None
//
// Release 1.2 (BL)
//		- Updated How To Test
//		- 2 JAN 2014
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 19 FEB 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_EFM8BB1_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

SI_SBIT(S1, SFR_P0, 2);    // PB0

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

   // Wait until S1 is pressed before switching to the external oscillator
   while (S1 == 1);

   enter_EXTCLCK_mode_from_DefaultMode(); //Initialize oscillator

   while (1);                           // Infinite Loop
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
