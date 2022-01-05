//-----------------------------------------------------------------------------
// F85x_PCA0_11Bit_Center_PWM_Output.c
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
// the crossbar is configured to send the CEX0 signal out on P0.0.
//
//-----------------------------------------------------------------------------
// Configurator Settings:
//-----------------------------------------------------------------------------
//    Peripherals:
//       Clock Control:
//          SYSCLK/1 is selected as the clock source divider, with 
//          High-Frequency Oscillator selected as the source.
//
//       Interrupts:
//          Enabled All Interrupts is set, with PCA0 interrupts enabled.      
//
//       PCA:  
//          Channel 0 is set to 8-11 bit mode, with 11 bit mode selected.
//          Cycle Overflow interrupts are enabled.
//          The Auto-Reload Registers are selected, with an initial value of
//          1024 placed in them via the Capture/Compare Registers to set the
//          initial duty cycle to 50%. (2024 / 2)
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
//       P0.0 is configured as a Digital Push-Pull Output.
//        
//    Crossbar0:
//        PCA0 - CEX0 is routed to the first available pin (P0.0).
//
//-----------------------------------------------------------------------------
// How To Test: UPMU-F850
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - JP2
// 2) Place an oscilloscope probe on P0.0.
// 3) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 4) Compile and download code to the 'F85x UDP MCU board by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
// 	The waveform should be visible on the osciloscope.
// 6) Verify that the duty cycle of the waveforms vary smoothly between
//    minimum and maximum values.
//
//-----------------------------------------------------------------------------
// How To Test: ToolStick F85x
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Place an oscilloscope probe on P0.0.
// 3) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 4) Compile and download code to the ToolStick F85x board by selecting
// 	Run -> Debug from the menus, clicking the Debug button in the quick menu,
//	   or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//	   Resume button in the quick menu, or pressing F8.
// 	The waveform should be visible on the osciloscope.
// 6) Verify that the duty cycle of the waveforms vary smoothly between
//	   minimum and maximum values.
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.3 (BL)
//    - Converted project to use Configurator
//    - 30 JAN 2014
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

#define SYSCLK       24500000          // Internal oscillator frequency in Hz

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
extern uint16_t CEX0_Compare_Value;       // Holds current PCA compare value

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
   bit duty_direction = 0;             // 0 = Decrease; 1 = Increase

   enter_DefaultMode_from_RESET();

   // Configure initial PWM duty cycle = 50%
   CEX0_Compare_Value = 2048 - (2048 * 0.5);

   while (1)
   {
      // Wait a little while
      for (delay_count = 1200; delay_count > 0; delay_count--);

      if (duty_direction == 1)         // Direction = Increase
      {
         // First, check the ECOM0 bit
         if ((PCA0CPM0 & 0x40) == 0x00)
         {
            PCA0CPM0 |= 0x40;          // Set ECOM0 if it is '0'
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
            PCA0CPM0 &= ~0x40;         // Clear ECOM0
            duty_direction = 1;        // Change direction for next time
         }
         else
         {
            CEX0_Compare_Value++;      // Decrease duty cycle
         }
      }
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
