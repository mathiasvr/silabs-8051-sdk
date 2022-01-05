//-----------------------------------------------------------------------------
// F99x_SleepMode_PortMatchWake.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program flashes the red LED on the C8051F996 target board using the
// interrupt handler for Timer2 when the MCU is awake.
//
// Pressing the switch on P0.3 will place the device in a low power mode.
// Pressing the switch on P0.2 will wake up the device using Port Match.
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
// 2) Connect the USB Debug Adapter to J4.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F99x device on a C8051F99x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Press P0.3 to place the device in sleep mode.
//    Press P0.2 to wake it back up. The Red LED will blink when
//    the device is awake.
//
//
// Target:         C8051F99x-C8051F98x
// Tool chain:     Silicon Labs Studio / Keil C51 9.51
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
#include "C8051F990_lib.h"

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

#define LED_ON           0
#define LED_OFF          1

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

	while(!SW2);                        // Debug Trap -- Prevents the system
									   // from entering sleep mode after
									   // a reset if Switch 2 is pressed.

	//Enter default mode
	enter_DefaultMode_from_RESET();

	RED_LED = LED_OFF;                  // Initilize the state of the signals
	YELLOW_LED = LED_OFF;

	LPM_Init();                         // Initialize Power Management
	LPM_Enable_Wakeup(PORT_MATCH);      // Enable Port Match wake-up source

	//----------------------------------
	// Main Application Loop
	//----------------------------------
	while (1)
	{

	  //-----------------------------------------------------------------------
	  // Task #1 - Handle Port Match Event
	  //-----------------------------------------------------------------------
	  if(Port_Match_Wakeup)
	  {

		 Port_Match_Wakeup = 0;        // Reset Port Match Flag to indicate
									   // that we have detected an event

		 // Adjust LED state for active mode
		 YELLOW_LED = LED_OFF;
		 RED_LED = LED_ON;

	  }

	  //-----------------------------------------------------------------------
	  // Task #2 - Enter sleep mode if P0.3 switch is pressed
	  //-----------------------------------------------------------------------

	  if(!SW3)                         // If the P0.3 switch is pressed
	  {

		 // Configure the Port I/O for Low Power Mode
		 RED_LED = LED_OFF;            // Turn off the RED_LED

		 YELLOW_LED = LED_ON;          // Turn on Yellow LED to indicate that
									   // the device is entering the low power
									   // state
		 LPM (SLEEP);

	  }
	}

	// NOTREACHED
	return 0;
}
