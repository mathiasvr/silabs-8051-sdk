/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
#ifndef UART_H_
#define UART_H_

#include "lcd.h"

extern SI_SEGMENT_VARIABLE(buttonStatus[OUT_ROWS][OUT_COLS], uint8_t, SI_SEG_IDATA);

/******************************************************************************
 * update UART output contents
 *****************************************************************************/
void updateUART(void);

#endif /* UART_H_ */
