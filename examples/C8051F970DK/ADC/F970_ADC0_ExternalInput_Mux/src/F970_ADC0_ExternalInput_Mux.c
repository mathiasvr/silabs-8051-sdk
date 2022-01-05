//-----------------------------------------------------------------------------
// F970_ADC0_ExternalInput_Mux.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This code example illustrates using the internal analog multiplexer to
// measure analog voltages on up to 8 different analog inputs. Results are
// printed to a PC terminal program via the UART.
//
// The inputs are sequentially scanned beginning with input P3.0 and up to P3.7
//
// Terminal output is done via printf, which directs the characters to
// UART0. A UART initialization routine is therefore necessary.
//
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
// Timer2 is set to start a conversion every <SAMPLE_PERIOD> ms.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP1: VCP RX <> P0.1/UART0.TX
//    - JP2: VCP TX <> P0.2/UART0.RX
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
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
// Global CONSTANTS
//-----------------------------------------------------------------------------
#define ANALOG_INPUTS     8             // Number of AIN pins to measure

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
long RESULTS[ANALOG_INPUTS];            // ADC0 decimated value, one for each
                                        // analog input

unsigned char AMUX_INPUT = 0;           // Index of analog MUX inputs
SI_SBIT(LED05, P0, 5);                  // LED=0 means ON

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
// This ISR is called after each ADC conversion which is triggered by Timer2.
// After the last conversion is completed, the results are printed out.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC_ISR, ADC0EOC_IRQn)
{
  uint32_t mV;                             // Measured voltage in mV
  SFRPAGE = ADC0_PAGE;

  ADC0CN_ADINT = 0;                   // Clear ADC0 Conversion Compelte flag

  RESULTS[AMUX_INPUT] = ADC0;         // Save Result

  if (AMUX_INPUT == ANALOG_INPUTS - 1) {
    // Print results. Conversions complete.
    unsigned char i;

    // Toggle LED
    LED05 = !LED05;

    printf("\f");
    for (i = 0; i < ANALOG_INPUTS; i++) {
      // The 10-bit ADC value is averaged across INT_DEC measurements.
      // The result is then stored in RESULT, and is right-justified
      // The measured voltage applied to the port pins is then:
      //
      //                           Vref (mV)
      //   measurement (mV) =   --------------- * Result (bits)
      //                        (2^10)-1 (bits)

      mV = RESULTS[i] * 3300 / 1024;

      printf("P3.%bu voltage: %4ld mV\n", i, mV);
    }

    // Reset AMUX0P3
    AMUX_INPUT = 0;
    AMUX0P3 = AMUX0P3_B0__CONNECTED;

    // Timer2 will restart conversion
  } else {
    // Connect next pin to Mux
    AMUX_INPUT++;
    AMUX0P3 = (1 << AMUX_INPUT);

    // Start next conversion
    ADC0CN_ADBUSY = 1;
  }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
