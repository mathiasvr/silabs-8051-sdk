//-----------------------------------------------------------------------------
// EFM8BB2_UART1_Lib_STDIO.c
//-----------------------------------------------------------------------------
// Copyright 2016 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the EFM8BB2 to use routines
// in UART1 peripheral driver (stdio configuration) to write to and read from
// the UART1 interface. The program reads a character using the UART peripheral
// driver routine _getkey() and outputs that character to the screen.
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
// 6) Follow the prompts in the terminal program.
//
// Target:         EFM8BB2
// Tool chain:     Generic
//
// Release 0.1 (ST;AT)
//    - Initial Revision
//    - 02 June 2016
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "bsp.h"
#include "uart_1.h"
#include "InitDevice.h"
#include <stdio.h>

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
   unsigned char input_char;           // Used to store character from UART

   enter_DefaultMode_from_RESET();
   UART1_initStdio(24500000, 115200);

   BSP_DISP_EN = BSP_DISP_BC_DRIVEN;   // Display not driven by EFM8

   BSP_BC_EN = BSP_BC_CONNECTED;       // Board controller connected to EFM8
                                       // UART pins
   while (1)
   {
    printf ("\r\nEnter character: ");
    input_char = getchar();
    printf ("\r\nCharacter entered: %c", input_char);
    printf ("\r\n     Value in Hex: %bx", input_char);
   }
}
