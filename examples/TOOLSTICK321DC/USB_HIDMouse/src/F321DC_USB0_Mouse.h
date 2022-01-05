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
// Target:         ToolStick321DC
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   See Readme.txt
// Project Name:   F3xx_MouseExample
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 21 MAR 2014
//
// Release 1.0
//    -Initial Revision (PD)
//    -27 MAR 2008
//
#ifndef  _MOUSE_H_
#define  _MOUSE_H_

extern signed char MOUSE_VECTOR;
extern uint8_t MOUSE_AXIS;
extern uint8_t MOUSE_BUTTON;
uint8_t IN_PACKET[];

#define X_Axis 0
#define Y_Axis 1

void System_Init(void);
void USB0_Init(void);

#endif  /* _MOUSE_H_ */