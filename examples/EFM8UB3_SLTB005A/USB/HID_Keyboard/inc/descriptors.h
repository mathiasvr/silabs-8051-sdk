/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef __SILICON_LABS_DESCRIPTORS_H__
#define __SILICON_LABS_DESCRIPTORS_H__

#include <endian.h>
#include <stdlib.h>
#include <string.h>
#include "si_toolchain.h"
#include "efm8_usb.h"

#ifdef __cplusplus
extern "C" {
#endif

// -------------------- USB Identification ------------------------------------
//
// **********
// NOTE: YOU MUST PROVIDE YOUR OWN USB VID/PID (below)
// **********
//
// Following are the definition of the USB VID and PID.  These are example
// values and are assigned to Silicon Labs.  You may not use the Silicon
// Labs VID/PID values in your product.  You must provide your own assigned
// VID and PID values.
///
// $[Vendor ID]
#define USB_VENDOR_ID                      htole16(0x10c4)
// [Vendor ID]$

// $[Product ID]
#define USB_PRODUCT_ID                     htole16(0xfe01)
// [Product ID]$

// Endpoint address of the HID keyboard IN endpoint
#define KEYBOARD_IN_EP_ADDR   EP1IN

// Interface number of the HID keyboard
#define HID_KEYBOARD_IFC                  0

extern SI_SEGMENT_VARIABLE(ReportDescriptor0[69], const uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(deviceDesc[], const USB_DeviceDescriptor_TypeDef, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(configDesc[], const uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(initstruct, const USBD_Init_TypeDef, SI_SEG_CODE);

typedef uint8_t KeyReport_TypeDef[8];

#ifdef __cplusplus
}
#endif

#endif  // #define __SILICON_LABS_DESCRIPTORS_H__
