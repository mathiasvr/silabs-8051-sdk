//-----------------------------------------------------------------------------
// F85x_SMBus_Slave.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header for main file:

#ifndef F85X_SMBUS_SLAVE_H_
#define F85X_SMBUS_SLAVE_H_

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define  SYSCLK               24500000 // System clock frequency in Hz

#define  SMB_FREQUENCY           10000 // Target SMBus frequency
                                       // This example supports between 10kHz
                                       // and 100kHz

#define  WRITE                    0x00 // SMBus WRITE command
#define  READ                     0x01 // SMBus READ command

#define  SLAVE_ADDR               0xF0 // Device addresses (7 bits,
                                       // lsb is a don't care)

// Status vector - top 4 bits only
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


//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

extern uint8_t SMB_DATA;                    // Global holder for SMBus data.
                                       // All receive data is written
                                       // here;
                                       // all transmit data is read
                                       // from here

extern bit DATA_READY;                 // Set to '1' by the SMBus ISR
                                       // when a new data byte has been
                                       // received.

#endif // F85X_SMBUS_SLAVE_H_
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
