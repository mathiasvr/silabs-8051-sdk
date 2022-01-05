
//-----------------------------------------------------------------------------
// F3xx_USB0_Descriptor.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Header file for USB firmware. Includes all USB core register
// addresses, register access macros, and register bit masks.
//
//
// How To Test:    See Readme.txt
//
//
// Target:         ToolStick342DC
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Project Name:   F3xx_BlinkyExample
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

#ifndef  _USB_REGS_H_
#define  _USB_REGS_H_

// USB Core Registers
#define  BASE     0x00
#define  EINCSR1  BASE + 0x11
#define  EINCSR2  BASE + 0x12
#define  EOUTCSR1 BASE + 0x14
#define  EOUTCSR2 BASE + 0x15
#define  FIFO_EP0 BASE + 0x20
#define  FIFO_EP1 BASE + 0x21
#define  FIFO_EP2 BASE + 0x22
#define  FIFO_EP3 BASE + 0x23

// USB Core Register Bits

// POWER
#define  rbISOUD        0x80
#define  rbSPEED        0x40
#define  rbUSBRST       0x08
#define  rbRESUME       0x04
#define  rbSUSMD        0x02
#define  rbSUSEN        0x01

// IN1INT
#define  rbIN3          0x08
#define  rbIN2          0x04
#define  rbIN1          0x02
#define  rbEP0          0x01

// OUT1INT
#define  rbOUT3         0x08
#define  rbOUT2         0x04
#define  rbOUT1         0x02

// CMINT
#define  rbSOF          0x08
#define  rbRSTINT       0x04
#define  rbRSUINT       0x02
#define  rbSUSINT       0x01

// IN1IE
#define  rbIN3E         0x08
#define  rbIN2E         0x04
#define  rbIN1E         0x02
#define  rbEP0E         0x01

// OUT1IE
#define  rbOUT3E        0x08
#define  rbOUT2E        0x04
#define  rbOUT1E        0x02

// CMIE
#define  rbSOFE         0x08
#define  rbRSTINTE      0x04
#define  rbRSUINTE      0x02
#define  rbSUSINTE      0x01

// E0CSR
#define  rbSSUEND       0x80
#define  rbSOPRDY       0x40
#define  rbSDSTL        0x20
#define  rbSUEND        0x10
#define  rbDATAEND      0x08
#define  rbSTSTL        0x04
#define  rbINPRDY       0x02
#define  rbOPRDY        0x01

// EINCSR1
#define  rbInCLRDT      0x40
#define  rbInSTSTL      0x20
#define  rbInSDSTL      0x10
#define  rbInFLUSH      0x08
#define  rbInUNDRUN     0x04
#define  rbInFIFONE     0x02
#define  rbInINPRDY     0x01

// EINCSR2
#define  rbInDBIEN      0x80
#define  rbInISO        0x40
#define  rbInDIRSEL     0x20
#define  rbInFCDT       0x08
#define  rbInSPLIT      0x04

// EOUTCSR1
#define  rbOutCLRDT     0x80
#define  rbOutSTSTL     0x40
#define  rbOutSDSTL     0x20
#define  rbOutFLUSH     0x10
#define  rbOutDATERR    0x08
#define  rbOutOVRUN     0x04
#define  rbOutFIFOFUL   0x02
#define  rbOutOPRDY     0x01

// EOUTCSR2
#define  rbOutDBOEN     0x80
#define  rbOutISO       0x40

// Register read/write macros

// These first two macros do not poll for busy, and can be used to increase
// program speed where neccessary, but should never be used for successive
// reads or writes.
#define READ_BYTE(addr, target) USB0ADR = (0x80 | addr); \
                                while (USB0ADR & 0x80); target = USB0DAT
#define WRITE_BYTE(addr, data) USB0ADR = (addr); USB0DAT = data

// These two macros are polling versions of the above macros, and can be
// used for successive reads/writes without taking the chance that the
// Interface Engine is busy from the last register access.
#define POLL_READ_BYTE(addr, target) while(USB0ADR & 0x80); \
                                     READ_BYTE(addr, target);
#define POLL_WRITE_BYTE(addr, data) while(USB0ADR & 0x80);  \
                                    WRITE_BYTE(addr, data);

#endif /* _USB_REGS_H_ */