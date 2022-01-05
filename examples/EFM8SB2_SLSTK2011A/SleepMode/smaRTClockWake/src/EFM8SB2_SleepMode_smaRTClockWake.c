//-----------------------------------------------------------------------------
// SB1_SleepMode_smaRTClockWake.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program flashes LED0 on the EFM8SB2 target board about
// five times a second using the interrupt handler for Timer2 when the
// MCU is awake.
//
// The smaRTClock is configured to generate a wake-up event every 2 seconds.
// The device will toggle between Sleep/Suspend mode and Normal mode. In
// the low power state, LED0 will be off.
//
// If a smaRTClock oscillator failure occurs, LED0 will remain on
// and the device will be in a high power state.
//
// How To Test:
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8SB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8SB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) LED0 (P2.0) will be: blinking when the device is awake.
//                         OFF when the device is sleeping.
//                         ON constantly if an RTC failure occurs
//
//    LED1 (P2.1) will be: LOW when the device is awake.
//                         HIGH when the device is sleeping.
//
//
// Target:         EFM8SB2
// Tool chain:     Silicon Labs Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 16 JAN 2015
//
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <si_toolchain.h>
#include <SI_EFM8SB2_Register_Enums.h>                  // SI_SFR declarations
#include "InitDevice.h"
#include "power.h"
#include "SmaRTClock.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 6);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (LED1, SFR_P2, 1);      // 1 = Sleeping, 0 = Awake
SI_SBIT (LED0,  SFR_P2, 0);         // Used to show analog acquisition time
SI_SBIT (PB0,   SFR_P0, 2);          // PB0 == 0 means switch pressed
SI_SBIT (PB1,   SFR_P0, 3);          // PB1 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
bool Blink_LED = 0;

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define LED_ON           0
#define LED_OFF          1

//-----------------------------------------------------------------------------
// Global VARIABLES
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
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{


	while(!PB0);                        // Debug Trap -- Prevents the system
									   // from entering sleep mode after
									   // a reset if Switch 2 is pressed.
	//Enter default mode
	enter_DefaultMode_from_RESET();

	DISP_EN = DISP_BC_DRIVEN;

	LED0 = LED_OFF;               // Initilize the state of the signals

	RTC0CN0_Local = 0xC0;                // Initialize Local Copy of RTC0CN0

	RTC0CN0_SetBits(RTC0TR+RTC0AEN+ALRM);// Enable Counter, Alarm, and Auto-Reset

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

		 LED0 = LED_ON;
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
	   LED0 = LED_OFF;
	   LED1 = LED_ON;
		 LPM(SLEEP);                      // Enter Sleep Until Next Alarm
		 LED1 = LED_OFF;

	  }

	}
}
