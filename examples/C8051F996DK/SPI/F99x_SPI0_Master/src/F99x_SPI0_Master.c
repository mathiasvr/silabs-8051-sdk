//-----------------------------------------------------------------------------
// F99x_SPI0_Master.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program configures a C8051F99x as a 4-wire SPI Single Master.
//
// The SPI clock in this example is limited to 500 kHz when used with the
// SPI0_Slave code example.  During a SPI_Read, the slave needs some time to
// interpret the command and write the appropriate data to the SPI0DAT
// register, and the slave no longer has enough time to complete the
// SPI_READ_BUFFER command with a clock greater than 500 kHz.  For faster SPI
// clocks, a dummy byte between the command and the first byte of Read data
// will be required.
//
// This example is intended to be used with the SPI0_Slave example.
//
// Pinout:
//
// P0.0 - SPI SCK    (digital output, push-pull)
// P0.1 - SPI MISO   (digital input, open-drain)
// P0.2 - SPI MOSI   (digital output, push-pull)
// P0.3 - SPI NSS    (digital output, push-pull)
//
// P1.3 - LED        (digital output, push-pull)
//
// all other port pins unused.
//
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
//	  Ensure that these jumpers are not connected:
//	     J14:  P0.1/GND
// 2) Connect the device to another 'Fxxx device running SPI0 - Slave code.
// 3) Connect the USB Debug Adapter to J4.
// 4) Turn the POWER switch (SW5) to the "ON" position.
// 5) Compile and download code to a 'F99x device on a C8051F99x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) If the communication passes, the LEDs on both the Master and Slave
//    boards will blink slowly. If it fails, the LEDs will be OFF.
//
//
// Target:         C8051F99x-C8051F98x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Lowered SPI frequency to 150kHz from 500kHz
//    - 13 JAN 2014
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F990_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"

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

SI_SBIT(LED, SFR_P1, 3);                 // LED==LED_ON means ON

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint8_t SPI_Data = 0xA5;

uint8_t SPI_Data_Array[MAX_BUFFER_SIZE] = {0};

bit Error_Flag = 0;

uint8_t Command = 0x00;

SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);


//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SPI_LED_On (void);
void SPI_LED_Off (void);
void SPI_Byte_Write (void);
void SPI_Byte_Read (void);
void SPI_Array_Write (void);
void SPI_Array_Read (void);

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
 
void Delay(void);
//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
	uint8_t test_value = 0x55;
	uint8_t test_array[MAX_BUFFER_SIZE] = {1,2,3,4,5,6,7,8};
	uint8_t i;

	//Enter default mode
	enter_DefaultMode_from_RESET();

	LED = LED_OFF;

	// TEST BEGIN --------------------------------------------------------------

	SPI_Data = test_value;

	// Write a value
	SPI_Byte_Write ();

	while (!SPI0CN_NSSMD0);                    // Wait until the Write transfer has
									   // finished

	// Read the same value back
	SPI_Data = 0x00;
	SPI_Byte_Read ();

	while (!SPI0CN_NSSMD0);                    // Wait until the Read transfer has
									   // finished

	// Check if the sent value and returned value match
	if (SPI_Data != test_value)
	{
	  Error_Flag = 1;
	}

	// Copy test_array into SPI_Data_Array
	for (i = 0; i < MAX_BUFFER_SIZE; i++)
	{
	  SPI_Data_Array[i] = test_array[i];
	}

	// Send the array to the slave
	SPI_Array_Write ();

	while (!SPI0CN_NSSMD0);                    // Wait until the Write transfer has
									   // finished

	// Clear SPI_Data_Array for the SPI_Buffer_Read function
	for (i = 0; i < MAX_BUFFER_SIZE; i++)
	{
	  SPI_Data_Array[i] = 0;
	}

	// Read the array back from the slave
	SPI_Array_Read ();

	while (!SPI0CN_NSSMD0);                    // Wait until the Read transfer has
									   // finished

	// Check if the received array matches the sent array
	for (i = 0; i < MAX_BUFFER_SIZE; i++)
	{
	  if (SPI_Data_Array[i] != test_array[i])
	  {
		 Error_Flag = 1;
	  }
	}

	// END OF TEST -------------------------------------------------------------

	while (1)
	{
	  // If no error has occurred, blink the LEDs on the Master and Slave
	  // boards
	  if (Error_Flag == 0)
	  {
		 LED = LED_ON;

		 SPI_LED_On ();

		 while (!SPI0CN_NSSMD0);

		 Delay ();

		 SPI_LED_Off ();

		 LED = LED_OFF;

		 while (!SPI0CN_NSSMD0);

		 Delay ();
	  }
	};

	// NOTREACHED
	return 0;
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
void SPI_LED_On (void)
{
   while (!SPI0CN_NSSMD0);                    // Wait until the SPI is free, in case
                                       // it's already busy

   SPI0CN_NSSMD0 = 0;

   Command = SLAVE_LED_ON;

   SPI0DAT = Command;

   // The rest of this command will be handled by the SPI ISR, which will
   // trigger when SPI0CN_SPIF is set from sending the Command
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
void SPI_LED_Off (void)
{
   while (!SPI0CN_NSSMD0);                    // Wait until the SPI is free, in case
                                       // it's already busy

   SPI0CN_NSSMD0 = 0;

   Command = SLAVE_LED_OFF;

   SPI0DAT = Command;

   // The rest of this command will be handled by the SPI ISR, which will
   // trigger when SPI0CN_SPIF is set from sending the Command
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
void SPI_Byte_Write (void)
{
   while (!SPI0CN_NSSMD0);                    // Wait until the SPI is free, in case
                                       // it's already busy

   SPI0CN_NSSMD0 = 0;

   Command = SPI_WRITE;

   SPI0DAT = Command;

   // The rest of this command will be handled by the SPI ISR, which will
   // trigger when SPI0CN_SPIF is set from sending the Command
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
void SPI_Byte_Read (void)
{
   while (!SPI0CN_NSSMD0);                    // Wait until the SPI is free, in case
                                       // it's already busy

   SPI0CN_NSSMD0 = 0;

   Command = SPI_READ;

   SPI0DAT = Command;

   // The rest of this command will be handled by the SPI ISR, which will
   // trigger when SPI0CN_SPIF is set from sending the Command
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
// being sent using the SPI0CN_TXBMT flag.
//
//-----------------------------------------------------------------------------
void SPI_Array_Write (void)
{
   uint8_t array_index;

   while (!SPI0CN_NSSMD0);                    // Wait until the SPI is free, in case
                                       // it's already busy

   IE_ESPI0 = 0;                          // Disable SPI interrupts

   SPI0CN_NSSMD0 = 0;

   SPI0DAT = SPI_WRITE_BUFFER;         // Load the XMIT register
   while (SPI0CN_TXBMT != 1)                  // Wait until the command is moved into
   {                                   // the XMIT buffer
   }

   for (array_index = 0; array_index < MAX_BUFFER_SIZE; array_index++)
   {
      SPI0DAT = SPI_Data_Array[array_index]; // Load the data into the buffer
      while (SPI0CN_TXBMT != 1)               // Wait until the data is moved into
      {                                // the XMIT buffer
      }
   }
   SPI0CN_SPIF = 0;
   while (SPI0CN_SPIF != 1)                   // Wait until the last byte of the
   {                                   // data reaches the Slave
   }
   SPI0CN_SPIF = 0;

   SPI0CN_NSSMD0 = 1;                         // Diable the Slave

   IE_ESPI0 = 1;                          // Re-enable SPI interrupts
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
void SPI_Array_Read (void)
{
   while (!SPI0CN_NSSMD0);                    // Wait until the SPI is free, in case
                                       // it's already busy

   SPI0CN_NSSMD0 = 0;

   Command = SPI_READ_BUFFER;

   SPI0DAT = Command;

   // The rest of this command will be handled by the SPI ISR, which will
   // trigger when SPI0CN_SPIF is set from sending the Command
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
void Delay (void)
{
   uint32_t count;

   for (count = 200000; count > 0; count--);
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
