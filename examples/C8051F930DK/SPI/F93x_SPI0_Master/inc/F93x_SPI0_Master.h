/*
 * F93x_SPI0_Master.h
 *
 *  Created on: Jun 6, 2014
 *      Author: jiguo1
 */

#ifndef F93X_SPI0_MASTER_MAIN_H_
#define F93X_SPI0_MASTER_MAIN_H_


//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define MAX_BUFFER_SIZE    8           // Maximum buffer Master will send

// Instruction Set
#define  SLAVE_LED_ON      0x01        // Turn the Slave LED on
#define  SLAVE_LED_OFF     0x02        // Turn the Slave LED off
#define  SPI_WRITE         0x04        // Send a byte from the Master to the
                                       // Slave
#define  SPI_READ          0x08        // Send a byte from the Slave to the
                                       // Master
#define  SPI_WRITE_BUFFER  0x10        // Send a series of bytes from the
                                       // Master to the Slave
#define  SPI_READ_BUFFER   0x20        // Send a series of bytes from the Slave
                                       // to the Master
#define  ERROR_OCCURRED    0x40        // Indicator for the Slave to tell the
                                       // Master an error occurred

#define  LED_ON             0
#define  LED_OFF            1

SI_SBIT(LED, SFR_P1, 6);                 // LED==LED_ON means ON

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

extern uint8_t SPI_Data;

extern uint8_t SPI_Data_Array[MAX_BUFFER_SIZE];

extern bit Error_Flag;

extern uint8_t Command;


#endif /* F93X_SPI0_MASTER_MAIN_H_ */
