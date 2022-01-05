//-----------------------------------------------------------------------------
// EFM8BB51_SPI_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates the SPI interface in slave mode. This example is
// intended to be used with the SPI_Master example with a maximum SPI_CLOCK
// frequency of 1 MHz.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   SPI    - Slave
//   P0.6   - SPI SCK
//   P0.7   - SPI MISO
//   P1.0   - SPI MOSI
//   P1.1   - SPI NSS
//   P1.5   - LED0
//   P2.0   - Display enable
//   GND    - EXP Header 1
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB51 PK (SPI Slave) + EFM8BB51 PK (SPI Master)
//-----------------------------------------------------------------------------
// 1) Connect the device first EFM8BB51 PK to second EFM8BB51 PK running the
//    corresponding SPI_Master code with SPI_CLOCK set to 1 MHz or lower.
// 2) Place the switch in "AEM" mode.
// 3) Connect the EFM8BB51 PK board to a PC using a USB cable.
// 4) Compile and download code to the first EFM8BB51 PK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) If the communication passes, the LEDs on both the Master
//    and Slave boards will blink slowly. If it fails, the LEDs will be off.
//
// Target:         EFM8BB51
// Tool chain:     Generic
//
// Release 0.1 (JS)
//    - Initial Revision
//    - 2021
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB51_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 0);          // Display Enable
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

   DISP_EN = DISP_BC_DRIVEN;           // EFM8 does not drive display

   IE_EA = 1;                          // Enable global interrupts

   while (1);                          // Loop forever
}
