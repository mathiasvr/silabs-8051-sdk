//-----------------------------------------------------------------------------
// F85x_SMBus_Master_Multibyte.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header for main file:

#ifndef F85X_SMBUS_MASTER_MULTIBYTE_H_
#define F85X_SMBUS_MASTER_MULTIBYTE_H_

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define  SYSCLK               24500000 // System clock frequency in Hz

#define  SMB_FREQUENCY           10000 // Target SCL clock rate
                                       // This example supports between 10kHz
                                       // and 100kHz

#define  WRITE                    0x00 // SMBus WRITE command
#define  READ                     0x01 // SMBus READ command

// Device addresses (7 bits, lsb is a don't care)
#define  SLAVE_ADDR               0xF0 // Device address for slave target

// Status vector - top 4 bits only
#define  SMB_MTSTA                0xE0 // (MT) start transmitted
#define  SMB_MTDB                 0xC0 // (MT) data byte transmitted
#define  SMB_MRDB                 0x80 // (MR) data byte received
// End status vector definition

#define  NUM_BYTES_WR                3 // Number of bytes to write
                                       // Master -> Slave
#define  NUM_BYTES_RD                3 // Number of bytes to read
                                       // Master <- Slave

#define LED_ON                       0
#define LED_OFF                      1

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

// Global holder for SMBus data
// All receive data is written here
extern uint8_t SMB_DATA_IN[NUM_BYTES_RD];

// Global holder for SMBus data.
// All transmit data is read from here
extern uint8_t SMB_DATA_OUT[NUM_BYTES_WR];

extern uint8_t TARGET;                      // Target SMBus slave address

extern volatile bit SMB_BUSY;          // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

extern volatile bit SMB_RW;            // Software flag to indicate the
                                       // direction of the current transfer

extern uint16_t NUM_ERRORS;                 // Counter for the number of errors.

#endif // F85X_SMBUS_MASTER_MULTIBYTE_H_
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------