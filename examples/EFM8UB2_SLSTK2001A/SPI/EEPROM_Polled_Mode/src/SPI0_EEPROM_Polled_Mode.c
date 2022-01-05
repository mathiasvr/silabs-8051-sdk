//-----------------------------------------------------------------------------
// SPI0_EEPROM_Polled_Mode.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program accesses a SPI EEPROM using polled mode access. The EFM8UB2 MCU
// is configured in 4-wire Single Master Mode, and the EEPROM is the only
// slave device connected to the SPI bus. The read/write operations are
// tailored to access a Microchip 4 kB EEPROM 25LC320. The relevant hardware
// connections of the EFM8UB2 MCU are shown here:
//
// P0.6 - SPI SCK    (digital output, push-pull)
// P0.7 - SPI MISO   (digital input,  open-drain)
// P1.0 - SPI MOSI   (digital output, push-pull)
// P1.1 - SPI NSS    (digital output, push-pull)
// P0.4 - UART TXD   (digital output, push-pull)
// P0.5 - UART RXD   (digital input,  open-drain)
// P1.6 - LED        (digital output, push-pull)
//
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect the device to a Microchip 25LC320 EEPROM.
// 3) Connect the EFM8UB2 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8UB2 STK board.
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
// Target:         EFM8UB2
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 14 JAN 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8UB2_Register_Enums.h>
#include "retargetserial.h"                     // printf is declared here
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define BAUDRATE           115200      // Baud rate of UART in bps
#define SYSCLK             24000000    // Internal oscillator frequency in Hz

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

SI_SBIT (LED1, SFR_P1, 6);                // LED='0' means ON

#define LED_ON             0
#define LED_OFF            1
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
  // Disable the watchdog here
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   uint16_t address;                        // EEPROM address
   uint8_t test_byte;                       // Used as a temporary variable

   enter_DefaultMode_from_RESET();
   SCON0_TI = 1;

   // The following code will test the EEPROM by performing write/read/verify
   // operations. The first test will write 0xFFs to the EEPROM, and the
   // second test will write the LSBs of the EEPROM addresses.

   // Fill EEPROM with 0xFF's
   LED1 = LED_ON;
   RETARGET_PRINTF("Filling with 0xFF's...\n");
   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = 0xFF;
      EEPROM_Write (address, test_byte);

      // Print status to UART0
      if ((address % 16) == 0)
      {
         RETARGET_PRINTF ("\nWriting 0x%04x: %02x ", address, (uint16_t)test_byte);
         LED1 = !LED1;
      }
      else
      {
         RETARGET_PRINTF ("%02x ", (uint16_t)test_byte);
      }
   }

   // Verify EEPROM with 0xFF's
   RETARGET_PRINTF("\n\nVerifying 0xFF's...\n");
   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = EEPROM_Read (address);

      // Print status to UART0
      if ((address % 16) == 0)
      {
         RETARGET_PRINTF ("\nVerifying 0x%04x: %02x ", address, (uint16_t)test_byte);
         LED1 = !LED1;
      }
      else
      {
         RETARGET_PRINTF ("%02x ", (uint16_t)test_byte);
      }
      if (test_byte != 0xFF)
      {
         LED1 = LED_OFF;
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
         RETARGET_PRINTF ("\nWriting 0x%04x: %02x ", address, (uint16_t)test_byte);
         LED1 = !LED1;
      }
      else
      {
         RETARGET_PRINTF ("%02x ", (uint16_t)test_byte);
      }
   }

   // Verify EEPROM with LSB of EEPROM addresses
   RETARGET_PRINTF("\n\nVerifying LSB of EEPROM addresses...\n");
   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = EEPROM_Read (address);

      // print status to UART0
      if ((address % 16) == 0)
      {
         RETARGET_PRINTF ("\nVerifying 0x%04x: %02x ", address, (uint16_t)test_byte);
         LED1 = !LED1;
      }
      else
      {
         RETARGET_PRINTF ("%02x ", (uint16_t)test_byte);
      }

      if (test_byte != (address & 0xFF))
      {
         LED1 = LED_OFF;
         RETARGET_PRINTF ("Error at %u\n", address);
         while (1);                    // Stop here on error (for debugging)
      }
   }

   RETARGET_PRINTF ("\n\nVerification success!\n");

   while (1)                           // Loop forever
   {
      LED1 = !LED1;                    // Flash LED when done (all verified)
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
   TMR2CN0_TR2   = 0;                          // Stop timer
   TMR2CN0_TF2H  = 0;                          // Clear timer overflow flag
   TMR2 = -( (uint16_t)(SYSCLK/1000000) * (uint16_t)(time_us) );
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
   // Writing a byte to the EEPROM is a five-step operation.

   // Step1: Set the Write Enable Latch to 1
   SPI0CN0_NSSMD0   = 0;                // Step1.1: Activate Slave Select
   SPI0DAT  = EEPROM_CMD_WREN;         // Step1.2: Send the WREN command
   while (!SPI0CN0_SPIF);               // Step1.3: Wait for end of transfer
   SPI0CN0_SPIF     = 0;                // Step1.4: Clear the SPI intr. flag
   SPI0CN0_NSSMD0   = 1;                // Step1.5: Deactivate Slave Select
   Delay_us (1);                       // Step1.6: Wait for at least
                                       //          T_NSS_DISABLE_MIN
   // Step2: Send the WRITE command
   SPI0CN0_NSSMD0   = 0;
   SPI0DAT  = EEPROM_CMD_WRITE;
   while (!SPI0CN0_SPIF);
   SPI0CN0_SPIF     = 0;

   // Step3: Send the EEPROM destination address (MSB first)
   SPI0DAT = (uint8_t)((address >> 8) & 0x00FF);
   while (!SPI0CN0_SPIF);
   SPI0CN0_SPIF     = 0;
   SPI0DAT = (uint8_t)(address & 0x00FF);
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
      SPI0CN0_NSSMD0   = 0;             // Activate Slave Select
      SPI0DAT  = EEPROM_CMD_RDSR;      // Send the Read Status Register command
      while (!SPI0CN0_SPIF);            // Wait for the command to be sent out
      SPI0CN0_SPIF     = 0;
      SPI0DAT  = 0;                    // Dummy write to output serial clock
      while (!SPI0CN0_SPIF);            // Wait for the register to be read
      SPI0CN0_SPIF     = 0;
      SPI0CN0_NSSMD0   = 1;             // Deactivate Slave Select after read
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
uint8_t EEPROM_Read (uint16_t address)
{
   // Reading a byte from the EEPROM is a three-step operation.

   // Step1: Send the READ command
   SPI0CN0_NSSMD0   = 0;                 // Activate Slave Select
   SPI0DAT  = EEPROM_CMD_READ;
   while (!SPI0CN0_SPIF);
   SPI0CN0_SPIF     = 0;

   // Step2: Send the EEPROM source address (MSB first)
   SPI0DAT = (uint8_t)((address >> 8) & 0x00FF);
   while (!SPI0CN0_SPIF);
   SPI0CN0_SPIF     = 0;
   SPI0DAT = (uint8_t)(address & 0x00FF);
   while (!SPI0CN0_SPIF);
   SPI0CN0_SPIF     = 0;

   // Step3: Read the value returned
   SPI0DAT  = 0;                       // Dummy write to output serial clock
   while (!SPI0CN0_SPIF);               // Wait for the value to be read
   SPI0CN0_SPIF     = 0;
   SPI0CN0_NSSMD0   = 1;                // Deactivate Slave Select
   Delay_us (1);

   return SPI0DAT;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
