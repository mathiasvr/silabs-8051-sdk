//-----------------------------------------------------------------------------
// EFM8UB1_ADC_Lib_Interrupt_Window_Compare.c
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
// Timer 2 triggers a conversion on ADC0 on each overflow. When a conversion
// is completed, if it is greater than or less than the ADC0's respective
// window compare values, it will trigger an interrupt. Inside the interrupt,
// these window compare values are adjusted to be around the latest conversion.
// This effectively means that the ADC will only trigger an interrupt when
// the voltage being sampled changes. This drastically reduces the number of
// interrupts serviced compared to the normal interrupt example.
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
//   SYSCLK - 24.5 MHz HFOSC0 / 8 (3.062 MHz)
//   ADC    - 10-bit, VREF = Internal 1.65 VREF / 0.5  (3.3V)
//            SARCLK = SYSCLK / 1 (3.062 MHz)
//   Timer2 - ADC start of conversion source, 100 Hz overflow
//   P1.4   - Green LED
//   P1.5   - Blue LED
//   P1.6   - Red LED
//   P1.7   - Joystick
//   P2.0   - Board Controller enable
//   P2.3   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8UB1 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8UB1 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8UB1 STK board.
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
// 6) Place a breakpoint within the ADC Window Compare ISR (ADC0WC_ISR in
//    Interrupts.c). Note that this interrupt only fires when the joystick
//    changes positions.
//
// Target:         EFM8UB1
// Tool chain:     Generic
//
// Release 0.1 (BL;AT)
//    - Initial Revision
//    - 08 JAN 2016
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8UB1_Register_Enums.h>
#include "InitDevice.h"
#include "adc_0.h"
#include "joystick.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 3);         // Display Enable
#define DISP_BC_DRIVEN   0            // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1            // 1 = EFM8 drives display

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
void ConvertDirectionToColor(uint8_t dir);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
extern bool ADC_WINDOW_COMPARE;

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

    if (ADC_WINDOW_COMPARE)
    {
    	ADC_WINDOW_COMPARE = false;

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
