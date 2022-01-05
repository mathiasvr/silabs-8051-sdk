/******************************************************************************
 * Copyright (c) 2021 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef __SMBUS_LIB_BUFFERED_SLAVE_MULTIBYTE_H__
#define __SMBUS_LIB_BUFFERED_SLAVE_MULTIBYTE_H__

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define  SYSCLK         24500000       // System clock frequency in Hz

#define  WRITE          0x00           // SMBus WRITE command
#define  READ           0x01           // SMBus READ command

// Device addresses (7 bits, lsb is a don't care)
#define  SLAVE_ADDR     0xF0           // Device address for slave target

#define  NUM_BYTES_WR   3              // Number of bytes to write
                                       // Master -> Slave
#define  NUM_BYTES_RD   3              // Number of bytes to read
                                       // Master <- Slave

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

// Global holder for SMBus data
// All receive data is written here
extern SI_SEGMENT_VARIABLE(SMB_DATA_IN[NUM_BYTES_RD+1], uint8_t, EFM8PDL_SMB0_RX_BUFTYPE);

// Global holder for SMBus data.
// All transmit data is read from here
extern SI_SEGMENT_VARIABLE(SMB_DATA_OUT[NUM_BYTES_WR], uint8_t, EFM8PDL_SMB0_TX_BUFTYPE);

// Counter for the number of errors.
extern uint16_t numErrors;

extern volatile bool transferInProgress;

#endif // __SMBUS_LIB_BUFFERED_SLAVE_MULTIBYTE_H__
