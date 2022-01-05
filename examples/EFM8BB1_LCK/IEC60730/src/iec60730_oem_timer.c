/**************************************************************************//**
 * Copyright (c) 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "si_toolchain.h"
#include "iec60730.h"
#include "iec60730_oem_timer.h"
#include "iec60730_oem_comm.h"

/**************************************************************************//**
 * @addtogroup efm8_iec60730
 * @{ 
 * @addtogroup IEC60730_SYSTEM_CLOCK_Test
 * @{
 *****************************************************************************/

SI_SEGMENT_VARIABLE(iec60730_clockTestTolerance, uint8_t, SI_SEG_IDATA) = IEC60730_CLOCK_TEST_TOLERANCE;

void iec60730_TimersInit(void)
{
  LFO0CN |= LFO0CN_OSCLEN__ENABLED | LFO0CN_OSCLD__DIVIDE_BY_1;
  // $[Wait for LFOSC Ready]
  while ((LFO0CN & LFO0CN_OSCLRDY__BMASK) != LFO0CN_OSCLRDY__SET)

  // Initialize timer 2 as test timer
  TMR2RL = -(SYSCLK/12/IEC_TEST_TIMER_OVERFLOW_FREQUENCY/4);
  TMR2   = -(SYSCLK/12/IEC_TEST_TIMER_OVERFLOW_FREQUENCY/4);
  TMR2CN0 |= TMR2CN0_TR2__RUN;

  // Enable interrupts
  IE |= IE_ET2__ENABLED;

  iec60730_systemClockToTestClockFrequency = SYSCLK /12l / TESTCLK;
  
  iec60730_clockTicksReset();
}

/**************************************************************************//**
 * Measure number of system clocks per LFO
 *****************************************************************************/
void set_system_clock_ticks(void)
{
  uint16_t system_clock_counter = 0;

  // Wait until TF3H is set, indicating LFO clock edge has
  // latched a system clock / 12 value
  TMR3CN0 &= ~TMR3CN0_TF3H__SET;
  while (!(TMR3CN0 & TMR3CN0_TF3H__SET));
  // Save latched value and wait for a second latch
  system_clock_counter = TMR3RL;
  TMR3CN0 &= ~TMR3CN0_TF3H__SET;
  while (!(TMR3CN0 & TMR3CN0_TF3H__SET));

  // Calculate the difference between two latches, accounting for overflow
  if (TMR3RL < system_clock_counter)
  {
    system_clock_counter = 0xFFFF - system_clock_counter + TMR3RL;
  }
  else
  {
    system_clock_counter = TMR3RL - system_clock_counter;
  }

  // Increment the system clock tick by the number of ticks in the latch difference
  while (system_clock_counter != 0)
  {
    iec60730_SystemClockTick();
    system_clock_counter--;
  }
}

/**************************************************************************//**
 * Timer 2 interrupt
 *****************************************************************************/
SI_INTERRUPT(timer2_isr, TIMER2_IRQn)
{
  static uint8_t counter = 0;
  counter++;
  if (iec60730_IRQExecCount[0] < 0xFF)
  {
    iec60730_IRQExecCount[0]++;
  }
  TMR2CN0_TF2H = 0;  // Clear the interrupt flag
  TMR2CN0_TF2L = 0;  // Clear the interrupt flag
  if (counter >= 4)
  {
    set_system_clock_ticks();
    iec60730_TestClockTick();

    iec60730_commTransmitStatusMessage();

    iec60730_commInitReceiveStatusMessage();

    counter = 0;
  }
}

/** @} (end addtogroup IEC60730_SYSTEM_CLOCK_Test) */
/** @} (end addtogroup efm8_iec60730) */

