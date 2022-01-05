/////////////////////////////////////////////////////////////////////////////
// descriptors.h
/////////////////////////////////////////////////////////////////////////////

#ifndef __SILICON_LABS_DESCRIPTORS_H__
#define __SILICON_LABS_DESCRIPTORS_H__

#include <endian.h>
#include <efm8_usb.h>

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
#define USB_PRODUCT_ID                     htole16(0xb)
// [Product ID]$

// Interface number of the HID Joystick
#define HID_JOYSTICK_IFC                  0


extern SI_SEGMENT_VARIABLE(ReportDescriptor0[50], const uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(deviceDesc[], const USB_DeviceDescriptor_TypeDef, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(configDesc[], const uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(initstruct, const USBD_Init_TypeDef, SI_SEG_CODE);

#ifdef __cplusplus
}
#endif

#endif  // #define __SILICON_LABS_DESCRIPTORS_H__
