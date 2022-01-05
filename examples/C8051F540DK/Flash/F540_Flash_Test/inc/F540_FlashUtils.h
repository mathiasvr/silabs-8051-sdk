//-----------------------------------------------------------------------------
// F540_FlashUtils.h
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program contains several useful utilities for writing and updating
// FLASH memory.
//
// Target:         C8051F54x
// Tool chain:     Simplicity Studio / Keil C51 9.53
// Command Line:   None
//
//
// Release 1.2 / 14 MAY 2015 (SY)
//    -Update SFR/bit name for Simplicity Studio
//
// Release 1.1 / 10 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0 / 05 NOV 2008 (GP)
//    -Initial Revision
//

//-----------------------------------------------------------------------------
// Open Header #define
//-----------------------------------------------------------------------------

#ifndef _F540_FLASHUTILS_H_
#define _F540_FLASHUTILS_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "F540_FlashPrimitives.h"

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

#endif    // _F540_FLASHUTILS_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------