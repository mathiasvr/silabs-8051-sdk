//-----------------------------------------------------------------------------
// F96x_Clock_Sleep_LCD0.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header for main file:

#ifndef F96X_CLOCK_SLEEP_LCD0_H_
#define F96X_CLOCK_SLEEP_LCD0_H_

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

// RTC frequency
#define RTC_XTAL_FREQ       32786
#define RTC_LFO_FREQ        16000

#define PORT_MATCH_WAKEUP   0x02
#define SUSPEND_MODE        0x40
#define SLEEP_MODE          0x80

//LCD Contrast Modes
enum LCD_CONTRAST_MODE{
   LCD_BYPASS_MODE,
   LCD_MIN_CONTRAST_MODE,
   LCD_CONSTANT_CONTRAST_MODE,
   LCD_AUTO_BYPASS_MODE,
};

//LCD Mux Modes
enum LCD_MUX_MODE{
   STATIC_MUX_MODE,
   TWO_MUX_MODE,
   THREE_MUX_MODE,
   FOUR_MUX_MODE,
};

#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

#define DISABLED 0
#define ENABLED 1

#endif // F96X_CLOCK_SLEEP_LCD0_H_
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
