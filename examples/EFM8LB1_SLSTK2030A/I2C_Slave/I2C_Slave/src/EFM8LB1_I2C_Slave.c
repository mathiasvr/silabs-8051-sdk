//-----------------------------------------------------------------------------
// EFM8LB1_I2C_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Example software to demonstrate the I2C Slave interface.
// - Interrupt-driven I2C Slave implementation
// - Only slave states defined
// - 1-byte I2C Slave data holder used for both transmit and receive
// - Timer3 used by I2C Slave for SCL low timeout detection
// - ARBLOST support included
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   I2C Slave
//   Timer3 - SMBus SCL low timeout detection
//   On (QFN32, QFP32) parts(QFN32 part is used on STK):
//     P2.0   - SMBus SDA
//     P2.1   - SMBus SCL
//   On (QFN24, QSOP24) parts:
//     P1.3   - SMBus SDA
//     P1.4   - SMBus SCL
//   P1.4   - LED green
//   P2.3   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8LB1 STK (I2C Slave) + Board running SMBus Master
//   (for example, EFM8LB1 STK or a C8051Fxxx DK board)
//-----------------------------------------------------------------------------
// 1) Connect the EFM8LB1 STK to the board running the corresponding
//    SMBus_Master code.
// 2) Place the switch in "AEM" mode.
// 3) Connect the EFM8LB1 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the first EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) The slave code will write data and read data from the
//    same data byte, so a successive write and read will effectively echo the
//    data written. To verify that the code is working properly, verify on the
//    master that the data written is the same as the data received.
// 7) This example also works with FIFO request interrupt(FIFO threshold) enabled mode
//    Enable the line in InitDevice.c to test
//       I2C0FCN0 |= I2C0FCN0_RFRQE__ENABLED
//
// Target:         EFM8LB1
// Tool chain:     Generic
//
// Release 0.1 (SY)
//    - Initial Revision
//    - 25 MAY 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8LB1_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include "EFM8LB1_I2C_Slave.h"
// $[Generated Includes]
// [Generated Includes]$
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
uint8_t I2C_DATA;                      // Global holder for SMBus data.
                                       // All receive data is written
                                       // here;
                                       // all transmit data is read
                                       // from here

bool DATA_READY = 0;                    // Set to '1' by the SMBus ISR
                                       // when a new data byte has been
                                       // received.

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
// Main routine performs all configuration tasks, then waits for SMBus
// communication.
//
//-----------------------------------------------------------------------------
int main (void)
{
	// Call hardware initialization routine
	enter_DefaultMode_from_RESET();

    DISP_EN = DISP_BC_DRIVEN;           // EFM8 does not drive display

    I2C_DATA = 0xFD;                    // Initialize SMBus data holder

    LED1 = 1;
    while (1)
    {
// $[Generated Run-time code]
// [Generated Run-time code]$
	    IE_EA = 0;
	    SFRPAGE = PG3_PAGE;
	    I2C0CN0 &= ~I2C0CN0_BUSY__BMASK;		// Clear BUSY bit
	    I2C0CN0 |= I2C0FCN0_RFLSH__FLUSH | I2C0FCN0_TFLSH__FLUSH;
	    IE_EA = 1;

        while (!DATA_READY);             // New I2C data received?
        DATA_READY = 0;
        LED1 = !LED1;
   }                             
}
