//-----------------------------------------------------------------------------
// SB1_PCA0_8Bit_PWM_Output.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program sends a PWM waveform out of an I/O pin, using the PCA's
// 8-bit PWM Output Mode.  The duty cycle of the waveform is modified
// periodically within the main loop.
//
// In this example, PCA Module 0 is used to generate the waveform, and the
// crossbar is configured to send the CEX0 pin out on P1.1.
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8SB1 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Observe the waveform on P1.1.
// 6) Verify that the duty cycle of the waveform varies smoothly between
//    minimum and maximum values.
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
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT (reserved, uint8_t, SI_SEG_XDATA, 0x0000);

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 7);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

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

	uint16_t delay_count;           // Used to implement a delay
	bool duty_direction = 0;             // 0 = Decrease; 1 = Increase

	//Enter default mode
	enter_DefaultMode_from_RESET();

	DISP_EN = DISP_BC_DRIVEN;

	while (1)
	{
	  // Wait a little while
	  for (delay_count = 30000; delay_count > 0; delay_count--);

	  if (duty_direction == 1)         // Direction = Increase
	  {
		 // First, check the ECOM0 bit
		 if ((PCA0CPM0 & 0x40) == 0x00)
		 {
			PCA0CPM0 |= 0x40;          // Set ECOM0 if it is '0'
		 }
		 else                          // Increase duty cycle otherwise
		 {
			PCA0CPH0--;                // Increase duty cycle

			if (PCA0CPH0 == 0x00)
			{
			   duty_direction = 0;     // Change direction for next time
			}
		 }
	  }
	  else                             // Direction = Decrease
	  {
		 if (PCA0CPH0 == 0xFF)
		 {
			PCA0CPM0 &= ~0x40;         // Clear ECOM0
			duty_direction = 1;        // Change direction for next time
		 }
		 else
		 {
			PCA0CPH0++;                // Decrease duty cycle
		 }
	  }

	};
}
