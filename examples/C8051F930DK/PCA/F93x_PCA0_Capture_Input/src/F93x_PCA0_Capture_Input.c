//-----------------------------------------------------------------------------
// F93x_PCA0_Capture_Input.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program uses the PCA's capture mode to time events on an input pin.
// The difference between subsequent captures is calculated and stored in
// memory.
//
// In this example, PCA Module 0 is used to capture rising edges of an
// incoming digital waveform, and the crossbar is configured to receive the
// CEX0 pin on on P0.0.  The PCA is configured to clock once every 10 us.
//
// Timer2 overflows are used to toggle pin P0.1 at an overflow rate of 4000.
// This signal at P0.1 is provided simply
// as a test method for the code.
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect P0.1 and P0.0 together.
// 3) Connect the USB Debug Adapter to J9.
// 4) Turn the POWER switch (SW5) to the "ON" position.
// 5) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Halt the program inside the PCA0_ISR function in Interrupts.c.
// 3) Verify that the variable "capture_period" is equal to 50 (=500us).
//
// Alternately:
// 1) Ensure that jumpers are placed on the following:
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect a signal generator to the P0.0 pin.
// 3) Connect the USB Debug Adapter to J9.
// 4) Turn the POWER switch (SW5) to the "ON" position.
// 5) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Halt the program inside the PCA0_ISR function in Interrupts.c.
// 8) Verify that the variable "capture_period" matches the expected time
//    between rising edges (in units of 10us).
//
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 15 JAN 2014
//
// Release 1.2
//    - Updated to be compatible with Raisonance Toolchain (FB)
//    - 14 APR 2010
//
// Release 1.1
//    - Compiled and tested for C8051F930-TB (JH)
//    - 06 JULY 2009
//
// Release 1.0
//    - Initial Revision (FB)
//    - 16 NOV 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F930_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

SI_SBIT(TEST_OUT, SFR_P0, 1);            // Output for testing purposes

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
// ----------------------------------------------------------------------------
int main (void)
{
	//Enter default mode
	enter_DefaultMode_from_RESET();

	while (1)
	{
	  // check to see if Timer2 overflowed
	  if (TMR2CN_TF2H)
	  {
		 TEST_OUT = !TEST_OUT;         // Toggle test pin
		 TMR2CN_TF2H = 0;                     // Clear T2 overflow
	  }

	};

	// NOTREACHED
	return 0;
}
