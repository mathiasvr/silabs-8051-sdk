//-----------------------------------------------------------------------------
// EFM8UB1_FlashPrimitives.h
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// flash memory.
//
// Target:         EFM8UB1
// Tool chain:     Keil C51
//
// Release 1.0
//    -Initial Revision (ST)
//    -10 OCT 2014
//

#ifndef _EFM8BB1_FLASHPRIMITIVES_H_
#define _EFM8BB1_FLASHPRIMITIVES_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8UB1_Register_Enums.h>

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
#define FLASH_TEMP 0x1C00L             // For 8 kB Flash devices
//#define FLASH_TEMP 0x0C00L           // For 4 kB Flash devices
//#define FLASH_TEMP 0x0400L           // For 2 kB Flash devices
#endif

#ifndef FLASH_LAST
#define FLASH_LAST 0x1E00L             // For 8 kB Flash devices
//#define FLASH_TEMP 0x0E00L           // For 4 kB Flash devices
//#define FLASH_TEMP 0x0600L           // For 2 kB Flash devices
#endif

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------
void          FLASH_ByteWrite (FLADDR addr, uint8_t byte);
uint8_t       FLASH_ByteRead  (FLADDR addr);
void          FLASH_PageErase (FLADDR addr);


#endif    // _EFM8BB1_FLASHPRIMITIVES_H_
