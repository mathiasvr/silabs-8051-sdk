//-----------------------------------------------------------------------------
// CapTouchSense_WakeOnRTC_RTC.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This file contains the smaRTClock functions related to the 
// Low-Power Capacitive TouchSense Switches with Wake-on-RTC.
//
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Silicon Labs Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 15 JAN 2014
//
// Release 1.0
//    -Initial Revision (PKC)
//    -20 MAY 2008
//    -Based on CapTouchSenseDC_Memory_Game.c (FB)
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F930_Register_Enums.h>
#include "CapTouchSense_WakeOnRTC.h"

//-----------------------------------------------------------------------------
// RTC_Read ()
//-----------------------------------------------------------------------------
//
// Return Value : RTC0DAT
// Parameters   : 1) uint8_t reg - address of RTC register to read
//
// This function will read one byte from the specified RTC register.
// Using a register number greater that 0x0F is not permited.
//
//-----------------------------------------------------------------------------

uint8_t RTC_Read (uint8_t reg)
{
   reg &= 0x0F;                        // mask low nibble
   RTC0ADR  = reg;                     // pick register
   RTC0ADR |= 0x80;                    // set BUSY bit to read
   while ((RTC0ADR & 0x80) == 0x80);   // poll on the BUSY bit
   return RTC0DAT;                     // return value
}

//-----------------------------------------------------------------------------
// RTC_Write ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) uint8_t reg - address of RTC register to write
//                2) uint8_t value - the value to write to <reg>
//
// This function will Write one byte to the specified RTC register.
// Using a register number greater that 0x0F is not permited.
//-----------------------------------------------------------------------------
void RTC_Write (uint8_t reg, uint8_t value)
{
   reg &= 0x0F;                        // mask low nibble
   RTC0ADR  = reg ;                    // pick register
   RTC0DAT = value;                    // write data
   while ((RTC0ADR & 0x80) == 0x80);   // poll on the BUSY bit
}

//-----------------------------------------------------------------------------
// RTC_SetAlarmInterval ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//    uint32_t interval - the new smaRTClock alarm interval
//
// This function will set the smaRTClock alarm interval and set an
// auto-repeating alarm.
//
//-----------------------------------------------------------------------------
void RTC_SetAlarmInterval (uint32_t interval)
{

   SI_UU32_t alarm_interval;

   alarm_interval.u32 = interval;

   //----------------------------------
   // Set the smaRTClock Alarm
   //----------------------------------

   // Stop the RTC
   RTC_Write (RTC0CN, 0xC0);     // Disable Timer

   // Clear the main timer
   RTC_Write (CAPTURE0, 0x00);   // Least significant byte
   RTC_Write (CAPTURE1, 0x00);
   RTC_Write (CAPTURE2, 0x00);
   RTC_Write (CAPTURE3, 0x00);   // Most significant byte

   RTC_Write(RTC0CN, 0xC2);            // Write '1' to RTC0SET
   while((RTC_Read(RTC0CN) & 0x02));   // Wait for RTC0SET -> 0

   // Copy the alarm interval to the alarm registers
   RTC_Write (ALARM0, alarm_interval.u8[B0]);   // Least significant byte
   RTC_Write (ALARM1, alarm_interval.u8[B1]);
   RTC_Write (ALARM2, alarm_interval.u8[B2]);
   RTC_Write (ALARM3, alarm_interval.u8[B3]);   // Most significant byte

   // Enable the smaRTClock timer and alarm with auto-reset
   RTC_Write (RTC0CN, 0xDC);
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
