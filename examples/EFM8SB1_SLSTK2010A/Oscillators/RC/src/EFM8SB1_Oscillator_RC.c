//-----------------------------------------------------------------------------
// SB1_Oscillator_RC.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the oscillator for use with an
// external RC Oscillator.
//
//
//
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect an RC network (33pF  and 300k Ohm is used in this example's
//    calculations) to EXTCLK (P0.3). The network should connect in the
//    following manner: VDD - R - P0.3 - C - GND.
// 3) Connect the EFM8SB1 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) Measure the frequency output on P0.1.
//
// Choosing Component Values:
//
//       frequency (Hz) = 1.23 x 10^9 / ( R * C )
//
//       where R is in KOhms
//             C is in pF
//
//       Note: Add 2pF stray capacitance to C.
//
//       Note: Resistor should be 10K Ohm or larger.
//       Note: Capacitor should be 100 pF or smaller.
//
// For a resistor value of 300K, and a capacitor value of 33pF, the
// expected RC_FREQUENCY is 120 kHz. The actual measured frequency
// on the target board was 32 kHz.
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
	//Enter default mode
	enter_DefaultMode_from_RESET();

	DISP_EN = DISP_BC_DRIVEN;

	while (1) {}                             // Spin forever
}
