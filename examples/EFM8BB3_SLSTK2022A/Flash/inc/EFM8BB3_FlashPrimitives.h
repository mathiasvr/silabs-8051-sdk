//-----------------------------------------------------------------------------
// EFM8BB3_FlashPrimitives.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// flash memory.
//
// Target:         EFM8BB3
// Tool chain:     Keil C51
//
// Release 1.0
//    -Initial Revision (ST)
//    -19 AUG 2015
//

#ifndef _EFM8BB3_FLASHPRIMITIVES_H_
#define _EFM8BB3_FLASHPRIMITIVES_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB3_Register_Enums.h>

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
#define FLASH_TEMP 0xFA00L             // For 64 kB Flash devices
//#define FLASH_TEMP 0x7C00L           // For 32 kB Flash devices
//#define FLASH_TEMP 0x3C00L           // For 16 kB Flash devices
#endif

#ifndef FLASH_LAST
#define FLASH_LAST 0xFC00L             // For 64 kB Flash devices
//#define FLASH_TEMP 0x7E00L           // For 32 kB Flash devices
//#define FLASH_TEMP 0x3E00L           // For 16 kB Flash devices
#endif

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------
void          FLASH_ByteWrite (FLADDR addr, uint8_t byte);
uint8_t       FLASH_ByteRead  (FLADDR addr);
void          FLASH_PageErase (FLADDR addr);


#endif    // _EFM8BB3_FLASHPRIMITIVES_H_
