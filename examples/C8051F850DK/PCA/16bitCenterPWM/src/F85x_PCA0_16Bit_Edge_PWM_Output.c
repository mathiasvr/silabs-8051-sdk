//-----------------------------------------------------------------------------
// F85x_PCA0_16Bit_Edge_PWM_Output.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program sends a PWM waveform out of an I/O pin, using the PCA's
// 16-bit PWM Output Mode.  The duty cycle of the waveform is modified
// periodically within the main loop.
//
// In this example, PCA Module 0 is used to generate the waveform, and the
// crossbar is configured to send the CEX0 pin out on P0.0.
//
//-----------------------------------------------------------------------------
// How To Test: UPMU-F850
//-----------------------------------------------------------------------------
//
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
//		The waveform should be visible on the oscilloscope.
// 6) Verify that the duty cycle of the waveform varies smoothly between
//    minimum and maximum values.
//
//-----------------------------------------------------------------------------
// How To Test: ToolStick F85x
//-----------------------------------------------------------------------------
//
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Place an oscilloscope probe on P0.0.
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

#define SYSCLK       24500000          // Internal oscillator frequency in Hz

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
uint16_t CEX0_Compare_Value;       // Holds current PCA compare value

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
   uint8_t delay_count;          // Used to implement a delay
   bit duty_direction = 0;             // 0 = Decrease; 1 = Increase

   enter_DefaultMode_from_RESET();
   
   // Configure initial PWM duty cycle = 50%
   CEX0_Compare_Value = 65536 - (65536 * 0.5);

   while (1)
   {
      // Wait a little while
      for (delay_count = 120; delay_count > 0; delay_count--);

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
         if (CEX0_Compare_Value == 0xFFFF)
         {
            PCA0CPM0 &= ~0x40;         // Clear ECOM0
            duty_direction = 1;        // Change direction for next time
         }
         else
         {
            CEX0_Compare_Value++;      // Decrease duty cycle
         }
      }

   };
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
