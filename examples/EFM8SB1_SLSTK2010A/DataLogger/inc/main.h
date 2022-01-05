//-----------------------------------------------------------------------------
// main.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//

#ifndef MAIN_H_
#define MAIN_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "bsp.h"
#include "demo_config.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define DISPLAY_ITEM_CELSIUS        0
#define DISPLAY_ITEM_FAHRENHEIT     1
#define DISPLAY_ITEM_HUMIDITY       2

#define DISPLAY_MODE_GRAPH          0
#define DISPLAY_MODE_LOGGER         1

#define REAL_TIME_MODE              0
#define HISTORY_MODE                1

#define LCD_PAGE_SIZE               16

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
// Current SW1_State, whether it is displaying F, C, or H
extern uint8_t displayItem;
// SW2 state determines between GRAPH and LOGGER mode
extern bool displayMode;

extern bool alarmWakeUp;

extern uint8_t idata hours;
extern uint8_t idata minutes;
extern uint8_t idata seconds;
//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------

#endif /* MAIN_H_ */
