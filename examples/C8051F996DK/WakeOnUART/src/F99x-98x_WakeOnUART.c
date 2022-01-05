//-----------------------------------------------------------------------------
// F99x_WakeOnUART.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program implements Low-Power Wake-on-UART. The MCU will be in sleep mode
// until a high-to-low transition is detected on the UART RX pin. It will then
// wake up and receive the character. The received character is echoed back.
//
// How To Test:
//
// Setup:
// 1) Ensure that jumpers are placed on the following:
//       J12:  P0.4/TX
//             P0.5/RX
//             P1.5/LED1.5
//             VDD/VIO
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect a USB cable from the development board P3 to a PC
// 3) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the USB port (virtual com port) at 57600 and 8-N-1
// 4) Connect the USB Debug Adapter to J4.
// 5) Turn the POWER switch (SW5) to the "ON" position.
// 6) Compile and download code to a 'F99x device on a C8051F99x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) Type characters into the terminal and click the "Send Data" button. The
//    MCU will echo them back. When the MCU is awake, the Red LED (P1.5) will
//    be lit.
//
//
// Target:         C8051F99x-C8051F98x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 13 JAN 2014
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F990_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include <stdio.h>
#include "F99x-98x_WakeOnUART.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(RED_LED,      SFR_P1, 5);        // '0' means ON, '1' means OFF
SI_SBIT(YELLOW_LED,   SFR_P1, 3);        // '0' means ON, '1' means OFF
SI_SBIT(SW2,          SFR_P0, 2);        // SW2 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);

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
	uint8_t rx_char;
	uint16_t temp;

	//Enter default mode
	enter_DefaultMode_from_RESET();

	SCON0_TI = 1;                            // Indicate TX0 ready

	for (temp = 0; temp < 0xffff; temp++);    // Delay before sending to ToolStick

	printf ("\f\f");
	printf ("\nHello! This program demonstrates low-power Wake-on-UART\n");
	printf ("MCU will enter sleep mode now, and will wake up on receiving a UART RX transition\n");

	while (1)
	{
	  if (TMR2CN_TF2H == 1 && SCON0_TI == 1)       // If timeout has occured AND last character was transmitted:
	  {
		 RED_LED = 1;                  // Turn off LED

		 PMU0CF |= 0x20;               // Clear flags
		 PMU0CF = 0x82;                // Enter Sleep mode with Port Match Wake Up

		 #if WAKE_LED_ACTIVE == 1
			RED_LED = 0;               // Turn On LED
		 #endif

		 TMR2CN_TR2 = 0;                      // Set timeout
		 TMR2CN_TF2H = 0;
		 TMR2 = -(TIMEOUT_MS*SYSCLK/12/1000);
		 TMR2CN_TR2 = 1;
	  }

	  while (SCON0_RI == 0 && TMR2CN_TF2H == 0); // Wait till byte fully received or timeout

	  if (SCON0_RI == 1)
	  {
		 SCON0_RI = 0;
		 rx_char = SBUF0;              // Read received character
		 printf ("%c", SBUF0);         // Echo received character
	  }
	}

	// NOTREACHED
	return 0;
}
