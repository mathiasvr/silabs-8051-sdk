//-----------------------------------------------------------------------------
// SB1_SleepMode_PortMatchWake.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program flashes LED0 on the EFM8SB2 target board using the
// interrupt handler for Timer2 when the MCU is awake.
//
// Pressing the switch on PB1  will place the device in a low power mode.
// Pressing the switch on PB0  will wake up the device using Port Match.
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
// 5) Press PB0 to place the device in sleep mode.  LED1 will stay lit
//    while the device is sleeping.
//    Press PB1 to wake it back up. The LED0 will blink when
//    the device is awake.
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
#include "smartclock.h"                // RTC Functionality
#include "power.h"                     // Power Management Functionality

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 6);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (LED0, SFR_P2, 0);          // '0' means ON, '1' means OFF
SI_SBIT (LED1, SFR_P2, 1);          // '0' means ON, '1' means OFF
SI_SBIT (PB0,  SFR_P0, 2);          // PB0 == 0 means switch pressed
SI_SBIT (PB1,  SFR_P0, 3);          // PB1 == 0 means switch pressed

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

	LED0 = LED_OFF;

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
		 LED0 = LED_ON;
		 LED1 = LED_OFF;
	  }

	  //-----------------------------------------------------------------------
	  // Task #2 - Enter sleep mode if P0.3 switch is pressed
	  //-----------------------------------------------------------------------

	  if(!PB1)                         // If the P0.3 switch is pressed
	  {

		 // Configure the Port I/O for Low Power Mode

		 LED0 = LED_OFF;
		 LED1 = LED_ON;  // Turn on LED0 to indicate that
									   // the device is entering the low power
									   // state
		 LPM (SLEEP);

	  }
	}
}
