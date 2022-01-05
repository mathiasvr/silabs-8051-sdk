//-----------------------------------------------------------------------------
// F700_FlashPrimitives.c
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

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

uint8_t  FLASH_ByteWrite (FLADDR addr, uint8_t byte);
uint8_t  FLASH_ByteRead  (FLADDR addr);
uint8_t  FLASH_PageErase (FLADDR addr);

//-----------------------------------------------------------------------------
// FLASH_ByteWrite
//-----------------------------------------------------------------------------
//
// Return Value :
//   0 if Write unsuccessful, 1 if Write successful
//
// Parameters   :
//   1) FLADDR addr - address of the byte to write to
//                    valid range is 0x0000 to 0xFBFF for 64K Flash devices
//                    valid range is 0x0000 to 0x7FFF for 32K Flash devices
//   2) uint8_t byte - byte to write to Flash.
//
// This routine writes <byte> to the linear FLASH address <addr>.
//
// This routine conforms to the recommendations in the C8051F70x data sheet
// 
// If the MCU is operating from the internal voltage regulator, the VDD
// monitor should be set threshold and enabled as a reset source only when
// writing or erasing Flash. Otherwise, it should be set to the low threshold.
//
// If the MCU is operating from an external voltage regulator powering VDD
// directly, the VDD monitor can be set to the high threshold permanently.
//-----------------------------------------------------------------------------

uint8_t FLASH_ByteWrite (FLADDR addr, uint8_t byte)
{
   SI_SEGMENT_VARIABLE(EA_Save, uint8_t, SI_SEG_DATA) = IE;

   SI_SEGMENT_VARIABLE_SEGMENT_POINTER(pwrite, uint8_t, SI_SEG_XDATA, SI_SEG_DATA);

   IE_EA = 0;                          // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor


   RSTSRC = 0x06;                      // Enable VDD monitor as a reset source
                                       // Leave missing clock detector enabled

   pwrite = (uint8_t SI_SEG_XDATA*) addr;

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
   return 1;
}

//-----------------------------------------------------------------------------
// FLASH_ByteRead
//-----------------------------------------------------------------------------
//
// Return Value :
//      uint8_t - byte read from Flash
// Parameters   :
//   1) FLADDR addr - address of the byte to read to
//                    valid range is 0x0000 to 0xFBFF for 64K Flash devices
//                    valid range is 0x0000 to 0x7FFF for 32K Flash devices
//
// This routine reads a <byte> from the linear FLASH address <addr>.
//-----------------------------------------------------------------------------

uint8_t FLASH_ByteRead (FLADDR addr)
{
   bit EA_SAVE = IE_EA;                // Preserve EA
   uint8_t code * data pread;               // FLASH read pointer
   uint8_t byte;

   IE_EA = 0;                          // Disable interrupts

   pread = (uint8_t code *) addr;

   byte = *pread;                      // Read the byte

   IE_EA = EA_SAVE;                    // Restore interrupts

   return byte;
}

//-----------------------------------------------------------------------------
// FLASH_PageErase
//-----------------------------------------------------------------------------
//
// Return Value :
//   0 if Write unsuccessful, 1 if Write successful
//
// Parameters   :
//   1) FLADDR addr - address of any byte in the page to erase
//                    valid range is 0x0000 to 0xF9FF for 64K Flash devices
//                    valid range is 0x0000 to 0x7DFF for 32K Flash devices
//
// This routine erases the FLASH page containing the linear FLASH address
// <addr>.  Note that the page of Flash containing the Lock Byte cannot be
// erased from application code.
//
// This routine conforms to the recommendations in the C8051F70x data sheet
//
// If the MCU is operating from the internal voltage regulator, the VDD
// monitor should be set threshold and enabled as a reset source only when
// writing or erasing Flash. Otherwise, it should be set to the low threshold.
//
// If the MCU is operating from an external voltage regulator powering VDD
// directly, the VDD monitor can be set to the high threshold permanently.
//-----------------------------------------------------------------------------

uint8_t FLASH_PageErase (FLADDR addr)
{

   SI_SEGMENT_VARIABLE(EA_Save, uint8_t, SI_SEG_DATA) = IE;

   SI_SEGMENT_VARIABLE_SEGMENT_POINTER(pwrite, uint8_t, SI_SEG_XDATA, SI_SEG_DATA);

   IE_EA = 0;                          // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x06;                      // Enable VDD monitor as a reset source
                                       // Leave missing clock detector enabled

   pwrite = (uint8_t SI_SEG_XDATA *) addr;

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

   return 1;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
