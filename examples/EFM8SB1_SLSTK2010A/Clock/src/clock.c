/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// clock.c
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "clock.h"
#include "bsp.h"
#include "demo_config.h"
#include "render.h"
#include "utils.h"

#if SHOW_ANALOG_CLOCK_FACE
#include "clock_background.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////

uint8_t DrawState = DS_BLANK;

// Coordinates of the seconds dot
SI_SEGMENT_VARIABLE(SecondsDotX, uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(SecondsDotY, uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(MinutesDotX, uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(MinutesDotY, uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(HoursDotX, uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(HoursDotY, uint8_t, SI_SEG_XDATA);

// Line equation variables for minute hand: Y = m*X + b
SI_SEGMENT_VARIABLE(MinuteHand_x_slope, int8_t, SI_SEG_DATA);
SI_SEGMENT_VARIABLE(MinuteHand_y_slope, int8_t, SI_SEG_DATA);
SI_SEGMENT_VARIABLE(MinuteHand_b, int16_t, SI_SEG_DATA);

// Line equation variables for hour hand: Y = m*X + b
SI_SEGMENT_VARIABLE(HoursHand_x_slope, int8_t, SI_SEG_DATA);
SI_SEGMENT_VARIABLE(HoursHand_y_slope, int8_t, SI_SEG_DATA);
SI_SEGMENT_VARIABLE(HoursHand_b, int16_t, SI_SEG_DATA);

// Used to determine how to draw the line (increment x and calculate y
// or increment y and calculate x)
// For example, if the line is flat, then x should be incremented and y should be calculated
// and vice versa.
SI_SEGMENT_VARIABLE(FlatLineMinute, uint8_t, SI_SEG_DATA);
SI_SEGMENT_VARIABLE(FlatLineHours, uint8_t, SI_SEG_DATA);

uint8_t SecondsDotDrawn;

// Line buffer used to render and display analog clock pixels to LCD
SI_SEGMENT_VARIABLE(ClockLine[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);

/////////////////////////////////////////////////////////////////////////////
// Global Variables - Sin Table Constant
/////////////////////////////////////////////////////////////////////////////

// Sin table of angles up to 90 degrees.
// This table is scaled by 100 for to be more accurate when doing fixed point calculations
SI_SEGMENT_VARIABLE(SIN_TABLE[SIN_TABLE_SIZE], uint8_t, const SI_SEG_CODE)=
{
    0,
    2,
    3,
    5,
    7,
    9,
    10,
    12,
    14,
    16,
    17,
    19,
    21,
    22,
    24,
    26,
    28,
    29,
    31,
    33,
    34,
    36,
    37,
    39,
    41,
    42,
    44,
    45,
    47,
    48,
    50,
    52,
    53,
    54,
    56,
    57,
    59,
    60,
    62,
    63,
    64,
    66,
    67,
    68,
    69,
    71,
    72,
    73,
    74,
    75,
    77,
    78,
    79,
    80,
    81,
    82,
    83,
    84,
    85,
    86,
    87,
    87,
    88,
    89,
    90,
    91,
    91,
    92,
    93,
    93,
    94,
    95,
    95,
    96,
    96,
    97,
    97,
    97,
    98,
    98,
    98,
    99,
    99,
    99,
    99,
    100,
    100,
    100,
    100,
    100,
    100,
};

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

uint16_t ClkAngleToPolarAngle(uint16_t angle);
void GetXY(int16_t* unitCircleX, int16_t* unitCircleY, uint8_t radius, uint16_t angle);
void DrawAnalogClockBackground();
void GetLineEquation(
    SI_VARIABLE_SEGMENT_POINTER(slope_run, int8_t, SI_SEG_DATA),
    SI_VARIABLE_SEGMENT_POINTER(slope_rise, int8_t, SI_SEG_DATA),
    SI_VARIABLE_SEGMENT_POINTER(b, int16_t, SI_SEG_DATA),
    int16_t unitCircleX,
    int16_t unitCircleY,
    SI_VARIABLE_SEGMENT_POINTER(flatLine, uint8_t, SI_SEG_DATA));
void CopyLineOfBackground(uint8_t lineNum);
void DrawAnalogClockHands();
void ConvertToString(uint8_t integer, char* string);

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
// ClearBackground
//---------------------------------------------------------------------------
//
// Description - clear the LCD display
//
void ClearBackground()
{
    if (DrawState != DS_BLANK)
    {
        DISP_ClearAll();
        DrawState = DS_BLANK;
    }
}

//---------------------------------------------------------------------------
// DrawClockBackground
//---------------------------------------------------------------------------
//
// Description - draws the static analog clock background to LCD
//
void DrawAnalogClockBackground()
{
#if SHOW_ANALOG_CLOCK_FACE
    uint8_t i;

    for(i = 0; i < clock_background_height; i++)
    {
        DISP_WriteLine(i, (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, SI_SEG_GENERIC))&clock_background_bits[clock_background_width / 8 * i]);
    }
#else
    DISP_ClearAll();
#endif
}

//---------------------------------------------------------------------------
// AnalogTimeDraw
//---------------------------------------------------------------------------
//
// Description - Draws the analog mode of the clock
//
// hours - hours value to draw as the hour hand
// minutes - minutes value to draw as the minute hand
// seconds - seconds value to draw as a 2x2 dot
//
void AnalogTimeDraw(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	uint16_t secondsAngle, minutesAngle, hoursAngle;
	int16_t unitCircleX, unitCircleY;

	// Used to move hours hand throughout the hour
	// so the hours hand doesn't just move from one hour to next directly
	uint8_t hoursOffset;

	if (DrawState != DS_ANALOG)
	{
	    DrawAnalogClockBackground();
	    DrawState = DS_ANALOG;
	}

	hoursOffset = minutes / 10;

	SecondsDotDrawn = false;

	// Computes the angle based on the seconds value
	secondsAngle = ((TOTAL_DEGREES_CIRCLE * seconds) / TOTAL_NUM_SECONDS);
	// Converts the clock angle, which is 360 degrees at 12 o'clock,
	// to a unit circle angle. For example 12 o'clock would be 90 degrees.
	secondsAngle = ClkAngleToPolarAngle(secondsAngle);

	// Get the x and y coordinate of the seconds value based on the SECONDS_RADIUS
	// and the polar angle computed.
	// X = r * cos(angle)
	// Y = r * sin(angle)
	// Since there is only a sin table, cos(angle) is converted its equivalent sin(angle) to calculate X
	GetXY(&unitCircleX, &unitCircleY, SECONDS_RADIUS,  secondsAngle);

	// Center the x and y coordinate calculated previously
	// Y axis is subtracted between LCD is from 0 - 127 (top = 0, bottom = 127)
	// while Y values calculated is Cartesian. (Y is greater on the top and lower on the bottom)
	SecondsDotX = (uint8_t)(CENTER_CIRCLE_X + unitCircleX);
	SecondsDotY = (uint8_t)(CENTER_CIRCLE_Y - unitCircleY);

	minutesAngle = ((TOTAL_DEGREES_CIRCLE * minutes) / TOTAL_NUM_MINUTES);
	minutesAngle = ClkAngleToPolarAngle(minutesAngle);
	GetXY(&unitCircleX, &unitCircleY, MINUTES_RADIUS, minutesAngle);

    // Center the x and y coordinate calculated previously
    // Y axis is subtracted between LCD is from 0 - 127 (top = 0, bottom = 127)
    // while Y values calculated is Cartesian. (Y is greater on the top and lower on the bottom)
    MinutesDotX = (uint8_t)(CENTER_CIRCLE_X + unitCircleX);
    MinutesDotY = (uint8_t)(CENTER_CIRCLE_Y - unitCircleY);

	// Since minute is drawn as a hand, calculate the line equation for hand
	// based on the x and y values computed.
	// There is the center point and the x and y values computed, so with these
	// two points, we can calculate the equation of the line
	// Y = m * X + b
	GetLineEquation(&MinuteHand_x_slope
			, &MinuteHand_y_slope
			, &MinuteHand_b
			, unitCircleX
			, unitCircleY
			, &FlatLineMinute);

	hoursAngle = ((TOTAL_DEGREES_CIRCLE * hours) / TOTAL_NUM_HOURS);

	// Check for overflow of angle calculated when hours > 12
	if(hoursAngle >= 360)
	{
		hoursAngle -= 360;
	}

	hoursAngle = hoursAngle + (hoursOffset * 5);
	hoursAngle = ClkAngleToPolarAngle(hoursAngle);
	GetXY(&unitCircleX, &unitCircleY, HOURS_RADIUS, hoursAngle);

    // Center the x and y coordinate calculated previously
    // Y axis is subtracted between LCD is from 0 - 127 (top = 0, bottom = 127)
    // while Y values calculated is Cartesian. (Y is greater on the top and lower on the bottom)
    HoursDotX = (uint8_t)(CENTER_CIRCLE_X + unitCircleX);
    HoursDotY = (uint8_t)(CENTER_CIRCLE_Y - unitCircleY);

	GetLineEquation(&HoursHand_x_slope
			, &HoursHand_y_slope
			, &HoursHand_b
			, unitCircleX
			, unitCircleY
			, &FlatLineHours);

	// Draw analog clock
	// Saves space by only drawing up to the MINUTES_RADIUS so
	// seconds dot may sometimes be out of range
	DrawAnalogClockHands();
}

//---------------------------------------------------------------------------
// DigitalTimeDraw
//---------------------------------------------------------------------------
//
// Description - Draws the digital mode of the clock
//
// hours - hour to display
// minutes - minute to display
// seconds - second to display
//
// Format - the time is displayed in this centered format XX:XX:XX
void DigitalTimeDraw(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	uint8_t i;
	uint8_t j;
	uint8_t fontScale = 2;

	char string[9];

	if (DrawState != DS_DIGITAL)
	{
	    ClearBackground();
	    DrawState = DS_DIGITAL;
	}

	ConvertToString(hours, string);
	string[2] = ':';
	ConvertToString(minutes, &(string[3]));
	string[5] = ':';
	ConvertToString(seconds, &(string[6]));
	string[8] = 0;

	// Render and Display string to LCD
	for (i = 0; i < FONT_HEIGHT; i++)
	{
		RENDER_ClrLine(ClockLine);
		RENDER_Numeric(ClockLine, STRING_POSX, i, string, fontScale);
		for(j = 1; j < fontScale + 1; j++)
		{
			DISP_WriteLine(STRING_POSY + (i * fontScale) + j, ClockLine);
		}
	}
}

//---------------------------------------------------------------------------
// GetLineEquation
//---------------------------------------------------------------------------
//
// Description - computes the equation of a line given a point.
// 				 Uses the default center as the second point in the calculations
//
// slope_run - the computed x slope of the line
// slope_rise - the computed y slope of the line
// b - the computed b value of the line
// unitCircleX - the x value of the point
// unitCircleY - the y value of the point
// flatLine - set to true the line is too flat to increment y and calculated x
//			   since x varies so little
//
void GetLineEquation(
        SI_VARIABLE_SEGMENT_POINTER(slope_run, int8_t, SI_SEG_DATA),
        SI_VARIABLE_SEGMENT_POINTER(slope_rise, int8_t, SI_SEG_DATA),
        SI_VARIABLE_SEGMENT_POINTER(b, int16_t, SI_SEG_DATA),
        int16_t unitCircleX,
        int16_t unitCircleY,
        SI_VARIABLE_SEGMENT_POINTER(flatLine, uint8_t, SI_SEG_DATA))
{
	uint8_t x2, y2;

	*flatLine = false;

	// Center the x and y coordinate calculated previously
	// Y axis is subtracted between LCD is from 0 - 127 (top = 0, bottom = 127)
	// while Y values calculated is Cartesian. (Y is greater on the top and lower on the bottom)
	y2 = CENTER_CIRCLE_Y - unitCircleY;
	x2 = CENTER_CIRCLE_X + unitCircleX;

	// Calculate slope using point and center
	*slope_rise = y2 - CENTER_CIRCLE_Y;
	*slope_run = x2 - CENTER_CIRCLE_X;

	if((*slope_run) == 0)
	{
		return;
	}

	// Calculate b since x_slope != 0
	*b = CENTER_CIRCLE_Y - ((CENTER_CIRCLE_X * (*slope_rise)) / (*slope_run));

	// Check all cases to see whether the change in x is greater or y is greater
	// if the delta x is greater than (3/2 * delta y), then the line is flat
	if (unitCircleX < 0 && unitCircleY < 0)
	{
		if (unitCircleX < unitCircleY)
		{
			*flatLine = true;
		}
	}
	else if (unitCircleX > 0 && unitCircleY > 0)
	{
		if (unitCircleX > unitCircleY)
		{
			*flatLine = true;
		}
	}
	else if (unitCircleX < 0)
	{
		if (-unitCircleX > unitCircleY)
		{
			*flatLine = true;
		}
	}
	else
	{
		if (unitCircleX > -unitCircleY)
		{
			*flatLine = true;
		}
	}
}

//---------------------------------------------------------------------------
// GetXY
//---------------------------------------------------------------------------
//
// Description - computes the x and y values given an angle and radius. (Polar to Cartesian)
//				  X = r * cos(angle)
// 				  Y = r * sin(angle)
// 				  Since there is only a sin table, cos(angle) is converted
//				  its equivalent sin(angle) to calculate X
//
// unitCircleX - the computed x value
// unitCircleY - the computed y value
// radius - radius of the line
// angle  - angle of the line
//
void GetXY(int16_t* unitCircleX, int16_t* unitCircleY, uint8_t radius, uint16_t angle)
{
    bool negate_x = false;
    bool negate_y = false;

	// Split into 4 quadrants for the purpose of converting cos(angle) to
	// its equivalent sin(angle) to calculate X
	if (angle <= 90)
	{
	}
	else if (angle >= 90 && angle <= 180)
	{
	    angle = 180 - angle;
	    negate_x = true;
	    negate_y = false;
	}
	else if (angle >= 180 && angle<= 270)
	{
        angle -= 180;
        negate_x = true;
        negate_y = true;
	}
	else
	{
        angle = 360 - angle;
        negate_x = false;
        negate_y = true;
	}

	*unitCircleX = ((uint16_t)radius * SIN_TABLE[(uint8_t)(90 - angle)]) / 100;
	*unitCircleY = ((uint16_t)radius * SIN_TABLE[(uint8_t)(angle)]) / 100;

    if (negate_x)
    {
        *unitCircleX = -*unitCircleX;
    }

    if (negate_y)
    {
        *unitCircleY = -*unitCircleY;
    }
}

//---------------------------------------------------------------------------
// DrawMinutesHoursHands
//---------------------------------------------------------------------------
//
// Description - draws the minutes and hours hand of the analog clock
//
//
// minutesAngle - the polar angle of the minutes hand
// hoursAngle - the polar angle of the hours hand
//
void DrawAnalogClockHands()
{
	int16_t minutes_x, hours_x;
	int16_t minutes_y, hours_y;

	uint8_t line_x;
	uint8_t line_y;

	// This for loop only draws the rows that affect up to the seconds dot for efficiency.
	// Avoids drawing the whole screen
	for(line_y = CENTER_CIRCLE_Y - SECONDS_RADIUS - 1; line_y <= CENTER_CIRCLE_Y + SECONDS_RADIUS + 1; line_y++)
	{
		// Copy the current background of the line so it is not erased on the redraw
		CopyLineOfBackground(line_y);

		// This section checks for the occurrence of flat minute hand.
		// The reason is because the previous code only computes one value of x per y value(row).
		// However, if the line is flat, there needs to be multiple x pixels per row to draw the line.
		if (FlatLineMinute == true)
		{
			uint8_t minuteLineStart_x;
			uint8_t minuteLineEnd_x;

			// Hand occupies right half of screen
            if (MinutesDotX >= CENTER_CIRCLE_X)
            {
                minuteLineStart_x = CENTER_CIRCLE_X;
                minuteLineEnd_x = MinutesDotX;
            }
            // Hand occupies left half of screen
            else
            {
                minuteLineStart_x = MinutesDotX;
                minuteLineEnd_x = CENTER_CIRCLE_X;
            }

			// For all x pixels within the radius of the hour or minute hand, compute the corresponding
			// y value. If the y value is equal to the current row being drawn, then render
			// the pixel into the line buffer.
			for (line_x = minuteLineStart_x; line_x <= minuteLineEnd_x; line_x++)
			{
				minutes_y = (((MinuteHand_y_slope * line_x) / MinuteHand_x_slope) + MinuteHand_b);
                if (minutes_y == line_y)
                {
                    ClockLine[line_x / 8] |= 1 << (7 - (line_x % 8));
                }
			}
		}
		// Not a flat line
		else
		{
            uint8_t minutesLineStart_y;
            uint8_t minutesLineEnd_y;

            // Hand occupies upper half of the clock
            if (MinutesDotY < CENTER_CIRCLE_Y)
            {
                minutesLineStart_y = MinutesDotY;
                minutesLineEnd_y = CENTER_CIRCLE_Y;
            }
            // Hand occupies lower half of the clock
            else
            {
                minutesLineStart_y = CENTER_CIRCLE_Y;
                minutesLineEnd_y = MinutesDotY;
            }

            // Current line_y is within bounds of the clock hand y-values
            if (line_y >= minutesLineStart_y &&
                line_y <= minutesLineEnd_y)
            {
                // Handle special case, where slope is infinity (at 90 and 270 degrees)
                // which prevented divide by zero earlier
                if (MinuteHand_x_slope == 0)
                {
                    minutes_x = CENTER_CIRCLE_X;
                }
                else
                {
                    // Calculate the x value of the minute hand based on the minute hand line equation
                    minutes_x = (((line_y - MinuteHand_b) * MinuteHand_x_slope) / MinuteHand_y_slope);
                }

                ClockLine[minutes_x / 8] |= 1 << ( 7 - (minutes_x % 8));
            }
		}

		// This section checks for the occurrence of flat hour hand.
		// The reason is because the previous code only computes one value of x per y value(row).
		// However, if the line is flat, there needs to be multiple x pixels per row to draw the line.
		if(FlatLineHours == true)
		{
			uint8_t hoursLineStart_x;
			uint8_t hoursLineEnd_x;

            // Hand occupies left half of screen
			if (HoursDotX < CENTER_CIRCLE_X)
            {
                hoursLineStart_x = HoursDotX;
                hoursLineEnd_x = CENTER_CIRCLE_X;
            }
			// Hand occupies right half of screen
			else
			{
				hoursLineStart_x = CENTER_CIRCLE_X;
				hoursLineEnd_x = HoursDotX;
			}

			// For all x pixels within the radius of the hour hand, compute the corresponding
			// y value. If the y value is equal to the current row being drawn, then render
			// the pixel into the line buffer.
			for (line_x = hoursLineStart_x; line_x <= hoursLineEnd_x; line_x++)
			{
				hours_y = (((HoursHand_y_slope * line_x) / HoursHand_x_slope) + HoursHand_b);
                if (hours_y == line_y)
                {
                    ClockLine[line_x / 8] |= 1 << (7 - (line_x % 8));
                }
			}
		}
		// Not a flat line
		else
		{
            uint8_t hoursLineStart_y;
            uint8_t hoursLineEnd_y;

	        // Hand occupies upper half of the clock
	        if (HoursDotY < CENTER_CIRCLE_Y)
	        {
	            hoursLineStart_y = HoursDotY;
	            hoursLineEnd_y = CENTER_CIRCLE_Y;
	        }
	        // Hand occupies lower half of the clock
	        else
	        {
	            hoursLineStart_y = CENTER_CIRCLE_Y;
                hoursLineEnd_y = HoursDotY;
	        }

	        // Current line_y is within bounds of the clock hand y-values
	        if (line_y >= hoursLineStart_y &&
                line_y <= hoursLineEnd_y)
	        {
                // Handle special case, where slope is infinity (at 90 and 270 degrees)
                // which prevented divide by zero earlier
                if (HoursHand_x_slope == 0)
                {
                    hours_x = CENTER_CIRCLE_X;
                }
                else
                {
                    // Calculate the x value of the hour hand based on the minute hand line equation
                    hours_x = (((line_y - HoursHand_b) * HoursHand_x_slope) / HoursHand_y_slope);
                }
                ClockLine[hours_x / 8] |= 1 << ( 7 - (hours_x % 8));
	        }
		}

		// Draws the seconds dot pixel into clockLine buffer.
		// Since seconds dot is two by two, have to check secondsDotY + 1 == i as well.
		if (SecondsDotY == line_y || SecondsDotY + 1 == line_y)
		{
			ClockLine[SecondsDotX / 8] |= 1 << ( 7 - (SecondsDotX % 8));
			ClockLine[(SecondsDotX + 1) / 8] |= 1 << ( 7 - ((SecondsDotX + 1) % 8));
			SecondsDotDrawn = true;
		}

		// Display the line buffer onto the LCD
		DISP_WriteLine(line_y, ClockLine);
	}
}

//---------------------------------------------------------------------------
// ClkAngletoPolarAngle
//---------------------------------------------------------------------------
//
// Description - Converts the clock angle, which is 360 degrees at 12 o'clock,
// 				  to a unit circle angle. For example 12 o'clock would be 90 degrees.
//
// angle  - clock angle to covert to polar angle
//
uint16_t ClkAngleToPolarAngle(uint16_t angle)
{
	uint16_t polarAngle = 360 - angle + 90;
	if(polarAngle >= 360)
	{
		polarAngle -= 360;
		return polarAngle;
	}
	else
	{
		return polarAngle;
	}
}

//---------------------------------------------------------------------------
// ConvertToString
//---------------------------------------------------------------------------
//
// Description - Converts a two digit integer into string
//
// integer - integer to convert
// string - the computed string
//
void ConvertToString(uint8_t integer, char* string)
{
	if(integer == BLANK)
	{
		*string = 0x2F;
		string++;
		*string = 0x2F;
	}
	else
	{
		*string = ((integer / 10) + '0');
		string++;
		*string = ((integer % 10) + '0');
	}
}

//---------------------------------------------------------------------------
// CopyLineOfBackground
//---------------------------------------------------------------------------
//
// Description - copies a line from the analog clock background to line buffer.
//				  Used for drawing the hands because since the background is drawn
//				  before the hands, the hands could overwrite the background.
//
// lineNum  - the line of clock background to copy
//
void CopyLineOfBackground(uint8_t lineNum)
{
#if SHOW_ANALOG_CLOCK_FACE
	uint8_t i;

	for( i = 0; i < DISP_BUF_SIZE; i++)
	{
		ClockLine[i] = clock_background_bits[lineNum * clock_background_width / 8 + i];
	}
#else
	UNREFERENCED_ARGUMENT(lineNum);
	RENDER_ClrLine(ClockLine);
#endif
}
