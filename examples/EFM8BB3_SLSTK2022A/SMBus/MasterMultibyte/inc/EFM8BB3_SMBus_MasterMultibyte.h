//-----------------------------------------------------------------------------
// EFM8BB3_SMBus_Master_Multibyte.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header for main file:

#ifndef SMBUS_MASTER_MULTIBYTE_H_
#define SMBUS_MASTER_MULTIBYTE_H_

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (SDA, SFR_P1, 2);              // SMBus on P1.2
SI_SBIT (SCL, SFR_P1, 3);              // and P1.3

SI_SBIT (LED1, SFR_P1, 4);             // LED green
#define LED_ON   0
#define LED_OFF  1

SI_SBIT (DISP_EN, SFR_P3, 4);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

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

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

// Global holder for SMBus data
// All receive data is written here
extern uint8_t SMB_DATA_IN[NUM_BYTES_RD];

// Global holder for SMBus data.
// All transmit data is read from here
extern uint8_t SMB_DATA_OUT[NUM_BYTES_WR];

extern uint8_t TARGET;                 // Target SMBus slave address

extern volatile bool SMB_BUSY;          // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

extern volatile bool SMB_RW;            // Software flag to indicate the
                                       // direction of the current transfer

extern uint16_t NUM_ERRORS;            // Counter for the number of errors.

#endif // SMBUS_MASTER_MULTIBYTE_H_
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
