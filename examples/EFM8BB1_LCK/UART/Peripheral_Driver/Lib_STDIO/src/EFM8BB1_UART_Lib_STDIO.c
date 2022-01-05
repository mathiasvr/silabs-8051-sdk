//-----------------------------------------------------------------------------
// EFM8BB1_UART_Lib_STDIO.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the EFM8BB1 to use routines
// in UART peripheral driver (stdio configuration) to write to and read from
// the UART0 interface. The program reads a character using the UART peripheral
// driver routine _getkey() and outputs that character to the screen.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC / 1
//   UART0  - 115200 baud, 8-N-1
//   Timer1 - UART0 clock source
//   P0.4   - UART0 TX
//   P0.5   - UART0 RX
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 LCK
//-----------------------------------------------------------------------------
// 1) Connect the EFM8BB1 LCK board to a PC using a micro USB cable.
// 2) Compile and download code to the EFM8BB1 LCK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 3) Disconnect from the device to begin running the example.
// 4) Connect to the device's UART pins on P0.4 (TXD) and P0.5 (RXD):
//		a) Option 1:
//			1. Bridge solder jumpers J7 and J9 on the LCK.
//			2. Download and install Toolstick Terminal: www.silabs.com/products/development-tools/software/8-bit-8051-microcontroller-software#toolstick
//			3. Open Toolstick Terminal and connect to the device.
//		b) Option 2:
//			1. Attach a USB-to-UART bridge to pins P0.4 and P0.5.
//			2. Open HyperTerminal (or any other terminal program) and connect
//				to the USB port (virtual COM port) at <BAUDRATE> (115200) and 8-N-1.
// 5) Follow the prompts in the terminal program.
//
// Target:         EFM8BB1
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 10 OCT 2014
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "bsp.h"
#include "uart_0.h"
#include "InitDevice.h"
#include <stdio.h>

#if defined __ICC8051__
#include <pgmspace.h>
#endif

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
   UART0_initStdio();

   while (1)
   {
#if defined __C51__
      printf ("\r\nEnter character: ");
      input_char = _getkey();
      printf ("\r\nCharacter entered: %c", (char)input_char);
      printf ("\r\n     Value in Hex: %x", (int16_t)input_char);
#elif defined __ICC8051__
      printf_P ("\r\nEnter character: ");
      input_char = getchar();
      printf_P ("\r\nCharacter entered: %c", (char)input_char);
      printf_P ("\r\n     Value in Hex: %x", (int16_t)input_char);
#endif
   }
}
