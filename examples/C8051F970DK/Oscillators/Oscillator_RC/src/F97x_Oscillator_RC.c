//-----------------------------------------------------------------------------
// F97x_Oscillator_RC.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the system clock using with an
// external RC network.
//
//
// How To Test:
// ------------
// 1) Connect a 33pF capacitor between P0.3 (XTAL2) and GND, and a
//    330kOhm resistor between P0.3 (XTAL2) and Vcc (3.3V). And
//    specify the desired frequency by changing the constant <RC_FREQUENCY>
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
//    pin is half of the oscillator frequency for RC and C modes.
//
//
// The capacitor should be no greater than 100 pF and the resistor should be no
// smaller than 10 kOhm.
// The oscillation frequency can be determined by the following equation:
//
//       Clock frequency (MHz) = 1.23 x 10^3 / ( R * C )
//
//       where
//       R = pull-up resistor value in kOhm
//       C = capacitor value on the XTAL2 pin in pF
//
// Note: Add 2pF stray capacitance to C.
//
// For a resistor value of 330K, and a capacitor value of 33pF, the
// expected RC_FREQUENCY is 112.9 kHz. The actual measured SYSCLK frequency
// on P0.1 was 54.5 kHz.
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
