//-----------------------------------------------------------------------------
// F3xx_USB0_ReportHandler.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Includes functions called by USB_ISR.c to handle input and output reports.//
//
// How To Test:    See Readme.txt
//
//
// Target:         ToolStick381DC
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Project Name:   F3xx_MouseExample
//
//
// Release 1.2 (BL)
//    - Updated Description / How to Test
//    - 21 MAR 2014
//
// Release 1.1
//    -Minor code comment changes
//    -16 NOV 2006
// Release 1.0
//    -Initial Revision (PD)
//    -07 DEC 2005
//

#ifndef  _UUSB_REPORTHANDLER_H_
#define  _USB_REPORTHANDLER_H_

#include "si_toolchain.h"

typedef struct {
   uint8_t ReportID;
   void (*hdlr)();
} VectorTableEntry;

typedef struct{
   uint8_t Length;
   uint8_t * Ptr;
} BufferStructure;

extern void ReportHandler_IN_ISR(uint8_t);
extern void ReportHandler_IN_Foreground(uint8_t);
extern void ReportHandler_OUT(uint8_t);
extern void Setup_OUT_BUFFER(void);

extern BufferStructure IN_BUFFER, OUT_BUFFER;

#endif
