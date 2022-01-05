#ifndef  SMARTCLOCKF960_H
#define  SMARTCLOCKF960_H
//================================================================================================
// SmaRTClockF960.h
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
// Indirect RTC Register Addresses (ifndef to not overload hader file)
//-----------------------------------------------------------------------------
#ifndef CAPTURE0
#define CAPTURE0  0x00                 // RTC address of CAPTURE0 register
#define CAPTURE1  0x01                 // RTC address of CAPTURE1 register
#define CAPTURE2  0x02                 // RTC address of CAPTURE2 register
#define CAPTURE3  0x03                 // RTC address of CAPTURE3 register
#define RTC0CN    0x04                 // RTC address of RTC0CN register
#define RTC0XCN   0x05                 // RTC address of RTC0XCN register
#define RTC0XCF   0x06                 // RTC address of RTC0XCF register
#define RTC0CF    0x07                 // RTC address of RTC0PIN register
#define ALARM0B0  0x08                 // RTC address of ALARM0 Byte 0
#define ALARM0B1  0x09                 // RTC address of ALARM0 Byte 1
#define ALARM0B2  0x0A                 // RTC address of ALARM0 Byte 2
#define ALARM0B3  0x0B                 // RTC address of ALARM0 Byte 3
#define ALARM1B0  0x0C                 // RTC address of ALARM1 Byte 0
#define ALARM1B1  0x0D                 // RTC address of ALARM1 Byte 1
#define ALARM1B2  0x0E                 // RTC address of ALARM1 Byte 2
#define ALARM1B3  0x0F                 // RTC address of ALARM1 Byte 3
#define ALARM2B0  0x10                 // RTC address of ALARM2 Byte 0
#define ALARM2B1  0x11                 // RTC address of ALARM2 Byte 1
#define ALARM2B2  0x12                 // RTC address of ALARM2 Byte 2
#define ALARM2B3  0x13                 // RTC address of ALARM2 Byte 3
#endif
//-----------------------------------------------------------------------------
// SmaRTClock Bit Definitions
//-----------------------------------------------------------------------------
#define RTC0CAP   0x01
#define RTC0SET   0x02
#define RTC0FAST  0x04
#define RTC0TR    0x10
#define OSCFAIL   0x20
#define MCLKEN    0x40
#define RTC0EN    0x80
//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define TWELVE_PF       0x0C
#define CAP_AUTO_STEP   0x80
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void  RTC_Init (void);
uint8_t    RTC_Read (uint8_t reg);
void  RTC_Write (uint8_t reg, uint8_t value);
uint8_t    RTC_SetTimer(uint32_t time);
uint32_t   RTC_CaptureTimer (void);
void  RTC_WriteAlarm(uint8_t alarm, uint32_t time);
uint32_t   RTC_ReadAlarm (uint8_t alarm);
//=============================================================================
#endif  // RTC_H
