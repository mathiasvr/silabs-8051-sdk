//-----------------------------------------------------------------------------
// F970_FlashPrimitive.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
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

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"
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
uint8_t   FLASH_ByteWrite (FLADDR addr, uint8_t byte, bit SFLE);
uint8_t   FLASH_ByteRead  (FLADDR addr, bit SFLE);
uint8_t   FLASH_PageErase (FLADDR addr, bit SFLE);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FLASH Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FLASH_ByteWrite
//-----------------------------------------------------------------------------
//
//
// This routine conforms to the recommendations in the C8051F58x data sheet
// 
// If the MCU is operating from the internal voltage regulator, the VDD
// monitor should be set threshold and enabled as a reset source only when
// writing or erasing Flash. Otherwise, it should be set to the low threshold.
//
// If the MCU is operating from an external voltage regulator powering VDD
// directly, the VDD monitor can be set to the high threshold permanently.
//-----------------------------------------------------------------------------
uint8_t FLASH_ByteWrite (FLADDR addr, uint8_t byte, bit SFLE)
{
   int8_t SFRPAGE_SAVE = SFRPAGE;          // Preserve SFRPAGE
   bit EA_SAVE = IE_EA & IE_EA__BMASK; // Preserve EA
   int8_t xdata * data pwrite;             // FLASH write pointer
   uint8_t i;

   IE &= ~IE_EA__BMASK;                // Disable interrupts

   SFRPAGE = LEGACY_PAGE;

   pwrite = (int8_t xdata *) addr;

   RSTSRC = 0x00;                      // 1. Disable VDD monitor as a reset source

   VDM0CN = 0xA0;                      // 2. Enable VDD monitor and high threshold

   for (i = 0; i < 255; i++) {}        // 3. Wait for VDD monitor to stabilize

   if (!(VDM0CN & 0x40))               // 4. If the VDD voltage is not high
      return 0;                        //    enough don't write to Flash

   RSTSRC = 0x02;                      // 5. Safe to enable VDD Monitor as a 
                                       //    reset source

                                       // 6. Enable Flash Writes

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x01;                      // PSWE = 1

   if (SFLE) {
      PSCTL |= 0x04;                   // Set SFLE
   }

   VDM0CN = 0xA0;                      // 7. Enable VDD monitor and high threshold

   RSTSRC = 0x02;                      // 8. Enable VDD monitor as a reset source

   *pwrite = byte;                     // 9. Write the byte

   RSTSRC = 0x00;                      // 10. Disable the VDD monitor as reset 
                                       //     source
   VDM0CN = 0x80;                      // 11. Change VDD Monitor to low threshold
   RSTSRC = 0x02;                      // 12. Re-enable the VDD monitor as a 
                                       //     reset source
   if (SFLE) {
      PSCTL &= ~0x04;                  // Clear SFLE
   }
   PSCTL &= ~0x01;                     // PSWE = 0

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFRPAGE

   IE &= ~IE_EA__BMASK;                // Restore interrupts
   IE |= EA_SAVE;    

   return 1;
}

//-----------------------------------------------------------------------------
// FLASH_ByteRead
//-----------------------------------------------------------------------------
//
// This routine reads a <byte> from the linear FLASH address <addr>.
//
//-----------------------------------------------------------------------------
uint8_t FLASH_ByteRead (FLADDR addr, bit SFLE)
{
   int8_t SFRPAGE_SAVE = SFRPAGE;          // Preserve SFRPAGE
   bit EA_SAVE = IE_EA & IE_EA__BMASK; // Preserve EA
   int8_t code * data pread;               // FLASH read pointer
   uint8_t byte;

   IE &= ~IE_EA__BMASK;                // Disable interrupts

   SFRPAGE = LEGACY_PAGE;

   pread = (int8_t code *) addr;
   
   if (SFLE) {
      PSCTL |= 0x04;                   // Set SFLE
   }

   byte = *pread;                      // Read the byte

   if (SFLE) {
      PSCTL &= ~0x04;                  // Clear SFLE
   }

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFRPAGE

   IE &= ~IE_EA__BMASK;                // Restore interrupts
   IE |= EA_SAVE;    

   return byte;
}

//-----------------------------------------------------------------------------
// FLASH_PageErase
//-----------------------------------------------------------------------------
//
// This routine erases the FLASH page containing the linear FLASH address
// <addr>.
//
// This routine conforms to the recommendations in the C8051F50x data sheet
// 
// If the MCU is operating from the internal voltage regulator, the VDD
// monitor should be set threshold and enabled as a reset source only when
// writing or erasing Flash. Otherwise, it should be set to the low threshold.
//
// If the MCU is operating from an external voltage regulator powering VDD
// directly, the VDD monitor can be set to the high threshold permanently.
//-----------------------------------------------------------------------------
uint8_t FLASH_PageErase (FLADDR addr, bit SFLE)
{
   int8_t SFRPAGE_SAVE = SFRPAGE;          // Preserve SFRPAGE
   bit EA_SAVE = IE_EA & IE_EA__BMASK; // Preserve EA
   int8_t xdata * data pwrite;             // FLASH write pointer
   uint8_t i;

   IE &= ~IE_EA__BMASK;                // Disable interrupts

   SFRPAGE = LEGACY_PAGE;

   pwrite = (int8_t xdata *) addr;
   
   RSTSRC = 0x00;                      // 1. Disable VDD monitor as a reset source

   VDM0CN = 0xA0;                      // 2. Enable VDD monitor and high threshold

   for (i = 0; i < 255; i++) {}        // 3. Wait for VDD monitor to stabilize

   if (!(VDM0CN & 0x40))               // 4. If the VDD voltage is not high enough
      return 0;                        //    don't attempt to write to Flash

   RSTSRC = 0x02;                      // 5. Safe to enable VDD Monitor as a reset 
                                       //    source

                                       // 6. Enable Flash Writes

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x03;                      // PSWE = 1; PSEE = 1

   if (SFLE) {
      PSCTL |= 0x04;                   // Set SFLE
   }

   VDM0CN = 0xA0;                      // 7. Enable VDD monitor and high threshold

   RSTSRC = 0x02;                      // 8. Enable VDD monitor as a reset source

   *pwrite = 0;                        // 9. Initiate page erase

   RSTSRC = 0x00;                      // 10. Disable the VDD monitor as a reset
                                       //     source
   VDM0CN = 0x80;                      // 11. Change VDD Monitor to low threshold
   RSTSRC = 0x02;                      // 12. Re-enable the VDD monitor as a reset 

   if (SFLE) {
      PSCTL &= ~0x04;                  // Clear SFLE
   }

   PSCTL &= ~0x03;                     // PSWE = 0; PSEE = 0

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFRPAGE

   IE &= ~IE_EA__BMASK;                // Restore interrupts
   IE |= EA_SAVE;    

   return 1;
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
