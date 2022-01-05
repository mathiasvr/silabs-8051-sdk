///-----------------------------------------------------------------------------
// F93x_SPI0_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program configures a C8051F336 as a SPI Slave. The 'F336 MCU
// is configured in 4-wire Slave Mode.
//
// This example is intended to be used with the SPI0_Master example.
//
// Pinout:
//
// P0.0 - SPI SCK    (digital input, open-drain)
// P0.1 - SPI MISO   (digital output, push-pull)
// P0.2 - SPI MOSI   (digital input, open-drain)
// P0.3 - SPI NSS    (digital input, open-drain)
//
// P1.6 - LED        (digital output, push-pull)
//
// all other port pins unused.
//
//
// How To Test:
//
// 1) Verify that J13 and J14 are not populated.
// 2) Connect the device to another 'F91x device running SPI Master code.
// 3) Ensure that jumpers are placed on the following:
//       J8:   P1.6/LED1.6
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 4) Connect the USB Debug Adapter to J9.
// 5) Turn the POWER switch (SW5) to the "ON" position.
// 6) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) If the communication passes, the LEDs on both the Master and Slave
//    boards will blink slowly. If it fails, the LEDs will be OFF.
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
//    - 07 JULY 2009
//
// Release 1.0
//    -Initial Revision (FB)
//    -19 NOV 2007
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
