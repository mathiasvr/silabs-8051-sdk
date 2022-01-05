//-----------------------------------------------------------------------------
// EFM8SB2_FlashPrimitives.h
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// flash memory.
//
// Target:         EFM8SB2
// Tool chain:     Keil C51
//
// Release 1.0
//    -Initial Revision (ST)
//    -10 OCT 2014
// Release 1.1
//    -fixed the flash parameters to support EFM8SB2
//    -10 APR 2020
#ifndef _EFM8SB2_FLASHPRIMITIVES_H_
#define _EFM8SB2_FLASHPRIMITIVES_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8SB2_Register_Enums.h>

//-----------------------------------------------------------------------------
// Structures, Unions, Enumerations, and Type Definitions
//-----------------------------------------------------------------------------
typedef uint16_t FLADDR;

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#ifndef FLASH_PAGESIZE
#define FLASH_PAGESIZE 1024
#endif

#ifndef FLASH_TEMP
#define FLASH_TEMP 0xF800L             // For 63 kB Flash devices
//#define FLASH_TEMP 0x7800L           // For 32 kB Flash devices
//#define FLASH_TEMP 0x3800L           // For 16 kB Flash devices
#endif

#ifndef FLASH_LAST
#define FLASH_LAST 0xFC00               // For 63 kB Flash devices
//#define FLASH_LAST 0x7C00             // For 32 kB Flash devices
//#define FLASH_LAST 0x3C00             // For 16 kB Flash devices
#endif

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------
void          FLASH_ByteWrite (FLADDR addr, uint8_t byte);
uint8_t       FLASH_ByteRead  (FLADDR addr);
void          FLASH_PageErase (FLADDR addr);


#endif    // _EFM8SB2_FLASHPRIMITIVES_H_
