//-----------------------------------------------------------------------------
// EFM8LB1_ADC_Autoscan_Large_Buffer.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the use of the ADC's Autoscan feature to fill a
// 2048 sample buffer using the EFM8 ADC peripheral driver library.
//
// The ADC's Autoscan feature can be used to fill a buffer with up to 64 samples
// without any CPU intervention. In this example, a single 2048 sample buffer is
// successively filled, 64 samples at a time, using autoscan mode.
//
// Timer 2 is set to trigger ADC conversions at 64 Hz. Each conversion fills one
// place in the current buffer. After the buffer is full (one second of samples),
// the ADC Conversion Complete callback is called, allowing the next buffer to be
// set as the active buffer.
//
// Inside the main loop, the CPU waits in Idle mode until an interrupt is triggered.
// This occurs once an autoscan is complete. After this, the full buffer's contents
// are printed out through UART, while the next buffer is loaded as the next
// autoscan buffer.
//
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   ADC    - 14-bit, VREF = VDD (3.3V)
//   UART0  - 115200 baud, 8-N-1
//   Timer1 - UART0 clock source
//   Timer2 - ADC start of conversion source
//   P0.4   - UART0 TX
//   P0.5   - UART0 RX
//   P1.7   - Joystick - ADC input
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
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the JLink CDC UART Port at 115200 baud rate and 8-N-1.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) Once per second, the program will output the results of 64 samples
//    of the joystick.
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
#include "retargetserial.h"
#include "bsp.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT(DISP_EN, SFR_P3, 4);            // Display Enable
#define DISP_BC_DRIVEN   0              // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1              // 1 = EFM8 drives display
SI_SBIT(BC_EN, SFR_P2, 2);              // Board Controller Enable
#define BC_DISCONNECTED 0               // 0 = Board Controller disconnected
                                        //     to EFM8 UART pins
#define BC_CONNECTED    1               // 1 = Board Controller connected
                                        //     to EFM8 UART pins
//-----------------------------------------------------------------------------
// ADC Defines
//-----------------------------------------------------------------------------
#define VREF_MV         (3300UL)
#define ADC_MAX_RESULT  ((1 << 14)-1) // 14 bit ADC

//-----------------------------------------------------------------------------
// Autoscan buffer initialization
//-----------------------------------------------------------------------------
#define BUFFER_SIZE (2048)      // The size, in samples, of the autoscan
                                // buffer BUFFER

#define SAMPLES_PER_SCAN (64)
// The starting address of autoscan buffer BUFFER in XDATA space.
// MUST END ON AN EVEN ADDRESS!
#define BUFFER_START_ADDR (0x0000)
ADC0_NEW_AUTOSCAN_BUFFER_ARRAY(BUFFER, BUFFER_SIZE, BUFFER_START_ADDR);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
uint16_t ADC0_convertSampleToMillivolts(uint16_t sample);
void FindMinMaxAverage(void);
void StartAutoscan(void);
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
bool BUFFER_FULL = false;
uint16_t BUFFER_INDEX = 0;

uint16_t MAX_mv;
uint16_t MIN_mv;
uint16_t AVG_mv;

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
void main(void)
{
  enter_DefaultMode_from_RESET();

  SCON0_TI = 1;                       // Enable UART printf
  BC_EN = BC_CONNECTED;               // UART is connected to the board
                                      // controller's virtual com port

  DISP_EN = DISP_BC_DRIVEN;           // Display not driven by EFM8

  StartAutoscan();

  while (1)
  {
    // Idle the CPU until the next interrupt
    PCON0 |= PCON0_IDLE__IDLE;

    if (BUFFER_FULL)
    {
      FindMinMaxAverage();
      RETARGET_PRINTF("Samples taken: %u | Min (mV): %u | Max (mV): %u | Avg (mV): %u\n", BUFFER_SIZE, MIN_mv, MAX_mv, AVG_mv);

      // Pause here until PB0 is pressed and released.
      while(BSP_PB0 == BSP_PB_UNPRESSED);
      while(BSP_PB0 == BSP_PB_PRESSED);
      StartAutoscan();
    }
  }
}

//-----------------------------------------------------------------------------
// StartAutoscan
//-----------------------------------------------------------------------------
//
// Starts a new Autoscan to fill up BUFFER. Clears all global variables to
// to their original states and begins the first scan. Subsequent scans
// are handled in the ADC ISR.
//
void StartAutoscan(void)
{
  BUFFER_FULL = false;
  BUFFER_INDEX = 0;
  // The first scan will begin at the start of BUFFER.
  ADC0_enableAutoscan(&BUFFER[BUFFER_INDEX], SAMPLES_PER_SCAN, ADC0_AUTOSCAN_MODE_CONTINUOUS);

  // The scan that will start after this one completes will begin SAMPLES_PER_SCAN ahead
  // of the first.
  BUFFER_INDEX += SAMPLES_PER_SCAN;
  ADC0_setNextAutoscanBuffer(&BUFFER[BUFFER_INDEX], SAMPLES_PER_SCAN);

  // Start the first scan.
  ADC0_startConversion();
}

#define MIN(a,b) (a<b?a:b)
#define MAX(a,b) (a>b?a:b)

//-----------------------------------------------------------------------------
// FindMinMaxAverage
//-----------------------------------------------------------------------------
//
// Finds the minimum, maximum, and average value for the values within
// BUFFER.
//
void FindMinMaxAverage(void)
{
  uint32_t sum = 0;
  uint16_t average;
  uint16_t min = 0xFFFF;
  uint16_t max = 0;
  uint16_t i;
  uint16_t sample;
  for (i = 0; i < BUFFER_SIZE; i++)
  {
    sample = BUFFER[i];
    min = MIN(min,sample);
    max = MAX(max,sample);
    sum += sample;
  }
  average = (uint16_t)(sum/BUFFER_SIZE);
  MAX_mv = ADC0_convertSampleToMillivolts(max);
  MIN_mv = ADC0_convertSampleToMillivolts(min);
  AVG_mv = ADC0_convertSampleToMillivolts(average);
}

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

bool ADC0_isAutoscanEnabled(void)
{
  return ((ADC0ASCF & ADC0ASCF_ASEN__BMASK) == ADC0ASCF_ASEN__START_SCAN);
}

//-----------------------------------------------------------------------------
// ADC0EOC_ISR
//-----------------------------------------------------------------------------
// ADC0 End of Conversion interrupt function
//
// Called once an ADC0 conversion interrupt fires. In this example, this
// occurs when an Autoscan is complete.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC0EOC_ISR, ADC0EOC_IRQn)
{
  static bool lastScan = false;
  ADC0_clearIntFlags(ADC0_CONVERSION_COMPLETE_IF);

  // If this completed scan was the last one, set the BUFFER_FULL flag
  // and return to let main() process the buffer.
  if (lastScan)
  {
    lastScan = false;
    BUFFER_FULL = true;
    return;
  }

  // Start the next scan
  ADC0_startConversion();

  // If the buffer isn't full, set the next autoscan to fill up the
  // next SAMPLES_PER_SCAN of the buffer. If the current scan that
  // is being executed is the last, disable Autoscan so that it
  // will be disabled after the last scan completes.
  BUFFER_INDEX += SAMPLES_PER_SCAN;
  if (BUFFER_INDEX < BUFFER_SIZE)
  {
    ADC0_setNextAutoscanBuffer(&BUFFER[BUFFER_INDEX], SAMPLES_PER_SCAN);
  }
  else
  {
    ADC0_disableAutoscan();
    lastScan = true;
  }
}
