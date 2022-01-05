//-----------------------------------------------------------------------------
// EFM8BB3_PCA_8bitCenterPWM.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program sends two center-aligned PWM waveforms out of two I/O pins
// using the PCA's 8-bit Center-Aligned PWM Output Mode.  The duty cycles of
// the waveforms are modified periodically within the main loop, and the
// waveforms are increasing/decreasing opposite of each other.
//
// In this example, PCA Modules 0 and 1 are used to generate the waveforms, and
// the crossbar is configured to send the CEX0 signal out on P1.4 and the CEX1
// signal out on P1.5.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   PCA    - 8-bit PWM
//   P1.4   - PCA CEX0
//   P1.5   - PCA CEX1
//   P3.4   - Display enable
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
// 5) Place an oscilloscope probe on P1.4 and P1.5.
// 6) Verify that the duty cycle of the waveforms vary smoothly between
//    minimum and maximum values.
//
// Target:         EFM8BB3
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 20 MAY 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB3_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P3, 4);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

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
   uint16_t delay_count;               // Used to implement a delay
   bool duty_direction0 = 0;            // Module 0: 0 = Decrease; 1 = Increase
   bool duty_direction1 = 0;            // Module 1: 0 = Increase; 1 = Decrease

   enter_DefaultMode_from_RESET();

   DISP_EN = DISP_BC_DRIVEN;           // EFM8 does not drive display

   while (1)
   {
      // Wait
      for (delay_count = 30000; delay_count > 0; delay_count--);

      // Module 0
      if (duty_direction0 == 1)        // Direction = Increase
      {
         // First, check the ECOM0 bit
         if ((PCA0CPM0 & PCA0CPM0_ECOM__BMASK) == PCA0CPM0_ECOM__DISABLED)
         {
            PCA0CPM0 |= PCA0CPM0_ECOM__BMASK; // Set ECOM0 if it is '0'
         }
         else                          // Increase duty cycle otherwise
         {
            PCA0CPH0--;                // Increase duty cycle

            if (PCA0CPH0 == 0x00)
            {
               duty_direction0 = 0;    // Change direction for next time
            }
         }
      }
      else                             // Direction = Decrease
      {
         if (PCA0CPH0 == 0xFF)
         {
            PCA0CPM0 &= ~PCA0CPM0_ECOM__BMASK; // Clear ECOM0
            duty_direction0 = 1;       // Change direction for next time
         }
         else
         {
            PCA0CPH0++;                // Decrease duty cycle
         }
      }

      // Module 1
      if (duty_direction1 == 1)        // Direction = Decrease
      {
         // First, check the ECOM1 bit
         if ((PCA0CPM1 & PCA0CPM1_ECOM__BMASK) == PCA0CPM1_ECOM__DISABLED)
         {
            PCA0CPM1 |= PCA0CPM1_ECOM__BMASK; // Set ECOM1 if it is '0'
         }
         else                          // Increase duty cycle otherwise
         {
            PCA0CPH1++;                // Decrease duty cycle

            if (PCA0CPH1 == 0xFF)
            {
               duty_direction1 = 0;    // Change direction for next time
            }
         }
      }
      else                             // Direction = Increase
      {
         if (PCA0CPH1 == 0x00)
         {
            PCA0CPM1 &= ~PCA0CPM1_ECOM__BMASK; // Clear ECOM1
            duty_direction1 = 1;       // Change direction for next time
         }
         else
         {
            PCA0CPH1--;                // Decrease duty cycle
         }
      }
   }
}

