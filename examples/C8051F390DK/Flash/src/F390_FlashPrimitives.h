//-----------------------------------------------------------------------------
// F390_FlashPrimitives.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// FLASH memory.
//
// Target:         C8051F39x
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.0
//    -Initial Revision (ST)
//    -22 NOV 2011
//

#ifndef F390_FLASHPRIMITIVES_H
#define F390_FLASHPRIMITIVES_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Structures, Unions, Enumerations, and Type Definitions
//-----------------------------------------------------------------------------

typedef unsigned long ULONG;
typedef unsigned int UINT;
typedef unsigned char UCHAR;

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#ifndef FLASH_PAGESIZE
#define FLASH_PAGESIZE 512
#endif

#ifndef FLASH_TEMP
#define FLASH_TEMP 0x03B00L            // For 16K Flash Devices
// #define FLASH_TEMP 0x01B00L         // For 8K Flash Devices
#endif

#ifndef FLASH_LAST
#define FLASH_LAST 0x03D00L            // For 16K Flash Devices
// #define FLASH_LAST 0x01D00L         // For 8K Flash Devices
#endif

typedef UINT FLADDR;

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------

// FLASH read/write/erase routines
extern void FLASH_ByteWrite (FLADDR addr, char byte);
extern unsigned char FLASH_ByteRead (FLADDR addr);
extern void FLASH_PageErase (FLADDR addr);

#endif // F310_FLASHPRIMITIVES_H
