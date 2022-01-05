//-----------------------------------------------------------------------------
// SPI0_Lib_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates the SPI interface in slave mode using the SPI
// peripheral driver library. This example is intended to be used with the
// SPI_Lib_Master example with a maximum SPI_CLOCK frequency of 1 MHz.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC / 1
//   SPI    - Slave
//   P0.0   - Display enable
//   P0.6   - SPI SCK
//   P0.7   - SPI MISO
//   P1.0   - SPI MOSI
//   P1.1   - SPI NSS
//   P1.4   - LED
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 STK (SPI Slave) + EFM8BB1 STK (SPI Master)
//-----------------------------------------------------------------------------
// 1) Connect the first EFM8BB1 STK SPI pins to the second EFM8BB1 STK running the
//    corresponding SPI_Master code with SPI_CLOCK set to 1 MHz or lower.
// 2) Place the switch in "AEM" mode.
// 3) Connect the EFM8BB1 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the first EFM8BB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) If the communication passes, the LEDs on both the Master
//    and Slave boards will blink slowly. If it fails, the LEDs will be off.
//
// Target:         EFM8BB1
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 10 OCT 2014
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "bsp.h"
#include "InitDevice.h"
#include "SPI0_Lib_Slave.h"
#include "spi_0.h"
#include <string.h>

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------

uint8_t TransferByte(uint8_t tx);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

SI_SEGMENT_VARIABLE(SPI_TxBuf[MAX_BUFFER_SIZE+1],
                    uint8_t,
                    EFM8PDL_SPI0_TX_SEGTYPE);
SI_SEGMENT_VARIABLE(SPI_RxBuf[MAX_BUFFER_SIZE+1],
                    uint8_t,
                    EFM8PDL_SPI0_RX_SEGTYPE);

uint8_t test_value_rx = 0;
uint8_t test_array_rx[MAX_BUFFER_SIZE];

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

// Receive a byte from the master
uint8_t ReceiveCommand(void)
{
  // Send dummy byte and read command byte
  return TransferByte(0xFF);
}

// Send and receive a byte
uint8_t TransferByte(uint8_t tx)
{
  // Send tx byte
  SPI0_writeByte(tx);

  // Return rx byte
  while (SPI0_isRxEmpty());
  return SPI0_readByte();
}

// Send the response for the specified command (if necessary)
void SendResponse(uint8_t command)
{
  uint8_t i;

  switch (command)
  {
    case SPI_WRITE:
      test_value_rx = TransferByte(0xFF);
      break;

    case SPI_READ:
      TransferByte(test_value_rx);
      break;

    case SPI_WRITE_BUFFER:
      for (i = 0; i < MAX_BUFFER_SIZE; i++)
      {
        test_array_rx[i] = TransferByte(0xFF);
      }
      break;

    case SPI_READ_BUFFER:
      for (i = 0; i < MAX_BUFFER_SIZE; i++)
      {
        TransferByte(test_array_rx[i]);
      }
      break;

    case SLAVE_LED_ON:
      BSP_LED_G = BSP_LED_ON;
      break;

    case SLAVE_LED_OFF:
      BSP_LED_G = BSP_LED_OFF;
      break;
  }
}

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
void main(void)
{
  uint8_t command;

  enter_DefaultMode_from_RESET();

  BSP_DISP_EN = 0;                    // Display not driven by EFM8
  BSP_LED_G = BSP_LED_OFF;

  while (1)
  {
    // Receieve a command from the master
    command = ReceiveCommand();

    // Send the command response to the master
    SendResponse(command);
  }
}
