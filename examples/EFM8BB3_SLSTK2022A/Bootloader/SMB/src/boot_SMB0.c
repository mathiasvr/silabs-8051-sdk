/******************************************************************************
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "efm8_device.h"
#include "boot.h"

// Bit definitions for SMB address byte
#define SMB0DAT_RW__BMASK 0x01  // Slave address R/nW bit mask
#define SMB0DAT_RW__WRITE 0x00  // SMBus WRITE command
#define SMB0DAT_RW__READ  0x01  // SMBus READ command

// SMB0CN0 status vector (MASTER, TXMODE, STA, STO, 0, 0, 0, 0)
#define SMB0CN0_STATUS__BMASK 0xF0

// SMB status vector definitions
// SMB_SRxx => slave receive; SMB_STxx => slave transmit
#define SMB_SRADD 0x20  // (SR) Slave ADDress received
#define SMB_SRDAT 0x00  // (SR) DATa byte received
#define SMB_SRSTO 0x10  // (SR) STOp detected while SR or ST (normal operation)
#define SMB_STDAT 0x40  // (ST) DATa byte transmitted
#define SMB_STSTO 0x50  // (ST) STOp detected during transfer (bus error)

// Local function prototypes
static uint8_t readByte(void);
static uint8_t smb_pollModule(void);

// Counts the number of bytes remaining in the boot frame 
uint8_t boot_rxRemain;

// Local buffer holds byte received from SMB
static uint8_t smb_rxByte;

// Local buffer holds byte to send on SMB
static uint8_t smb_txByte;

// ----------------------------------------------------------------------------
// Wait for the next boot record to arrive.
// ----------------------------------------------------------------------------
void boot_nextRecord(void)
{
  // Return frame error if master reads reply before sending a complete frame
  smb_txByte = BOOT_ERR_FRAME;

  // Wait indefinitely to receive a frame start character
  while (BOOT_FRAME_START != readByte())
    ;
  // Receive and save the frame length
  boot_rxRemain = readByte();
}

// ----------------------------------------------------------------------------
// Get the next byte in the boot record.
// ----------------------------------------------------------------------------
uint8_t boot_getByte(void)
{
  // Read the next byte from the SMB interface
  uint8_t next = readByte();

  // Adjust the number of bytes remaining in the boot record
  boot_rxRemain--;

  // If that was the last byte in the frame
  if (boot_rxRemain == 0)
  {
    // Disable the SMB block so the slave address is NAK'd until we are ready
    SMB0CF &= ~SMB0CF_ENSMB__BMASK;
  }
  return next;
}

// ----------------------------------------------------------------------------
// Get the next word in the boot record.
// ----------------------------------------------------------------------------
uint16_t boot_getWord(void)
{
  SI_UU16_t word;

  // 16-bit words are received in big-endian order
  word.u8[0] = boot_getByte();
  word.u8[1] = boot_getByte();
  return word.u16;
}

// ----------------------------------------------------------------------------
// Send a one byte reply to the host.
//
// Re-enables the SMB peripheral, then waits indefinitely for one byte reply
// to be transmitted on SMBus.
// ----------------------------------------------------------------------------
void boot_sendReply(uint8_t reply)
{
  // Re-enable the SMB block (start ACK'ing matching slave address)
  SMB0CF |= SMB0CF_ENSMB__ENABLED;

  // Store status reply in local buffer
  smb_txByte = reply;

  // Wait indefinitely for the reply to transfer
  while (smb_pollModule() != SMB_STDAT)
    ;
}

// ----------------------------------------------------------------------------
// Receive and return one byte from SMB0.
// ----------------------------------------------------------------------------
static uint8_t readByte(void)
{
  // Wait indefinitely for a byte to arrive
  while (smb_pollModule() != SMB_SRDAT)
    ;
  // Retrieve received byte from local buffer
  return smb_rxByte;
}

// -----------------------------------------------------------------------------
// Polls and handles all SMB bus events.
//
// Waits indefinitely for an SMB slave event and takes appropriate action
// based on the event type. Actions include clearing SMB status bits and
// either reading or writing the SMB data register. Bus errors are treated
// like a stop condition. Data is exchanged through one byte buffers
// smb_rxByte and smb_txByte. Returns the event status vector.
// -----------------------------------------------------------------------------
static uint8_t smb_pollModule(void)
{
  uint8_t status;

  // Wait indefinitely for SMB event to occur
  while (!SMB0CN0_SI)
    ;

  // Read and decode the SMB status vector
  status = SMB0CN0 & SMB0CN0_STATUS__BMASK;
  switch (status)
  {
    // Slave Receiver: Start + Address + RnW received
    case SMB_SRADD:
      // Firmware must clear the start status
      SMB0CN0_STA = 0;

      // Check if this is a read or write transfer
      if ((SMB0DAT & SMB0DAT_RW__BMASK) == SMB0DAT_RW__WRITE)
      {
        // Its a master write, make sure we ACK each byte received
        SMB0CN0_ACK = 1;
      }
      else
      {
        // Its a master read, send the response byte
        SMB0DAT = smb_txByte;
      }
      break;

    // Slave Receiver: Data byte received
    case SMB_SRDAT:
      // Store received data byte in local buffer
      smb_rxByte = SMB0DAT;
      break;

    // Slave Transmitter: Data byte sent
    case SMB_STDAT:
      // Do nothing, master will receive 0xFF if it continues to read more bytes
      break;

    // Default: STOP condition or bus error will land here
    default:
      // Treat bus error as if it were a STOP condition
      status = SMB_SRSTO;

      // Reset status bits for bus error or stop condition
      SMB0CN0_STA = 0;
      SMB0CN0_STO = 0;
      break;
  }

  // The SMB event flag must be cleared last
  SMB0CN0_SI = 0;

  // Return SMB status vector to caller
  return status;
}
