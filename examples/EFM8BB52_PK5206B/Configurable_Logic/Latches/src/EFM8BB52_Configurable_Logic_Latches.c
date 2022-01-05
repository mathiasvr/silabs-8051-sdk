//-----------------------------------------------------------------------------
// EFM8BB52_Configurable_Logic_Latches.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to use the Configurable Logic to
// create SR latch and D latch using only LUT logic.  This is especially
// useful in applications where we may wish to have some sort of
// memory element output to the carry pin.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   P0.2   - Data input of D latch
//   P0.4   - Clock input of D latch
//   P1.0   - Output of D latch
//   P1.1   - Reset input of SR latch
//   P0.6   - Set input of SR latch
//   P2.5   - Output of SR latch
//   All CLUs
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
#include <SI_EFM8BB52_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include "SI_LUT.h"

#define S_IN    P0_B6
#define R_IN    P1_B1
#define D_IN    P0_B2
#define CLK_IN  P0_B4

#define Q_SR    P2_B5
#define Q_D     P1_B0

// $[Generated Includes]
// [Generated Includes]$

// Check the SR latch output is same as state
uint8_t verify_Q_SR(bool state)
{
    if (Q_SR != state) {
        return 0;
    }
    // wait some clock cycles to check again
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    return (Q_SR == state);
}

// Check the D latch output is same as state
uint8_t verify_Q_D(bool state)
{
    if (Q_D != state) {
        return 0;
    }
    // wait some clock cycles to check again
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    return (Q_D == state);
}

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

    // Workaround bug where P2MDOUT is not initialized by Configurator generated firmware
    P2MDOUT = 0x20;

    SFRPAGE = 0x20;
    CLU0FN = SI_LUT_A;
    // D latch boolean function:
    //  Q = Q.(DATA + not(CLK)) + DATA.CLK
    // In CLU1, these are the input assignments
    //  DATA = Carry
    //  CLK = MXA
    //  Q = MXB
    CLU1FN = LUT_OR(LUT_AND(SI_LUT_B, LUT_OR(SI_LUT_C, LUT_NOT(SI_LUT_A))),
            LUT_AND(SI_LUT_A, SI_LUT_C));
    CLU2FN = SI_LUT_A;
    // SR latch boolean function:
    //  Q = Q.not(RESET) + not(Q).SET
    // In CLU3, these are the input assignments:
    //  SET = MXA
    //  RESET = Carry
    //  Q = MXB
    CLU3FN = LUT_OR(LUT_AND(SI_LUT_B, LUT_NOT(SI_LUT_C)),
            LUT_AND(LUT_NOT(SI_LUT_B), SI_LUT_A));
    S_IN = 0;
    R_IN = 0;
    D_IN = 0;
    CLK_IN = 0;
    CLEN0 = 0x0f; // Enable all the CLUs
    R_IN = 1; // Reset the SR latch
    R_IN = 0;
    CLK_IN = 1; // Reset the D latch
    CLK_IN = 0;
    SFRPAGE = 0x00;

    // Validate the SR latch output
    if (!verify_Q_SR(0))
    {
        while (1)
            ; // Failed if output of SR latch is not low
    }

    // Check the set
    S_IN = 1;
    S_IN = 0;
    if (!verify_Q_SR(1))
    {
        while (1)
            ; // Failed if output of SR latch is not high
    }

    // Check the set again
    S_IN = 1;
    S_IN = 0;
    if (!verify_Q_SR(1))
    {
        while (1)
            ; // Failed if output of SR latch is not high
    }

    // Check the reset
    R_IN = 1;
    R_IN = 0;
    if (!verify_Q_SR(0))
    {
        while (1)
            ; // Failed if output of SR latch is not low
    }

    // Check the reset again
    R_IN = 1;
    R_IN = 0;
    if (!verify_Q_SR(0))
    {
        while (1)
            ; // Failed if output of SR latch is not low
    }

    // Check the D latch - toggle D while CLK is low
    D_IN = 1;
    if (!verify_Q_D(0))
    {
        while (1)
            ; // Should not be high
    }
    NOP();
    NOP();
    D_IN = 0;
    NOP();
    NOP();
    // Check again after resetting D to 0
    if (!verify_Q_D(0))
    {
        while (1)
            ; // Should not be high
    }

    // Clock in high
    D_IN = 1;
    CLK_IN = 1;
    if (!verify_Q_D(1))
    {
        while (1);      // Should be high
    }
    D_IN = 0;
    if (!verify_Q_D(0)) // Q should follow D while clock is high
    {
        while (1)
            ; // Should follow D when clock is high
    }
    D_IN = 1;
    CLK_IN = 0;
    if (!verify_Q_D(1)) // Q should be high - the last latched value before CLK is low
    {
        while (1);
    }

    D_IN = 0;
    if (!verify_Q_D(1))   // Q should remain high - the last latched value before CLK was low
    {
        while (1);
    }
    NOP();
    NOP();
    D_IN = 1;
    NOP();
    NOP();
    // Check again after setting D to 1
    if (!verify_Q_D(1))
    {
        while (1)
            ; // Should not be low
    }

    D_IN = 0;
    CLK_IN = 1;     // Clock in the low data
    if (!verify_Q_D(0))
    {
        while (1);
    }
    D_IN = 1;
    if (!verify_Q_D(1)) // Q should follow D while clock is high
    {
        while (1)
            ; // Should follow D when clock is high
    }
    D_IN = 0;
    CLK_IN = 0;
    if (!verify_Q_D(0)) // Q should be low - the last latched value before CLK is low
    {
        while (1);
    }


    while (1)
    {
// $[Generated Run-time code]
// [Generated Run-time code]$
    }
}
