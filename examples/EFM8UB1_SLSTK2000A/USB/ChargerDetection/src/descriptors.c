/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// descriptorss.c
/////////////////////////////////////////////////////////////////////////////

// USB HID Mouse descriptors

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include <bsp.h>
#include <descriptors.h>


// Created with usb.org's "HID Descriptor Tool(DT)", version 2.4
SI_SEGMENT_VARIABLE(ReportDescriptor[REPORT_DESCRIPTOR_SIZE],
                    const uint8_t,
                    SI_SEG_CODE) =
{
	0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
	0x09, 0x06,                    // USAGE (Keyboard)
	0xa1, 0x01,                    // COLLECTION (Application)
	0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
	0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
	0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
	0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
	0x75, 0x01,                    //   REPORT_SIZE (1)
	0x95, 0x08,                    //   REPORT_COUNT (8)
	0x81, 0x02,                    //   INPUT (Data,Var,Abs)
	0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
	0x75, 0x01,                    //   REPORT_SIZE (1)
	0x95, 0x08,                    //   REPORT_COUNT (8)
	0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)
	0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
	0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
	0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
	0x75, 0x08,                    //   REPORT_SIZE (8)
	0x95, 0x06,                    //   REPORT_COUNT (6)
	0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
	0x05, 0x08,                    //   USAGE_PAGE (LEDs)
	0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
	0x29, 0x03,                    //   USAGE_MAXIMUM (Scroll Lock)
	0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
	0x75, 0x01,                    //   REPORT_SIZE (1)
	0x95, 0x03,                    //   REPORT_COUNT (3)
	0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
	0x75, 0x01,                    //   REPORT_SIZE (1)
	0x95, 0x05,                    //   REPORT_COUNT (5)
	0x91, 0x01,                    //   OUTPUT (Cnst,Ary,Abs)
	0xc0                           // END_COLLECTION
};

SI_SEGMENT_VARIABLE(deviceDesc[],
                    const USB_DeviceDescriptor_TypeDef,
                    SI_SEG_CODE) =
{
	USB_DEVICE_DESCSIZE,      // bLength
	USB_DEVICE_DESCRIPTOR,    // bDescriptorType
	htole16(0x0200),          // bcdUSB
	0,                        // bDeviceClass
	0,                        // bDeviceSubClass
	0,                        // bDeviceProtocol
	USB_FULL_EP0_SIZE,        // bMaxPacketSize0
	USB_VENDOR_ID,            // idVendor
	USB_PRODUCT_ID,           // idProduct
	htole16(0x0100),          // bcdDevice
	1,                        // iManufacturer
	2,                        // iProduct
	3,                        // iSerialNumber
	1                         // bNumConfigurations
};

SI_SEGMENT_VARIABLE(configDesc[],
                    const uint8_t,
                    SI_SEG_CODE) =
{
	// Configuration descriptor
	USB_CONFIG_DESCSIZE,    // bLength
	USB_CONFIG_DESCRIPTOR,  // bDescriptorType

	USB_CONFIG_DESCSIZE +   // wTotalLength (LSB)
	USB_INTERFACE_DESCSIZE +
	USB_HID_DESCSIZE +
	(USB_ENDPOINT_DESCSIZE * (SLAB_USB_NUM_EPS_USED - 1)),

	(USB_CONFIG_DESCSIZE +  // wTotalLength (MSB)
	USB_INTERFACE_DESCSIZE +
	USB_HID_DESCSIZE +
	(USB_ENDPOINT_DESCSIZE * (SLAB_USB_NUM_EPS_USED - 1))) >> 8,

	1,                      // bNumInterfaces
	1,                      // bConfigurationValue
	0,                      // iConfiguration

#if defined(SLAB_USB_BUS_POWERED)
	CONFIG_DESC_BM_RESERVED_D7,    // bmAttrib: Bus powered
#else
	CONFIG_DESC_BM_RESERVED_D7 |   // bmAttrib: Self powered
	CONFIG_DESC_BM_SELFPOWERED,
#endif

	CONFIG_DESC_MAXPOWER_mA(100),   // bMaxPower: 100 mA

	// Interface descriptor
	USB_INTERFACE_DESCSIZE, // bLength
	USB_INTERFACE_DESCRIPTOR,// bDescriptorType
	HID_KEYBOARD_IFC,       // bInterfaceNumber
	0,                      // bAlternateSetting
	1,                      // bNumEndpoints
	0x03,                   // bInterfaceClass (HID)
	0,                      // bInterfaceSubClass
	1,                      // bInterfaceProtocol
	0,                      // iInterface

	// HID descriptor
	USB_HID_DESCSIZE,       // bLength
	USB_HID_DESCRIPTOR,     // bDescriptorType
	0x11,                   // bcdHID (LSB)
	0x01,                   // bcdHID (MSB)
	0,                      // bCountryCode
	1,                      // bNumDescriptors
	USB_HID_REPORT_DESCRIPTOR,     // bDecriptorType
	sizeof(ReportDescriptor),      // wDescriptorLength(LSB)
	sizeof(ReportDescriptor) >> 8, // wDescriptorLength(MSB)

	// Endpoint descriptor
	USB_ENDPOINT_DESCSIZE,          // bLength
	USB_ENDPOINT_DESCRIPTOR,        // bDescriptorType
	0x81,                           // bEndpointAddress (IN)
	USB_EPTYPE_INTR,                // bmAttributes
	USB_FULL_INT_BULK_MAX_EP_SIZE,  // wMaxPacketSize (LSB)
	0,                              // wMaxPacketSize (MSB)
	24,                             // bInterval
};

#define LANG_STRING   htole16( SLAB_USB_LANGUAGE )

#define MFR_STRING    'S','i','l','i','c','o','n',' ','L','a','b','o','r','a','t','o','r','i','e','s','\0'
#define MFR_SIZE      21
#define PROD_STRING   'E','F','M','8',' ','C','h','a','r','g','e','r',' ','D','e','t','e','c','t','i','o','n','\0'
#define PROD_SIZE     23
#define SER_STRING    '0','1','2','3','4','5','6','7','8','A','B','C','D','E','F','\0'
#define SER_SIZE      16

LANGID_STATIC_CONST_STRING_DESC(langDesc[], LANG_STRING);
UTF16LE_PACKED_STATIC_CONST_STRING_DESC(mfrDesc[], MFR_STRING, MFR_SIZE);
UTF16LE_PACKED_STATIC_CONST_STRING_DESC(prodDesc[], PROD_STRING, PROD_SIZE);
UTF16LE_PACKED_STATIC_CONST_STRING_DESC(serDesc[], SER_STRING, SER_SIZE);

//-----------------------------------------------------------------------------
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(myUsbStringTable_USEnglish[], static const USB_StringDescriptor_TypeDef, SI_SEG_GENERIC, const SI_SEG_CODE) = 
{
	(SI_VARIABLE_SEGMENT_POINTER(, uint8_t, SI_SEG_CODE))langDesc,
	mfrDesc,
	prodDesc,
	serDesc,
};

//-----------------------------------------------------------------------------
SI_SEGMENT_VARIABLE(initstruct,
                    const USBD_Init_TypeDef,
                    SI_SEG_CODE) =
{
  (SI_VARIABLE_SEGMENT_POINTER(, USB_DeviceDescriptor_TypeDef, SI_SEG_GENERIC))deviceDesc,              // deviceDescriptor
  (SI_VARIABLE_SEGMENT_POINTER(, USB_ConfigurationDescriptor_TypeDef, SI_SEG_GENERIC))configDesc,       // configDescriptor
  (SI_VARIABLE_SEGMENT_POINTER(, USB_StringTable_TypeDef, SI_SEG_GENERIC))myUsbStringTable_USEnglish,   // stringDescriptors
  sizeof(myUsbStringTable_USEnglish) / sizeof(myUsbStringTable_USEnglish[0])                            // numberOfStrings
};
