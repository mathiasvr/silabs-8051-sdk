//-----------------------------------------------------------------------------
// SB1_Timer0_two_8bitTimers.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program presents an example of use of the Timer0 of the EFM8SB1 in
// two 8-bit reload counter/timer mode. It uses the EFM8SB1DK as HW platform.
//
// It sets the two timers to interrupt every millisecond.
//
// When the timer low interrupts a counter (low_counter) increments and upon
// reaching the value defined by LED0_TOGGLE_RATE toggles LED0.
// When the timer high interrupts a counter (high_counter) increments and upon
// reaching the value defined by TEST_OUT_TOGGLE_RATE toggles the TEST_OUT pin.
//
// Pinout:
//
//    P1.1 -> LED0        (toggled by TL0)
//    P0.1 -> TEST_OUT    (toggled by TH0)
//
//    all other port pins unused
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8SB1 STK board to a PC using a mini USB cable.
// 3) If a different LED0 blink rate is desired change the following constant:
//       -> LED0_TOGGLE_RATE in Interrupts.c
// 4) If a different TEST_OUT toggle rate is desired change the following constant:
//       -> TEST_OUT_TOGGLE_RATE in Interrupts.c
// 5) Compile and download code to the EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 6) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 7) The LED0 should blink and TEST_OUT should toggle.
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
