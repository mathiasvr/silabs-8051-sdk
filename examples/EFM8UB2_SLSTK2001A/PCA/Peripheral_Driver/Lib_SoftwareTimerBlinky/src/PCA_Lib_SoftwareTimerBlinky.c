//-----------------------------------------------------------------------------
// PCA_Lib_SoftwareTimerBlinky.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program uses the PCA's Software Timer Mode to periodically schedule
// an interrupt.  Inside the ISR, a port pin is toggled to blink an LED on the
// board.
//
// In this example, PCA Module 0 is used to generate the interrupt, and the
// port pin driving the LED is configured for push-pull mode.
//
// The PCA uses Timer0 Overflows as a clock source. Timer0 is configured
// for 8-bit auto-reload mode, where T0_CLOCKS is the number of System clocks
// per T0 Overflow. The code then sets up Software Timer mode for Module 0
// (CEX0 pin).
//
// On every interrupt from Module 0, software toggles the port I/O pin for the
// LED. The frequency of the LED toggling is determined by the parameter
// CEX0_FREQUENCY.
//
// The maximum frequency output for this example is about 50 kHz.
// The minimum frequency output for this example is about 1 Hz
//
// The PCA time base in this example is configured to use T0 overflows, and
// SYSCLK is set up for the internal oscillator running at 24 MHz.
// Using different PCA clock sources, different T0 reload values, or a
// different processor clock will result in a different frequency for the LED
// square wave, and different maximum and minimum options.
//
//    How "Software Timer Mode" Works:
//
//    The PCA's Software Timer Mode works by generating an interrupt for the
//    associated module every time the PCA0 register increments and the new
//    16-bit PCA0 counter value matches the module's capture/compare
//    register (PCA0CPn).
//
//    By loading the PCA0CPn register with the next match value every time a
//    match happens, arbitrarily timed interrupts can be generated.
//
// When setting the capture/compare register for the next match value, the low
// byte of the PCA0CPn register (PCA0CPLn) should be written first, followed
// by the high byte (PCA0CPHn).  Writing the low byte clears the ECOMn bit,
// and writing the high byte will restore it.  This ensures that a match does
// not occur until the full 16-bit value has been written to the compare
// register.  Writing the high byte first will result in the ECOMn bit being
// set to '0' after the 16-bit write, and the next match will not occur at
// the correct time.
//
// It is best to update the capture/compare register as soon after a match
// occurs as possible so that the PCA counter will not have incremented past
// the next desired edge value. This code implements the compare register
// update in the PCA ISR upon a match interrupt.
//
// Resources:
//   SYSCLK - 24 MHz HFOSC / 1
//   PCA    - Software Timer Mode
//   Timer0 - PCA clock source
//   P1.4   - Display enable
//   P1.6   - LED green
//
//-----------------------------------------------------------------------------
// How To Test: EFM8UB2 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8UB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8UB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) The green LED should blink at 5 Hz.
//
// Target:         EFM8UB2
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
#include "InitDevice.h"
#include "PCA_Lib_SoftwareTimerBlinky.h"
#include "pca_0.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
uint16_t Next_Compare_Value;           // Next edge to be sent out

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

  // Invert the LED pin
  BSP_LED_G = ~BSP_LED_G;

  // Set up the variable for the following edge
  Next_Compare_Value = PCA0_readChannel(PCA0_CHAN0) + PCA_TIMEOUT;
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

  // Display not driven by EFM8
  BSP_DISP_EN = 0;

  // Set up the variable for the following match
  Next_Compare_Value = PCA0_readChannel(PCA0_CHAN0) + PCA_TIMEOUT;

  // Spin here to wait for ISR
  while (1);
}
