//-----------------------------------------------------------------------------
// F85x_SMBus_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Example software to demonstrate the SMBus interface in Slave mode
// - Interrupt-driven SMBus implementation
// - Only slave states defined
// - 1-byte SMBus data holder used for both transmit and receive
// - Timer1 used as SMBus clock rate (used only for free timeout detection)
// - Timer3 used by SMBus for SCL low timeout detection
// - SMB0CN_ARBLOST support included
// - Pinout:
//    P0.0 -> SDA (SMBus)
//    P0.1 -> SCL (SMBus)
//
//    P1.6 -> LED1
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
// How To Test: UPMU-F850
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - JP2
// 2) Ensure that J32 is not populated.
// 3) Connect the device to an SMBus master running the corresponding
//    SMBus_Master code.
// 4) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 5) Compile and download code to the 'F85x UDP MCU board by selecting 
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the 
//		Resume button in the quick menu, or pressing F8.
//    The slave code will write data and read data from the
//    same data byte, so a successive write and read will effectively echo the
//    data written. To verify that the code is working properly, verify on the
//    master that the data written is the same as the data received.
//
//
//-----------------------------------------------------------------------------
// How To Test: ToolStick F85x
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Connect the device to an SMBus master running the corresponding
//    SMBus_Master code.
// 3) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 4) Compile and download code to the ToolStick F85x board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//    The slave code will write data and read data from the
//    same data byte, so a successive write and read will effectively echo the
//    data written. To verify that the code is working properly, verify on the
//    master that the data written is the same as the data received.
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.2 (BL)
//		- Updated How To Test with Studio specific instructions
//		- 3 JAN 2014
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 27 FEB 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>
#include "InitDevice.h"
#include "F85x_SMBus_Slave.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(LED1, SFR_P1, 0);                 // DS5 P1.0 LED

#define LED_ON                       0
#define LED_OFF                      1

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

uint8_t SMB_DATA;                           // Global holder for SMBus data.
                                       // All receive data is written
                                       // here;
                                       // all transmit data is read
                                       // from here

bit DATA_READY = 0;                    // Set to '1' by the SMBus ISR
                                       // when a new data byte has been
                                       // received.

//-----------------------------------------------------------------------------
// Function PROTOTYPES
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
// MAIN Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, then waits for SMBus
// communication.
//
//-----------------------------------------------------------------------------
void main (void)
{
   enter_DefaultMode_from_RESET();

   SMB_DATA = 0xFD;                    // Initialize SMBus data holder

   while(1)
   {
      while(!DATA_READY);              // New SMBus data received?
      DATA_READY = 0;
      LED1 = !LED1;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
