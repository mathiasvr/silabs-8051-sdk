//-----------------------------------------------------------------------------
// SB1_CRC.c
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
// calculate the CRC of all bytes except the last two. The last two bytes
// store the resulting CRC value. The second section uses the automatic CRC
// feature to calculate the entire CRC, including the result stored in the last
// two bytes of the last Flash pages. The final resulting CRC should be 0.
//
// NOTE: The block size of the CRC module is not equal to the Flash page size.
//       The CRC block size is 256 bytes and the Flash page size is 512 bytes.
//
// How To Test:
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8SB1 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8
//            If the Yellow LED is ON, the program is working.
//            If the Yellow LED is Blinking, the program successfully finished
//            If the Yellow LED is OFF, the program completed with errors
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
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8SB1_Register_Enums.h>
#include "InitDevice.h"
#include "EFM8SB1_FlashUtils.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 7);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (LED0, SFR_P1, 1);          // '1' means ON, '0' means OFF

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK            20000000/8   // SYSCLK frequency in Hz

#define LED_ON            1
#define LED_OFF           0

#define START_PAGE        3            // Index of the Flash page to start the
                                       // auto calculation. The starting
                                       // address can be calculated by
                                       // 2 * FLASH_PAGESIZE * START_PAGE

#define PAGES_TO_CRC	   10          // defines the number of pages to CRC

SI_UU16_t CRC_Result;                  // Will store the CRC results

// The following three pointers are used to pass a pointer into the Byte CRC
// function. This allows for an array of bytes from any of the three memory
// space below to be fed into the CRC engine.
SI_VARIABLE_SEGMENT_POINTER (Code_PTR, uint8_t, SI_SEG_CODE);
SI_VARIABLE_SEGMENT_POINTER (Xdata_PTR, uint8_t, SI_SEG_XDATA);
SI_VARIABLE_SEGMENT_POINTER (Data_PTR, uint8_t, SI_SEG_DATA);

uint16_t Number_Of_Bytes;              // Number of consecutive bytes to CRC.
                                       // This is used with the three segment
                                       // pointers above.

// A page of flash is reserved for flash utils to perform a read-modify-write
// when updating the last page of flash to write the CRC value to the last
// two bytes of the page
SI_LOCATED_VARIABLE_NO_INIT(flash_temp_reserved[FLASH_PAGESIZE], uint8_t, const SI_SEG_CODE, FLASH_TEMP);

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void Auto_CRC16 (uint8_t Start_Page, uint8_t Pages_to_CRC);
void Byte_CRC16 (SI_VARIABLE_SEGMENT_POINTER(Start_Byte, uint8_t, SI_SEG_CODE), uint16_t Num_Bytes);
void Read_CRC16 (SI_UU16_t* Result);

void Write_CRC_to_Flash (void);

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

   DISP_EN = DISP_BC_DRIVEN;

   CRC_Result.u16 = 0x0000;

   LED0 = LED_ON;

   // Calculate arguments passed into the Byte_CRC16 function
   Code_PTR = (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, SI_SEG_CODE))(FLASH_PAGESIZE*START_PAGE);
   Number_Of_Bytes = ((FLASH_PAGESIZE*PAGES_TO_CRC)-2);

   // Manually calculate CRC
   Byte_CRC16 (Code_PTR, Number_Of_Bytes);
   Read_CRC16 (&CRC_Result);

   // Write CRC to last bytes of last page that is used in CRC calculation
   Write_CRC_to_Flash ();

   // Calculate CRC including the CRC value written to the last bytes of last page
   // Since the Block size of the EFM8SB1 is 256 (or half the Flash page size),
   // multiply by two.
   Auto_CRC16 ((2*START_PAGE), (2*PAGES_TO_CRC));
   Read_CRC16 (&CRC_Result);

   // If the CRC was calculated correctly, the resulting CRC will be 0.
   // If the CRC is not 0, then the CRC that was written in the Byte_CRC is
   // incorrect
   LED0 = LED_OFF;

   if (CRC_Result.u16 == 0x00)
   {
     // Success! Start Timer2 to begin blinking the Yellow LED
     TMR2CN0_TR2 = 1;
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
//         address of the first page is (FLASH_PAGESIZE * Start_Page)
//   2) uint8_t Pages_to_CRC - Number of pages in Flash to CRC. This determines the
//         total numbero of pages to CRC. If this is set to 1, the only Flash
//         page that will be included in the CRC is the Start_Page.
//
//   After this function completes, the result is kept in CRC0DAT
//-----------------------------------------------------------------------------
void Auto_CRC16 (uint8_t Start_Page, uint8_t Pages_to_CRC)
{

   // set CRC0SEL to 1 (16-bit calc),
   CRC0CN0 |= 0x10;
   // initialize CRC result to 0x00 and clear the current result.
   // If the result should be initialized to 0xFF, CRC0CN0 |= 0x0C;
   CRC0CN0 |= 0x08;

   // set starting page
   CRC0AUTO = Start_Page;
   // set # pages
   CRC0CNT = Pages_to_CRC;

   // set AUTOEN to enable automatic computation
   CRC0AUTO |= 0x80;

   // write to CRC0CN0 to start computation
   CRC0CN0 &= ~0x03;

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
void Byte_CRC16 (SI_VARIABLE_SEGMENT_POINTER(Start_Byte, uint8_t, SI_SEG_CODE), uint16_t Num_Bytes)
{
   uint16_t i;

   // set CRC0SEL to 1 (16-bit calc),
   CRC0CN0 |= 0x10;
   // initialize CRC result to 0x00 and clear the current result.
   // If the result should be initialized to 0xFF, CRC0CN0 |= 0x0C;
   CRC0CN0 |= 0x08;

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
void Read_CRC16 (SI_UU16_t* Result)
{

   // Initialize CRC0PNT to access bits 7-0 of the CRC result
   CRC0CN0 &= ~0x01;

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
   uint8_t crc_flash_value[2];
   uint16_t end_address;

   // The resulting CRC will be stored in
   // last 2 bytes of the last page of
   // Flash (for 16-bit mode).
   end_address = FLASH_PAGESIZE * (START_PAGE + PAGES_TO_CRC) - 2;
   
   crc_flash_value[0] = CRC_Result.u8[MSB];
   crc_flash_value[1] = CRC_Result.u8[LSB];
   
   FLASH_Update(end_address, crc_flash_value, 2);
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
