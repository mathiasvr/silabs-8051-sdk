/////////////////////////////////////////////////////////////////////////////
// button.h
/////////////////////////////////////////////////////////////////////////////

#ifndef __BUTTON_H__
#define __BUTTON_H__

/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////

#define LEFT_BUTTON         (1 << 0)
#define RIGHT_BUTTON        (1 << 1)
#define CENTER_BUTTON       (1 << 2)
#define JOY_UP		          (1 << 3)
#define JOY_DOWN	          (1 << 4)
#define JOY_LEFT	          (1 << 5)
#define JOY_RIGHT	          (1 << 6)

#define JOY_MASK            (JOY_UP | JOY_DOWN | JOY_LEFT | JOY_RIGHT)
#define BUTTON_MASK         (LEFT_BUTTON | RIGHT_BUTTON | CENTER_BUTTON)
// P0.2 and P0.3
#define LEFT_BUTTON_PIN		P0_B3
#define RIGHT_BUTTON_PIN	P0_B2


/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

uint8_t Joystick_GetStatus(void);

#endif
