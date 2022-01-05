/**************************************************************************//**
 * Copyright (c) 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include <iec60730_oem_timer.h>
#include "iec60730_oem_comm.h"
#include "si_toolchain.h"
#include "iec60730.h"
#include "rtc_driver.h"

/**************************************************************************//**
 * @addtogroup efm8_iec60730
 * @{ 
 * @addtogroup IEC60730_SYSTEM_CLOCK_Test
 * @{
 *****************************************************************************/

SI_SEGMENT_VARIABLE(iec60730_clockTestTolerance, uint8_t, SI_SEG_IDATA) = IEC60730_CLOCK_TEST_TOLERANCE;

void iec60730_TimersInit(void)
{
  SI_UU16_t reload_value;
  DECL_PAGE;
  SET_PAGE(LEGACY_PAGE);
  RTC_init();                          // Use RTC with LFO as clock source
  CKCON0 |= 0x01;                      // Enable timer 2 as 16-bit timer
  TMR2CN0 = 0x07;
  TMR2RL = -IEC_TEST_TIMER_OVERFLOW;
  TMR2 = -IEC_TEST_TIMER_OVERFLOW;
  IE_ET2 = 1;                          // Enable timer 2 interrupt

  PCA0CPM1 = 0x89;                     // Module 1 = software timer, interrupts enabled, match enabled

  reload_value.u8[LSB] = PCA0L;        // Read current low value (latches high value)
  reload_value.u8[MSB] = PCA0H;        // Read high value

  reload_value.u16 += IEC_SYSTEM_TIMER_OVERVFLOW;

  PCA0CPL1 = reload_value.u8[LSB];     // Set new match value
  PCA0CPH1 = reload_value.u8[MSB];

  EIE1 |= 0x10;                        // Enable PCA interrupts

  iec60730_systemClockToTestClockFrequency = IEC_SYSTEM_TIMER_OVERFLOW_FREQUENCY/IEC_TEST_TIMER_OVERFLOW_FREQUENCY;
  
  iec60730_clockTicksReset();
  
  RESTORE_PAGE;
}

/**************************************************************************//**
 * Timer 2 interrupt
 *****************************************************************************/
SI_INTERRUPT(timer2_isr, TIMER2_IRQn)
{
  DECL_PAGE;
  SET_PAGE(LEGACY_PAGE);
  if (iec60730_IRQExecCount[0] < 0xFF)
  {
    iec60730_IRQExecCount[0]++;
  }

  TMR2CN0 &= ~0xC0;                    // Clear interrupt flag

  iec60730_TestClockTick();

  iec60730_commTransmitStatusMessage();

  iec60730_commInitReceiveStatusMessage();

  RESTORE_PAGE;
}

/**************************************************************************//**
 * PCA interrupt
 *****************************************************************************/
SI_INTERRUPT(pca0_isr, PCA0_IRQn)
{
  SI_UU16_t reload_value;
  DECL_PAGE;
  SET_PAGE(LEGACY_PAGE);
  if (iec60730_IRQExecCount[1] < 0xFF)
  {
    iec60730_IRQExecCount[1]++;
  }
  // Channel 0 interrupt, update haptic output
  if (PCA0CN0_CCF1 == 1)
	{
    PCA0CN0_CCF1 = 0;                  // Clear interrupt flag
    reload_value.u8[MSB] = PCA0CPH1;   // Read last match value to calculate
    reload_value.u8[LSB] = PCA0CPL1;   // next overflow value

    reload_value.u16 += IEC_SYSTEM_TIMER_OVERVFLOW;

    PCA0CPL1 = reload_value.u8[LSB];   // Set new match value
    PCA0CPH1 = reload_value.u8[MSB];

    iec60730_SystemClockTick();

	}
  RESTORE_PAGE;
}

/** @} (end addtogroup IEC60730_SYSTEM_CLOCK_Test) */
/** @} (end addtogroup efm8_iec60730) */

