//-----------------------------------------------------------------------------
// EFM8BB52_PortIO_PortMatch.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This software shows the necessary configuration to use Port Match as an
// interrupt source.  If the left button is pressed, the Port Match Interrupt
// toggles the red LED. If the right button is pressed, the Port Match Interrupt
// toggles the blue LED.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   P0.2   - right push button, Port Match
//   P0.3   - left push button, Port Match
//   P1.5   - Blue LED
//   P1.6   - Red LED
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
// 5) Press one of the switches. The corresponding LED will toggle.
//
// Target:         EFM8BB52
// Tool chain:     Generic
//
// Release 0.1 (JS)
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
SI_SBIT(S1, SFR_P0, 2);
SI_SBIT(S2, SFR_P0, 3);

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

   IE_EA = 1;                          // Enable global interrupts

   while(1)
   {
      // Wait for both switches to be released before re-enabling port match
      // interrupts
      if ((S1 == 1) && (S2 == 1))
      {
         EIE1 |= 0x02;
      }
   }
}
