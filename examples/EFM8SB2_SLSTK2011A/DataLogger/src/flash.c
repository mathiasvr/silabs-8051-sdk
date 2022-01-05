/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// Flash.c
/////////////////////////////////////////////////////////////////////////////

#include "flash.h"

//-----------------------------------------------------------------------------
// Flash Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FLASH_ByteRead
//-----------------------------------------------------------------------------
//
// Return Value :
//      unsigned char - byte read from Flash
// Parameters   :
//   1) FLADDR addr - address of the byte to read to
//                    valid range is from 0x0000 to 0xFBFF for 64K devices
//                    valid range is from 0x0000 to 0x7FFF for 32K devices
//
// This routine reads a <byte> from the linear FLASH address <addr>.
//-----------------------------------------------------------------------------
uint8_t FLASH_ByteRead (uint16_t addr)
{
   uint8_t EA_Save = IE;                    // Preserve EA

   SI_VARIABLE_SEGMENT_POINTER (pread, uint8_t, const SI_SEG_CODE);

   unsigned char byte;

   IE_EA = 0;                             // Disable interrupts

   pread = (SI_VARIABLE_SEGMENT_POINTER (, uint8_t, const SI_SEG_CODE)) addr;

   byte = *pread;                      // Read the byte

   PSCTL &= ~0x04;                     // SFLE = 0

   if ((EA_Save & 0x80) != 0)          // Restore EA
   {
      IE_EA = 1;
   }

   return byte;

}
//-----------------------------------------------------------------------------
// FLASH_ByteWrite
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - address of the byte to write to
//                    valid range is from 0x0000 to 0xFBFF for 64K devices
//                    valid range is from 0x0000 to 0x7FFF for 32K devices
//   2) char byte - byte to write to Flash.
//
// This routine writes <byte> to the linear FLASH address <addr>.
//-----------------------------------------------------------------------------
void FLASH_ByteWrite (uint16_t addr, uint8_t byte)
{
   uint8_t EA_Save = IE;               // Preserve EA
   SI_VARIABLE_SEGMENT_POINTER (pwrite, uint8_t, SI_SEG_XDATA);

   IE_EA = 0;                          // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor


   RSTSRC = 0x06;                      // Enable VDD monitor as a reset source
                                       // Leave missing clock detector enabled

   pwrite = (SI_VARIABLE_SEGMENT_POINTER (, uint8_t, SI_SEG_XDATA)) addr;

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x01;                      // PSWE = 1

   VDM0CN = 0x80;                      // Enable VDD monitor


   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   *pwrite = byte;                     // Write the byte

   PSCTL &= ~0x05;                     // SFLE = 0; PSWE = 0

   if ((EA_Save & 0x80) != 0)          // Restore EA
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
//                    valid range is from 0x0000 to 0xFBFF for 64K devices
//                    valid range is from 0x0000 to 0x7FFF for 32K devices
//
// This routine erases the FLASH page containing the linear FLASH address
// <addr>.  Note that the page of Flash containing the Lock Byte cannot be
// erased if the Lock Byte is set.
//
//-----------------------------------------------------------------------------
void FLASH_PageErase (uint16_t addr)
{
   uint8_t EA_Save = IE;               // Preserve EA

   SI_VARIABLE_SEGMENT_POINTER (pwrite, uint8_t, SI_SEG_XDATA);

   IE_EA = 0;                          // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x06;                      // Enable VDD monitor as a reset source
                                       // Leave missing clock detector enabled

   pwrite = (SI_VARIABLE_SEGMENT_POINTER (, uint8_t, SI_SEG_XDATA)) addr;

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x03;                      // PSWE = 1; PSEE = 1

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
   *pwrite = 0;                        // Initiate page erase

   PSCTL &= ~0x07;                     // SFLE = 0; PSWE = 0; PSEE = 0

   if ((EA_Save & 0x80) != 0)          // Restore EA
   {
       IE_EA = 1;
   }
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
