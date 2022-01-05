//-----------------------------------------------------------------------------
// F85x_UART0_Interrupt.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the C8051F850 to write to and
// read from the UART interface. The program reads a word using the UART
// interrupts and outputs that word to the screen, with all characters in
// uppercase.
//
//-----------------------------------------------------------------------------
// How To Test: UPMU-F850
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - J7: all
//    - JP2
// 2) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to the 'F85x UDP MCU board by selecting 
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 4) Connect a USB cable from the J5 of the MCU card to a PC.
// 5) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the USB port (virtual COM port) at <BAUDRATE> (115200) and 8-N-1.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the 
//		Resume button in the quick menu, or pressing F8.
// 7) Using a terminal program on the PC, input any number of lower-case
//    characters, up to UART_BUFFERSIZE (default 64), followed by either
//    a carriage return ('\r'), a newline character ('\n'), or a tilda ('~').
//    The program will change the input characters to upper-case and output
//    them over UART.
//
//-----------------------------------------------------------------------------
// How To Test: ToolStick F85x
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 3) Compile and download code to the ToolStick F85x board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Disconnect from the device by selecting Run -> Disconnect from the menus.
// 5) On the PC, open Silicon Laboratories ToolStick Terminal, and navigate to
//    ToolStick -> Settings in the menus. Under UART Settings, select the
//    following options:
//       -Baud Rate: as defined by <BAUDRATE> (115200 by default)
//       -Parity: None
//       -Data Bits: 8
//       -Stop Bits: 1
// 6) In ToolStick Terminal, press "Connect" to connect to the device.
// 7) Using a terminal program on the PC, input any number of lower-case
//    characters, up to UART_BUFFERSIZE (default 64), followed by either
//    a carriage return ('\r'), a newline character ('\n'), or a tilda ('~').
//    The program will change the input characters to upper-case and output
//    them over UART.
//
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.2 (BL)
//		- Updated How To Test with Studio specific instructions
//    - Updated the program to accept newline characters in addition to
//      carriage returns as a signal to display the modified input.
//		- 3 JAN 2014
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 16 JAN 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>
#include <stdio.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

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
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{

   enter_DefaultMode_from_RESET();
   IE_EA = 1;

   while(1)
   {
      // If the complete word has been entered via the terminal followed by
      // carriage return or newline

      if((TX_Ready == 1) && (UART_Buffer_Size != 0) && ((Byte == '\r') || (Byte == '\n') || (Byte == '~')))
      {
         TX_Ready = 0;                 // Set the flag to zero
         SCON0_TI = 1;                       // Set transmit flag to 1
      }
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
