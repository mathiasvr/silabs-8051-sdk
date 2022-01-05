//-----------------------------------------------------------------------------
// CSLibraryExample_NoOverlay
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Capacitive Sensing library example tuned for 500 ms low power scanning
// 
// This example uses the capacitive sensing firmware library to detect
// touches on the TOUCH SENSE SWITCH (P0.3) found on the ToolStick F990 Daughter
// Card. The LED D4 (P0.0) will be lit while the touch sensor is pressed.
// The library puts the device in a low power mode if no touches appear on
// the sensors after 1 second of scanning.  When in a low power mode, the device
// periodically wakes at a rate of 500 ms to check for touches.
//
// Additionally, the switch S2 (P0.2) is set to trigger a port-match interrupt
// on every press and release. Pressing S2 will immediately wake the device
// from sleep mode and turn on LED D3 (P0.1). Releasing the switch will wake
// the device and turn the LED off.
//
//
// For more information on the capacitive sense library and 
// Capacitive Sense Profiler, see AN0828: “Capacitive Sensing Library Overview”
// and AN0829: “Capacitive Sensing Library Configuration Guide” by clicking the
// Application Notes tile from the main Simplicity Studio launcher.
//
// Note: The Capacitive Sense Profiler functionality has been disabled for this
//       example, since ToolSticks do not currently support UART out.
//
//
// How To Test:
// 1) Connect the ToolStick990DC to a ToolStick Base Adapter.
// 2) Compile and download code to the ToolStick Daughter Card by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 3) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 4) LED0 (P0.0 : D4) will be lit whenever the TOUCH SENSE SWITCH is pressed.
// 5) LED1 (P0.1 : D3) will be lit whenever S2 (P0.2) is pressed.
// 6) To measure current consumption:
//		a) Disconnect the ToolStick Base Adapter
//		b) Place a 3V CR2032 battery in the batter holder
//		c) Depopulate the resistor R2
//		d) Place the power switch S1 in the "BAT POWER / LED OFF" position
//		e) Connect an ammeter between the pins labeled 3V->VDD
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "SI_C8051F990_Register_Enums.h"
#include "ProjectConfig.h"
#include "CSLibrary.h"
#include "DeviceInit.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------
SI_SBIT(LED0, SFR_P0, 0);		// P0.0 LED : D4
SI_SBIT(LED1, SFR_P0, 1);		// P0.1 LED : D3
SI_SBIT(S2, SFR_P0, 2);		// P0.2 Button : S2

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
enum CS0_SENSORS
{
	TOUCH_SENSE_SWITCH,
};

enum LED_STATES
{
	LED_ON = 0,
	LED_OFF = 1
};

enum SWITCH_STATES
{
	SWITCH_PRESSED = 0,
	SWITCH_RELEASED = 1
};


//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{

   SystemInit();                       // Configures all peripherals not
                                       // controlled by the capsense library

   InitAlgorithmControlledHardware();  // Configures all peripherals 
                                       // controlled by capsense, including
                                       // the sensing block and port pins
  
   InitAlgorithm();                    // Initializes capsense variables
                                       // and performs some scans to initialize
                                       // the baselines

   LED0 = LED_OFF;
   LED1 = LED_OFF;

   while(1)
   {
      MasterAlgorithmTick();           // Performs all scanning and data
                                       // structure updates

     if(IsSensorDebounceActive(TOUCH_SENSE_SWITCH)){
    	  LED0 = LED_ON;
      } else {
    	  LED0 = LED_OFF;
      }

      LowPowerTick();                  // If low power features are enabled,
                                       // this will either put the device into
                                       // a low power state until it is time
                                       // to take another scan or put the
                                       // device into a low-power sleep mode
                                       // if no touches are active
   }
}

//-----------------------------------------------------------------------------
// PortMatch ISR
//-----------------------------------------------------------------------------
//
// This ISR is triggered upon a portmatch event, caused when S2 (P0.2) is
// either pressed or released.
//
// If S2 is pressed, LED1 (P0.1) is turned on.
// If S2 is released, LED1 is turned off.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PortMatch, PMATCH_IRQn)
{
	if (S2 == SWITCH_PRESSED) {
		LED1 = LED_ON;
	} else {
		LED1 = LED_OFF;
	}

	P0MAT ^= P0MAT_B2__BMASK;		// Flip the S2 match bit
									// This causes a port match event to occur
									// on both press and on release
}

