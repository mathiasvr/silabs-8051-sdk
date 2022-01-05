//-----------------------------------------------------------------------------
// F380_FlashUtils.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// FLASH memory.
//
// Target:         C8051F38x
// Tool chain:     KEIL C51
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (ST)
//    -06 AUG 2014
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "F380_FlashPrimitives.h"
#include "F380_FlashUtils.h"

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

// FLASH read/write/erase routines
void FLASH_Write (FLADDR dest, char *src, unsigned int numbytes);
char * FLASH_Read (char *dest, FLADDR src, unsigned int numbytes);
void FLASH_Clear (FLADDR addr, unsigned int numbytes);

// FLASH update/copy routines
void FLASH_Update (FLADDR dest, char *src, unsigned int numbytes);
void FLASH_Copy (FLADDR dest, FLADDR src, unsigned int numbytes);

// FLASH test routines
void FLASH_Fill (FLADDR addr, ULONG length, UCHAR fill);

//-----------------------------------------------------------------------------
// FLASH_Clear
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - address of the byte to write to
//                    valid range is 0x0000 to 0xF9FF for 64K devices
//                    valid range is 0x0000 to 0x7DFF for 32K devices
//                    valid range is 0x0000 to 0x3DFF for 16K devices
//   2) unsigned int numbytes - the number of bytes to clear to 0xFF
//                    valid range is 0 to FLASH_PAGESIZE
//
// This routine erases <numbytes> starting from the FLASH addressed by
// <dest> by performing a read-modify-write operation using <FLASH_TEMP> as
// a temporary holding area.
// <addr> + <numbytes> must be less than 0xF9FF/0x7DFF/0x3DFF.
//
//-----------------------------------------------------------------------------

void FLASH_Clear (FLADDR dest, unsigned int numbytes)
{
   FLADDR dest_1_page_start;           // First address in 1st page
                                       // containing <dest>
   FLADDR dest_1_page_end;             // Last address in 1st page
                                       // containing <dest>
   FLADDR dest_2_page_start;           // First address in 2nd page
                                       // containing <dest>
   FLADDR dest_2_page_end;             // Last address in 2nd page
                                       // containing <dest>
   unsigned numbytes_remainder;        // When crossing page boundary,
                                       // number of <src> bytes on 2nd page
   unsigned FLASH_pagesize;            // Size of FLASH page to update

   FLADDR  wptr;                       // Write address
   FLADDR  rptr;                       // Read address

   unsigned length;

   FLASH_pagesize = FLASH_PAGESIZE;

   dest_1_page_start = dest & ~(FLASH_pagesize - 1);
   dest_1_page_end   = dest_1_page_start + FLASH_pagesize - 1;
   dest_2_page_start = (dest + numbytes)  & ~(FLASH_pagesize - 1);
   dest_2_page_end   = dest_2_page_start + FLASH_pagesize - 1;

   if (dest_1_page_end == dest_2_page_end)
   {
      // 1. Erase Scratch page
      FLASH_PageErase (FLASH_TEMP);

      // 2. Copy bytes from first byte of dest page to dest-1 to Scratch page

      wptr = FLASH_TEMP;
      rptr = dest_1_page_start;
      length = dest - dest_1_page_start;
      FLASH_Copy (wptr, rptr, length);

      // 3. Copy from (dest+numbytes) to dest_page_end to Scratch page

      wptr = FLASH_TEMP + dest - dest_1_page_start + numbytes;
      rptr = dest + numbytes;
      length = dest_1_page_end - dest - numbytes + 1;
      FLASH_Copy (wptr, rptr, length);

      // 4. Erase destination page
      FLASH_PageErase (dest_1_page_start);

      // 5. Copy Scratch page to destination page
      wptr = dest_1_page_start;
      rptr = FLASH_TEMP;
      length = FLASH_pagesize;
      FLASH_Copy (wptr, rptr, length);

   }
   else
   {                            // value crosses page boundary
      // 1. Erase Scratch page
      FLASH_PageErase (FLASH_TEMP);

      // 2. Copy bytes from first byte of dest page to dest-1 to temp page

      wptr = FLASH_TEMP;
      rptr = dest_1_page_start;
      length = dest - dest_1_page_start;
      FLASH_Copy (wptr, rptr, length);

      // 3. Erase destination page 1
      FLASH_PageErase (dest_1_page_start);

      // 4. Copy Scratch page to destination page 1
      wptr = dest_1_page_start;
      rptr = FLASH_TEMP;
      length = FLASH_pagesize;
      FLASH_Copy (wptr, rptr, length);

      // Now handle 2nd page

      // 5. Erase Scratch page
      FLASH_PageErase (FLASH_TEMP);

      // 6. Copy bytes from numbytes remaining to dest-2_page_end to temp page

      numbytes_remainder = numbytes - (dest_1_page_end - dest + 1);
      wptr = FLASH_TEMP + numbytes_remainder;
      rptr = dest_2_page_start + numbytes_remainder;
      length = FLASH_pagesize - numbytes_remainder;
      FLASH_Copy (wptr, rptr, length);

      // 7. Erase destination page 2
      FLASH_PageErase (dest_2_page_start);

      // 8. Copy Scratch page to destination page 2
      wptr = dest_2_page_start;
      rptr = FLASH_TEMP;
      length = FLASH_pagesize;
      FLASH_Copy (wptr, rptr, length);
   }
}

//-----------------------------------------------------------------------------
// FLASH_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR dest - starting address of the byte(s) to write to
//                    valid range is 0x0000 to 0xFBFF for 64K devices
//                    valid range is 0x0000 to 0x7FFF for 32K devices
//                    valid range is 0x0000 to 0x3FFF for 16K devices
//   2) char *src - pointer to source bytes
//   3) unsigned int numbytes - the number of bytes to update
//                              valid range is 0 to FLASH_PAGESIZE
//
// This routine replaces <numbytes> from <src> to the FLASH addressed by
// <dest>.  This function calls FLASH_Clear() to initialize all <dest> bytes
// to 0xff's prior to copying the bytes from <src> to <dest>.
// <dest> + <numbytes> must be less than 0xFBFF/0x7DFF/0x3FFF.
//
//-----------------------------------------------------------------------------

void FLASH_Update (FLADDR dest, char *src, unsigned int numbytes)
{
   // 1. Erase <numbytes> starting from <dest>
   FLASH_Clear (dest, numbytes);

   // 2. Write <numbytes> from <src> to <dest>
   FLASH_Write (dest, src, numbytes);
}


//-----------------------------------------------------------------------------
// FLASH_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR dest - starting address of the byte(s) to write to
//                    valid range is 0x0000 to 0xFBFE for 64K Flash devices
//                    valid range is 0x0000 to 0x7FFE for 32K Flash devices
//                    valid range is 0x0000 to 0x3FFE for 16K Flash devices
//   2) char *src - pointer to source bytes
//   3) unsigned int numbytes - the number of bytes to write
//                              valid range is is range of integer
//
// This routine copies <numbytes> from <src> to the linear FLASH address
// <dest>.  The bytes must be erased to 0xFF before writing.
// <dest> + <numbytes> must be less than 0xFBFF/0x7FFF/0x3FFF.
//
//-----------------------------------------------------------------------------

void FLASH_Write (FLADDR dest, char *src, unsigned int numbytes)
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
// Return Value : None
//   1) char *dest - pointer to destination bytes
// Parameters   :
//   1) char *dest - pointer to destination bytes
//   2) FLADDR src - address of source bytes in Flash
//                   valid range is 0x0000 to 0x7BFF for 64K Flash devices
//                   valid range is 0x0000 to 0x7FFF for 32K Flash devices
//                   valid range is 0x0000 to 0x3FFF for 16K Flash devices
//   3) unsigned int numbytes - the number of bytes to read
//                              valid range is range of integer
//
// This routine copies <numbytes> from the linear FLASH address <src> to
// <dest>.
// <src> + <numbytes> must be less than 0x7BFF/0x7DFF/0x3FFF.
//
//-----------------------------------------------------------------------------

char * FLASH_Read (char *dest, FLADDR src, unsigned int numbytes)
{
   FLADDR i;

   for (i = 0; i < numbytes; i++) {
      *dest++ = FLASH_ByteRead (src+i);
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
//                   valid range is 0x0000 to 0xFBFF for 64K Flash devices
//                   valid range is 0x0000 to 0x7FFF for 32K Flash devices
//                   valid range is 0x0000 to 0x3FFF for 16K Flash devices
//   2) FLADDR src - address of source bytes in Flash
//                   valid range is 0x0000 to 0xFBFF for 64K Flash devices
//                   valid range is 0x0000 to 0x7FFF for 32K Flash devices
//                   valid range is 0x0000 to 0x3FFF for 16K Flash devices
//   3) unsigned int numbytes - the number of bytes to copy
//                              valid range is range of integer
//
// This routine copies <numbytes> from <src> to the linear FLASH address
// <dest>.  The destination bytes must be erased to 0xFF before writing.
// <src>/<dest> + <numbytes> must be less than 0xFBFF/0x7FFF/0x3FFF.
//
//-----------------------------------------------------------------------------

void FLASH_Copy (FLADDR dest, FLADDR src, unsigned int numbytes)
{
   FLADDR i;

   for (i = 0; i < numbytes; i++) {
      FLASH_ByteWrite ((FLADDR) dest+i, FLASH_ByteRead((FLADDR) src+i));
   }
}

//-----------------------------------------------------------------------------
// FLASH_Fill
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - starting address of bytes to fill in Flash
//                   valid range is 0x0000 to 0xFBFF for 64K Flash devices
//                   valid range is 0x0000 to 0x7FFF for 32K Flash devices
//                   valid range is 0x0000 to 0x3FFF for 16K Flash devices
//   2) ULONG length - number of bytes to fill
//                     range is total Flash size
//   3) UCHAR fill - the character used the Flash should be filled with
//
// This routine fills the FLASH beginning at <addr> with <length> bytes.
// The target bytes must be erased before writing to them.
// <addr> + <length> must be less than 0xFBFF/0x7DFF/0x3FFF.
//
//-----------------------------------------------------------------------------

void FLASH_Fill (FLADDR addr, ULONG length, UCHAR fill)
{
   FLADDR i;

   for (i = 0; i < length; i++) {
      FLASH_ByteWrite (addr+i, fill);
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
