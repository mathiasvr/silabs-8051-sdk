//-----------------------------------------------------------------------------
// EFM8BB51_ADC_ExternalInput.c
//-----------------------------------------------------------------------------
// Copyright 2020 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example code takes and averages 2048 analog measurements from input
// P0.2 using ADC0, then prints the results to a terminal window via the UART.
//
// The system is clocked by the internal 24.5 MHz oscillator divided by 1.
// Timer 2 triggers a conversion on ADC0 on each overflow.  The completion of
// this conversion in turn triggers an interrupt service routine (ISR).  The ISR
// averages 2048 measurements, then prints the value to the terminal via printf
// before starting another average cycle.
//
// The analog multiplexer selects P0.2 as the positive ADC0 input. This pin is
// configured as an analog input in the port initialization routine. The
// negative ADC0 input is connected via mux to internal ground, which provides
// for a single-ended ADC input.
//
// Terminal output is done via printf, which directs the characters to
// UART0. A UART initialization routine is therefore necessary.
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
//                   | 2^12 |
//    Tsettle =   ln | ---- | * 100e3 * 5e-12 = 4.85 us
//                   | 0.25 |
//
// In addition, one must allow at least 1.5 uS after changing analog mux
// inputs or PGA settings.  The settling time in this example, then, is
// dictated by the large external source resistance.
//
// The conversion time is given in the datasheet (12-bit mode, 30 clocks;
// SAR clock 24.5 MHz) as 1.2 us.
//
// Tsample, minimum  = Tsettle + Tconvert
//                   = 4.85 us + 1.2 us
//                   = 6.05 us
//
// Timer2 is set to start a conversion every 100 us, which is far longer
// than the minimum required.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   ADC    - 12-bit, VREF = VDD Range 2 (3.3V)
//   UART0  - 115200 baud, 8-N-1
//   Timer1 - UART0 clock source
//   Timer2 - ADC start of conversion source
//   P0.2   - ADC positive input
//   P0.4   - UART0 TX
//   P0.5   - UART0 RX
//   P0.0   - Board Controller enable
//   P2.0   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB51 Pro Kit (PK)
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB51 PK board to a PC using a USB cable.
// 3) Compile and download code to the EFM8BB51 PK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the JLink CDC UART Port at 115200 baud rate and 8-N-1.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) Apply a voltage source to P0.2 with reference to the EFM8BB51 PK board
//    ground. Vary the voltage or apply different sources. The terminal program
//    will print the measured voltage observed at P0.2.
//
//
// Target:         EFM8BB51
// Tool chain:     Generic
//
// Release 0.1 (AG)
//    - Initial Revision
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB51_Register_Enums.h>
#include <InitDevice.h>

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 0);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (BC_EN, SFR_P0, 0);            // Board Controller Enable
#define BC_DISCONNECTED 0              // 0 = Board Controller disconnected
                                       //     to EFM8 UART pins
#define BC_CONNECTED    1              // 1 = Board Controller connected
                                       //     to EFM8 UART pins

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

    DISP_EN = DISP_BC_DRIVEN;           // Display not driven by EFM8

    BC_EN = BC_CONNECTED;               // Board controller connected to EFM8
                                        // UART pins

    SCON0_TI = 1;                       // This STDIO library requires TI to
                                        // be set for prints to occur

    IE_EA = 1;                          // Enable global interrupts

    while (1) {}                        // Spin forever
}
