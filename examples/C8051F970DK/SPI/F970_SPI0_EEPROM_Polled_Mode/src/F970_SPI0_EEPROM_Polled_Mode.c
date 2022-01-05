//-----------------------------------------------------------------------------
// F970_SPI0_EEPROM_Polled_Mode.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program accesses a SPI EEPROM using polled mode access. The 'F970 MCU
// is configured in 4-wire Single Master Mode, and the EEPROM is the only
// slave device connected to the SPI bus. The read/write operations are
// tailored to access a Microchip 4 kB EEPROM 25LC320. The relevant hardware
// connections of the 'F970 MCU are shown here:
//
// P0.1 - UART TX    (digital output, push-pull)
// P0.2 - UART RX    (digital input,  open-drain)
// P0.5 - LED        (digital output, push-pull)
// P2.0 - SPI SCK    (digital output, push-pull)
// P2.1 - SPI MISO   (digital input,  open-drain)
// P2.2 - SPI MOSI   (digital output, push-pull)
// P2.3 - SPI NSS    (digital output, push-pull)
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP1: VCP RX <> P0.1/UART0.TX
//    - JP2: VCP TX <> P0.2/UART0.RX
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Connect SPI bus to a Microchip 4 kB EEPROM 25LC320 SPI EEPROM.
// 4) Connect a USB cable from J9 of the development board to a PC,
// 5) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the virtual COM port at <BAUDRATE> (115200) and 8-N-1.
// 6) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 7) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 8) HyperTerminal will print the progress of the write/read operation, and in
//    the end will print the test result as pass or fail. Additionally, if the
//    verification passes, the LED will blink during the write/read/verify
//    operations. If it fails, the LED will be OFF.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2/JL)
//    - 15 MAY 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK             24500000    // Internal oscillator frequency in Hz
#define BAUDRATE           115200      // Baud rate of UART in bps

// Microchip 25LC320 Slave EEPROM Parameters
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

SI_SBIT(LED, P0, 5);                     // LED==0 means ON

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void Delay_us (uint8_t time_us);
void Delay_ms (uint8_t time_ms);
void EEPROM_Write (uint16_t address, uint8_t value);
uint8_t EEPROM_Read (uint16_t address);

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
  SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
  PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   uint16_t address;                        // EEPROM address
   uint8_t test_byte;                       // Used as a temporary variable

   enter_DefaultMode_from_RESET();

   SCON0_TI = 1; // enable printf

   // The following code will test the EEPROM by performing write/read/verify
   // operations. The first test will write 0xFFs to the EEPROM, and the
   // second test will write the LSBs of the EEPROM addresses.

   SFRPAGE = UART0_PAGE;               // Set for printf()

   // Fill EEPROM with 0xFF's
   LED = 1;

   // Fill EEPROM with 0xFF's
   LED = 0;
   printf("Filling with 0xFF's...\n");
   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = 0xFF;
      EEPROM_Write (address, test_byte);

      // Print status to UART0
      if ((address % 16) == 0)
      {
         printf ("\nWriting 0x%04X: %02X ", address, (uint16_t)test_byte);
         LED = !LED;
      }
      else
      {
         printf ("%02X ", (uint16_t)test_byte);
      }
   }

   // Verify EEPROM with 0xFF's
   printf("\n\nVerifying 0xFF's...\n");
   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = EEPROM_Read (address);

      // Print status to UART0
      if ((address % 16) == 0)
      {
         printf ("\nVerifying 0x%04X: %02X ", address, (uint16_t)test_byte);
         LED = !LED;
      }
      else
      {
         printf ("%02X ", (uint16_t)test_byte);
      }

      if (test_byte != 0xFF)
      {
         LED = 0;
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
         printf ("\nWriting 0x%04X: %02X ", address, (uint16_t)test_byte);
         LED = !LED;
      }
      else
      {
         printf ("%02X ", (uint16_t)test_byte);
      }
   }

   // Verify EEPROM with LSB of EEPROM addresses
   printf("\n\nVerifying LSB of EEPROM addresses...\n");
   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = EEPROM_Read (address);

      // print status to UART0
      if ((address % 16) == 0)
      {
         printf ("\nVerifying 0x%04X: %02X ", address, (uint16_t)test_byte);
         LED = !LED;
      }
      else
      {
         printf ("%02X ", (uint16_t)test_byte);
      }

      if (test_byte != (address & 0xFF))
      {
         LED = 0;
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
void Delay_us (uint8_t time_us)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = TIMER2_PAGE;

   TMR2CN_TR2   = 0;                          // Stop timer
   TMR2CN_TF2H  = 0;                          // Clear timer overflow flag
   TMR2  = -((uint16_t)(SYSCLK / 1000000) * (uint16_t)(time_us));
   TMR2CN_TR2   = 1;                          // Start timer
   while (!TMR2CN_TF2H);                      // Wait till timer overflow occurs
   TMR2CN_TR2   = 0;                          // Stop timer

   SFRPAGE = SFRPAGE_save;
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
void Delay_ms (uint8_t time_ms)
{
   uint8_t i;

   while(time_ms--)
   {
      for(i = 0; i< 10; i++)           // 10 * 100 microsecond delay
      {
         Delay_us (100);
      }
   }
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
void EEPROM_Write (uint16_t address, uint8_t value)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = SPI0_PAGE;

   // Writing a byte to the EEPROM is a five-step operation.

   // Step1: Set the Write Enable Latch to 1
   SPI0CN_NSSMD0   = 0;                // Step1.1: Activate Slave Select
   SPI0DAT  = EEPROM_CMD_WREN;         // Step1.2: Send the WREN command
   while (!SPI0CN_SPIF);               // Step1.3: Wait for end of transfer
   SPI0CN_SPIF     = 0;                // Step1.4: Clear the SPI intr. flag
   SPI0CN_NSSMD0   = 1;                // Step1.5: Deactivate Slave Select
   Delay_us (1);                       // Step1.6: Wait for at least
                                       //          T_NSS_DISABLE_MIN
   // Step2: Send the WRITE command
   SPI0CN_NSSMD0   = 0;
   SPI0DAT  = EEPROM_CMD_WRITE;
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF     = 0;

   // Step3: Send the EEPROM destination address (MSB first)
   SPI0DAT  = (uint8_t)((address >> 8) & 0x00FF);
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF     = 0;
   SPI0DAT  = (uint8_t)(address & 0x00FF);
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
      SPI0CN_NSSMD0   = 0;             // Activate Slave Select
      SPI0DAT  = EEPROM_CMD_RDSR;      // Send the Read Status Register command
      while (!SPI0CN_SPIF);            // Wait for the command to be sent out
      SPI0CN_SPIF     = 0;
      SPI0DAT  = 0;                    // Dummy write to output serial clock
      while (!SPI0CN_SPIF);            // Wait for the register to be read
      SPI0CN_SPIF     = 0;
      SPI0CN_NSSMD0   = 1;             // Deactivate Slave Select after read
      Delay_us (1);
   } while((SPI0DAT & 0x01) == 0x01);

   SFRPAGE = SFRPAGE_save;
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
uint8_t EEPROM_Read (uint16_t address)
{
   uint8_t spi_data;

   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = SPI0_PAGE;

   // Reading a byte from the EEPROM is a three-step operation.

   // Step1: Send the READ command
   SPI0CN_NSSMD0   = 0;                // Activate Slave Select
   SPI0DAT  = EEPROM_CMD_READ;
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF     = 0;

   // Step2: Send the EEPROM source address (MSB first)
   SPI0DAT  = (uint8_t)((address >> 8) & 0x00FF);
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF     = 0;
   SPI0DAT  = (uint8_t)(address & 0x00FF);
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF     = 0;

   // Step3: Read the value returned
   SPI0DAT  = 0;                       // Dummy write to output serial clock
   while (!SPI0CN_SPIF);               // Wait for the value to be read
   SPI0CN_SPIF     = 0;
   SPI0CN_NSSMD0   = 1;                // Deactivate Slave Select
   Delay_us (1);

   spi_data = SPI0DAT;                 // Read data before restoring SFR page

   SFRPAGE = SFRPAGE_save;

   return spi_data;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
