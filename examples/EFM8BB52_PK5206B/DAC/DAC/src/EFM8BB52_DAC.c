//-----------------------------------------------------------------------------
// EFM8BB52_DAC.c
//-----------------------------------------------------------------------------
// Copyright 2020 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program outputs a sine wave using DAC0. The DAC outputs are scheduled to
// update at a rate determined by the constant <SAMPLE_RATE_DAC>, managed and
// timed by Timer3.
//
// Implements a 256-entry full-cycle sine table of 10-bit precision.
//
// The output frequency is determined by a 16-bit phase adder.
// At each DAC update cycle, the phase adder value is added to a running
// phase accumulator, <phase_accumulator>, the upper bits of which are used
// to access the sine lookup table.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   DAC0   - 1 kHz sine wave
//   VREF   - 2.4V for DAC reference
//   Timer3 - 200 kHz interrupt / DAC update source
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
// 5) Use an oscilloscope to observe DAC0 output on P3.0. Verify that there is a
//    sine wave with a peak-to-peak amplitude of 2.4V
//
// Target:         EFM8BB52
// Tool chain:     Generic
//
// Release 0.1 (MR)
//    - Initial Revision
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB52_Register_Enums.h>
#include <InitDevice.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK          24500000       // SYSCLK frequency in Hz
#define SAMPLE_RATE_DAC 200000L        // DAC sampling rate in Hz

//-----------------------------------------------------------------------------
// Timer3_Init
//-----------------------------------------------------------------------------
void Timer3_Init(int counts)
{
  TMR3CN0  = 0x00;                    // Resets Timer 3, sets to 16 bit mode
  CKCON0  |= 0x40;                    // Use system clock
  TMR3RL   = -counts;                 // Initial reload value

  TMR3    = 0xffff;                   // Sets timer to reload automatically
  EIE1   |= 0x80;                     // Enable Timer 3 interrupts
  TMR3CN0 = 0x04;                     // Start Timer 3
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
// Main Routine
//-----------------------------------------------------------------------------
void main (void)
{
  enter_DefaultMode_from_RESET();

  Timer3_Init(SYSCLK/SAMPLE_RATE_DAC);

  IE_EA = 1;                          // Enable global interrupts

  while (1);
}


