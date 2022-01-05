//-----------------------------------------------------------------------------
// F91x_CapTouchSense_Main.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This file contains the constant declarations and function prototypes for 
// the Low-Power Capacitive TouchSense with Wake-on-RTC.
//
//
// Target:         C8051F90x-C8051F91x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Compiled and tested for Silicon Labs Studio
//    - 13 JAN 2014
//
// Release 1.2 
//    - Compiled and tested for Raisonance Toolchain (FB)
//    - 17 MAY 10
//
// Release 1.1 
//    - Port to C8051F912-TB from C8051F930-TB (JH)
//    - 01 JULY 2009
//
// Release 1.0
//    - Initial Revision (PKC)
//    - 20 MAY 2008
//

#ifndef _CAPTOUCHSENSE_MAIN_H_
#define _CAPTOUCHSENSE_MAIN_H_

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK          5000000L       // SYSCLK frequency in Hz
#define RTCCLK          32768L         // smaRTClock frequency in Hz
#define SECOND_COUNT    RTCCLK
#define SWITCH_STANDARD_THRESHOLD 30   // Standard threshold for CapTouchSense

#define COMPARATOR_POWER_UP_TIME 10L   // Comparator power-up time (in us)
#define COMPARATOR_POWER_UP_TIME_CNT (COMPARATOR_POWER_UP_TIME*SYSCLK/1000000)

// PCB Choices
#define CAPTOUCHSENSEDC 1
#define C8051F930TB     2
#define C8051F912TB     3

// ***Customize these parameters ****************************************
#define PCB C8051F912TB                // Available Choices:
                                       // CAPTOUCHSENSEDC
                                       // C8051F930TB
                                       // C8051F912TB

#define SWITCH_SENSITIVITY 10          // 0  = Least sensitive
                                       // 20 = Most sensitive

#define WAKEUP_INTERVAL SECOND_COUNT/4       // Wakeup interval 1/4 sec
#if ((PCB == CAPTOUCHSENSEDC) || (PCB == C8051F930TB))
   #define CAL_CONSTANTS_PAGE_ADDR  0x7800   // Flash page start address
#elif PCB == C8051F912TB
   #define CAL_CONSTANTS_PAGE_ADDR  0x3800   // Flash page start address
#endif
// **********************************************************************

// Define the number of CapTouchSense switches
#if PCB == CAPTOUCHSENSEDC
   #define NUM_SWITCHES    4
#elif PCB == C8051F930TB
   #define NUM_SWITCHES    2
#elif PCB == C8051F912TB
   #define NUM_SWITCHES    2
#endif

// Switch states
#define SWITCH_PRESSED  0
#define SWITCH_IDLE     1

// System_State variable values
#define INACTIVE  0x01
#define ACTIVE    0x02

// PCON masks
#define IDLE 0x01
#define STOP 0x02

//-------------------------------------
// Calculated Constants
//-------------------------------------

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

#if PCB == CAPTOUCHSENSEDC
   SI_SBIT(BLUE_LED,     SFR_P1, 0);        // '0' means ON, '1' means OFF
   SI_SBIT(GREEN_LED,    SFR_P1, 1);        // '0' means ON, '1' means OFF
   SI_SBIT(RED_LED,      SFR_P1, 5);        // '0' means ON, '1' means OFF
   SI_SBIT(YELLOW_LED,   SFR_P1, 6);        // '0' means ON, '1' means OFF
   SI_SBIT(SW2,          SFR_P0, 2);        // SW2 == 0 means switch pressed
#elif PCB == C8051F930TB
   SI_SBIT(RED_LED,      SFR_P1, 5);        // '0' means ON, '1' means OFF
   SI_SBIT(YELLOW_LED,   SFR_P1, 6);        // '0' means ON, '1' means OFF
   SI_SBIT(SW2,          SFR_P0, 2);        // SW2 == 0 means switch pressed
   SI_SBIT(SW3,          SFR_P0, 3);        // SW2 == 0 means switch pressed
#elif PCB == C8051F912TB
   SI_SBIT(RED_LED,      SFR_P1, 5);        // '0' means ON, '1' means OFF
   SI_SBIT(YELLOW_LED,   SFR_P1, 6);        // '0' means ON, '1' means OFF
   SI_SBIT(SW2,          SFR_P0, 2);        // SW2 == 0 means switch pressed
   SI_SBIT(SW3,          SFR_P0, 3);        // SW2 == 0 means switch pressed
#endif

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

extern uint8_t CS_Switch_State[NUM_SWITCHES];
//SI_LOCATED_VARIABLE(CS_Switch_Cal_Threshold[NUM_SWITCHES], UU16, code, 0x7800, 0);
#ifdef SDCC
   extern	SI_LOCATED_VARIABLE_NO_INIT(CS_Switch_Cal_Threshold[NUM_SWITCHES],
					  SI_UU16_t, SI_SEG_CODE,
					  CAL_CONSTANTS_PAGE_ADDR);
#else
   extern	SI_UU16_t code CS_Switch_Cal_Threshold[NUM_SWITCHES];
#endif

extern uint8_t CS_Switch_Mux_Setting[NUM_SWITCHES];

extern uint8_t LED_P1_Mask[NUM_SWITCHES];

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

// Initialization Routines
void SYSCLK_Init (void);
void PORT_IO_Init (void);
void RTC_Init (void);
void TouchSense_Init (void);

// Interrupt Service Routines
SI_INTERRUPT_PROTO(TIMER2_ISR, TIMER2_IRQn);

// Application Level Support Routines
void Calibrate (void);
void TouchSense_Update (void);
void Delay_Microseconds (uint16_t count);

// Low Level Support Routines
uint8_t RTC_Read (uint8_t reg);
void RTC_Write (uint8_t reg, uint8_t value);
void RTC_SetAlarmInterval (uint32_t interval);

// FLASH read/write/erase routines
void FLASH_ByteWrite (uint16_t addr, uint8_t byte, uint8_t SFLE);
void FLASH_PageErase (uint16_t addr, uint8_t SFLE);

#endif //_CAPTOUCHSENSE_MAIN_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
