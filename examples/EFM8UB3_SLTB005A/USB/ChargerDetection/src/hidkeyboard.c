/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// hidkeyboard.c
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include <bsp.h>
#include <efm8_usb.h>
#include <descriptors.h>
#include <idle.h>
#include <button.h>

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------
#define numLock     BSP_LED_G
#define capsLock    BSP_LED_R

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------
uint8_t      keySeqNo;           // Current position in report table.
bool         keyPushed;          // Current pushbutton status.
int8_t       lastReportIndex;    // Last report sent to host.
uint8_t      tmpBuffer;

// Empty report
SI_SEGMENT_VARIABLE(noKeyReport, const KeyReport_TypeDef, SI_SEG_CODE) =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// A sequence of keystroke input reports.
SI_SEGMENT_VARIABLE(reportTable[], const KeyReport_TypeDef, SI_SEG_CODE) =
{
	{ 0x02, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 'H'
	{ 0x02, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 'I'
	{ 0x02, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 'D'
	{ 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00 },  // space
	{ 0x02, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 'K'
	{ 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 'e'
	{ 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 'y'
	{ 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 'b'
	{ 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 'o'
	{ 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 'a'
	{ 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 'r'
	{ 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 'd'
	{ 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00 },  // space
};

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
#if SLAB_USB_RESET_CB
void USBD_ResetCb(void)
{

}
#endif // SLAB_USB_RESET_CB

#if SLAB_USB_SOF_CB
void USBD_SofCb(uint16_t sofNr)
{
	static bool keyReleased = 0;

	UNREFERENCED_ARGUMENT(sofNr);

	idleTimerTick();

	// Check if the device should send a report
	if (isIdleTimerExpired() == true) {
		keyPushed = Buttons_GetStatus() ? true : false;
		if (keyPushed == true) {
			keyPushed = 0;
			keyReleased = 1;
			USBD_Write(KEYBOARD_IN_EP_ADDR,
                 (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, SI_SEG_GENERIC))&reportTable[keySeqNo],
                 sizeof(KeyReport_TypeDef),
                 false);

			keySeqNo++;
			if (keySeqNo == (sizeof(reportTable) / sizeof(KeyReport_TypeDef))) {
				keySeqNo = 0;
			}
		} else if ((isIdleTimerIndefinite() == false) || (keyReleased == true)) {
			keyReleased = 0;
			USBD_Write(KEYBOARD_IN_EP_ADDR,
                 (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, SI_SEG_GENERIC))&noKeyReport,
                 sizeof(KeyReport_TypeDef),
                 false);
		}
	}
}
#endif // SLAB_USB_SOF_CB

#if SLAB_USB_STATE_CHANGE_CB
void USBD_DeviceStateChangeCb(USBD_State_TypeDef oldState,
                              USBD_State_TypeDef newState)
{
	bool numLockSave, capsLockSave;

	// Entering suspend mode, power internal and external blocks down
	if (newState == USBD_STATE_SUSPENDED) {
		// Disable the LED's
		numLockSave = numLock;
		capsLockSave = capsLock;
		numLock = 0;
		capsLock = 0;
	}
	// Exiting suspend mode, power internal and external blocks up
	if (oldState == USBD_STATE_SUSPENDED) {
		// Restore the LED's to their previous values
		numLock = numLockSave;
		capsLock = capsLockSave;
	}
	if (newState == USBD_STATE_CONFIGURED) {
		idleTimerSet(POLL_RATE);
	}
}
#endif // SLAB_USB_STATE_CHANGE_CB

#if SLAB_USB_IS_SELF_POWERED_CB
bool USBD_IsSelfPoweredCb(void)
{
	return false;
}
#endif // SLAB_USB_IS_SELF_POWERED_CB

#if SLAB_USB_SETUP_CMD_CB
USB_Status_TypeDef USBD_SetupCmdCb(SI_VARIABLE_SEGMENT_POINTER(
                                     setup,
                                     USB_Setup_TypeDef,
                                     MEM_MODEL_SEG))
{
	USB_Status_TypeDef retVal = USB_STATUS_REQ_UNHANDLED;

	if ((setup->bmRequestType.Type == USB_SETUP_TYPE_STANDARD)
	        && (setup->bmRequestType.Direction == USB_SETUP_DIR_IN)
	        && (setup->bmRequestType.Recipient == USB_SETUP_RECIPIENT_INTERFACE)) {
		// A HID device must extend the standard GET_DESCRIPTOR command
		// with support for HID descriptors.
		switch (setup->bRequest) {
		case GET_DESCRIPTOR:
			if ((setup->wValue >> 8) == USB_HID_REPORT_DESCRIPTOR) {
				USBD_Write(EP0,
                   (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, SI_SEG_GENERIC))ReportDescriptor,
				           EFM8_MIN(sizeof(ReportDescriptor), setup->wLength),
                   false);
				retVal = USB_STATUS_OK;
			} else if ((setup->wValue >> 8) == USB_HID_DESCRIPTOR) {
				USBD_Write(EP0,
                   (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, SI_SEG_GENERIC))(configDesc + 17),
                   EFM8_MIN(USB_HID_DESCSIZE, setup->wLength),
                   false);
				retVal = USB_STATUS_OK;
			}
			break;
		}
	}

	else if ((setup->bmRequestType.Type == USB_SETUP_TYPE_CLASS)
	         && (setup->bmRequestType.Recipient == USB_SETUP_RECIPIENT_INTERFACE)
	         && (setup->wIndex == HID_KEYBOARD_IFC)) {
		// Implement the necessary HID class specific commands.
		switch (setup->bRequest) {
		case USB_HID_SET_REPORT:
			if (((setup->wValue >> 8) == 2)               // Output report
			        && ((setup->wValue & 0xFF) == 0)          // Report ID
			        && (setup->wLength == 1)                  // Report length
			        && (setup->bmRequestType.Direction != USB_SETUP_DIR_IN)) {
				USBD_Read(EP0, &tmpBuffer, 1, true);
				retVal = USB_STATUS_OK;
			}
			break;

		case USB_HID_GET_REPORT:
			if (((setup->wValue >> 8) == 1)               // Input report
			        && ((setup->wValue & 0xFF) == 0)          // Report ID
			        && (setup->wLength == 8)                  // Report length
			        && (setup->bmRequestType.Direction == USB_SETUP_DIR_IN)) {
				keyPushed = Buttons_GetStatus() ? true : false;
				if (keyPushed) {
					// Send a key pushed report
					USBD_Write(EP0,
                     (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, SI_SEG_GENERIC))&reportTable[keySeqNo],
                     sizeof(KeyReport_TypeDef),
                     false);
				} else {
					// Send an empty (key released) report
					USBD_Write(EP0,
                     (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, SI_SEG_GENERIC))&noKeyReport,
                     sizeof(KeyReport_TypeDef),
                     false);
				}
				retVal = USB_STATUS_OK;
			}
			break;

		case USB_HID_SET_IDLE:
			if (((setup->wValue & 0xFF) == 0)             // Report ID
			        && (setup->wLength == 0)
			        && (setup->bmRequestType.Direction != USB_SETUP_DIR_IN)) {
				idleTimerSet(setup->wValue >> 8);
				retVal = USB_STATUS_OK;
			}
			break;

		case USB_HID_GET_IDLE:
			if ((setup->wValue == 0)                      // Report ID
			        && (setup->wLength == 1)
			        && (setup->bmRequestType.Direction == USB_SETUP_DIR_IN)) {
				tmpBuffer = idleGetRate();
				USBD_Write(EP0, &tmpBuffer, 1, false);
				retVal = USB_STATUS_OK;
			}
			break;
		}
	}

	return retVal;
}
#endif // SLAB_USB_SETUP_CMD_CB

#if SLAB_USB_SUPPORT_ALT_INTERFACES
USB_Status_TypeDef USBD_SetInterfaceCb(uint8_t interface, uint8_t altSetting)
{
	USB_Status_TypeDef retVal = USB_STATUS_REQ_ERR;

	UNREFERENCED_ARGUMENT(interface);
	UNREFERENCED_ARGUMENT(altSetting);

	return retVal;
}
#endif // SLAB_USB_SUPPORT_ALT_INTERFACES

#if SLAB_USB_REMOTE_WAKEUP_ENABLED
void USBD_RemoteWakeupDelay(void)
{
	// Delay 10 - 15 ms here

}
#endif

uint16_t USBD_XferCompleteCb(uint8_t epAddr,
                             USB_Status_TypeDef status,
                             uint16_t xferred,
                             uint16_t remaining)
{
	UNREFERENCED_ARGUMENT(xferred);
	UNREFERENCED_ARGUMENT(remaining);

	// The only output reported supported is the SetReport to enable
	// Num Key and Caps Lock LED's.
	if ((epAddr == EP0) && (status == USB_STATUS_OK)) {
		numLock = tmpBuffer & 0x01;
		capsLock = tmpBuffer & 0x02;
	}

	return 0;
}
