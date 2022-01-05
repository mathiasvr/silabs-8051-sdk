//-----------------------------------------------------------------------------
// F970_FlashUtils.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
//
// This program contains several useful utilities for writing and updating
// FLASH memory.
//
//
// Target:         C8051F97x
// Tool chain:     Keil
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 5 MAY 2014

#ifndef F970_FLASHUTILS_H
#define F970_FLASHUTILS_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

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
extern void FLASH_Write (FLADDR dest, uint8_t *src, uint16_t numbytes,
			 bit SFLE);
extern uint8_t * FLASH_Read  (uint8_t *dest, FLADDR src, uint16_t numbytes,
			      bit SFLE);
extern void FLASH_Clear (FLADDR addr, uint16_t numbytes, bit SFLE);

// FLASH update/copy routines
extern void FLASH_Update (FLADDR dest, uint8_t *src, uint16_t numbytes,
			  bit SFLE);
extern void FLASH_Copy   (FLADDR dest, bit destSFLE, FLADDR src, bit srcSFLE,
                          uint16_t numbytes);

// FLASH test routines
extern void FLASH_Fill (FLADDR addr, uint32_t length, uint8_t fill, bit SFLE);

#endif // F970_FLASHUTILS_H


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
