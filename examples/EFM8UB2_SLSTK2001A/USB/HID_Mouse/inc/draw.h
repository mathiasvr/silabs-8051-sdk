/////////////////////////////////////////////////////////////////////////////
// draw.h
/////////////////////////////////////////////////////////////////////////////

#ifndef __DRAW_H__
#define __DRAW_H__

/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////
#define TEXT_AREA_X1                16
#define TEXT_AREA_X2                112
#define TEXT_AREA_Y1                73
#define TEXT_AREA_Y2                116

#define ARROW_POS_Y1                        89

#define TEXT_LINE1_Y1                       80
#define TEXT_LINE1_Y2                       (TEXT_LINE1_Y1 + FONT_HEIGHT)
#define TEXT_LINE2_Y1                       96
#define TEXT_LINE2_Y2                       (TEXT_LINE2_Y1 + FONT_HEIGHT)

#define JOYSTICK_STATUS_NONE                0
#define JOYSTICK_STATUS_UP                  1
#define JOYSTICK_STATUS_UP_RIGHT            2
#define JOYSTICK_STATUS_RIGHT               3
#define JOYSTICK_STATUS_DOWN_RIGHT          4
#define JOYSTICK_STATUS_DOWN                5
#define JOYSTICK_STATUS_DOWN_LEFT           6
#define JOYSTICK_STATUS_LEFT                7
#define JOYSTICK_STATUS_UP_LEFT             8
#define JOYSTICK_STATUS_LEFT_BUTTON         9
#define JOYSTICK_STATUS_RIGHT_BUTTON        10

#define MULTI_BUTTON_STR_FORMAT             "  -/-/- Button"
#define MULTI_BUTTON_STR_LEFT_OFFSET        2
#define MULTI_BUTTON_STR_RIGHT_OFFSET       4
#define MULTI_BUTTON_STR_CENTER_OFFSET      6

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

void DrawScreenText(SI_VARIABLE_SEGMENT_POINTER(str, char, SI_SEG_GENERIC),
                    uint8_t rowNum,
                    uint8_t fontScale);
void DrawMouseStatus(void);
void DrawSplashScreen(void);
#endif
