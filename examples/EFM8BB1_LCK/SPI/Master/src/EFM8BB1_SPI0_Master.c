//-----------------------------------------------------------------------------
// EFM8BB1_SPI0_Master.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates the SPI interface in 4-wire, master mode. This
// example is intended to be used with the SPI_Slave example.
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
//   P0.6   - SPI SCK
//   P0.7   - SPI MISO
//   P1.0   - SPI MOSI
//   P1.1   - SPI NSS
//   P1.4   - LED
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 LCK (SPI Master) + EFM8BB1 LCK (SPI Slave)
//-----------------------------------------------------------------------------
// 1) Connect the device first EFM8BB1 LCK to second EFM8BB1 LCK running the
//    corresponding SPI_Slave code.
// 2) Connect the EFM8BB1 LCK board to a PC using a micro USB cable.
// 3) Compile and download code to the first EFM8BB1 LCK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) If the communication passes, the LEDs on both the Master
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
#include <SI_EFM8BB1_Register_Enums.h>
#include "InitDevice.h"
#include "EFM8BB1_SPI0_Master.h"
#include "stdbool.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT (LED0, SFR_P1, 4);             // P1.4 LED Green
#define LED_ON	0
#define LED_OFF	1

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
uint8_t SPI_Data = 0xA5;
uint8_t SPI_Data_Array[MAX_BUFFER_SIZE] = {0};
bool Error_Flag = 0;
uint8_t Command = 0x00;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void SPI_LED_On (void);
void SPI_LED_Off (void);
void SPI_Byte_Write (void);
void SPI_Byte_Read (void);
void SPI_Array_Write (void);
void SPI_Array_Read (void);
void Delay(void);

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
   uint8_t test_value = 0x55;
   uint8_t test_array[MAX_BUFFER_SIZE] = {1,2,3,4,5,6,7,8};
   uint8_t i;

   enter_DefaultMode_from_RESET();


   IE_EA = 1;                          // Enable global interrupts

   LED0 = LED_ON;

   // Begin Test --------------------------------------------------------------

   SPI_Data = test_value;

   // Write a value
   SPI_Byte_Write();

   while(!SPI0CN0_NSSMD0);             // Wait until the Write transfer has
                                       // finished

   // Read the same value back
   SPI_Data = 0x00;
   SPI_Byte_Read ();

   while(!SPI0CN0_NSSMD0);             // Wait until the Read transfer has
                                       // finished

   // Check if the sent value and returned value match
   if(SPI_Data != test_value)
   {
      Error_Flag = 1;
   }

   // Copy test_array into SPI_Data_Array
   for(i = 0; i < MAX_BUFFER_SIZE; i++)
   {
      SPI_Data_Array[i] = test_array[i];
   }

   // Send the array to the slave
   SPI_Array_Write();

   while(!SPI0CN0_NSSMD0);             // Wait until the Write transfer has
                                       // finished

   // Clear SPI_Data_Array for the SPI_Buffer_Read function
   for(i = 0; i < MAX_BUFFER_SIZE; i++)
   {
      SPI_Data_Array[i] = 0;
   }

   // Read the array back from the slave
   SPI_Array_Read();

   while(!SPI0CN0_NSSMD0);             // Wait until the Read transfer has
                                       // finished

   // Check if the received array matches the sent array
   for(i = 0; i < MAX_BUFFER_SIZE; i++)
   {
      if(SPI_Data_Array[i] != test_array[i])
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
         LED0 = LED_ON;

         SPI_LED_On();

         while(!SPI0CN0_NSSMD0);

         Delay();

         SPI_LED_Off();

         LED0 = LED_OFF;

         while(!SPI0CN0_NSSMD0);

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
   while(!SPI0CN0_NSSMD0);             // Wait until the SPI is free, in case
                                       // it's already busy

   SPI0CN0_NSSMD0 = 0;

   Command = SLAVE_LED_ON;

   SPI0DAT = Command;

   // The rest of this command will be handled by the SPI ISR, which will
   // trigger when SPI0CN0_SPIF is set from sending the Command
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
   while(!SPI0CN0_NSSMD0);             // Wait until the SPI is free, in case
                                       // it's already busy

   SPI0CN0_NSSMD0 = 0;

   Command = SLAVE_LED_OFF;

   SPI0DAT = Command;

   // The rest of this command will be handled by the SPI ISR, which will
   // trigger when SPI0CN0_SPIF is set from sending the Command
}

//-----------------------------------------------------------------------------
// SPI_Byte_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Note: SPI_Data must contain the data to be sent before calling this
// function.
//
// Writes a single byte to the SPI Slave.  The slave does not respond to this
// command, so the command consists of:
//
// Command = SPI_WRITE
// Length = 1 byte of command, 1 byte of data
//
//-----------------------------------------------------------------------------
void SPI_Byte_Write(void)
{
   while(!SPI0CN0_NSSMD0);             // Wait until the SPI is free, in case
                                       // it's already busy

   SPI0CN0_NSSMD0 = 0;

   Command = SPI_WRITE;

   SPI0DAT = Command;

   // The rest of this command will be handled by the SPI ISR, which will
   // trigger when SPI0CN0_SPIF is set from sending the Command
}

//-----------------------------------------------------------------------------
// SPI_Byte_Read
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Note: SPI_Data will contain the data received after calling this function.
//
// Reads a single byte from the SPI Slave.  The command consists of:
//
// Command = SPI_READ
// Length = 1 byte of command, 1 byte of data
//
//-----------------------------------------------------------------------------
void SPI_Byte_Read(void)
{
   while(!SPI0CN0_NSSMD0);             // Wait until the SPI is free, in case
                                       // it's already busy

   SPI0CN0_NSSMD0 = 0;

   Command = SPI_READ;

   SPI0DAT = Command;

   // The rest of this command will be handled by the SPI ISR, which will
   // trigger when SPI0CN0_SPIF is set from sending the Command
}

//-----------------------------------------------------------------------------
// SPI_Array_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Note: SPI_Data_Array must contain the data to be sent before calling this
// function.
//
// Writes an array of values of size MAX_BUFFER_SIZE to the SPI Slave.  The
// command consists of:
//
// Command = SPI_WRITE_BUFFER
// Length = 1 byte of command, MAX_BUFFER_SIZE bytes of data
//
// Note: Polled mode is used for this function in order to buffer the data
// being sent using the SPI0CN0_TXBMT flag.
//
//-----------------------------------------------------------------------------
void SPI_Array_Write(void)
{
   uint8_t array_index;

   while(!SPI0CN0_NSSMD0);             // Wait until the SPI is free, in case
                                       // it's already busy

   IE_ESPI0 = 0;                       // Disable SPI interrupts

   SPI0CN0_NSSMD0 = 0;

   SPI0DAT = SPI_WRITE_BUFFER;         // Load the XMIT register
   while(SPI0CN0_TXBMT != 1)           // Wait until the command is moved into
   {                                   // the XMIT buffer
   }

   for(array_index = 0; array_index < MAX_BUFFER_SIZE; array_index++)
   {
      SPI0DAT = SPI_Data_Array[array_index]; // Load the data into the buffer
      while(SPI0CN0_TXBMT != 1)        // Wait until the data is moved into
      {                                // the XMIT buffer
      }
   }
   SPI0CN0_SPIF = 0;
   while(SPI0CN0_SPIF != 1)            // Wait until the last byte of the
   {                                   // data reaches the Slave
   }
   SPI0CN0_SPIF = 0;

   SPI0CN0_NSSMD0 = 1;                 // Disable the Slave

   IE_ESPI0 = 1;                       // Re-enable SPI interrupts
}

//-----------------------------------------------------------------------------
// SPI_Array_Read
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Note: SPI_Data_Array will contain the data received after calling this
// function.
//
// Reads a single byte from the SPI Slave.  The command consists of:
//
// Command = SPI_READ_BUFFER
// Length = 1 byte of command, MAX_BUFFER_SIZE bytes of data
//
//-----------------------------------------------------------------------------
void SPI_Array_Read(void)
{
   while(!SPI0CN0_NSSMD0);             // Wait until the SPI is free, in case
                                       // it's already busy

   SPI0CN0_NSSMD0 = 0;

   Command = SPI_READ_BUFFER;

   SPI0DAT = Command;

   // The rest of this command will be handled by the SPI ISR, which will
   // trigger when SPI0CN0_SPIF is set from sending the Command
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
