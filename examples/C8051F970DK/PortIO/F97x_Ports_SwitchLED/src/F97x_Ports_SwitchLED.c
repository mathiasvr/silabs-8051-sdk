//-----------------------------------------------------------------------------
// F97x_Ports_SwitchLED.c
//-----------------------------------------------------------------------------
// Copyright 2010 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Configure port pins as different types of inputs and outputs
//
// This program demonstrates how to configure port pins as digital inputs
// and outputs.  The C8051F970 target board has 2 LEDs connected to port pins.
// The program turn on/off these 2 LEDs alternatively.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) Push BTN03 or BTN04 buttons and see that the corresponding
//    LED05 or LED15 turns on.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 02 MAY 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(LED05, SFR_P0, 5);         // '0' means ON, '1' means OFF
SI_SBIT(LED15, SFR_P1, 5);         // '0' means ON, '1' means OFF

SI_SBIT(BTN03, SFR_P0, 3);         // '0' means ON, '1' means OFF
SI_SBIT(BTN04, SFR_P0, 4);         // '0' means ON, '1' means OFF

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define LED_ON             0
#define LED_OFF            1

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
  SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
  PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main(void)
{
  enter_DefaultMode_from_RESET();

  LED05 = LED_ON;
  LED15 = LED_OFF;

  while (1) {
    if (BTN03 == 0)                   // If switch pressed
        {
      LED05 = LED_ON;                // Turn off LED
    } else {
      LED05 = LED_OFF;               // Turn off LED
    }

    if (BTN04 == 0)                   // If switch pressed
        {
      LED15 = LED_ON;                // Turn off LED
    } else {
      LED15 = LED_OFF;               // Turn off LED
    }
  }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
