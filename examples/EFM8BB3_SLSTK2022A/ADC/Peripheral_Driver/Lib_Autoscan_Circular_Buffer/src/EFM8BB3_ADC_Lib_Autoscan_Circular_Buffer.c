//-----------------------------------------------------------------------------
// EFM8BB3_ADC_Lib_Autoscan_Circular_Buffer.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the use of the ADC's Autoscan feature using the EFM8
// ADC peripheral driver library.
//
// The ADC's Autoscan feature can be used to fill a buffer with up to 64 samples
// without any CPU intervention. In this example, two buffers are used in
// ping-pong fashion.
//
// Timer 2 is set to trigger ADC conversions at 64 Hz. Each conversion fills one
// place in the current buffer. After the buffer is full (one second of samples),
// a conversion complete interrupt is triggered, allowing the next buffer to be
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
//   ADC    - 12-bit, VREF = VDD (3.3V)
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
// How To Test: EFM8BB3 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB3 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8BB3 STK board.
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
// Target:         EFM8BB3
// Tool chain:     Generic
//
// Release 0.1 (BL)
//    - Initial Revision
//    - 04 AUG 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB3_Register_Enums.h>
#include "InitDevice.h"
#include "adc_0.h"
#include "retargetserial.h"
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
// Global Constants
//-----------------------------------------------------------------------------
#define MAX_SAMPLES_PER_LINE 8  // How many samples to print per line over UART
                                // in ProcessBuffer()

//-----------------------------------------------------------------------------
// Autoscan buffer initialization
//-----------------------------------------------------------------------------
#define BUFFER_SIZE 64          // The size, in 12-bit samples, of the autoscan
                                // buffers BUFFER1 and BUFFER2

// The starting address of autoscan buffer BUFFER1 in XDATA space.
// MUST END ON AN EVEN ADDRESS!
#define BUFFER1_START_ADDR (0x0000)
ADC0_NEW_AUTOSCAN_BUFFER_ARRAY(BUFFER1, BUFFER_SIZE, BUFFER1_START_ADDR);

// The starting address of autoscan buffer BUFFER2 in XDATA space. This is placed
// immediately after BUFFER1
#define BUFFER2_START_ADDR (BUFFER1_START_ADDR + sizeof(BUFFER1))
ADC0_NEW_AUTOSCAN_BUFFER_ARRAY(BUFFER2, BUFFER_SIZE, BUFFER2_START_ADDR);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
ADC0_AutoscanBuffer_t * CURRENT_BUFFER;   // A pointer to an autoscan buffer
bool CONVERSION_COMPLETE = false;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void PrintBuffer(const ADC0_AutoscanBuffer_t * buffer, uint8_t size);
void Autoscan_init(void);

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

  Autoscan_init();                    // Initialize autoscan mode

  while (1)
  {
    // Idle the CPU until the next interrupt
    PCON0 |= PCON0_IDLE__IDLE;

    if (CONVERSION_COMPLETE)
    {
      CONVERSION_COMPLETE = false;
      PrintBuffer(CURRENT_BUFFER, BUFFER_SIZE);
    }
  }
}

//-----------------------------------------------------------------------------
// Autoscan_init
//-----------------------------------------------------------------------------
// Initialize the ADC's autoscan mode.
//
// This function enables the ADC's autoscan to use BUFFER1 as the first buffer,
// then BUFFER2 as the second buffer.
//
//-----------------------------------------------------------------------------
void Autoscan_init(void)
{

  CURRENT_BUFFER = BUFFER1;
  ADC0_enableAutoscan(CURRENT_BUFFER, BUFFER_SIZE, ADC0_AUTOSCAN_MODE_CONTINUOUS);
  ADC0_setNextAutoscanBuffer(BUFFER2, BUFFER_SIZE);

  // Fill the first buffer so that the first print isn't random garbage
  while(!CONVERSION_COMPLETE);
  CONVERSION_COMPLETE = false;
}


//-----------------------------------------------------------------------------
// PrintBuffer
//-----------------------------------------------------------------------------
//
// Prints the contents of a given autoscan buffer
//
// buffer - a pointer to an autoscan buffer
// size - the size, in elements (not bytes!) of the buffer
//
//
void PrintBuffer(const ADC0_AutoscanBuffer_t * buffer, uint8_t size)
{
  uint8_t i;
  uint8_t samples_per_line = 0;
  RETARGET_PRINTF("\n\fBuffer Address: 0x%04X\n", (uint16_t) buffer);
  RETARGET_PRINTF("Buffer Contents:\n");

  for (i = 0; i < size; i++)
  {
    RETARGET_PRINTF("0x%04X, ", buffer[i]);
    samples_per_line++;
    if (samples_per_line >= MAX_SAMPLES_PER_LINE)
    {
      samples_per_line = 0;
      RETARGET_PRINTF("\n");
    }
  }
}

//-----------------------------------------------------------------------------
// ADC0EOC_ISR
//-----------------------------------------------------------------------------
// ADC0 End of Conversion interrupt function
//
// Called once an ADC0 conversion interrupt fires. In this example, this
// occurs when an Autoscan is complete.
//
// This function keeps track of the recently filled buffer, then sets this
// buffer to be used after the current scan is complete (which will be using
// the other buffer).
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC0EOC_ISR, ADC0EOC_IRQn)
{
  ADC0_clearIntFlags(ADC0_CONVERSION_COMPLETE_IF);

  // Swap current buffers
  if (CURRENT_BUFFER == BUFFER1)
  {
    CURRENT_BUFFER = BUFFER2;
  }
  else
  {
    CURRENT_BUFFER = BUFFER1;
  }

  // Set the next autoscan buffer to be the newly filled buffer
  // This will be used after the current scan is complete,
  // which will already have started with the other buffer
  ADC0_setNextAutoscanBuffer(CURRENT_BUFFER, BUFFER_SIZE);

  CONVERSION_COMPLETE = true;
}
