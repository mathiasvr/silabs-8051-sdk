//-----------------------------------------------------------------------------
// F96x_Clock_Sleep_LCD0_Config.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// This header file sets the desired options to be used for measuring current.

#ifndef F96_CLOCK_SLEEP_LCD0_CONFIG_H_
#define F96_CLOCK_SLEEP_LCD0_CONFIG_H_

#include "F96x_Clock_Sleep_LCD0.h"
//-----------------------------------------------------------------------------
// Start of F96x_Clock_Sleep_LCD0 Configuration Options
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Contrast Mode Selection
//-----------------------------------------------------------------------------
//
// Select a Contrast Mode:
//
//    LCD_BYPASS_MODE            - LCD Voltage follows battery voltage (Vbat)
//    LCD_MIN_CONTRAST_MODE      - LCD voltage follows Vbat unless Vbat falls
//                                  below a set threshold (VLCD_mV below). LCD
//                                  voltage is then regulated to VLCD_mV.
//    LCD_CONSTANT_CONTRAST_MODE - LCD voltage is regulated to stay at VLCD_mV.
//    LCD_AUTO_BYPASS_MODE       - LCD voltage is regulated at VLCD_mV until
//                                  Vbat falls below VLCD_mV. LCD voltage then
//                                  follows Vbat.
//
#define CONTRAST_MODE               LCD_CONSTANT_CONTRAST_MODE

// Set the constant below to the desired LCD drive voltage, in millivolts
#define VLCD_mV                     2700

// Select a Mux mode that corresponds to your LCD:
// The F960 MCU MUX LCD board has a four-mux LCD.
//
//   STATIC_MUX_MODE
//   TWO_MUX_MODE
//   THREE_MUX_MODE
//   FOUR_MUX_MODE
//
#define MUX_MODE                    FOUR_MUX_MODE

// Set the constant below to the desired refresh rate, in Hertz.
#define REFRESH_RATE_HZ             60

// Set the constant below to equal the number of segments that the connected
// LCD has. The F960 MCU MUX LCD board has 32 segments.
#define LCD_SEGMENTS                32

// Set the constant below to 'DISABLED' to disable the VBAT supply monitor on
// 'F96x devices, 'ENABLED' to enable.
#define VBAT_MONITOR                DISABLED

// Set the constant below to use the low frequency oscillator (instead of
// an external 32.768 kHz crystal). On devices that do not have an LFO, the
// SmaRTClock oscillator will be configured into self oscillate mode
#define RTC_LFO                     DISABLED

//-----------------------------------------------------------------------------
// End of F96x_Clock_Sleep_LCD0 Configuration Options
//-----------------------------------------------------------------------------

#endif // F96_CLOCK_SLEEP_LCD0_CONFIG_H_
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
