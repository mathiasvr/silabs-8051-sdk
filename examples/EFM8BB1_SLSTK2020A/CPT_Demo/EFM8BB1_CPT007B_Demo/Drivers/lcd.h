/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
#ifndef LCD_H_
#define LCD_H_

// lcd display dimensions
#define OUT_COLS                      2
#define OUT_ROWS                      4
#define OUT_HEIGHT                    out_off_height
#define OUT_WIDTH                     out_off_width
#define TOTAL_OUT_HEIGHT              (OUT_HEIGHT * OUT_ROWS)
#define TOTAL_OUT_WIDTH               (OUT_WIDTH * OUT_COLS)

extern uint16_t capsenseCurrent;
extern uint16_t capsensePrevious;

/******************************************************************************
 * initialize and clear the 128x128 lcd display
 *****************************************************************************/
void initLCD(void);

/******************************************************************************
 * update lcd display frame
 *****************************************************************************/
void updateLCD(void);

#endif /* LCD_H_ */
