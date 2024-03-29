//-----------------------------------------------------------------------------
// EFM8BB51_SPI_Slave.h
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header for main file:

#ifndef SPI0_SLAVE_H_
#define SPI0_SLAVE_H_

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (LED0, SFR_P1, 5);             // LED0
#define LED_ON   0
#define LED_OFF  1

SI_SBIT (DISP_EN, SFR_P2, 0);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK                24500000 // Internal oscillator frequency in Hz

#define SPI_CLOCK               500000 // Maximum SPI clock
                                       // The SPI clock is a maximum of 1 MHz
                                       // when this example is used with
                                       // the SPI0_Slave code example.

#define MAX_BUFFER_SIZE              8 // Maximum buffer Master will send

// Instruction Set
#define  SLAVE_LED_ON             0x01 // Turn the Slave LED on
#define  SLAVE_LED_OFF            0x02 // Turn the Slave LED off
#define  SPI_WRITE                0x04 // Send a byte from the Master to the
                                       // Slave
#define  SPI_READ                 0x08 // Send a byte from the Slave to the
                                       // Master
#define  SPI_WRITE_BUFFER         0x10 // Send a series of bytes from the
                                       // Master to the Slave
#define  SPI_READ_BUFFER          0x20 // Send a series of bytes from the Slave
                                       // to the Master
#define  ERROR_OCCURRED           0x40 // Indicator for the Slave to tell the
                                       // Master an error occurred

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
extern uint8_t SPI_Data;
extern uint8_t SPI_Data_Array[MAX_BUFFER_SIZE];
extern bool Error_Flag;
extern uint8_t Command;

#endif // SPI0_SLAVE_H_
