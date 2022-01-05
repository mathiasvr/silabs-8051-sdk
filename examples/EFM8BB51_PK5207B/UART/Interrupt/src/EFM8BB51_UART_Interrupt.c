//-----------------------------------------------------------------------------
// EFM8BB51_UART0_Interrupt.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the EFM8BB51 to write to and
// read from the UART0 interface. The program reads a word using the UART0
// interrupt and outputs that word to the screen, with all characters in
// uppercase.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   UART0  - 115200 baud, 8-N-1
//   Timer1 - UART0 clock source
//   P0.0   - Board Controller VCOM enable
//   P0.4   - UART0 TX
//   P0.5   - UART0 RX
//   P2.0   - Display enable, shared by C2D pin
//            See AN127 for C2D pin sharing information
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB51 Pro Kit
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB51 Pro Kit board to a PC using a USB cable.
// 3) Compile and download code to the EFM8BB51 Pro Kit board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the JLink CDC UART Port at 115200 baud rate and 8-N-1.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) Using a terminal program on the PC, input any number of lower-case
//    characters, up to UART_BUFFERSIZE (default 64), followed by either
//    a carriage return ('\r'), a newline character ('\n'), or a tilda ('~').
//    The program will change the input characters to upper-case and output
//    them over UART.
//
// Target:         EFM8BB51
// Tool chain:     Generic
//
// Release 0.1 (DS)
//    - Initial Revision
//    - 21 JAN 2021
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB51_Register_Enums.h>
#include "retargetserial.h"
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 0);          // Display Enable, shared by C2D pin
                          // See AN127 for C2D pin sharing info
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (BC_VCOM_EN, SFR_P0, 0);       // Board Controller VCOM Enable
#define BC_VCOM_DISCONNECTED 0         // 0 = Board Controller disconnected
                          //     to EFM8 UART pins
#define BC_VCOM_CONNECTED    1         // 1 = Board Controller connected
                          //     to EFM8 UART pins

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
// Variables in Interrupts.c
extern uint8_t UART_Buffer_Size;
extern uint8_t TX_Ready;
extern uint8_t Byte;

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

  DISP_EN = DISP_BC_DRIVEN;         // EFM8 does not drive display

  BC_VCOM_EN = BC_VCOM_CONNECTED;   // Board controller connected to EFM8
                                    // UART pins
  IE_EA = 1;

  while (1)
  {
    // If the complete word has been entered via the terminal followed by
    // carriage return or newline

    if ((TX_Ready == 1) && (UART_Buffer_Size != 0) && ((Byte == '\r') || (Byte == '\n') || (Byte == '~')))
    {
      TX_Ready = 0;                 // Set the flag to zero
      SCON0_TI = 1;                 // Set transmit flag to 1
    }
  }
}
