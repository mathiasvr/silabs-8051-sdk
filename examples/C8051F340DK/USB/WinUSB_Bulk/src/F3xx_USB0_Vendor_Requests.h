//-----------------------------------------------------------------------------
// F3xx_USB0_Vendor_Requests.h
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

#ifndef _USB_VENDOR_REQ_H_
#define _USB_VENDOR_REQ_H_

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------

#define REQ_RST_STATE      0x01        // Reset state machine
#define VALUE_DEFAULT      0x0000      // Leave flash interface unchanged
#define VALUE_UNLOCK       0x0001      // Unlock flash interface

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Handle_Vendor (void);
void Reset_State (void);

#endif // _USB_VENDOR_REQ_H_
