//-----------------------------------------------------------------------------
// EFM8BB1_Ports_SwitchLED.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure port pins as digital inputs
// and outputs.  The EFM8BB1 has two push-button switches and an RGB LED connected
// to port pins.  The program constantly checks the status of the switches and
// if PB0 is closed, it turns off Green LED; otherwise, Green LED is on.
// PB1 controls Blue LED in the same manner.
//
//-----------------------------------------------------------------------------
// How To Test:
//-----------------------------------------------------------------------------
// 1) Place the power switch in the AEM position and power the board
//    using the DBG USB connector.
// 2) Compile and download code to the EFM8BB1 board by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 3) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
// 4) Push the buttons (PB0 or PB1) and see that the corresponding LED (green
//    or blue) turns off.
//
//
// Target:         EFM8BB1
// Command Line:   None
//
//
// Release 1.2 (BL)
//		- Updated How To Test
//		- 2 JAN 2014
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 19 FEB 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_EFM8BB1_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(GREEN_LED, SFR_P1, 4);   // Green LED
SI_SBIT(BLUE_LED, SFR_P1, 5);    // Blue LED
SI_SBIT(S1, SFR_P0, 2);				   // PB0
SI_SBIT(S2, SFR_P0, 3);				   // PB1

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK      24500000/8         // Clock speed in Hz (default)

#define SW_PRESSED           0
#define SW_NOT_PRESSED       1

#define LED_ON               0
#define LED_OFF              1

//-----------------------------------------------------------------------------
// Function Prototypes
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
//-----------------------------------------------------------------------------

void main (void)
{
	enter_DefaultMode_from_RESET();

   while (1)
   {
      // Set the state of LED1
      if (S1 == SW_PRESSED)
      {
         GREEN_LED = LED_OFF;
      }
      else
      {
         GREEN_LED = LED_ON;
      }

      // Set the state of LED2
      if (S2 == SW_PRESSED)
      {
         BLUE_LED = LED_OFF;
      }
      else
      {
         BLUE_LED = LED_ON;
      }
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
