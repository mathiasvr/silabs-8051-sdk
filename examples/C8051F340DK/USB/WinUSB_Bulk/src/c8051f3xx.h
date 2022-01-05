//-----------------------------------------------------------------------------
// c8051f3xx.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Stub file for Firmware Template.
//
//
// How To Test:    See Readme.txt
//
//
// Target:         C8051F320/1,
//                 C8051F326/7,
//                 C8051F34x,
//                 C8051F38x
//
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.6
// Command Line:   See Readme.txt
// Project Name:   USB0_Bulk
//
// Release 2.0
//    -Rewrite (CM)
//    -02 NOV 2012
//
// Release 1.0
//    -Initial Revision (PD)
//    -04 JUN 2008
//

#ifndef _c8051f3xx_H_
#define _c8051f3xx_H_

//-----------------------------------------------------------------------------
// Target Definitions
//-----------------------------------------------------------------------------

#define MCU_F320     0
#define MCU_F326     1
#define MCU_F340     2
#define MCU_F380     3

//-----------------------------------------------------------------------------
// Target Selection
//-----------------------------------------------------------------------------

#define TARGET_MCU   MCU_F340

//-----------------------------------------------------------------------------
// Target Includes
//-----------------------------------------------------------------------------

#if (TARGET_MCU == MCU_F320)

#include <SI_C8051F320_Register_Enums.h>

#define FLASH_PAGE_SIZE       512      // Size of flash page in bytes
#define FLASH_START           0x1200   // Address of first writeable page
#define FLASH_NUM_PAGES       21       // Number of writeable pages
#define FLASH_GROUP_WRITE_EN  0        // Doesn't support 2-byte flash writes

#endif // (TARGET_MCU == MCU_F320)


#if (TARGET_MCU == MCU_F326)

#include <SI_C8051F326_Register_Enums.h>

#define FLASH_PAGE_SIZE       512      // Size of flash page in bytes
#define FLASH_START           0x1200   // Address of first writeable page
#define FLASH_NUM_PAGES       21       // Number of writeable pages
#define FLASH_GROUP_WRITE_EN  0        // Doesn't support 2-byte flash writes

#endif // (TARGET_MCU == MCU_F326)


#if (TARGET_MCU == MCU_F340)

#include <SI_C8051F340_Register_Enums.h>

#define FLASH_PAGE_SIZE       512      // Size of flash page in bytes
#define FLASH_START           0x1200   // Address of first writeable page
#define FLASH_NUM_PAGES       54       // Number of writeable pages
#define FLASH_GROUP_WRITE_EN  1        // Allow 2-byte flash writes

#endif // (TARGET_MCU == MCU_F340)


#if (TARGET_MCU == MCU_F380)

#include <SI_C8051F380_Register_Enums.h>

#define FLASH_PAGE_SIZE       512      // Size of flash page in bytes
#define FLASH_START           0x1200   // Address of first writeable page
#define FLASH_NUM_PAGES       54       // Number of writeable pages
#define FLASH_GROUP_WRITE_EN  1        // Allow 2-byte flash writes

#endif // (TARGET_MCU == MCU_F380)

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------

// Return the LSB given a uint16_t
#define LOBYTE(w)       ((uint8_t)(w))

// Return the MSB given a uint16_t
#define HIBYTE(w)       ((uint8_t)(((uint16_t)(w) >> 8) & 0xFF))

// Define a little-endian multi-byte array initialization given a uint16_t
#define LE_ARRAY(w)     LOBYTE(w), HIBYTE(w)

// Define a big-endian multi-byte array initialization given a uint16_t
#define BE_ARRAY(w)     HIBYTE(w), LOBYTE(w)

#endif // _c8051f3xx_H_
