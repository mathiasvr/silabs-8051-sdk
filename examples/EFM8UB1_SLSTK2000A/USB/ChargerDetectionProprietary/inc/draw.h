/////////////////////////////////////////////////////////////////////////////
// draw.h
/////////////////////////////////////////////////////////////////////////////

#ifndef __DRAW_H__
#define __DRAW_H__

/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////
#define TEXT_AREA_X1 16
#define TEXT_AREA_X2 112
#define TEXT_AREA_Y1 73
#define TEXT_AREA_Y2 116

#define ARROW_POS    89

#define MOUSE_STATUS0  0
#define MOUSE_STATUS1  1
#define MOUSE_STATUS2  2
#define MOUSE_STATUS3  3
#define MOUSE_STATUS4  4
#define MOUSE_STATUS5  5
#define MOUSE_STATUS6  6

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

extern void DrawScreenText(uint8_t * str, uint8_t rowNum, uint8_t fontScale);
extern void DrawSplashScreen(void);
#endif
