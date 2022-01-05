//-----------------------------------------------------------------------------
// EFM8LB1_FlashPrimitives.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// flash memory.
//
// Target:         EFM8LB1
// Tool chain:     Keil C51
//
// Release 1.0
//    -Initial Revision (ST)
//    -19 AUG 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8LB1_Register_Enums.h>
#include "EFM8LB1_FlashPrimitives.h"

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
//   2) uint8_t - byte to write to flash.
//
// This routine writes <byte> to the linear flash address <addr>.
//
//-----------------------------------------------------------------------------
void FLASH_ByteWrite (FLADDR addr, uint8_t byte)
{
   bool EA_SAVE = IE_EA;                // Preserve IE_EA
   SI_VARIABLE_SEGMENT_POINTER(pwrite, uint8_t, SI_SEG_XDATA); // Flash write pointer

   IE_EA = 0;                          // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   pwrite = (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, SI_SEG_XDATA)) addr;

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x01;                      // PSWE = 1 which enables writes

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
   *pwrite = byte;                     // Write the byte

   PSCTL &= ~0x01;                     // PSWE = 0 which disable writes

   IE_EA = EA_SAVE;                    // Restore interrupts
}

//-----------------------------------------------------------------------------
// FLASH_ByteRead
//-----------------------------------------------------------------------------
//
// Return Value :
//      uint8_t - byte read from flash
// Parameters   :
//   1) FLADDR addr - address of the byte to read to
//                    valid range is from 0x0000 to 0x1FFF for 8 kB devices
//                    valid range is from 0x0000 to 0x0FFF for 4 kB devices
//                    valid range is from 0x0000 to 0x07FF for 2 kB devices
//
// This routine reads a <byte> from the linear flash address <addr>.
//
//-----------------------------------------------------------------------------
uint8_t FLASH_ByteRead (FLADDR addr)
{
   bool EA_SAVE = IE_EA;                // Preserve IE_EA
   SI_VARIABLE_SEGMENT_POINTER(pread, uint8_t, const SI_SEG_CODE); // Flash read pointer
   uint8_t byte;

   IE_EA = 0;                          // Disable interrupts

   pread = (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, const SI_SEG_CODE)) addr;

   byte = *pread;                      // Read the byte

   IE_EA = EA_SAVE;                    // Restore interrupts

   return byte;
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
void FLASH_PageErase (FLADDR addr)
{
   bool EA_SAVE = IE_EA;                // Preserve IE_EA
   SI_VARIABLE_SEGMENT_POINTER(pwrite, uint8_t, SI_SEG_XDATA); // Flash write pointer

   IE_EA = 0;                          // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   pwrite = (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, SI_SEG_XDATA)) addr;

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x03;                      // PSWE = 1; PSEE = 1

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
   *pwrite = 0;                        // Initiate page erase

   PSCTL &= ~0x03;                     // PSWE = 0; PSEE = 0

   IE_EA = EA_SAVE;                    // Restore interrupts
}
