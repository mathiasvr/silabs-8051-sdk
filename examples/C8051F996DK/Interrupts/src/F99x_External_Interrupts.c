//-----------------------------------------------------------------------------
// F99x_External_Interrupts.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This software shows the necessary configuration to use External Interrupt 0
// (/INT0) or External Interrupt 1 (/INT1) as an interrupt source.  The code
// executes the initialization routines and then spins in an infinite while()
// loop.  If the button on P0.2 (on the target board) is pressed, then the
// edge-triggered /INT0 input on P0.2 will cause an interrupt and toggle the
// yellow LED. If the button on P0.3 is pressed, then the edge-triggered
// /INT1 input on P0.3 will cause an interrupt and toggle the Red LED.
//
// Pinout:
//
// P0.2 - /INT0 (Switch 2)
// P0.3 - /INT1 (Swtich 3)
//
// P1.5 - Red LED
// P1.3 - Yellow LED
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J8:   P1.3/LED1.3
//             P1.5/LED1.5
//             P0.2/SW0.2
//             P0.3/SW0.3
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J4.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F99x device on a C8051F99x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Press the switches.  Every time a switch is pressed, one of
//    the LEDs should toggle.
//
// Target:         C8051F99x-C8051F98x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 13 JAN 2014
//
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F990_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);

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
