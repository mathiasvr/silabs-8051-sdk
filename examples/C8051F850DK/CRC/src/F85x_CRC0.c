//-----------------------------------------------------------------------------
// F85x_CRC0.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program shows how to use the CRC engine in the automatic mode and the
// manual mode. The automatic mode can be used to calculate the CRC of flash
// pages. The manual mode can be used to calculate the CRC of any number of
// bytes manually written to the CRC engine.
//
// This program calculates the 16-bit CRC of the number of flash pages found in
// the global constant section. The first section uses the manual method to
// calculate the CRC of all bytes except the last four. The last four bytes
// store the resulting CRC value. The second section uses the automatic CRC
// feature to calculate the entire CRC, including the result stored in the last
// four bytes of the last flash pages. The final resulting CRC should be 0.
//
// NOTE: The block size of the CRC module is half the flash page size.
//       The CRC block size is 256 bytes and the Flash page size is 512 bytes.
//
//-----------------------------------------------------------------------------
// How To Test: UPMU-F850
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - J27: DS6 P1.1 - P1.1/ADC0.9
//    - JP2
// 2) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to the 'F85x UDP MCU board by selecting 
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the 
//		Resume button in the quick menu, or pressing F8.
//						  If the DS5 P1.0 LED is ON, the program is working.
//                  If the DS6 P1.1 LED is ON, a CRC error was detected.
//                  If both LEDs are off, the program did not run to
//                    completion.
//
//-----------------------------------------------------------------------------
// How To Test: ToolStick F85x
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 3) Compile and download code to the ToolStick F85x board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//                  If the DS5 P1.0 LED is ON, the program is working.
//                  If the DS6 P1.1 LED is ON, a CRC error was detected.
//                  If both LEDs are off, the program did not run to
//                    completion.
//
//
// Target:         F85x/86x
// Tool chain:     Keil ,Raisonance, SDCC
// Command Line:   None
//
// Release 1.2 (BL)
//		- Updated How To Test
//		- 2 JAN 2014
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    -Initial Revision (TP)
//    -19 FEB 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK            24500000/8   // SYSCLK frequency in Hz

SI_SBIT(LED1, SFR_P1, 0);                // DS5 P1.0 LED
SI_SBIT(LED2, SFR_P1, 1);                // DS6 P1.1 LED

#define LED_ON            0
#define LED_OFF           1

#define FLASH_PAGE_SIZE   512          // Flash page size in Bytes
#define START_PAGE        2            // Index of the Flash page to start the
                                       // auto calculation. The starting
                                       // address can be calculated by
                                       // FLASH_PAGE_SIZE * START_PAGE

#define PAGES_TO_CRC      5            // Defines the number of pages to CRC

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

   LED1 = LED_OFF;
   LED2 = LED_OFF;

   // Calculate arguments passed into the Byte_CRC16 function
   Code_PTR = FLASH_PAGE_SIZE * START_PAGE;
   Number_Of_Bytes = (FLASH_PAGE_SIZE * PAGES_TO_CRC) - 2;

   // Manually calculate CRC
   Byte_CRC16(Code_PTR, Number_Of_Bytes);
   Read_CRC16(&CRC_Result);

   // Write the CRC result to last bytes of last page that is used in CRC
   // calculation
   Write_CRC_to_Flash();

   // Calculate CRC including the CRC value written to the last bytes of the
   // last page.
   // Since the Block size of the F85x/86x is 256 (or half the flash page
   // size), multiply by two.
   Auto_CRC16 ((2*START_PAGE), (2*PAGES_TO_CRC));
   Read_CRC16(&CRC_Result);

   // If the CRC was calculated correctly, the resulting CRC will be 0.
   // If the CRC is not 0, then the CRC that was written in the Byte_CRC is
   // incorrect
   if (CRC_Result.u16 == 0x00)
   {
      LED1 = LED_ON;
      LED2 = LED_OFF;
   }
   else
   {
      LED1 = LED_OFF;
      LED2 = LED_ON;
   }

   while (1);
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
//  After this function completes, the CRC result is available in CRC0DAT.
//
//-----------------------------------------------------------------------------
void Auto_CRC16 (uint8_t Start_Page, uint8_t Pages_to_CRC)
{
   bit EA_save;

   EA_save = IE_EA;
   IE_EA = 0;

   // Initialize CRC result to 0x0000 and clear the current result.
   // If the result should be initialized to 0xFFFF, set CRC0CN |= 0x04 before
   // CRC0CN |= 0x08
   CRC0CN |= 0x08;

   // Set the starting page
   CRC0AUTO = Start_Page;

   // Set the number of pages
   CRC0CNT = Pages_to_CRC;

   // Set AUTOEN to enable automatic computation
   CRC0AUTO |= 0x80;

   // Write a dummy value to CRC0CN to start computation
   CRC0CN &= ~0x03;

   // Follow AUTO CRC with benign 3-byte opcode
   CRC0AUTO &= ~0x80;                  // Clear AUTOEN

   IE_EA = EA_save;
}

//-----------------------------------------------------------------------------
// Byte_CRC16
//-----------------------------------------------------------------------------
//
// Return Value:  uint8_t - returns value indicating if the App FW is valid.
//   1) uint8_t* Start_Byte - Pointer to first byte of data to CRC
//   2) uint16_t Num_Bytes - Number of consecutive bytes to include in the CRC
//
//  After this function completes, the CRC result is available in CRC0DAT.
//
//-----------------------------------------------------------------------------
void Byte_CRC16 (uint8_t * Start_Byte, uint16_t Num_Bytes)
{
   uint16_t i;

   // Initialize CRC result to 0x0000 and clear the current result.
   // If the result should be initialized to 0xFFFF, set CRC0CN |= 0x04 before
   // CRC0CN |= 0x08
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
// Copies CRC result to two bytes starting at the location passed into this
// function.
//
//-----------------------------------------------------------------------------
void Read_CRC16 (SI_UU16_t * Result)
{
   // Initialize CRC0PNT to access bits 7-0 of the CRC result
   CRC0CN &= ~0x01;

   // With each read of CRC0DAT, the value in CRC0PNT will toggle
   (*Result).u8[LSB] = CRC0DAT;
   (*Result).u8[MSB] = CRC0DAT;
}

//-----------------------------------------------------------------------------
// Write_CRC_to_Flash
//-----------------------------------------------------------------------------
//
// Return Value:  None.
// Parameters:    None
//
//-----------------------------------------------------------------------------
void Write_CRC_to_Flash (void)
{
   uint16_t end_address;
   end_address = (FLASH_PAGE_SIZE * (START_PAGE + PAGES_TO_CRC))-2;

   // The resulting CRC will be stored in last 2 bytes of the last page of
   // flash.

   // First erase last page where CRC will be written to.
   // The original CRC calculation assumes this page is blank (all 0xFF)
   FLASH_SetKeys ();
   FLASH_PageErase(end_address);

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
//   1) FLADDR addr - address of the byte to write to
//                    valid range is from 0x0000 to 0x1FFF for 8 kB devices
//                    valid range is from 0x0000 to 0x0FFF for 4 kB devices
//                    valid range is from 0x0000 to 0x07FF for 2 kB devices
//   2) char byte - byte to write to flash.
//
// This routine writes <byte> to the linear flash address <addr>.
//
//-----------------------------------------------------------------------------
void FLASH_ByteWrite (uint16_t addr, uint8_t byte)
{
   uint8_t EA_save = IE;                    // Preserve IE_EA
   SI_SEGMENT_VARIABLE_SEGMENT_POINTER(pwrite, uint8_t, SI_SEG_XDATA, SI_SEG_DATA);

   IE_EA = 0;                             // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor


   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   pwrite = (char xdata *) addr;

   // Keys are set in Flash_SetKeys();

   PSCTL |= 0x01;                      // PSWE = 1

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   *pwrite = byte;                     // Write the byte

   PSCTL &= ~0x01;                     // PSWE = 0

   if ((EA_save & 0x80) != 0)          // Restore IE_EA
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
//   1) FLADDR addr - address of any byte in the page to erase
//                    valid range is from 0x0000 to 0x1FFF for 8 kB devices
//                    valid range is from 0x0000 to 0x0FFF for 4 kB devices
//                    valid range is from 0x0000 to 0x07FF for 2 kB devices
//
// This routine erases the flash page containing the linear flash address
// <addr>.  Note that the page of flash page containing the Lock Byte cannot be
// erased if the Lock Byte is set.
//
//-----------------------------------------------------------------------------
void FLASH_PageErase (uint16_t addr)
{
   uint8_t EA_save = IE;                    // Preserve IE_EA

   SI_SEGMENT_VARIABLE_SEGMENT_POINTER(pwrite, uint8_t, xdata, data);

   IE_EA = 0;                             // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   pwrite = (char xdata *) addr;

   // Keys are set in Flash_SetKeys();

   PSCTL |= 0x03;                      // PSWE = 1; PSEE = 1

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
   *pwrite = 0;                        // Initiate page erase

   PSCTL &= ~0x03;                     // PSWE = 0; PSEE = 0

   if ((EA_save & 0x80) != 0)          // Restore IE_EA
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
// This routine sets the two required Flash keys for writes and erases.
//
//-----------------------------------------------------------------------------
void FLASH_SetKeys (void)
{
   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
