//-----------------------------------------------------------------------------
// F3xx_USB0_ReportHandler.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
//
// Target:         C8051F3xx
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
// Project Name:   F3xx_BlinkyExample
//
//
// Release 1.2 (ES)
//    -Added support for Raisonance
//    -No change to this file
//    -02 APR 2010
// Release 1.1
//    -Added feature reports for dimming controls
//    -Added PCA dimmer functionality
//    -16 NOV 2006
// Release 1.0
//    -Initial Revision (PD)
//    -07 DEC 2005
//

#ifndef  _UUSB_REPORTHANDLER_H_
#define  _USB_REPORTHANDLER_H_


typedef struct {
   unsigned char ReportID;
   void (*hdlr)();
} VectorTableEntry;

typedef struct{
   unsigned char Length;
   unsigned char* Ptr;
} BufferStructure;

extern void ReportHandler_IN_ISR(unsigned char);
extern void ReportHandler_IN_Foreground(unsigned char);
extern void ReportHandler_OUT(unsigned char);
extern void Setup_OUT_BUFFER(void);

extern BufferStructure IN_BUFFER, OUT_BUFFER;

#endif