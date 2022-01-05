//-----------------------------------------------------------------------------
// EFM8BB52_UART0_STDIO.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the EFM8BB52 to use routines
// in STDIO.h to write to and read from the UART0 interface.  The program
// reads a character using the STDIO routine getchar(), outputs that character
// to the screen, and then outputs the ASCII hex value of that character.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   UART0  - 115200 baud, 8-N-1
//   Timer1 - UART0 clock source
//   P0.4   - UART0 TX
//   P0.5   - UART0 RX
//   P2.2   - Board Controller VCOM enable
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
// 6) Follow the prompts in the terminal program.
//
// Target:         EFM8BB52
// Tool chain:     Generic
//
// Release 0.1 (DS)
//    - Initial Revision
//    - 21 JAN 2021
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB52_Register_Enums.h>
#include "retargetserial.h"
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P3, 4);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (BC_VCOM_EN, SFR_P2, 2);       // Board Controller Enable
#define BC_VCOM_DISCONNECTED 0         // 0 = Board Controller disconnected
                          //     to EFM8 UART pins
#define BC_VCOM_CONNECTED    1         // 1 = Board Controller connected
                          //     to EFM8 UART pins

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
  unsigned char inputcharacter;       // Used to store character from UART

  enter_DefaultMode_from_RESET();

  DISP_EN = DISP_BC_DRIVEN;           // EFM8 does not drive display

  BC_VCOM_EN = BC_VCOM_CONNECTED;     // Board controller connected to EFM8
                                      // UART pins

  SCON0_TI = 1;                       // This STDIO library requires TI to
                                      // be set for prints to occur

  while (1)
  {
    RETARGET_PRINTF ("\nEnter character: ");
    inputcharacter = getchar();
    RETARGET_PRINTF ("\nCharacter entered: %c", inputcharacter);
    RETARGET_PRINTF ("\n     Value in Hex: %bx", inputcharacter);
  }
}
