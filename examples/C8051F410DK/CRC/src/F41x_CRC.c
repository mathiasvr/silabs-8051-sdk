//-----------------------------------------------------------------------------
//F41x_CRC.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program shows how to use the built-in CRC engine.
//
// This program calculates the 32-bit CRC of the number of Flash pages found in
// the global constant section. First, the CRC of the flash pages, minus the
// last four bytes, is calculated. This CRC is this written to the last four
// bytes of the last flash page. Then, the CRC is calculated again, this time
// covering all of the bytes of the flash pages. This final result should be
// zero if the CRC engine is working properly.
//
// This example uses the 32-bit mode CRC, but functions for the 16-bit CRC are
// included.
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    J9:   VUNREG (9V adapter) or 5VEC3 (Debugger) 
//    J10:  +3VD/+3VD
//    J12:  +3VD/VREGIN
//          VREGIN_EN
//    J17:  5VEC3/VIO or +3VD/VIO
//          VIO_EN
//    J5:   P2.1/D3
// 2) Connect the USB Debug Adapter to J4. If VUNREG is selected on J9,
//    connect the 9V power adapter to P1.
// 3) On the target board, connect P0.0 to P1.4 and P0.1 to P2.1
// 4) Compile and download code to the C8051F410-TB development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in the
//    quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) If the LED D3 is On, the CRC was calculated correctly.
//
//
//
// Target:         C8051F41x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 specific code with compiler agnostic code from
//      compiler_defs.h.
//    - Added the functions to write the CRC to flash and then re-calculate the
//       total CRC, and turn the LED on / off based on whether the CRC
//       generated is correctly zero.
//    - 21 JAN 2014
//
// Release 1.0
//    -Initial Revision (ES)
//    -10 JAN 2011
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F410_Register_Enums.h>

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(GREEN_LED, SFR_P2, 1);          // '1' means ON, '0' means OFF

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000 / 64     // SYSCLK frequency in Hz

#define LED_ON            1
#define LED_OFF           0

#define FLASH_PAGE_SIZE   512          // Flash page size in Bytes
#define START_PAGE        8            // Index of the Flash page to start the
                                       // auto calculation. The starting
                                       // address can be calculated by
                                       // FLASH_PAGE_SIZE * START_PAGE

#define PAGES_TO_CRC   1               // defines the number of pages to CRC

SI_UU32_t CRC_Result;                       // Will store the CRC results

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
void PORT_Init (void);

void Byte_CRC16 (uint8_t * Start_Byte, uint16_t Num_Bytes);
void Byte_CRC32 (uint8_t * Start_Byte, uint16_t Num_Bytes);
void Read_CRC16 (SI_UU16_t * Result);
void Read_CRC32 (SI_UU32_t * Result);

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
   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{

                                       // enable)
   CRC_Result.u32 = 0x00000000;
   PORT_Init ();

   GREEN_LED = LED_OFF;

   // Calculate arguments passed into the Byte_CRC32 or Byte_CRC16 function
   Code_PTR =(FLASH_PAGE_SIZE*START_PAGE);
   Number_Of_Bytes = ((FLASH_PAGE_SIZE*PAGES_TO_CRC));

   // Manually calculate CRC
   Byte_CRC32 (Code_PTR, Number_Of_Bytes - 4);
   Read_CRC32 (&CRC_Result);

   // Write CRC to last bytes of last page that is used in CRC calculation
   Write_CRC_to_Flash ();

   // Calculate CRC including the CRC value written to the last bytes of last page
   Byte_CRC32 (Code_PTR, Number_Of_Bytes);
   Read_CRC32 (&CRC_Result);

   // If the CRC was calculated correctly, the resulting CRC will be 0.
   // If the CRC is not 0, then the CRC that was written in the Byte_CRC is incorrect
   if (CRC_Result.u32 == 0x00)
   {
      GREEN_LED = LED_ON;
   }
   else
   {
      GREEN_LED = LED_OFF;
   }

   while (1) {}                        // Spin forever
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P2.1   digital   push-pull     Green LED
//-----------------------------------------------------------------------------
void PORT_Init (void)
{

   P2MDIN |= 0x02;                     // P2.1 are digital

   P2MDOUT |= 0x02;                    // P2.1 are push-pull

   XBR1    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
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
// Byte_CRC32
//-----------------------------------------------------------------------------
//
// Return Value:  uint8_t - returns value indicating if the App FW is valid.
//   1) uint8_t* Start_Byte - Pointer to first byte of data to CRC
//   2) uint16_t Num_Bytes - Number of consecutive bytes to include in the CRC
//
//   After this function completes, the result is kept in CRC0DAT
//-----------------------------------------------------------------------------
void Byte_CRC32 (uint8_t * Start_Byte, uint16_t Num_Bytes)
{

   uint16_t i;

   // clear CRC0SEL to 0 (32-bit calc),
   CRC0CN &= ~0x10;
   // initialize CRC result to 0x00 and clear the current result.
   // If the result should be initialized to 0xFF, CRC0CN |= 0x0C;
   CRC0CN |= 0x08;

   for (i = 0; i < Num_Bytes; i++)
   {
      CRC0IN = Start_Byte[i];
   }

   CRC0CN &= ~0x03;

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
// Read_CRC32
//-----------------------------------------------------------------------------
//
// Return Value : None.
// Parameters   :
//   1) uint8_t* Result - Stores CRC into 4 bytes starting at this location
//
// Copies CRC result to four bytes starting at location passed into function
//-----------------------------------------------------------------------------
void Read_CRC32 (SI_UU32_t * Result)
{

   // Initialize CRC0PNT to access bits 7-0 of the CRC result
   CRC0CN &= ~0x03;

   // With each read of CRC0DAT, the value in CRC0PNT will auto-increment
   (*Result).u8[B0] = CRC0DAT;
   (*Result).u8[B1] = CRC0DAT;
   (*Result).u8[B2] = CRC0DAT;
   (*Result).u8[B3] = CRC0DAT;

}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------

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

   end_address -= 4;                   // The resulting CRC will be stored in
                                       // last 4 bytes of the last page of
                                       // Flash (for 32-bit mode).
   FLASH_SetKeys();
   FLASH_ByteWrite (end_address++, CRC_Result.u8[B0]);
   FLASH_SetKeys();
   FLASH_ByteWrite (end_address++, CRC_Result.u8[B1]);
   FLASH_SetKeys();
   FLASH_ByteWrite (end_address++, CRC_Result.u8[B2]);
   FLASH_SetKeys();
   FLASH_ByteWrite (end_address++, CRC_Result.u8[B3]);
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
