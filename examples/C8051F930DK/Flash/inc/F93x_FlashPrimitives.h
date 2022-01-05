//-----------------------------------------------------------------------------
// F93x_FlashPrimitives.h
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
// Open Header #define
//-----------------------------------------------------------------------------

#ifndef _F93X_FLASHPRIMITIVES_H_
#define _F93X_FLASHPRIMITIVES_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F930_Register_Enums.h>

//-----------------------------------------------------------------------------
// Structures, Unions, Enumerations, and Type Definitions
//-----------------------------------------------------------------------------

typedef uint32_t ULONG;
typedef uint16_t  UINT;
typedef uint8_t UCHAR;
typedef UINT FLADDR;

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#ifndef FLASH_PAGESIZE
#define FLASH_PAGESIZE 1024
#endif

#ifndef FLASH_SCRATCHSIZE
#define FLASH_SCRATCHSIZE 1024
#endif

#ifndef FLASH_TEMP
#define FLASH_TEMP 0xF400L             // For 64K Flash devices
//#define FLASH_TEMP 0x7800L           // For 32K Flash devices
#endif

#ifndef FLASH_LAST
#define FLASH_LAST 0xFC00L             // For 64K Flash devices
//#define FLASH_LAST 0x8000L           // For 32K Flash devices
#endif

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------

void          FLASH_ByteWrite (FLADDR addr, char byte, bit SFLE);
uint8_t FLASH_ByteRead  (FLADDR addr, bit SFLE);
void          FLASH_PageErase (FLADDR addr, bit SFLE);

//-----------------------------------------------------------------------------
// Close Header #define
//-----------------------------------------------------------------------------

#endif    // _F93X_FLASHPRIMITIVES_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
