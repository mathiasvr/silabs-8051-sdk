//-----------------------------------------------------------------------------
// EFM8BB1_UART_Lib_Buffer.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the EFM8BB1 to use routines
// in UART peripheral driver (buffer configuration) to write to and read from
// the UART0 interface. The program reads five lower case characters,
// capitalizes the letters, then outputs the capitalized letters.
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
// 5) Type five lower case letters. Note: terminal will not display anything
//    until five letters are entered.
// 6) Observe the capitalized input in the terminal.
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
   UART0_init(UART0_RX_ENABLE, UART0_WIDTH_8, UART0_MULTIPROC_DISABLE);

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
// UART ISR Callbacks
//-----------------------------------------------------------------------------
void UART0_receiveCompleteCb ()
{
   uint8_t i;
   unsigned char byte;

   for (i = 0; i<BUFFER_LENGTH; i++)
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

void UART0_transmitCompleteCb ()
{
}
