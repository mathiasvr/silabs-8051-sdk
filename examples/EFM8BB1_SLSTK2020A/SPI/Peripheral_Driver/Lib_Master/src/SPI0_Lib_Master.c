//-----------------------------------------------------------------------------
// SPI0_Lib_Master.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates the SPI interface in 4-wire, master mode using
// the SPI peripheral driver library. This example is intended to be used
// with the SPI_Lib_Slave example.
//
// The SPI clock in this example is limited to 1 MHz when used with the
// SPI0_Slave code example. During a SPI_Read, the slave needs some time to
// interpret the command and write the appropriate data to the SPI0DAT
// register, and the slave no longer has enough time to complete the
// SPI_READ_BUFFER command with a clock greater than 1 MHz. For faster SPI
// clocks, a dummy byte between the command and the first byte of Read data
// will be required.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC / 1
//   SPI    - Master
//   P0.0   - Display enable
//   P0.6   - SPI SCK
//   P0.7   - SPI MISO
//   P1.0   - SPI MOSI
//   P1.1   - SPI NSS
//   P1.4   - LED
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 STK (SPI Master) + EFM8BB1 STK (SPI Slave)
//-----------------------------------------------------------------------------
// 1) Connect the first EFM8BB1 STK SPI pins to the second EFM8BB1 STK running
//    the corresponding SPI_Slave code.
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
#include "SPI0_Lib_Master.h"
#include "spi_0.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

SI_SEGMENT_VARIABLE(SPI_TxBuf[MAX_BUFFER_SIZE+1], uint8_t, EFM8PDL_SPI0_TX_SEGTYPE);
SI_SEGMENT_VARIABLE(SPI_RxBuf[MAX_BUFFER_SIZE+1], uint8_t, EFM8PDL_SPI0_RX_SEGTYPE);

bool Error_Flag = 0;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void SPI_LED_On (void);
void SPI_LED_Off (void);
void SPI_Byte_Write (uint8_t writeData);
uint8_t SPI_Byte_Read(void);
void SPI_Array_Write(uint8_t writeData[], uint8_t size);
void SPI_Array_Read(uint8_t readData[], uint8_t size);
void Delay(void);

//-----------------------------------------------------------------------------
// Callbacks
//-----------------------------------------------------------------------------

// Typical Write:
//
//              | 1st sent | 2nd sent | 3rd sent |   ...    | last sent |
//              ---------------------------------------------------------
//  Master NSSv | Command  |   Data1  |   Data2  |   ...    |   DataN   |  NSS^
//  Slave       |   N/A    |    N/A   |    N/A   |   ...    |    N/A    |
//
// Typical Read:
//
//              | 1st sent | 2nd sent | 3rd sent |   ...    | last sent |
//              ---------------------------------------------------------
//  Master NSSv | Command  |   dummy  |   dummy  |   ...    |   dummy   |  NSS^
//  Slave       |   N/A    |   Data1  |   Data2  |   ...    |   DataN   |
//

void SPI0_transferCompleteCb(void)
{

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
   uint8_t test_value_tx = 0x55;
   uint8_t test_value_rx = 0x00;
   uint8_t test_array_tx[MAX_BUFFER_SIZE] = {1,2,3,4,5,6,7,8};
   uint8_t test_array_rx[MAX_BUFFER_SIZE];
   uint8_t i;

   enter_DefaultMode_from_RESET();

   BSP_DISP_EN = 0;                    // Display not driven by EFM8

   IE_EA = 1;                          // Enable global interrupts

   BSP_LED_G = BSP_LED_ON;

   // Begin Test --------------------------------------------------------------

   // Write a value
   SPI_Byte_Write(test_value_tx);

   // Read the same value back
   if ((test_value_rx = SPI_Byte_Read()) != test_value_tx)
   {
      Error_Flag = 1;
   }

   // Send the array to the slave
   SPI_Array_Write(test_array_tx, MAX_BUFFER_SIZE);

   // Clear receive buffer
   for(i = 0; i < MAX_BUFFER_SIZE; i++)
   {
      test_array_rx[i] = 0;
   }

   // Read the array back from the slave
   SPI_Array_Read(test_array_rx, MAX_BUFFER_SIZE);

   // Check if the received array matches the sent array
   for(i = 0; i < MAX_BUFFER_SIZE; i++)
   {
      if(test_array_rx[i] != test_array_tx[i])
      {
         Error_Flag = 1;
      }
   }

   // End of Test -------------------------------------------------------------

   while (1)
   {
      // If no error has occurred, blink the LEDs on the Master and Slave
      // boards
      if (Error_Flag == 0)
      {
         BSP_LED_G = BSP_LED_ON;

         SPI_LED_On();

         Delay();

         SPI_LED_Off();

         BSP_LED_G = BSP_LED_OFF;

         Delay();
      }
   }
}

//-----------------------------------------------------------------------------
// Support Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SPI_LED_On
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Turns the LED on the SPI Slave on.  The slave does not respond to this
// command, so the command consists of:
//
// Command = SLAVE_LED_ON
// Length = 1 byte (the command itself)
//
//-----------------------------------------------------------------------------
void SPI_LED_On(void)
{
   // Wait until the SPI is free, in case it's already busy
   while(!SPI0CN0_NSSMD0);

   // Setup command buffer
   SPI_TxBuf[0] = SLAVE_LED_ON;

   // Transfer command
   SPI0_transfer(SPI_TxBuf, NULL, SPI0_TRANSFER_TX, 1);

   // Wait until the SPI transfer is complete
   while(!SPI0CN0_NSSMD0);
}

//-----------------------------------------------------------------------------
// SPI_LED_Off
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Turns the LED on the SPI Slave off.  The slave does not respond to this
// command, so the command consists of:
//
// Command = SLAVE_LED_OFF
// Length = 1 byte (the command itself)
//
//-----------------------------------------------------------------------------
void SPI_LED_Off(void)
{
  // Wait until the SPI is free, in case it's already busy
  while(!SPI0CN0_NSSMD0);

  // Setup command buffer
  SPI_TxBuf[0] = SLAVE_LED_OFF;

  // Transfer command
  SPI0_transfer(SPI_TxBuf, NULL, SPI0_TRANSFER_TX, 1);

  // Wait until the SPI transfer is complete
  while(!SPI0CN0_NSSMD0);
}

//-----------------------------------------------------------------------------
// SPI_Byte_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : writeData - the byte to send to the slave
//
// Writes a single byte to the SPI Slave.  The slave does not respond to this
// command, so the command consists of:
//
// Command = SPI_WRITE
// Length = 1 byte of command, 1 byte of data
//
//-----------------------------------------------------------------------------
void SPI_Byte_Write(uint8_t writeData)
{
   // Wait until the SPI is free, in case it's already busy
   while(!SPI0CN0_NSSMD0);

   // Setup command buffer
   SPI_TxBuf[0] = SPI_WRITE;
   SPI_TxBuf[1] = writeData;

   // Transfer command
   SPI0_transfer(SPI_TxBuf, NULL, SPI0_TRANSFER_TX, 2);

   // Wait until the SPI transfer is complete
   while(!SPI0CN0_NSSMD0);
}

//-----------------------------------------------------------------------------
// SPI_Byte_Read
//-----------------------------------------------------------------------------
//
// Return Value : The byte read from the slave
// Parameters   : None
//
// Reads a single byte from the SPI Slave.  The command consists of:
//
// Command = SPI_READ
// Length = 1 byte of command, 1 byte of data
//
//-----------------------------------------------------------------------------
uint8_t SPI_Byte_Read(void)
{
  uint8_t dummy = 0xFF;

  // Wait until the SPI is free, in case it's already busy
  while(!SPI0CN0_NSSMD0);

  // Setup command buffer
  SPI_TxBuf[0] = SPI_READ;
  SPI_TxBuf[1] = dummy;

  // Transfer command
  SPI0_transfer(SPI_TxBuf, SPI_RxBuf, SPI0_TRANSFER_RXTX, 2);

  // Wait until the SPI transfer is complete
  while(!SPI0CN0_NSSMD0);

  return SPI_RxBuf[1];
}

//-----------------------------------------------------------------------------
// SPI_Array_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : writeData - A buffer of data to send to the slave
//                size - The number of bytes in the buffer to send
//
// Writes an array of values of size MAX_BUFFER_SIZE to the SPI Slave.  The
// command consists of:
//
// Command = SPI_WRITE_BUFFER
// Length = 1 byte of command, MAX_BUFFER_SIZE bytes of data
//
//-----------------------------------------------------------------------------
void SPI_Array_Write(uint8_t writeData[], uint8_t size)
{
  uint8_t i;
  uint8_t dummy = 0xFF;

  // Wait until the SPI is free, in case it's already busy
  while(!SPI0CN0_NSSMD0);

  // Setup command buffer
  SPI_TxBuf[0] = SPI_WRITE_BUFFER;

  // Copy write data to TX buffer
  for (i = 0; i < size; i++)
  {
    SPI_TxBuf[1 + i] = writeData[i];
  }

  // Transfer command
  SPI0_transfer(SPI_TxBuf, NULL, SPI0_TRANSFER_TX, size + 1);

  // Wait until the SPI transfer is complete
  while(!SPI0CN0_NSSMD0);
}

//-----------------------------------------------------------------------------
// SPI_Array_Read
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : readData - A buffer to store the received byte array
//                size - Number of bytes to read from the slave
//
// Reads multiple bytes from the SPI Slave.  The command consists of:
//
// Command = SPI_READ_BUFFER
// Length = 1 byte of command, MAX_BUFFER_SIZE bytes of data
//
//-----------------------------------------------------------------------------
void SPI_Array_Read(uint8_t readData[], uint8_t size)
{
  uint8_t i;
  uint8_t dummy = 0xFF;

  if (size > MAX_BUFFER_SIZE)
  {
     return;
  }

  // Wait until the SPI is free, in case it's already busy
  while(!SPI0CN0_NSSMD0);

  // Setup command buffer
  SPI_TxBuf[0] = SPI_READ_BUFFER;

  // Send dummy bytes to shift in RX data
  for (i = 0; i < size; i++)
  {
     SPI_TxBuf[1 + i] = dummy;
  }

  // Transfer command
  SPI0_transfer(SPI_TxBuf, SPI_RxBuf, SPI0_TRANSFER_RXTX, size + 1);

  // Wait until the SPI transfer is complete
  while(!SPI0CN0_NSSMD0);

  // Copy RX data into user buffer
  for (i = 0; i < size; i++)
  {
      readData[i] = SPI_RxBuf[1 + i];
  }
}

//-----------------------------------------------------------------------------
// Delay
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Delay for little while (used for blinking the LEDs)
//
//-----------------------------------------------------------------------------
void Delay(void)
{
   uint32_t count;

   for (count = 200000; count > 0; count--);
}
