//-----------------------------------------------------------------------------
// EFM8UB3_ADC_Lib_Interrupt.c
//-----------------------------------------------------------------------------
// Copyright 2016 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates using the EFM8 ADC peripheral driver library to
// sample a voltage in interrupt mode.
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
// The analog multiplexer selects P0.2 as the positive ADC0 input. This pin is
// configured as an analog input in the port initialization routine. The
// negative ADC0 input is connected via mux to ground, which provides for a
// single-ended ADC input.
//
// A push button is connected to P0.2.
//
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 8 (3.062 MHz)
//   ADC    - 10-bit, VREF = Internal 1.65 VREF / 0.5  (3.3V)
//            SARCLK = SYSCLK / 1 (3.062 MHz)
//   Timer2 - ADC start of conversion source, 100 Hz overflow
//   P0.2   - Push Button
//   P1.4   - Green LED
//   P1.5   - Blue LED
//   P1.6   - Red LED
//   P2.0   - Board Controller enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8UB3 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8UB3 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8UB3 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Press the button. LED0 will change color based on the button's position
//
// Target:         EFM8UB3
// Tool chain:     Generic
//
// Release 0.1 (BL;AT)
//    - Initial Revision
//    - 08 JAN 2016
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8UB3_Register_Enums.h>
#include "InitDevice.h"
#include "adc_0.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT (BC_EN, SFR_P2, 0);           // Board Controller Enable
#define BC_DISCONNECTED 0             // 0 = Board Controller disconnected
                                      //     to EFM8 UART pins
#define BC_CONNECTED    1             // 1 = Board Controller connected
                                      //     to EFM8 UART pins

SI_SBIT(GREEN_LED, SFR_P1, 4);        // Green LED
SI_SBIT(BLUE_LED, SFR_P1, 5);         // Blue LED
SI_SBIT(RED_LED, SFR_P1, 6);          // Red LED

#define LED_ON  (0)
#define LED_OFF (1)

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

  while (1)
  {
    // Idle the CPU until the next interrupt
    PCON0 |= PCON0_IDLE__IDLE;

    if (ADC_CONVERSION_COMPLETE)
    {
      ADC_CONVERSION_COMPLETE = false;

      // Convert sample to mV
      mV = ADC0_convertSampleToMillivolts(ADC0_getResult());

      if(mV > 1500)
      {
    	  GREEN_LED = LED_ON;
    	  RED_LED = LED_OFF;
      } else{
    	  GREEN_LED = LED_OFF;
    	  RED_LED = LED_ON;
      }
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
