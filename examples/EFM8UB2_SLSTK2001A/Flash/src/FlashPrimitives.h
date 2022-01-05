//-----------------------------------------------------------------------------
// FlashPrimitives.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// FLASH memory.
//
// Target:         EFM8SB1
// Tool chain:     KEIL C51
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (ST)
//    -06 AUG 2014
//

//-----------------------------------------------------------------------------
// Open Header #define
//-----------------------------------------------------------------------------

#ifndef _FLASHPRIMITIVES_H_
#define _FLASHPRIMITIVES_H_

//-----------------------------------------------------------------------------
// Structures, Unions, Enumerations, and Type Definitions
//-----------------------------------------------------------------------------

typedef uint16_t FLADDR;

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#ifndef FLASH_PAGESIZE
#define FLASH_PAGESIZE 512
#endif

#ifndef FLASH_TEMP
#define FLASH_TEMP 0xF800L             // For 64K Flash devices
//#define FLASH_TEMP 0x7C00L           // For 32K Flash devices
//#define FLASH_TEMP 0x3C00L           // For 16K Flash devices
#endif

#ifndef FLASH_LAST
#define FLASH_LAST 0xFA00L             // For 64K Flash devices
//#define FLASH_LAST 0x7E00L           // For 32K Flash devices
//#define FLASH_LAST 0x3E00L           // For 16K Flash devices
#endif

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------
void          FLASH_ByteWrite (FLADDR addr, uint8_t byte);
uint8_t       FLASH_ByteRead  (FLADDR addr);
void          FLASH_PageErase (FLADDR addr);

//-----------------------------------------------------------------------------
// Close Header #define
//-----------------------------------------------------------------------------

#endif    // _FLASHPRIMITIVES_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------