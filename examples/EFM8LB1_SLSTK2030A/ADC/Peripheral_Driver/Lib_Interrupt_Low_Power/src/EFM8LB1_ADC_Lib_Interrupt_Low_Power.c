//-----------------------------------------------------------------------------
// EFM8LB1_ADC_Lib_Interrupt_Low_Power.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates using the EFM8 ADC peripheral driver library to
// sample a voltage in interrupt mode using the ADC's low power features.
//
// The system is clocked by the internal 24.5 MHz oscillator divided by 32
// in order to conserve power. The ADC is clocked by the internal 24.5 MHz
// oscillator divided by 2 to minimize the ADC's sampling time.
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
// An analog joystick is connected to P1.7. Each joystick position corresponds
// to a unique voltage.
//
// The main loop converts the sampled voltage into a joystick direction, which,
// in turn, is used to turn on LEDs to make a particular color.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 32 (765 kHz)
//   ADC    - 14-bit, VREF = Internal 1.65 VREF / 0.5  (3.3V)
//            SARCLK = HFOSC0 / 2 (12.25 MHz)
//   Timer2 - ADC start of conversion source, 100 Hz overflow
//   P1.4   - Green LED
//   P1.5   - Blue LED
//   P1.6   - Red LED
//   P1.7   - Joystick
//   P2.2   - Board Controller enable
//   P3.4   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8LB1 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8LB1 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8LB1 STK board.
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
// 6) Run the code in the Energy Profiler. Note that the average current consumption
//    for this example is approximately 680uA, compared to approximately 2.2mA with
//    the non low-power interrupt example.
//
// Target:         EFM8LB1
// Tool chain:     Generic
//
// Release 0.1 (BL)
//    - Initial Revision
//    - 04 AUG 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8LB1_Register_Enums.h>
#include "InitDevice.h"
#include "adc_0.h"
#include "joystick.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P3, 4);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (BC_EN, SFR_P2, 2);            // Board Controller Enable
#define BC_DISCONNECTED 0              // 0 = Board Controller disconnected
                                       //     to EFM8 UART pins
#define BC_CONNECTED    1              // 1 = Board Controller connected
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
#define ADC_MAX_RESULT  ((1 << 14)-1) // 14 bit ADC

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
uint16_t ADC0_convertSampleToMillivolts(uint16_t sample);
void ConvertDirectionToColor(uint8_t dir);

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
  uint8_t joystickDir;
  enter_DefaultMode_from_RESET();

  DISP_EN = DISP_BC_DRIVEN;           // Display not driven by EFM8

  while (1)
  {
    // Idle the CPU until the next interrupt
    PCON0 |= PCON0_IDLE__IDLE;

    if (ADC_CONVERSION_COMPLETE)
    {
      ADC_CONVERSION_COMPLETE = false;

      // Convert sample to mV
      mV = ADC0_convertSampleToMillivolts(ADC0_getResult());

      // Get joystick direction
      joystickDir = JOYSTICK_convert_mv_to_direction(mV);

      // Convert to color
      ConvertDirectionToColor(joystickDir);
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
  //                       (2^14)-1 (bits)
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
