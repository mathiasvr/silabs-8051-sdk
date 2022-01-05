//-----------------------------------------------------------------------------
// F85x_PCA0_8Bit_Center_PWM_Output.c
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
// the crossbar is configured to send the CEX0 signal out on P0.0 and the CEX1
// signal out on P0.1.
//
//-----------------------------------------------------------------------------
// Configurator Settings:
//-----------------------------------------------------------------------------
//    Peripherals:
//       Clock Control:
//          SYSCLK/1 is selected as the clock source divider, with 
//          High-Frequency Oscillator selected as the source.    
//
//       PCA:
//          Channel 0 and 2 are set as 8-11 bit pwm, with 8 bits cycle length
//          selected.
//          128 (2^8 / 2) is put into each Capture/Compare register to set the
//          initial duty cycle to 50%.
//          PCA Counter/Timer Run Control is set to Start.         
//
//       Reset Sources:
//          Missing clock detector is enabled as a reset source.
//
//       WatchDog Timer:
//          Watchdog timer is disabled.
//
//    Port I/O:
//       Crossbar is enabled, with Pull-ups enabled.
//       P0.0 and P0.1 are configured as a Digital Push-Pull Outputs.
//        
//    Crossbar0:
//        PCA0 - CEX0 and CEX1 are routed.
//
//-----------------------------------------------------------------------------
// How To Test: UPMU-F850
//-----------------------------------------------------------------------------
//
// 1) Ensure shorting blocks are place on the following:
//    - JP2
// 2) Place oscilloscope probes on P0.0 and P0.1.
// 3) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 4) Compile and download code to the 'F85x UDP MCU board by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
// 6) Verify that the duty cycle of the waveforms vary smoothly between
//    minimum and maximum values.
//
//
//-----------------------------------------------------------------------------
// How To Test: ToolStick F85x
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Place oscilloscope probes on P0.0 and P0.1.
// 3) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 4) Compile and download code to the ToolStick F85x board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//    The waveform should be visible on the osciloscope.
// 6) Verify that the duty cycle of the waveforms vary smoothly between
//    minimum and maximum values.
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.3 (BL)
//		- Converted project to use Configurator
//		- 30 JAN 2014
//
// Release 1.2 (BL)
//		- Updated How To Test
//		- 2 JAN 2014
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 15 JAN 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F850_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

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
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   uint16_t delay_count;           // Used to implement a delay
   bit duty_direction0 = 0;            // Module 0: 0 = Decrease; 1 = Increase
   bit duty_direction1 = 0;            // Module 1: 0 = Increase; 1 = Decrease

   enter_DefaultMode_from_RESET();

   while (1)
   {
      // Wait a little while
      for (delay_count = 30000; delay_count > 0; delay_count--);

      // Module 0
      if (duty_direction0 == 1)        // Direction = Increase
      {
         // First, check the ECOM0 bit
         if ((PCA0CPM0 & 0x40) == 0x00)
         {
            PCA0CPM0 |= 0x40;          // Set ECOM0 if it is '0'
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
            PCA0CPM0 &= ~0x40;         // Clear ECOM0
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
         if ((PCA0CPM1 & 0x40) == 0x00)
         {
            PCA0CPM1 |= 0x40;          // Set ECOM1 if it is '0'
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
            PCA0CPM1 &= ~0x40;         // Clear ECOM1
            duty_direction1 = 1;       // Change direction for next time
         }
         else
         {
            PCA0CPH1--;                // Decrease duty cycle
         }
      }
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------

