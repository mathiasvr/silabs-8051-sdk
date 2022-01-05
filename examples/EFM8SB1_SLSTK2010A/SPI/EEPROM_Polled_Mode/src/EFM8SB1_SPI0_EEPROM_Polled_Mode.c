//-----------------------------------------------------------------------------
// SB1_SPI0_EEPROM_Polled_Mode.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program accesses a SPI EEPROM using polled mode access. The MCU
// is configured in 4-wire Single Master Mode, and the EEPROM is the only
// slave device connected to the SPI bus. The read/write operations are
// tailored to access a Microchip 4 kB EEPROM 25LC320. The relevant hardware
// connections of the MCU are shown here:
//
// P0.6 - SPI SCK        (digital output, push-pull)
// P0.7 - SPI MISO       (digital input, open-drain)
// P1.0 - SPI MOSI       (digital output, push-pull)
// P1.1 - SPI NSS / LED0 (digital output, push-pull)
// P0.4 - UART TXD       (digital output, push-pull)
// P0.5 - UART RXD       (digital input, open-drain)
// P1.1 - LED            (digital output, push-pull)
//
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect the device to a Microchip 25LC320 EEPROM.
// 3) Connect the EFM8SB1 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the JLink CDC UART Port at 115200 baud rate and 8-N-1.
// 6) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 7) The terminal program will print the progress of the write/read operation, and in
//    the end will print the test result as pass or fail. Additionally, if the
//    verification passes, LED0 will blink slowly. If it fails, LED0 will
//    be OFF.
//
//
// Target:         EFM8SB1
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 9 JAN 2015
//
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <si_toolchain.h>
#include <SI_EFM8SB1_Register_Enums.h>                  // SI_SFR declarations
#include "InitDevice.h"
#include "retargetserial.h"

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
#define BAUDRATE           115200      // Baud rate of UART in bps
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

#define  LED_ON             1
#define  LED_OFF            0

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 7);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (LED0, SFR_P1, 1);                 // LED==LED_ON means ON

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT (reserved, uint8_t, SI_SEG_XDATA, 0x0000);

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

	DISP_EN = DISP_BC_DRIVEN;

	SCON0_TI = 1;                            // Indicate TX0 ready

	// The following code will test the EEPROM by performing write/read/verify
	// operations. The first test will write 0xFFs to the EEPROM, and the
	// second test will write the LSBs of the EEPROM addresses.

	// Fill EEPROM with 0xFF's
	RETARGET_PRINTF("Filling with 0xFF's...\n");
	for (address = 0; address < EEPROM_CAPACITY; address++)
	{
	  test_byte = 0xFF;
	  EEPROM_Write (address, test_byte);

	  // Print status to UART0
	  if ((address % 16) == 0)
	  {
		 RETARGET_PRINTF ("\nWriting 0x%04x: %02x ", address, (UINT)test_byte);
	  }
	  else
		 RETARGET_PRINTF ("%02x ", (UINT)test_byte);
	}

	// Verify EEPROM with 0xFF's
	RETARGET_PRINTF("\n\nVerifying 0xFF's...\n");
	for (address = 0; address < EEPROM_CAPACITY; address++)
	{
	  test_byte = EEPROM_Read (address);

	  // Print status to UART0
	  if ((address % 16) == 0)
	  {
		 RETARGET_PRINTF ("\nVerifying 0x%04x: %02x ", address, (UINT)test_byte);
	  }
	  else
		 RETARGET_PRINTF ("%02x ", (UINT)test_byte);
	  if (test_byte != 0xFF)
	  {
		 RETARGET_PRINTF ("Error at %u\n", address);
		 while (1);                    // Stop here on error (for debugging)
	  }
	}

	// Fill EEPROM with LSB of EEPROM addresses
	RETARGET_PRINTF("\n\nFilling with LSB of EEPROM addresses...\n");
	for (address = 0; address < EEPROM_CAPACITY; address++)
	{
	  test_byte = address & 0xFF;
	  EEPROM_Write (address, test_byte);

	  // Print status to UART0
	  if ((address % 16) == 0)
	  {
		 RETARGET_PRINTF ("\nWriting 0x%04x: %02x ", address, (UINT)test_byte);
	  }
	  else
		 RETARGET_PRINTF ("%02x ", (UINT)test_byte);
	}

	// Verify EEPROM with LSB of EEPROM addresses
	RETARGET_PRINTF("\n\nVerifying LSB of EEPROM addresses...\n");
	for (address = 0; address < EEPROM_CAPACITY; address++)
	{
	  test_byte = EEPROM_Read (address);

	  // print status to UART0
	  if ((address % 16) == 0)
	  {
		 RETARGET_PRINTF ("\nVerifying 0x%04x: %02x ", address, (UINT)test_byte);
	  }
	  else
		 RETARGET_PRINTF ("%02x ", (UINT)test_byte);

	  if (test_byte != (address & 0xFF))
	  {
		 RETARGET_PRINTF ("Error at %u\n", address);
		 while (1);                    // Stop here on error (for debugging)
	  }
	}

	RETARGET_PRINTF ("\n\nVerification success!\n");

	XBR0 &= ~XBR0_SPI0E__BMASK;         // Remove SPI0 from the crossbar to allow
	                                    // the usage of the LED

	while (1)                           // Loop forever
	{
	  LED0 = !LED0;                      // Flash LED when done (all verified)
	  Delay_ms (200);
	}
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
   TMR2CN0_TR2   = 0;                          // Stop timer
   TMR2CN0_TF2H  = 0;                          // Clear timer overflow flag
   TMR2  = -( (UINT)(SYSCLK/1000000) * (UINT)(time_us) );
   TMR2CN0_TR2   = 1;                          // Start timer
   while (!TMR2CN0_TF2H);                      // Wait till timer overflow occurs
   TMR2CN0_TR2   = 0;                          // Stop timer
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
   SPI0CN0_NSSMD0   = 0;                       // Step1.1: Activate Slave Select
   SPI0DAT  = EEPROM_CMD_WREN;         // Step1.2: Send the WREN command
   while (!SPI0CN0_SPIF);                      // Step1.1: Wait for end of transfer
   SPI0CN0_SPIF     = 0;                       // Step1.4: Clear the SPI intr. flag
   SPI0CN0_NSSMD0   = 1;                       // Step1.5: Deactivate Slave Select
   Delay_us (1);                       // Step1.6: Wait for at least
                                       //          T_NSS_DISABLE_MIN
   // Step2: Send the WRITE command
   SPI0CN0_NSSMD0   = 0;
   SPI0DAT  = EEPROM_CMD_WRITE;
   while (!SPI0CN0_SPIF);
   SPI0CN0_SPIF     = 0;

   // Step3: Send the EEPROM destination address (MSB first)
   SPI0DAT  = (BYTE)((address >> 8) & 0x00FF);
   while (!SPI0CN0_SPIF);
   SPI0CN0_SPIF     = 0;
   SPI0DAT  = (BYTE)(address & 0x00FF);
   while (!SPI0CN0_SPIF);
   SPI0CN0_SPIF     = 0;

   // Step4: Send the value to write
   SPI0DAT  = value;
   while (!SPI0CN0_SPIF);
   SPI0CN0_SPIF     = 0;
   SPI0CN0_NSSMD0   = 1;
   Delay_us (1);

   // Step5: Poll on the Write In Progress (WIP) bit in Read Status Register
   do
   {
      SPI0CN0_NSSMD0   = 0;                    // Activate Slave Select
      SPI0DAT  = EEPROM_CMD_RDSR;      // Send the Read Status Register command
      while (!SPI0CN0_SPIF);                   // Wait for the command to be sent out
      SPI0CN0_SPIF     = 0;
      SPI0DAT  = 0;                    // Dummy write to output serial clock
      while (!SPI0CN0_SPIF);                   // Wait for the register to be read
      SPI0CN0_SPIF     = 0;
      SPI0CN0_NSSMD0   = 1;                    // Deactivate Slave Select after read
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
   SPI0CN0_NSSMD0   = 0;                       // Activate Slave Select
   SPI0DAT  = EEPROM_CMD_READ;
   while (!SPI0CN0_SPIF);
   SPI0CN0_SPIF     = 0;

   // Step2: Send the EEPROM source address (MSB first)
   SPI0DAT  = (BYTE)((address >> 8) & 0x00FF);
   while (!SPI0CN0_SPIF);
   SPI0CN0_SPIF     = 0;
   SPI0DAT  = (BYTE)(address & 0x00FF);
   while (!SPI0CN0_SPIF);
   SPI0CN0_SPIF     = 0;

   // Step3: Read the value returned
   SPI0DAT  = 0;                       // Dummy write to output serial clock
   while (!SPI0CN0_SPIF);                      // Wait for the value to be read
   SPI0CN0_SPIF     = 0;
   SPI0CN0_NSSMD0   = 1;                       // Deactivate Slave Select
   Delay_us (1);

   return SPI0DAT;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
