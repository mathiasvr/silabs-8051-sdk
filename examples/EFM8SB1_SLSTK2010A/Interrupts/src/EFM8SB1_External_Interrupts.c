//-----------------------------------------------------------------------------
// SB1_External_Interrupts.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This software shows the necessary configuration to use External Interrupt 0
// (/INT0) or External Interrupt 1 (/INT1) as an interrupt source.  The code
// executes the initialization routines and then spins in an infinite while()
// loop.
//
// When PB0 or PB1 are pressed, pins P0.2 and P0.3 are toggled, respectively.
// The external interrupts INT0 and INT1 are routed to these pins, causing
// and interrupt to fire whenever the pins are toggled. These interrupts are
// then used to toggle the state of LED0.
//
//
// Pinout:
//
// P1.2 - PB0
// P1.3 - PB1
// P0.2 - /INT0
// P0.3 - /INT1
//
// P1.1 - Yellow LED
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8SB1 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Press the switches.  Every time a switch is pressed, LED0 should
//    toggle.
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
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 7);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT(PB0, SFR_P1, 2);
SI_SBIT(PB1, SFR_P1, 3);

SI_SBIT(INT0, SFR_P0, 2);
SI_SBIT(INT1, SFR_P0, 3);

enum
{
  SW_PRESSED = 0,
  SW_RELEASED,
};

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
int main(void)
{

  //Enter default mode
  enter_DefaultMode_from_RESET();

	DISP_EN = DISP_BC_DRIVEN;

  while (1)
  {
    if (PB0 == SW_PRESSED)
    {
      INT0 = 0;
    }
    else
    {
      INT0 = 1;
    }

    if (PB1 == SW_PRESSED)
    {
      INT1 = 0;
    }
    else
    {
      INT1 = 1;
    }
  }                             // Spin forever
}
