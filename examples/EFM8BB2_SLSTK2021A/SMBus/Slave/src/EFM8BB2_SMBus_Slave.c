//-----------------------------------------------------------------------------
// EFM8BB2_SMBus_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Example software to demonstrate the SMBus interface in Slave mode.
// - Interrupt-driven SMBus implementation
// - Only slave states defined
// - 1-byte SMBus data holder used for both transmit and receive
// - SMB0CN_ARBLOST support included
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   SMBus  - SMBus master, SCL = 10 kHz
//   Timer1 - SMBus clock rate (used only for free timeout detection)
//   Timer3 - SMBus SCL low timeout detection
//   P1.2   - SMBus SDA
//   P1.3   - SMBus SCL
//   P1.4   - LED green
//   P2.3   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB2 STK (SMBus Slave) + EFM8BB2 STK (SMBus Master)
//-----------------------------------------------------------------------------
// 1) Connect the device first EFM8BB2 STK to second EFM8BB2 STK running the
//    corresponding SMBus_Master code.
// 2) Place the switch in "AEM" mode.
// 3) Connect the EFM8BB2 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the first EFM8BB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) The slave code will write data and read data from the
//    same data byte, so a successive write and read will effectively echo the
//    data written. To verify that the code is working properly, verify on the
//    master that the data written is the same as the data received.
//
// Target:         EFM8BB2
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 10 OCT 2014
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB2_Register_Enums.h>
#include "InitDevice.h"
#include "EFM8BB2_SMBus_Slave.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
uint8_t SMB_DATA;                      // Global holder for SMBus data.
                                       // All receive data is written
                                       // here;
                                       // all transmit data is read
                                       // from here

bool DATA_READY = 0;                    // Set to '1' by the SMBus ISR
                                       // when a new data byte has been
                                       // received.

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
//
// Main routine performs all configuration tasks, then waits for SMBus
// communication.
//
//-----------------------------------------------------------------------------
void main (void)
{
   enter_DefaultMode_from_RESET();

   DISP_EN = DISP_BC_DRIVEN;           // EFM8 does not drive display

   SMB_DATA = 0xFD;                    // Initialize SMBus data holder

   while (1)
   {
      while (!DATA_READY);             // New SMBus data received?
      DATA_READY = 0;
      LED1 = !LED1;
   }
}
