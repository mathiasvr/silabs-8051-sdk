//-----------------------------------------------------------------------------
// EFM8BB52_DAC_Lib.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program outputs a sine wave using DAC0. The DAC outputs are scheduled
// to update at a rate determined by the overflow frequency of Timer 4,
// configured within Hardware Configurator.
//
// Implements a 256-entry full-cycle sine table of 10-bit precision.
//
// The output frequency is determined by a 16-bit phase adder.
// At each DAC update cycle, the phase adder value is added to a running
// phase accumulator, <phase_accumulator>, the upper bits of which are used
// to access the sine lookup table.
//
// Additionally, push-button PB0 is set to hold DAC0 output when pressed.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   DAC0   - 1 kHz sine wave @ 0 degrees
//   Timer4 - 128 kHz interrupt / DAC update source
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
// 5) Use an oscilloscope to observe DAC0 output on P3.0. Verify that there
//    is a sine wave with a peak-to-peak amplitude of VDD.
// 6) Press PB0 to hold DAC0 output.
//
// Target:         EFM8BB52
// Tool chain:     Generic
//
// Release 0.1 (AG)
//    - Initial Revision
//    - JAN 2021
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB52_Register_Enums.h>
#include <InitDevice.h>
#include "dac_0.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
// A full cycle, 10-bit, unsigned sine wave lookup table
SI_SEGMENT_VARIABLE(SINE_TABLE[256], uint16_t, const SI_SEG_CODE) = 
{
  0x200,0x20C,0x219,0x225,0x232,0x23E,0x24B,0x257,
  0x263,0x270,0x27C,0x288,0x294,0x2A0,0x2AC,0x2B8,
  0x2C3,0x2CF,0x2DA,0x2E5,0x2F1,0x2FC,0x306,0x311,
  0x31C,0x326,0x330,0x33A,0x344,0x34E,0x357,0x360,
  0x369,0x372,0x37A,0x383,0x38B,0x393,0x39A,0x3A2,
  0x3A9,0x3B0,0x3B6,0x3BD,0x3C3,0x3C8,0x3CE,0x3D3,
  0x3D8,0x3DD,0x3E1,0x3E5,0x3E9,0x3EC,0x3F0,0x3F3,
  0x3F5,0x3F7,0x3F9,0x3FB,0x3FD,0x3FE,0x3FE,0x3FF,
  0x3FF,0x3FF,0x3FE,0x3FE,0x3FD,0x3FB,0x3F9,0x3F7,
  0x3F5,0x3F3,0x3F0,0x3EC,0x3E9,0x3E5,0x3E1,0x3DD,
  0x3D8,0x3D3,0x3CE,0x3C8,0x3C3,0x3BD,0x3B6,0x3B0,
  0x3A9,0x3A2,0x39A,0x393,0x38B,0x383,0x37A,0x372,
  0x369,0x360,0x357,0x34E,0x344,0x33A,0x330,0x326,
  0x31C,0x311,0x306,0x2FC,0x2F1,0x2E5,0x2DA,0x2CF,
  0x2C3,0x2B8,0x2AC,0x2A0,0x294,0x288,0x27C,0x270,
  0x263,0x257,0x24B,0x23E,0x232,0x225,0x219,0x20C,
  0x200,0x1F3,0x1E6,0x1DA,0x1CD,0x1C1,0x1B4,0x1A8,
  0x19C,0x18F,0x183,0x177,0x16B,0x15F,0x153,0x147,
  0x13C,0x130,0x125,0x11A,0x10E,0x103,0x0F9,0x0EE,
  0x0E3,0x0D9,0x0CF,0x0C5,0x0BB,0x0B1,0x0A8,0x09F,
  0x096,0x08D,0x085,0x07C,0x074,0x06C,0x065,0x05D,
  0x056,0x04F,0x049,0x042,0x03C,0x037,0x031,0x02C,
  0x027,0x022,0x01E,0x01A,0x016,0x013,0x00F,0x00C,
  0x00A,0x008,0x006,0x004,0x002,0x001,0x001,0x000,
  0x000,0x000,0x001,0x001,0x002,0x004,0x006,0x008,
  0x00A,0x00C,0x00F,0x013,0x016,0x01A,0x01E,0x022,
  0x027,0x02C,0x031,0x037,0x03C,0x042,0x049,0x04F,
  0x056,0x05D,0x065,0x06C,0x074,0x07C,0x085,0x08D,
  0x096,0x09F,0x0A8,0x0B1,0x0BB,0x0C5,0x0CF,0x0D9,
  0x0E3,0x0EE,0x0F9,0x103,0x10E,0x11A,0x125,0x130,
  0x13C,0x147,0x153,0x15F,0x16B,0x177,0x183,0x18F,
  0x19C,0x1A8,0x1B4,0x1C1,0x1CD,0x1DA,0x1E6,0x1F3,
};

#define SINE_TABLE_LENGTH       (sizeof(SINE_TABLE)/sizeof(SINE_TABLE[0]))

// DAC0 at 0 degrees
#define DAC0_STARTING_POSITION  (0)

#define SAMPLE_RATE_DAC         128000UL   // DAC sampling rate in Hz

#define PHASE_PRECISION         65536UL    // Range of phase accumulator

#define FREQUENCY               1000UL     // Frequency of output in Hz

#define PHASE_ADD       (FREQUENCY * PHASE_PRECISION / SAMPLE_RATE_DAC)

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P3, 4);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (PB0, SFR_P0, 2);              // Button PB0
#define PB_PRESSED  0                  // 0 = Button pressed
#define PB_RELEASED 1                  // 1 = Button released

//-----------------------------------------------------------------------------
// Local Variables
//-----------------------------------------------------------------------------
static bool holdOutput = false;        // Local variable for tracking PB0 state
                                       //  and holding DAC output
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void HandleButtonPresses(void);

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

  DISP_EN = DISP_BC_DRIVEN;        // Display not driven by EFM8

  IE_EA = 1;                       // Enable global interrupts

  while (1)
  {
    PCON0 |= 0x1;                  // Idle the CPU while waiting for the next
                                   // Interrupt

    HandleButtonPresses();
  }
}

///////////////////////////////////////////////////////////////////////////////
// Supporting Functions
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// HandleButtonPresses
//-----------------------------------------------------------------------------
//
// Checks the state of the PB0 button, hold the DAC output.
//
// If PB0 is pressed, this function will hold enable holding the output voltage
// of DAC0.
//
//-----------------------------------------------------------------------------
void HandleButtonPresses(void)
{
  if (PB0 == PB_PRESSED)
  {
    holdOutput = true;
  }
  else
  {
    holdOutput = false;
  }
}

//-----------------------------------------------------------------------------
// TIMER4_ISR
//-----------------------------------------------------------------------------
//
// Updates the output value for DAC0.
//
// A Timer 4 overflow triggers an update of the DAC0 output, the interrupt
// will load the output value that will take effect on the next Timer 4
// overflow. Only updates DAC output if PB0 is not being pressed.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER4_ISR, TIMER4_IRQn)
{
  // Holds phase accumulator for each DAC
  static uint16_t dac0_phase_acc = DAC0_STARTING_POSITION << 8;

  // Clear ISR flag
  TMR4CN0_TF4H = 0;

  if (holdOutput == false) // update DAC output
    DAC0_setOutput(SINE_TABLE[dac0_phase_acc >> 8]);

  dac0_phase_acc += PHASE_ADD; // Increment phase accumulators
}
