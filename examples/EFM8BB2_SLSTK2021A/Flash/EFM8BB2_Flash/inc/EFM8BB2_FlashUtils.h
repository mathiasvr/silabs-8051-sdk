//-----------------------------------------------------------------------------
// EFM8BB2_FlashUtils.h
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// flash memory.
//
// Target:         EFM8BB2
// Tool chain:     Keil C51
//
// Release 1.0
//    -Initial Revision (ST)
//    -10 OCT 2014
//

#ifndef _EFM8BB2_FLASHUTILS_H_
#define _EFM8BB2_FLASHUTILS_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB2_Register_Enums.h>
#include "EFM8BB2_FlashPrimitives.h"

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


#endif    // _EFM8BB2_FLASHUTILS_H_
