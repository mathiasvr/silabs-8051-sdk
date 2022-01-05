//-----------------------------------------------------------------------------
// F93x_SleepMode_smaRTClockWake.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program flashes the red LED on the C8051F930 target board about
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
//             P1.6/LED1.6
//             P0.2/SW0.2
//             P0.3/SW0.3
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J9.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F91x device on a C8051F91x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8
// 6) The device should alternate being awake and going to sleep every 2
//    seconds. The Red LED will blink when the device is awake, the yellow LED
//    will be ON when the device is sleeping.
//
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Silicon Labs Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.5 (BL)
//    - Updated Description / How to Test
//    - 15 JAN 2014
//
// Release 1.4
//    - Updated to be compatible with Raisonance Toolchain (FB)
//    - 14 APR 2010
//
// Release 1.3
//    -Fixed self-oscillating functionality (AS)
//    -23 Dec 2009
//
// Release 1.2
//    - Compiled and tested for C8051F930-TB (JH)
//    - 06 JULY 2009
//
// Release 1.1
//    - Updated SmaRTClock Initialization Procedure (FB)
//    - 15 AUG 2008
//
// Release 1.0
//    - Initial Revision (FB)
//    - 15 JAN 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F930_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include "C8051F930_lib.h"             // library declarations

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(RED_LED,     SFR_P1, 5);         // Generates a 1ms trigger pulse
SI_SBIT(YELLOW_LED,   SFR_P1, 6);         // Used to show analog acquisition time
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

	#if(RTC_CLKSRC == CRYSTAL)
		enter_RTC_Crystal_from_DefaultMode();
	#elif(RTC_CLKSRC == SELFOSC)
		enter_RTC_SelfOsc_from_DefaultMode();
	#else
		#error "Must select crystal or self oscillate mode"
	#endif

	RTC0CN_Local = 0xC0;                // Initialize Local Copy of RTC0CN
	RTC_WriteAlarm(WAKE_INTERVAL_TICKS);// Set the Alarm Value
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
		 PMU0CF = CLEAR;

	  }
	  else
	  {
		 // Place the device in Sleep Mode
		 LPM(SLEEP);                      // Enter Sleep Until Next Alarm
	  }

	}

	// NOTREACHED
	return 0;
}
