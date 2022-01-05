//-----------------------------------------------------------------------------
// F85x_SMBus_EEPROM.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header for main file:

#ifndef F85X_SMBUS_EEPROM_H_
#define F85X_SMBUS_EEPROM_H_

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define  SYSCLK               24500000 // System clock frequency in Hz

#define  SMB_FREQUENCY           50000 // Target SCL clock rate
                                       // This example supports between 10kHz
                                       // and 100kHz

#define  WRITE                    0x00 // SMBus WRITE command
#define  READ                     0x01 // SMBus READ command

// Device addresses (7 bits, lsb is a don't care)
#define  EEPROM_ADDR              0xA0 // Device address for slave target
                                       // Note: This address is specified
                                       // in the Microchip 24LC02B
                                       // datasheet.
// SMBus Buffer Size
#define  SMB_BUFF_SIZE            0x08 // Defines the maximum number of bytes
                                       // that can be sent or received in a
                                       // single transfer

// Status vector - top 4 bits only
#define  SMB_MTSTA                0xE0 // (MT) start transmitted
#define  SMB_MTDB                 0xC0 // (MT) data byte transmitted
#define  SMB_MRDB                 0x80 // (MR) data byte received
// End status vector definition

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
extern uint8_t* pSMB_DATA_IN;          // Global pointer for SMBus data
                                       // All receive data is written here

extern uint8_t SMB_SINGLEBYTE_OUT;     // Global holder for single byte writes.

extern uint8_t* pSMB_DATA_OUT;         // Global pointer for SMBus data.
                                       // All transmit data is read from here

extern uint8_t SMB_DATA_LEN;           // Global holder for number of bytes
                                       // to send or receive in the current
                                       // SMBus transfer.

extern uint8_t WORD_ADDR;              // Global holder for the EEPROM word
                                       // address that will be accessed in
                                       // the next transfer

extern uint8_t TARGET;                 // Target SMBus slave address

extern volatile bool SMB_BUSY;         // Software flag to indicate when the
                                       // EEPROM_ByteRead() or
                                       // EEPROM_ByteWrite()
                                       // functions have claimed the SMBus

extern volatile bool SMB_RW;           // Software flag to indicate the
                                       // direction of the current transfer

extern volatile bool SMB_SENDWORDADDR; // When set, this flag causes the ISR
                                       // to send the 8-bit <WORD_ADDR>
                                       // after sending the slave address.


extern volatile bool SMB_RANDOMREAD;   // When set, this flag causes the ISR
                                       // to send a START signal after sending
                                       // the word address.
                                       // For the 24LC02B EEPROM, a random read
                                       // (a read from a particular address in
                                       // memory) starts as a write then
                                       // changes to a read after the repeated
                                       // start is sent. The ISR handles this
                                       // switchover if the <SMB_RANDOMREAD>
                                       // bit is set.

extern volatile bool SMB_ACKPOLL;      // When set, this flag causes the ISR
                                       // to send a repeated START until the
                                       // slave has acknowledged its address

#endif // F85X_SMBUS_EEPROM_H_
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
