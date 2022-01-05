//-----------------------------------------------------------------------------
// EFM8LB1_SMBus_SlaveMultibyte.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Example software to demonstrate the SMBus interface in Slave mode:
// - Interrupt-driven SMBus implementation
// - Only slave states defined
// - Multi-byte SMBus data holders used for both transmit and receive
// - SMB0CN_ARBLOST support included
// - supports multiple-byte writes and multiple-byte reads
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   SMBus  - SMBus master, SCL = 10 kHz
//   Timer1 - SMBus clock rate (used only for free timeout detection)
//   Timer3 - SMBus SCL low timeout detection
//   P1.2   - SMBus SDA
//   P1.3   - SMBus SCL
//   P1.4   - LED green
//   P3.4   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8LB1 STK (SMBus Slave) + EFM8LB1 STK (SMBus Master)
//-----------------------------------------------------------------------------
// 1) Connect the device first EFM8LB1 STK to second EFM8LB1 STK running the
//    corresponding SMBus_Master_Multibyte code.
// 2) Place the switch in "AEM" mode.
// 3) Connect the EFM8LB1 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the first EFM8LB1 STK board.
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
// Target:         EFM8LB1
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 06 MAR 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8LB1_Register_Enums.h>
#include "InitDevice.h"
#include "EFM8LB1_SMBus_SlaveMultibyte.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
// Global holder for SMBus data.
// All receive data is written here
// NUM_BYTES_WR used because an SMBus write is Master->Slave
uint8_t SMB_DATA_IN[NUM_BYTES_WR];

// Global holder for SMBus data.
// All transmit data is read from here
// NUM_BYTES_RD used because an SMBus read is Slave->Master
uint8_t SMB_DATA_OUT[NUM_BYTES_RD];

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
   uint8_t i;

   enter_DefaultMode_from_RESET();

   DISP_EN = DISP_BC_DRIVEN;           // EFM8 does not drive display

   // Initialize the outgoing data array in case a read is done before a
   // write
   for (i = 0; i < NUM_BYTES_RD; i++)
   {
      SMB_DATA_OUT[i] = 0xFD;
   }

   while(1)
   {
      while(!DATA_READY);              // New SMBus data received?
      DATA_READY = 0;

      // Copy the data from the input array to the output array
      for (i = 0; i < NUM_BYTES_RD; i++)
      {
         SMB_DATA_OUT[i] = SMB_DATA_IN[i];
      }

      LED1 = !LED1;
   }
}
