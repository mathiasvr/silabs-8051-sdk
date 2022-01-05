//-----------------------------------------------------------------------------
// Ports_SwitchesLEDs.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the port pins as digital inputs
// and outputs.  The EFM8UB2 target board has two push-button switches
// connected to port pins and and two LEDs.  The program constantly checks the 
// status of the switches and if they are pushed, it turns on the 
// respective LED.
//
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8UB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8UB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Press PB0 and PB1 and observe that the LEDs turn on.
//
//
// Target:         EFM8UB2
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 13 JAN 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------
SI_SBIT(SW1, SFR_P0, 2);                  // SW1 ='0' means switch pressed
SI_SBIT(SW2, SFR_P0, 3);                  // SW2 ='0' means switch pressed
SI_SBIT(LED1, SFR_P1, 6);                      // LED='0' means ON
SI_SBIT(LED2, SFR_P1, 7);                      // LED='0' means ON

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
      if (SW1 == 0)                    // If switch depressed
      { 
         LED1 = 0;                     // Turn on LED
      }
	  else   
      {  
         LED1 = 1;                     // Else, turn it off
      }                   
	  
      if (SW2 == 0)                    // If switch depressed
      { 
         LED2 = 0;                     // Turn on LED
      }
	  else   
      {  
         LED2 = 1;                     // Else, turn it off
      }                   
   }                                   // end of while(1)
}                                      // end of main()

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
