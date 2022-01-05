//=============================================================================
// SmaRTClock.c
//=============================================================================
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// C File Description:
//
// This file contains rtc code for the F960.
//
// Target:
//    C8051F96x.
//
// IDE:
//    Silicon Laboratories IDE   version 3.3
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
//    C8051F960 Example Code
//
// Beta Release
//    - TBD
//
// This software must be used in accordance with the End User License Agreement.
//
//=============================================================================
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F960_Register_Enums.h>
#include "SmaRTClockF960.h"
//-----------------------------------------------------------------------------
// RTC_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will initialize the RTC.
//
//-----------------------------------------------------------------------------
void RTC_Init (void)
{
   SFRPAGE = LEGACY_PAGE;

   // Unlocking the interface is not necessary for the 'F960.

   // configure using low-power LFO (lowest power option)

   RTC_Write (RTC0XCN, 0x08);          // LFO mode

   RTC_Write (RTC0XCF, 0x06);          // load capacitance to 12 pF

   RTC_Write (RTC0CN, 0x80);           // Enable smaRTClock

}
//-----------------------------------------------------------------------------
// RTC_Write ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : reg, value
//
// This function will Write one byte from the specified RTC register.
// Using a register number greater that 0x0F is not permited,
//
// This function uses the new F960 fast write mode.
//
//-----------------------------------------------------------------------------
void RTC_Write (unsigned char reg, unsigned char value)
{
   reg &= 0x0F;                        // mask low nibble
   SFRPAGE = LEGACY_PAGE;
   RTC0ADR  = reg;                     // pick register
   RTC0DAT = value;                    // write data
}
//-----------------------------------------------------------------------------
// RTC_Read ()
//-----------------------------------------------------------------------------
//
// Return Value : RTC0DAT
// Parameters   : reg
//
// This function will read one byte from the specified RTC register.
// Using a register number greater that 0x0F is not permited,
//
//
// This function uses the new F960 fast read mode.
//
//-----------------------------------------------------------------------------
unsigned char RTC_Read (uint8_t reg)
{
   reg &= 0x0F;                        // mask low nibble
   SFRPAGE = LEGACY_PAGE;
   RTC0ADR  = (reg |0x80);             // write address setting READ bit
   return RTC0DAT;                     // return value
}
//=============================================================================
// Timer Set and Capture Functions using new F960 fast mode.
//-----------------------------------------------------------------------------
// RTC_SetTimer  ()
//-----------------------------------------------------------------------------
//
// Parameters   : uint32_t Timer set time
// Return Value : uint8_t  returns 0xFF for error.
//
// This function will write to the Capture registers and set the timer.
//
//-----------------------------------------------------------------------------
uint8_t RTC_SetTimer(uint32_t time)
{
   SI_UU32_t value;
   uint8_t control;

   value.u32 = time;

   SFRPAGE = LEGACY_PAGE;
   control = RTC_Read(RTC0CN);

   if((control&0x80)==0)
      return 0xFF;                     // error RTC must be enabled

   // write using auto-increment
   RTC0ADR = 0x00;
   RTC0DAT = value.u8[B0];
   RTC0DAT = value.u8[B1];
   RTC0DAT = value.u8[B2];
   RTC0DAT = value.u8[B3];

   RTC_Write(RTC0CN, (control|0x06));  // set using F960 fast mode

   // wait for set bit to go to zero
   while(((RTC_Read(RTC0CN))&0x02)==0x02);

   return 0;
}
//-----------------------------------------------------------------------------
// RTC_CaptureTimer  ()
//-----------------------------------------------------------------------------
//
// Return Value : uint32_t capture time
// Parameters   : none
//
// This function will read the 32-bit capture value.
//
//-----------------------------------------------------------------------------
uint32_t RTC_CaptureTimer (void)
{
   SI_UU32_t timer;
   uint8_t control;

   SFRPAGE = LEGACY_PAGE;

   control = RTC_Read(RTC0CN);

   if((control&0x80)==0)
      return 0xFFFF;                   // error RTC must be enabled

   RTC_Write(RTC0CN, (control|0x05));  // capture using F960 fast mode

   // wait for capture bit to go to zero
   while(((RTC_Read(RTC0CN))&0x01)==0x01);

   // read using auto-increment
   RTC0ADR = (0x80 | CAPTURE0);        // read CAPTURE0
   timer.u8[B0]= RTC0DAT;
   timer.u8[B1]= RTC0DAT;
   timer.u8[B2]= RTC0DAT;
   timer.u8[B3]= RTC0DAT;

   return timer.u32;
}
//-----------------------------------------------------------------------------
// RTC_WriteAlarm  ()
//-----------------------------------------------------------------------------
//
// Parameters   : uint32_t Alarm time
// Parameters   : uint8_t alarm  0, 1, or 2
//
// This function will write to the alarm, but does not enable it.
//
//-----------------------------------------------------------------------------
void RTC_WriteAlarm(uint8_t alarm, uint32_t time)
{
   SI_UU32_t value;

   value.u32 = time;

   SFRPAGE = LEGACY_PAGE;

   // wrte using auto-increment
   RTC0ADR = ALARM0B0 + (alarm<<2);
   RTC0DAT = value.u8[B0];
   RTC0DAT = value.u8[B1];
   RTC0DAT = value.u8[B2];
   RTC0DAT = value.u8[B3];
}
//-----------------------------------------------------------------------------
// RTC_ReadAlarm  ()
//-----------------------------------------------------------------------------
//
// Return Value : uint32_t Alarm time
// Parameters   : uint8_t alarm  0, 1, or 2
//
// This function is provided if you want to read the current alarm value and
// modify the results.
//
//-----------------------------------------------------------------------------
uint32_t RTC_ReadAlarm (uint8_t alarm)
{
   SI_UU32_t time;
   uint8_t addr;

   addr = ALARM0B0 + (alarm<<2);

   SFRPAGE = LEGACY_PAGE;

   // read using auto-increment
   RTC0ADR = (0x80 | addr);          // read byte 0 of ALARMn
   time.u8[B0]= RTC0DAT;
   time.u8[B1]= RTC0DAT;
   time.u8[B2]= RTC0DAT;
   time.u8[B3]= RTC0DAT;

   return time.u32;
}


//=============================================================================
