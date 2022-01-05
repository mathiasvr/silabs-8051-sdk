//-----------------------------------------------------------------------------
// SB1_WakeOnUART.c
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
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8SB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8SB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the JLink CDC UART Port at 115200 baud rate and 8-N-1.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) Type characters into the terminal and click the "Send Data" button. The
//    MCU will echo them back. When the MCU is awake, LED0 (P2.0) will
//    be lit.
//
//
// Target:         EFM8SB2
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 16 JAN 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <si_toolchain.h>
#include <SI_EFM8SB2_Register_Enums.h>                  // SI_SFR declarations
#include "InitDevice.h"
#include "retargetserial.h"
#include "EFM8SB2_WakeOnUART.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 6);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (LED0,   SFR_P2, 0);          // '0' means ON, '1' means OFF
SI_SBIT (SW2,    SFR_P0, 2);    // SW2 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

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

	DISP_EN = DISP_BC_DRIVEN;

	SCON0_TI = 1;                            // Indicate TX0 ready

	for (temp = 0; temp < 0xffff; temp++);    // Delay before sending to ToolStick

	RETARGET_PRINTF ("\f\f");
	RETARGET_PRINTF ("\nHello! This program demonstrates low-power Wake-on-UART\n");
	RETARGET_PRINTF ("MCU will enter sleep mode now, and will wake up on receiving a UART RX transition\n");

	while (1)
	{
	  if (TMR2CN0_TF2H == 1 && SCON0_TI == 1)       // If timeout has occured AND last character was transmitted:
	  {
	   LED0 = 1;                  // Turn off LED

		 PMU0CF |= 0x20;               // Clear flags
		 PMU0CF = 0x82;                // Enter Sleep mode with Port Match Wake Up

		 #if WAKE_LED_ACTIVE == 1
		 LED0 = 0;               // Turn On LED
		 #endif

		 TMR2CN0_TR2 = 0;                      // Set timeout
		 TMR2CN0_TF2H = 0;
		 TMR2 = -(TIMEOUT_MS*SYSCLK/12/1000);
		 TMR2CN0_TR2 = 1;
	  }

	  while (SCON0_RI == 0 && TMR2CN0_TF2H == 0); // Wait till byte fully received or timeout

	  if (SCON0_RI == 1)
	  {
		 SCON0_RI = 0;
		 rx_char = SBUF0;              // Read received character
		 RETARGET_PRINTF ("%c", SBUF0);         // Echo received character
	  }
	}
}
