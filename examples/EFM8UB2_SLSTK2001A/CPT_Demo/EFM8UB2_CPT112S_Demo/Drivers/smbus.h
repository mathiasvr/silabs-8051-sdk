/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
#ifndef SMBUS_H_
#define SMBUS_H_

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
#define  SYSCLK         24000000       // System clock frequency in Hz

#define  WRITE          0x00           // SMBus WRITE command
#define  READ           0x01           // SMBus READ command

// Device addresses (7 bits address)
#define  SLAVE_ADDR     0xE0           // Device address for slave target

#define  NUM_BYTES_WR   3              // Number of bytes to write
                                       // Master -> Slave
#define  NUM_BYTES_RD   3              // Number of bytes to read
                                       // Master <- Slave

// project variables
extern uint16_t capsenseCurrent;
extern uint16_t capsensePrevious;
extern uint16_t sliderCurrent;
extern uint16_t sliderPrevious;
extern uint16_t sliderValue;

extern volatile bool transferInProgress;

/******************************************************************************
 * SMBUS data structure initialization
 *****************************************************************************/
void initSMBUS(void);

/******************************************************************************
 * update SMBUS data structure from current packet of SMBUS
 *****************************************************************************/
void updateSMBUS(void);

#endif /* SMBUS_H_ */
