//-----------------------------------------------------------------------------
// F93x_FlashUtils.c
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

#include "F93x_FlashPrimitives.h"
#include "F93x_FlashUtils.h"

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

// FLASH read/write/erase routines
void FLASH_Write (FLADDR dest, char *src, uint16_t numbytes, bit SFLE);
char * FLASH_Read (char *dest, FLADDR src, uint16_t numbytes, bit SFLE);
void FLASH_Clear (FLADDR addr, uint16_t numbytes, bit SFLE);

// FLASH update/copy routines
void FLASH_Update (FLADDR dest, char *src, uint16_t numbytes, bit SFLE);
void FLASH_Copy (FLADDR dest, bit destSFLE, FLADDR src, bit srcSFLE,
                 unsigned numbytes);

// FLASH test routines
void FLASH_Fill (FLADDR addr, ULONG length, UCHAR fill, bit SFLE);

//-----------------------------------------------------------------------------
// FLASH_Clear
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - address of the byte to write to
//                    valid range is 0x0000 to 0x79FF for 32K devices
//                    valid range is 0x0000 to 0x3BFF for 16K devices
//   2) uint16_t numbytes - the number of bytes to clear to 0xFF
//                    valid range is 0 to FLASH_PAGESIZE
//   3) SFLE bit - switches between scratchpad and Flash
//
// This routine erases <numbytes> starting from the FLASH addressed by
// <dest> by performing a read-modify-write operation using <FLASH_TEMP> as
// a temporary holding area.
// <addr> + <numbytes> must be less than 0x7FFF/0xFBFF.
//
//-----------------------------------------------------------------------------

void FLASH_Clear (FLADDR dest, unsigned numbytes, bit SFLE)
{
   FLADDR dest_1_page_start;           // first address in 1st page
                                       // containing <dest>
   FLADDR dest_1_page_end;             // last address in 1st page
                                       // containing <dest>
   FLADDR dest_2_page_start;           // first address in 2nd page
                                       // containing <dest>
   FLADDR dest_2_page_end;             // last address in 2nd page
                                       // containing <dest>
   unsigned numbytes_remainder;        // when crossing page boundary,
                                       // number of <src> bytes on 2nd page
   unsigned FLASH_pagesize;            // size of FLASH page to update

   FLADDR  wptr;                       // write address
   FLADDR  rptr;                       // read address

   unsigned length;

   if (SFLE) {                         // update Scratchpad
      FLASH_pagesize = FLASH_SCRATCHSIZE;
   } else {
      FLASH_pagesize = FLASH_PAGESIZE;
   }

   dest_1_page_start = dest & ~(FLASH_pagesize - 1);
   dest_1_page_end = dest_1_page_start + FLASH_pagesize - 1;
   dest_2_page_start = (dest + numbytes)  & ~(FLASH_pagesize - 1);
   dest_2_page_end = dest_2_page_start + FLASH_pagesize - 1;

   if (dest_1_page_end == dest_2_page_end) {

      // 1. Erase Scratch page
      FLASH_PageErase (FLASH_TEMP, 0);

      // 2. Copy bytes from first byte of dest page to dest-1 to Scratch page

      wptr = FLASH_TEMP;
      rptr = dest_1_page_start;
      length = dest - dest_1_page_start;
      FLASH_Copy (wptr, 0, rptr, SFLE, length);

      // 3. Copy from (dest+numbytes) to dest_page_end to Scratch page

      wptr = FLASH_TEMP + dest - dest_1_page_start + numbytes;
      rptr = dest + numbytes;
      length = dest_1_page_end - dest - numbytes + 1;
      FLASH_Copy (wptr, 0, rptr, SFLE, length);

      // 4. Erase destination page
      FLASH_PageErase (dest_1_page_start, SFLE);

      // 5. Copy Scratch page to destination page
      wptr = dest_1_page_start;
      rptr = FLASH_TEMP;
      length = FLASH_pagesize;
      FLASH_Copy (wptr, SFLE, rptr, 0, length);

   } else {                            // value crosses page boundary
      // 1. Erase Scratch page
      FLASH_PageErase (FLASH_TEMP, 0);

      // 2. Copy bytes from first byte of dest page to dest-1 to Scratch page

      wptr = FLASH_TEMP;
      rptr = dest_1_page_start;
      length = dest - dest_1_page_start;
      FLASH_Copy (wptr, 0, rptr, SFLE, length);

      // 3. Erase destination page 1
      FLASH_PageErase (dest_1_page_start, SFLE);

      // 4. Copy Scratch page to destination page 1
      wptr = dest_1_page_start;
      rptr = FLASH_TEMP;
      length = FLASH_pagesize;
      FLASH_Copy (wptr, SFLE, rptr, 0, length);

      // now handle 2nd page

      // 5. Erase Scratch page
      FLASH_PageErase (FLASH_TEMP, 0);

      // 6. Copy bytes from numbytes remaining to dest-2_page_end to Scratch page

      numbytes_remainder = numbytes - (dest_1_page_end - dest + 1);
      wptr = FLASH_TEMP + numbytes_remainder;
      rptr = dest_2_page_start + numbytes_remainder;
      length = FLASH_pagesize - numbytes_remainder;
      FLASH_Copy (wptr, 0, rptr, SFLE, length);

      // 7. Erase destination page 2
      FLASH_PageErase (dest_2_page_start, SFLE);

      // 8. Copy Scratch page to destination page 2
      wptr = dest_2_page_start;
      rptr = FLASH_TEMP;
      length = FLASH_pagesize;
      FLASH_Copy (wptr, SFLE, rptr, 0, length);
   }
}

//-----------------------------------------------------------------------------
// FLASH_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR dest - starting address of the byte(s) to write to
//                    valid range is 0x0000 to 0x7FFF for 32K devices
//                    valid range is 0x0000 to 0xFBFF for 64K devices
//   2) char *src - pointer to source bytes
//   3) uint16_t numbytes - the number of bytes to update
//                              valid range is 0 to FLASH_PAGESIZE
//   4) SFLE bit - switches between scratchpad and Flash
//
// This routine replaces <numbytes> from <src> to the FLASH addressed by
// <dest>.  This function calls FLASH_Clear() to initialize all <dest> bytes
// to 0xff's prior to copying the bytes from <src> to <dest>.
// <dest> + <numbytes> must be less than 0x7FFF/0xFBFF.
//
//-----------------------------------------------------------------------------

void FLASH_Update (FLADDR dest, char *src, unsigned numbytes, bit SFLE)
{
   // 1. Erase <numbytes> starting from <dest>
   FLASH_Clear (dest, numbytes, SFLE);

   // 2. Write <numbytes> from <src> to <dest>
   FLASH_Write (dest, src, numbytes, SFLE);
}


//-----------------------------------------------------------------------------
// FLASH_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR dest - starting address of the byte(s) to write to
//                    valid range is 0x0000 to 0x7FFF for 32K devices
//                    valid range is 0x0000 to 0xFBFF for 64K devices
//   2) char *src - pointer to source bytes
//   3) uint16_t numbytes - the number of bytes to write
//                              valid range is is range of integer
//   4) SFLE bit - switches between scratchpad and Flash
//
// This routine copies <numbytes> from <src> to the linear FLASH address
// <dest>.  The bytes must be erased to 0xFF before writing.
// <dest> + <numbytes> must be less than 0x7FFF/0xFBFF.
//
//-----------------------------------------------------------------------------

void FLASH_Write (FLADDR dest, char *src, unsigned numbytes, bit SFLE)
{
   FLADDR i;

   for (i = dest; i < dest+numbytes; i++) {
      FLASH_ByteWrite (i, *src++, SFLE);
   }
}

//-----------------------------------------------------------------------------
// FLASH_Read
//-----------------------------------------------------------------------------
//
// Return Value : None
//   1) char *dest - pointer to destination bytes
// Parameters   :
//   1) char *dest - pointer to destination bytes
//   2) FLADDR src - address of source bytes in Flash
//                    valid range is 0x0000 to 0x7FFF for 32K devices
//                    valid range is 0x0000 to 0xFBFF for 64K devices
//   3) uint16_t numbytes - the number of bytes to read
//                              valid range is range of integer
//   4) SFLE bit - switches between scratchpad and Flash
//
// This routine copies <numbytes> from the linear FLASH address <src> to
// <dest>.
// <src> + <numbytes> must be less than 0x7FFF/0xFBFF.
//
//-----------------------------------------------------------------------------

char * FLASH_Read (char *dest, FLADDR src, unsigned numbytes, bit SFLE)
{
   FLADDR i;

   for (i = 0; i < numbytes; i++) {
      *dest++ = FLASH_ByteRead (src+i, SFLE);
   }
   return dest;
}

//-----------------------------------------------------------------------------
// FLASH_Copy
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR dest - pointer to destination bytes in Flash
//                    valid range is 0x0000 to 0x7FFF for 32K devices
//                    valid range is 0x0000 to 0xFBFF for 64K devices
//   2) FLADDR src - address of source bytes in Flash
//                    valid range is 0x0000 to 0x7FFF for 32K devices
//                    valid range is 0x0000 to 0xFBFF for 64K devices
//   3) uint16_t numbytes - the number of bytes to copy
//                              valid range is range of integer
//   4) SFLE bit - switches between scratchpad and Flash
//
// This routine copies <numbytes> from <src> to the linear FLASH address
// <dest>.  The destination bytes must be erased to 0xFF before writing.
// <src>/<dest> + <numbytes> must be less than 0x7FFF/0xFBFF.
//
//-----------------------------------------------------------------------------

void FLASH_Copy (FLADDR dest, bit destSFLE, FLADDR src, bit srcSFLE,
                 unsigned numbytes)
{
   FLADDR i;

   for (i = 0; i < numbytes; i++) {

      FLASH_ByteWrite ((FLADDR) dest+i,
                       FLASH_ByteRead((FLADDR) src+i, srcSFLE),
                       destSFLE);
   }
}

//-----------------------------------------------------------------------------
// FLASH_Fill
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - starting address of bytes to fill in Flash
//                    valid range is 0x0000 to 0x7FFF for 32K devices
//                    valid range is 0x0000 to 0xFBFF for 64K devices
//   2) ULONG length - number of bytes to fill
//                     range is total Flash size
//   3) UCHAR fill - the character used the Flash should be filled with
//   3) SFLE bit - switches between scratchpad and Flash
//
// This routine fills the FLASH beginning at <addr> with <length> bytes.
// The target bytes must be erased before writing to them.
// <addr> + <length> must be less than 0x7FFF/0xFBFF.
//
//-----------------------------------------------------------------------------

void FLASH_Fill (FLADDR addr, ULONG length, uint8_t fill, bit SFLE)
{
   FLADDR i;

   for (i = 0; i < length; i++) {
      FLASH_ByteWrite (addr+i, fill, SFLE);
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------