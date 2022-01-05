/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
#ifndef LCD_H_
#define LCD_H_

#include "project_config.h"

// lcd display dimensions
#define OUT_COLS                      3
#if CPT_DEMO_MODE
#define OUT_ROWS                      5
#else
#define OUT_ROWS                      4
#endif
#define OUT_HEIGHT                    out_off_height
#define OUT_WIDTH                     out_off_width
#define TOTAL_OUT_HEIGHT              (OUT_HEIGHT * OUT_ROWS)
#define TOTAL_OUT_WIDTH               (OUT_WIDTH * OUT_COLS)

extern uint16_t xdata capsenseCurrent;
extern uint16_t xdata capsensePrevious;

/******************************************************************************
 * initialize and clear the 128x128 lcd display
 *****************************************************************************/
void initLCD(void);

/******************************************************************************
 * update lcd display frame
 *****************************************************************************/
void updateLCD(void);

/******************************************************************************
 * Print heart/skull depending on state
 *****************************************************************************/
extern void printNCM(uint8_t factorNCM);
// State factors
#define HEART_EMPTY 		0 // Device State - good
#define HEART_FULL			1 // Device State - bad
#define HEART_BROKEN		2 // Device State - No confidence mode
#define SAFE_STATE			3 // Device state - safe state

#endif /* LCD_H_ */
