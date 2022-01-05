//-----------------------------------------------------------------------------
// F560_FlashUtils.h
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
//
// Release 1.0 / 15 JAN 2009 (GP)
//    -Initial Revision
//

//-----------------------------------------------------------------------------
// Open Header #define
//-----------------------------------------------------------------------------

#ifndef _F560_FLASHUTILS_H_
#define _F560_FLASHUTILS_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "F560_FlashPrimitives.h"

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------

// FLASH read/write/erase routines
void FLASH_Write (FLADDR dest, uint8_t *src, uint16_t numbytes);
uint8_t * FLASH_Read (uint8_t *dest, FLADDR src, uint16_t numbytes);
void FLASH_Clear (FLADDR addr, uint16_t numbytes);

// FLASH update/copy routines
void FLASH_Update (FLADDR dest, uint8_t *src, uint16_t numbytes);
void FLASH_Copy (FLADDR dest, FLADDR src, uint16_t numbytes);

// FLASH test routines
void FLASH_Fill (FLADDR addr, uint16_t length, uint8_t fill);

//-----------------------------------------------------------------------------
// Close Header #define
//-----------------------------------------------------------------------------

#endif    // _F560_FLASHUTILS_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------