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
//   P0.6   - SPI SCK
//   P0.7   - SPI MISO
//   P1.0   - SPI MOSI
//   P1.1   - SPI NSS
//   P1.4   - LED
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 LCK (SPI Slave) + EFM8BB1 LCK (SPI Master)
//-----------------------------------------------------------------------------
// 1) Connect the device first EFM8BB1 LCK to second EFM8BB1 LCK running the
//    corresponding SPI_Master code with SPI_CLOCK set to 1 MHz or lower.
// 2) Connect the EFM8BB1 LCK board to a PC using a micro USB cable.
// 3) Compile and download code to the first EFM8BB1 LCK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) If the communication passes, the LEDs on both the Master
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


   IE_EA = 1;                          // Enable global interrupts

   while (1);                          // Loop forever
}
