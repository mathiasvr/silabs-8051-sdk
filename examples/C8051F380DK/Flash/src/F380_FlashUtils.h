//-----------------------------------------------------------------------------
// F380_FlashUtils.h
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

#ifndef _F380_FLASHUTILS_H_
#define _F380_FLASHUTILS_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "F380_FlashPrimitives.h"

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------

// FLASH read/write/erase routines
void FLASH_Write (FLADDR dest, char *src, unsigned int numbytes);
char * FLASH_Read (char *dest, FLADDR src, unsigned int numbytes);
void FLASH_Clear (FLADDR addr, unsigned int numbytes);

// FLASH update/copy routines
void FLASH_Update (FLADDR dest, char *src, unsigned int numbytes);
void FLASH_Copy (FLADDR dest, FLADDR src, unsigned int numbytes);

// FLASH test routines
void FLASH_Fill (FLADDR addr, ULONG length, UCHAR fill);

//-----------------------------------------------------------------------------
// Close Header #define
//-----------------------------------------------------------------------------

#endif    // _F380_FLASHUTILS_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------