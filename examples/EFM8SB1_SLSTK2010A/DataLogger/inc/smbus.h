//-----------------------------------------------------------------------------
// smbus.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//

#ifndef SMBUS_H_
#define SMBUS_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "bsp.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define  WRITE          0x00           // SMBus WRITE command
#define  READ           0x01           // SMBus READ command

// Status vector - top 4 bits only
#define  SMB_MTSTA      0xE0           // (MT) start transmitted
#define  SMB_MTDB       0xC0           // (MT) data byte transmitted
#define  SMB_MRDB       0x80           // (MR) data byte received
// End status vector definition

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
extern uint8_t SMB_DATA_IN;            // Global holder for SMBus data
                                       // All receive data is written here

extern uint8_t SMB_DATA_OUT;           // Global holder for SMBus data.
                                       // All transmit data is read from here

extern uint8_t TARGET;                 // Target SMBus slave address

extern volatile bool SMB_BUSY;         // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

extern volatile bool SMB_RW;           // Software flag to indicate the
                                       // direction of the current transfer

extern uint16_t NUM_ERRORS;            // Counter for the number of errors.
//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
// Functions used to communicate with the offchip temperature/humidity sensor
void ResetSensor();
//uint16_t MeasureAndReadTemperature();
uint16_t ReadTemperature();
uint16_t MeasureAndReadHumidity();

#endif /* SMBUS_H_ */
