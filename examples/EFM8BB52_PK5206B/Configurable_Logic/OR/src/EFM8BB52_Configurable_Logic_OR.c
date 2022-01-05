//-----------------------------------------------------------------------------
// EFM8BB52_Configurable_Logic_OR.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program uses the Configurable Logic Unit (CLU) to implement a two-input,
// one-output OR gate.
//
// The CLU interrupt will trigger on CLU0 rising and falling edges and will:
//   turn on the green LED when the CLU0 output is high
//   turn off the green LED when the CLU0 output is low
//
//          +--------+
//   P0.2 --|A       |
//   P0.3 --|B      Y|-- CLOUT0[0]
//   x    --|C       |
//          +--------+
//
// Look Up Table
//   +------------------+-----------+
//   |       Inputs     |  Output   |
//   |  P0.2  P0.3   C  | CLOUT0[0] |
//   |------------------+-----------|
//   |    1     1    x  |     1     |
//   |    1     1    x  |     1     |
//   |    1     0    x  |     1     |
//   |    1     0    x  |     1     |
//   |    0     1    x  |     1     |
//   |    0     1    x  |     1     |
//   |    0     0    x  |     0     |
//   |    0     0    x  |     0     |
//                            ^--- 0xFC = CLUnFN setting to implement this
//                                        look up table
//   x is don't care
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   P0.2   - CLU0 input A / Push Button 0 (PB0)
//   P0.3   - CLU0 input B / Push Button 1 (PB1)
//   P1.4   - LED green
//   P3.4   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB52 PK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB52 PK board to a PC using a USB cable.
// 3) Compile and download code to the EFM8BB52 PK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Press the push buttons and observe LED0.
//    When a button is pressed, the CLU input is logic low.
//    When a button is not pressed, the CLU input is logic high.
//    When the CLU output is logic high, the LED will be on.
//    When the CLU output is logic low, the LED will be off.
//
// Target:         EFM8BB52
// Tool chain:     Generic
//
// Release 0.1 (JS)
//    - Initial Revision
//    - 2021
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB52_Register_Enums.h>
#include <InitDevice.h>

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P3, 4);          // Display Enable
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
// Main Routine
//-----------------------------------------------------------------------------
void main (void)
{
   enter_DefaultMode_from_RESET();

   DISP_EN = DISP_BC_DRIVEN;           // Display not driven by EFM8

   IE_EA = 1;                          // Enable global interrupts

   while (1) {}
}


