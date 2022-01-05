//-----------------------------------------------------------------------------
// EFM8BB1_ADC_ExternalInput.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example code takes and averages 2048 analog measurements from input
// P1.7 using ADC0, then prints the results to a terminal window via the UART.
//
// The system is clocked by the internal 24.5 MHz oscillator divided by 1.
// Timer 2 triggers a conversion on ADC0 on each overflow.  The completion of
// this conversionin turn triggers an interrupt service routine (ISR).  The ISR
// averages 2048 measurements, then prints the value to the terminal via printf
// before starting another average cycle.
//
// The analog multiplexer selects P1.7 (ADC0.15) as the positive ADC0 input.
// This pin is configured as an analog input in the port initialization
// routine. The negative ADC0 input is connected via mux to ground, which
// provides for a single-ended ADC input.
//
// An analog joystick is connected to P1.7. Each joystick position corresponds
// to a unique voltage.
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
// For example, assume Rtotal = 100 kohm. The expression evaluates to:
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
// Resources:
//   SYSCLK - 24.5 MHz HFOSC / 1
//   ADC    - 10-bit, VREF = VDD (3.3V)
//   UART0  - 115200 baud, 8-N-1
//   Timer1 - UART0 clock source
//   Timer2 - ADC start of conversion source
//   P0.4   - UART0 TX
//   P0.5   - UART0 RX
//   P1.7   - ADC input
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 LCK
//-----------------------------------------------------------------------------
// 1) Connect the EFM8BB1 LCK board to a PC using a micro USB cable.
// 2) Compile and download code to the EFM8BB1 LCK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 3) Disconnect from the device to begin running the example.
// 4) Connect to the device's UART pins on P0.4 (TXD) and P0.5 (RXD):
//		a) Option 1:
//			1. Bridge solder jumpers J7 and J9 on the LCK.
//			2. Download and install Toolstick Terminal: www.silabs.com/products/development-tools/software/8-bit-8051-microcontroller-software#toolstick
//			3. Open Toolstick Terminal and connect to the device.
//		b) Option 2:
//			1. Attach a USB-to-UART bridge to pins P0.4 and P0.5.
//			2. Open HyperTerminal (or any other terminal program) and connect
//				to the USB port (virtual COM port) at <BAUDRATE> (115200) and 8-N-1.
// 5) Apply a voltage (0-3.3V) to P1.7. The terminal program will print the
//    voltage.
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
#include <SI_EFM8BB1_Register_Enums.h>
#include "InitDevice.h"

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
   enter_DefaultMode_from_RESET();

   SCON0_TI = 1;                       // This STDIO library requires TI to
                                       // be set for prints to occur

   IE_EA = 1;                          // Enable global interrupts

   while (1) {}
}
