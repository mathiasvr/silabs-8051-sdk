/////////////////////////////////////////////////////////////////////////////
// hidmouse.h
/////////////////////////////////////////////////////////////////////////////

#ifndef __HIDMOUSE_H__
#define __HIDMOUSE_H__

/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////

// Min/Max speed in pixels per reporting interval (8 ms)
#define MOUSE_MIN_SPEED    1
#define MOUSE_MAX_SPEED    10

// Controls when mouse starts and stops accelerating from min to max
// speed in ms
#define MOUSE_ACC_START_TIME  250
#define MOUSE_ACC_STOP_TIME   1250

// Endpoint address of the HID Mouse IN endpoint
#define MOUSE_IN_EP_ADDR   EP1IN

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

#endif
