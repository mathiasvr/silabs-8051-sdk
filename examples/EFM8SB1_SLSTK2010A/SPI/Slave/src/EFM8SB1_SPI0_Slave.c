//-----------------------------------------------------------------------------
// SB1_SPI0_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program configures a SPI Slave. The MCU
// is configured in 4-wire Slave Mode.
//
// This example is intended to be used with the SPI0_Master example.
//
// Pinout:
//
// P0.6 - SPI SCK    (digital input, open-drain)
// P0.7 - SPI MISO   (digital output, push-pull)
// P1.0 - SPI MOSI   (digital input, open-drain)
// P1.1 - SPI NSS    (digital input, open-drain)
//
// P0.1 - TEST_OUT    (digital output, push-pull)
//
// all other port pins unused.
//
//
// How To Test:
// 1) Place the switch in "AEM" mode.
// 2) Connect the device to another EFM8 device running SPI0 - Master code.
// 3) Connect the EFM8SB1 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) If the communication passes, TEST_OUT (P0.1) will toggle between
//    HIGH and LOW. If the communication fails, TEST_OUT will remain HIGH.
//
//
// Target:         EFM8SB1
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 9 JAN 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <si_toolchain.h>
#include <SI_EFM8SB1_Register_Enums.h>                  // SI_SFR declarations
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define  LED_OFF            1

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 7);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (TEST_OUT, SFR_P0, 1);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

SI_LOCATED_VARIABLE_NO_INIT (reserved, uint8_t, SI_SEG_XDATA, 0x0000);


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

	DISP_EN = DISP_BC_DRIVEN;

	TEST_OUT = 0;
	while (1) {}                             // Spin forever
}
