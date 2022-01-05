//-----------------------------------------------------------------------------
// F3xx_Flash.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Flash utilities for writing/erasing flash pages
//
//
// How To Test:    See Readme.txt
//
//
// Target:         C8051F320/1,
//                 C8051F326/7,
//                 C8051F34x,
//                 C8051F38x
//
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.6
// Command Line:   See Readme.txt
// Project Name:   USB0_Bulk
//
// Release 2.0
//    -Rewrite (CM)
//    -02 NOV 2012
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "c8051f3xx.h"
#include "F3xx_Flash.h"

//-----------------------------------------------------------------------------
// Static Global Variables
//-----------------------------------------------------------------------------

static uint8_t FlashKey[2];

//-----------------------------------------------------------------------------
// Global Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SetFlashKey
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//                1) uint8_t key[2] : Sets the flash key code used to write/erase
//                               flash
//
// Sets the flash key code for flash writes/erases. The MCU will be more
// resistant to flash corruption if the key codes are not stored on chip.
// For example, the flash key can be set from a USB packet prior to programming
// and then cleared once finished to ensure that the flash key is not available.
//
// Enable flash writes:  key[2] = { 0xA5, 0xF1 }
// Disable flash writes: key[2] = { 0x00, 0x00 }
//
//-----------------------------------------------------------------------------
void SetFlashKey(uint8_t key[2])
{
   FlashKey[0] = key[0];
   FlashKey[1] = key[1];
}

//-----------------------------------------------------------------------------
// EraseFlashPage
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//                1) uint16_t pageAddress : the address of the flash page to erase
//
// Erases the specified flash page
//
//-----------------------------------------------------------------------------
void EraseFlashPage(uint16_t pageAddress)
{
   uint8_t EA_Save = IE_EA;
   SI_VARIABLE_SEGMENT_POINTER(pAddr, uint8_t, SI_SEG_XDATA) = (uint8_t SI_SEG_XDATA *) pageAddress;

   // Disable interrupts
   IE_EA = 0;

   // Write flash key codes
   FLKEY = FlashKey[0];
   FLKEY = FlashKey[1];

   // Enable program erase
   PSCTL = 0x03;

   // Erase page by writing to a byte within the page
   *pAddr = 0x00;

   // Disable program erase
   PSCTL = 0x00;

   // Restore interrupts
   IE_EA = EA_Save;
}

//-----------------------------------------------------------------------------
// WriteFlashPage
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//                1) uint16_t address : the address of the flash page to write
//                2) uint8_t buffer[] : a buffer to write to the flash page starting
//                                 at the specified page address
//                3) uint16_t size : the number of bytes in the buffer to write
//
// Write the specified number of bytes in the buffer to the specified address.
//
//-----------------------------------------------------------------------------
void WriteFlashPage(uint16_t address, uint8_t * buffer, uint16_t size)
{
   uint8_t EA_Save = IE_EA;
   SI_VARIABLE_SEGMENT_POINTER(pAddr, uint8_t, SI_SEG_XDATA) = (uint8_t SI_SEG_XDATA *) address;
   uint16_t i;

   // Disable interrupts
   IE_EA = 0;

   // Enable program writes
   PSCTL = 0x01;

#if FLASH_GROUP_WRITE_EN
   // Enable two-byte flash writes
   PFE0CN |= 0x01;
#endif // FLASH_GROUP_WRITE_EN

   for (i = 0; i < size; i++)
   {
      // Write flash key codes
      FLKEY = FlashKey[0];
      FLKEY = FlashKey[1];

      // Write a single byte to the page
      pAddr[i] = buffer[i];
   }

   // Disable program writes
   PSCTL = 0x00;

   // Restore interrupts
   IE_EA = EA_Save;
}

//-----------------------------------------------------------------------------
// ReadFlashPage
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//                1) uint16_t address : the address of the flash page to read
//                2) uint8_t buffer[] : a buffer to read from the flash page starting
//                                 at the specified page address
//                3) uint16_t size : the number of bytes to read into the buffer
//
// Read the specified number of bytes from flash and store in the buffer.
//
//-----------------------------------------------------------------------------
void ReadFlashPage(uint16_t address, uint8_t * buffer, uint16_t size)
{
   SI_VARIABLE_SEGMENT_POINTER(pAddr, uint8_t, SI_SEG_CODE) = (uint8_t SI_SEG_CODE *) address;
   uint16_t i;

   for (i = 0; i < size; i++)
   {
      buffer[i] = pAddr[i];
   }
}
