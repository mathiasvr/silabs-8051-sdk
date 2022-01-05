//-----------------------------------------------------------------------------
// EFM8LB1_PCA_11bitCenterPWM.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program sends one center-aligned PWM waveform out of one I/O pin
// using the PCA's 11-bit Center-Aligned PWM Output Mode.  The duty cycle of
// the waveform is modified periodically within the main loop.
//
// In this example, PCA Module 0 is used to generate the waveform, and
// the crossbar is configured to send the CEX0 signal out on P1.4.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   PCA    - 11-bit PWM
//   P1.4   - PCA CEX0 / LED green
//   P3.4   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8LB1 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8LB1 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8LB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Place an oscilloscope probe on P1.4.
// 6) Verify that the duty cycle of the waveform vary smoothly between
//    minimum and maximum values.
//
// Target:         EFM8LB1
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 06 MAR 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8LB1_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P3, 4);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
extern uint16_t CEX0_Compare_Value;    // Holds current PCA compare value

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
   bool duty_direction = 0;             // 0 = Decrease; 1 = Increase

   enter_DefaultMode_from_RESET();

   DISP_EN = DISP_BC_DRIVEN;           // EFM8 does not drive display

   // Configure initial PWM duty cycle = 50%
   CEX0_Compare_Value = 2048 - (2048 * 0.5);

   while (1)
   {
      // Wait a little while
      for (delay_count = 1200; delay_count > 0; delay_count--);

      if (duty_direction == 1)         // Direction = Increase
      {
         // First, check the ECOM0 bit
         if ((PCA0CPM0 & PCA0CPM0_ECOM__BMASK) == PCA0CPM0_ECOM__DISABLED)
         {
            PCA0CPM0 |= PCA0CPM0_ECOM__BMASK; // Set ECOM0 if it is '0'
         }
         else                          // Increase duty cycle otherwise
         {
            CEX0_Compare_Value--;      // Increase duty cycle

            if (CEX0_Compare_Value == 0x0000)
            {
               duty_direction = 0;     // Change direction for next time
            }
         }
      }
      else                             // Direction = Decrease
      {
         if (CEX0_Compare_Value == 0x07FF)
         {
            PCA0CPM0 &= ~PCA0CPM0_ECOM__BMASK; // Clear ECOM0
            duty_direction = 1;        // Change direction for next time
         }
         else
         {
            CEX0_Compare_Value++;      // Decrease duty cycle
         }
      }
   }
}
