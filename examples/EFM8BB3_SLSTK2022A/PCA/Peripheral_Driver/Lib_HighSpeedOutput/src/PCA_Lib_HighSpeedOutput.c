//-----------------------------------------------------------------------------
// PCA_Lib_HighSpeedOutput.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program sends a square wave out on an I/O pin, using the PCA's
// High-Speed Output Mode.
//
// In this example, PCA Module 0 is used to generate the waveform, and the
// crossbar is configured to send the CEX0 pin out on P1.4.
//
// The frequency of the square wave generated at the CEX0 pin is defined by
// the parameter CEX0_FREQUENCY.
//
// The maximum square wave frequency output for this example is about 230 kHz.
// The minimum square wave frequency output for this example is about 190 Hz
//
// The PCA time base in this example is configured to use SYSCLK, and SYSCLK
// is set up to use the internal oscillator running at 24.5 MHz.
// Using different PCA clock sources or a different processor clock will
// result in a different frequency for the square wave, and different
// maximum and minimum options.
//
//    How "High Speed Output Mode" Works:
//
//    The PCA's High Speed Output Mode works by toggling the output pin
//    associated with the module every time the PCA0 register increments and
//    the new 16-bit PCA0 counter value matches the module's capture/compare
//    register (PCA0CPn). When initially enabled in high-speed output mode, the
//    CEXn pin associated with the module will be at a logic high state.  The
//    first match will cause a falling edge on the pin.  The next match will
//    cause a rising edge on the pin, and so on.
//
//    By loading the PCA0CPn register with the next match value every time a
//    match happens, arbitrary waveforms can be generated on the pin with high
//    levels of precision.
//
//  When setting the capture/compare register for the next edge value, the low
//  byte of the PCA0CPn register (PCA0CPLn) should be written first, followed
//  by the high byte (PCA0CPHn).  Writing the low byte clears the ECOMn bit,
//  and writing the high byte will restore it.  This ensures that a match does
//  not occur until the full 16-bit value has been written to the compare
//  register.  Writing the high byte first will result in the ECOMn bit being
//  set to '0' after the 16-bit write, and the next match will not occur at
//  the correct time.
//
//  It is best to update the capture/compare register as soon after a match
//  occurs as possible so that the PCA counter will not have incremented past
//  the next desired edge value. This code implements the compare register
//  update in the PCA ISR upon a match interrupt.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC / 1
//   PCA    - High Speed Output
//   P2.3   - Display enable
//   P1.4   - PCA CEX0 / LED green
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
// 5) Place an oscilloscope probe on P1.4.
//    The frequency is determined by CEX0_FREQUENCY, default 10kHz.
//
// Target:         EFM8BB3
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 10 OCT 2014
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "bsp.h"
#include "PCA_Lib_HighSpeedOutput.h"
#include "InitDevice.h"
#include "pca_0.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
uint16_t Next_Compare_Value;           // Next edge to be sent out in HSO mode

//-----------------------------------------------------------------------------
// Callbacks
//-----------------------------------------------------------------------------

void PCA0_overflowCb()
{

}

void PCA0_intermediateOverflowCb()
{
}

void PCA0_channel0EventCb()
{
  PCA0_writeChannel(PCA0_CHAN0, Next_Compare_Value);

  // Set up the variable for the following edge
  Next_Compare_Value = PCA0_readChannel(PCA0_CHAN0) + DIVIDE_RATIO;
}

void PCA0_channel1EventCb()
{

}

void PCA0_channel2EventCb()
{

}

void PCA0_channel3EventCb()
{

}

void PCA0_channel4EventCb()
{

}

void PCA0_channel5EventCb()
{

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
void main(void)
{
  enter_DefaultMode_from_RESET();

  BSP_DISP_EN = 0;                // Display not driven by EFM8

  // Set up the variable for the following match
  Next_Compare_Value = PCA0_readChannel(PCA0_CHAN0) + DIVIDE_RATIO;

  // Spin here to wait for ISR
  while (1);
}
