//-----------------------------------------------------------------------------
// F93x_PCA0_16Bit_PWM_Output.c
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
// crossbar is configured to send the CEX0 pin out on P0.0.
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J9.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Monitor the output waveform on P0.0.
// 7) Verify that the duty cycle of the waveform varies smoothly between
//    minimum and maximum values.
//
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 15 JAN 2014
//
// Release 1.2
//    - Updated to be compatible with Raisonance Toolchain (FB)
//    - 14 APR 2010
//
// Release 1.1
//    - Compiled and tested for C8051F930-TB (JH)
//    - 07 JULY 2009
//
// Release 1.0
//    - Initial Revision (FB)
//    - 16 NOV 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F930_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint16_t CEX0_Compare_Value;       // Holds current PCA compare value

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
	uint8_t delay_count;          // Used to implement a delay
	bit duty_direction = 0;             // 0 = Decrease; 1 = Increase

	//Enter default mode
	enter_DefaultMode_from_RESET();
	// Configure initial PWM duty cycle = 50%
	CEX0_Compare_Value = 65536 - (65536 * 0.5);

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

	};

	// NOTREACHED
	return 0;
}
