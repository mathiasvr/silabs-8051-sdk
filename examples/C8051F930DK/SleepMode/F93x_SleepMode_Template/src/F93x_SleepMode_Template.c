//-----------------------------------------------------------------------------
// F93x_SleepMode_Template.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program is a Template for 'F93x firmware that uses Sleep or Suspend Mode
//
// Target:         C8051F93x-C8051F90x
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.2
//    - Updated to fix self oscillate mode (FB)
//    - 03 JUN 2011
//
// Release 1.1
//    - Updated to be compatible with Raisonance Toolchain (FB)
//    - 14 APR 2010
//
// Release 1.0
//    -23 JUL 2009
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

SI_SBIT(TRIGGER,     SFR_P1, 5);         // Generates a 1ms trigger pulse
SI_SBIT(DEBUG_PIN,   SFR_P1, 6);         // Used to show analog acquisition time

SI_SBIT(POT_ENABLE,  SFR_P1, 4);         // '0' means potentiometer enabled
SI_SBIT(SW2,         SFR_P0, 2);         // SW2 == 0 means switch pressed
SI_SBIT(SW3,         SFR_P0, 3);         // SW3 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

// Constants for the analog measurement
#define VREF      (1650L * 2L)         // Internal 1.65V Reference with a
                                       // 2x multiplier due to the 0.5 gain
                                       // at the front end of the ADC

#define THRESHOLD ((2500L*1024L)/VREF) // 2.5 Volt Threshold converted to
                                       // a 10-bit ADC code used to determine
                                       // if the analog voltage is greater
                                       // or less than 2.5V

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

	TRIGGER = 0;                        // Initilize the state of the signals
	DEBUG_PIN = 0;

	#if(RTC_CLKSRC == CRYSTAL)
		enter_CrystalMode_from_DefaultMode();
	#elif(RTC_CLKSRC == SELFOSC)
		enter_SelfOscMode_from_DefaultMode();
	#else
		#error "Must select crystal or self oscillate mode"
	#endif

	RTC0CN_Local = 0xC0;                // Initialize Local Copy of RTC0CN
	RTC_WriteAlarm(WAKE_INTERVAL_TICKS);// Set the Alarm Value
	RTC0CN_SetBits(RTC0TR+RTC0AEN+ALRM);// Enable Counter, Alarm, and Auto-Reset

	LPM_Init();                         // Initialize Power Management
	LPM_Enable_Wakeup(RTC);

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

		 //-----------------------------------------------------------------------
		 // Generate Oscilloscope Trigger Pulse
		 //-----------------------------------------------------------------------
		 //
		 // This code generates a 1 ms pulse using the SmaRTClock. The RTC alarm
		 // is temporarily "borrowed" for 1ms and restored at the end of the
		 // 1ms interval.

		 TRIGGER = 1;                    // Set TRIGGER indicator HIGH

		 RTC_SleepTicks(ONE_MS_TICKS);   // Sleep for 1 ms

		 TRIGGER = 0;                    // Set TRIGGER indicator low

		 //-----------------------------------------------------------------------
		 // Measure Analog Value
		 //-----------------------------------------------------------------------
		 //
		 // Take ADC conversion.
		 //
		 // Enable the Potentiometer
		 POT_ENABLE = 0;                     // Enable the Potentiometer (active-low)

		 DEBUG_PIN = 1;                      // Turn on the debug signal

		 // Initiate a Conversion
		 ADC0CN_ADINT = 0;                         // clear ADC0 conv. complete flag
		 ADC0CN_ADWINT = 0;                        // clear window detector flag
		 ADC0CN_ADBUSY = 1;                        // initiate conversion

		 // Wait for conversion to complete
		 while(!ADC0CN_ADINT);

		 DEBUG_PIN = 0;                      // Turn off the debug pin

		 POT_ENABLE = 1;                     // Disable the Potentiometer (active-low)

		 // Check if window detector has detected a voltage less than 2.5 V, then
		 // the ADC0CN_ADWINT flag will be logic 1. If the voltage is higher than 2.5V,
		 // then the ADC0CN_ADWINT flag will be a logic 0.
		 if(ADC0CN_ADWINT == 0)
		 {
			// Do something if the potentiometer voltage is > 2.5V
		 }

	  }

	  //-----------------------------------------------------------------------
	  // Task #3 - Sleep for the remainder of the interval
	  //-----------------------------------------------------------------------

	  // Place the device in Sleep Mode
	  LPM(SLEEP);                      // Enter Sleep Until Next Alarm

	}

	// NOTREACHED
	return 0;
}
