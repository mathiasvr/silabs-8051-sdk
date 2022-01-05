//-----------------------------------------------------------------------------
// F93x_PCA0_Frequency_Output.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program sends a square wave out of an I/O pin, using the PCA's
// Frequency Output Mode.
//
// In this example, PCA Module 0 is used to generate the waveform, and the
// crossbar is configured to send the CEX0 pin out on P0.0.
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J9.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Modify PCA0 clock divider in the PCA module of Configurator to change the
//    frequency.
// 5) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Measure the frequency output on P0.0. The default frequency is 51 kHz.
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
//    - Change SAR clock formula (JH)
//    - 07 JULY 2009
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

	while (1) {}                             // Spin forever

	// NOTREACHED
	return 0;
}
