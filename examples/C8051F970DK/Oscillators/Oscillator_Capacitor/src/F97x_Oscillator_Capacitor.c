//-----------------------------------------------------------------------------
// F97x_Oscillator_Capacitor.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the system clock using with an
// external capacitor.
//
//
// How To Test:
// ------------
// 1) Connect a 33pF capacitor between P0.3 (XTAL2) and GND. And
//    specify the desired XFCN setting by modifying the constant <XFCN>.
// 2) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
// 3) Connect the USB Debug Adapter to H8.
// 4) Place the VDD SELECT switch (SW1) in the VREG position and power the board.
// 5) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 6) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 7) Measure the frequency output on P0.1. Note that the SYSCLK routed on I/O
//    pin is half of the oscillator frequency for C and RC modes.
//
//
// The oscillation frequency and the required External Oscillator Frequency
// Control value (XFCN) can be determined by the following equation:
//
//       Clock frequency (MHz) = KF(XFCN) / (C * VDD)
//
//       where
//       KF(XFCN) is a factor based on XFCN.
//       C = capacitor value on the XTAL2 pin in pF
//       Vdd = power supply voltage in Volts
//
//       XFCN | K Factor
//       ---------------
//         0  |    0.87
//         1  |    2.6
//         2  |    7.7
//         3  |   22
//         4  |   65
//         5  |  180
//         6  |  664
//         7  | 1590
//
// The actual frequency is best determined by measurement. In this example,
// a 33pF capacitor was used with VDD = 3.3V. The actual measured SYSCLK
// frequency on P0.1 was 109.9 kHz when XFCN is 3.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (CM2/JL)
//    - 29 MAY 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h>
#include "InitDevice.h"

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
  SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
  PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main(void)
{
  enter_DefaultMode_from_RESET();

  while (1)
    ;
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
