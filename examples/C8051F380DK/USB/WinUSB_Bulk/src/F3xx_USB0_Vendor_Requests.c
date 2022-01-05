//-----------------------------------------------------------------------------
// F3xx_USB0_Vendor_Requests.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Handle vendor specific control requests.
//
//
// How To Test:    See Readme.txt
//
//
// Target:         C8051F320/1,
//                 C8051F326/7,
//                 C8051F34x,
//                 C8051F38x
//
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.6
// Command Line:   See Readme.txt
// Project Name:   USB0_Bulk
//
// Release 2.0
//    -Rewrite (CM)
//    -02 NOV 2012
//

//-----------------------------------------------------------------------------
// Header Files
//-----------------------------------------------------------------------------

#include "c8051f3xx.h"
#include "F3xx_USB0_Register.h"
#include "F3xx_USB0_Descriptor.h"
#include "F3xx_USB0_InterruptServiceRoutine.h"
#include "F3xx_USB0_Vendor_Requests.h"
#include "F3xx_USB0_Main.h"
#include "F3xx_USB0_Bulk.h"

//-----------------------------------------------------------------------------
// Global Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Handle_Vendor
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Handle vendor-specific control requests
// - Decode Incoming Setup requests
// - Load data packets on fifo while in transmit mode
//
//-----------------------------------------------------------------------------
void Handle_Vendor ()
{
   switch (Setup.bRequest)
   {
      case REQ_RST_STATE:
         Reset_State ();
         break;
      default:
         Force_Stall ();      // Send stall to host if invalid request
         break;
   }
}

//-----------------------------------------------------------------------------
// Reset_State
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Reset device state machine to idle. Optionally unlock the flash write/erase
// interface by resetting the device.
//
//-----------------------------------------------------------------------------
void Reset_State ()
{
   uint8_t reset = 0;

   // Send procedural stall if device isn't configured
   // Or request is made to host(remote wakeup not supported)
   // Or request is made to interface
   // Or msbs of value or index set to non-zero value
   // Or data length set to non-zero.
   if ((USB0_State != DEV_CONFIGURED) ||
       (Setup.bmRequestType != (DIR_OUT | REQ_TYPE_VENDOR | RECP_DEVICE)) ||
       (Setup.wIndex.u16 != 0) ||
       (Setup.wLength.u16 != 0) ||
       (Setup.wValue.u16 > 1))
   {
      Force_Stall ();
   }
   else
   {
      // Signal the foreground to transition to the IDLE state
      AsyncResetState = 1;

      // Flash write/erase interface is locked
      if (Setup.wValue.u16 == VALUE_UNLOCK && FLKEY == 0x03)
      {
         reset = 1;
      }
   }

   // Reset Index to 0
   POLL_WRITE_BYTE (INDEX, 0);

   if (EP_Status[0] != EP_STALL)
   {
      // Set Serviced Out packet ready and
      // data end to indicate transaction
      // is over
      POLL_WRITE_BYTE (E0CSR, (rbSOPRDY | rbDATAEND));
   }

   if (reset)
   {
      // Wait to allow the control transfer
      // to finish before resetting the device
      Delay ();

      // Perform software reset
      RSTSRC |= 0x10;
   }
}
