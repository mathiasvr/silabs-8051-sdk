//-----------------------------------------------------------------------------
// F390_FlashUtils.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// FLASH memory.
//
// Target:         C8051F39x
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.0
//    -Initial Revision (ST)
//    -22 NOV 2011
//

#ifndef F390_FLASHUTILS_H
#define F390_FLASHUTILS_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "F390_FlashPrimitives.h"

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
extern void FLASH_Write (FLADDR dest, char *src, unsigned numbytes);
extern char * FLASH_Read (char *dest, FLADDR src, unsigned numbytes);
extern void FLASH_Clear (FLADDR addr, unsigned numbytes);

// FLASH update/copy routines
extern void FLASH_Update (FLADDR dest, char *src, unsigned numbytes);   // copy with destination preservation
extern void FLASH_Copy (FLADDR dest, FLADDR src, unsigned numbytes);    // low-level FLASH/FLASH byte copy

// FLASH test routines
extern void FLASH_Fill (FLADDR addr, ULONG length, UCHAR fill);

#endif // F390_FLASHUTILS_H
