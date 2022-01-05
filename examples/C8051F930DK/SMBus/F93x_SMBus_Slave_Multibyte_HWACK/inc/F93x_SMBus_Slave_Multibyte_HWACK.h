/*
 * F93x_SMBus_Slave_Multibyte_HWACK.h
 *
 *  Created on: Jun 9, 2014
 *      Author: jiguo1
 */

#ifndef F93X_SMBUS_SLAVE_MULTIBYTE_HWACK_MAIN_H_
#define F93X_SMBUS_SLAVE_MULTIBYTE_HWACK_MAIN_H_

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define  EXTHOLD_ENABLE  1             // Sets EXTHOLD to 1 to prevent the MCU
                                       // from receiving an interrupt when
                                       // another slave is being addressed.

#define  WRITE          0x00           // SMBus WRITE command
#define  READ           0x01           // SMBus READ command

#define LED_ON           0             // Turns the LED on
#define LED_OFF          1             // Turns the LED off

#define  SLAVE_ADDR     0xF0           // Device addresses (7 bits,
                                       // lsb is a don't care)

// Status vector - top 4 bits only
#define  SMB_SRADD      0x20           // (SR) slave address received
                                       //    (also could be a lost
                                       //    arbitration)
#define  SMB_SRSTO      0x10           // (SR) STOP detected while SR or ST,
                                       //    or lost arbitration
#define  SMB_SRDB       0x00           // (SR) data byte received, or
                                       //    lost arbitration
#define  SMB_STDB       0x40           // (ST) data byte transmitted
#define  SMB_STSTO      0x50           // (ST) STOP detected during a
                                       //    transaction; bus error
// End status vector definition

#define  NUM_BYTES_WR   3              // Number of bytes to write
                                       // Slave <- Master
#define  NUM_BYTES_RD   3              // Number of bytes to read
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

extern bit DATA_READY;                    // Set to '1' by the SMBus ISR
                                       // when a new data byte has been
                                       // received.

#if(EXTHOLD_ENABLE)
extern uint8_t first_entry;                    // Set to 1 to indicate that
                                       // the start bit should be set
                                       // on the next SMBus interrupt
#endif

#endif /* F93X_SMBUS_SLAVE_MULTIBYTE_HWACK_MAIN_H_ */
