//-----------------------------------------------------------------------------
// F99x_CRC.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program shows how to use the CRC engine in the automatic mode and the
// manual mode. The automatic mode can be used to calculate the CRC of Flash
// pages. The manual mode can be used to calculate the CRC of any number of
// bytes manually written to the CRC engine.
//
// This program calculates the 16-bit CRC of the number of Flash pages found in
// the global constant section. The first section uses the manual method to
// calculate the CRC of all bytes except the last four. The last four bytes
// store the resulting CRC value. The second section uses the automatic CRC
// feature to calculate the entire CRC, including the result stored in the last
// four bytes of the last Flash pages. The final resulting CRC should be 0.
//
// NOTE: The block size of the CRC module is not equal to the Flash page size.
//       The CRC block size is 256 bytes and the Flash page size is 512 bytes.
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J8:   P1.3/LED1.3
//             P1.5/LED1.5
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J4.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F99x device on a C8051F99x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//                      If the Yellow LED is ON, the program is working.
//                      If the Red LED is ON, the program successfully finished
//                      If both LEDs are off, the program completed with errors
//
//
// Target:         C8051F99x-C8051F98x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 13 JAN 2014
//
// Release 1.0
//    -Initial Revision (ES)
//    -03 JAN 2011
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F990_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(RED_LED,    SFR_P1, 5);          // '0' means ON, '1' means OFF
SI_SBIT(YELLOW_LED, SFR_P1, 3);          // '0' means ON, '1' means OFF

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK            20000000/8   // SYSCLK frequency in Hz

#define LED_ON            0
#define LED_OFF           1

#define FLASH_PAGE_SIZE   512          // Flash page size in Bytes
#define START_PAGE        2            // Index of the Flash page to start the
                                       // auto calculation. The starting
                                       // address can be calculated by
                                       // 2 * FLASH_PAGE_SIZE * START_PAGE

#define PAGES_TO_CRC	  10           // defines the number of pages to CRC

SI_UU16_t CRC_Result;                       // Will store the CRC results

// The following three pointers are used to pass a pointer into the Byte CRC
// function. This allows for an array of bytes from any of the three memory
// space below to be fed into the CRC engine.
SI_VARIABLE_SEGMENT_POINTER(Code_PTR, uint8_t, SI_SEG_CODE);
SI_VARIABLE_SEGMENT_POINTER(Xdata_PTR, uint8_t, SI_SEG_XDATA);
SI_VARIABLE_SEGMENT_POINTER(Data_PTR, uint8_t, SI_SEG_DATA);

uint16_t Number_Of_Bytes;                   // Number of consecutive bytes to CRC.
                                       // This is used with the three segment
                                       // pointers above.

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void Auto_CRC16 (uint8_t Start_Page, uint8_t Pages_to_CRC);
void Byte_CRC16 (uint8_t * Start_Byte, uint16_t Num_Bytes);
void Read_CRC16 (SI_UU16_t * Result);

void Write_CRC_to_Flash (void);
void FLASH_PageErase (uint16_t addr);
void FLASH_ByteWrite (uint16_t addr, uint8_t byte);
void FLASH_SetKeys (void);


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
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   enter_DefaultMode_from_RESET();

   CRC_Result.u16 = 0x0000;

   YELLOW_LED = LED_ON;
   RED_LED = LED_OFF;

   // Calculate arguments passed into the Byte_CRC16 function
   Code_PTR =(FLASH_PAGE_SIZE*START_PAGE);
   Number_Of_Bytes = ((FLASH_PAGE_SIZE*PAGES_TO_CRC)-2);

   // Manually calculate CRC
   Byte_CRC16 (Code_PTR, Number_Of_Bytes);
   Read_CRC16 (&CRC_Result);

   // Write CRC to last bytes of last page that is used in CRC calculation
   Write_CRC_to_Flash ();

   // Calculate CRC including the CRC value written to the last bytes of last page
   // Since the Block size of the F990 is 256 (or half the Flash page size),
   // multiply by two.
   Auto_CRC16 ((2*START_PAGE), (2*PAGES_TO_CRC));
   Read_CRC16 (&CRC_Result);

   // If the CRC was calculated correct, the resulting CRC will be 0.
   // If the CRC is not 0, then the CRC that was written in the Byte_CRC is incorrect
   if (CRC_Result.u16 == 0x00)
   {
      YELLOW_LED = LED_OFF;
      RED_LED = LED_ON;
   }
   else
   {
      YELLOW_LED = LED_OFF;
      RED_LED = LED_OFF;
   }

   while (1) {}                        // Spin forever
}

//-----------------------------------------------------------------------------
// Auto_CRC16
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters   :
//   1) uint8_t Start_Page - Starting page of Flash memory to CRC. The starting
//         address of the first page is (FLASH_PAGE_SIZE * Start_Page)
//   2) uint8_t Pages_to_CRC - Number of pages in Flash to CRC. This determines the
//         total numbero of pages to CRC. If this is set to 1, the only Flash
//         page that will be included in the CRC is the Start_Page.
//
//   After this function completes, the result is kept in CRC0DAT
//-----------------------------------------------------------------------------
void Auto_CRC16 (uint8_t Start_Page, uint8_t Pages_to_CRC)
{

   // set CRC0SEL to 1 (16-bit calc),
   CRC0CN |= 0x10;
   // initialize CRC result to 0x00 and clear the current result.
   // If the result should be initialized to 0xFF, CRC0CN |= 0x0C;
   CRC0CN |= 0x08;

   // set starting page
   CRC0AUTO = Start_Page;
   // set # pages
   CRC0CNT = Pages_to_CRC;

   // set AUTOEN to enable automatic computation
   CRC0AUTO |= 0x80;

   // write to CRC0CN to start computation
   CRC0CN &= ~0x03;

   // follow AUTO CRC with benign 3-byte opcode
   CRC0FLIP = 0xFF;                    // benign 3-byte opcode
   CRC0AUTO &= ~0x80;                  // clear AUTOEN

}

//-----------------------------------------------------------------------------
// Byte_CRC16
//-----------------------------------------------------------------------------
//
// Return Value:  uint8_t - returns value indicating if the App FW is valid.
//   1) uint8_t* Start_Byte - Pointer to first byte of data to CRC
//   2) uint16_t Num_Bytes - Number of consecutive bytes to include in the CRC
//
//   After this function completes, the result is kept in CRC0DAT
//-----------------------------------------------------------------------------
void Byte_CRC16 (uint8_t * Start_Byte, uint16_t Num_Bytes)
{
   uint16_t i;

   // set CRC0SEL to 1 (16-bit calc),
   CRC0CN |= 0x10;
   // initialize CRC result to 0x00 and clear the current result.
   // If the result should be initialized to 0xFF, CRC0CN |= 0x0C;
   CRC0CN |= 0x08;

   for (i = 0; i < Num_Bytes; i++)
   {
      CRC0IN = Start_Byte[i];
   }

}

//-----------------------------------------------------------------------------
// Read_CRC16
//-----------------------------------------------------------------------------
//
// Return Value : None.
// Parameters   :
//   1) uint8_t* Result - Stores CRC into 2 bytes starting at this location
//
// Copies CRC result to two bytes starting at location passed into function
//-----------------------------------------------------------------------------
void Read_CRC16 (SI_UU16_t * Result)
{

   // Initialize CRC0PNT to access bits 7-0 of the CRC result
   CRC0CN &= ~0x03;

   // With each read of CRC0DAT, the value in CRC0PNT will auto-increment
   (*Result).u8[LSB] = CRC0DAT;
   (*Result).u8[MSB] = CRC0DAT;

}

//-----------------------------------------------------------------------------
// Write_CRC_to_Flash
//-----------------------------------------------------------------------------
//
// Return Value:  uint8_t - returns value indicating if the App FW is valid.
// Parameters:    None
//
//-----------------------------------------------------------------------------
void Write_CRC_to_Flash (void)
{
   uint16_t end_address;
   end_address = FLASH_PAGE_SIZE * (START_PAGE + PAGES_TO_CRC);

   // First erase last page where CRC will be written to
   FLASH_SetKeys ();
   FLASH_PageErase(FLASH_PAGE_SIZE * ((START_PAGE + PAGES_TO_CRC)-1));

   end_address -= 2;                   // The resulting CRC will be stored in
                                       // last 2 bytes of the last page of
                                       // Flash (for 16-bit mode).
   FLASH_SetKeys();
   FLASH_ByteWrite (end_address++, CRC_Result.u8[MSB]);
   FLASH_SetKeys();
   FLASH_ByteWrite (end_address++, CRC_Result.u8[LSB]);
}

//-----------------------------------------------------------------------------
// FLASH_ByteWrite
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint16_t addr - address of the byte to write to
//   2) uint8_t byte - byte to write to Flash.
//
// This routine writes <byte> to the linear FLASH address <addr>.
//-----------------------------------------------------------------------------

void FLASH_ByteWrite (uint16_t addr, uint8_t byte)
{

   uint8_t EA_Save = IE;                    // Preserve IE_EA
   SI_SEGMENT_VARIABLE_SEGMENT_POINTER(pwrite, uint8_t, SI_SEG_XDATA, SI_SEG_DATA);

   IE_EA = 0;                             // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor


   RSTSRC = 0x06;                      // Enable VDD monitor as a reset source
                                       // Leave missing clock detector enabled

   pwrite = (char xdata *) addr;

   // Keys are set in Flash_SetKeys();

   PSCTL |= 0x01;                      // PSWE = 1

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   *pwrite = byte;                     // Write the byte

   PSCTL &= ~0x05;                     // SFLE = 0; PSWE = 0

   if ((EA_Save & 0x80) != 0)          // Restore IE_EA
   {
      IE_EA = 1;
   }
}

//-----------------------------------------------------------------------------
// FLASH_PageErase
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint16_t addr - address of any byte in the page to erase
//
// This routine erases the FLASH page containing the linear FLASH address
// <addr>.  Note that the page of Flash containing the Lock Byte cannot be
// erased from application code.
//-----------------------------------------------------------------------------

void FLASH_PageErase (uint16_t addr)
{

   uint8_t EA_Save = IE;                    // Preserve IE_EA

   SI_SEGMENT_VARIABLE_SEGMENT_POINTER(pwrite, uint8_t, xdata, data);

   IE_EA = 0;                             // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x06;                      // Enable VDD monitor as a reset source
                                       // Leave missing clock detector enabled

   pwrite = (char xdata *) addr;

   // Keys are set in Flash_SetKeys();

   PSCTL |= 0x03;                      // PSWE = 1; PSEE = 1

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
   *pwrite = 0;                        // Initiate page erase

   PSCTL &= ~0x07;                     // SFLE = 0; PSWE = 0; PSEE = 0

   if ((EA_Save & 0x80) != 0)          // Restore IE_EA
   {
      IE_EA = 1;
   }

}

//-----------------------------------------------------------------------------
// FLASH_SetKeys
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine erases sets the two required Flash keys for writes and erases.
//-----------------------------------------------------------------------------

void FLASH_SetKeys (void)
{
   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
