//-----------------------------------------------------------------------------
// SMBus_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Example software to demonstrate the EFM8UB2 SMBus interface in Slave mode
// - Interrupt-driven SMBus implementation
// - Only slave states defined
// - 1-byte SMBus data holder used for both transmit and receive
// - Timer1 used as SMBus clock rate (used only for free timeout detection)
// - Timer3 used by SMBus for SCL low timeout detection
// - SMB0CN0_ARBLOST support included
// - Pinout:
//    P1.2 -> SDA (SMBus)
//    P1.3 -> SCL (SMBus)
//
//    P1.6 -> LED
//
//    all other port pins unused
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect the device to another EFM8 device running SMBus - Master code.
// 3) Connect the EFM8UB2 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8UB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) The slave code will write data and read data from the
//    same data byte, so a successive write and read will effectively echo the
//    data written.  To verify that the code is working properly, verify on the
//    master that the data written is the same as the data received.
//
//
// Target:         EFM8UB2
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 13 JAN 2015
//

//-----------------------------------------------------------------------------
// Includes and Device-Specific Parameters
//-----------------------------------------------------------------------------

#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define  SYSCLK         12000000       // System clock frequency in Hz

#define  SMB_FREQUENCY  10000          // Target SMBus frequency
                                       // This example supports between 10kHz
                                       // and 100kHz

#define  WRITE          0x00           // SMBus WRITE command
#define  READ           0x01           // SMBus READ command
#define LED_ON           0             // Turns the LED on
#define LED_OFF          1             // Turns the LED off

#define  SLAVE_ADDR     0xF0           // Device addresses (7 bits,
                                       // lsb is a don't care)

// Status vector - top 4 bits only
#define  SMB_SRADD      0x20           // (SR) slave address received
                                       //    (also could be a lost
                                       //    arbitration)
#define  SMB_SRSTO      0x10           // (SR) STOP detected while SR or ST,
                                       //    or lost arbitration
#define  SMB_SRDB       0x00           // (SR) data byte received, or
                                       //    lost arbitration
#define  SMB_STDB       0x40           // (ST) data byte transmitted
#define  SMB_STSTO      0x50           // (ST) STOP detected during a
                                       //    transaction; bus error
// End status vector definition

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

uint8_t SMB_DATA;                           // Global holder for SMBus data.
                                       // All receive data is written
                                       // here;
                                       // all transmit data is read
                                       // from here

bool DATA_READY = 0;                    // Set to '1' by the SMBus ISR
                                       // when a new data byte has been
                                       // received.

// 16-bit SI_SFR declarations

SI_SBIT (LED, SFR_P1, 6);                 // LED on port P1.6

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
// MAIN Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, then waits for SMBus
// communication.
//
void main(void)
{

  enter_DefaultMode_from_RESET();

  SMB_DATA = 0xFD;                    // Initialize SMBus data holder

  while (1)
  {
    while (!DATA_READY);              // New SMBus data received?
    DATA_READY = 0;
    LED = ~LED;
  }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
