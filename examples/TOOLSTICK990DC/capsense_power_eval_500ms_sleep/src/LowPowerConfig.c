/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "ProjectConfig.h"
#include "LowPowerHardware.h"

#include "SI_C8051F990_Register_Enums.h"
#include "LowPowerConfig.h"

xdata uint8_t TimerTick = 0;


//-----------------------------------------------------------------------------
// Local function prototypes
//-----------------------------------------------------------------------------
void RTC_Init (void);
void LPM_Init (void);
void RTC_Set_Alarm_Fequency(uint16_t alarm_frequency);
uint8_t RTC_Read (uint8_t reg);
void RTC_Write (uint8_t reg, uint8_t value);
void RTC_WriteAlarm (uint32_t alarm);
void RTC0CN_SetBits(uint8_t bits);
void RTC0CN_ClearBits(uint8_t bits);
void RTC_ZeroCurrentTime(void);
void ConfigureRTCActiveMode(void);
void ConfigureRTCSleepMode(void);
uint8_t UpdateRTCFlags(void);
void ConfigureCS0SleepMode(void);
void ConfigurePortsSleepMode(void);


//-----------------------------------------------------------------------------
// File-scope variables
//-----------------------------------------------------------------------------
// Variables used for the RTC interface

// Holds the desired RTC0CN settings
SI_SEGMENT_VARIABLE(RTC0CN_Local, uint8_t, SI_SEG_DATA);  

SI_SEGMENT_VARIABLE(rtcclk_freq, uint16_t, SI_SEG_XDATA);

// Variables used for the RTC interface
uint8_t PMU0CF_Local;                       // Holds the desired Wake-up sources
 
// Define Wake-Up Flags
uint8_t RTC_Alarm;
uint8_t RTC_Failure;
uint8_t Comparator_Wakeup;
uint8_t Port_Match_Wakeup;

typedef struct RegisterSaveState{
   uint8_t CLKSEL_save;
   uint8_t P0MDIN_save;
   uint8_t P1MDIN_save;
   uint8_t XBR1_save;
} RegisterSaveStateStruct;

xdata RegisterSaveStateStruct RegisterSaveState;
//-----------------------------------------------------------------------------
// Implementation-dependent functions called by LowPowerRoutines.c
//-----------------------------------------------------------------------------

void ReadyRegistersForSleep(void)
{
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = LEGACY_PAGE;


   RegisterSaveState.CLKSEL_save = CLKSEL;
   CLKSEL = 0x14;
   while (!(CLKSEL & 0x80));

   RegisterSaveState.P0MDIN_save = P0MDIN;
   RegisterSaveState.P1MDIN_save = P1MDIN;
   RegisterSaveState.XBR1_save = XBR1;

   P0MDIN = 0x07;	// Keep LEDs and S2 active during sleep mode
   P1MDIN = 0x00;

   XBR1 = 0x00;

   SFRPAGE = SFRPAGE_save;
}
void RestoreRegistersFromSleep(void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   CLKSEL = RegisterSaveState.CLKSEL_save;
   while (!(CLKSEL & 0x80));

   P0MDIN = RegisterSaveState.P0MDIN_save;
   P1MDIN = RegisterSaveState.P1MDIN_save;
   XBR1 = RegisterSaveState.XBR1_save;
   SFRPAGE = SFRPAGE_save;
}
//-----------------------------------------------------------------------------
// EnterLowPowerState
//-----------------------------------------------------------------------------
//
// Enter low power sleep mode.
//
void EnterLowPowerState(void)
{
   ReadyRegistersForSleep();

   // Enable the Flash read one-shot timer
   FLSCL &= ~BYPASS;                   // Clear the one-shot bypass bit
   FLWR  =   NON_ZERO;                 // Write a "dummy" value to FLWR

   // Verify that the system clock setting has been applied
   while(!(CLKSEL & 0x80));            // Wait until CLKRDY -> 1

   PMU0CF_Config(SLEEP | RTC | PORT_MATCH);   // Enter Sleep Until Next Event

   // Disable (Bypass) the Flash Read one-shot timer if the system clock

   FLSCL |= BYPASS;                 // Set the one-shot bypass bit

   UpdateRTCFlags();             // This call will clear the alarm flag
   RestoreRegistersFromSleep();
}

//-----------------------------------------------------------------------------
// Implementation-dependent functions called by LowPowerRoutines.c
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// ConfigureTimerForActiveMode
//-----------------------------------------------------------------------------
//
// This is a top-level call to configure the timer to active mode, one of the
// two defined modes of operation in the system.  This instance of the function
// configures the SmaRTClock to the frequency defined in ProjectConfig.h.
//
void ConfigureTimerForActiveMode(void)
{
   ConfigureRTCActiveMode();
}



//-----------------------------------------------------------------------------
// CheckTimer
//-----------------------------------------------------------------------------
//
// Top-level call into an abstracted timer.  This function is in charge
// of setting the TimerTick variable whenever a configured amount of time
// passes according to the timer being used.  
// This instance of the CheckTimer() routine uses the SmaRTClock through
// a call to UPdateRTCFlags(), which returns TRUE if the configured
// elapsed time has passed.
//
void CheckTimer(void)
{
   
   if(UpdateRTCFlags())
   {
      TimerTick = 1;                   // Checked and cleared in low power code
   }
}


//-----------------------------------------------------------------------------
// ConfigureSensorForSleepMode
//-----------------------------------------------------------------------------
//
// This is a top-level call to configure the sensor to its operational state
// during sleep mode.
//
void ConfigureSensorForSleepMode(void)
{
   ConfigurePortsSleepMode();
   ConfigureCS0SleepMode();
}


//-----------------------------------------------------------------------------
// ConfigureTimerForSleepMode
//-----------------------------------------------------------------------------
//
// This is a top-level call to configure the timer to sleep mode, one of the
// two defined modes of operation in the system.  This instance of the function
// configures the SmaRTClock to the frequency defined in ProjectConfig.h.
//
void ConfigureTimerForSleepMode(void)
{
   ConfigureRTCSleepMode();
}

//-----------------------------------------------------------------------------
// ConfigurePortsSleepMode
//-----------------------------------------------------------------------------
//
// This is a low-level local routine to handle configuration of the ports
// for active mode.  It configures the analog pins to select
// channels that are bound to a single sensor input.
//
void ConfigurePortsSleepMode(void)
{

   P0MDIN &= ~0x08;            // Set TOUCH_SENSE_SWITCH P0.3 to analog
}



//-----------------------------------------------------------------------------
// ConfigureCS0SleepMode
//-----------------------------------------------------------------------------
//
// This is a low-level local routine to handle configuration of the ports
// for active mode.
//
void ConfigureCS0SleepMode(void)
{

   CS0CN = 0x88;                       // Enable CS0, Enable Digital Comparator
                                       // Clear CS0INT, Clear CS0CMPF
   // Bind channels 
   CS0SCAN0 = 0x08;						// Bind channel P0.3, TOUCH_SENSE_SWITCH

   CS0CF  = 0x8A;                      // Enable binding and enable channel masking, 8x oversample
   CS0MD1 &= ~0x07;                    // Clear Gain Field
   CS0MD1  |= 0x02;                    // Set Gain to 3X
   CS0MD2 = 0x00;                      // 12-bit mode
}


//-----------------------------------------------------------------------------
// ConfigureRTCSleepMode
//-----------------------------------------------------------------------------
//
// Initialize the SmaRTClock to overflow at the frequency specified in
// ProjectConfig.h.
//
void ConfigureRTCSleepMode(void)
{
   LPM_Init ();                        // Initialize Power Management Unit
   RTC_Init ();                        // Initialize SmaRTClock

   RTC_Set_Alarm_Fequency(DEF_SLEEP_MODE_PERIOD); // Set the Alarm Frequency to 25 Hz
   RTC0CN_SetBits(RTC0TR+RTC0AEN+ALRM);// Enable Counter, Alarm, and Auto-Reset
}

//-----------------------------------------------------------------------------
// ConfigureRTCActiveMode
//-----------------------------------------------------------------------------
//
// Initialize the SmaRTClock to overflow at the frequency specified in
// ProjectConfig.h.
//
void ConfigureRTCActiveMode(void)
{
   LPM_Init ();                        // Initialize Power Management Unit
   RTC_Init ();                        // Initialize SmaRTClock

   RTC_Set_Alarm_Fequency(DEF_ACTIVE_MODE_PERIOD); // Set the Alarm Frequency to 25 Hz
   RTC0CN_SetBits(RTC0TR+RTC0AEN+ALRM);// Enable Counter, Alarm, and Auto-Reset
   
}


//-----------------------------------------------------------------------------
// ConfigureRTCActiveMode
//-----------------------------------------------------------------------------
//
// Reads all potential wake-up sources.  Only the alarm is communicated
// to upper levels because this is the flag that indicates when
// the sleep/active mode state machine needs to take action.
//
uint8_t UpdateRTCFlags(void)
{
   uint8_t PMU0CF_state, b;
   PMU0CF_state = PMU0CF_Get();
   // Check for an RTC Alarm
   if((PMU0CF_state & RTCAWK))
   {
      RTC_Alarm = 1;
   }
   // Check for an RTC Clock Failure
   if((PMU0CF_state & RTCFWK))
   {
      RTC_Failure = 1;  
   }
   // Check for a Port Match Wakeup
   if(PMU0CF_state & PMATWK)
   {
      Port_Match_Wakeup = 1;  
   }
   // Check for a reset pin Wakeup
   if(PMU0CF_state & RSTWK)
   {
      // Delay greater than 15uS per datasheet recommendation
      b = 255; 
      while(b > 0) b--;
   }

   PMU0CF_Config(CLEAR);

   if(RTC_Alarm)
   {
      RTC_Alarm = 0;
      return 1;
   }
   else
   {
      return 0;
   }
}

// LPM_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will initialize the low power functionality
//
//-----------------------------------------------------------------------------
void LPM_Init (void)
{
  PMU0CF_Config(CLEAR);
  PMU0CF_Local = 0;
  RTC_Alarm = 0;
  RTC_Failure = 0;
  Comparator_Wakeup = 0;
  Port_Match_Wakeup = 0;
}

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
   static uint8_t configured = 0;

   uint8_t CLKSEL_SAVE = CLKSEL;

   if(configured) return;

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

      // Configure smaRTClock to self-oscillate mode with bias X2 disabled
      RTC_Write (RTC0XCN, 0x08);

      // Disable Auto Load Cap Stepping
      RTC_Write (RTC0XCF, (0x00 | LOADCAP_VALUE));

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
      for (i=0x550; i!=0; i--);

      // Wait for smaRTClock valid
      while ((RTC_Read (RTC0XCN) & 0x10)== 0x00);

      // Wait for Load Capacitance Ready
      while ((RTC_Read (RTC0XCF) & 0x40)== 0x00);

      RTC_Write (RTC0XCN, 0xC0);          // Enable Automatic Gain Control
                                          // and disable bias doubling

   #endif

   RTC_Write (RTC0CN, 0xC0);           // Enable missing smaRTClock detector
                                       // and leave smaRTClock oscillator
                                       // enabled.

   // Wait > 2 ms
   for (i=0x540; i!=0; i--);

   RTC_Write (RTC0CN, 0xC0);           // Clear the clock fail flag (precautionary).

   PMU0CF = 0x20;                      // Clear PMU wake-up source flags

   CLKSEL = CLKSEL_SAVE;               // Restore system clock
   while(!(CLKSEL & 0x80));            // Poll CLKRDY

   RTC0CN_Local = 0xC0;                // Initialize Local Copy of RTC0CN
   configured = 1;
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

//-----------------------------------------------------------------------------
// RTC_Set_Alarm_Fequency ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : alarm_frequency
//
// This function will set the RTC Alarm Interval to alarm_ms milliseconds
//
//-----------------------------------------------------------------------------
void RTC_Set_Alarm_Fequency(uint16_t alarm_frequency)
{

   RTC_ZeroCurrentTime();              // Reset the RTC Timer
   RTC_WriteAlarm((RTCCLK * (uint32_t)alarm_frequency) / 1000L);

}

void RTC_ZeroCurrentTime(void)
{
	// Write the time to the capture registers
	RTC0ADR = (0x10 | CAPTURE0);
	RTC0DAT = 0x00;                   // write data
	NOP();
	RTC0DAT = 0x00;                   // write data
	NOP();
	RTC0DAT = 0x00;                   // write data
	NOP();
	RTC0DAT = 0x00;                   // write data
	NOP();

	RTC_Write( RTC0CN, RTC0CN_Local | RTC0SET);   // Write '1' to RTC0SET
	while((RTC_Read(RTC0CN) & RTC0SET));          // Wait for RTC0SET -> 0
}
/*



//-----------------------------------------------------------------------------
// Low level RTC and LPM control
//-----------------------------------------------------------------------------



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

   static unsigned char RTC_Configured = 0;
   uint16_t i;

   uint8_t CLKSEL_SAVE = CLKSEL;

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

      // Configure smaRTClock to self-oscillate mode with bias X2 disabled
      RTC_Write (RTC0XCN, 0x00);
                                      
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
      for (i=0x550; i!=0; i--);

      // Wait for smaRTClock valid
      while ((RTC_Read (RTC0XCN) & 0x10)== 0x00);

      // Wait for Load Capacitance Ready
      while ((RTC_Read (RTC0XCF) & 0x40)== 0x00);

      RTC_Write (RTC0XCN, 0xC0);          // Enable Automatic Gain Control
                                          // and disable bias doubling

   #endif

   RTC_Write (RTC0CN, 0xC0);           // Enable missing smaRTClock detector
                                       // and leave smaRTClock oscillator
                                       // enabled.

   // Wait > 2 ms
   for (i=0x540; i!=0; i--);

   RTC_Write (RTC0CN, 0xC0);           // Clear the clock fail flag (precautionary).
   
   PMU0CF = 0x20;                      // Clear PMU wake-up source flags

   CLKSEL = CLKSEL_SAVE;               // Restore system clock
   while(!(CLKSEL & 0x80));            // Poll CLKRDY

   RTC0CN_Local = 0xC0;                // Initialize Local Copy of RTC0CN



   RTC_Configured = 1;
}


//-----------------------------------------------------------------------------
// RTC_Set_Alarm_Fequency ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : alarm_frequency 
//
// This function will set the RTC Alarm Interval to alarm_ms milliseconds
//
//-----------------------------------------------------------------------------
void RTC_Set_Alarm_Fequency(uint16_t alarm_frequency)
{

   RTC_ZeroCurrentTime();              // Reset the RTC Timer
   RTC_WriteAlarm((RTCCLK * (uint32_t)alarm_frequency) / 1000L);

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

	while (RTC0ADR & 0x80);			// Poll to ensure the smartclk interface is free
	RTC0ADR = (reg | 0x80);				// write the read register and write 1 to BUSY bit (bit 7)
	while (RTC0ADR & 0x80);  			// Poll for busy bit

   return RTC0DAT;                     	// return value
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


	while (RTC0ADR & 0x80);			// Poll busy bit to ensure the smartclk interface is free
   RTC0ADR = reg;								//write to address
	RTC0DAT = value;							// Write data
	while (RTC0ADR & 0x80);  			// Poll for busy bit
	
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
   SI_SEGMENT_VARIABLE(alarm_value, UU32, SI_SEG_IDATA);

   alarm_value.u32 = alarm;   
   //alarm_local = alarm;

   // Temporarily disable alarm while updating registers
   RTC0ADR = (0x10 | RTC0CN);
   RTC0DAT = (RTC0CN_Local & ~RTC0AEN);       
   while (RTC0ADR & 0x80);						 // poll for busy bit -> 0
   
   // Write the value to the alarm registers
   RTC0ADR = (0x10 | ALARM0);       
   RTC0DAT = alarm_value.u8[B0];    // write data
   while (RTC0ADR & 0x80);						 // poll for busy bit -> 0
   RTC0DAT = alarm_value.u8[B1];    // write data
   while (RTC0ADR & 0x80);						 // poll for busy bit -> 0
   RTC0DAT = alarm_value.u8[B2];    // write data
   while (RTC0ADR & 0x80);					 // poll for busy bit -> 0
   RTC0DAT = alarm_value.u8[B3];    // write data
   while (RTC0ADR & 0x80);						 // poll for busy bit -> 0
   
   // Restore alarm state after registers have been written
   RTC0ADR = (0x10 | RTC0CN);
   RTC0DAT = RTC0CN_Local;  

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

//-----------------------------------------------------------------------------
// LPM_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will initialize the low power functionality
//
//-----------------------------------------------------------------------------
void LPM_Init (void)
{
  PMU0CF_Config(CLEAR);
  PMU0CF_Local = 0;
  RTC_Alarm = 0;
  RTC_Failure = 0;
  Comparator_Wakeup = 0;
  Port_Match_Wakeup = 0;
}


//-----------------------------------------------------------------------------
// LPM_Enable_Wakeup ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will enable wakeup sources
//
//-----------------------------------------------------------------------------
void LPM_Enable_Wakeup (uint8_t wakeup)
{
  PMU0CF_Local |= (wakeup & (RTC | PORT_MATCH | COMPARATOR));
}

//-----------------------------------------------------------------------------
// LPM_Disable_Wakeup ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will disable wakeup sources
//
//-----------------------------------------------------------------------------
void LPM_Disable_Wakeup (uint8_t wakeup)
{
  PMU0CF_Local &= ~(wakeup & (RTC | PORT_MATCH | COMPARATOR));
}

//-----------------------------------------------------------------------------
// RTC_ZeroCurrentTime()
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
void RTC_ZeroCurrentTime(void)
{
   // Write the time to the capture registers
   RTC0ADR = (0x10 | CAPTURE0);       
   RTC0DAT = 0x00;                   // write data
   NOP();
   RTC0DAT = 0x00;                   // write data
   NOP();
   RTC0DAT = 0x00;                   // write data
   NOP(); 
   RTC0DAT = 0x00;                   // write data   
   NOP(); 
 
   RTC_Write( RTC0CN, RTC0CN_Local | RTC0SET);   // Write '1' to RTC0SET   
   while((RTC_Read(RTC0CN) & RTC0SET));          // Wait for RTC0SET -> 0
}

*/
