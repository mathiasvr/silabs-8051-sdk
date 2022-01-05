//-----------------------------------------------------------------------------
// SB1_PCA0_16Bit_PWM_Output.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program sends a PWM waveform out of an I/O pin, using the PCA's
// 16-bit PWM Output Mode.  The duty cycle of the waveform is modified
// periodically within the main loop.
//
// In this example, PCA Module 0 is used to generate the waveform, and the
// crossbar is configured to send the CEX0 pin out on P2.0.
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
// 5) Observe the waveform on P2.0.
// 6) Verify that the duty cycle of the waveform varies smoothly between
//    minimum and maximum values.
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
// Global VARIABLES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint16_t CEX0_Compare_Value;       // Holds current PCA compare value

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 6);          // Display Enable
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
	//unsigned int16_t delay_count;
	uint8_t delay_count;          // Used to implement a delay
	bool duty_direction = 0;             // 0 = Decrease; 1 = Increase

	// Configure initial PWM duty cycle = 50%
	CEX0_Compare_Value = 65536 - (65536 * 0.5);

	//Enter default mode
	enter_DefaultMode_from_RESET();

	DISP_EN = DISP_BC_DRIVEN;

	while (1)
	{
		// Wait a little while
		for (delay_count = 120; delay_count > 1; delay_count--);

		if (duty_direction == 1)         // Direction = Increase
		{
		 // First, check the ECOM0 bit
		 if ((PCA0CPM0 & 0x40) == 0x00)
		 {
			PCA0CPM0 |= 0x40;          // Set ECOM0 if it is '0'
		 }
		 else                          // Increase duty cycle otherwise
		 {
			CEX0_Compare_Value--;      // Increase duty cycle

			if (CEX0_Compare_Value == 0x0000)
			{
			   duty_direction = 0;     // Change direction for next time
			}
		 }
		}
		else                             // Direction = Decrease
		{
		 if (CEX0_Compare_Value == 0xFFFF)
		 {
			PCA0CPM0 &= ~0x40;         // Clear ECOM0
			duty_direction = 1;        // Change direction for next time
		 }
		 else
		 {
			CEX0_Compare_Value++;      // Decrease duty cycle
		 }
		}
	}                             // Spin forever
}
