//-----------------------------------------------------------------------------
// F3xx_USB0_Descriptor.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Header file for USB firmware. Includes descriptor definition.
//
//
// Target:         C8051F3xx
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
// Project Name:   USB0_Interrupt
//
// Release 2.0
//    -Rewrite (CM)
//    -02 NOV 2012
//
// Release 1.3
//    - Changes by PD, GP
//    - Modified Descriptors
//    - Changed name from USB_Descriptor.c
//    - 07 December 2005
//
// Release 1.0
//    -Initial Revision (DM)
//    -22 NOV 2002
//

#ifndef _USB_DESC_H_
#define _USB_DESC_H_

//-----------------------------------------------------------------------------
// Standard Device Descriptor Type Definition
//-----------------------------------------------------------------------------

typedef struct Device_Descriptor
{
   uint8_t    bLength;             // Size of this Descriptor in Bytes
   uint8_t    bDescriptorType;     // Descriptor Type (=1)
   uint8_t    bcdUsbLsb;           // USB Spec Release Number in BCD
   uint8_t    bcdUsbMsb;
   uint8_t    bDeviceClass;        // Device Class Code
   uint8_t    bDeviceSubClass;     // Device Subclass Code
   uint8_t    bDeviceProtocol;     // Device Protocol Code
   uint8_t    bMaxPacketSize0;     // Maximum Packet Size for EP0
   uint8_t    idVendorLsb;         // Vendor ID
   uint8_t    idVendorMsb;
   uint8_t    idProductLsb;        // Product ID
   uint8_t    idProductMsb;
   uint8_t    bcdDeviceLsb;        // Device Release Number in BCD
   uint8_t    bcdDeviceMsb;
   uint8_t    iManufacturer;       // Index of String Desc for Manufacturer
   uint8_t    iProduct;            // Index of String Desc for Product
   uint8_t    iSerialNumber;       // Index of String Desc for SerNo
   uint8_t    bNumConfigurations;  // Number of possible Configurations

} Device_Descriptor;

//-----------------------------------------------------------------------------
// Standard Configuration Descriptor Type Definition
//-----------------------------------------------------------------------------

typedef struct Configuration_Descriptor
{
   uint8_t    bLength;             // Size of this Descriptor in Bytes
   uint8_t    bDescriptorType;     // Descriptor Type (=2)
   uint8_t    wTotalLengthLsb;     // Total Length of Data for this Conf
   uint8_t    wTotalLengthMsb;
   uint8_t    bNumInterfaces;      // Number of Interfaces supported by this
                              // Conf
   uint8_t    bConfigurationValue; // Designator Value for *this*
                              // Configuration
   uint8_t    iConfiguration;      // Index of String Desc for this Conf
   uint8_t    bmAttributes;        // Configuration Characteristics (see below)
   uint8_t    bMaxPower;           // Max. Power Consumption in this
                              // Conf (*2mA)
} Configuration_Descriptor;

//-----------------------------------------------------------------------------
// Standard Interface Descriptor Type Definition
//-----------------------------------------------------------------------------

typedef struct Interface_Descriptor
{
   uint8_t    bLength;             // Size of this Descriptor in Bytes
   uint8_t    bDescriptorType;     // Descriptor Type (=4)
   uint8_t    bInterfaceNumber;    // Number of *this* Interface (0..)
   uint8_t    bAlternateSetting;   // Alternative for this Interface (if any)
   uint8_t    bNumEndpoints;       // No of EPs used by this IF (excl. EP0)
   uint8_t    bInterfaceClass;     // Interface Class Code
   uint8_t    bInterfaceSubClass;  // Interface Subclass Code
   uint8_t    bInterfaceProtocol;  // Interface Protocol Code
   uint8_t    iInterface;          // Index of String Desc for this Interface

} Interface_Descriptor;

//-----------------------------------------------------------------------------
// Standard Endpoint Descriptor Type Definition
//-----------------------------------------------------------------------------

typedef struct Endpoint_Descriptor
{
   uint8_t    bLength;             // Size of this Descriptor in Bytes
   uint8_t    bDescriptorType;     // Descriptor Type (=5)
   uint8_t    bEndpointAddress;    // Endpoint Address (Number + Direction)
   uint8_t    bmAttributes;        // Endpoint Attributes (Transfer Type)
   uint8_t    wMaxPacketSizeLsb;   // Max. Endpoint Packet Size
   uint8_t    wMaxPacketSizeMsb;
   uint8_t    bInterval;           // Polling Interval (Interrupt) ms

} Endpoint_Descriptor;

//-----------------------------------------------------------------------------
// SETUP Packet Type Definition
//-----------------------------------------------------------------------------

typedef struct Setup_Buffer
{
   uint8_t    bmRequestType;       // Request recipient, type, and dir.
   uint8_t    bRequest;            // Specific standard request number
   SI_UU16_t  wValue;              // varies according to request
   SI_UU16_t  wIndex;              // varies according to request
   SI_UU16_t  wLength;             // Number of bytes to transfer

} Setup_Buffer;

#endif // _USB_DESC_H_