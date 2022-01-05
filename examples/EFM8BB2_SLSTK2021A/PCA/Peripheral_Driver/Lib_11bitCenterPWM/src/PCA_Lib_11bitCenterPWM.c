//-----------------------------------------------------------------------------
// PCA_Lib_11bitCenterPWM.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program sends one center-aligned PWM waveform out of one I/O pin
// using the PCA's 11-bit Center-Aligned PWM Output Mode. The duty cycle of
// the waveform is modified periodically within the main loop.
//
// In this example, PCA Module 0 is used to generate the waveform, and
// the crossbar is configured to send the CEX0 signal out on P1.4.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC / 1
//   PCA    - 11-bit PWM
//   P2.3   - Display enable
//   P1.4   - PCA CEX0 / LED green
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB2 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8BB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Place an oscilloscope probe on P1.4.
// 6) Verify that the duty cycle of the waveform vary smoothly between
//    minimum and maximum values.
//
// Target:         EFM8BB2
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
// Global Variables
//-----------------------------------------------------------------------------
uint16_t CEX0_Compare_Value;          // Holds current PCA compare value

//-----------------------------------------------------------------------------
// Callbacks
//-----------------------------------------------------------------------------

void PCA0_overflowCb()
{

}

void PCA0_intermediateOverflowCb()
{
  // Target Auto-reload registers
  PCA0PWM |= PCA0PWM_ARSEL__AUTORELOAD;

  PCA0_writeChannel(PCA0_CHAN0, CEX0_Compare_Value);

  // Target PCA0CPH/L registers
  PCA0PWM &= ~PCA0PWM_ARSEL__AUTORELOAD;
}

void PCA0_channel0EventCb()
{

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
  uint16_t delay_count;                     // Used to implement a delay
  bool duty_direction = 0;                   // 0 = Decrease; 1 = Increase

  enter_DefaultMode_from_RESET();

  BSP_DISP_EN = 0;                          // Display not driven by EFM8

  // Configure initial PWM duty cycle = 50%
  CEX0_Compare_Value = 2048 - (2048 * 0.5);

  while (1)
  {
    // Wait a little while
    for (delay_count = 1200; delay_count > 0; delay_count--);

    if (duty_direction == 1)              // Direction = Increase
    {
      // First, check the ECOM0 bit
      if ((PCA0CPM0 & PCA0CPM0_ECOM__BMASK) == PCA0CPM0_ECOM__DISABLED)
      {
        PCA0CPM0 |= PCA0CPM0_ECOM__BMASK; // Set ECOM0 if it is '0'
      }
      else                                // Increase duty cycle otherwise
      {
        CEX0_Compare_Value--;             // Increase duty cycle

        if (CEX0_Compare_Value == 0x0000)
        {
          duty_direction = 0;             // Change direction for next time
        }
      }
    }
    else                                  // Direction = Decrease
    {
      if (CEX0_Compare_Value == 0x07FF)
      {
        PCA0CPM0 &= ~PCA0CPM0_ECOM__BMASK;// Clear ECOM0
        duty_direction = 1;               // Change direction for next time
      }
      else
      {
        CEX0_Compare_Value++;             // Decrease duty cycle
      }
    }
  }
}
