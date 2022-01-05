//-----------------------------------------------------------------------------
// F93x_FlashPrimitives.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// FLASH memory.
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Silicon Labs Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 15 JAN 2014
//
// Release 1.0
//    -Initial Revision (TP)
//    -14 JAN 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F930_Register_Enums.h>
#include "F93x_FlashPrimitives.h"

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void          FLASH_ByteWrite (FLADDR addr, char byte, bit SFLE);
uint8_t FLASH_ByteRead  (FLADDR addr, bit SFLE);
void          FLASH_PageErase (FLADDR addr, bit SFLE);

//-----------------------------------------------------------------------------
// FLASH_ByteWrite
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - address of the byte to write to
//                    valid range is from 0x0000 to 0x7DFE for 32K devices
//                    valid range is from 0x0000 to 0x3FFE for 16K devices
//   2) char byte - byte to write to Flash.
//   3) SFLE bit - switches between scratchpad and Flash
//
// This routine writes <byte> to the linear FLASH address <addr>.
//-----------------------------------------------------------------------------

void FLASH_ByteWrite (FLADDR addr, char byte, bit SFLE)
{
   bit EA_SAVE = IE_EA;                   // Preserve IE_EA
   char xdata * data pwrite;           // FLASH write pointer

   IE_EA = 0;                             // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   pwrite = (char xdata *) addr;

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x01;                      // PSWE = 1 which enables writes

   VDM0CN = 0x80;                      // Enable VDD monitor

   if (SFLE) {
      PSCTL |= 0x04;                   // Set SFLE
   }

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
   *pwrite = byte;                     // Write the byte

   if (SFLE) {
      PSCTL &= ~0x04;                  // Clear SFLE
   }

   PSCTL &= ~0x01;                     // PSWE = 0 which disable writes

   IE_EA = EA_SAVE;                       // Restore interrupts
}

//-----------------------------------------------------------------------------
// FLASH_ByteRead
//-----------------------------------------------------------------------------
//
// Return Value :
//      uint8_t - byte read from Flash
// Parameters   :
//   1) FLADDR addr - address of the byte to read to
//                    valid range is from 0x0000 to 0x7DFE for 32K devices
//                    valid range is from 0x0000 to 0x3FFE for 16K devices
//
// This routine reads a <byte> from the linear FLASH address <addr>.
//-----------------------------------------------------------------------------

uint8_t FLASH_ByteRead (FLADDR addr, bit SFLE)
{
   bit EA_SAVE = IE_EA;                   // Preserve IE_EA
   char code * data pread;             // FLASH read pointer
   uint8_t byte;

   IE_EA = 0;                             // Disable interrupts

   pread = (char code *) addr;

   if (SFLE) {
      PSCTL |= 0x04;                   // set SFLE
   }

   byte = *pread;                      // Read the byte

   if (SFLE) {
      PSCTL &= ~0x04;                  // clear SFLE
   }

   IE_EA = EA_SAVE;                       // Restore interrupts

   return byte;
}

//-----------------------------------------------------------------------------
// FLASH_PageErase
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - address of any byte in the page to erase
//                    valid range is from 0x0000 to 0x7BFF for 32K devices
//                    valid range is from 0x0000 to 0x3DFF for 16K devices
//
// This routine erases the FLASH page containing the linear FLASH address
// <addr>.  Note that the page of Flash containing the Lock Byte cannot be
// erased if the Lock Byte is set.
//
//-----------------------------------------------------------------------------

void FLASH_PageErase (FLADDR addr, bit SFLE)
{
   bit EA_SAVE = IE_EA;                   // Preserve IE_EA
   char xdata * data pwrite;           // FLASH write pointer

   IE_EA = 0;                             // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   pwrite = (char xdata *) addr;

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x03;                      // PSWE = 1; PSEE = 1


   VDM0CN = 0x80;                      // Enable VDD monitor

   if (SFLE) {
      PSCTL |= 0x04;                   // Set SFLE
   }

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
   *pwrite = 0;                        // Initiate page erase

   if (SFLE) {
      PSCTL &= ~0x04;                  // Clear SFLE
   }

   PSCTL &= ~0x03;                     // PSWE = 0; PSEE = 0

   IE_EA = EA_SAVE;                       // Restore interrupts
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------