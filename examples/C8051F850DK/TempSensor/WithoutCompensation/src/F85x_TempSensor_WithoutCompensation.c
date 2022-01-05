//-----------------------------------------------------------------------------
// F85x_TemperatureSensor_WithoutCompensation.c
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
// The ADC0 reading lookings like:
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
//    T(C) = 0.351Vtemp - 265.61 (V in millivolts)
//
// Converting from V to an ADC reading:
//
//    T(C) = (0.351*VREF*ADC) - 265.61;
//
// Sampling 256 times:
//
//    T(C) = (0.351*VREF)*(ADC_sum/256) - 265.61;
//
// Assuming a VREF = 1650 mV:
//
//    T(C) = 2.261*ADC_sum - 265.61;
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
//-----------------------------------------------------------------------------
// How To Test: UPMU-F850
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - J7: all
//    - JP2
// 2) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to the 'F85x UDP MCU board by selecting 
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 4) Connect a USB cable from the J5 of the MCU card to a PC.
// 5) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the USB port (virtual COM port) at <BAUDRATE> (9600) and 8-N-1.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
//    The format of the output is as follows:
//
//    T = 24.3(C)
//
//    where T is the temperature in Celsius
//
// 7) The temperature will change by simply touching the IC with any warm
//    object (including a finger) or any cold object, like freeze spray (be
//    careful to avoid ESD issues!).
//
//-----------------------------------------------------------------------------
// How To Test: ToolStick F85x
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 3) Compile and download code to the ToolStick F85x board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Disconnect from the device by selecting Run -> Disconnect from the menus.
// 5) On the PC, open Silicon Laboratories ToolStick Terminal, and navigate to
//    ToolStick -> Settings in the menus. Under UART Settings, select the
//    following options:
//       -Baud Rate: as defined by <BAUDRATE> (9600 by default)
//       -Parity: None
//       -Data Bits: 8
//       -Stop Bits: 1
// 6) In ToolStick Terminal, press "Connect" to connect to the device.
//    The device should now be printing the temperature to the terminal.
//    The format of the output is as follows:
//
//    T = 24.3(C)
//
//    where T is the temperature in Celsius
//
// 7) The temperature will change by simply touching the IC with any warm
//    object (including a finger) or any cold object, like freeze spray (be
//    careful to avoid ESD issues!).
//
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
// Revision History:
//
//
// Release 1.2 (BL)
//		- Updated How To Test with Studio specific instructions
//		- 3 JAN 2014
//
// Release 1.2 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.1
//    - Added slope/offset for Revision B devices (TP)
//    - 5 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 25 FEB 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>
#include "InitDevice.h"
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SCALE                    1000L // Scale for temp calculations
#define OVER_ROUND                  10 // Number of shifts (>>N) to reach the
                                       // correct number of bits of precision

#define REV_B                     0x01 // Value of the REVID register for
                                       // revision B
#define SLOPE_REV_B               2414 // Slope of the temp transfer function
#define OFFSET_REV_B           267041L // Offset for the temp transfer function
                                       // These values are for revision B
                                       // devices with the equation
                                       //     Vtemp = 2.67T(C) + 713mV

#define REV_C                     0x02 // Value of the REVID register for
                                       // revision C
#define SLOPE_REV_C               2261 // Slope of the temp transfer function
#define OFFSET_REV_C           265610L // Offset for the temp transfer function

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
uint32_t ADC_SUM = 0;                       // Accumulates the ADC samples
bit CONV_COMPLETE = 0;                 // ADC accumulated result ready flag

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

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
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   int32_t temp_scaled;                    // Stores scaled temperature value
   int32_t temp_whole;                     // Stores unscaled whole number portion
                                       // of temperature for output.
   int16_t temp_frac;                      // Stores unscaled decimal portion of
                                       // temperature for output.

   enter_DefaultMode_from_RESET();

   //printf requires that the UART be primed by setting TI
   SCON0_TI = 1;

   IE_EA = 1;                             // Enable all interrupts

   printf("\f");                       // Clear the screen before outputs

   while (1)                           // Spin forever
   {
      if(CONV_COMPLETE)
      {
         temp_scaled = ADC_SUM;        // Apply our derived equation to ADC

         if (REVID == REV_B)
         {
            temp_scaled *= SLOPE_REV_B;

            // With a left-justified ADC, we have to shift the decimal place
            // of temp_scaled to the right so we can match the format of
            // OFFSET. Once the formats are matched, we can subtract OFFSET.
            temp_scaled = temp_scaled >> OVER_ROUND;

            temp_scaled -= OFFSET_REV_B;
         }
         else
         {
            temp_scaled *= SLOPE_REV_C;

            // With a left-justified ADC, we have to shift the decimal place
            // of temp_scaled to the right so we can match the format of
            // OFFSET. Once the formats are matched, we can subtract OFFSET.
            temp_scaled = temp_scaled >> OVER_ROUND;

            temp_scaled -= OFFSET_REV_C;
         }

         // Calculate the temp's whole number portion by unscaling
         temp_whole = temp_scaled/SCALE;

         // The temp_frac value is the unscaled decimal portion of temperature
         temp_frac = (int16_t)((temp_scaled - temp_whole*SCALE) / (SCALE/10));

         if(temp_frac < 0)
         {
            temp_frac *= -1;           // If negative, remove negative from
                                       // temp_frac portion for output.
         }

         printf(" T = %ld.%d(C) \n", temp_whole, temp_frac);

         CONV_COMPLETE = 0;
      }
   }
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
