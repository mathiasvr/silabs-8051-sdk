//-----------------------------------------------------------------------------
// EFM8BB1_PortIO_PortMatch.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This software shows the necessary configuration to use Port Match as an
// interrupt source.  If the right button is pressed, the Port Match Interrupt
// toggles LED_GREEN. If the left button is pressed, the Port Match Interrupt
// toggles LED_BLUE.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC / 1
//   P0.0   - Display enable
//   P0.2   - right push button, Port Match
//   P0.3   - left push button, Port Match
//   P1.4   - LED green
//   P1.5   - LED blue
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB1 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8BB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Press one of the switches. The corresponding LED will toggle.
//
// Target:         EFM8BB1
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 10 OCT 2014
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------
#include <SI_EFM8BB1_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P0, 0);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT(S1, SFR_P0, 2);
SI_SBIT(S2, SFR_P0, 3);

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

   while(1)
   {
      // Wait for both switches to be released before re-enabling port match
      // interrupts
      if ((S1 == 1) && (S2 == 1))
      {
         EIE1 |= EIE1_EMAT__ENABLED;
      }
   }
}
