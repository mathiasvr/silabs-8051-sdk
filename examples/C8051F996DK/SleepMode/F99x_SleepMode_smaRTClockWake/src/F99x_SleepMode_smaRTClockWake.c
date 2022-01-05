//-----------------------------------------------------------------------------
// F99x_SleepMode_smaRTClockWake.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program flashes the red LED on the C8051F996 target board about
// five times a second using the interrupt handler for Timer2 when the
// MCU is awake.
//
// The smaRTClock is configured to generate a wake-up event every 2 seconds.
// The device will toggle between Sleep/Suspend mode and Normal mode. In
// the low power state, the Yellow LED will be turned on.
//
// If a smaRTClock oscillator failure occurs, both LEDs will remain on
// and the device will be in a high power state.
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J8:   P1.5/LED1.5
//             P1.3/LED1.3
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J4.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F99x device on a C8051F99x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Run the program. The Red LED will blink when the device is awake, the
//    Yellow LED will be ON when the device is asleep.
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
#include "C8051F990_lib.h"             // library declarations

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(RED_LED,     SFR_P1, 5);         // Generates a 1ms trigger pulse
SI_SBIT(YELLOW_LED,  SFR_P1, 3);         // Used to show analog acquisition time
SI_SBIT(SW2,        SFR_P0, 2);          // SW2 == 0 means switch pressed
SI_SBIT(SW3,        SFR_P0, 3);          // SW3 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
bit Blink_LED = 0;

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

	YELLOW_LED = LED_OFF;               // Initilize the state of the signals
	RED_LED = LED_OFF;
	RTC0CN_Local = 0xC0;                // Initialize Local Copy of RTC0CN

	RTC0CN_SetBits(RTC0TR+RTC0AEN+ALRM);// Enable Counter, Alarm, and Auto-Reset

	LPM_Init();                         // Initialize Power Management
	LPM_Enable_Wakeup(RTC);

	RTC_Alarm = 1;                      // Set the RTC Alarm Flag on startup

	//----------------------------------
	// Main Application Loop
	//----------------------------------
	while (1)
	{

	  //-----------------------------------------------------------------------
	  // Task #1 - Handle RTC Failure
	  //-----------------------------------------------------------------------
	  if(RTC_Failure)
	  {
		 RTC_Failure = 0;              // Reset RTC Failure Flag to indicate
									   // that we have detected an RTC failure
									   // and are handling the event

		 // Do something...RTC Has Stopped Oscillating
		 while(1);                     // <Insert Handler Code Here>
	  }

	  //-----------------------------------------------------------------------
	  // Task #2 - Handle RTC Alarm
	  //-----------------------------------------------------------------------
	  if(RTC_Alarm)
	  {

		 RTC_Alarm = 0;                // Reset RTC Alarm Flag to indicate
									   // that we have detected an alarm
									   // and are handling the alarm event

		 Blink_LED = !Blink_LED;       // Change blinking state

		 if(Blink_LED)
		 {
			// Adjust LED state for active mode
			YELLOW_LED = LED_OFF;
			RED_LED = LED_ON;

		 }
		 else
		 {
			// Adjust LED state for sleep mode
			RED_LED = LED_OFF;
			YELLOW_LED = LED_ON;
		 }

	  }

	  //-----------------------------------------------------------------------
	  // Task #3 - Sleep for the remainder of the interval
	  //-----------------------------------------------------------------------
	  // If blinking, stay awake, otherwise sleep

	  if(Blink_LED)
	  {
		 // Wait for next alarm or clock failure, then clear flags
		 while((PMU0CF & RTCAWK) == 0);
		 if(PMU0CF & RTCAWK) RTC_Alarm = 1;
		 if(PMU0CF & RTCFWK) RTC_Failure = 1;
		 PMU0CF = 0x20;

	  }
	  else
	  {
		 // Place the device in Sleep Mode
		 P0 &= ~0x01;
		 LPM(SLEEP);                      // Enter Sleep Until Next Alarm
		 P0 |= 0x01;

	  }

	}

	// NOTREACHED
	return 0;
}
