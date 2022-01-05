/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef __SILICON_LABS_USBCONFIG_H__
#define __SILICON_LABS_USBCONFIG_H__

// -----------------------------------------------------------------------------
// Specify bus- or self-powered
// -----------------------------------------------------------------------------
#define SLAB_USB_BUS_POWERED              0

// -----------------------------------------------------------------------------
// Specify USB speed
// -----------------------------------------------------------------------------
#define SLAB_USB_FULL_SPEED               1

// -----------------------------------------------------------------------------
// Enable or disable the clock recovery
// -----------------------------------------------------------------------------
#define SLAB_USB_CLOCK_RECOVERY_ENABLED   1

// -----------------------------------------------------------------------------
// Enable or disable remote wakeup
// -----------------------------------------------------------------------------
#define SLAB_USB_REMOTE_WAKEUP_ENABLED    0

// -----------------------------------------------------------------------------
// Specify number of interfaces and whether any interfaces support alternate
// settings
// -----------------------------------------------------------------------------
#define SLAB_USB_NUM_INTERFACES           1
#define SLAB_USB_SUPPORT_ALT_INTERFACES   0

// -----------------------------------------------------------------------------
// Enable or disable each endpoint
// -----------------------------------------------------------------------------
#define SLAB_USB_EP1IN_USED               1
#define SLAB_USB_EP1OUT_USED              0
#define SLAB_USB_EP2IN_USED               0
#define SLAB_USB_EP2OUT_USED              0
#define SLAB_USB_EP3IN_USED               0
#define SLAB_USB_EP3OUT_USED              0

// -----------------------------------------------------------------------------
// Specify maximum packet size for each endpoint
// -----------------------------------------------------------------------------
#define SLAB_USB_EP1IN_MAX_PACKET_SIZE    64
#define SLAB_USB_EP1OUT_MAX_PACKET_SIZE   0
#define SLAB_USB_EP2IN_MAX_PACKET_SIZE    0
#define SLAB_USB_EP2OUT_MAX_PACKET_SIZE   0
#define SLAB_USB_EP3IN_MAX_PACKET_SIZE    0
#define SLAB_USB_EP3OUT_MAX_PACKET_SIZE   0

// -----------------------------------------------------------------------------
// Specify transfer type of each endpoint
// -----------------------------------------------------------------------------
#define SLAB_USB_EP1IN_TRANSFER_TYPE      USB_EPTYPE_INTR
#define SLAB_USB_EP1OUT_TRANSFER_TYPE     USB_EPTYPE_BULK
#define SLAB_USB_EP2IN_TRANSFER_TYPE      USB_EPTYPE_INTR
#define SLAB_USB_EP2OUT_TRANSFER_TYPE     USB_EPTYPE_BULK
#define SLAB_USB_EP3IN_TRANSFER_TYPE      USB_EPTYPE_ISOC
#define SLAB_USB_EP3OUT_TRANSFER_TYPE     USB_EPTYPE_ISOC

// -----------------------------------------------------------------------------
// Enable or disable callback functions
// -----------------------------------------------------------------------------
#define SLAB_USB_RESET_CB                 1
#define SLAB_USB_SOF_CB                   1
#define SLAB_USB_STATE_CHANGE_CB          1
#define SLAB_USB_IS_SELF_POWERED_CB       1
#define SLAB_USB_SETUP_CMD_CB             1
#define SLAB_USB_HANDLER_CB               0

// -----------------------------------------------------------------------------
// Specify number of languages supported by string descriptors
// -----------------------------------------------------------------------------
#define SLAB_USB_NUM_LANGUAGES            1

// -----------------------------------------------------------------------------
// If only one descriptor language is supported, specify that language here.
// If multiple descriptor languages are supported, this value is ignored and
// the supported languages must listed in the
// myUsbStringTableLanguageIDsDescriptor structure.
// -----------------------------------------------------------------------------
#define SLAB_USB_LANGUAGE                 USB_LANGID_ENUS

// -----------------------------------------------------------------------------
// Set the power saving mode
//
// SLAB_USB_PWRSAVE_MODE configures when the device will automatically enter
// the USB power-save mode. It is a bitmask constant with bit values:
//
//   USB_PWRSAVE_MODE_OFF       - No energy saving mode selected
//   USB_PWRSAVE_MODE_ONSUSPEND - Enter USB power-save mode on USB suspend
//   USB_PWRSAVE_MODE_ONVBUSOFF - Enter USB power-save mode when not attached
//                                to the USB host.
//   USB_PWRSAVE_MODE_FASTWAKE  - Exit USB power-save mode more quickly, but
//                                consume more power while in USB power-save
//                                mode.
//                                While the device is in USB power-save mode
//                                (typically during USB suspend), the
//                                internal voltage regulator stays in normal
//                                power mode instead of entering suspend
//                                power mode.
//                                This is an advanced feature that may be
//                                useful in certain applications that support
//                                remote wakeup.
// -----------------------------------------------------------------------------
#define SLAB_USB_PWRSAVE_MODE             (USB_PWRSAVE_MODE_ONVBUSOFF \
                                           | USB_PWRSAVE_MODE_ONSUSPEND)

// -----------------------------------------------------------------------------
// Enable or disable polled mode
//
// When enabled, the application must call USBD_Run() periodically to process
// USB events.
// When disabled, USB events will be handled automatically by an interrupt
// handler.
// -----------------------------------------------------------------------------
#define SLAB_USB_POLLED_MODE              0

#endif // __SILICON_LABS_USBCONFIG_H__
