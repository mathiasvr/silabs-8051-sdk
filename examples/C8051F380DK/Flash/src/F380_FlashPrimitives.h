//-----------------------------------------------------------------------------
// F380_FlashPrimitives.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://www.silabs.com
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
// Open Header #define
//-----------------------------------------------------------------------------

#ifndef _F380_FLASHPRIMITIVES_H_
#define _F380_FLASHPRIMITIVES_H_

//-----------------------------------------------------------------------------
// Structures, Unions, Enumerations, and Type Definitions
//-----------------------------------------------------------------------------

typedef unsigned long ULONG;
typedef unsigned int  UINT;
typedef unsigned char UCHAR;
typedef UINT FLADDR;

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

void          FLASH_ByteWrite (FLADDR addr, char byte);
unsigned char FLASH_ByteRead  (FLADDR addr);
void          FLASH_PageErase (FLADDR addr);

//-----------------------------------------------------------------------------
// Close Header #define
//-----------------------------------------------------------------------------

#endif    // _F380_FLASHPRIMITIVES_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------