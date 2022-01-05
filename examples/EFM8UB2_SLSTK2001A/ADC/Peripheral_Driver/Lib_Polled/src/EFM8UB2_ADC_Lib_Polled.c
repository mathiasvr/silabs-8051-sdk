//-----------------------------------------------------------------------------
// EFM8UB2_ADC_Lib_Polled.c
//-----------------------------------------------------------------------------
// Copyright 2016 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates using the EFM8 ADC peripheral driver library to
// sample a voltage in polled mode.
//
// The analog multiplexer selects P2.5 as the positive ADC0 input. This pin is
// configured as an analog input in the port initialization routine. The
// negative ADC0 input is connected via mux to ground, which provides for a
// single-ended ADC input.
//
// An analog joystick is connected to P2.5. Each joystick position corresponds
// to a unique voltage.
//
// Inside the main loop, the ADC is triggered to start a conversion. The
// device then waits in the main loop until the ADC conversion is complete.
// The sampled voltage is then converted into a joystick direction, which,
// in turn, is used to turn on LEDs to make a particular color.
//
// Resources:
//   SYSCLK - 48 MHz HFOSC0 / 1
//   ADC    - 10-bit, VREF = Unregulated 3.3 VDD / 1  (3.3V)
//   P1.6   - Green LED
//   P1.7   - Blue LED
//   P2.0   - Red LED
//   P2.5   - Joystick
//   P0.0   - Board Controller enable
//   P1.4   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8UB2 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8UB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8UB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Move the joystick. LED0 will change color based on the joystick's position:
//      UP    - White
//      LEFT  - Blue
//      DOWN  - Green
//      RIGHT - Red
//
// Target:         EFM8UB2
// Tool chain:     Generic
//
// Release 0.1 (BL;AT)
//    - Initial Revision
//    - 08 JAN 2016
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"
#include "adc_0.h"
#include "joystick.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P1, 4);         // Display Enable
#define DISP_BC_DRIVEN   0            // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1            // 1 = EFM8 drives display

SI_SBIT (BC_EN, SFR_P0, 0);           // Board Controller Enable
#define BC_DISCONNECTED 0             // 0 = Board Controller disconnected
                                      //     to EFM8 UART pins
#define BC_CONNECTED    1             // 1 = Board Controller connected
                                      //     to EFM8 UART pins

SI_SBIT(GREEN_LED, SFR_P1, 6);        // Green LED
SI_SBIT(BLUE_LED, SFR_P1, 7);         // Blue LED
SI_SBIT(RED_LED, SFR_P2, 0);          // Red LED

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
void ConvertDirectionToColor(uint8_t dir);

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
  uint8_t joystickDir;
  enter_DefaultMode_from_RESET();

  DISP_EN = DISP_BC_DRIVEN;           // Display not driven by EFM8

  while (1)
  {
    //Start conversion
    ADC0_startConversion();

    // Wait for conversion to complete
    while (!ADC0_isConversionComplete());

    // Convert sample to mV
    mV = ADC0_convertSampleToMillivolts(ADC0_getResult());

    // Get joystick direction from ADC sample
    joystickDir = JOYSTICK_convert_mv_to_direction(mV);

    // Convert joystick direction to LED color
    ConvertDirectionToColor(joystickDir);

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
  // The measured voltage applied to P2.5 is given by:
  //
  //                           Vref (mV)
  //   measurement (mV) =   --------------- * result (bits)
  //                       (2^10)-1 (bits)
  return ((uint32_t)sample * VREF_MV) / ADC_MAX_RESULT;
}

//-----------------------------------------------------------------------------
// ConvertDirectionToColor
//-----------------------------------------------------------------------------
//
// Turns on LEDs depending on the joystick direction given
//
// dir -  JOYSTICK_N = White
//        JOYSTICK_E = Red
//        JOYSTICK_S = Green
//        JOYSTICK_W = Blue
//        Any other direction = Off
//
void ConvertDirectionToColor(uint8_t dir)
{
  GREEN_LED = LED_OFF;
  BLUE_LED  = LED_OFF;
  RED_LED   = LED_OFF;

  switch (dir)
  {
  case JOYSTICK_N:
    GREEN_LED = LED_ON;
    BLUE_LED  = LED_ON;
    RED_LED   = LED_ON;
    break;

  case JOYSTICK_E:
    RED_LED   = LED_ON;
    break;

  case JOYSTICK_S:
    GREEN_LED = LED_ON;
    break;

  case JOYSTICK_W:
    BLUE_LED  = LED_ON;
    break;

  default:
    break;
  }
}
