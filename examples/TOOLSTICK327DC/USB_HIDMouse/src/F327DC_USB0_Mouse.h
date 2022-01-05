//-----------------------------------------------------------------------------
// F3xx_USB0_Mouse.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Header file for mouse routines.
//
//
// How To Test:    See Readme.txt
//
//
// Target:         ToolStick327DC
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Project Name:   TS_F326-7_USB_HIDMouse
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 20 MAY 2014
//
// Release 1.0
//    -Initial Revision (PD)
//    -27 MAR 2008
//
#ifndef  _MOUSE_H_
#define  _MOUSE_H_

#include "SI_C8051F326_Register_Enums.h"

extern signed char MOUSE_VECTOR;
extern uint8_t MOUSE_AXIS;
extern uint8_t MOUSE_BUTTON;
uint8_t IN_PACKET[];

#define X_Axis 0
#define Y_Axis 1

void System_Init(void);
void USB0_Init(void);

#endif  /* _MOUSE_H_ */
