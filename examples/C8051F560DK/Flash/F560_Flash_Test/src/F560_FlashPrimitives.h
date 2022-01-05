//-----------------------------------------------------------------------------
// F560_FlashPrimitives.h
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// FLASH memory.
//
// Target:         C8051F560
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.2 / 8  MAY 2015 (DL)
//    -Updated to use with Simplicity Studio
//
// Release 1.1 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//    -Updated to meet new data sheet guidelines for writing/erasing
//
// Release 1.0 / 15 JAN 2009 (GP)
//    -Initial Revision
//

#include "si_toolchain.h"

//-----------------------------------------------------------------------------
// Open Header #define
//-----------------------------------------------------------------------------

#ifndef _F560_FLASHPRIMITIVES_H_
#define _F560_FLASHPRIMITIVES_H_

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
#define FLASH_TEMP 0x7800L             // For 32K Flash devices
//#define FLASH_TEMP 0x3C00L           // For 16K Flash devices
#endif

#ifndef FLASH_LAST
#define FLASH_LAST 0x7A00L             // For 32K Flash devices
//#define FLASH_LAST 0x3E00L           // For 16K Flash devices
#endif

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------

uint8_t   FLASH_ByteWrite (FLADDR addr, uint8_t byte);
uint8_t   FLASH_ByteRead  (FLADDR addr);
uint8_t   FLASH_PageErase (FLADDR addr);

//-----------------------------------------------------------------------------
// Close Header #define
//-----------------------------------------------------------------------------

#endif    // _F560_FLASHPRIMITIVES_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------