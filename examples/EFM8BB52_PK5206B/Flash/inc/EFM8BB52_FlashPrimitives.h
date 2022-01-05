//-----------------------------------------------------------------------------
// EFM8BB52_FlashPrimitives.h
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// flash memory.
//
// Target:         EFM8BB52
// Tool chain:     Keil C51
//
// Release 1.0
//    - Initial Revision (DS)
//    - 11 FEB 2021
//

#ifndef _EFM8BB52_FLASHPRIMITIVES_H_
#define _EFM8BB52_FLASHPRIMITIVES_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB52_Register_Enums.h>

//-----------------------------------------------------------------------------
// Structures, Unions, Enumerations, and Type Definitions
//-----------------------------------------------------------------------------
typedef uint16_t FLADDR;

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#ifndef FLASH_PAGESIZE
#define FLASH_PAGESIZE 2048
#endif

#ifndef FLASH_TEMP
#define FLASH_TEMP 0x7000L           // For 32 kB Flash devices
#endif

#ifndef FLASH_LAST
#define FLASH_LAST 0x7000L           // For 32 kB Flash devices
#endif

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------
void          FLASH_ByteWrite (FLADDR addr, uint8_t byte);
uint8_t       FLASH_ByteRead  (FLADDR addr);
void          FLASH_PageErase (FLADDR addr);


#endif    // _EFM8BB52_FLASHPRIMITIVES_H_
