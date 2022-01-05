//-----------------------------------------------------------------------------
// EFM8UB1_Isolated_USB.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to isolate USB and is to be used in conjunction
// with the Isolated-USB-EK.  The program configures the EFM8UB1 to write to and
// read from the UART1 interface which is then isolated via the Isolated USB Expansion Board.
// The program reads a word using the UART0 interrupt and outputs that word
// to the screen, with all characters in uppercase.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   UART1  - 115200 baud, 8-N-1
//   Timer1 - UART1 clock source
//   P0.4   - UART1 TX
//   P0.5   - UART1 RX
//   P2.0   - Board Controller enable
//   P2.3   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8UB1 STK + Isolated USB EXP
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8UB1 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8UB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
// 5) Disconnect the USB cable from the UB1 STK.
// 6) Connect the Isolated USB Expansion Board in to the STK via the 20-pin header.
// 7) Connect the micro USB port on the Isolated USB Expansion Board to the host PC
// 8) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the CP2102N Port at 115200 baud rate and 8-N-1.
// 6) Using a terminal program on the PC, input any lower-case
//    character.  The program will change the input characters to upper-case and output
//    them.
//
// Target:         EFM8UB1
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 16 AUG 2017
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8UB1_Register_Enums.h>
#include <stdio.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 3);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (BC_EN, SFR_P2, 0);            // Board Controller Enable
#define BC_DISCONNECTED 0              // 0 = Board Controller disconnected
                                       //     to EFM8 UART pins
#define BC_CONNECTED    1              // 1 = Board Controller connected
                                       //     to EFM8 UART pins

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main (void)
{
   enter_DefaultMode_from_RESET();

   DISP_EN = DISP_BC_DRIVEN;           // EFM8 does not drive display

   BC_EN = BC_DISCONNECTED;               // Board controller connected to EFM8
                                       // UART pins
   IE_EA = 1;
   while(1)
   {
      //waiting for interrupts

   }
}
