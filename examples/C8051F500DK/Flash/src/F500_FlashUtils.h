//-----------------------------------------------------------------------------
// F500_FlashUtils.h
//-----------------------------------------------------------------------------
// Copyright 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// FLASH memory.
//
// Target:         C8051F500/1/2/3/4/5/6/7/8/9/10/11
// Tool chain:     Raisonance / Keil / SDCC
// Command Line:   None
//
//
// Release 1.1 / 02 MAR 2010 (GP)
//    -Tested with Raisonance
//    -Updated to meet new data sheet guidelines for writing/erasing
//
// Release 1.0
//    -Initial Revision (GP)
//    -08 APR 2008
//

//-----------------------------------------------------------------------------
// Open Header #define
//-----------------------------------------------------------------------------

#ifndef _F500_FLASHUTILS_H_
#define _F500_FLASHUTILS_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "F500_FlashPrimitives.h"

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

#endif    // _F500_FLASHUTILS_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------