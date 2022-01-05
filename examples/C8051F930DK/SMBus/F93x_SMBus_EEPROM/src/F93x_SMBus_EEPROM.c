//-----------------------------------------------------------------------------
// F93x_SMBus_EEPROM.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates how the C8051F93x SMBus interface can communicate
// with a 256 byte I2C Serial EEPROM (Microchip 24LC02B).
// - Interrupt-driven SMBus implementation
// - Only master states defined (no slave or arbitration)
// - Timer1 used as SMBus clock source
// - Timer2 used by SMBus for SCL low timeout detection
// - SCL frequency defined within Configurator
// - Pinout:
//    P0.0 -> SDA (SMBus)
//    P0.1 -> SCL (SMBus)
//
//    P1.3 -> YELLOW_LED
//
//    P2.0 -> C2D (debug interface)
//
//    all other port pins unused
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J8:   P1.6/LED1.6
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J9.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//         a) the test will indicate proper communication with the EEPROM by
//            turning on the LED at the end the end of the test
//         b) the test can also be verified by running to the if statements
//            in main and checking the sent and received values by adding
//            the variables to the Watch Window
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
//    - 08 JULY 2009
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
#include "F93x_SMBus_EEPROM.h"

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
uint8_t * pSMB_DATA_IN;                      // Global pointer for SMBus data
                                       // All receive data is written here

uint8_t SMB_SINGLEBYTE_OUT;                 // Global holder for single byte writes.

uint8_t * pSMB_DATA_OUT;                     // Global pointer for SMBus data.
                                       // All transmit data is read from here

uint8_t SMB_DATA_LEN;                       // Global holder for number of bytes
                                       // to send or receive in the current
                                       // SMBus transfer.

uint8_t WORD_ADDR;                          // Global holder for the EEPROM word
                                       // address that will be accessed in
                                       // the next transfer

uint8_t TARGET;                             // Target SMBus slave address

volatile bit SMB_BUSY = 0;             // Software flag to indicate when the
                                       // EEPROM_ByteRead() or
                                       // EEPROM_ByteWrite()
                                       // functions have claimed the SMBus

volatile bit SMB_RW;                   // Software flag to indicate the
                                       // direction of the current transfer

volatile bit SMB_SENDWORDADDR;         // When set, this flag causes the ISR
                                       // to send the 8-bit <WORD_ADDR>
                                       // after sending the slave address.


volatile bit SMB_RANDOMREAD;           // When set, this flag causes the ISR
                                       // to send a START signal after sending
                                       // the word address.
                                       // For the 24LC02B EEPROM, a random read
                                       // (a read from a particular address in
                                       // memory) starts as a write then
                                       // changes to a read after the repeated
                                       // start is sent. The ISR handles this
                                       // switchover if the <SMB_RANDOMREAD>
                                       // bit is set.

volatile bit SMB_ACKPOLL;              // When set, this flag causes the ISR
                                       // to send a repeated START until the
                                       // slave has acknowledged its address

SI_SBIT(YELLOW_LED, SFR_P1, 6);          // YELLOW_LED==LED_ON means ON

SI_SBIT(SDA, SFR_P0, 0);                 // SMBus on P0.0
SI_SBIT(SCL, SFR_P0, 1);                 // and P0.1

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SMBus_Init(void);
void Timer1_Init(void);
void Timer3_Init(void);
void Port_Init(void);

SI_INTERRUPT_PROTO(TIMER3_ISR, TIMER3_IRQn);
SI_INTERRUPT_PROTO(SMBUS0_ISR, SMBUS0_IRQn);

void EEPROM_ByteWrite(uint8_t addr, uint8_t dat);
void EEPROM_WriteArray(uint8_t dest_addr, uint8_t * src_addr,
                       uint8_t len);
uint8_t EEPROM_ByteRead(uint8_t addr);
void EEPROM_ReadArray(uint8_t * dest_addr, uint8_t src_addr,
                      uint8_t len);

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
	char in_buff[8] = {0};              // Incoming data buffer
	char out_buff[8] = "ABCDEFG";       // Outgoing data buffer

	uint8_t temp_char;                                // Temporary variable
	bit error_flag = 0;                          // Flag for checking EEPROM contents
	uint8_t i;                                        // Temporary counter variable


	//Enter default mode
	enter_DefaultMode_from_RESET();

	// If slave is holding SDA low because of an improper SMBus reset or error
	while(!SDA)
	{
	  // Provide clock pulses to allow the slave to advance out
	  // of its current state. This will allow it to release SDA.
	  XBR1 = 0x40;                     // Enable Crossbar
	  SCL = 0;                         // Drive the clock low
	  for(i = 0; i < 255; i++);        // Hold the clock low
	  SCL = 1;                         // Release the clock
	  while(!SCL);                     // Wait for open-drain
									   // clock output to rise
	  for(i = 0; i < 10; i++);         // Hold the clock high
	  XBR1 = 0x00;                     // Disable Crossbar
	}

	//Enter default mode 2
	enter_DefaultMode_2_from_DefaultMode();

	YELLOW_LED = LED_OFF;               // Turn off the LED before the test
									   // starts

	// Read and write some bytes to the EEPROM and check for proper
	// communication

	// Write the value 0xAA to location 0x25 in the EEPROM
	EEPROM_ByteWrite(0x25, 0xAA);

	// Read the value at location 0x25 in the EEPROM
	temp_char = EEPROM_ByteRead(0x25);

	// Check that the data was read properly
	if (temp_char != 0xAA)
	{
	  error_flag = 1;
	}

	// Write the value 0xBB to location 0x25 in the EEPROM
	EEPROM_ByteWrite(0x25, 0xBB);

	// Write the value 0xCC to location 0x38 in the EEPROM
	EEPROM_ByteWrite(0x38, 0xCC);

	// Read the value at location 0x25 in the EEPROM
	temp_char = EEPROM_ByteRead(0x25);

	// Check that the data was read properly
	if (temp_char != 0xBB)
	{
	  error_flag = 1;
	}

	// Read the value at location 0x38 in the EEPROM
	temp_char = EEPROM_ByteRead(0x38);

	// Check that the data was read properly
	if (temp_char != 0xCC)
	{
	  error_flag = 1;
	}

	// Store the outgoing data buffer at EEPROM address 0x50
	EEPROM_WriteArray(0x50, out_buff, sizeof(out_buff));

	// Fill the incoming data buffer with data starting at EEPROM address 0x50
	EEPROM_ReadArray(in_buff, 0x50, sizeof(in_buff));

	// Check that the data that came from the EEPROM is the same as what was
	// sent
	for (i = 0; i < sizeof(in_buff); i++)
	{
	  if (in_buff[i] != out_buff[i])
	  {
		 error_flag = 1;
	  }
	}

	// Indicate communication is good
	if (error_flag == 0)
	{
	  // LED = ON indicates that the test passed
	  YELLOW_LED = LED_ON;
	}

	while(1);

	// NOTREACHED
	return 0;
}

//-----------------------------------------------------------------------------
// Support Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// EEPROM_ByteWrite ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint8_t addr - address to write in the EEPROM
//                        range is full range of character: 0 to 255
//
//   2) uint8_t dat - data to write to the address <addr> in the EEPROM
//                        range is full range of character: 0 to 255
//
// This function writes the value in <dat> to location <addr> in the EEPROM
// then polls the EEPROM until the write is complete.
//
//-----------------------------------------------------------------------------
void EEPROM_ByteWrite(uint8_t addr, uint8_t dat)
{
   while (SMB_BUSY);                   // Wait for SMBus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)

   // Set SMBus ISR parameters
   TARGET = EEPROM_ADDR;               // Set target slave address
   SMB_RW = WRITE;                     // Mark next transfer as a write
   SMB_SENDWORDADDR = 1;               // Send Word Address after Slave Address
   SMB_RANDOMREAD = 0;                 // Do not send a START signal after
                                       // the word address
   SMB_ACKPOLL = 1;                    // Enable Acknowledge Polling (The ISR
                                       // will automatically restart the
                                       // transfer if the slave does not
                                       // acknoledge its address.

   // Specify the Outgoing Data
   WORD_ADDR = addr;                   // Set the target address in the
                                       // EEPROM's internal memory space

   SMB_SINGLEBYTE_OUT = dat;           // Store <dat> (local variable) in a
                                       // global variable so the ISR can read
                                       // it after this function exits

   // The outgoing data pointer points to the <dat> variable
   pSMB_DATA_OUT = &SMB_SINGLEBYTE_OUT;

   SMB_DATA_LEN = 1;                   // Specify to ISR that the next transfer
                                       // will contain one data byte

   // Initiate SMBus Transfer
   SMB0CN_STA = 1;

}

//-----------------------------------------------------------------------------
// EEPROM_WriteArray ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint8_t dest_addr - beginning address to write to in the EEPROM
//                        range is full range of character: 0 to 255
//
//   2) uint8_t* src_addr - pointer to the array of data to be written
//                        range is full range of character: 0 to 255
//
//   3) uint8_t len - length of the array to be written to the EEPROM
//                        range is full range of character: 0 to 255
//
// Writes <len> data bytes to the EEPROM slave specified by the <EEPROM_ADDR>
// constant.
//
//-----------------------------------------------------------------------------
void EEPROM_WriteArray(uint8_t dest_addr, uint8_t * src_addr, uint8_t len)
{
   uint8_t i;
   uint8_t * pData = (uint8_t*) src_addr;

   for( i = 0; i < len; i++ ){
      EEPROM_ByteWrite(dest_addr++, *pData++);
   }

}

//-----------------------------------------------------------------------------
// EEPROM_ByteRead ()
//-----------------------------------------------------------------------------
//
// Return Value :
//   1) uint8_t data - data read from address <addr> in the EEPROM
//                        range is full range of character: 0 to 255
//
// Parameters   :
//   1) uint8_t addr - address to read data from the EEPROM
//                        range is full range of character: 0 to 255
//
// This function returns a single byte from location <addr> in the EEPROM then
// polls the <SMB_BUSY> flag until the read is complete.
//
//-----------------------------------------------------------------------------
uint8_t EEPROM_ByteRead(uint8_t addr)
{
   uint8_t retval;               // Holds the return value

   while (SMB_BUSY);                   // Wait for SMBus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)

   // Set SMBus ISR parameters
   TARGET = EEPROM_ADDR;               // Set target slave address
   SMB_RW = WRITE;                     // A random read starts as a write
                                       // then changes to a read after
                                       // the repeated start is sent. The
                                       // ISR handles this switchover if
                                       // the <SMB_RANDOMREAD> bit is set.
   SMB_SENDWORDADDR = 1;               // Send Word Address after Slave Address
   SMB_RANDOMREAD = 1;                 // Send a START after the word address
   SMB_ACKPOLL = 1;                    // Enable Acknowledge Polling


   // Specify the Incoming Data
   WORD_ADDR = addr;                   // Set the target address in the
                                       // EEPROM's internal memory space

   pSMB_DATA_IN = &retval;             // The incoming data pointer points to
                                       // the <retval> variable.

   SMB_DATA_LEN = 1;                   // Specify to ISR that the next transfer
                                       // will contain one data byte

   // Initiate SMBus Transfer
   SMB0CN_STA = 1;
   while(SMB_BUSY);                    // Wait until data is read

   return retval;

}

//-----------------------------------------------------------------------------
// EEPROM_ReadArray ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint8_t* dest_addr - pointer to the array that will be filled
//                                 with the data from the EEPROM
//                        range is full range of character: 0 to 255
//
//   2) uint8_t src_addr - beginning address to read data from the EEPROM
//                        range is full range of character: 0 to 255
//
//   3) uint8_t len - length of the array to be read from the EEPROM
//                        range is full range of character: 0 to 255
//
// Reads up to 256 data bytes from the EEPROM slave specified by the
// <EEPROM_ADDR> constant.
//
//-----------------------------------------------------------------------------
void EEPROM_ReadArray (uint8_t * dest_addr, uint8_t src_addr, uint8_t len)
{
   while (SMB_BUSY);                   // Wait for SMBus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)

   // Set SMBus ISR parameters
   TARGET = EEPROM_ADDR;               // Set target slave address
   SMB_RW = WRITE;                     // A random read starts as a write
                                       // then changes to a read after
                                       // the repeated start is sent. The
                                       // ISR handles this switchover if
                                       // the <SMB_RANDOMREAD> bit is set.
   SMB_SENDWORDADDR = 1;               // Send Word Address after Slave Address
   SMB_RANDOMREAD = 1;                 // Send a START after the word address
   SMB_ACKPOLL = 1;                    // Enable Acknowledge Polling

   // Specify the Incoming Data
   WORD_ADDR = src_addr;               // Set the target address in the
                                       // EEPROM's internal memory space

   // Set the the incoming data pointer
   pSMB_DATA_IN = (uint8_t*) dest_addr;


   SMB_DATA_LEN = len;                 // Specify to ISR that the next transfer
                                       // will contain <len> data bytes


   // Initiate SMBus Transfer
   SMB0CN_STA = 1;
   while(SMB_BUSY);                    // Wait until data is read

}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
