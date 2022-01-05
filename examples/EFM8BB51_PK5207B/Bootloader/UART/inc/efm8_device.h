/******************************************************************************
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef __EFM8_DEVICE_H__
#define __EFM8_DEVICE_H__

#ifdef __C51__
#include "SI_EFM8BB51_Register_Enums.h"
#else
#include "SI_EFM8BB51_Defs.inc"
#endif

// Select the PK device if one has not been specified by the project
#ifndef EFM8BB51_DEVICE
#define EFM8BB51_DEVICE EFM8BB51F16G_QFN20
#endif
#include "SI_EFM8BB51_Devices.h"

// Bootloader firmware revision number
#define BL_REVISION 0x90

// Device specific ID is checked by the prefix command
#define BL_DERIVATIVE_ID (0x3900 | DEVICE_DERIVID)

// Holding the boot pin low at reset will start the bootloader
#if defined(DEVICE_PKG_QFN20) || defined(DEVICE_PKG_TSSOP20)
#define BL_START_PIN P2_B0

#else
#error Unknown or unsupported device package!

#endif

// Number of cycles (at reset system clock) boot pin must be held low
#define BL_PIN_LOW_CYCLES (50 * 25 / 8)

// Parameters that describe the flash memory geometry
#define BL_FLASH1_START 0xF000
#define BL_FLASH1_LIMIT 0xF800
#define BL_FLASH1_PSIZE 2048

// Define range bootloader is allowed to write
#if DEVICE_FLASH_SIZE < BL_FLASH1_START
#define BL_FLASH0_LIMIT DEVICE_FLASH_SIZE
#else
#define BL_FLASH0_LIMIT BL_FLASH1_START
#endif

// Define the starting address for the bootloader's code segments
#define BL_LIMIT_ADDRESS (BL_FLASH1_START)
#define BL_START_ADDRESS (BL_FLASH1_START)
#define BL_LOCK_ADDRESS  (BL_FLASH1_LIMIT - 1)

// Defines for managing SFR pages (used for porting between devices)
#define SET_SFRPAGE(p)  SFRPAGE = (p)

#endif // __EFM8_DEVICE_H__
