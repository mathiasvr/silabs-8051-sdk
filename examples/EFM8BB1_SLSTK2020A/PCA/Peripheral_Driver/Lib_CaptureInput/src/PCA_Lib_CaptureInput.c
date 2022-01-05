//-----------------------------------------------------------------------------
// PCA_Lib_CaptureInput.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program uses the PCA's capture mode to time events on an input pin.
// The difference between subsequent captures is calculated and stored in
// memory.
//
// In this example, PCA Module 0 is used to capture rising edges of an
// incoming digital waveform, and the crossbar is configured to receive the
// CEX0 pin on on P1.2. The PCA is configured to clock once every 10 us.
//
// Timer2 overflows are used to toggle pin P1.3 at an overflow rate specified
// by the definition T2_RELOAD_CLOCKS. This signal at P1.3 is provided simply
// as a test method for the code.
//
// The PCA will capture rising edges on the CEX0 pin, and interrupt the
// processor.  Software subtracts the previous capture value from the most
// recent capture value to set the "captured_clocks" variable.
//
// The PCA time base in this example is configured to use Timer 0, overflowing
// every 10 us. The PCA clock rate will determine the maximum time between
// edges that can be reliably detected (this is equal to 65535 * the PCA clock).
//
// The time range that can be measured using this example is 10us (MIN) to
// 655.350 ms (MAX) when the clocks are set up as they are in this example.
// The precision is limited to the PCA clock rate (in this example, 10 us).
// Using different PCA clock sources or a different processor clock will allow
// for different time capture ranges or levels of precision.
//
//    How "Capture Mode" Works:
//
//    The PCA's Capture Mode works by making a copy of the current PCA timer
//    value (PCA0) into the capture/compare register for the module (PCA0CPn).
//    The module can be configured to capture rising edges, falling edges, or
//    both rising and falling edges.  When a capture occurs, the CCFn flag is
//    set, and an interrupt is (optionally) generated by the PCA module.
//
//    To detect an edge (and not reject it as noise) the new logic level must
//    be present at the pin for at least two SYSCLK cycles.
//
//    With a constantly-running PCA timer, the time between two edges on a
//    signal can be accurately measured to within 1 PCA clock cycle.
//    Multiple PCA channels can be used in capture mode to measure timing
//    between different signals.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC / 1
//   PCA    - Capture CEX0 positive edge
//   Timer0 - PCA time base
//   Timer2 - Test output
//   P0.0   - Display enable
//   P1.2   - PCA CEX0
//   P1.3   - Timer2 test output
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 STK
//-----------------------------------------------------------------------------
// 1) Short P1.2 and P1.3 together
// 2) Place the switch in "AEM" mode.
// 3) Connect the EFM8BB1 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8BB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Set a breakpoint in the PCA0_ISR function.
// 6) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 7) Verify that the variable "capture_period" is equal to 50 (=500us).
//
// Alternately:
//
// 1) Run a bench signal generator into the P1.2 pin.
// 2) Place the switch in "AEM" mode.
// 3) Connect the EFM8BB1 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8BB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Set a breakpoint in the PCA0_ISR function.
// 6) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 7) Verify that the variable "capture_period" matches the expected time
//    between rising edges (in units of 10us).
//
//
// Target:         EFM8BB1
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
#include "pca_0.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT(TEST_OUT, SFR_P1, 3);         // Output for testing purposes

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define T0_CLOCKS         245          // Use 245 clocks per T0 Overflow
//    (245 = 10 us)
#define T2_RELOAD_CLOCKS 6125          // SYSCLKs per 1/2 cycle square wave
//    (245 = 10 us)

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
  static uint16_t current_capture_value, previous_capture_value;
  static uint16_t capture_period;

  // Store most recent capture value
  current_capture_value = PCA0_readChannel(PCA0_CHAN0);

  // Calculate capture period from last two values.
  capture_period = current_capture_value - previous_capture_value;

  // Update previous capture value with most recent info.
  previous_capture_value = current_capture_value;
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

  BSP_DISP_EN = 0;           // Display not driven by EFM8

  IE_EA = 1;

  while (1)
  {
    // check to see if Timer2 overflowed
    if (TMR2CN0_TF2H)
    {
      TEST_OUT = ~TEST_OUT;         // Toggle test pin
      TMR2CN0_TF2H = 0;             // Clear T2 overflow
    }
  }
}
