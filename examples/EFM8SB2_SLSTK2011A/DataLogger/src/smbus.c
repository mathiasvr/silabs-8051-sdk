//-----------------------------------------------------------------------------
// smbus.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "bsp.h"
#include "smbus.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
// Device addresses (7 bits, LSB is a don't care)
#define  SLAVE_ADDR     0x80           // Device address for slave target

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
uint8_t SMB_DATA_IN;                   // Global holder for SMBus data
                                       // All receive data is written here

uint8_t SMB_DATA_OUT;                  // Global holder for SMBus data.
                                       // All transmit data is read from here

uint8_t TARGET;                        // Target SMBus slave address

volatile bool SMB_BUSY;                // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

volatile bool SMB_RW;                  // Software flag to indicate the
                                       // direction of the current transfer

uint16_t NUM_ERRORS;                   // Counter for the number of errors.
//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
// Functions used to communicate with the offchip temperature/humidity sensor
void ResetSensor();
//uint16_t MeasureAndReadTemperature();
uint16_t ReadTemperature();
uint16_t MeasureAndReadHumidity();
uint16_t SendSensorCommand(uint8_t command, bool readback);

void SMB_Write (void);
void SMB_Read (void);

//-----------------------------------------------------------------------------
// Sensor Functions
//-----------------------------------------------------------------------------
#if 0
//---------------------------------------------------------------------------
// ResetSensor
//---------------------------------------------------------------------------
//
// Description - resets the offchip temperature/humidity sensor
//
//
// return - returns the measured temperature value.
void ResetSensor()
{
    SendSensorCommand(0xFE, 0);
}
#endif
//---------------------------------------------------------------------------
// MeasureAndReadTemperature
//---------------------------------------------------------------------------
//
// Description - commands the offchip temperature/humidity sensor to measure temperature
//
//
// return - returns the measured temperature value.
//uint16_t MeasureAndReadTemperature()
//{
//        return SendSensorCommand(0xE3, 1);
//}

//---------------------------------------------------------------------------
// ReadTemperature
//---------------------------------------------------------------------------
//
// Description - reads the last measured temperature from the offchip temperature/humidity sensor sensor
//
//
// return - returns the last measured temperature value.
uint16_t ReadTemperature()
{
    return SendSensorCommand(0xE0, 1);
}

//---------------------------------------------------------------------------
// MeasureAndReadHumidity
//---------------------------------------------------------------------------
//
// Description - commands the offchip temperature/humidity sensor to measure humidity.
// Note - This sensor measures both temperature and humidity when commanded to measure humidity.
//
//
// return - returns the measured humidity value.
uint16_t MeasureAndReadHumidity()
{
    return SendSensorCommand(0xE5, 1);
}

//---------------------------------------------------------------------------
// SendSensorCommand
//---------------------------------------------------------------------------
//
// Description - sends a hex value command to the sensor
//
// command - the hex value of a command that the offchip temperature/humidity sensor is to perform
//
//
// return - returns the measured humidity value.
uint16_t SendSensorCommand(uint8_t command, bool readback)
{
    uint16_t result;

    // SMBus Write Sequence
    SMB_DATA_OUT = command;          // Define next outgoing byte
    TARGET = SLAVE_ADDR;             // Target the EFM8 Slave for next
                                     // SMBus transfer
    SMB_Write();                     // Initiate SMBus write

    if (readback == 1)
    {
        // SMBus Read Sequence
        TARGET = SLAVE_ADDR;         // Target the EFM8 Slave for next
                                     // SMBus transfer
        SMB_Read();
        result = SMB_DATA_IN;
        result = result << 8;
        SMB_Read();
        result += SMB_DATA_IN;
    }
    else
    {
        result = 0;
    }

    return result;
}

//-----------------------------------------------------------------------------
// SMB_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Writes a single byte to the slave with address specified by the <TARGET>
// variable.
// Calling sequence:
// 1) Write target slave address to the <TARGET> variable
// 2) Write outgoing data to the <SMB_DATA_OUT> variable
// 3) Call SMB_Write()
//
//-----------------------------------------------------------------------------
void SMB_Write (void)
{
        while (SMB_BUSY);                   // Wait for SMBus to be free.
        SMB_BUSY = 1;                       // Claim SMBus (set to busy)
        SMB_RW = 0;                         // Mark this transfer as a WRITE
        SMB0CN0_STA = 1;                     // Start transfer
}

//-----------------------------------------------------------------------------
// SMB_Read
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Reads a single byte from the slave with address specified by the <TARGET>
// variable.
// Calling sequence:
// 1) Write target slave address to the <TARGET> variable
// 2) Call SMB_Write()
// 3) Read input data from <SMB_DATA_IN> variable
//
//-----------------------------------------------------------------------------
void SMB_Read (void)
{
        while (SMB_BUSY != 0);              // Wait for bus to be free.
        SMB_BUSY = 1;                       // Claim SMBus (set to busy)
        SMB_RW = 1;                         // Mark this transfer as a READ

        SMB0CN0_STA = 1;                     // Start transfer

        while (SMB_BUSY);                   // Wait for transfer to complete
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
