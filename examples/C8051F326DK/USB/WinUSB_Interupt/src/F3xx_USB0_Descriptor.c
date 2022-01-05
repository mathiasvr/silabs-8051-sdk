//-----------------------------------------------------------------------------
// F3xx_USB0_Descriptor.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Source file for USB firmware. Includes descriptor data.
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
// Release 1.0
//    -Initial Revision (PD)
//    -04 JUN 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "c8051f3xx.h"
#include "F3xx_USB0_InterruptServiceRoutine.h"
#include "F3xx_USB0_Descriptor.h"

//-----------------------------------------------------------------------------
// Descriptor Declarations
//-----------------------------------------------------------------------------

// Note: Multi-byte fields are expressed in little-endian (LSB first)

SI_SEGMENT_VARIABLE(DEVICE_DESC, const Device_Descriptor, SI_SEG_CODE) =
{
   18,                  // bLength
   0x01,                // bDescriptorType
   LE_ARRAY(0x0110),    // bcdUSB
   0x00,                // bDeviceClass
   0x00,                // bDeviceSubClass
   0x00,                // bDeviceProtocol
   EP0_PACKET_SIZE,     // bMaxPacketSize0
   LE_ARRAY(0x10C4),    // idVendor
   LE_ARRAY(0x8846),    // idProduct
   LE_ARRAY(0x0000),    // bcdDevice
   0x01,                // iManufacturer
   0x02,                // iProduct
   0x00,                // iSerialNumber
   0x01                 // bNumConfigurations
};

SI_SEGMENT_VARIABLE(CONFIG_DESC, const Configuration_Descriptor, SI_SEG_CODE) =
{
   0x09,                // bLength
   0x02,                // bDescriptorType
   LE_ARRAY(0x0020),    // wTotalLength
   0x01,                // bNumInterfaces
   0x01,                // bConfigurationValue
   0x00,                // iConfiguration
   0x80,                // bmAttributes (Bus-powered)
   0x14                 // bMaxPower (40mA)
};

SI_SEGMENT_VARIABLE(INTERFACE_DESC, const Interface_Descriptor, SI_SEG_CODE) =
{
   0x09,                // bLength
   0x04,                // bDescriptorType
   0x00,                // bInterfaceNumber
   0x00,                // bAlternateSetting
   0x02,                // bNumEndpoints (Excluding EP0)
   0xFF,                // bInterfaceClass (Vendor specific)
   0x00,                // bInterfaceSubClass
   0x00,                // bInterfaceProcotol
   0x00                 // iInterface
};

SI_SEGMENT_VARIABLE(IN_EP1_DESC, const Endpoint_Descriptor, SI_SEG_CODE) =
{
   0x07,                // bLength
   0x05,                // bDescriptorType
   0x81,                // bEndpointAddress (IN EP1)
   0x03,                // bmAttributes
   LE_ARRAY(IN_EP1_PACKET_SIZE), // MaxPacketSize
   10                   // bInterval
};

SI_SEGMENT_VARIABLE(OUT_EP1_DESC, const Endpoint_Descriptor, SI_SEG_CODE) =
{
   0x07,                // bLength
   0x05,                // bDescriptorType
   0x01,                // bEndpointAddress (OUT EP1)
   0x03,                // bmAttributes
   LE_ARRAY(OUT_EP1_PACKET_SIZE), // MaxPacketSize
   10                   // bInterval
};

#define STRING0_LEN 4

SI_SEGMENT_VARIABLE(STRING0_DESC[STRING0_LEN], const uint8_t, SI_SEG_CODE) =
{
   STRING0_LEN, 0x03, 0x09, 0x04
};

// Note: sizeof("") returns an additional +1 for the null-terminator,
// which in this case is used in place of the first two bytes
// in the string descriptor
#define STRING1_LEN sizeof ("Silicon Laboratories Inc.") * 2

SI_SEGMENT_VARIABLE(STRING1_DESC[STRING1_LEN], const uint8_t, SI_SEG_CODE) =
{
   STRING1_LEN, 0x03,
   'S', 0,
   'i', 0,
   'l', 0,
   'i', 0,
   'c', 0,
   'o', 0,
   'n', 0,
   ' ', 0,
   'L', 0,
   'a', 0,
   'b', 0,
   'o', 0,
   'r', 0,
   'a', 0,
   't', 0,
   'o', 0,
   'r', 0,
   'i', 0,
   'e', 0,
   's', 0,
   ' ', 0,
   'I', 0,
   'n', 0,
   'c', 0,
   '.', 0
};

// Note: sizeof("") returns an additional +1 for the null-terminator,
// which in this case is used in place of the first two bytes
// in the string descriptor
#define STRING2_LEN sizeof("C8051F3xx Interrupt Example") * 2

SI_SEGMENT_VARIABLE(STRING2_DESC[STRING2_LEN], const uint8_t, SI_SEG_CODE) =
{
   STRING2_LEN, 0x03,
   'C', 0,
   '8', 0,
   '0', 0,
   '5', 0,
   '1', 0,
   'F', 0,
   '3', 0,
   'x', 0,
   'x', 0,
   ' ', 0,
   'I', 0,
   'n', 0,
   't', 0,
   'e', 0,
   'r', 0,
   'r', 0,
   'u', 0,
   'p', 0,
   't', 0,
   ' ', 0,
   'E', 0,
   'x', 0,
   'a', 0,
   'm', 0,
   'p', 0,
   'l', 0,
   'e', 0
};

SI_SEGMENT_VARIABLE_SEGMENT_POINTER(STRING_DESC_TABLE[], uint8_t, const SI_SEG_CODE, const SI_SEG_CODE) =
{
   STRING0_DESC,
   STRING1_DESC,
   STRING2_DESC
};
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------