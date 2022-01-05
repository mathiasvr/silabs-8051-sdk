#ifndef CLOCK_H_
#define CLOCK_H_

#include <si_toolchain.h>
#include "tick.h"
#include "disp.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define DS_BLANK                        0
#define DS_ANALOG                       1
#define DS_DIGITAL                      2

#define BLANK                           60

#define CENTER_CIRCLE_X                 63
#define CENTER_CIRCLE_Y                 63

#define SECONDS_RADIUS	                43
#define MINUTES_RADIUS	                38
#define HOURS_RADIUS	                28

#define SIN_TABLE_SIZE                  91

#define TOTAL_DEGREES_CIRCLE            360
#define TOTAL_NUM_SECONDS               60
#define TOTAL_NUM_HOURS                 12
#define TOTAL_NUM_MINUTES               60

#define STRING_POSX		                15
#define STRING_POSY 	                55

#define LCD_MIDDLE_LINE		            64

//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------

void ClearBackground();
void DrawAnalogClockBackground();
void AnalogTimeDraw(uint8_t hours, uint8_t minutes, uint8_t seconds);
void DigitalTimeDraw(uint8_t hours, uint8_t minutes, uint8_t seconds);

#endif /* CLOCK_H_ */
