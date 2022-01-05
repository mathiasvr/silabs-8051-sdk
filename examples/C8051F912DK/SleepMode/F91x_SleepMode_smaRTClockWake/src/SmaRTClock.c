//-----------------------------------------------------------------------------
// SmaRTClock.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Driver for the SmaRTClock peripheral.
//
// Target:         C8051F90x-C8051F91x
// Tool chain:     Silicon Labs Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 
//    - Compiled and tested for Raisonance Toolchain (FB)
//    - 17 MAY 10
//
// Release 1.0
//    -23 JUL 2009
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F912_Register_Enums.h>
#include "C8051F912_lib.h"

#if SMARTCLOCK_ENABLED

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Variables used for the RTC interface
uint8_t RTC0CN_Local;                       // Holds the desired RTC0CN settings
 

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void RTC_Init (void);
uint8_t   RTC_Read (uint8_t reg);
void RTC_Write (uint8_t reg, uint8_t value);
void RTC_WriteAlarm (uint32_t alarm);
uint32_t  RTC_GetCurrentTime(void);
void RTC_SetCurrentTime(uint32_t time);
void RTC0CN_SetBits(uint8_t bits);
void RTC0CN_ClearBits(uint8_t bits);
void RTC_SleepTicks(uint32_t ticks);

//-----------------------------------------------------------------------------
// RTC_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will initialize the smaRTClock.
//
//-----------------------------------------------------------------------------
void RTC_Init (void)
{
   uint16_t i;

   uint8_t CLKSEL_SAVE = CLKSEL;
   uint8_t RTC0PIN_READ;

   // If the interface is locked
   if(RTC0KEY == 0x00)
   {
      RTC0KEY = 0xA5;                  // Unlock the smaRTClock interface
      RTC0KEY = 0xF1;
   }

   //----------------------------------------
   // Configure the SmaRTClock to crystal
   // or self-oscillate mode
   //----------------------------------------  

   #if(RTC_CLKSRC == CRYSTAL)
         
      RTC_Write (RTC0XCN, 0x60);       // Configure the smaRTClock
                                       // oscillator for crystal mode
                                       // Bias Doubling Enabled
                                       // AGC Disabled


      RTC_Write (RTC0XCF, 0x83);       // Enable Automatic Load Capacitance
                                       // stepping and set the desired
                                       // load capacitance to 4.5pF plus
                                       // the stray PCB capacitance

   #elif(RTC_CLKSRC == SELFOSC)


      // Short XTAL3 and XTAL4 together
      RTC0PIN_READ = RTC_Read (RTC0PIN);
      RTC_Write (RTC0PIN, RTC0PIN_READ | 0x80);   

      // Configure smaRTClock to self-oscillate mode with bias X2 disabled
      // If device has an LFO, then it will be used instead of self-oscillate mode
      RTC_Write (RTC0XCN, 0x08);   
                                      
      // Disable Auto Load Cap Stepping
      RTC_Write (RTC0XCF, (0x00 | LOADCAP_VALUE));   
   
   #else
      #error "Must select crystal or self oscillate mode"                                   
   #endif



   RTC_Write (RTC0CN, 0x80);           // Enable smaRTClock oscillator

   CLKSEL    =  0x74;                  // Switch to 156 kHz low power
                                       // internal oscillator

   //----------------------------------------
   // Wait for crystal to start
   // No need to wait in self-oscillate mode
   //---------------------------------------- 
   
   #if(RTC_CLKSRC == CRYSTAL)

      // Wait > 20 ms
      for (i=0x150; i!=0; i--);

      // Wait for smaRTClock valid
      while ((RTC_Read (RTC0XCN) & 0x10)== 0x00);


      // Wait for Load Capacitance Ready
      while ((RTC_Read (RTC0XCF) & 0x40)== 0x00);

      RTC_Write (RTC0XCN, 0xC0);          // Enable Automatic Gain Control
   
   #endif


   RTC_Write (RTC0CN, 0xC0);           // Enable missing smaRTClock detector
                                       // and leave smaRTClock oscillator
                                       // enabled.
   // Wait > 2 ms
   for (i=0x340; i!=0; i--);

   RTC_Write (RTC0CN, 0xC0);           // Clear the clock fail flag (precautionary).
   
   PMU0CF = 0x20;                      // Clear PMU wake-up source flags

   CLKSEL = CLKSEL_SAVE;               // Restore system clock
   while(!(CLKSEL & 0x80));            // Poll CLKRDY

   RTC0CN_Local = 0xC0;                // Initialize Local Copy of RTC0CN


}

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
//  Timing of SmaRTClock register read with short bit set
//
//     mov      RTC0ADR, #094h
//     nop
//     nop
//     nop
//     mov      A, RTC0DAT
//
//-----------------------------------------------------------------------------
uint8_t RTC_Read (uint8_t reg)
{

   RTC0ADR  = (0x90 | reg);            // pick register and set BUSY to 
                                       // initiate the read
                              
   NOP(); NOP(); NOP();                // delay 3 system clocks
   
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
//
// Timing of SmaRTClock register write with short bit set
//
//       mov      RTC0ADR, #014h
//       mov      RTC0DAT, #088h
//       nop
//
//-----------------------------------------------------------------------------
void RTC_Write (uint8_t reg, uint8_t value)
{
   RTC0ADR  = (0x10 | reg);            // pick register
   RTC0DAT = value;                    // write data
}


//-----------------------------------------------------------------------------
// RTC_WriteAlarm ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) uint32_t alarm - the value to write to <ALARM>
//
// This function writes a value to the <ALRM> registers
//
// Instruction timing of multi-byte write with short bit set
//
//       mov      RTC0ADR, #010h
//       mov      RTC0DAT, #05h
//       nop
//       mov      RTC0DAT, #06h
//       nop
//       mov      RTC0DAT, #07h
//       nop
//       mov      RTC0DAT, #08h
//       nop
//
//-----------------------------------------------------------------------------

void RTC_WriteAlarm (uint32_t alarm)
{   
   SI_UU32_t alarm_value;

   alarm_value.u32 = alarm;   

   // Temporarily disable alarm while updating registers
   RTC0ADR = (0x10 | RTC0CN);
   RTC0DAT = (RTC0CN_Local & ~RTC0AEN);       
   NOP();
   
   // Write the value to the alarm registers
   RTC0ADR = (0x10 | ALARM0);       
   RTC0DAT = alarm_value.u8[B0];    // write data
   NOP();
   RTC0DAT = alarm_value.u8[B1];    // write data
   NOP();
   RTC0DAT = alarm_value.u8[B2];    // write data
   NOP(); 
   RTC0DAT = alarm_value.u8[B3];    // write data
   NOP(); 
   
   // Restore alarm state after registers have been written
   RTC0ADR = (0x10 | RTC0CN);
   RTC0DAT = RTC0CN_Local;  
}

//-----------------------------------------------------------------------------
// RTC_GetCurrentTime()
//-----------------------------------------------------------------------------
//
// Return Value : uint32_t value - the value of the SmaRTClock
// Parameters   : none
//
// This function reads the current value of the SmaRTClock
//
// Instruction timing of multi-byte read with short bit set
//
//       mov      RTC0ADR, #0d0h
//       nop
//       nop
//       nop
//       mov      A, RTC0DAT
//       nop
//       nop
//       mov      A, RTC0DAT
//       nop
//       nop
//       mov      A, RTC0DAT
//       nop
//       nop
//       mov      A, RTC0DAT
//
//-----------------------------------------------------------------------------
uint32_t RTC_GetCurrentTime(void)
{
   SI_UU32_t current_time;   

   RTC_Write( RTC0CN, RTC0CN_Local | RTC0CAP);   // Write '1' to RTC0CAP   
   while((RTC_Read(RTC0CN) & RTC0CAP));          // Wait for RTC0CAP -> 0
   
   RTC0ADR = (0xD0 | CAPTURE0);
   NOP(); NOP(); NOP();
   current_time.u8[B0] = RTC0DAT;               // Least significant byte
   NOP(); NOP();
   current_time.u8[B1] = RTC0DAT;
   NOP(); NOP();
   current_time.u8[B2] = RTC0DAT;
   NOP(); NOP();
   current_time.u8[B3] = RTC0DAT;               // Most significant byte
   
   return current_time.u32;

}

//-----------------------------------------------------------------------------
// RTC_SetCurrentTime()
//-----------------------------------------------------------------------------
//
// Return Value : none 
// Parameters   : 
//
// This function sets the current value of the SmaRTClock
//
// Instruction timing of multi-byte write with short bit set
//
//       mov      RTC0ADR, #010h
//       mov      RTC0DAT, #05h
//       nop
//       mov      RTC0DAT, #06h
//       nop
//       mov      RTC0DAT, #07h
//       nop
//       mov      RTC0DAT, #08h
//       nop

//-----------------------------------------------------------------------------
void RTC_SetCurrentTime(uint32_t time)
{
   SI_UU32_t current_time;
   
   current_time.u32 = time;

   // Write the time to the capture registers
   RTC0ADR = (0x10 | CAPTURE0);       
   RTC0DAT = current_time.u8[B0];    // write data
   NOP();
   RTC0DAT = current_time.u8[B1];    // write data
   NOP();
   RTC0DAT = current_time.u8[B2];    // write data
   NOP(); 
   RTC0DAT = current_time.u8[B3];    // write data
   NOP(); 
 
   RTC_Write( RTC0CN, RTC0CN_Local | RTC0SET);   // Write '1' to RTC0SET   
   while((RTC_Read(RTC0CN) & RTC0SET));          // Wait for RTC0SET -> 0


}

//-----------------------------------------------------------------------------
// RTC0CN_SetBits()
//-----------------------------------------------------------------------------
//
// Return Value : none 
// Parameters   : 
//
// This function sets bits in the RTC0CN register
//-----------------------------------------------------------------------------
void RTC0CN_SetBits(uint8_t bits)
{
   RTC0CN_Local |= (bits & ~0x03);
   RTC_Write( RTC0CN, RTC0CN_Local | bits);
}

//-----------------------------------------------------------------------------
// RTC0CN_ClearBits()
//-----------------------------------------------------------------------------
//
// Return Value : none 
// Parameters   : 
//
// This function clears bits in the RTC0CN register
//-----------------------------------------------------------------------------
void RTC0CN_ClearBits(uint8_t bits)
{
   RTC0CN_Local &= bits;
   RTC_Write( RTC0CN, RTC0CN_Local);
}    

/*
//-----------------------------------------------------------------------------
// RTC_SleepTicks
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) uint32_t the number of ticks to Sleep
//
// This function sleeps for the specified number of RTC ticks.  This function
// is commented out and has been replaced by a simplified version below which
// does not check for a "wrap" condition.  The simplified version of this
// function cannot be used asynchronously with another "sleep" process,
// however, this version may be used asynchronously at the expense of 
// increased code space and execution time.
// 
// Note: The minimum number of ticks is 3
//
// Note: This routine will exit if a pre-configured RTC Alarm occurs, or if
// another wake-up source occurs.
// 
//-----------------------------------------------------------------------------

void RTC_SleepTicks(uint32_t ticks)
{
   uint8_t EA_save;
   uint8_t PMU0CF_save;
   uint8_t RTC0CN_save;

   UU32 current_time;
   UU32 current_alarm;
   UU32 alarm_value;
   
   uint8_t pending_alarm;
   uint8_t alarm_wrap;  
   
   // Disable Interrupts
   EA_save = IE_EA;
   IE_EA = 0;
   
   // Check for pending alarms
   pending_alarm = RTC_Alarm;

   // Initiate Capture of the current time 
   RTC_Write( RTC0CN, RTC0CN_Local | RTC0CAP);   // Write '1' to RTC0CAP   
   
   // Read the current alarm value
   RTC0ADR = (0xD0 | ALARM0);
   NOP(); NOP(); NOP();
   current_alarm.u8[B0] = RTC0DAT;               // Least significant byte
   NOP(); NOP();
   current_alarm.u8[B1] = RTC0DAT;
   NOP(); NOP();
   current_alarm.u8[B2] = RTC0DAT;
   NOP(); NOP();
   current_alarm.u8[B3] = RTC0DAT;               // Most significant byte
   
   // Copy the current time into <current_time>
   while((RTC_Read(RTC0CN) & RTC0CAP));         // Wait for RTC0CAP -> 0
   RTC0ADR = (0xD0 | CAPTURE0);
   NOP(); NOP(); NOP();
   current_time.u8[B0] = RTC0DAT;               // Least significant byte
   NOP(); NOP();
   current_time.u8[B1] = RTC0DAT;
   NOP(); NOP();
   current_time.u8[B2] = RTC0DAT;
   NOP(); NOP();
   current_time.u8[B3] = RTC0DAT;               // Most significant byte

   // Preserve RTC0CN
   RTC0CN_save = RTC0CN_Local;

   // Check for wrap if alarm is enabled
   if((RTC0CN_Local & RTC0AEN) && (current_time.u32 + ticks > current_alarm.u32))
   {  
      alarm_value.u32 = current_alarm.u32;
      alarm_wrap = 1;

   } else
   {
      alarm_value.u32 = current_time.u32 + ticks;
      RTC0CN_Local &= ~ALRM;
      alarm_wrap = 0;
   }
          
   // Write the alarm value
   RTC_WriteAlarm(alarm_value.u32);   

   // Force the RTC wake-up sources to be enabled
   PMU0CF_save = PMU0CF_Local;
   PMU0CF_Local |= RTC;
   
   // Place device in Sleep Mode
   LPM(SLEEP);

   // Restore Enabled Wake-up Sources and RTC state
   PMU0CF_Local = PMU0CF_save;
   RTC0CN_Local = RTC0CN_save;   

   // Clear Alarm Flag unless a wrap was detected or it was already
   // set upon entry into the function
   if(alarm_wrap == 0 && pending_alarm == 0)
   {
      RTC_Alarm = 0;
   }

   // Restore alarm value unless a wrap was detected
   if(alarm_wrap == 0)
   {
      RTC_WriteAlarm(current_alarm.u32);
   }  
   
   // Restore Interrupts
   IE_EA = EA_save;

}   

*/

//-----------------------------------------------------------------------------
// RTC_SleepTicks
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) uint32_t the number of ticks to Sleep
//
// This function sleeps for the specified number of RTC ticks.  Software should
// ensure that the device will wake up and restore the original RTC alarm 
// value before the next alarm occurs. 
// 
// Note: The minimum number of ticks is 3
//
// 
//-----------------------------------------------------------------------------

void RTC_SleepTicks(uint32_t ticks)
{
   uint8_t EA_save;
   uint8_t PMU0CF_save;
   uint8_t RTC0CN_save;

   SI_UU32_t current_time;
   
   
   // Disable Interrupts
   EA_save = IE_EA;
   IE_EA = 0;

   // Preserve RTC0CN
   RTC0CN_save = RTC0CN_Local;
   
   // Disable Auto Reset
   RTC0CN_ClearBits(~ALRM);

   // Obtain the current time
   current_time.u32 = RTC_GetCurrentTime();
              
   // Write the alarm value
   RTC_WriteAlarm(current_time.u32 + ticks);   

   // Force the RTC wake-up sources to be enabled
   PMU0CF_save = PMU0CF_Local;
   PMU0CF_Local |= RTC;
   
   // Place device in Sleep Mode
   LPM(SLEEP);

   // Restore Enabled Wake-up Sources and RTC state
   PMU0CF_Local = PMU0CF_save;
   RTC0CN_Local = RTC0CN_save;   

   // Clear Alarm Flag
   RTC_Alarm = 0;

   // Restore alarm value unless a wrap was detected
   RTC_WriteAlarm(WAKE_INTERVAL_TICKS);
   
   // Restore Interrupts
   IE_EA = EA_save;

}

#endif       
