//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Use scope to observe DAC outputs on P3.0 - P3.3.
// Move joystick left/right to change functions.
// Move joystick up/down to change frequency.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   DAC0
//   DAC1
//   DAC2
//   DAC3
//   ADC0   - 10-bit, VREF = VDD (3.3 V)
//   SPI0   - 1 MHz
//   Timer2 - 2 MHz (SPI CS delay)
//   Timer3 - 1 kHz (1 ms tick)
//   Timer4 - 150 kHz interrupt (DAC update trigger)
//   P0.2   - Push button (kill splash screen)
//   P0.3   - Push button (kill splash screen)
//   P0.6   - SPI SCK
//   P1.0   - SPI MOSI
//   P1.7   - ADC input / Joystick (analog voltage divider)
//   P2.6   - SPI CS (Active High)
//   P3.0   - DAC0 output
//   P3.1   - DAC1 output
//   P3.1   - DAC2 output
//   P3.3   - DAC3 output
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
// 5) The Function Generator demo should start.
//    Press any button, move joystick, or wait 4 sec to kill splash screen
//    Use scope to observe DAC output on P3.0 - P3.3.
//    Move the joystick left/right to change functions.
//    Move the joystick up/down to increase/decrease the frequency.
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
#include "function_generator.h"

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
  enter_DefaultMode_from_RESET();

  // Enable all interrupts
  IE_EA = 1;

  DISP_Init();
  FunctionGenerator_main();

  while (1);
}
