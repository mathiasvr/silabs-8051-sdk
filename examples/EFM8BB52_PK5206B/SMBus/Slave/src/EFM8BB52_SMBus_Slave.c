//-----------------------------------------------------------------------------
// EFM8BB52_SMBus_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
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
//   Timer1 - SMBus clock source
//   Timer3 - SMBus SCL low timeout detection
//   P1.2   - SMBus SDA
//   P1.3   - SMBus SCL
//   P1.4   - LED Green
//   P3.4   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB52 PK (SMBus Slave) + EFM8BB52 PK (SMBus Master)
//-----------------------------------------------------------------------------
// 1) Connect the device first EFM8BB52 PK to second EFM8BB52 PK running the
//    corresponding SMBus_Master code.
// 2) Place the switch in "AEM" mode.
// 3) Connect the EFM8BB52 PK board to a PC using a USB cable.
// 4) Compile and download code to the first EFM8BB52 PK board.
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
// Target:         EFM8BB52
// Tool chain:     Generic
//
// Release 0.1 (DS)
//    - Initial Revision
//    - 07 JAN 2021
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB52_Register_Enums.h>
#include "InitDevice.h"
#include "EFM8BB52_SMBus_Slave.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
uint8_t SMB_DATA;         // Global holder for SMBus data.
                          // All receive data is written
                          // here;
                          // all transmit data is read
                          // from here

bool DATA_READY = 0;      // Set to '1' by the SMBus ISR
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
