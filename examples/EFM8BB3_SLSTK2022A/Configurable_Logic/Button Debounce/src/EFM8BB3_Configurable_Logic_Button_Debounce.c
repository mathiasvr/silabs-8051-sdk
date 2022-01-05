//-----------------------------------------------------------------------------
// EFM8BB3_Configurable_Logic_Button_Debounce.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to use the Configurable Logic and Timer to
// debounce a button without using any CPU resources.  The state of
// CLOUT0.CLOUT0_C1OUT__BMASK represents the debounced state of the button
// - high indicates button is pressed, low indicates button is released
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   P2.2   - button input, active low - routed to MXB of CLU2
//   P1.0   - output to represent debounced logic - high = pressed, low = released
//   CLU1 and CLU2   - used to implement debounce logic
//   Timer 2 - reload count of 10 ms.  Reload force select CLU2 output
//
// Target:         EFM8BB3
// Tool chain:     Generic
//
// Release 0.1 (KHS)
//    - Initial Revision
//    - 9 OCT 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB3_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include "SI_LUT.h"

// $[Generated Includes]
// [Generated Includes]$

#define BTN_IN      P2_B3

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
    // Call hardware initialization routine
    enter_DefaultMode_from_RESET();

    // Workaround bug in configurator not generating P2MDOUT
    P2MDOUT = 0x01;
    SFRPAGE = 0x20;

    CLU1FN = LUT_NOT(SI_LUT_B);
    CLU3FN = LUT_XOR(SI_LUT_A, SI_LUT_B);
    CLEN0 |= CLEN0_C1EN__ENABLE | CLEN0_C3EN__ENABLE;
    SFRPAGE = 0x00;

    while (1)
    {
// $[Generated Run-time code]
// [Generated Run-time code]$
    }
}

