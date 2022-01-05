//-----------------------------------------------------------------------------
// EFM8BB1_SMBus_Multimaster.h
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header for main file:

#ifndef EFM8BB1_SMBUS_MULTIMASTER_H_
#define EFM8BB1_SMBUS_MULTIMASTER_H_

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define  SYSCLK               24500000 // System clock frequency in Hz

#define  SMB_FREQUENCY           10000 // Target SCL clock rate
                                       // This example supports between 10kHz
                                       // and 100kHz

#define  WRITE                    0x00 // SMBus WRITE command
#define  READ                     0x01 // SMBus READ command

// Device addresses (7 bits, lsb is a don't care)
// +++++ Change these to be opposite when downloading to the second master +++++
#define  MY_ADDR                  2	   // Device address for this device target
#define  MULTI_ADDR               4	   // Device address for other multimaster
                                       // target

// Status vector - top 4 bits only
#define  SMB_MTSTA                0xE0 // (MT) start transmitted
#define  SMB_MTDB                 0xC0 // (MT) data byte transmitted
#define  SMB_MRDB                 0x80 // (MR) data byte received

#define  SMB_SRADD                0x20 // (SR) slave address received
                                       //    (also could be a lost
                                       //    arbitration)
#define  SMB_SRSTO                0x10 // (SR) STOP detected while SR or ST,
                                       //    or lost arbitration
#define  SMB_SRDB                 0x00 // (SR) data byte received, or
                                       //    lost arbitration
#define  SMB_STDB                 0x40 // (ST) data byte transmitted
#define  SMB_STSTO                0x50 // (ST) STOP detected during a
                                       //    transaction; bus error
// End status vector definition

// Data to indicate the switch was pressed or released
#define  SW_PRESSED               0x0A
#define  SW_RELEASED              0x50

#define LED_ON                       0
#define LED_OFF                      1

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Global holder for SMBus master data. All receive data is written here
// while in master mode
// Slave->Master
extern uint8_t SMB_DATA_IN_MASTER;

// Global holder for SMBus master data. All transmit data is read from here
// while in master mode
// Master->Slave
extern uint8_t SMB_DATA_OUT_MASTER;

// Global holder for SMBus slave data. All receive data is written here
// while slave mode
// Master->Slave
extern uint8_t SMB_DATA_IN_SLAVE;

// Global holder for SMBus slave data. All transmit data is read from here
// while in slave mode
// Slave->Master
extern uint8_t SMB_DATA_OUT_SLAVE;

extern volatile bool DATA_READY;       // Set to '1' by the SMBus ISR
                                       // when a new data byte has been
                                       // received in slave mode.

extern uint8_t TARGET;                 // Target SMBus slave address

extern volatile bool SMB_BUSY;         // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

extern volatile bool SMB_RW;           // Software flag to indicate the
                                       // direction of the current transfer

extern uint8_t NUM_ERRORS;             // Counter for the number of errors.

#endif // F85X_SMBUS_MULTIMASTER_H_
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
