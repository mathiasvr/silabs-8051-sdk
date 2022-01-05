//-----------------------------------------------------------------------------
// F93x_FlashUtils.h
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

#ifndef _F93X_FLASHUTILS_H_
#define _F93X_FLASHUTILS_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "F93x_FlashPrimitives.h"

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------

// FLASH read/write/erase routines
void FLASH_Write (FLADDR dest, char *src, uint16_t numbytes, bit SFLE);
char * FLASH_Read (char *dest, FLADDR src, uint16_t numbytes, bit SFLE);
void FLASH_Clear (FLADDR addr, uint16_t numbytes, bit SFLE);

// FLASH update/copy routines
void FLASH_Update (FLADDR dest, char *src, uint16_t numbytes, bit SFLE);
void FLASH_Copy (FLADDR dest, bit destSFLE, FLADDR src, bit srcSFLE,
                 unsigned numbytes);

// FLASH test routines
void FLASH_Fill (FLADDR addr, ULONG length, UCHAR fill, bit SFLE);

//-----------------------------------------------------------------------------
// Close Header #define
//-----------------------------------------------------------------------------

#endif    // _F410_FLASHUTILS_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------