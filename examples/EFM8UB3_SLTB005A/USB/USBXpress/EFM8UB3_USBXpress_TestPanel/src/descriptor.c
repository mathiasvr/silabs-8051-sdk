/**************************************************************************//**
 * @file    descriptor.c
 * @brief   Descriptor information to pass to USBX_init()
 * @author  Silicon Laboratories
 *
 *******************************************************************************
 * @section License
 * (C) Copyright 2016 Silicon Labs Inc,
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *
 ******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

// -----------------------------------------------------------------------------
// Includes

#include "efm8_usbxpress.h"

/*** [BEGIN] USB Descriptor Information [BEGIN] ***/

#define MFR_STRING          "Silicon Labs"  /// Manufacturer String
#define PROD_STRING         "Test Panel"    /// Product String
#define SERIAL_STRING       "0001"          /// Serial String

USBX_STRING_DESC(USB_MfrStr[], MFR_STRING);
USBX_STRING_DESC(USB_ProductStr[], PROD_STRING);
USBX_STRING_DESC(USB_SerialStr[], SERIAL_STRING);

// -----------------------------------------------------------------------------
// Variables

const USBX_Init_t code initStruct =
{
   0x10C4,                 // Vendor ID
   0xEA61,                 // Product ID
   USB_MfrStr,             // Pointer to Manufacturer String
   USB_ProductStr,         // Pointer to Product String
   USB_SerialStr,          // Pointer to Serial String
   50,                     // Max Power / 2
   0x80,                   // Power Attribute
   0x0100,                 // Device Release # (BCD format)
   false                   // Use USB FIFO space true
};
