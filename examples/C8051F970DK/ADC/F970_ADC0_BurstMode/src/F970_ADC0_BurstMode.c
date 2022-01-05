//-----------------------------------------------------------------------------
// F97x_ADC0_BurstMode.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This example code uses the Burst Mode with Auto-Averaging capability to
// generate 12-bit samples from the ADC. No CPU intervention is required
// to generate the 12-bit samples. Measurements are taken from P5.0
// (Potentiometer input) configured as an analog input and then prints the
// results to a terminal window via the UART.
//
// The system is clocked by the internal 24.5 MHz oscillator. Timer 2 triggers
// a burst of 16 ADC0 conversions on each overflow. The completion of these
// conversions triggers an interrupt service routine (ISR). The ISR
// prints the value to the terminal via printf before starting another average
// cycle.
//
// The analog multiplexer selects P5.0 as the positive ADC0 input.
// This pin is configured as an analog input in the port initialization
// routine. The negative ADC0 input is connected to ground, which provides
// for a single-ended ADC input.
//
// A 10 kohm potentiometer is connected as a voltage divider to VDD (3.30 V)
// and AGND as shown below:
//
// ---------
//          |
//         o| VDD  ----|
//         o| GND   ---|<-|
//         o| P5.0     |  |
//         o|    |        |
//         o|     --------
//         o|
//          |
//----------
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
// In this application, assume a 10 kohm potentiometer as the voltage divider.
// The expression evaluates to:
//
//                   | 2^10 |
//    Tsettle =   ln | ---- | * 105e3 * 5e-12 = 4.4 us
//                   | 0.25 |
//
// In addition, one must allow at least 1.5 us after changing analog mux
// inputs or PGA settings.  The settling time in this example, then, is
// dictated by the large external source resistance.
//
// The conversion is given in the datasheet (10-bit mode, SAR clock 12.25 MHz)
// as 1 us.
//
// Tsample, minimum  = Tsettle + Tconvert
//                   = 4.4 us + 1 us
//                   = 5.4 u
//
// Timer2 is set to start a burst of 16 conversions at <SAMPLE_RATE>.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP1: VCP RX <> P0.1/UART0.TX
//    - JP2: VCP TX <> P0.2/UART0.RX
//    - JP3:    VDD <> VMCU
//    - JP5:    POT <> P5.0/ADC0.10
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Connect a USB cable from J9 of the development board to a PC,
// 5) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the virtual COM port at <BAUDRATE> (115200) and 8-N-1.
// 6) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 7) The terminal program will print the voltage measured by the device.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 30 APR 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include <stdio.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(ADC_ISR, ADC0EOC_IRQn);

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
  SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
  PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main(void)
{
  enter_DefaultMode_from_RESET();

  // Enable printf
  SCON0_TI = 1;

  while (1)
    ;                           // Spin forever
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// ADC0_ISR
//-----------------------------------------------------------------------------
//
// This ISR reads a 12-bit conversion code then prints the result to the terminal.
// The ISR is called after each ADC conversion which is triggered by Timer2.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC_ISR, ADC0EOC_IRQn)
{
  static uint8_t sample_count = 0;
  uint32_t result;
  uint16_t mV;                   // measured voltage in mV

  ADC0CN_ADINT = 0;         // clear ADC0 conv. complete flag

  result = ADC0;            // copy ADC0 into result

  // The 10-bit ADC value represents the voltage applied
  // to P5.0 according to the following equation:
  //
  //                           Vref (mV)
  //   measurement (mV) =   --------------- * result (bits)
  //                       ((2^10)-1)*2^2 (bits)

  mV = result * 3300 / 4091;

  // Print the results every 10 samples
  if (sample_count >= 10) {
    printf("\f\nP5.0 voltage: %d mV", mV);
    sample_count = 0;

  } else {
    sample_count++;
  }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
