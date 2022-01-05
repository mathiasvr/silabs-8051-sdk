//-----------------------------------------------------------------------------
// EFM8BB3_I2C_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2017 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Example software to demonstrate the I2C Slave interface.
// - Interrupt-driven I2C Slave implementation
// - Only slave states defined
// - Received data will be stored in the i2cReceivedData
// - Transmit the data value 0-255 repeatedly
// - Timer4 used by I2C Slave for SCL low timeout detection
// - ARBLOST support included
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   I2C Slave
//   Timer4 - I2C SCL low timeout detection
//   On (QFN32, QFP32) parts(QFN32 part is used on STK):
//     P2.0   - I2C SDA
//     P2.1   - I2C SCL
//   On (QFN24, QSOP24) parts:
//     P1.3   - I2C SDA
//     P1.4   - I2C SCL
//   P1.4   - LED green
//   P2.3   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB3 STK (I2C Slave) + Board running SMBus Master
//   (for example, EFM8BB3 STK or a C8051Fxxx DK board)
//-----------------------------------------------------------------------------
// 1) Connect the EFM8BB3 STK to the board running the corresponding
//    SMBus_Master code.
// 2) Place the switch in "AEM" mode.
// 3) Connect the EFM8BB3 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8BB3 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) This example works with FIFO request interrupt(FIFO threshold) enabled mode.
// 7) Firmware fill the TX FIFO on each Transmit FIFO Request (TFRQ) interrupt
//    until the TX FIFO full, clock stretching won't be caused by RD interrupt.
// 8) Firmware read one or more bytes of data from the RX FIFO on each Receive
//    FIFO Request (RFRQ) interrupt, clock stretching won't be caused by WR interrupt.
// Target:         EFM8BB3
// Tool chain:     Generic
//
// Release 0.1 (CY)
//    - Initial Revision
//    - 19 June 2017
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB3_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include "EFM8BB3_I2C_Slave.h"
// $[Generated Includes]
// [Generated Includes]$
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
uint8_t i2cReceivedData;               // Global holder for I2C data.
                                       // All receive data is written
                                       // here;

bool dataReady = 0;                    // Set to '1' by the I2C ISR
                                       // when a new data byte has been
                                       // received.

bool txDataReady = 1;                  // Set to '1' indicate that Tx data ready.
uint8_t sendDataValue = 0;             // Transmit the data value 0-255 repeatedly.
uint8_t sendDataCnt = 0;               // Transmit data counter. Count the Tx data
                                       // in a I2C transaction.

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
  // Disable the watchdog here
}
 
//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, then waits for I2C
// communication.
//
//-----------------------------------------------------------------------------
int main (void)
{
	// Call hardware initialization routine
    enter_DefaultMode_from_RESET();

    DISP_EN = DISP_BC_DRIVEN;           // EFM8 does not drive display

    LED1 = 1;

    IE_EA = 0;
    SFRPAGE = PG3_PAGE;
    I2C0CN0 &= ~I2C0CN0_BUSY__BMASK;		// Clear BUSY bit
    I2C0FCN0 |= I2C0FCN0_RFLSH__FLUSH | I2C0FCN0_TFLSH__FLUSH;
    IE_EA = 1;

    while (1)
    {
// $[Generated Run-time code]

        while (!dataReady);             // New I2C data received?
        dataReady = 0;
        LED1 = !LED1;
   }                             
}
