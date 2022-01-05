//-----------------------------------------------------------------------------
// F85x_SPI0_EEPROM_Polled_Mode.c
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
// P0.0 - SPI SCK    (digital output, push-pull)
// P0.1 - SPI MISO   (digital input,  open-drain)
// P0.2 - SPI MOSI   (digital output, push-pull)
// P0.3 - SPI NSS    (digital output, push-pull)
// P0.4 - UART TXD   (digital output, push-pull)
// P0.5 - UART RXD   (digital input,  open-drain)
//
// P1.0 - LED1       (digital output, push-pull)
//
//-----------------------------------------------------------------------------
// How To Test: UPMU-F850
//-----------------------------------------------------------------------------
// Method1:
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - JP2
// 2) Connect the device to a Microchip 25LC320 SPI EEPROM.
// 3) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 4) Compile and download code to the 'F85x UDP MCU board by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
//		The LED will blink fast during the write/read/verify
//    operations.
// 6) If the verification passes, the LED will blink slowly. If it fails,
//    the LED will be off.
//
// Method2 (optional):
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - J7: all
//    - JP2
// 2) Connect the device to a Microchip 25LC320 SPI EEPROM.
// 3) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 4) Compile and download code to the 'F85x UDP MCU board by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 5) Connect a USB cable from the J5 of the MCU card to a PC.
// 6) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the USB port (virtual COM port) at <BAUDRATE> (230400) and 8-N-1.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
//		The terminal will print the progress of the write/read
//    operation and will print the test result as pass or fail. Additionally,
//    if the verification passes, the DS5 P1.0 LED will blink slowly. If it
//    fails, the LED will be off.
//
//-----------------------------------------------------------------------------
// How To Test: ToolStick F85x
//-----------------------------------------------------------------------------
// Method1:
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Connect the device to a Microchip 25LC320 SPI EEPROM.
// 3) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 4) Compile and download code to the ToolStick F85x board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//    The LED will blink fast during the write/read/verify
//    operations.
// 6) If the verification passes, the LED will blink slowly. If it fails,
//    the LED will be off.
//
// Method2 (optional):
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Connect the device to a Microchip 25LC320 SPI EEPROM.
// 3) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 4) Compile and download code to the ToolStick F85x board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Disconnect from the device by selecting Run -> Disconnect from the menus.
// 6) On the PC, open Silicon Laboratories ToolStick Terminal, and navigate to
//    ToolStick -> Settings in the menus. Under UART Settings, select the
//    following options:
//       -Baud Rate: as defined by <BAUDRATE> (230400 by default)
//       -Parity: None
//       -Data Bits: 8
//       -Stop Bits: 1
// 7) In ToolStick Terminal, press "Connect" to connect to the device.
//    The terminal will print the progress of the write/read
//    operation and will print the test result as pass or fail. Additionally,
//    if the verification passes, the DS5 P1.0 LED will blink slowly. If it
//    fails, the LED will be off.
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.3 (ST)
//	  - Updated to use Simplicity Configurator initializations
//    - 30 JAN 2014
//
// Release 1.2 (BL)
//	  - Updated How To Test with Studio specific instructions
//    - 3 JAN 2014
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 25 FEB 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>
#include <stdio.h>                     // printf is declared here
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK                24500000 // Internal oscillator frequency in Hz
#define BAUDRATE                230400 // Baud rate of UART in bps

// Microchip 25LC320 Slave EEPROM Parameters
#define  F_SCK_MAX             2000000 // Max SCK freq (Hz)
#define  T_NSS_DISABLE_MIN         500 // Min NSS disable time (ns)
#define  EEPROM_CAPACITY          4096 // EEPROM capacity (bytes)

// EEPROM Instruction Set
#define  EEPROM_CMD_READ          0x03 // Read Command
#define  EEPROM_CMD_WRITE         0x02 // Write Command
#define  EEPROM_CMD_WRDI          0x04 // Reset Write Enable Latch Command
#define  EEPROM_CMD_WREN          0x06 // Set Write Enable Latch Command
#define  EEPROM_CMD_RDSR          0x05 // Read Status Register Command
#define  EEPROM_CMD_WRSR          0x01 // Write Status Register Command

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(LED1, SFR_P1, 0);                // DS5 P1.0 LED

#define LED_ON                       0
#define LED_OFF                      1

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

   SCON0_TI = 1;                       // Prime uart printf

   // The following code will test the EEPROM by performing write/read/verify
   // operations. The first test will write 0xFFs to the EEPROM, and the
   // second test will write the LSBs of the EEPROM addresses.

   // Fill EEPROM with 0xFF's
   LED1 = LED_ON;
   printf("Filling with 0xFF's...\n");
   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = 0xFF;
      EEPROM_Write (address, test_byte);

      // Print status to UART0
      if ((address % 16) == 0)
      {
         printf ("\nWriting 0x%04x: %02x ", address, (uint16_t)test_byte);
         LED1 = !LED1;
      }
      else
      {
         printf ("%02x ", (uint16_t)test_byte);
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
         printf ("\nVerifying 0x%04x: %02x ", address, (uint16_t)test_byte);
         LED1 = !LED1;
      }
      else
      {
         printf ("%02x ", (uint16_t)test_byte);
      }

      if (test_byte != 0xFF)
      {
         LED1 = LED_OFF;
         printf ("Error at %u\n", address);
         while(1);                     // Stop here on error (for debugging)
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
         printf ("\nWriting 0x%04x: %02x ", address, (uint16_t)test_byte);
         LED1 = !LED1;
      }
      else
      {
         printf ("%02x ", (uint16_t)test_byte);
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
         printf ("\nVerifying 0x%04x: %02x ", address, (uint16_t)test_byte);
         LED1 = !LED1;
      }
      else
      {
         printf ("%02x ", (uint16_t)test_byte);
      }

      if (test_byte != (address & 0xFF))
      {
         LED1 = LED_OFF;
         printf ("Error at %u\n", address);
         while(1);                     // Stop here on error (for debugging)
      }
   }

   printf ("\n\nVerification success!\n");

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
   TMR2CN_TR2 = 0;                     // Stop timer
   TMR2CN_TF2H = 0;                    // Clear timer overflow flag
   TMR2 = -( (uint16_t)(SYSCLK/1000000) * (uint16_t)(time_us) );
   TMR2CN_TR2 = 1;                     // Start timer
   while(!TMR2CN_TF2H);                // Wait till timer overflow occurs
   TMR2CN_TR2 = 0;                     // Stop timer
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
   SPI0CN_NSSMD0 = 0;                  // Step1.1: Activate Slave Select
   SPI0DAT = EEPROM_CMD_WREN;          // Step1.2: Send the WREN command
   while(!SPI0CN_SPIF);                // Step1.3: Wait for end of transfer
   SPI0CN_SPIF = 0;                    // Step1.4: Clear the SPI intr. flag
   SPI0CN_NSSMD0 = 1;                  // Step1.5: Deactivate Slave Select
   Delay_us (1);                       // Step1.6: Wait for at least
                                       //          T_NSS_DISABLE_MIN
   // Step2: Send the WRITE command
   SPI0CN_NSSMD0 = 0;
   SPI0DAT = EEPROM_CMD_WRITE;
   while(!SPI0CN_SPIF);
   SPI0CN_SPIF = 0;

   // Step3: Send the EEPROM destination address (MSB first)
   SPI0DAT = (uint8_t)((address >> 8) & 0x00FF);
   while(!SPI0CN_SPIF);
   SPI0CN_SPIF = 0;
   SPI0DAT = (uint8_t)(address & 0x00FF);
   while(!SPI0CN_SPIF);
   SPI0CN_SPIF = 0;

   // Step4: Send the value to write
   SPI0DAT = value;
   while(!SPI0CN_SPIF);
   SPI0CN_SPIF = 0;
   SPI0CN_NSSMD0 = 1;
   Delay_us(1);

   // Step5: Poll on the Write In Progress (WIP) bit in Read Status Register
   do
   {
      SPI0CN_NSSMD0 = 0;               // Activate Slave Select
      SPI0DAT = EEPROM_CMD_RDSR;       // Send the Read Status Register command
      while(!SPI0CN_SPIF);             // Wait for the command to be sent out
      SPI0CN_SPIF = 0;
      SPI0DAT = 0;                     // Dummy write to output serial clock
      while(!SPI0CN_SPIF);             // Wait for the register to be read
      SPI0CN_SPIF = 0;
      SPI0CN_NSSMD0 = 1;               // Deactivate Slave Select after read
      Delay_us (1);
   } while((SPI0DAT & 0x01) == 0x01);
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
   SPI0CN_NSSMD0 = 0;                  // Activate Slave Select
   SPI0DAT = EEPROM_CMD_READ;
   while(!SPI0CN_SPIF);
   SPI0CN_SPIF = 0;

   // Step2: Send the EEPROM source address (MSB first)
   SPI0DAT = (uint8_t)((address >> 8) & 0x00FF);
   while(!SPI0CN_SPIF);
   SPI0CN_SPIF = 0;
   SPI0DAT = (uint8_t)(address & 0x00FF);
   while(!SPI0CN_SPIF);
   SPI0CN_SPIF = 0;

   // Step3: Read the value returned
   SPI0DAT = 0;                        // Dummy write to output serial clock
   while(!SPI0CN_SPIF);                // Wait for the value to be read
   SPI0CN_SPIF = 0;
   SPI0CN_NSSMD0 = 1;                  // Deactivate Slave Select
   Delay_us (1);

   return SPI0DAT;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------