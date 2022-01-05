/******************************************************************************
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef __EFM8_DEVICE_H__
#define __EFM8_DEVICE_H__

#ifdef __C51__
#include "SI_EFM8UB3_Register_Enums.h"
#else
#include "SI_EFM8UB3_Defs.inc"
#endif

// Select the STK device if one has not been specified by the project
#ifndef EFM8UB3_DEVICE
#define EFM8UB3_DEVICE EFM8UB31F40G_QFN24
#endif
#include "SI_EFM8UB3_Devices.h"

// Bootloader firmware revision number
#define BL_REVISION 0x90

// Device specific ID is checked by the prefix command
#define BL_DERIVATIVE_ID  (0x3600 | DEVICE_DERIVID)

// USB Vendor and Product ID's
#define BL_USB_VID 0x10C4
#define BL_USB_PID 0xEACB

// Holding the boot pin low at reset will start the bootloader
#if defined(DEVICE_PKG_QFN20)
#define BL_START_PIN P2_B0

#elif defined(DEVICE_PKG_QFN24)
#define BL_START_PIN P2_B0

#elif defined(DEVICE_PKG_QSOP24)
#define BL_START_PIN P2_B0

#else
#error Unknown or unsupported device package!

#endif

// Number of cycles (at reset system clock) boot pin must be held low
#define BL_PIN_LOW_CYCLES (50 * 25 / 8)

// Parameters that describe the flash memory geometry
#define BL_FLASH_PSIZE 512

// Define the starting address for the bootloader's code segments
#define BL_LIMIT_ADDRESS (DEVICE_FLASH_SIZE - (BL_FLASH_PSIZE * 2))
#define BL_START_ADDRESS (0xFC00 - BL_FLASH_PSIZE)
#define BL_LOCK_ADDRESS  (0xFC00 - 1)

// Defines for managing SFR pages (used for porting between devices)
#define SET_SFRPAGE(p)  SFRPAGE = (p)

#endif // __EFM8_DEVICE_H__
