//-----------------------------------------------------------------------------
// F85x_TemperatureSensor_WithCompensation.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program shows the use of the temperature sensor along with the ADC.
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
// 8) To change the offset compensation calibration of the temperature sensor,
//    send a string to the part using the terminal program. The string must
//    have the following format:
//
//    245\n
//
//    where 245 indicates the current ambient temperature is 24.5C. The
//    program will automatically calculate and program the proper offset to
//    adjust the temperature sensor.
//
//
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
// 8) To change the offset compensation calibration of the temperature sensor,
//    send a string to the part using the terminal program. The string must
//    have the following format:
//
//    245\n
//
//    where 245 indicates the current ambient temperature is 24.5C. The
//    program will automatically calculate and program the proper offset to
//    adjust the temperature sensor.
//
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
// Revision History:
//
//
// Release 1.3 (BL)
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
#include <stdio.h>
#include <stdlib.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define TRUE                         1
#define FALSE                        0

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

#define COMP_ADDRESS            0x1C00 // Location of compensation

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
bit NEW_CAL;                           // Flag - new calibration value available
int16_t CAL_VAL;                           // Calibration value
uint32_t ADC_SUM = 0;                       // Accumulates the ADC samples
bit CONV_COMPLETE = FALSE;             // ADC_SUM result ready flag

// Flash stored compensation
SI_LOCATED_VARIABLE_NO_INIT(COMPENSATION, uint32_t, code, COMP_ADDRESS);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void Calibrate(int16_t cal_input, int32_t temp);

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
   int32_t temp_scaled;                    // Scaled temperature value
   int32_t temp_whole;                     // Stores integer portion for output
   int32_t temp_comp;                      // Scaled and compensated temp
   int16_t temp_frac = 0;                  // Stores fractional portion of temp

   enter_DefaultMode_from_RESET();

   //the printf being used requires that the transmit complete
   //  interrupt be set prior to the first byte being transmitted
   SCON0_TI = SCON0_TI__SET;

   IE_EA = 1;                             // Enable all interrupts

   printf("\f");                       // Clear screen before output begins

   while (1)                           // Spin forever
   {
      if (CONV_COMPLETE)               // If ADC completed
      {
         // Calculate the temperature with rounding

         // Round sum to proper precision first
         temp_scaled = (int32_t) ADC_SUM;

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

         temp_comp = temp_scaled - COMPENSATION; //Apply compensation

         // Take the whole number portion unscaled
         temp_whole = temp_comp/SCALE;

         // The temp_frac value is the unscaled decimal portion of
         // temperature
         temp_frac = (int16_t)((temp_comp - temp_whole * SCALE) / (SCALE/10));

         // If the temperature is negative, remove the negative from
         // temp_frac portion for output.
         if(temp_frac < 0)
         {
            temp_frac *= -1;
         }

         printf("\r  T = %ld.%d(C)\n   ",temp_whole,temp_frac);

         CONV_COMPLETE = FALSE;

         if (NEW_CAL)                  // If new calibration data
         {
            Calibrate(CAL_VAL, temp_scaled); // Rewrite Calibration value
            NEW_CAL = FALSE;           // Reset flag
         }
      }

      if(SCON0_RI)  // Check if any key has been pressed to enter new calibration
      {
         scanf("%d", &CAL_VAL );
         printf("You entered %d as the new calibration value \n", CAL_VAL);
         NEW_CAL = TRUE;               // Set flag for new calibration
      }
   }
}

//-----------------------------------------------------------------------------
// Calibrate
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : cal - the value used as reference temperature
//                temp - the temperature measured at the moment
//
// This function rewrites the value of the compensation temperature, the
// difference between the reference temperature and the one being measured at
// that moment. This difference is saved in flash to be added to further
// measurements.
//
// This function erases the code page where the compensation variable is
// stored. This is done to enable the variable to be correctly modified. The
// content of the page is not saved because this is the only info stored in
// this page and there is no code in this page. A program that has code and/or
// data in this page must first save the content of the page before erasing it.
//
//-----------------------------------------------------------------------------
void Calibrate(int16_t cal_input, int32_t temp)
{
   bit EA_state = IE_EA;                  // Preserves IE_EA state
   uint8_t xdata * codePtr;                 // Used to write calibration
                                       // Value into flash memory
   SI_UU32_t COMP_VALUE;                    // Byte-addressable int32_t variable

   // Point to the compensation register that will contain the temperature
   // offset to be used with the temperature sensor
   codePtr = (uint8_t xdata*) &COMPENSATION;

   COMP_VALUE.s32 = (int32_t) cal_input;
   COMP_VALUE.s32 *= 100;
   COMP_VALUE.s32 = temp - (int32_t) COMP_VALUE.s32;

   IE_EA = 0;                             // Disable interrupts

   PSCTL = 0x02;                       // Enable page erase
   PSCTL |= 0x01;

   FLKEY = 0xA5;                       // Input first flash unlock code
   FLKEY = 0xF1;                       // Input second flash unlock code,

   *codePtr = 0xFF;                    // Write something in the page, which
                                       // initiates the erase process
   PSCTL = 0;                          // Disable all, flash locked again

   FLKEY = 0xA5;                       // Input first flash unlock code
   FLKEY = 0xF1;                       // Input second flash unlock code,
                                       // flash is now unlocked
   PSCTL |= 0x01;                      // Enable flash Writes

   // Write high byte of compensation
   *(codePtr++) = (uint8_t)(COMP_VALUE.u8[B3]);

   PSCTL &= ~0x01;                     // Disable flash Writes, flash locked

   FLKEY = 0xA5;                       // Input first flash unlock code
   FLKEY = 0xF1;                       // Input second flash unlock code,
                                       // flash is now unlocked

   PSCTL |= 0x01;                      // Enable flash Writes

   // Write 2nd byte of compensation
   *(codePtr++) = (uint8_t)(COMP_VALUE.u8[B2]);

   PSCTL &= ~0x01;                     // Disable flash Writes, flash locked

   FLKEY = 0xA5;                       // Input first flash unlock code
   FLKEY = 0xF1;                       // Input second flash unlock code,
                                       // flash is now unlocked

   PSCTL |= 0x01;                      // Enable flash Writes

   // Write 3rd byte of compensation
   *(codePtr++) = (uint8_t)(COMP_VALUE.u8[B1]);

   PSCTL &= ~0x01;                     // Disable FLASH Writes

   FLKEY = 0xA5;                       // Input first flash unlock code
   FLKEY = 0xF1;                       // Input second flash unlock code,
                                       // FLASH is now unlocked

   PSCTL |= 0x01;                      // Enable FLASH Writes

   // Write last byte of compensation
   *codePtr = (uint8_t)(COMP_VALUE.u8[B0]);

   PSCTL = 0;                          // Flash locked
   IE_EA = EA_state;                      // Restore interrupt state
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
