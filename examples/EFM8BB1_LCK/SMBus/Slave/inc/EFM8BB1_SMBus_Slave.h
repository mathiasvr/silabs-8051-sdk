//-----------------------------------------------------------------------------
// EFM8BB1_SMBus_Slave.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header for main file:

#ifndef SMBUS_SLAVE_H_
#define SMBUS_SLAVE_H_

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P0, 0);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (LED1, SFR_P1, 4);             // LED
#define LED_ON   0
#define LED_OFF  1

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
// Global Variables
//-----------------------------------------------------------------------------
extern uint8_t SMB_DATA;                    // Global holder for SMBus data.
                                       // All receive data is written
                                       // here;
                                       // all transmit data is read
                                       // from here

extern bool DATA_READY;                 // Set to '1' by the SMBus ISR
                                       // when a new data byte has been
                                       // received.

#endif // SMBUS_SLAVE_H_
