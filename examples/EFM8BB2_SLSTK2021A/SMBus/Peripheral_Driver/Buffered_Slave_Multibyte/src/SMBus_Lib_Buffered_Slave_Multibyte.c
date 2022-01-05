//-----------------------------------------------------------------------------
// SMBus_Lib_Buffered_Slave_Multibyte.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Example software to demonstrate the SMBus interface in Slave mode using
// the EFM8 SMBus peripheral driver in buffered mode.
// - Interrupt-driven SMBus implementation
// - Only slave states defined
// - Multi-byte SMBus data holders used for both transmit and receive
// - Timer1 used as SMBus clock rate (used only for free timeout detection)
// - Timer3 used by SMBus for SCL low timeout detection
// - SMB0CN0_ARBLOST support included
// - supports multiple-byte writes and multiple-byte reads
// - Pinout:
//    P1.2 -> SDA (SMBus)
//    P1.3 -> SCL (SMBus)
//
//    P1.4 -> LED
//
//    all other port pins unused
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect the device to another EFM8 device running SMBus - Master Multibyte code.
// 3) Connect the EFM8BB2 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8BB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
// 6) The slave code will copy the write data to the read
//    data, so a successive write and read will effectively echo the data
//    written.  To verify that the code is working properly, verify on the
//    master that the data received is the same as the data written.
//
// Target:         EFM8BB2
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 9 JAN 2015
//
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "bsp.h"
#include "smb_0.h"
#include "InitDevice.h"
#include "SMBus_Lib_Buffered_Slave_Multibyte.h"

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

// Global holder for SMBus data
// All receive data is written here
SI_SEGMENT_VARIABLE(SMB_DATA_IN[NUM_BYTES_RD+1],
                    uint8_t,
                    EFM8PDL_SMB0_RX_BUFTYPE);

// Global holder for SMBus data.
// All transmit data is read from here
SI_SEGMENT_VARIABLE(SMB_DATA_OUT[NUM_BYTES_WR],
                    uint8_t,
                    EFM8PDL_SMB0_TX_BUFTYPE);

// Counter for the number of errors.
uint16_t numErrors;

SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);

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
// main() Routine
// ----------------------------------------------------------------------------
int main(void)
{
  uint8_t i;

  //Enter default mode
  enter_DefaultMode_from_RESET();

  BSP_DISP_EN = 0;
  BSP_LED_G = BSP_LED_OFF;

  // Initialize the outgoing data array in case a read is done before a
  // write
  for (i = 0; i < NUM_BYTES_RD; i++)
  {
    SMB_DATA_OUT[i] = 0xFD;
  }

  SMB0_reset();

  // Use Timer 1 for SCL clock rate
  // Enable SCL low timeout using Timer 3
  SMB0_init(SMB0_TIMER1, true);

  // Set slave address for hardware ack
  // Assign RX buffer to store received data
  // (allocate an additional byte so we don't get an extra
  // SMB0_commandReceivedCb() call due to the buffer
  // being full)
  SMB0_initSlave(SLAVE_ADDR, SMB_DATA_IN, NUM_BYTES_RD+1);

  while (1)
  {
  }
}

//-----------------------------------------------------------------------------
// Callbacks
//-----------------------------------------------------------------------------

// Callback is called when the slave receives data from the master
void SMB0_commandReceivedCb()
{
  uint8_t i;
  uint8_t len = SMB0_getCommandLength();

  // Don't send more data than will fit in the TX buffer
  if (len > NUM_BYTES_WR)
  {
    len = NUM_BYTES_WR;
  }

  // Copy the data from the input array to the output array
  for (i = 0; i < len; i++)
  {
    SMB_DATA_OUT[i] = SMB_DATA_IN[i];
  }

  // Send the response to the master
  SMB0_sendResponse(SMB_DATA_OUT, len);

  BSP_LED_G = !BSP_LED_G;
}

// Callback is called when an SMBus error occurs
void SMB0_errorCb(SMB0_TransferError_t error)
{
  switch (error)
  {
    case SMB0_ARBLOST_ERROR:
      case SMB0_NACK_ERROR:
      case SMB0_UNKNOWN_ERROR:
      case SMB0_TXUNDER_ERROR:
      numErrors++;
      break;
  }
}

void SMB0_transferCompleteCb()
{
  // Not used
}
