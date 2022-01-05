/*
 * descriptor.c
 *
 *  Descriptor information to pass to USB_Init()
 *
 */

#include "descriptor.h"

/*** [BEGIN] USB Descriptor Information [BEGIN] ***/

#define MFR_STRING          "Silicon Labs"    /// Manufacturer String
#define PROD_STRING         "USBXpress Echo"  /// Product String
#define SERIAL_STRING       "0001"            /// Serial String

USBX_STRING_DESC(USB_MfrStr[], MFR_STRING);
USBX_STRING_DESC(USB_ProductStr[], PROD_STRING);
USBX_STRING_DESC(USB_SerialStr[], SERIAL_STRING);


const USBX_Init_t initStruct =
{
   0x10C4,                 // Vendor ID
   0xEA61,                 // Product ID
   USB_MfrStr,             // Pointer to Manufacturer String
   USB_ProductStr,         // Pointer to Product String
   USB_SerialStr,          // Pointer to Serial String
   32,                     // Max Power / 2
   0x80,                   // Power Attribute
   0x0100,                 // Device Release # (BCD format)
   false                   // Use USB FIFO space true
};
