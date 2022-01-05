//-----------------------------------------------------------------------------
// EFM8BB2_UART1_Lib_Buffer.c
//-----------------------------------------------------------------------------
// Copyright 2016 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the EFM8BB2 to use routines
// in UART1 peripheral driver (buffer configuration) to write to and read from
// the UART1 interface. The program reads five lower case characters,
// capitalizes the letters, then outputs the capitalized letters.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   UART1  - 115200 baud, 8-N-1
//   P0.4   - UART1 TX, Board Controller TX
//   P0.5   - UART1 RX, Board Controller RX
//   P2.0   - Board Controller enable (set high to enable board controller functionality)
//   P2.3   - Display enable
//     
//-----------------------------------------------------------------------------
// How To Test: EFM8BB2 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8BB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the JLink CDC UART Port at 115200 baud rate and 8-N-1.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) Type five lower case letters. Note: terminal will not display anything
//    until five letters are entered.
// 7) Observe the capitalized input in the terminal.
//
// Target:         EFM8BB2
// Tool chain:     Generic
//
// Release 0.1 (ST; AT)
//    - Initial Revision
//    - 02 JUNE 2016
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "bsp.h"
#include "uart_1.h"
#include "InitDevice.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define BUFFER_LENGTH   5

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
SI_SEGMENT_VARIABLE(buffer[BUFFER_LENGTH], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(errorCount, uint32_t, SI_SEG_XDATA) = 0;

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
   enter_DefaultMode_from_RESET();
   UART1_init(24500000, 115200, UART1_DATALEN_8, UART1_STOPLEN_SHORT,
              UART1_FEATURE_DISABLE, UART1_PARITY_ODD, UART1_RX_ENABLE, UART1_MULTIPROC_DISABLE);
   UART1_initTxFifo(UART1_TXTHRSH_ZERO, UART1_TXFIFOINT_DISABLE);
   UART1_initRxFifo(UART1_RXTHRSH_ZERO, UART1_RXTIMEOUT_16, UART1_RXFIFOINT_ENABLE);

   BSP_DISP_EN = BSP_DISP_BC_DRIVEN;   // Display not driven by EFM8

   BSP_BC_EN = BSP_BC_CONNECTED;       // Board controller connected to EFM8
                                       // UART pins
   IE_EA = 1;

   while (1)
   {
      if ((UART1_rxBytesRemaining() == 0) && (UART1_txBytesRemaining() == 0))
      {
        UART1_readBuffer(buffer, BUFFER_LENGTH);
      }
   }
}

//-----------------------------------------------------------------------------
// UART ISR Callbacks
//-----------------------------------------------------------------------------
void UART1_receiveCompleteCb ()
{
   uint8_t i;
   unsigned char byte;

   for (i = 0; i < BUFFER_LENGTH; i++)
   {
      byte = buffer[i];

      // if lower case letter
      if ((byte >= 'a') && (byte <= 'z'))
      {
         byte -= 32;
      }

      buffer[i] = byte;
   }
   UART1_writeBuffer(buffer, BUFFER_LENGTH);
}

void UART1_transmitCompleteCb ()
{
}

void UART1_transferErrorCb(uint8_t error)
{
  if(error & UART1_PARITY_EF)
  {
    errorCount++;
  }

  if(error & UART1_RXOVR_EF)
  {
    errorCount++;
  }
}
