//-----------------------------------------------------------------------------
// EFM8BB1_ADC_TempSensor_WithoutCompensation.c
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
//    Vtemp = 2.85T(C) + 757mV
//
// Moving T(C) to the left-hand side:
//
//    T(C) = (1 / 2.85)Vtemp - (757 mV / 2.85)
//
// In this instance, the equivalent linear equation is given by:
//
//    T(C) = 0.351Vtemp - 265.614 (V in millivolts)
//
// Converting from V to an ADC reading:
//
//    T(C) = (0.351*VREF*ADC) - 265.614;
//
// Sampling 256 times:
//
//    T(C) = (0.351*VREF)*(ADC_sum/256) - 265.61;
//
// Assuming a VREF = 1650 mV:
//
//    T(C) = 2.262*ADC_sum - 265.614;
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
//   SYSCLK - 24.5 MHz HFOSC / 1
//   ADC    - 10-bit, VREF = internal 1.65V
//   UART0  - 115200 baud, 8-N-1
//   Timer1 - UART0 clock source
//   Timer2 - ADC start of conversion source
//   P0.4   - UART0 TX
//   P0.5   - UART0 RX
//   P2.1   - Board Controller enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 LCK
//-----------------------------------------------------------------------------
// 1) Power the board using the DBG USB connector.
// 2) Compile and download code to the EFM8BB1 board by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
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
//    The format of the output is as follows:
//
//    T = 24.3(C)
//
//    where T is the temperature in Celsius
//
// 5) The temperature will change by simply touching the IC with any warm
//    object (including a finger) or any cold object, like freeze spray (be
//    careful to avoid ESD issues).
//
// Target:         EFM8BB1
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 10 OCT 2014
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB1_Register_Enums.h>
#include "retargetserial.h"
#include <stdlib.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SCALE                    1000L // Scale for temp calculations
#define OVER_ROUND                  10 // Number of shifts (>>N) to reach the
                                       // correct number of bits of precision

#define SLOPE                     2262 // Slope of the temp transfer function
#define OFFSET                 265614L // Offset for the temp transfer function

//-----------------------------------------------------------------------------
// Global VARIABLES
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

   //the printf being used requires that the transmit complete
   //  interrupt be set prior to the first byte being transmitted
   SCON0_TI = SCON0_TI__SET;

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
