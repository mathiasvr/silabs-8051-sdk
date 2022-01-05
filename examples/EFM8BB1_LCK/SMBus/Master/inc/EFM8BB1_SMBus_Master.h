//-----------------------------------------------------------------------------
// EFM8BB1_SMBus_Master.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header for main file:

#ifndef SMBUS_MASTER_H_
#define SMBUS_MASTER_H_

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P0, 0);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (SDA, SFR_P1, 2);              // SMBus on P1.2
SI_SBIT(SCL, SFR_P1, 3);               // and P1.3

SI_SBIT (LED1, SFR_P1, 4);             // LED
#define LED_ON   0
#define LED_OFF  1

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define  SYSCLK               24500000 // System clock frequency in Hz

#define  SMB_FREQUENCY           10000 // Target SCL clock rate
                                       // This example supports between 10kHz
                                       // and 100kHz

#define  WRITE                    0x00 // SMBus WRITE command
#define  READ                     0x01 // SMBus READ command

// Device addresses (7 bits, LSB is a don't care)
#define  SLAVE_ADDR               0xF0 // Device address for slave target

// Status vector - top 4 bits only
#define  SMB_MTSTA                0xE0 // (MT) start transmitted
#define  SMB_MTDB                 0xC0 // (MT) data byte transmitted
#define  SMB_MRDB                 0x80 // (MR) data byte received
// End status vector definition

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
extern uint8_t SMB_DATA_IN;            // Global holder for SMBus data
                                       // All receive data is written here

extern uint8_t SMB_DATA_OUT;           // Global holder for SMBus data.
                                       // All transmit data is read from here

extern uint8_t TARGET;                 // Target SMBus slave address

extern volatile bool SMB_BUSY;          // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

extern volatile bool SMB_RW;            // Software flag to indicate the
                                       // direction of the current transfer

extern uint16_t NUM_ERRORS;            // Counter for the number of errors.


#endif // SMBUS_MASTER_H_
