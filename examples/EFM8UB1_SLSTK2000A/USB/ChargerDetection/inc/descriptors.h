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
#define USB_VENDOR_ID                     htole16(USB_VENDOR_ID_SILICON_LABS)
#define USB_PRODUCT_ID                    htole16(0x89D7)

// Endpoint address of the HID keyboard IN endpoint
#define KEYBOARD_IN_EP_ADDR   EP1IN

// Interface number of the HID keyboard
#define HID_KEYBOARD_IFC                  0

// Interface number of the HID Mouse
#define HID_MOUSE_IFC                  0

#define REPORT_DESCRIPTOR_SIZE 69

extern SI_SEGMENT_VARIABLE(ReportDescriptor[REPORT_DESCRIPTOR_SIZE], const uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(deviceDesc[], const USB_DeviceDescriptor_TypeDef, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(configDesc[], const uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(initstruct, const USBD_Init_TypeDef, SI_SEG_CODE);

typedef uint8_t KeyReport_TypeDef[ 8 ];
extern const KeyReport_TypeDef SI_SEG_CODE noKeyReport;
extern const KeyReport_TypeDef SI_SEG_CODE reportTable[];

#ifdef __cplusplus
}
#endif

#endif  // #define __SILICON_LABS_DESCRIPTORS_H__
