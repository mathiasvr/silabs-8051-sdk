//-----------------------------------------------------------------------------
// SB1_SMBus_EEPROM.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates how the SMBus interface can communicate
// with a 256 byte I2C Serial EEPROM (Microchip 24LC02B).
// - Interrupt-driven SMBus implementation
// - Only master states defined (no slave or arbitration)
// - Timer1 used as SMBus clock source
// - Timer2 used by SMBus for SCL low timeout detection
// - SCL frequency defined by SMBUS module in Configurator
// - Pinout:
//    P1.2 -> SDA (SMBus)
//    P1.3 -> SCL (SMBus)
//
//    P1.1 -> LED0
//
//    P2.0 -> C2D (debug interface)
//
//    all other port pins unused
//
// How To Test:
// 1) Place the switch in "AEM" mode.
// 2) Connect the device to a 24LC02B serial EEPROM (see the EEPROM datasheet
//    for the pinout information).
// 3) Connect the EFM8SB1 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
//         a) the test will indicate proper communication with the EEPROM by
//            turning on the LED0 at the end the end of the test
//         b) the test can also be verified by running to the if statements
//            in main and checking the sent and received values by adding
//            the variables to the Watch Window
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
#include "EFM8SB1_SMBus_EEPROM.h"

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
uint8_t* pSMB_DATA_IN;                      // Global pointer for SMBus data
                                       // All receive data is written here

uint8_t SMB_SINGLEBYTE_OUT;                 // Global holder for single byte writes.

uint8_t* pSMB_DATA_OUT;                     // Global pointer for SMBus data.
                                       // All transmit data is read from here

uint8_t SMB_DATA_LEN;                       // Global holder for number of bytes
                                       // to send or receive in the current
                                       // SMBus transfer.

uint8_t WORD_ADDR;                          // Global holder for the EEPROM word
                                       // address that will be accessed in
                                       // the next transfer

uint8_t TARGET;                             // Target SMBus slave address

volatile bool SMB_BUSY = 0;                      // Software flag to indicate when the
                                       // EEPROM_ByteRead() or
                                       // EEPROM_ByteWrite()
                                       // functions have claimed the SMBus

volatile bool SMB_RW;                            // Software flag to indicate the
                                       // direction of the current transfer

volatile bool SMB_SENDWORDADDR;                  // When set, this flag causes the ISR
                                       // to send the 8-bit <WORD_ADDR>
                                       // after sending the slave address.


volatile bool SMB_RANDOMREAD;                    // When set, this flag causes the ISR
                                       // to send a START signal after sending
                                       // the word address.
                                       // For the 24LC02B EEPROM, a random read
                                       // (a read from a particular address in
                                       // memory) starts as a write then
                                       // changes to a read after the repeated
                                       // start is sent. The ISR handles this
                                       // switchover if the <SMB_RANDOMREAD>
                                       // bit is set.

volatile bool SMB_ACKPOLL;                       // When set, this flag causes the ISR
                                       // to send a repeated START until the
                                       // slave has acknowledged its address

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 7);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (LED0, SFR_P1, 1);          // LED0==LED_ON means ON

SI_SBIT (SDA, SFR_P1, 2);                 // SMBus on P1.2
SI_SBIT (SCL, SFR_P1, 3);                 // and P1.3

SI_LOCATED_VARIABLE_NO_INIT (reserved, uint8_t, SI_SEG_XDATA, 0x0000);


//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

SI_INTERRUPT_PROTO (TIMER3_ISR, TIMER3_IRQn);
SI_INTERRUPT_PROTO (SMBUS0_ISR, SMBUS0_IRQn);

void EEPROM_ByteWrite(uint8_t addr, uint8_t dat);
void EEPROM_WriteArray(uint8_t dest_addr, uint8_t* src_addr,
                       uint8_t len);
uint8_t EEPROM_ByteRead(uint8_t addr);
void EEPROM_ReadArray(uint8_t* dest_addr, uint8_t src_addr,
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
	uint8_t in_buff[8] = {0};          // Incoming data buffer
	uint8_t out_buff[8] = "ABCDEFG";   // Outgoing data buffer

	uint8_t temp_char;                 // Temporary variable
	bool error_flag = 0;               // Flag for checking EEPROM contents
	uint8_t i;                         // Temporary counter variable


	//Enter default mode
	enter_DefaultMode_from_RESET();

	DISP_EN = DISP_BC_DRIVEN;

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

	enter_Mode2_from_DefaultMode();

	LED0 = LED_OFF;               // Turn off the LED before the test

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
	  LED0 = LED_ON;
	}

	while (1) {}                             // Spin forever
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
   SMB0CN0_STA = 1;

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
void EEPROM_WriteArray(uint8_t dest_addr, uint8_t* src_addr, uint8_t len)
{
   uint8_t i;
   uint8_t* pData = (uint8_t*) src_addr;

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
   SMB0CN0_STA = 1;
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
void EEPROM_ReadArray (uint8_t* dest_addr, uint8_t src_addr, uint8_t len)
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
   SMB0CN0_STA = 1;
   while(SMB_BUSY);                    // Wait until data is read

}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
