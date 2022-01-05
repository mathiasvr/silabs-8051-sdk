/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
#ifndef LCD_H_
#define LCD_H_

#include "project_config.h"

// LCD display dimensions
#define OUT_COLS                      4
#define OUT_ROWS                      3
#define OUT_HEIGHT                    out_off_height
#define OUT_WIDTH                     out_off_width
#define TOTAL_OUT_HEIGHT              (OUT_HEIGHT * OUT_ROWS)
#define TOTAL_OUT_WIDTH               (OUT_WIDTH * OUT_COLS)

extern uint16_t capsenseCurrent, sliderCurrent;
extern uint16_t capsensePrevious, sliderPrevious;

/******************************************************************************
 * initialize and clear the 128x128 lcd display
 *****************************************************************************/
void initLCD(void);

/******************************************************************************
 * update lcd display frame
 *****************************************************************************/
void updateLCD(void);

/******************************************************************************
 * print slider movements
 *****************************************************************************/
void PrintBall (void);

/******************************************************************************
 * print information on the slider
 *****************************************************************************/
void SliderName (void);

#endif /* LCD_H_ */
