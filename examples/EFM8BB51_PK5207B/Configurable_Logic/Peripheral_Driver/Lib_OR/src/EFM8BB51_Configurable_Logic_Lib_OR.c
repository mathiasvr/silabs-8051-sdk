//-----------------------------------------------------------------------------
// EFM8BB51_Configurable_Logic_Lib_OR.c
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
//   P1.5   - LED0
//   P2.0   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB51 PK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB51 PK board to a PC using a USB cable.
// 3) Compile and download code to the EFM8BB51 PK board.
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
// Target:         EFM8BB51
// Tool chain:     Generic
//
// Release 0.1 (JS)
//    - Initial Revision
//    - 2021
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB51_Register_Enums.h>
#include <InitDevice.h>
#include "clu_0.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 0);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT(LED0, SFR_P1, 5);// P1.5 LED0
#define LED_ON  1
#define LED_OFF 0

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
   // Enable global Configurable Logic interrupts
   // as well as global interrupt enable in Configurator
   enter_DefaultMode_from_RESET();

   // Init CLU0 with P0.2 as Input A, P0.3 as Input B. Output from LUT
   CLU0_init(CLU0_INPUT_A_P0_2, CLU0_INPUT_B_P0_3, CLU0_OUTPUT_MODE_LUT);

   // Set output function to A OR B
   CLU0_setOutputFunction(CLU0_INPUT_A | CLU0_INPUT_B);

   // Enable CLU0 Rising-edge and Falling-edge interrupts.
   CLU0_enableInt(CLU0_RISING_EDGE_IF | CLU0_FALLING_EDGE_IF, true);

   DISP_EN = DISP_BC_DRIVEN;           // Display not driven by EFM8

   while (1) {}
}

//-----------------------------------------------------------------------------
// CLU0_fallingEdgeCb
//-----------------------------------------------------------------------------
//
// Called whenever the CLU0 output transitions from "1" to "0"
//
// Turns the LED0 off.
//
void CLU0_fallingEdgeCb()
{
  LED0 = LED_OFF;
}

//-----------------------------------------------------------------------------
// CLU0_fallingEdgeCb
//-----------------------------------------------------------------------------
//
// Called whenever the CLU0 output transitions from "0" to "1"
//
// Turns the LED0 on.
//
void CLU0_risingEdgeCb()
{
  LED0 = LED_ON;
}

