//-----------------------------------------------------------------------------
// F3xx_USB0_Standard_Requests.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Stub file for Firmware Template.
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
// Release 1.0
//    -Initial Revision (PD)
//    -04 JUN 2008
//

#ifndef _USB_STD_REQ_H_
#define _USB_STD_REQ_H_

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Get_Status(void);
void Clear_Feature(void);
void Set_Feature(void);
void Set_Address(void);
void Get_Descriptor(void);
void Get_Configuration(void);
void Set_Configuration(void);
void Get_Interface(void);
void Set_Interface(void);

#endif // _USB_STD_REQ_H_
