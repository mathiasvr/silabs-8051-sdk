//-----------------------------------------------------------------------------
// FlashUtils.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// FLASH memory.
//
// Target:         EFM8SB1
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

#ifndef _FLASHUTILS_H_
#define _FLASHUTILS_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "FlashPrimitives.h"

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------
// Flash read/write/erase routines
void FLASH_Write (FLADDR dest, uint8_t *src, uint16_t numbytes);
uint8_t * FLASH_Read (uint8_t *dest, FLADDR src, uint16_t numbytes);
void FLASH_Clear (FLADDR addr, uint16_t numbytes);

// Flash update/copy routines
void FLASH_Update (FLADDR dest, uint8_t *src, uint16_t numbytes);
void FLASH_Copy (FLADDR dest, FLADDR src, uint16_t numbytes);

// Flash test routines
void FLASH_Fill (FLADDR addr, uint32_t length, uint8_t fill);

//-----------------------------------------------------------------------------
// Close Header #define
//-----------------------------------------------------------------------------

#endif    // _FLASHUTILS_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------