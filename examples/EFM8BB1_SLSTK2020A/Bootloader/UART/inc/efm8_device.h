/******************************************************************************
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef __EFM8_DEVICE_H__
#define __EFM8_DEVICE_H__

#ifdef __C51__
#include "SI_EFM8BB1_Register_Enums.h"
#else
#include "SI_EFM8BB1_Defs.inc"
#endif

// Select the STK device if one has not been specified by the project
#ifndef EFM8BB1_DEVICE
#define EFM8BB1_DEVICE EFM8BB10F8G_QSOP24
#endif
#include "SI_EFM8BB1_Devices.h"

// Bootloader firmware revision number
#define BL_REVISION 0x90

// Device specific ID is checked by the prefix command
#define BL_DERIVATIVE_ID  (0x3000 | DEVICE_DERIVID)

// Holding the boot pin low at reset will start the bootloader
#if defined(DEVICE_PKG_QFN20)
#define BL_START_PIN P2_B0

#elif defined(DEVICE_PKG_QSOP24)
#define BL_START_PIN P2_B0

#elif defined(DEVICE_PKG_SOIC16)
#define BL_START_PIN P2_B0

#else
#error Unknown or unsupported device package!

#endif

// Number of cycles (at reset system clock) boot pin must be held low
#define BL_PIN_LOW_CYCLES (50 * 25 / 8)

// Parameters that describe the flash memory geometry
#define BL_FLASH0_LIMIT DEVICE_FLASH_SIZE
#define BL_FLASH0_PSIZE 512

// Define the size of the bootloader in flash pages
// UART = 1, USB = 3
#define BL_PAGE_COUNT 1

// Define the starting address for the bootloader's code segments
#define BL_LIMIT_ADDRESS (BL_FLASH0_LIMIT - (BL_FLASH0_PSIZE * BL_PAGE_COUNT))
#define BL_START_ADDRESS (BL_FLASH0_LIMIT - BL_FLASH0_PSIZE)
#define BL_LOCK_ADDRESS  (BL_FLASH0_LIMIT - 1)

// Defines for managing SFR pages (used for porting between devices)
#define SET_SFRPAGE(p)

#endif // __EFM8_DEVICE_H__
