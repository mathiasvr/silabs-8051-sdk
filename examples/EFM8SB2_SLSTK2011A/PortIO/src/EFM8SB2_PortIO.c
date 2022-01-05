//-----------------------------------------------------------------------------
// SB1_Ports_SwitchLED.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure port pins as digital inputs
// and outputs.  The EFM8SB2 target board has two push-button switches
// and one LED connected to port pins.  The program constantly checks the
// status of the switches and if one or both are closed, it turns off
// the LED or sets the output of TEST_OUT (P0.1) to be low.
//
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8SB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8SB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Push the button (PB0 or PB1) and see that the corresponding
//    LED0 turns off, or TEST_OUT (P0.1) goes low.
//
//
// Target:         EFM8SB2
// Tool chain:     Simplicity Studio / Keil C51 9.51
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

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 6);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT(LED1, SFR_P2, 1);          // '0' means ON, '1' means OFF
SI_SBIT(LED0, SFR_P2, 0);          // '0' means ON, '1' means OFF
SI_SBIT(PB0, SFR_P0, 2);          // PB0 == 0 means switch pressed
SI_SBIT(PB1, SFR_P0, 3);          // PB1 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define LED_ON  0
#define LED_OFF 1

#define SWITCH_PRESSED  0
#define SWITCH_RELEASED 1

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
int main(void)
{
  //Enter default mode
  enter_DefaultMode_from_RESET();

	DISP_EN = DISP_BC_DRIVEN;

  while (1)
  {

    // Set the state of the red LED
    if (PB1 == SWITCH_PRESSED)      // If switch pressed
    {
      LED1 = LED_ON;               // Turn on LED1
    }
    else
    {
      LED1 = LED_OFF;                 // Else, turn it off
    }

    // Set the state of the yellow LED
    if (PB0 == SWITCH_PRESSED)      // If switch pressed
    {
      LED0 = LED_ON;         // Turn on LED0
    }
    else
    {
      LED0 = LED_OFF;          // Else, turn it off
    }
  }                                 // end of while(1)
}
