//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates a Space Invaders game on the EFM8BB2 STK using
// the LCD, joystick, push buttons, and RGB LED.
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB2 STK
//-----------------------------------------------------------------------------
// 1) Place the SW104 switch in "AEM" mode.
// 2) Connect the EFM8BB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8BB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) The Space Invaders game should start.
//
// Target:         EFM8BB2
// Tool chain:     Generic
//
// Release 0.1 (CM)
//    - Initial Revision
//    - 10 OCT 2014
//
//-----------------------------------------------------------------------------
// Resources
//-----------------------------------------------------------------------------
// SYSCLK - 24.5 MHz HFOSC0 / 1
// ADC0   - 10-bit
// SPI0   - 1 MHz
// Timer2 - 2 MHz (SPI CS delay)
// Timer3 - 1 kHz (1 ms tick)
// P0.1 - CS (Active High)
// P0.2 - push button
// P0.3 - push button
// P0.6 - SCK
// P1.0 - MOSI
// P1.4 - LED Green
// P1.5 - LED Blue
// P1.6 - LED Red
// P1.7 - Joystick (analog voltage divider)
// P2.3 - Display enable
//

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "InitDevice.h"
#include "disp.h"
#include "space_invaders.h"

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
 
//---------------------------------------------------------------------------
// main() Routine
// --------------------------------------------------------------------------
void main (void)
{
	// Enter default mode
	enter_DefaultMode_from_RESET();

	// Enable all interrupts
	IE_EA = 1;

	DISP_Init();

	while (1)
	{
	    SpaceInvaders_Main();
	}
}
