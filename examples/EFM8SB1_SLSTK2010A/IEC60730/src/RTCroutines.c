/**************************************************************************//**
 * Copyright (c) 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "si_toolchain.h"
#include "SI_EFM8SB1_Defs.h"
#include "RTC_driverEnums.h"

// Variables used for the RTC interface
uint8_t RTC_RTC0CNLocal;                       // Holds the desired Wake-up sources
bit configured = false;
void RTC_write(uint8_t reg, uint8_t value);
void RTC_zeroCurrentTime(void);
void RTC_writeAlarm(uint32_t alarm);

/**************************************************************************//**
 * Set alarm for RTC
 *****************************************************************************/
void RTC_setAlarmPeriod(uint16_t alarm_frequency)
{
  RTC_zeroCurrentTime();               // Reset the RTC Timer
  RTC_writeAlarm((RTCCLK * (uint32_t)alarm_frequency) / 1000L);
}

/**************************************************************************//**
 * Initialize RTC block
 *****************************************************************************/
void RTC_init(void)
{
  uint16_t i;

  uint8_t CLKSEL_SAVE = CLKSEL;

  if (configured == true)
  {
    return;
  }
  RTC_RTC0CNLocal = 0;
  // If the interface is locked
  if (RTC0KEY == 0x00)
  {
    RTC0KEY = 0xA5;                    // Unlock the smaRTClock interface
    RTC0KEY = 0xF1;
  }

  // Configure the SmaRTClock to crystal or self-oscillate mode

#if (RTC_CLKSRC == CRYSTAL)

  RTC_write(RTC0XCN, 0x60);            // Configure the smaRTClock
                                       // oscillator for crystal mode
                                       // Bias Doubling Enabled
                                       // AGC Disabled

  RTC_write(RTC0XCF, 0x83);            // Enable Automatic Load Capacitance
                                       // stepping and set the desired
                                       // load capacitance to 4.5pF plus
                                       // the stray PCB capacitance

#elif (RTC_CLKSRC == SELFOSC) || (RTC_CLKSRC == LFO)

  // Configure smaRTClock to self-oscillate mode with bias X2 disabled
  RTC_write(RTC0XCN, 0x08);

  // Disable Auto Load Cap Stepping
  RTC_write(RTC0XCF, (0x00 | LOADCAP_VALUE));

#endif

  RTC_write(RTC0CN, 0x80);             // Enable smaRTClock oscillator

  CLKSEL = 0x74;                       // Switch to 156 kHz low power
                                       // internal oscillator

  // Wait for crystal to start if not using self-oscillate

#if (RTC_CLKSRC == CRYSTAL)

  // Wait > 20 ms
  for (i = 0x550; i != 0; i--)
  {
    ;
  }

  // Wait for smaRTClock valid
  while ((RTC_read(RTC0XCN) & 0x10) == 0x00)
  {
    ;
  }

  // Wait for Load Capacitance Ready
  while ((RTC_read(RTC0XCF) & 0x40) == 0x00)
  {
    ;
  }

  RTC_write(RTC0XCN, 0xC0);               // Enable Automatic Gain Control
                                          // and disable bias doubling

#endif

  RTC_write(RTC0CN, 0xC0);             // Enable missing smaRTClock detector
                                       // and leave smaRTClock oscillator
                                       // enabled.

  // Wait > 2 ms (assumption: running at 22MHz/128)
  for (i = 50; i != 0; i--)
  {
  }

  RTC_write(RTC0CN, 0xC0);             // Clear the clock fail flag (precautionary).

  PMU0CF = 0x20;                       // Clear PMU wake-up source flags

  CLKSEL = CLKSEL_SAVE;                // Restore system clock
  while (!(CLKSEL & 0x80))
  {                                    // Poll CLKRDY
  }
  RTC_RTC0CNLocal = 0xC0;                 // Initialize Local Copy of RTC0CN
  configured = true;
}

/**************************************************************************//**
 * Read indirect RTC register
 * @param reg address of indirect register to read
 *
 * Function follows datasheet guidelines to initiate a read, wait
 * a defined period of time, and then return the value.
 *
 * @return value retrieved through indirect register access
 *
 *****************************************************************************/
uint8_t RTC_read(uint8_t reg)
{

  RTC0ADR = (0x90 | reg);              // pick register and set BUSY to
                                       // initiate the read

  NOP(); NOP(); NOP();                 // delay 3 system clocks

  return RTC0DAT;                      // return value
}

/**************************************************************************//**
 * Write indirect RTC register
 * @param reg address of indirect register to read
 * @param value value to write into indirect register
 *
 * Function follows datasheet guidelines to initiate a read, wait
 * a defined period of time, and then return the value.
 *
 * This function will Write one byte to the specified RTC register.
 * Using a register number greater that 0x0F is not permitted.
 *
 *****************************************************************************/
void RTC_write(uint8_t reg, uint8_t value)
{
  RTC0ADR = (0x10 | reg);              // pick register
  RTC0DAT = value;                     // write data
}

/**************************************************************************//**
 * Write to alarm registers in RTC
 * @param alarm 4-byte alarm value
 *
 * Function disables the RTC alarm, writes four bytes to indirect ALARM
 * registers with auto-increment enabled, and then restores RTC state.
 *
 *****************************************************************************/
void RTC_writeAlarm(uint32_t alarm)
{
  SI_UU32_t alarm_value;

  alarm_value.u32 = alarm;

  // Temporarily disable alarm while updating registers
  RTC0ADR = (0x10 | RTC0CN);
  RTC0DAT = (RTC_RTC0CNLocal & ~RTC0AEN);
  NOP();

  // Write the value to the alarm registers
  RTC0ADR = (0x10 | ALARM0);
  RTC0DAT = alarm_value.u8[B0];     // write data
  NOP();
  RTC0DAT = alarm_value.u8[B1];     // write data
  NOP();
  RTC0DAT = alarm_value.u8[B2];     // write data
  NOP();
  RTC0DAT = alarm_value.u8[B3];     // write data
  NOP();

  // Restore alarm state after registers have been written
  RTC0ADR = (0x10 | RTC0CN);
  RTC0DAT = RTC_RTC0CNLocal;
}

/**************************************************************************//**
 * Set wake-up source bits for RTC
 *****************************************************************************/
void RTC0CN_setBits(uint8_t bits)
{
  RTC_RTC0CNLocal |= (bits & ~0x03);
  RTC_write(RTC0CN, RTC_RTC0CNLocal | bits);
}

/**************************************************************************//**
 * Clear wake-up sources for RTC
 *****************************************************************************/
void RTC0CN_clearBits(uint8_t bits)
{
  RTC_RTC0CNLocal &= ~bits;
  RTC_write(RTC0CN, RTC_RTC0CNLocal);
}

/**************************************************************************//**
 * Clear current time in RTC
 *
 * Function disables the RTC alarm, writes four bytes to indirect ALARM
 * registers with auto-increment enabled, and then restores RTC state.
 *
 *****************************************************************************/
void RTC_zeroCurrentTime(void)
{
  // Write the time to the capture registers
  RTC0ADR = (0x10 | CAPTURE0);
  RTC0DAT = 0x00;                      // write data
  NOP();
  RTC0DAT = 0x00;                      // write data
  NOP();
  RTC0DAT = 0x00;                      // write data
  NOP();
  RTC0DAT = 0x00;                      // write data
  NOP();

  // Write '1' to RTC0SET
  RTC_write(RTC0CN, RTC_RTC0CNLocal | RTC0SET);
  while ((RTC_read(RTC0CN) & RTC0SET))
  {                                    // Wait for RTC0SET -> 0
  }
}

