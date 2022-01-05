//-----------------------------------------------------------------------------
// EFM8BB52_ADC_Autoscan_Multiple_Inputs.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the use of the ADC's Autoscan feature, using the
// EFM8 ADC peripheral driver library, to sample two consecutive ADC inputs 32
// times each without CPU intervention.
//
// The ADC's Autoscan feature can be used to scan multiple consecutive ADC
// channels  without CPU intervention. In Autoscan mode, a conversion complete
// interrupt is only triggered after all conversions in the scan have been
// completed.
//
// Timer 2 is set to trigger ADC conversions at 64 Hz
// Each conversion trigger causes the ADC to complete an entire scan. In this
// case, 64 conversions are completed, 32 from each of the two input channels.
// The two input channels are:
//    -P1.7 (Joystick)
//    -P2.0
//
// Inside the main loop, the CPU waits in Idle mode until an interrupt is
// triggered. This occurs once an autoscan is complete. After this, the results
// from the scan are printed out over UART.
//
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   ADC    - 12-bit, VREF = VDD (3.3V)
//   UART0  - 115200 baud, 8-N-1
//   Timer1 - UART0 clock source
//   Timer3 - ADC start of conversion source
//   P1.7   - Joystick / ADC sample channel 1
//   P2.0   - ADC sample channel 2
//   P0.4   - UART0 TX
//   P0.5   - UART0 RX
//   P2.2   - Board Controller enable
//   P3.4   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB52 Pro Kit
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB52 Pro Kit board to a PC using a USB cable.
// 3) Compile and download code to the EFM8BB52 Pro Kit board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the JLink CDC UART Port at 115200 baud rate and 8-N-1.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) Once per second, the program will output the results (in mV) of scanning
//    the following inputs:
//    -P1.7 (Joystick)
//    -P2.0
//
// Target:         EFM8BB52
// Tool chain:     Generic
//
// Release 1.0 (RN)
//    - Initial Revision
//    - Feb 2021
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB52_Register_Enums.h>
#include "InitDevice.h"
#include "adc_0.h"
#include "retargetserial.h"
#include "endian.h"

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
#define ADC_MAX_RESULT  ((1 << 12)-1)           // 12 bit ADC

#define START_CHANNEL (ADC0_POSITIVE_INPUT_P9)  // P1.7 (Joystick) is the ADC's
                                                // first channel

#define NUM_CHANNELS (2)   // Max 4
#define NUM_SCANS (32)     // Number of samples to take per channel
                           // Max (64/NUM_CHANNELS)
//-----------------------------------------------------------------------------
// Autoscan buffer initialization
//-----------------------------------------------------------------------------
#define BUFFER_SIZE (NUM_CHANNELS * NUM_SCANS) // The size, in 12-bit samples,
                                               // of the autoscan buffer BUFFER

// The starting address of autoscan buffer BUFFER in XDATA space.
// MUST END ON AN EVEN ADDRESS!
#define BUFFER_START_ADDR (0x0000)

// The scan samples will be placed in the buffer in the following order, if
// four channels are sampled starting at ADC0_POSITIVE_INPUT_P13
// (see ADC0_PositiveInput_t for more information):
// ADC0_POSITIVE_INPUT_P9,     //!< P1.7 (joystick) is ADC Positive Input
// ADC0_POSITIVE_INPUT_P10,     //!< P2.0 is ADC Positive Input
// This structure will map the ADC's samples to the correct input.
typedef struct {
  uint16_t p1_7;
  uint16_t p2_0;
}scan_t;

// Declare the buffer that will store the samples.
// Must be in XDATA space, must start at an even address!
SI_LOCATED_VARIABLE_NO_INIT(BUFFER[NUM_SCANS], scan_t, SI_SEG_XDATA,
                            BUFFER_START_ADDR);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
bool CONVERSION_COMPLETE = false;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
uint16_t ADC0_convertSampleToMillivolts(uint16_t sample);
void PrintScan(void);

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
 
#if LSB == 0
void convertEndianess(ADC0_AutoscanBuffer_t * buffer, uint8_t numElements);
#endif
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

  // Initialize Autoscan mode
  ADC0_setAutoscanInputs(START_CHANNEL, NUM_CHANNELS);
  ADC0_enableAutoscan((ADC0_AutoscanBuffer_t *)BUFFER, BUFFER_SIZE,
                       ADC0_AUTOSCAN_MODE_CONTINUOUS);

  while (1) {
    // Idle the CPU until the next interrupt
    PCON0 |= (PCON0_CPUIDLE__BMASK << PCON0_CPUIDLE__SHIFT);

    if (CONVERSION_COMPLETE) {
      CONVERSION_COMPLETE = false;
      
#if LSB == 0
      convertEndianess((ADC0_AutoscanBuffer_t *)BUFFER, BUFFER_SIZE);
#endif
      
      PrintScan();
    }
  }
}

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
// PrintScan
//-----------------------------------------------------------------------------
//
// Prints the results (in mV) of every sample in the autoscan.
//
void PrintScan(void)
{
  uint8_t scan;
  uint16_t p1_7_mV, p2_1_mV;
  RETARGET_PRINTF("\n\f    SCAN    | P1.7 (mV) |  P2.0 (mV) \n");
  for (scan = 0; scan < NUM_SCANS; scan++) {
    p1_7_mV = ADC0_convertSampleToMillivolts(BUFFER[scan].p1_7);
    p2_1_mV = ADC0_convertSampleToMillivolts(BUFFER[scan].p2_0);
    RETARGET_PRINTF("     %d    ", (int16_t)scan);
    RETARGET_PRINTF("|      %d     ", (int16_t)p1_7_mV);
    RETARGET_PRINTF("|      %d     \n", (int16_t)p2_1_mV);
  }
  RETARGET_PRINTF("\n");
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
  // The measured voltage is given by:
  //
  //                           Vref (mV)
  //   measurement (mV) =   --------------- * result (bits)
  //                       (2^12)-1 (bits)
  return ((uint32_t)sample * VREF_MV) / ADC_MAX_RESULT;
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
  ADC0_clearIntFlags(ADC0_CONVERSION_COMPLETE_IF);
  CONVERSION_COMPLETE = true;
}
