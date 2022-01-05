//-----------------------------------------------------------------------------
// EFM8BB3_DAC_Lib.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program outputs a sine wave using DAC0, DAC1, DAC2 and DAC3. The DAC
// outputs are scheduled to update at a rate determined by the overflow
// frequency of Timer 4, configured within Hardware Configurator.
//
// Implements a 256-entry full-cycle sine table of 12-bit precision.
//
// The output frequency is determined by a 16-bit phase adder.
// At each DAC update cycle, the phase adder value is added to a running
// phase accumulator, <phase_accumulator>, the upper bits of which are used
// to access the sine lookup table.
//
// Additionally, push-buttons PB0 and PB1 are set to hold DAC0/DAC1 and
// DAC2/DAC3 outputs, respectively, when pressed.
//
// Resources:
//   SYSCLK - 49 MHz HFOSC0 / 1
//   DAC0   - 1 kHz sine wave @ 0 degrees
//   DAC1   - 1 kHz sine wave + 180 degrees
//   DAC2   - 1 kHz sine wave + 90 degrees
//   DAC3   - 1 kHz sine wave + 270 degrees
//   Timer4 - 256 kHz interrupt / DAC update source
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB3 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB3 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8BB3 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Use an oscilloscope to observe DAC0, DAC1, DAC2 and DAC3 output on
//    P3.0, P3.1, P3.2 and P3.3 respectively. Verify that there is a sine wave
//    with a peak-to-peak amplitude of VDD.
// 6) Press PB0 to hold DAC0 and DAC1 outputs.
// 7) Press PB1 to hold DAC2 and DAC3 outputs.
//
// Target:         EFM8BB3
// Tool chain:     Generic
//
// Release 0.1 (BL)
//    - Initial Revision
//    - 10 AUG 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB3_Register_Enums.h>
#include <InitDevice.h>
#include "dac_0.h"
#include "dac_1.h"
#include "dac_2.h"
#include "dac_3.h"
//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
// A full cycle, 12-bit, unsigned sine wave lookup table
SI_SEGMENT_VARIABLE(SINE_TABLE[256], uint16_t, const SI_SEG_CODE) = 
{
   0x7FF, 0x831, 0x863, 0x895, 0x8C7, 0x8F9, 0x92B, 0x95C,
   0x98E, 0x9BF, 0x9F0, 0xA20, 0xA51, 0xA81, 0xAB0, 0xADF,
   0xB0E, 0xB3C, 0xB6A, 0xB97, 0xBC3, 0xBEF, 0xC1B, 0xC46,
   0xC70, 0xC99, 0xCC2, 0xCEA, 0xD11, 0xD38, 0xD5D, 0xD82,
   0xDA6, 0xDC9, 0xDEB, 0xE0D, 0xE2D, 0xE4C, 0xE6B, 0xE88,
   0xEA5, 0xEC0, 0xEDA, 0xEF4, 0xF0C, 0xF23, 0xF39, 0xF4E,
   0xF62, 0xF74, 0xF86, 0xF96, 0xFA5, 0xFB3, 0xFC0, 0xFCC,
   0xFD6, 0xFDF, 0xFE7, 0xFEE, 0xFF4, 0xFF8, 0xFFB, 0xFFD,
   0xFFE, 0xFFD, 0xFFB, 0xFF8, 0xFF4, 0xFEE, 0xFE7, 0xFDF,
   0xFD6, 0xFCC, 0xFC0, 0xFB3, 0xFA5, 0xF96, 0xF86, 0xF74,
   0xF62, 0xF4E, 0xF39, 0xF23, 0xF0C, 0xEF4, 0xEDA, 0xEC0,
   0xEA5, 0xE88, 0xE6B, 0xE4C, 0xE2D, 0xE0D, 0xDEB, 0xDC9,
   0xDA6, 0xD82, 0xD5D, 0xD38, 0xD11, 0xCEA, 0xCC2, 0xC99,
   0xC70, 0xC46, 0xC1B, 0xBEF, 0xBC3, 0xB97, 0xB6A, 0xB3C,
   0xB0E, 0xADF, 0xAB0, 0xA81, 0xA51, 0xA20, 0x9F0, 0x9BF,
   0x98E, 0x95C, 0x92B, 0x8F9, 0x8C7, 0x895, 0x863, 0x831,
   0x7FF, 0x7CC, 0x79A, 0x768, 0x736, 0x704, 0x6D2, 0x6A1,
   0x66F, 0x63E, 0x60D, 0x5DD, 0x5AC, 0x57C, 0x54D, 0x51E,
   0x4EF, 0x4C1, 0x493, 0x466, 0x43A, 0x40E, 0x3E2, 0x3B7,
   0x38D, 0x364, 0x33B, 0x313, 0x2EC, 0x2C5, 0x2A0, 0x27B,
   0x257, 0x234, 0x212, 0x1F0, 0x1D0, 0x1B1, 0x192, 0x175,
   0x158, 0x13D, 0x123, 0x109, 0x0F1, 0x0DA, 0x0C4, 0x0AF,
   0x09B, 0x089, 0x077, 0x067, 0x058, 0x04A, 0x03D, 0x031,
   0x027, 0x01E, 0x016, 0x00F, 0x009, 0x005, 0x002, 0x001,
   0x000, 0x001, 0x002, 0x005, 0x009, 0x00F, 0x016, 0x01E,
   0x027, 0x031, 0x03D, 0x04A, 0x058, 0x067, 0x077, 0x089,
   0x09B, 0x0AF, 0x0C4, 0x0DA, 0x0F1, 0x109, 0x123, 0x13D,
   0x158, 0x175, 0x192, 0x1B1, 0x1D0, 0x1F0, 0x212, 0x234,
   0x257, 0x27B, 0x2A0, 0x2C5, 0x2EC, 0x313, 0x33B, 0x364,
   0x38D, 0x3B7, 0x3E2, 0x40E, 0x43A, 0x466, 0x493, 0x4C1,
   0x4EF, 0x51E, 0x54D, 0x57C, 0x5AC, 0x5DD, 0x60D, 0x63E,
   0x66F, 0x6A1, 0x6D2, 0x704, 0x736, 0x768, 0x79A, 0x7CC,
};

#define SINE_TABLE_LENGTH       (sizeof(SINE_TABLE)/sizeof(SINE_TABLE[0]))

// DAC0 at 0 degrees
#define DAC0_STARTING_POSITION  (0)
// DAC1 at 180 degrees
#define DAC1_STARTING_POSITION  (SINE_TABLE_LENGTH/2)
// DAC2 at 90 degrees
#define DAC2_STARTING_POSITION  (SINE_TABLE_LENGTH/4)
// DAC3 at 270 degrees
#define DAC3_STARTING_POSITION  ((SINE_TABLE_LENGTH/4) * 3)

#define SAMPLE_RATE_DAC 44000UL        // DAC sampling rate in Hz

#define PHASE_PRECISION 65536UL          // Range of phase accumulator

#define FREQUENCY       1000UL          // Frequency of output in Hz

#define PHASE_ADD       (FREQUENCY * PHASE_PRECISION / SAMPLE_RATE_DAC)

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P3, 4);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (PB0, SFR_P0, 2);              // Button PB0
SI_SBIT (PB1, SFR_P0, 3);              // Button PB1
#define PB_PRESSED  0                  // 0 = Button pressed
#define PB_RELEASED 1                  // 1 = Button released

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
    PCON0 |= PCON0_IDLE__IDLE;     // Idle the CPU while waiting for the next
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
// Checks the state of the buttons, hold the appropriate DAC outputs.
//
// If PB0 is pressed, this function will hold the output voltage of DAC0 and
// DAC1. If PB1 is pressed, DAC2 and DAC3 will be held.
//
//-----------------------------------------------------------------------------
void HandleButtonPresses(void)
{
  if (PB0 == PB_PRESSED)
  {
    DAC0_holdOutput(true);
    DAC1_holdOutput(true);
  }
  else
  {
    DAC0_holdOutput(false);
    DAC1_holdOutput(false);
  }

  if (PB1 == PB_PRESSED)
  {
    DAC2_holdOutput(true);
    DAC3_holdOutput(true);
  }
  else
  {
    DAC2_holdOutput(false);
    DAC3_holdOutput(false);
  }
}

//-----------------------------------------------------------------------------
// TIMER4_ISR
//-----------------------------------------------------------------------------
//
// Updates the output values for DAC0 and DAC2.
//
// A Timer 4 overflow triggers an update of the DAC0-3 outputs, the interrupt
// will load the output value that will take effect on the next Timer 4
// overflow.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER4_ISR, TIMER4_IRQn)
{
  // Holds phase accumulator for each DAC
  static uint16_t dac0_phase_acc = DAC0_STARTING_POSITION << 8;
  static uint16_t dac1_phase_acc = DAC1_STARTING_POSITION << 8;
  static uint16_t dac2_phase_acc = DAC2_STARTING_POSITION << 8;
  static uint16_t dac3_phase_acc = DAC3_STARTING_POSITION << 8;

  // Clear ISR flag
  TMR4CN0_TF4H = 0;

  DAC0_setOutput(SINE_TABLE[dac0_phase_acc >> 8]);
  DAC1_setOutput(SINE_TABLE[dac1_phase_acc >> 8]);
  DAC2_setOutput(SINE_TABLE[dac2_phase_acc >> 8]);
  DAC3_setOutput(SINE_TABLE[dac3_phase_acc >> 8]);

  dac0_phase_acc += PHASE_ADD; // Increment phase accumulators
  dac1_phase_acc += PHASE_ADD;
  dac2_phase_acc += PHASE_ADD;
  dac3_phase_acc += PHASE_ADD;
}


