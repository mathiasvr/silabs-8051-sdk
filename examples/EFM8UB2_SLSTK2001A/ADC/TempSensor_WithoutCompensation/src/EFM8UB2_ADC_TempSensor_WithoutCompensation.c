//-----------------------------------------------------------------------------
// EFM8UB2_ADC_TempSensor_WithoutCompensation.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program shows the use of the temperature sensor along with the ADC.
// Without calibration, the temperature accuracy is about +/- 10 degrees C.
//
// The ADC is set to left-justified mode. When in this mode, the ADC uses bits
// 6 through 15 to output the ADC readings with the decimal point being to the
// left of bit 15 instead of to the right of bit 0 in right-justified mode.
// The ADC0 reading looks like:
//
//    Left-justified:     0.xxxxxxxxxx000000
//    Right-justified:    000000xxxxxxxxxx.0
//
//    where x's represent the readings from the ADC, and 0's are always 0
//
// This mode's output is fractional since the decimal point is on the left
// side. Since the ADC output is fractional, we can directly multiply the ADC
// reading with the VREF voltage and find the voltage of the temp sensor
// reading.
//
// The only caveat of this is that we have to keep track of where the decimal
// point is after multiplication.
//
// This program uses the following equation from the datasheet:
//
//    Vtemp = 2.87T(C) + 764mV
//
// Moving T(C) to the left-hand side:
//
//    T(C) = (1 / 2.87)Vtemp - (764 mV / 2.87)
//
// In this instance, the equivalent linear equation is given by:
//
//    T(C) = 0.348Vtemp - 266.202 (V in millivolts)
//
// Converting from V to an ADC reading:
//
//    T(C) = (0.348*VREF*ADC) - 266.202;
//
// Sampling 256 times:
//
//    T(C) = (0.348*VREF)*(ADC_sum/256) - 266.202;
//
// Assuming a VREF = 2420 mV:
//
//    T(C) = 3294*ADC_sum - 266.202;
//
// To allow for precise calculations using fixed point math these
// factors are going to be scaled up 1000x
//
// Remarks:
//
//    SLOPE - coefficient of the line that multiplies the ADC_sum value
//    OFFSET - the coefficient to be added or subtracted to the line
//    T(C) - Temperature in Celsius
//    ADC_sum - Sum of the ADC conversions (Oversampled)
//    OVER_ROUND - This variable keeps track of where the decimal place is.
//       We get right-justified by shifting right OVER_ROUND times.
//    Oversampling - For more information on oversampling to improve ADC
//       resolution, please refer to Application Note AN118.
//
// Resources:
//   SYSCLK - 48 MHz HFOSC / 2
//   ADC    - 10-bit, VREF = internal 2.42V
//   UART0  - 115200 baud, 8-N-1
//   Timer1 - UART0 clock source
//   Timer2 - ADC start of conversion source
//   P0.0   - Board Controller enable
//   P0.4   - UART0 TX
//   P0.5   - UART0 RX
//   P1.5   - VREF output
//   P2.1   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8UB2 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8UB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8UB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the JLink CDC UART Port at 115200 baud rate and 8-N-1.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
//    The format of the output is:
//
//    T = 24.3(C)
//
//    where T is the temperature in Celsius
//
// 7) The temperature will change by simply touching the IC with any warm
//    object (including a finger) or any cold object, like freeze spray (be
//    careful to avoid ESD issues).
//
// Target:         EFM8UB2
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 10 OCT 2014
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"
#include "retargetserial.h"
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P1, 4);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (BC_EN, SFR_P0, 0);            // Board Controller Enable
#define BC_DISCONNECTED 0              // 0 = Board Controller disconnected
                                       //     to EFM8 UART pins
#define BC_CONNECTED    1              // 1 = Board Controller connected
                                       //     to EFM8 UART pins

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SCALE                    1000L // Scale for temp calculations
#define OVER_ROUND                  10 // Number of shifts (>>N) to reach the
                                       // correct number of bits of precision

#define SLOPE                     3294 // Slope of the temp transfer function
#define OFFSET                 266202L // Offset for the temp transfer function

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
uint32_t ADC_SUM = 0;                  // Accumulates the ADC samples
bool CONV_COMPLETE = 0;                 // ADC accumulated result ready flag

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
   int16_t temp_frac;                  // Stores unscaled decimal portion of
                                       // temperature for output.

   enter_DefaultMode_from_RESET();

   DISP_EN = DISP_BC_DRIVEN;           // EFM8 does not drive display

   BC_EN = BC_CONNECTED;   			   // Board controller connected to EFM8
                                       // UART pins

   //printf requires that the UART be primed by setting TI
   SCON0_TI = 1;

   IE_EA = 1;                          // Enable all interrupts

   RETARGET_PRINTF("\f");                       // Clear the screen before outputs

   while (1)                           // Spin forever
   {
      if(CONV_COMPLETE)
      {
         temp_scaled = ADC_SUM;        // Apply our derived equation to ADC

         temp_scaled *= SLOPE;

         // With a left-justified ADC, we have to shift the decimal place
         // of temp_scaled to the right so we can match the format of
         // OFFSET. Once the formats are matched, we can subtract OFFSET.
         temp_scaled = temp_scaled >> OVER_ROUND;

         temp_scaled -= OFFSET;

         // Calculate the temp's whole number portion by unscaling
         temp_whole = temp_scaled/SCALE;

         // The temp_frac value is the unscaled decimal portion of temperature
         temp_frac = (int16_t)((temp_scaled - temp_whole*SCALE) / (SCALE/10));

         if(temp_frac < 0)
         {
            temp_frac *= -1;           // If negative, remove negative from
                                       // temp_frac portion for output.
         }

         RETARGET_PRINTF(" T = %ld.%d(C) \n", temp_whole, temp_frac);

         CONV_COMPLETE = 0;
      }
   }
}
