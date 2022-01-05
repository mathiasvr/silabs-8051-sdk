//-----------------------------------------------------------------------------
// EFM8BB1_SPI0_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates the SPI interface in slave mode. This example is
// intended to be used with the SPI_Master example with a maximum SPI_CLOCK
// frequency of 1 MHz.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC / 1
//   SPI    - Slave
//   P0.0   - Display enable
//   P0.6   - SPI SCK
//   P0.7   - SPI MISO
//   P1.0   - SPI MOSI
//   P1.1   - SPI NSS
//   P1.4   - LED
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 STK (SPI Slave) + EFM8BB1 STK (SPI Master)
//-----------------------------------------------------------------------------
// 1) Connect the device first EFM8BB1 STK to second EFM8BB1 STK running the
//    corresponding SPI_Master code with SPI_CLOCK set to 1 MHz or lower.
// 2) Place the switch in "AEM" mode.
// 3) Connect the EFM8BB1 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the first EFM8BB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) If the communication passes, the LEDs on both the Master
//    and Slave boards will blink slowly. If it fails, the LEDs will be off.
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
#include <SI_EFM8BB1_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P0, 0);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

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

   DISP_EN = DISP_BC_DRIVEN;           // Display not driven by EFM8

   IE_EA = 1;                          // Enable global interrupts

   while (1);                          // Loop forever
}
