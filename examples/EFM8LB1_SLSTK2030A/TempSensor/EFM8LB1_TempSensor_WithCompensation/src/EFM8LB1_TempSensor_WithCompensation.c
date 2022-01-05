//-----------------------------------------------------------------------------
// EFM8LB1_ADC_TempSensor_WithCompensation.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program shows the use of the temperature sensor with the factory
// programmed calibration value stored in flash.
// With calibration, the temperature accuracy is about +/- ?? degrees C.
//
// Timer 3 is configured to overflow at 20 Hz. This overflow will trigger
// and ADC conversion.
//
// The ADC is configured to accumulate 32 14-bit samples of the temperature
// sensor. This result is divided by eight in hardware (right shift by 3),
// then divided by an additional four in software to complete the averaging.
// The result is a single 14-bit value.
//
// When the ADC conversion is complete, a conversion complete interrupt
// fires, signaling the main loop to calculate the current temperature
// and print it over UART.
//
// The temperature is calculated by using the following formula:
//
// T(C) = (ADC Sample - Temp Sensor 0C value) / 28
//
// The temperature sensor is measured by the ADC during production at 0C.
// This value is taken with the ADC in 14-bit mode, using the 1.65V internal
// reference with a gain of 1. This 16-bit value is then stored in flash at
// address 0xFFD4.
//
// In this example, the temperature is scaled by a factor of 100 to give
// the result the precision of two decimal places. The scaled temperature
// is given by the formula:
//
// T(C) = ((ADC Sample - Temp Sensor 0C value) / 28) * 100
//
// This will result in a value that represents the temperature in
// hundredths of a degree C. For example, 3212 would represent 32.12 C.
//
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   ADC    - 14-bit, VREF = internal 1.65V
//   UART0  - 115200 baud, 8-N-1
//   Timer1 - UART0 clock source
//   Timer3 - ADC start of conversion source, 20 Hz
//   P0.4   - UART0 TX
//   P0.5   - UART0 RX
//   P2.1   - Board Controller enable
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
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the JLink CDC UART Port at 115200 baud rate and 8-N-1.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
//    The format of the output is:
//
//    T = 24.32(C)
//
//    where T is the temperature in Celsius
//
// 7) The temperature will change by simply touching the IC with any warm
//    object (including a finger) or any cold object, like freeze spray (be
//    careful to avoid ESD issues).
//
// Target:         EFM8LB1
// Tool chain:     Generic
//
// Release 0.2 (ST)
//    - Corrected format the 0C cal value to [high byte][low byte]
//    - 7 OCT 2015
//
// Release 0.1 (BL)
//    - Initial Revision
//    - 9 SEP 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8LB1_Register_Enums.h>
#include "InitDevice.h"
#include "retargetserial.h"
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT(DISP_EN, SFR_P3, 4);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT(BC_EN, SFR_P2, 2);            // Board Controller Enable
#define BC_DISCONNECTED 0              // 0 = Board Controller disconnected
                                       //     to EFM8 UART pins
#define BC_CONNECTED    1              // 1 = Board Controller connected
                                       //     to EFM8 UART pins

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SCALE                     100L // Scale for temp calculations

#define TEMP_CAL_ADDRESS_LOW    0xFFD4  // Address in flash where the
                                        // temp cal low byte value is stored

#define TEMP_CAL_ADDRESS_HIGH   0xFFD5  // Address in flash where the
                                        // temp cal high byte value is stored

// Calibration value for the temperature sensor at 0 degrees C, stored in CODE space
SI_LOCATED_VARIABLE_NO_INIT(TEMPSENSOR_0C_LOW, uint8_t const, SI_SEG_CODE, TEMP_CAL_ADDRESS_LOW);
SI_LOCATED_VARIABLE_NO_INIT(TEMPSENSOR_0C_HIGH, uint8_t const, SI_SEG_CODE, TEMP_CAL_ADDRESS_HIGH);

// Absolute value macro
#define ABS(num) (num < 0 ? -num: num)

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
uint32_t ADC_AVG = 0;                  // The Averaged ADC temp sensor result
bool CONV_COMPLETE = 0;                 // ADC result ready flag

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
  int32_t temp_scaled;                // Stores scaled temperature value
  int32_t temp_whole;                 // Stores unscaled whole number portion
                                      // of temperature for output.
   uint16_t temp_frac;                 // Stores unscaled decimal portion of
                                      // temperature for output.

  uint16_t tempsensor_0c;             // Stores the temp sensor cal value at
                                      // 0C as [high byte][low byte]

  enter_DefaultMode_from_RESET();

  DISP_EN = DISP_BC_DRIVEN;           // EFM8 does not drive display

  BC_EN = BC_CONNECTED;            // Board controller connected to EFM8
                                   // UART pins

  //printf requires that the UART be primed by setting TI
  SCON0_TI = 1;

  IE_EA = 1;                          // Enable all interrupts

   RETARGET_PRINTF("\f");                       // Clear the screen before outputs

  // format the temperature calibration value
  tempsensor_0c = (TEMPSENSOR_0C_HIGH << 8) | TEMPSENSOR_0C_LOW;

  while (1)                           // Spin forever
  {
    PCON0 |= PCON0_IDLE__IDLE;       // Put the CPU in Idle mode while waiting
    // for conversions to complete
      if(CONV_COMPLETE)
      {
        // Calculate the scaled temperature using the formula provided in the datasheet
        temp_scaled = (((int32_t) ADC_AVG - tempsensor_0c) * SCALE) / 28;

        // Calculate the temp's whole number portion by unscaling
        temp_whole = temp_scaled / SCALE;

        // The temp_frac value is the unscaled decimal portion of temperature
        temp_frac = ABS(temp_scaled) % SCALE;

        RETARGET_PRINTF(" T = %ld.%d(C) \n", temp_whole, temp_frac);

        CONV_COMPLETE = 0;
    }
  }
}
