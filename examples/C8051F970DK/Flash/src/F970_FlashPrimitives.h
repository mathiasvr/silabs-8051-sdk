//-----------------------------------------------------------------------------
// F970_FlashPrimitives.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
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


#ifndef F970_FLASHPRIMITIVES_H
#define F970_FLASHPRIMITIVES_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "si_toolchain.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define FLASH_SIZE  32                  // 32K or 16K

#ifndef FLASH_PAGESIZE
#define FLASH_PAGESIZE 512L
#endif

#ifndef FLASH_SCRATCHSIZE
#define FLASH_SCRATCHSIZE 512L
#endif

//---------------------

#if (FLASH_SIZE == 32)

#ifndef FLASH_TEMP                     // Address of temp page
#define FLASH_TEMP 0x7C00L             // For 32 devices
#endif

#ifndef FLASH_LAST                     // Last page of FLASH
#define FLASH_LAST 0x7E00L             // For 32 devices
#endif

#endif

//---------------------

#if (FLASH_SIZE == 16)

#ifndef FLASH_TEMP                     // Address of temp page
#define FLASH_TEMP 0x3C00L             // For 16 devices
#endif

#ifndef FLASH_LAST                     // Last page of FLASH
#define FLASH_LAST 0x3E00L             // For 16 devices
#endif

#endif

//---------------------

typedef uint16_t FLADDR;                    // 16-bit address


//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------

// FLASH read/write/erase routines

uint8_t   FLASH_ByteWrite (FLADDR addr, uint8_t byte, bit SFLE);
uint8_t   FLASH_ByteRead  (FLADDR addr, bit SFLE);
uint8_t   FLASH_PageErase (FLADDR addr, bit SFLE);

#endif // F970_FLASHPRIMITIVES_H


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
