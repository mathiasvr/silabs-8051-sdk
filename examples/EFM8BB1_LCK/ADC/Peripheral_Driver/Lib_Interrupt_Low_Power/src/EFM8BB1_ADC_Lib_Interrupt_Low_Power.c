//-----------------------------------------------------------------------------
// EFM8BB1_ADC_Lib_Interrupt_Low_Power.c
//-----------------------------------------------------------------------------
// Copyright 2016 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates using the EFM8 ADC peripheral driver library to
// sample a voltage in interrupt mode using the ADC's low power features.
//
// The system is clocked by the internal 24.5 MHz oscillator divided by 8
// in order to conserve power. The ADC is clocked by the system 3.062 MHz
// clock divided by 1 to minimize the ADC's sampling time.
//
// Timer 2 triggers a conversion on ADC0 on each overflow.  The completion of
// this conversion in turn triggers an interrupt service routine (ISR). The
// ISR sets an ADC_CONVERSION_COMPLETE flag which is checked within the main
// loop. After each iteration of the main loop, the device is put into Idle
// mode to conserve power until the next interrupt fires.
//
// The analog multiplexer selects P1.7 as the positive ADC0 input. This pin is
// configured as an analog input in the port initialization routine. The
// negative ADC0 input is connected via mux to ground, which provides for a
// single-ended ADC input.
//
// The main loop prints the sampled voltage over UART.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 8 (3.062 MHz)
//   ADC    - 10-bit, VREF = Internal 1.65 VREF / 0.5  (3.3V)
//            SARCLK = SYSCLK / 1 (3.062 MHz)
//   UART0  - 115200 baud, 8-N-1
//   Timer2 - ADC start of conversion source, 100 Hz overflow
//   P1.7   - ADC Input
//   P0.4   - UART0 TXD
//   P0.5   - UART0 RXD
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
// Release 0.1 (BL;AT)
//    - Initial Revision
//    - 08 JAN 2016
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB1_Register_Enums.h>
#include "InitDevice.h"
#include "adc_0.h"
#include "retargetserial.h"

//-----------------------------------------------------------------------------
// ADC Defines
//-----------------------------------------------------------------------------
#define VREF_MV         (3300UL)
#define ADC_MAX_RESULT  ((1 << 10)-1) // 10 bit ADC

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
uint16_t ADC0_convertSampleToMillivolts(uint16_t sample);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
extern bool ADC_CONVERSION_COMPLETE;

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
  uint16_t mV;
  enter_DefaultMode_from_RESET();

  SCON0_TI = 1;

  while (1)
  {
    // Idle the CPU until the next interrupt
    PCON0 |= PCON0_IDLE__IDLE;

    if (ADC_CONVERSION_COMPLETE)
    {
      ADC_CONVERSION_COMPLETE = false;

      // Convert sample to mV
      mV = ADC0_convertSampleToMillivolts(ADC0_getResult());

	  RETARGET_PRINTF ("\nP1.7 voltage: %d mV\n", mV);
    }

  }
}

///////////////////////////////////////////////////////////////////////////////
// Supporting Functions
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// ADC0_convertSampleToMillivolts
//-----------------------------------------------------------------------------
//
// Converts a raw ADC sample to millivolts.
// sample - the raw sample from the ADC
//
// returns - a two byte number representing the adc sample in millivolts
//  e.g. 1.65V would equal 1650.
//
uint16_t ADC0_convertSampleToMillivolts(uint16_t sample)
{
  // The measured voltage applied to P1.7 is given by:
  //
  //                           Vref (mV)
  //   measurement (mV) =   --------------- * result (bits)
  //                       (2^10)-1 (bits)
  return ((uint32_t)sample * VREF_MV) / ADC_MAX_RESULT;
}
