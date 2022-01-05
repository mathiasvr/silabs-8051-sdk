//-----------------------------------------------------------------------------
// SB1_PCA0_Capture_Input.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program uses the PCA's capture mode to time events on an input pin.
// The difference between subsequent captures is calculated and stored in
// memory.
//
// In this example, PCA Module 0 is used to capture rising edges of an
// incoming digital waveform, and the crossbar is configured to receive the
// CEX0 pin on on P0.1.  The PCA is configured to clock once every 10 us.
//
// Timer2 overflows are used to toggle pin P1.0 at an overflow rate specified
// in Configurator. This signal at P1.0 is provided simply
// as a test method for the code.
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect P0.1 and P1.0 together.
// 3) Connect the EFM8SB1 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) Halt the program inside the PCA0_ISR function.
// 7) Verify that the variable "capture_period" is equal to 50 (=500us).
//
// Alternately:
// 1) Place the switch in "AEM" mode.
// 2) Connect a signal generator to the P0.1 pin.
// 3) Connect the EFM8SB1 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) Verify that the variable "capture_period" matches the expected time
//    between rising edges (in units of 10us).
//
//
// Target:         EFM8SB1
// Tool chain:     Simplicity Studio / Keil C51 9.51
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

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 7);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (TEST_OUT, SFR_P1, 0);    // Output for testing purposes
                                  //    (245 = 10 us
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
	//Enter default mode
	enter_DefaultMode_from_RESET();

	DISP_EN = DISP_BC_DRIVEN;

	while (1)
	{
	  // check to see if Timer2 overflowed
	  if (TMR2CN0_TF2H)
	  {
		 TEST_OUT = !TEST_OUT;         // Toggle test pin
		 TMR2CN0_TF2H = 0;                     // Clear T2 overflow
	  }

	};
}
