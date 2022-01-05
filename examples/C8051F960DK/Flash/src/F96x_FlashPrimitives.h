//-----------------------------------------------------------------------------
// F96x_FlashPrimitives.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// This program contains several useful utilities for writing and updating
// FLASH memory.
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil
// Command Line:   None
//
// Release 1.1 (ST)
//    - Removed references to scratchpad memory/SFLE bit
//    - Corrected FLASH_TEMP and FLASH_LAST addresses
//    - Added support for 64, 32 and 16 Kb flash sizes
//    - 26 AUG 2014
//
// Release 1.0 (ST)
//    - Initial Revision
//    - 25 AUG 2014
//

#ifndef F96X_FLASHPRIMITIVES_H
#define F96X_FLASHPRIMITIVES_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "si_toolchain.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define FLASH_SIZE  128                // 128K, 64K, 32K or 16K

#ifndef FLASH_PAGESIZE
#define FLASH_PAGESIZE 1024L
#endif

//---------------------

#if (FLASH_SIZE == 128)

#ifndef FLASH_TEMP                     // Address of temp page
#define FLASH_TEMP 0x1F800L            // For 128K devices
#endif

#ifndef FLASH_LAST                     // Last page of FLASH
#define FLASH_LAST 0x1FC00L            // For 128K devices
#endif

#endif

//---------------------

#if (FLASH_SIZE == 64)

#ifndef FLASH_TEMP                     // Address of temp page
#define FLASH_TEMP 0x0F800L            // For 64K devices
#endif

#ifndef FLASH_LAST                     // Last page of FLASH
#define FLASH_LAST 0x0FC00L            // For 64K devices
#endif

#endif

//---------------------

#if (FLASH_SIZE == 32)

#ifndef FLASH_TEMP                     // Address of temp page
#define FLASH_TEMP 0x07800L            // For 32K devices
#endif

#ifndef FLASH_LAST                     // Last page of FLASH
#define FLASH_LAST 0x07C00L            // For 32K devices
#endif

#endif

//---------------------

#if (FLASH_SIZE == 16)

#ifndef FLASH_TEMP                     // Address of temp page
#define FLASH_TEMP 0x03800L            // For 16K devices
#endif

#ifndef FLASH_LAST                     // Last page of FLASH
#define FLASH_LAST 0x03C00L            // For 16K devices
#endif

#endif

//---------------------

typedef uint32_t FLADDR;                    // Allows for 17-bit address


//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------

// FLASH read/write/erase routines

uint8_t   FLASH_ByteWrite (FLADDR addr, uint8_t byte);
uint8_t   FLASH_ByteRead  (FLADDR addr);
uint8_t   FLASH_PageErase (FLADDR addr);

#endif // F96X_FLASHPRIMITIVES_H


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
