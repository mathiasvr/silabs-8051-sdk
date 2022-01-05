//-----------------------------------------------------------------------------
// PCA_Lib_8bitCenterPWM.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program sends two center-aligned PWM waveforms out of two I/O pins
// using the PCA's 8-bit Center-Aligned PWM Output Mode. The duty cycles of
// the waveforms are modified periodically within the main loop, and the
// waveforms are increasing/decreasing opposite of each other.
//
// In this example, PCA Modules 0 and 1 are used to generate the waveforms, and
// the crossbar is configured to send the CEX0 signal out on P1.4 and the CEX1
// signal out on P1.5.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC / 1
//   PCA    - 8-bit PWM
//   P1.4   - PCA CEX0 / LED
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 LCK
//-----------------------------------------------------------------------------
// 1) Connect the EFM8BB1 LCK board to a PC using a micro USB cable.
// 2) Compile and download code to the EFM8BB1 LCK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 3) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 4) Place an oscilloscope probe on P1.4 and P1.5.
// 5) Verify that the duty cycle of the waveforms vary smoothly between
//    minimum and maximum values.
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
  uint16_t delay_count;             // Used to implement a delay
  bool duty_direction0 = 0;          // Module 0: 0 = Decrease; 1 = Increase
  bool duty_direction1 = 0;          // Module 1: 0 = Increase; 1 = Decrease

  uint8_t duty_cycle0 = 0x80;
  uint8_t duty_cycle1 = 0xFF;

  enter_DefaultMode_from_RESET();

  while (1)
  {
    // Wait
    for (delay_count = 30000; delay_count > 0; delay_count--);

    // Module 0
    if (duty_direction0 == 1)               // Direction = Increase
    {
      // First, check the ECOM0 bit
      if ((PCA0CPM0 & PCA0CPM0_ECOM__BMASK) == PCA0CPM0_ECOM__DISABLED)
      {
        PCA0CPM0 |= PCA0CPM0_ECOM__BMASK;   // Set ECOM0 if it is '0'
      }
      else                                  // Increase duty cycle otherwise
      {
        duty_cycle0--;                      // Increase duty cycle

        PCA0_writeChannel(PCA0_CHAN0, duty_cycle0 << 8);

        if (duty_cycle0 == 0x00)
        {
          duty_direction0 = 0;              // Change direction for next time
        }
      }
    }
    else                                    // Direction = Decrease
    {
      if (duty_cycle0 == 0xFF)
      {
        PCA0CPM0 &= ~PCA0CPM0_ECOM__BMASK;  // Clear ECOM0
        duty_direction0 = 1;                // Change direction for next time
      }
      else
      {
        duty_cycle0++;                      // Decrease duty cycle

        PCA0_writeChannel(PCA0_CHAN0, duty_cycle0 << 8);
      }
    }

    // Module 1
    if (duty_direction1 == 1)               // Direction = Decrease
    {
      // First, check the ECOM1 bit
      if ((PCA0CPM1 & PCA0CPM1_ECOM__BMASK) == PCA0CPM1_ECOM__DISABLED)
      {
        PCA0CPM1 |= PCA0CPM1_ECOM__BMASK;   // Set ECOM1 if it is '0'
      }
      else                                  // Increase duty cycle otherwise
      {
        duty_cycle1++;                      // Decrease duty cycle

        PCA0_writeChannel(PCA0_CHAN1, duty_cycle1 << 8);

        if (duty_cycle1 == 0xFF)
        {
          duty_direction1 = 0;              // Change direction for next time
        }
      }
    }
    else                                    // Direction = Increase
    {
      if (duty_cycle1 == 0x00)
      {
        PCA0CPM1 &= ~PCA0CPM1_ECOM__BMASK;  // Clear ECOM1
        duty_direction1 = 1;                // Change direction for next time
      }
      else
      {
        duty_cycle1--;                      // Increase duty cycle

        PCA0_writeChannel(PCA0_CHAN1, duty_cycle1 << 8);
      }
    }
  }
}
