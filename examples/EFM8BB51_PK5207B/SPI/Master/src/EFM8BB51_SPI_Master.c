//-----------------------------------------------------------------------------
// EFM8BB51_SPI_Master.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
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
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   SPI    - Master
//   P0.6   - SPI SCK
//   P0.7   - SPI MISO
//   P1.0   - SPI MOSI
//   P1.1   - SPI NSS
//   P1.5   - LED
//   P2.0   - Display enable
//   GND    - EXP Header 1
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB51 PK (SPI Master) + EFM8BB51 PK (SPI Slave)
//-----------------------------------------------------------------------------
// 1) Connect the device first EFM8BB51 PK to second EFM8BB51 PK running the
//    corresponding SPI_Slave code.
// 2) Place the switch in "AEM" mode.
// 3) Connect the EFM8BB51 PK board to a PC using a USB cable.
// 4) Compile and download code to the first EFM8BB51 PK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) If the communication passes, the LEDs on both the Master
//    and Slave boards will blink slowly. If it fails, the LEDs will be off.
//
// Target:         EFM8BB51
// Tool chain:     Generic
//
// Release 0.1 (JS)
//    - Initial Revision
//    - 2021
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB51_Register_Enums.h>
#include "InitDevice.h"
#include "EFM8BB51_SPI_Master.h"
#include "stdbool.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (LED1, SFR_P1, 5);             // LED0
#define LED_ON   0
#define LED_OFF  1

SI_SBIT (DISP_EN, SFR_P2, 0);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

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

   DISP_EN = DISP_BC_DRIVEN;           // EFM8 does not drive display

   // Flush TX and RX fifos
   SFRPAGE = PG3_PAGE;
   SPI0FCN0 |= SPI0FCN0_TFLSH__BMASK | SPI0FCN0_RFLSH__BMASK;
   while ((SPI0FCN0 & (SPI0FCN0_TFLSH__BMASK | SPI0FCN0_RFLSH__BMASK)));
   SFRPAGE = LEGACY_PAGE;

   IE_EA = 1;                          // Enable global interrupts

   LED1 = LED_OFF;

   // Test: Write and read one byte -------------------------------------------

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

   // Test: Write and read 8 bytes --------------------------------------------

   // Copy test_array into SPI_Data_Array
   for(i = 0; i < MAX_BUFFER_SIZE; i++)
   {
      SPI_Data_Array[i] = test_array[i];
   }

   // Send the array to the slave
   SPI_Array_Write();

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

   // End of tests ------------------------------------------------------------

   while (1)
   {
      // If no error has occurred, blink the LEDs on the Master and Slave
      // boards
      if (Error_Flag == 0)
      {
         LED1 = LED_ON;

         SPI_LED_On();

         while(!SPI0CN0_NSSMD0);

         Delay();

         SPI_LED_Off();

         LED1 = LED_OFF;

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
// being sent using the SPI0CN0_TXNF flag.
//
//-----------------------------------------------------------------------------
void SPI_Array_Write(void)
{
   uint8_t array_index;
   uint8_t SFRPAGE_save = SFRPAGE;

   while(!SPI0CN0_NSSMD0);             // Wait until the SPI is free, in case
                                       // it's already busy

   SFRPAGE = PG3_PAGE;

   IE_ESPI0 = 0;                       // Disable SPI interrupts

   SPI0CN0_NSSMD0 = 0;                 // Enable the slave

   while(!SPI0CN0_TXNF);               // Wait until the FIFO isn't full
   SPI0DAT = SPI_WRITE_BUFFER;         // Load the XMIT register

   for(array_index = 0; array_index < MAX_BUFFER_SIZE; array_index++)
   {
      while(!SPI0CN0_TXNF);               // Wait until the FIFO isn't full
      SPI0DAT = SPI_Data_Array[array_index]; // Load the data into the buffer
   }

   // Wait until TX FIFO count is 0
   // (the FIFO is empty, but the shift register may contain the last
   // byte being transmitted)
   while ((SPI0FCT & SPI0FCT_TXCNT__FMASK) >> SPI0FCT_TXCNT__SHIFT);

   // Wait until the last byte in the shift register is transferred
   while (SPI0CFG & SPI0CFG_SPIBSY__SET);

   SPI0CN0_SPIF = 0;

   // Flush the data received during the write transfer
   SPI0FCN0 |= SPI0FCN0_RFLSH__BMASK;
   while (SPI0FCN0 & SPI0FCN0_RFLSH__BMASK);

   SPI0CN0_NSSMD0 = 1;                 // Disable the Slave

   IE_ESPI0 = 1;                       // Re-enable SPI interrupts

   SFRPAGE = SFRPAGE_save;
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
