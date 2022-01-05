//-----------------------------------------------------------------------------
// F99x_Ports_SwitchLED.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure port pins as digital inputs
// and outputs.  The C8051F99x target board has two push-button switches
// and two LEDs connected to port pins.  The program constantly checks the
// status of the switches and if one or both are closed, it turns off
// one or both LEDs, respectively.
//
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J8:   P1.5/LED1.5
//             P1.3/LED1.3
//             P0.2/SW0.2
//             P0.3/SW0.3
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Ensure C21 and C20 (used for external oscillators) are not populated.
// 3) Connect the USB Debug Adapter to J4.
// 4) Turn the POWER switch (SW5) to the "ON" position.
// 5) Compile and download code to a 'F99x device on a C8051F99x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Push the button (P0.2 or P0.3) and see that the corresponding
//    LED (Red or Yellow) turns off.
//
//
// Target:         C8051F99x-C8051F98x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 13 JAN 2014
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
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(RED_LED,    SFR_P1, 5);          // '0' means ON, '1' means OFF
SI_SBIT(YELLOW_LED, SFR_P1, 3);          // '0' means ON, '1' means OFF
SI_SBIT(SW2,        SFR_P0, 2);          // SW2 == 0 means switch pressed
SI_SBIT(SW3,        SFR_P0, 3);          // SW3 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define LED_ON  0
#define LED_OFF 1

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

	 while (1)
	   {


		   // Set the state of the red LED
		   if (SW2 == 0)                    // If switch pressed
		   {
			  RED_LED = LED_OFF;            // Turn off Red LED
		   }
		   else
		   {
			  RED_LED = LED_ON;            // Else, turn it on
		   }

		   // Set the state of the yellow LED
		   if (SW3 == 0)                    // If switch pressed
		   {
			  YELLOW_LED = LED_OFF;         // Turn off Yellow LED
		   }
		   else
		   {
			  YELLOW_LED = LED_ON;          // Else, turn it on
		   }
	   }                                  // end of while(1)

	// NOTREACHED
	return 0;
}
