/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// hidjoystick.c
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "bsp.h"
#include "retargetserial.h"
#include "efm8_usb.h"
#include "hidjoystick.h"
#include "descriptors.h"
#include "idle.h"
#include "button.h"

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------
extern uint8_t tmpBuffer;

struct joystickReportData
{
  uint8_t Button; /**< Button mask for currently pressed buttons in the game pad. */
  uint8_t X;
  uint8_t Y;
} joystickReportData;

static struct joystickReportData prevJoystickReportData;

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// CreateJoystickReport() Routine
// ----------------------------------------------------------------------------
//
// Description - Generate joystick status report according Joystick and Button
//               status.
//
//-----------------------------------------------------------------------------
void CreateJoystickReport(void)
{
  uint8_t joyStatus = Joystick_GetStatus();

  memset(&joystickReportData, 0, sizeof(joystickReportData));

  if (joyStatus & JOY_UP)
  {
    joystickReportData.Y = 0xFF;
  }
  else if (joyStatus & JOY_DOWN)
  {
    joystickReportData.Y = 0x01;
  }

  if (joyStatus & JOY_LEFT)
  {
    joystickReportData.X = 0xFF;
  }
  else if (joyStatus & JOY_RIGHT)
  {
    joystickReportData.X = 0x01;
  }

  joystickReportData.Button = joyStatus & BUTTON_MASK;
}

#if SLAB_USB_HANDLER_CB
void USBD_EnterHandler(void)
{
  CLKSEL = CLKSEL_CLKSL__HFOSC;
}

void USBD_ExitHandler(void)
{
  CLKSEL = CLKSEL_CLKSL__LFOSC;
}
#endif

#if SLAB_USB_RESET_CB
void USBD_ResetCb(void)
{

}
#endif // SLAB_USB_RESET_CB

#if SLAB_USB_SOF_CB
void USBD_SofCb(uint16_t sofNr)
{
  bool sendReport = 0;
  int8_t status;

  UNREFERENCED_ARGUMENT(sofNr);

  CreateJoystickReport();

  /* Check to see if the report data has changed - if so a report MUST be sent */
  sendReport = prevJoystickReportData.Button != joystickReportData.Button ||
               prevJoystickReportData.X != joystickReportData.X ||
               prevJoystickReportData.Y != joystickReportData.Y;

  /* HID joystick device sends report to host with idle tick count period interval */
  idleTimerTick();

  // Check if the device should send a report
  if (isIdleTimerExpired() == true)
  {
    sendReport = true;
  }

  if (sendReport)
  {
    status = USBD_Write(JOYSTICK_IN_EP_ADDR,
                        (uint8_t *) &joystickReportData,
                        sizeof(joystickReportData),
                        false);

    // Since the host polls slower than the 1 ms SOF rate,
    // the EP may be busy, so not all writes complete successfully
    if (status == USB_STATUS_OK)
    {
      prevJoystickReportData = joystickReportData;
    }
  }
}
#endif // SLAB_USB_SOF_CB

#if SLAB_USB_STATE_CHANGE_CB
void USBD_DeviceStateChangeCb(USBD_State_TypeDef oldState,
                              USBD_State_TypeDef newState)
{
  // Entering suspend mode, power internal and external blocks down
  if (newState == USBD_STATE_SUSPENDED)
  {
  }
  // Exiting suspend mode, power internal and external blocks up
  if (oldState == USBD_STATE_SUSPENDED)
  {
  }
  if (newState == USBD_STATE_CONFIGURED)
  {
    idleSetDuration(DEFAULT_IDLE_RATE_MS / 4);
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
      && (setup->bmRequestType.Recipient == USB_SETUP_RECIPIENT_INTERFACE))
  {
    // A HID device must extend the standard GET_DESCRIPTOR command
    // with support for HID descriptors.
    switch (setup->bRequest)
    {
      case GET_DESCRIPTOR:
        if ((setup->wValue >> 8) == USB_HID_REPORT_DESCRIPTOR)
        {
          switch (setup->wIndex)
          {
            case 0: // Interface 0

              USBD_Write(EP0,
                         (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, SI_SEG_GENERIC))ReportDescriptor0,
                         EFM8_MIN(sizeof(ReportDescriptor0), setup->wLength),
                         false);
              retVal = USB_STATUS_OK;
              break;

            default: // Unhandled Interface
              break;
          }
        }
        else if ((setup->wValue >> 8) == USB_HID_DESCRIPTOR)
        {
          switch (setup->wIndex)
          {
            case 0: // Interface 0

              USBD_Write(EP0,
                         (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, SI_SEG_GENERIC))(&configDesc[18]),
                         EFM8_MIN(USB_HID_DESCSIZE, setup->wLength),
                         false);
              retVal = USB_STATUS_OK;
              break;

            default: // Unhandled Interface
              break;
          }
        }
        break;
    }
  }

  else if ((setup->bmRequestType.Type == USB_SETUP_TYPE_CLASS)
           && (setup->bmRequestType.Recipient == USB_SETUP_RECIPIENT_INTERFACE)
           && (setup->wIndex == HID_JOYSTICK_IFC))
  {
    // Implement the necessary HID class specific commands.
    switch (setup->bRequest)
    {
      case USB_HID_SET_REPORT:
        if (((setup->wValue >> 8) == 2)               // Output report
            && ((setup->wValue & 0xFF) == 0)          // Report ID
            && (setup->wLength == 1)                  // Report length
            && (setup->bmRequestType.Direction != USB_SETUP_DIR_IN))
        {
          USBD_Read(EP0, &tmpBuffer, 1, true);
          retVal = USB_STATUS_OK;
        }
        break;

      case USB_HID_GET_REPORT:
        if (((setup->wValue >> 8) == 1)               // Input report
            && ((setup->wValue & 0xFF) == 0)          // Report ID
            && (setup->wLength == 8)                  // Report length
            && (setup->bmRequestType.Direction == USB_SETUP_DIR_IN))
        {
          CreateJoystickReport();
          USBD_Write(EP0,
                     (uint8_t *) &joystickReportData,
                     sizeof(joystickReportData),
                     false);
          retVal = USB_STATUS_OK;
        }
        break;

      case USB_HID_SET_IDLE:
        // Report ID
        // 0  - Idle duration applies to all report IDs
        // >0 - Idle duration applies to specified report ID only (not supported)
        if (((setup->wValue & 0xFF) == 0)
            && (setup->wLength == 0)
            && (setup->bmRequestType.Direction != USB_SETUP_DIR_IN))
        {
          // Set the idle duration in units of 4 ms
          //idleSetDuration(setup->wValue >> 8);
          retVal = USB_STATUS_OK;
        }
        break;

      case USB_HID_GET_IDLE:
        if ((setup->wValue == 0)                      // Report ID
            && (setup->wLength == 1)
            && (setup->bmRequestType.Direction == USB_SETUP_DIR_IN))
        {
          tmpBuffer = idleGetDuration();
          USBD_Write(EP0, &tmpBuffer, 1, false);
          retVal = USB_STATUS_OK;
        }
        break;
    }
  }

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
  UNREFERENCED_ARGUMENT(epAddr);
  UNREFERENCED_ARGUMENT(status);
  UNREFERENCED_ARGUMENT(xferred);
  UNREFERENCED_ARGUMENT(remaining);
  /* We need do nothing for HID joystick example here */
  return 0;
}
