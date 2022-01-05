//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This demo demonstrates the ADC on the EFM8LB1 STK using the analog joystick
// or P1.1 as the ADC input.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   ADC0   - 14-bit, VREF = VDD (3.3 V)
//   SPI0   - 1 MHz
//   Timer0 - ADC start of conversion
//   Timer2 - 2 MHz (SPI CS delay)
//   Timer3 - 1 kHz (1 ms tick)
//   P0.2   - Push button
//   P0.6   - SPI SCK
//   P1.0   - SPI MOSI
//   P1.1   - ADC input
//   P1.7   - ADC input/Joystick (analog voltage divider)
//   P2.6   - SPI CS (Active High)
//
//-----------------------------------------------------------------------------
// How To Test: EFM8LB1 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8LB1 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8LB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) The Voltmeter demo should start.
//
// Target:         EFM8LB1
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 15 JUL 2015
//

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "InitDevice.h"
#include "disp.h"
#include "voltmeter.h"

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
int main(void)
{
  // Enter default mode
  enter_DefaultMode_from_RESET();

  // Enable all interrupts
  IE_EA = 1;

  DISP_Init();
  Voltmeter_main();

  while (1);
}
