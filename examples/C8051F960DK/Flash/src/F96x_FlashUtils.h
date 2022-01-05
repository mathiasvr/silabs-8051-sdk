//-----------------------------------------------------------------------------
// F960_FlashUtils.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// This program contains several useful utilities for writing and updating
// FLASH memory.
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil
// Command Line:   None
//
// Release 1.1 (ST)
//    - Removed references to scratchpad memory/SFLE bit
//    - Corrected FLASH_TEMP and FLASH_LAST addresses
//    - Added support for 64, 32 and 16 Kb flash sizes
//    - 26 AUG 2014
//
// Release 1.0 (ST)
//    - Initial Revision
//    - 25 AUG 2014
//

#ifndef F96X_FLASHUTILS_H
#define F96X_FLASHUTILS_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "si_toolchain.h"
#include "F96x_FlashPrimitives.h"

//-----------------------------------------------------------------------------
// Structures, Unions, Enumerations, and Type Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------

// FLASH read/write/erase routines
extern void FLASH_Write (FLADDR dest, uint8_t *src, uint16_t numbytes);
extern uint8_t * FLASH_Read  (uint8_t *dest, FLADDR src, uint16_t numbytes);
extern void FLASH_Clear (FLADDR addr, uint16_t numbytes);

// FLASH update/copy routines
extern void FLASH_Update (FLADDR dest, uint8_t *src, uint16_t numbytes);
extern void FLASH_Copy   (FLADDR dest, FLADDR src, uint16_t numbytes);

// FLASH test routines
extern void FLASH_Fill (FLADDR addr, uint32_t length, uint8_t fill);

#endif // F96X_FLASHUTILS_H


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
