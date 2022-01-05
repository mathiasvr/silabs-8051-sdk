//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// The Configurable Logic Unit on EFM8LB1 is configured to implement 
// AND, NAND, OR, NOR, XOR and XNOR logic gates. The push buttons are the
// logic gate inputs. Move the joystick to change the logic gate.
//
//-----------------------------------------------------------------------------
// How To Test: EFM8LB1 STK
//-----------------------------------------------------------------------------
// 1) Place the  switch in "AEM" mode.
// 2) Connect the EFM8LB1 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8LB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Press the push buttons and observe the input and output states on the
//    screen.
// 6) Move the joystick in any direction to change the logic gate.
//
// Target:         EFM8LB1
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 7 July 2015
//
//-----------------------------------------------------------------------------
// Resources
//-----------------------------------------------------------------------------
// SYSCLK - 24.5 MHz HFOSC0 / 1
// CLU0   - Logic AND, NAND, OR, NOR, XOR, XNOR
// ADC    - 10-bit (VREF = VDD)
// SPI0   - 1 MHz
// Timer2 - 2 MHz (SPI CS delay)
// Timer3 - 1 kHz (1 ms tick)
// P0.1   - SPI CS (Active High)
// P0.2   - CLU0 input A / Push Button 0 (PB0)
// P0.3   - CLU0 input B / Push Button 1 (PB1)
// P0.6   - SPI SCK
// P1.0   - SPI MOSI
// P1.7   - Joystick (analog voltage divider)
// P2.3   - Display enable
//

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "InitDevice.h"
#include "bsp.h"
#include "disp.h"
#include "logic.h"

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

	logic_main();
}


