//-----------------------------------------------------------------------------
// F85x_ADC0_ExternalInput.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This example code takes and averages 2048 analog measurements from input
// P1.2 using ADC0, then prints the results to a terminal window via the UART.
//
// The system is clocked by the internal 24.5 MHz oscillator divided by 1.
// Timer 2 triggers a conversion on ADC0 on each overflow.  The completion of
// this conversionin turn triggers an interrupt service routine (ISR).  The ISR
// averages 2048 measurements, then prints the value to the terminal via printf
// before starting another average cycle.
//
// The analog multiplexer selects P1.2 (ADC0.10) as the positive ADC0 input.
// This pin is configured as an analog input in the port initialization
// routine. The negative ADC0 input is connected via mux to ground, which
// provides for a single-ended ADC input.
//
// A 100 kohm potentiometer may be connected (J27) as a voltage divider between
// VDD (3.30 V) and AGND on the terminal strip as shown below:
//
// ---------
//          |
//         o| VDD  ----|
//         o| GND   ---|<-|
//         o| P1.2     |  |
//         o|    |        |
//         o|     --------
//         o|
//          |
//----------
// C8051F850 UDP MCU Card
//
// Terminal output is done via printf, which directs the characters to
// UART0.  A UART initialization routine is therefore necessary.
//
// ADC Settling Time Requirements, Sampling Rate:
// ----------------------------------------------
//
// The total sample time per input is comprised of an input setting time
// (Tsettle), followed by a conversion time (Tconvert):
//
// Tsample  = Tsettle + Tconvert
//
// |--------Settling-------|==Conversion==|----Settling--- . . .
// Timer 2 overflow        ^
// ADC0 ISR                               ^
//
// The ADC input voltage must be allowed adequate time to settle before the
// conversion is made.  This settling depends on the external source
// impedance, internal mux impedance, and internal capacitance.
// Settling time is given by:
//
//                   | 2^n |
//    Tsettle =   ln | --- | * Rtotal * Csample
//                   | SA  |
//
// In this application, assume a 100 kohm potentiometer as the voltage divider.
// The expression evaluates to:
//
//                   | 2^10 |
//    Tsettle =   ln | ---- | * 105e3 * 5e-12 = 4.4 us
//                   | 0.25 |
//
// In addition, one must allow at least 1.5uS after changing analog mux
// inputs or PGA settings.  The settling time in this example, then, is
// dictated by the large external source resistance.
//
// The conversion is given in the datasheet (10-bit mode, SAR clock 12.25 MHz)
// as 1 us.
//
// Tsample, minimum  = Tsettle + Tconvert
//                   = 4.4 us + 1 us
//                   = 5.4 us
//
// Timer2 is set to start a conversion every 100 us, which is far longer
// than the minimum required.
//
// F850 Resources:
// ---------------
// Timer1: clocks UART
// Timer2: overflow initiates ADC conversion
//
//-----------------------------------------------------------------------------
// How To Test: UPMU-F850
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - J27: POT - P1.2/ADC0.10
//    - J7: all
//    - JP2
// 2) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to the 'F85x UDP MCU board by selecting 
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 4) Connect a USB cable from a PC to J5 (COM PORT) on the development board.
// 5) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the virtual COM port at <BAUDRATE> (115200) and 8-N-1.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the 
//		Resume button in the quick menu, or pressing F8.
// 7) The terminal program will print the voltage measured by the device.
//
//-----------------------------------------------------------------------------
// How To Test: ToolStick F85x
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 3) Compile and download code to the ToolStick F85x board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Disconnect from the device by selecting Run -> Disconnect from the menus.
// 5) On the PC, open Silicon Laboratories ToolStick Terminal, and navigate to
//    ToolStick -> Settings in the menus. Under UART Settings, select the
//    following options:
//       -Baud Rate: as defined by <BAUDRATE> (115200 by default)
//       -Parity: None
//       -Data Bits: 8
//       -Stop Bits: 1
// 6) In ToolStick Terminal, press "Connect" to connect to the device.
// 7) The terminal program will print the voltage measured by the device.
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
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
//    - 16 JAN 2013

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // SYSCLK frequency in Hz
#define BAUDRATE       115200          // Baud rate of UART in bps

SI_SBIT(LED, SFR_P1, 0);                 // DS5 P1.0 LED

//-----------------------------------------------------------------------------
// Function PROTOTYPES
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
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   enter_DefaultMode_from_RESET();

   //Some printf implementations require that the UART be primed by
   //  setting the TX complete interrupt flag
   SCON0_TI = 1;

   IE_EA = 1;                             // Enable global interrupts
   while (1) {}
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
