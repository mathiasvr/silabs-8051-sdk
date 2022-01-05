//-----------------------------------------------------------------------------
// F99x_ADC0_12bit_ExternalInput.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This example code uses ADC in 12-bit mode to take analog measurements from
// input  P0.6 (Potentiometer input), then prints the results to a terminal
// window via the UART.
//
// The system is clocked by the internal 24.5MHz oscillator.  Timer 2 triggers
// a conversion on ADC0 on each overflow.  The completion of this conversion
// in turn triggers an interrupt service routine (ISR).
//
// The analog multiplexer selects P0.6 as the ADC0 input. All samples are
// single-ended with respect to GND. P0.6 is configured as an analog input
// in the port initialization routine.
//
// When J16 is shorted, the potentiometer (R14) wiper is connected to P0.6.
// When J16 is open, P0.6 may also be supplied with a voltage from the H1
// Terminal block.
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
// In this application, assume a 100kohm potentiometer as the voltage divider.
// The expression evaluates to:
//
//                   | 2^10 |
//    Tsettle =   ln | ---- | * 105e3 * 5e-12 = 4.4uS
//                   | 0.25 |
//
// In addition, one must allow at least 1.7uS after changing analog mux
// inputs or PGA settings.  The settling time in this example, then, is
// dictated by the large external source resistance.
//
// The conversion is 10 periods of the SAR clock <SAR_CLK>.  At 8.33 MHz,
// this time is 10 * 120nS = 1.2 uS.
//
//
// Tsample, minimum  = Tsettle + Tconvert
//                   = 4.4uS + 1.2uS
//                   = 5.6 uS
//
// Timer 2 is set to start a conversion every 100uS
//
// Resources:
// ---------------
// Timer1: clocks UART
// Timer2: overflow initiates ADC conversion
//
// How To Test:
// ------------
// 1) Ensure that jumpers are placed on the following:
//       J12:  P0.4/TX
//             P0.5/RX
//             VDD/VIO
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect a USB cable from the development board P3 to a PC
// 3) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the USB port (virtual com port) at <230400> and 8-N-1
// 4) Connect a variable voltage source to P0.6 or verify that J15 and J16 are
//    installed. J15 should be connected to the side labeled P1.4.
// 5) Connect the USB Debug Adapter to J4.
// 6) Turn the POWER switch (SW5) to the "ON" position.
// 7) Compile and download code to a 'F99x device on a C8051F99x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) HyperTerminal will print the voltage measured by the device if
//    everything is working properly
//
// Target:         C8051F99x-C8051F98x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 code with compiler agnostic code
//    - 13 JAN 2014
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F990_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

SI_SBIT(R15_ENABLE, SFR_P1, 4);          // Open-Drain output. Write '0' to
                                       // enable the potentiometer.

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);

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
// ----------------------------------------------------------------------------
int main (void)
{
	//Enter default mode
	enter_DefaultMode_from_RESET();

	R15_ENABLE = 0;                     // Enable the Potentiometer

	SCON0_TI = 1;                            // Indicate TX0 ready

	while (1) {}                             // Spin forever

	// NOTREACHED
	return 0;
}
