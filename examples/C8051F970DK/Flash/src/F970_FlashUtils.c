//-----------------------------------------------------------------------------
// F970_FlashUtils.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// This program contains several useful utilities for writing and updating
// FLASH memory.
//
//
// Target:         C8051F97x
// Tool chain:     Keil
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 5 MAY 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "F970_FlashPrimitives.h"

//-----------------------------------------------------------------------------
// Structures, Unions, Enumerations, and Type Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

// FLASH read/write/erase routines
void FLASH_Write (FLADDR dest, uint8_t *src, uint16_t numbytes, bit SFLE);
uint8_t * FLASH_Read  (uint8_t *dest, FLADDR src, uint16_t numbytes, bit SFLE);
void FLASH_Clear (FLADDR addr, uint16_t numbytes, bit SFLE);

// FLASH update/copy routines
void FLASH_Update (FLADDR dest, uint8_t *src, uint16_t numbytes, bit SFLE);
void FLASH_Copy   (FLADDR dest, bit destSFLE, FLADDR src, bit srcSFLE,
                   uint16_t numbytes);

// FLASH test routines
void FLASH_Fill (FLADDR addr, uint32_t length, uint8_t fill, bit SFLE);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FLASH Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FLASH_Clear
//-----------------------------------------------------------------------------
//
// This routine erases <numbytes> starting from the FLASH addressed by
// <dest> by performing a read-modify-write operation using <FLASH_TEMP> as
// a temporary holding area.  This function accepts <numbytes> up to
// <FLASH_PAGESIZE>.
//
//-----------------------------------------------------------------------------
void FLASH_Clear (FLADDR dest, uint16_t numbytes, bit SFLE)
{
   FLADDR dest_1_page_start;           // First address in 1st page
                                       // containing <dest>
   FLADDR dest_1_page_end;             // Last address in 1st page
                                       // containing <dest>
   FLADDR dest_2_page_start;           // First address in 2nd page
                                       // containing <dest>
   FLADDR dest_2_page_end;             // Last address in 2nd page
                                       // containing <dest>
   uint16_t numbytes_remainder;             // When crossing page boundary,
                                       // number of <src> bytes on 2nd page
   uint16_t FLASH_pagesize;                 // Size of FLASH page to update

   FLADDR  wptr;                       // Write address
   FLADDR  rptr;                       // Read address

   uint16_t length;

   if (SFLE) {                         // Update Scratchpad
      FLASH_pagesize = FLASH_SCRATCHSIZE;
   } else {
      FLASH_pagesize = FLASH_PAGESIZE;
   }

   dest_1_page_start = dest & (0x10000 | ~(FLASH_pagesize - 1));
   dest_1_page_end = dest_1_page_start + FLASH_pagesize - 1;
   dest_2_page_start = (dest + numbytes)  & (0x10000 | ~(FLASH_pagesize - 1));
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
// This routine replaces <numbytes> from <src> to the FLASH addressed by
// <dest>.  This function calls FLASH_Clear() to handle the dirty work of
// initializing all <dest> bytes to 0xff's prior to copying the bytes from
// <src> to <dest>. This function accepts <numbytes> up to <FLASH_PAGESIZE>.
//
//-----------------------------------------------------------------------------
void FLASH_Update (FLADDR dest, uint8_t *src, uint16_t numbytes, bit SFLE)
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
// This routine copies <numbytes> from <src> to the linear FLASH address
// <dest>.
//
//-----------------------------------------------------------------------------
void FLASH_Write (FLADDR dest, uint8_t *src, uint16_t numbytes, bit SFLE)
{
   uint32_t i;

   for (i = dest; i < dest+numbytes; i++) {
      FLASH_ByteWrite (i, *src++, SFLE);
   }
}

//-----------------------------------------------------------------------------
// FLASH_Read
//-----------------------------------------------------------------------------
//
// This routine copies <numbytes> from the linear FLASH address <src> to
// <dest>.
//
//-----------------------------------------------------------------------------
uint8_t * FLASH_Read (uint8_t *dest, FLADDR src, uint16_t numbytes, bit SFLE)
{
   uint32_t i;

   for (i = 0; i < numbytes; i++) {
      *dest++ = FLASH_ByteRead (src+i, SFLE);
   }
   return dest;
}

//-----------------------------------------------------------------------------
// FLASH_Copy
//-----------------------------------------------------------------------------
//
// This routine copies <numbytes> from <src> to the linear FLASH address
// <dest>.
//
//-----------------------------------------------------------------------------
void FLASH_Copy (FLADDR dest, bit destSFLE, FLADDR src, bit srcSFLE,
                 uint16_t numbytes)
{
   uint32_t i;

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
// This routine fills the FLASH beginning at <addr> with <lenght> bytes.
//
//-----------------------------------------------------------------------------
void FLASH_Fill (FLADDR addr, uint32_t length, uint8_t fill, bit SFLE)
{
   uint32_t i;

   for (i = 0; i < length; i++) {
      FLASH_ByteWrite (addr+i, fill, SFLE);
   }
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
