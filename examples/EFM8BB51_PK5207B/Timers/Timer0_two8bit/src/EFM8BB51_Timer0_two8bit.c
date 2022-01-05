//-----------------------------------------------------------------------------
// EFM8BB51_Timer0_two8bit.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program uses Timer0/1 in two 8-bit reload counter/timer mode.
//
// When the timer low interrupts, a software counter (low_counter) increments
// and toggles LED0 when it reaches the value defined by LED_0_TOGGLE_RATE.
//
// When the timer high interrupts, a software counter (high_counter) increments
// and toggles LED1 when it reaches the value defined by LED_1_TOGGLE_RATE.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 8
//   Timer0 - 10 Hz interrupt
//   Timer1 - 33 Hz interrupt
//   P1.5   - LED0
//   P1.6   - LED1
//   P2.0   - Display enable, shared by C2D pin
//            See AN127 for C2D pin sharing information
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB51 Pro Kit
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB51 Pro Kit board to a PC using a USB cable.
// 3) Compile and download code to the EFM8BB51 Pro Kit board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Check that LED0 and LED1 are blinking.
//
// Target:         EFM8BB51
// Tool chain:     Generic
//
// Release 0.1 (DS)
//    - Initial Revision
//    - 27 JAN 2021
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB51_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 0);          // Display Enable, shared by C2D pin
                          // See AN127 for C2D pin sharing info
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

  IE_EA = 1;                             // Enable global interrupts

  while (1);                          // Loop forever
}
