//-----------------------------------------------------------------------------
// EFM8BB52_ADC_Lib_Accumulate_and_Average.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates using the EFM8 ADC peripheral driver library to
// sample a voltage in interrupt mode, accumulating and averaging an
// additional thirty-two samples in hardware to improve noise performance.
//
// In this example, the ADC is configured to sample the internal temperature
// sensor.
//
// The ADC is configured initially to accumulate 32 samples and divide the
// result by two (right shift by 1) for each conversion. 64 samples are taken
// in this mode, and the standard deviation (in millivolts) and mean (in volts)
// are calculated. The ADC is then configured to take 64 samples without
// accumulation and averaging. The standard deviation and mean of these samples
// are also calculated. The results from both sets of calculations are then
// printed over UART.
//
// After the results are printed, the MCU is put into Idle mode to conserve
// power. Once every two seconds, Timer3 interrupts, causing the device to
// leave Idle mode and re-run the sampling and calculations.
//
// For more information on how accumulating and averaging can improve ADC
// performance, please see application note AN118.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1.5 -> 16.33 MHz
//   ADC    - 12-bit, VREF = Internal 1.65 VREF
//            SARCLK = SYSCLK (16.33 MHz)
//   Timer3 - MCU Wake-up source, every 2 seconds.
//   Timer1 - UART0 clock source, 234500 Hz
//   UART0  - 115200 baud
//   P2.2   - Board Controller enable
//   P3.4   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB52 PK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB52 PK board to a PC using a USB cable.
// 3) Compile and download code to the EFM8BB52 PK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the JLink CDC UART Port at 115200 baud rate and 8-N-1.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) Once every two seconds, the device will output the standard deviation and
//    average value for each of the two sampling methods - accumulating and
//    averaging and single-sample. The standard deviation for the accumulated
//    samples should be approximately 1/4 the standard deviation of the
//    single samples.
//
// Target:         EFM8BB52
// Tool chain:     Generic
//
// Release 0.1 (JS)
//    - Initial Revision
//    - 2021
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB52_Register_Enums.h>
#include "InitDevice.h"
#include "adc_0.h"
#include "retargetserial.h"
#include "math.h"
#include "endian.h"
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

//-----------------------------------------------------------------------------
// ADC Defines
//-----------------------------------------------------------------------------
#define VREF_V         (1.65)
#define ADC_MAX_RESULT_ACCUMULATED  (0x10000UL - 16) // 12 bit ADC oversampled to 14 bits
#define ADC_MAX_RESULT_SINGLE  (0x1000UL - 1) // 12 bit ADC result

// Declare two autoscan buffers to hold 64 samples each.
#define NUM_SAMPLES  64
ADC0_NEW_AUTOSCAN_BUFFER_ARRAY(ACCUMULATED,NUM_SAMPLES, 0x0000);
ADC0_NEW_AUTOSCAN_BUFFER_ARRAY(SINGLESAMPLE,NUM_SAMPLES, 0x0000 + sizeof(ACCUMULATED[0]) * NUM_SAMPLES);

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
// Function Prototypes
//-----------------------------------------------------------------------------
float ADC0_convertSampleToVoltage(float sample, uint16_t maxResult);
void printResults(void);
void fillBuffers(void);
float AVG(SI_VARIABLE_SEGMENT_POINTER(buffer, const uint16_t, SI_SEG_GENERIC), uint8_t size);
float STDDEV(SI_VARIABLE_SEGMENT_POINTER(buffer, const uint16_t, SI_SEG_GENERIC), uint8_t size);
//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main (void)
{
  enter_AccumulateMode_from_RESET();

  SCON0_TI = 1;

  DISP_EN = DISP_BC_DRIVEN;           // Display not driven by EFM8
  BC_EN = BC_CONNECTED;

  while (1)
  {
      // Fill the two 64-element buffers with samples using Autoscan.
      fillBuffers();

      // Print the resulting STDDEV and AVG for each buffer.
      printResults();

      // Put the MCU into Idle mode to conserve power. The MCU will be brought
      // out of this mode by a Timer3 interrupt, set to fire every two seconds.
      PCON0 |= (PCON0_CPUIDLE__BMASK << PCON0_CPUIDLE__SHIFT);
  }
}

///////////////////////////////////////////////////////////////////////////////
// Supporting Functions
///////////////////////////////////////////////////////////////////////////////

#if LSB == 0
//-----------------------------------------------------------------------------
// convertEndianess
//-----------------------------------------------------------------------------
//
// Convert 16-bit big-endian values to host format (ie IAR is little endian)
//
//
void convertEndianess(ADC0_AutoscanBuffer_t * buffer, uint8_t numElements)
{
  uint8_t i;
  
  for (i = 0; i < numElements; i++)
  {
    buffer[i] = be16toh(buffer[i]);
  }
}
#endif

//-----------------------------------------------------------------------------
// fillBuffers
//-----------------------------------------------------------------------------
//
// Uses the ADC to fill the two buffers ACCUMULATED[] and SINGLESAMPLE[].
//
// This function uses the ADC's autoscan functionality to store 64 samples
// in each buffer using only one conversion trigger.
//
//
void fillBuffers(void)
{
  // Fill the ACCUMULATED Buffer
  ADC0_enableAutoscan(ACCUMULATED, NUM_SAMPLES, ADC0_AUTOSCAN_MODE_SINGLE);
  ADC0_startConversion();
  while(!ADC0_isConversionComplete());  // Wait for scan to complete

#if LSB == 0
  convertEndianess(ACCUMULATED, NUM_SAMPLES);
#endif
  
  // Change the ADC settings to single sample mode for the next buffer
  enter_SingleSampleMode_from_AccumulateMode();

  // Fill the SINGLESAMPLE buffer
  ADC0_enableAutoscan(SINGLESAMPLE, NUM_SAMPLES, ADC0_AUTOSCAN_MODE_SINGLE);
  ADC0_startConversion();
  while(!ADC0_isConversionComplete()); // Wait for scan to complete

#if LSB == 0  
  convertEndianess(SINGLESAMPLE, NUM_SAMPLES);
#endif
  
  // Change the ADC settings to Accumulate mode for the next buffer
  enter_AccumulateMode_from_SingleSampleMode();
}

//-----------------------------------------------------------------------------
// printResults
//-----------------------------------------------------------------------------
//
// Calculates the mean and standard deviation for both the ACCUMULATED and
// SINGLESAMPLE buffers, then converts these values to V and mV, respectively,
// then prints these results out over UART.
//
void printResults(void)
{
  float accStddev_mV, accAvg_V;
  float singleStddev_mV, singleAvg_v;

  accStddev_mV = ADC0_convertSampleToVoltage(STDDEV(ACCUMULATED, NUM_SAMPLES), ADC_MAX_RESULT_ACCUMULATED) * 1000;
  accAvg_V = ADC0_convertSampleToVoltage(AVG(ACCUMULATED, NUM_SAMPLES), ADC_MAX_RESULT_ACCUMULATED);

  singleStddev_mV = ADC0_convertSampleToVoltage(STDDEV(SINGLESAMPLE, NUM_SAMPLES), ADC_MAX_RESULT_SINGLE) * 1000;
  singleAvg_v = ADC0_convertSampleToVoltage(AVG(SINGLESAMPLE, NUM_SAMPLES), ADC_MAX_RESULT_SINGLE);

  RETARGET_PRINTF("Accumulated and Averaged:: STDDEV: %1.3fmV , MEAN: %1.4fV | ", accStddev_mV, accAvg_V);
  RETARGET_PRINTF("Single Sample:: STDDEV: %1.3fmV , MEAN: %1.4fV\n", singleStddev_mV, singleAvg_v);
}

//-----------------------------------------------------------------------------
// AVG
//-----------------------------------------------------------------------------
//
// Calculates the average of all elements in a given buffer.
//
// -buffer : a buffer of unsigned 16-bit integers
// -size : the number of elements in the buffer
//
// -returns : the average of the given buffer
//
float AVG(SI_VARIABLE_SEGMENT_POINTER(buffer, const uint16_t, SI_SEG_GENERIC), uint8_t size)
{
  uint8_t i;
  uint32_t sum = 0;
  for (i = 0; i < size; i++)
  {
    sum += buffer[i];
  }
  return (float)sum/size;
}

//-----------------------------------------------------------------------------
// STDDEV
//-----------------------------------------------------------------------------
//
// Calculates the Standard Deviation for a given buffer.
//
// -buffer : a buffer of unsigned 16-bit integers
// -size : the number of elements in the buffer
//
// -returns : the standard deviation of the elements in the buffer
//
float STDDEV(SI_VARIABLE_SEGMENT_POINTER(buffer, const uint16_t, SI_SEG_GENERIC), uint8_t size)
{
  float stddev;
  float average;
  float sum = 0;
  float diff;
  uint8_t i;

  average = AVG(buffer, size);
  for (i = 0; i < size; i ++)
  {
    diff = (float)buffer[i] - average;
    sum += (diff*diff);
  }

  stddev = sqrt((float)sum/size);

  return stddev;

}

//-----------------------------------------------------------------------------
// ADC0_convertSampleToMillivolts
//-----------------------------------------------------------------------------
//
// Converts a raw ADC sample to a voltage representation.
// sample - the raw sample from the ADC
//
// returns - a float representing the sample voltage, e.g. 1.62
//
float ADC0_convertSampleToVoltage(float sample, uint16_t maxResult)
{
  // A sample can be converted to voltage using the following formula:
  //
  //                           Vref (mV)
  //   measurement (mV) =   --------------- * result (bits)
  //                          Max Result
  return ((float)sample*VREF_V) / maxResult;
}
