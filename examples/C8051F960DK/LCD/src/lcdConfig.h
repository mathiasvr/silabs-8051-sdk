#ifndef  LCD_CONFIG_H
#define  LCD_CONFIG_H
//================================================================================================
// lcdConfig.h
//================================================================================================
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header File Description:
//
// This file is SmaRTClock header file for the C8051F96x.
//
// Target:
//    Any Silicon Labs C8051 MCU.
//
// IDE:
//    Silicon Laboratories IDE   version 4.18
//
// Tool Chains:
//    Keil
//       c51.exe     version 8.0.8
//       bl51.exe    version 6.0.5
//    SDCC
//       sdcc.exe    version 2.8.0
//       aslink.exe  version 1.75
//
// Project Name:
//    F96x  Example code
//
//
// This software must be used in accordance with the End Users License Agreement.
//
//=============================================================================
// Includes
//-----------------------------------------------------------------------------
#ifndef COMPILER_DEFS_H
#endif
//-----------------------------------------------------------------------------
// LCD Contrast modes
//-----------------------------------------------------------------------------
#define LCD_BYPASS_MODE             1
#define LCD_MIN_CONTRAST_MODE       2
#define LCD_CONSTANT_CONTRAST_MODE  3
#define LCD_AUTO_BYPASS_MODE        4
//-----------------------------------------------------------------------------
// RTC frequency
//-----------------------------------------------------------------------------
#define RTC_XTAL_FREQ               32786
#define RTC_LFO_FREQ                16000
//-----------------------------------------------------------------------------
// Define RTC mode here
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
#define RTC_LFO_MODE
//#define RTC_XTAL_MODE
//-----------------------------------------------------------------------------
// Public function prototypes (API)
//-----------------------------------------------------------------------------
void LCD0_Config(uint8_t muxMode, uint8_t segments, uint8_t contrastMode,
		 uint16_t contrast, uint8_t refresh, uint8_t toggle);
void LCD0_ConfigClear(void);
void LCD0_ConfigMuxMode(uint8_t muxMode);
void LCD0_ConfigSegments(uint8_t segments);
void LCD0_ConfigContrastMode(uint8_t contrastMode);
void LCD0_SetContrast(uint16_t contrast);
void LCD0_ConfigRefresh(uint8_t refresh, uint8_t muxMode);
void LCD0_ConfigToggle(uint8_t toggle, uint8_t refresh,  uint8_t muxMode);
void LCD0_ConfigEnable(void);
//=============================================================================
// end LCD_CONFIG_H
//=============================================================================
#endif //LCD_CONFIG_H