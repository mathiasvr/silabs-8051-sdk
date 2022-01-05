//-----------------------------------------------------------------------------
// F93x_UART0_Interrupt.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the C8051F930 to write to and read
// from the UART interface. The program reads a word using the UART interrupts
// and outputs that word to the screen, with all characters in uppercase
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J12:  P0.4/TX
//             P0.5/RX
//             VDD/VIO
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect a USB cable from the development board P3 to a PC
// 3) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the USB port (virtual com port) at 230400 and 8-N-1
// 4) Connect the USB Debug Adapter to J9.
// 5) Turn the POWER switch (SW5) to the "ON" position.
// 6) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) Type up to 64 characters into the Terminal and press Enter.  The MCU
//    will then print back the characters that were typed in uppercase.
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 15 JAN 2014
//
// Release 1.2
//    - Updated to be compatible with Raisonance Toolchain (FB)
//    - 14 APR 2010
//
// Release 1.1
//    - Updated to be compatible with SDCC. (FB)
//    - 09 NOV 2007
//
// Release 1.0
//    - Initial Revision (FB)
//    - 14 AUG 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F930_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include "F93x_UART0_Interrupt.h"

//-----------------------------------------------------------------------------
// Generic UART definitions to support both Keil and SDCC
//-----------------------------------------------------------------------------
#ifdef SDCC
#include <sdcc_stdio.h>
#endif

char *GETS (char *, uint16_t);

#ifdef __C51__
#define GETS gets
#endif

#ifdef SDCC
#include <sdcc_stdio.c>
#endif

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint8_t UART_Buffer[UART_BUFFERSIZE];
uint8_t UART_Buffer_Size = 0;
uint8_t UART_Input_First = 0;
uint8_t UART_Output_First = 0;
uint8_t TX_Ready =1;
uint8_t Byte;

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
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
	//Enter default mode
	enter_DefaultMode_from_RESET();
	TX_Ready = 1;                       // Flag showing that UART can transmit

	while (1)
	{
		// If the complete word has been entered via the terminal followed by
		// carriage return or newline

		if(TX_Ready == 1 && UART_Buffer_Size != 0 && (Byte == '\r' || Byte == '\n'))
		{
		 TX_Ready = 0;                 // Set the flag to zero
		 SCON0_TI = 1;                      // Set transmit flag to 1
		}
	}

	// NOTREACHED
	return 0;
}
