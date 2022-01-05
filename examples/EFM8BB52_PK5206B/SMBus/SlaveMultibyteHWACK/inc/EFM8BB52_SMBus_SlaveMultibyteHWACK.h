//-----------------------------------------------------------------------------
// EFM8BB52_SMBus_SlaveMultibyteHWACK.h
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header for main file:

#ifndef SMBUS_SLAVE_MULTIBYTE_H_
#define SMBUS_SLAVE_MULTIBYTE_H_


//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (LED1, SFR_P1, 4);             // LED Green
#define LED_ON   0
#define LED_OFF  1

SI_SBIT (DISP_EN, SFR_P3, 4);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display
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

#define  NUM_BYTES_WR                3 // Number of bytes to write
                                       // Slave <- Master
#define  NUM_BYTES_RD                3 // Number of bytes to read
                                       // Slave -> Master

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
// Global holder for SMBus data.
// All receive data is written here
// NUM_BYTES_WR used because an SMBus write is Master->Slave
extern uint8_t SMB_DATA_IN[NUM_BYTES_WR];

// Global holder for SMBus data.
// All transmit data is read from here
// NUM_BYTES_RD used because an SMBus read is Slave->Master
extern uint8_t SMB_DATA_OUT[NUM_BYTES_RD];

extern bool DATA_READY;                 // Set to '1' by the SMBus ISR
                                       // when a new data byte has been
                                       // received.

#endif // SMBUS_SLAVE_MULTIBYTE_H_

