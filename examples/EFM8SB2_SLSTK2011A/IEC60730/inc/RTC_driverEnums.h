/**************************************************************************//**
 * Copyright (c) 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef _RTC_DRIVER_ENUMS_H_
#define _RTC_DRIVER_ENUMS_H_
#include "si_toolchain.h"

// Power Management Bit Definitions
#define SLEEP        0x80              // Sleep Mode Select
#define SUSPEND      0x40              // Suspend Mode Select
#define CLEAR        0x20              // Wake-Up Flag Clear
#define RSTWK        0x10              // Reset Pin Falling Edge Wake-Up
#define RTCFWK       0x08              // SmaRTClock Failure Wake-Up
#define RTCAWK       0x04              // SmaRTClock Alarm Wake-Up
#define PMATWK       0x02              // Port Match Wake-Up
#define CPT0WK       0x01              // Comparator0 Wake-Up

// SmaRTClock Internal Registers
#define CAPTURE0  0x00                 // RTC address of CAPTURE0 register
#define CAPTURE1  0x01                 // RTC address of CAPTURE1 register
#define CAPTURE2  0x02                 // RTC address of CAPTURE2 register
#define CAPTURE3  0x03                 // RTC address of CAPTURE3 register
#define RTC0CN    0x04                 // RTC address of RTC0CN register
#define RTC0XCN   0x05                 // RTC address of RTC0XCN register
#define RTC0XCF   0x06                 // RTC address of RTC0XCF register
#define RTC0PIN   0x07                 // RTC address of RTC0PIN register
#define ALARM0    0x08                 // RTC address of ALARM0 register
#define ALARM1    0x09                 // RTC address of ALARM1 register
#define ALARM2    0x0A                 // RTC address of ALARM2 register
#define ALARM3    0x0B                 // RTC address of ALARM3 register

// SmaRTClock Bit Definitions
#define RTC0CAP   0x01
#define RTC0SET   0x02
#define ALRM      0x04
#define RTC0AEN   0x08
#define RTC0TR    0x10
#define OSCFAIL   0x20
#define MCLKEN    0x40
#define RTC0EN    0x80

extern SI_SEGMENT_VARIABLE(RTC_clkFreq, uint16_t, SI_SEG_XDATA);

#define SELFOSC  1
#define LFO      2
#define CRYSTAL  3

// RTC Clock Source
#define RTC_CLKSRC   SELFOSC

// Crystal Load Capacitance
#define LOADCAP_VALUE  6               // 0 =  4.0 pf,  1 =  4.5 pf
                                       // 2 =  5.0 pf,  3 =  5.5 pf
                                       // 4 =  6.0 pf,  5 =  6.5 pf
                                       // 6 =  7.0 pf,  7 =  7.5 pf
                                       // 8 =  8.0 pf,  9 =  8.5 pf
                                       // 10 =  9.0 pf, 11 =  9.5 pf
                                       // 12 = 10.5 pf, 13 = 11.5 pf
                                       // 14 = 12.5 pf, 15 = 13.5 pf

#if RTC_CLKSRC == SELFOSC
   #define RTCCLK   32768// 31770
#elif RTC_CLKSRC == LFO
   #define RTCCLK   16400// 16000
#else
   #define RTCCLK   32768
#endif


#endif
