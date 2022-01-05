//-----------------------------------------------------------------------------
// UART_STDIO.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the EFM8UB2 to use routines
// in STDIO.h to write to and read from the UART interface.  The program
// reads a character using the STDIO routine getkey(), outputs that character
// to the screen, and then outputs the ASCII hex value of that character.
//
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8UB2 STK board to a PC using a mini USB cable..
// 3) Compile and download code to the EFM8UB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) For UART0: On the PC, open HyperTerminal (or any other terminal program)
//    and connect to the JLink CDC UART Port at 115200 baud rate and 8-N-1.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) The device will ask for a character and return its hex value.
//
// Target:         EFM8UB2
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 14 JAN 2015
//
//
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_EFM8UB2_Register_Enums.h>
#include "retargetserial.h"
#include "InitDevice.h"

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
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void) 
{
   uint8_t inputcharacter;       // Used to store character from UART

   enter_DefaultMode_from_RESET();

   SCON0_TI = 1;

   while (1)
   {
      RETARGET_PRINTF ("\nEnter character: ");
      inputcharacter = getchar ();
      RETARGET_PRINTF ("\nCharacter entered : %c",inputcharacter);
      RETARGET_PRINTF ("\n      Value in Hex: %bx",inputcharacter);

   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
