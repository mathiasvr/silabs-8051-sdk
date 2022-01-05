//-----------------------------------------------------------------------------
// EFM8BB52_UART_Lib_Buffer.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the EFM8BB52 to use routines
// in UART peripheral driver (buffer configuration) to write to and read from
// the UART0 interface. The program reads five lower case characters,
// capitalizes the letters, then outputs the capitalized letters.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   UART0  - 115200 baud, 8-N-1
//   Timer1 - UART0 clock source
//   P0.4   - UART0 TX
//   P0.5   - UART0 RX
//   P2.2   - Board Controller enable (set high to enable board controller functionality)
//   P2.3   - Display enable
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
// 6) Type five lower case letters. Note: terminal will not display anything
//    until five letters are entered.
// 7) Observe the capitalized input in the terminal.
//
// Target:         EFM8BB52
// Tool chain:     Generic
//
// Release 0.1 (SP)
//    - Initial Revision
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "bsp.h"
#include "uart_0.h"
#include "InitDevice.h"
#include "retargetserial.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define BUFFER_LENGTH   5

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
SI_SEGMENT_VARIABLE(buffer[BUFFER_LENGTH], uint8_t, SI_SEG_XDATA);

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup(void)
{
  // Disable the watchdog here
}
 
//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main(void) 
{
  enter_DefaultMode_from_RESET();

  // Initialize the UART
  UART0_init(UART0_RX_ENABLE, UART0_WIDTH_8, UART0_MULTIPROC_DISABLE);

  // Display not driven by EFM8
  BSP_DISP_EN = BSP_DISP_BC_DRIVEN;

  // Board controller connected to EFM8 UART pins
  BSP_VCOM_EN = BSP_BC_CONNECTED;
  IE_EA = 1;

  while (1) 
  {
    if ((UART0_rxBytesRemaining() == 0) && (UART0_txBytesRemaining() == 0)) 
	{
      UART0_readBuffer(buffer, BUFFER_LENGTH);
    }
  }
}

//-----------------------------------------------------------------------------
// UART0 ISR Callbacks
//-----------------------------------------------------------------------------
void UART0_receiveCompleteCb() 
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
  UART0_writeBuffer(buffer, BUFFER_LENGTH);
}

void UART0_transmitCompleteCb() {}