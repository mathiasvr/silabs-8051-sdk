//-----------------------------------------------------------------------------
// F85x_PCA0_High_Speed_Output.c
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
// crossbar is configured to send the CEX0 pin out on P0.0.
//
//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA time base, and sets up High-Speed output
// mode for Module 0 (CEX0 pin).
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
//    -------------------------------------------------------------------------
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
//    -------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------
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
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
// 	The waveform should be visible on the oscilloscope.
//		The frequency is determined by CEX0_FREQUENCY, default 10kHz.
//
//-----------------------------------------------------------------------------
// How To Test: UPMU-F850
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Place an oscilloscope probe on P0.0.
// 3) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 4) Compile and download code to the ToolStick F85x board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//    The waveform should be visible on the oscilloscope.
//    The frequency is determined by CEX0_FREQUENCY, default 10kHz.
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.3 (BL)
//    - Reworked example to use hwconfig
//    - 29 JAN 2014
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
#include "F85x_PCA0_High_Speed_Output.h"
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK       24500000          // Internal oscillator frequency in Hz
#define CEX0_FREQUENCY  10000          // Frequency to output in Hz
#define DIVIDE_RATIO (SYSCLK/CEX0_FREQUENCY/2) // SYSCLK cycles per interrupt

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
uint16_t Next_Compare_Value;       // Next edge to be sent out in HSO mode

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
   enter_DefaultMode_from_RESET();

   // Set up the variable for the following match
   Next_Compare_Value = PCA0CP0 + DIVIDE_RATIO;

   while (1);                          // Spin here to wait for ISR
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
