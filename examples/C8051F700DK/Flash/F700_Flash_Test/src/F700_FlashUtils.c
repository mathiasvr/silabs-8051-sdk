//-----------------------------------------------------------------------------
// F700_FlashUtils.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// FLASH memory.
//
// Target:         C8051F700/1/2/3/4/5/6/7/8/9/10/11
// Tool chain:     Simplicity Studio / Keil C51 9.53
// Command Line:   None
//
//
// Release 1.1 / 02 MAR 2010 (GP)
//    -Tested with Raisonance
//    -Updated to meet new data sheet guidelines for writing/erasing
//
// Release 1.0
//    -Initial Revision (GP)
//    -08 APR 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "si_toolchain.h"
#include <SI_c8051F700_defs.h>
#include "F700_FlashPrimitives.h"
#include "F700_FlashUtils.h"



//-----------------------------------------------------------------------------
// FLASH_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR dest - starting address of the byte(s) to write to
//                    valid range is 0x0000 to 0xFBFF for 64K Flash devices
//                    valid range is 0x0000 to 0x7FFF for 32K Flash devices
//   2) uint8_t *src      - pointer to source bytes
//   3) uint16_t numbytes - the number of bytes to write
//                              valid range is is range of integer
//
// This routine copies <numbytes> from <src> to the linear FLASH address
// <dest>.  The bytes must be erased to 0xFF before writing.
// <dest> + <numbytes> must be less than 0xFBFF/0x7FFF
//
//-----------------------------------------------------------------------------

void FLASH_Write (FLADDR dest, uint8_t *src, uint16_t numbytes)
{
   FLADDR i;

   for (i = dest; i < dest+numbytes; i++) {
      FLASH_ByteWrite (i, *src++);
   }
}

//-----------------------------------------------------------------------------
// FLASH_Read
//-----------------------------------------------------------------------------
//
// Return Value :
//   1) uint8_t *dest - pointer to destination bytes
// Parameters   :
//   1) uint8_t *dest - pointer to destination bytes
//   2) FLADDR src - address of source bytes in Flash
//                    valid range is 0x0000 to 0xFBFF for 64K Flash devices
//                    valid range is 0x0000 to 0x7FFF for 32K Flash devices
//   3) uint16_t numbytes - the number of bytes to read
//                              valid range is range of integer
//
// This routine copies <numbytes> from the linear FLASH address <src> to
// <dest>. 
// <dest> + <numbytes> must be less than 0xFBFF/0x7FFF
//
//-----------------------------------------------------------------------------

uint8_t * FLASH_Read (uint8_t *dest, FLADDR src, uint16_t numbytes)
{
   FLADDR i;

   for (i = 0; i < numbytes; i++) {
      *dest++ = FLASH_ByteRead (src+i);
   }
   return dest;
}

//-----------------------------------------------------------------------------
// FLASH_Fill
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - starting address of bytes to fill in Flash
//                    valid range is 0x0000 to 0xFBFF for 64K Flash devices
//                    valid range is 0x0000 to 0x7FFF for 32K Flash devices
//   2) ULONG length - number of bytes to fill
//                     range is total Flash size
//   3) uint8_t fill - the character used the Flash should be filled with
//
// This routine fills the FLASH beginning at <addr> with <length> bytes.
// The target bytes must be erased before writing to them.
// <addr> + <length> must be less than 0xFBFF/0x7FFF.
//
//-----------------------------------------------------------------------------

void FLASH_Fill (FLADDR addr, uint16_t length, uint8_t fill)
{
   FLADDR i;

   for (i = 0; i < length; i++) {
      FLASH_ByteWrite (addr+i, fill);
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
