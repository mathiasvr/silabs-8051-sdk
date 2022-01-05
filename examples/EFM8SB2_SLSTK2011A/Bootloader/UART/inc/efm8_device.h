/******************************************************************************
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef __EFM8_DEVICE_H__
#define __EFM8_DEVICE_H__

#ifdef __C51__
#include "SI_EFM8SB2_Register_Enums.h"
#else
#include "SI_EFM8SB2_Defs.inc"
#endif

// Select the STK device if one has not been specified by the project
#ifndef EFM8SB2_DEVICE
#define EFM8SB2_DEVICE EFM8SB20F64G_QFN32
#endif
#include "SI_EFM8SB2_Devices.h"

// Bootloader firmware revision number
#define BL_REVISION 0x90

// Device specific ID is checked by the prefix command
#define BL_DERIVATIVE_ID  (0x1600 | DEVICE_DERIVID)

// Holding the boot pin low at reset will start the bootloader
#if defined(DEVICE_PKG_QFN32) || defined(DEVICE_PKG_QFP32)
#define BL_START_PIN P2_B7

#elif defined(DEVICE_PKG_QFN24)
#define BL_START_PIN P2_B7

#else
#error Unknown or unsupported device package!

#endif

// Number of cycles (at reset system clock) boot pin must be held low
#define BL_PIN_LOW_CYCLES (50 * 20 / 8)

// Parameters that describe the flash memory geometry
#define BL_FLASH0_LIMIT DEVICE_FLASH_SIZE
#define BL_FLASH0_PSIZE 1024
#define BL_FLASH1_LIMIT 0x400

// Define the size of the bootloader in flash pages
// UART = 1, USB = 3
#define BL_PAGE_COUNT 1

// Define the starting address for the bootloader's code segments
#define BL_LIMIT_ADDRESS (BL_FLASH0_LIMIT - (BL_FLASH0_PSIZE * BL_PAGE_COUNT))
#define BL_START_ADDRESS (BL_FLASH0_LIMIT - BL_FLASH0_PSIZE)
#define BL_LOCK_ADDRESS  (BL_FLASH0_LIMIT - 1)

// Defines for managing SFR pages (used for porting between devices)
#define SET_SFRPAGE(p)  SFRPAGE = (p)

#endif // __EFM8_DEVICE_H__
