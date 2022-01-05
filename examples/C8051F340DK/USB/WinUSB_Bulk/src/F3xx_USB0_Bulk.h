//-----------------------------------------------------------------------------
// F3xx_USB0_Bulk.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Device specific initialization routines.
//
//
// How To Test:    See Readme.txt
//
//
// Target:         C8051F38x
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.6
// Command Line:   See Readme.txt
// Project Name:   USB0_Bulk
//
// Release 2.0
//    -Rewrite (CM)
//    -02 NOV 2012
//
// Release 1.1
//    -All changes by BW
//    -1 SEP 2010
//    -Updated USB0_Init() to write 0x89 to CLKREC instead of 0x80
//
// Release 1.0
//    -Initial Revision (PD)
//    -04 JUN 2008
//

#ifndef _USB_BULK_H_
#define _USB_BULK_H_

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void System_Init (void);
void USB0_Init (void);
void USB0_Suspend (void);
void Delay (void);
void SetLed (uint8_t led, uint8_t on);

#endif // _USB_BULK_H_
