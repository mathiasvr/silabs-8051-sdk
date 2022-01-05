//-----------------------------------------------------------------------------
// F93x_SPI0_EEPROM_Polled_Mode.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program accesses a SPI EEPROM using polled mode access. The 'F336 MCU
// is configured in 4-wire Single Master Mode, and the EEPROM is the only
// slave device connected to the SPI bus. The read/write operations are
// tailored to access a Microchip 4 kB EEPROM 25LC320. The relevant hardware
// connections of the 'F930 MCU are shown here:
//
// P0.0 - SPI SCK    (digital output, push-pull)
// P0.1 - SPI MISO   (digital input, open-drain)
// P0.2 - SPI MOSI   (digital output, push-pull)
// P0.3 - SPI NSS    (digital output, push-pull)
// P0.4 - UART TXD   (digital output, push-pull)
// P0.5 - UART RXD   (digital input, open-drain)
// P1.6 - LED        (digital output, push-pull)
//
//
// How To Test:
//
// Method1:
// 1) Ensure that jumpers are placed on the following:
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the device to a Microchip 25LC320 EEPROM.
// 3) Connect the USB Debug Adapter to J9.
// 4) Turn the POWER switch (SW5) to the "ON" position.
// 5) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) The LED will blink fast during the write/read/verify operations.If the
//    verification passes, the LED will blink slowly. If it fails, the LED will
//    be OFF.
//
// Method2 (optional):
// 1) Ensure that jumpers are placed on the following:
//       J12:  P0.4/TX
//             P0.5/RX
//             VDD/VIO
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect a USB cable from the development board P3 to a PC
// 3) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the USB port (virtual com port) at 230400 and 8-N-1
// 4) Connect the USB Debug Adapter to J9.
// 5) Turn the POWER switch (SW5) to the "ON" position.
// 6) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) HyperTerminal will print the progress of the write/read operation, and in
//    the end will print the test result as pass or fail. Additionally, if the
//    verification passes, the LED will blink slowly. If it fails, the LED will
//    be OFF.
//
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 15 JAN 2014
//
// Release 1.2
//    - Updated to be compatible with Raisonance Toolchain (FB)
//    - 14 APR 2010
//
// Release 1.1
//    - Compiled and tested for C8051F930-TB (JH)
//    - 07 JULY 2009
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 NOV 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F930_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include <stdio.h>                     // printf is declared here

//-----------------------------------------------------------------------------
// User-defined types, structures, unions etc
//-----------------------------------------------------------------------------
#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef UINT
#define UINT unsigned int
#endif

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK             24500000    // Internal oscillator frequency in Hz

// Microchip 25AA320 Slave EEPROM Parameters
#define  F_SCK_MAX         2000000     // Max SCK freq (Hz)
#define  T_NSS_DISABLE_MIN 500         // Min NSS disable time (ns)
#define  EEPROM_CAPACITY   4096        // EEPROM capacity (bytes)

// EEPROM Instruction Set
#define  EEPROM_CMD_READ   0x03        // Read Command
#define  EEPROM_CMD_WRITE  0x02        // Write Command
#define  EEPROM_CMD_WRDI   0x04        // Reset Write Enable Latch Command
#define  EEPROM_CMD_WREN   0x06        // Set Write Enable Latch Command
#define  EEPROM_CMD_RDSR   0x05        // Read Status Register Command
#define  EEPROM_CMD_WRSR   0x01        // Write Status Register Command

#define  LED_ON             0
#define  LED_OFF            1

SI_SBIT(LED, SFR_P1, 6);                 // LED==LED_ON means ON

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Delay_us (BYTE time_us);
void Delay_ms (BYTE time_ms);
void EEPROM_Write (UINT address, BYTE value);
BYTE EEPROM_Read (UINT address);

//-----------------------------------------------------------------------------
// Generic UART definitions to support both Keil and SDCC
//-----------------------------------------------------------------------------

#ifdef SDCC
#include <sdcc_stdio.h>
#endif

char *GETS (char *, uint16_t);

#ifdef __C51__
#define GETS gets
#endif

#ifdef SDCC
#include <sdcc_stdio.c>
#endif

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
// ----------------------------------------------------------------------------
int main (void)
{
	UINT  address;                      // EEPROM address
	BYTE  test_byte;                    // Used as a temporary variable

	//Enter default mode
	enter_DefaultMode_from_RESET();
	SCON0_TI = 1;                            // Indicate TX0 ready


	// The following code will test the EEPROM by performing write/read/verify
	// operations. The first test will write 0xFFs to the EEPROM, and the
	// second test will write the LSBs of the EEPROM addresses.

	// Fill EEPROM with 0xFF's
	LED = LED_ON;
	printf("Filling with 0xFF's...\n");
	for (address = 0; address < EEPROM_CAPACITY; address++)
	{
	  test_byte = 0xFF;
	  EEPROM_Write (address, test_byte);

	  // Print status to UART0
	  if ((address % 16) == 0)
	  {
		 printf ("\nWriting 0x%04x: %02x ", address, (UINT)test_byte);
		 LED = !LED;
	  }
	  else
		 printf ("%02x ", (UINT)test_byte);
	}

	// Verify EEPROM with 0xFF's
	printf("\n\nVerifying 0xFF's...\n");
	for (address = 0; address < EEPROM_CAPACITY; address++)
	{
	  test_byte = EEPROM_Read (address);

	  // Print status to UART0
	  if ((address % 16) == 0)
	  {
		 printf ("\nVerifying 0x%04x: %02x ", address, (UINT)test_byte);
		 LED = !LED;
	  }
	  else
		 printf ("%02x ", (UINT)test_byte);
	  if (test_byte != 0xFF)
	  {
		 LED = LED_OFF;
		 printf ("Error at %u\n", address);
		 while (1);                    // Stop here on error (for debugging)
	  }
	}

	// Fill EEPROM with LSB of EEPROM addresses
	printf("\n\nFilling with LSB of EEPROM addresses...\n");
	for (address = 0; address < EEPROM_CAPACITY; address++)
	{
	  test_byte = address & 0xFF;
	  EEPROM_Write (address, test_byte);

	  // Print status to UART0
	  if ((address % 16) == 0)
	  {
		 printf ("\nWriting 0x%04x: %02x ", address, (UINT)test_byte);
		 LED = !LED;
	  }
	  else
		 printf ("%02x ", (UINT)test_byte);
	}

	// Verify EEPROM with LSB of EEPROM addresses
	printf("\n\nVerifying LSB of EEPROM addresses...\n");
	for (address = 0; address < EEPROM_CAPACITY; address++)
	{
	  test_byte = EEPROM_Read (address);

	  // print status to UART0
	  if ((address % 16) == 0)
	  {
		 printf ("\nVerifying 0x%04x: %02x ", address, (UINT)test_byte);
		 LED = !LED;
	  }
	  else
		 printf ("%02x ", (UINT)test_byte);

	  if (test_byte != (address & 0xFF))
	  {
		 LED = LED_OFF;
		 printf ("Error at %u\n", address);
		 while (1);                    // Stop here on error (for debugging)
	  }
	}

	printf ("\n\nVerification success!\n");

	while (1)                           // Loop forever
	{
	  LED = !LED;                      // Flash LED when done (all verified)
	  Delay_ms (200);
	}

	// NOTREACHED
	return 0;
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Delay_us
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : 1. time_us - time delay in microseconds
//                   range: 1 to 255
//
// Creates a delay for the specified time (in microseconds) using TIMER2. The
// time tolerance is approximately +/-50 ns (1/SYSCLK + function call time).
//
//-----------------------------------------------------------------------------
void Delay_us (BYTE time_us)
{
   TMR2CN_TR2   = 0;                          // Stop timer
   TMR2CN_TF2H  = 0;                          // Clear timer overflow flag
   TMR2  = -( (UINT)(SYSCLK/1000000) * (UINT)(time_us) );
   TMR2CN_TR2   = 1;                          // Start timer
   while (!TMR2CN_TF2H);                      // Wait till timer overflow occurs
   TMR2CN_TR2   = 0;                          // Stop timer
}

//-----------------------------------------------------------------------------
// Delay_ms
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : 1. time_ms - time delay in milliseconds
//                   range: 1 to 255
//
// Creates a delay for the specified time (in milliseconds) using TIMER2. The
// time tolerance is approximately +/-50 ns (1/SYSCLK + function call time).
//
//-----------------------------------------------------------------------------
void Delay_ms (BYTE time_ms)
{
   BYTE i;

   while(time_ms--)
      for(i = 0; i< 10; i++)           // 10 * 100 microsecond delay
         Delay_us (100);
}

//-----------------------------------------------------------------------------
// EEPROM_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : 1. address - the destination EEPROM address.
//                   range: 0 to EEPROM_CAPACITY
//                2. value - the value to write.
//                   range: 0x00 to 0xFF
//
// Writes one byte to the specified address in the EEPROM. This function polls
// the EEPROM status register after the write operation, and returns only after
// the status register indicates that the write cycle is complete. This is to
// prevent from having to check the status register before a read operation.
//
//-----------------------------------------------------------------------------
void EEPROM_Write (UINT address, BYTE value)
{
   // Writing a byte to the EEPROM is a five-step operation.

   // Step1: Set the Write Enable Latch to 1
   SPI0CN_NSSMD0   = 0;                       // Step1.1: Activate Slave Select
   SPI0DAT  = EEPROM_CMD_WREN;         // Step1.2: Send the WREN command
   while (!SPI0CN_SPIF);                      // Step1.3: Wait for end of transfer
   SPI0CN_SPIF     = 0;                       // Step1.4: Clear the SPI intr. flag
   SPI0CN_NSSMD0   = 1;                       // Step1.5: Deactivate Slave Select
   Delay_us (1);                       // Step1.6: Wait for at least
                                       //          T_NSS_DISABLE_MIN
   // Step2: Send the WRITE command
   SPI0CN_NSSMD0   = 0;
   SPI0DAT  = EEPROM_CMD_WRITE;
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF     = 0;

   // Step3: Send the EEPROM destination address (MSB first)
   SPI0DAT  = (BYTE)((address >> 8) & 0x00FF);
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF     = 0;
   SPI0DAT  = (BYTE)(address & 0x00FF);
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF     = 0;

   // Step4: Send the value to write
   SPI0DAT  = value;
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF     = 0;
   SPI0CN_NSSMD0   = 1;
   Delay_us (1);

   // Step5: Poll on the Write In Progress (WIP) bit in Read Status Register
   do
   {
      SPI0CN_NSSMD0   = 0;                    // Activate Slave Select
      SPI0DAT  = EEPROM_CMD_RDSR;      // Send the Read Status Register command
      while (!SPI0CN_SPIF);                   // Wait for the command to be sent out
      SPI0CN_SPIF     = 0;
      SPI0DAT  = 0;                    // Dummy write to output serial clock
      while (!SPI0CN_SPIF);                   // Wait for the register to be read
      SPI0CN_SPIF     = 0;
      SPI0CN_NSSMD0   = 1;                    // Deactivate Slave Select after read
      Delay_us (1);
   } while( (SPI0DAT & 0x01) == 0x01 );
}

//-----------------------------------------------------------------------------
// EEPROM_Read
//-----------------------------------------------------------------------------
//
// Return Value : The value that was read from the EEPROM
//                   range: 0x00 to 0xFF
// Parameters   : 1. address - the source EEPROM address.
//                   range: 0 to EEPROM_CAPACITY
//
// Reads one byte from the specified EEPROM address.
//
//-----------------------------------------------------------------------------
BYTE EEPROM_Read (UINT address)
{
   // Reading a byte from the EEPROM is a three-step operation.

   // Step1: Send the READ command
   SPI0CN_NSSMD0   = 0;                       // Activate Slave Select
   SPI0DAT  = EEPROM_CMD_READ;
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF     = 0;

   // Step2: Send the EEPROM source address (MSB first)
   SPI0DAT  = (BYTE)((address >> 8) & 0x00FF);
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF     = 0;
   SPI0DAT  = (BYTE)(address & 0x00FF);
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF     = 0;

   // Step3: Read the value returned
   SPI0DAT  = 0;                       // Dummy write to output serial clock
   while (!SPI0CN_SPIF);                      // Wait for the value to be read
   SPI0CN_SPIF     = 0;
   SPI0CN_NSSMD0   = 1;                       // Deactivate Slave Select
   Delay_us (1);

   return SPI0DAT;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
