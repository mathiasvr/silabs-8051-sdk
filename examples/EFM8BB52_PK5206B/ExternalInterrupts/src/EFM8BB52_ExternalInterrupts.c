//-----------------------------------------------------------------------------
// EFM8BB52_ExternalInterrupts.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This software shows the necessary configuration to use External Interrupt 0
// (/INT0) or External Interrupt 1 (/INT1) as an interrupt source. As is the
// case on a standard 8051 architecture, certain controls for these two
// interrupt sources are available in the Timer0/1 registers. INT0 and INT1 are
// configured to be edge-triggered with active low polarity.
// The code executes the initialization routines and then spins in an infinite
// while() loop. If BTN0 is pressed, then the edge-triggered /INT0 input on P0.2
// will cause an interrupt and toggle LED_BLUE. If BTN1 is pressed,then the
// edge-triggered /INT1 input on P0.3 will cause an interrupt and toggle
// LED_RED.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   P0.2   - Push Button BTN0, /INT0 input
//   P0.3   - Push Button BTN1, /INT1 input
//   P1.5   - LED Blue
//   P1.6   - LED Red
//   P2.0   - Display enable
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
// 5) Press the switches.  Every time a switch is pressed, one of
//    the LEDs should toggle.
//
// Target:         EFM8BB52
// Tool chain:     Generic
//
// Release 0.1 (MR)
//    - Initial Revision
//
//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------
#include <SI_EFM8BB52_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 0);          // Display Enable
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

   DISP_EN = DISP_BC_DRIVEN;           // EFM8 does not drive display

   IE_EA = 1;

   while(1);                           // Infinite while loop waiting for
                                       // an interrupt from /INT0 or /INT1
}
