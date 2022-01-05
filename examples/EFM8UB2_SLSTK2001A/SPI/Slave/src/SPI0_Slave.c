//-----------------------------------------------------------------------------
// SPI0_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program configures a EFM8UB2 as a SPI Slave. The EFM8UB2 MCU
// is configured in 4-wire Slave Mode.
//
// This example is intended to be used with the SPI0_Master example.
//
// Pinout:
//
// P0.6 - SPI SCK    (digital input, open-drain)
// P0.7 - SPI MISO   (digital output, push-pull)
// P1.0 - SPI MOSI   (digital input, open-drain)
// P1.1 - SPI NSS    (digital input, open-drain)
//
// P1.6 - LED        (digital output, push-pull)
//
// all other port pins unused.
//
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect the device to another EFM8 device running SPI Master code.
// 3) Connect the EFM8UB2 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8UB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) If the communication passes, the LEDs on both the Master and Slave
//    boards will blink slowly. If it fails, the LEDs will be OFF.
//
//
// Target:         EFM8UB2
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             12000000    // Internal oscillator frequency in Hz

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

SI_SBIT (LED1, SFR_P1, 6);                // DS5 P1.0 LED
#define LED_ON                       0
#define LED_OFF                      1
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint8_t SPI_Data = 0xA5;

uint8_t SPI_Data_Array[MAX_BUFFER_SIZE] = {0};

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
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   enter_DefaultMode_from_RESET();

   LED1 = LED_OFF;

   while (1);                          // Loop forever
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
