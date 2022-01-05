//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program is a Template for firmware that uses Sleep or Suspend Mode
//
// Target:         EFM8SB1
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 9 JAN 2015
//
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <si_toolchain.h>
#include <SI_EFM8SB1_Register_Enums.h>                  // SI_SFR declarations
#include "InitDevice.h"
#include "smartclock.h"                // RTC Functionality
#include "power.h"                     // Power Management Functionality

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 7);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display


SI_SBIT (TRIGGER,     SFR_P0, 1);         // Generates a 1ms trigger pulse
SI_SBIT (DEBUG_PIN,   SFR_P1, 0);         // Used to show analog acquisition time
SI_SBIT (LED0,        SFR_P1, 1);         // LED == 1 means ON
SI_SBIT (PB0,         SFR_P1, 2);         // PB0 == 0 means switch pressed
SI_SBIT (PB1,         SFR_P1, 3);         // PB1 == 0 means switch pressed



//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define LED_ON  1
#define LED_OFF 0

// Constants for the analog measurement
#define VREF      (1650L * 2L)         // Internal 1.65V Reference with a
                                       // 2x multiplier due to the 0.5 gain
                                       // at the front end of the ADC

#define THRESHOLD ((2500L*1024L)/VREF) // 2.5 Volt Threshold converted to
                                       // a 10-bit ADC code used to determine
                                       // if the analog voltage is greater
                                       // or less than 2.5V

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT (reserved, uint8_t, SI_SEG_XDATA, 0x0000);

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
	RTC0CN0_Local = 0xC0;                // Initialize Local Copy of RTC0CN0

	TRIGGER = 0;                        // Initilize the state of the signals
	DEBUG_PIN = 0;

	RTC_WriteAlarm(WAKE_INTERVAL_TICKS);// Set the Alarm Value
	RTC0CN0_SetBits(RTC0TR+RTC0AEN+ALRM);// Enable Counter, Alarm, and Auto-Reset

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

		 DEBUG_PIN = 1;                      // Turn on the debug signal

		 // Initiate a Conversion
		 ADC0CN0_ADINT = 0;                         // clear ADC0 conv. complete flag
		 ADC0CN0_ADWINT = 0;                        // clear window detector flag
		 ADC0CN0_ADBUSY = 1;                        // initiate conversion

		 // Wait for conversion to complete
		 while(!ADC0CN0_ADINT);

		 DEBUG_PIN = 0;                      // Turn off the debug pin

		 // Check if window detector has detected a voltage less than 2.5 V, then
		 // the ADC0CN0_ADWINT flag will be logic 1. If the voltage is higher than 2.5V,
		 // then the ADC0CN0_ADWINT flag will be a logic 0.
		 LED0 = LED_OFF;
		 if(ADC0CN0_ADWINT == 0)
		 {
		   LED0 = LED_ON;
			// Do something if the potentiometer voltage is > 2.5V
		 }


	  }

	  //-----------------------------------------------------------------------
	  // Task #3 - Sleep for the remainder of the interval
	  //-----------------------------------------------------------------------

	  // Place the device in Sleep Mode
	  LPM(SLEEP);                      // Enter Sleep Until Next Alarm


	}
}
