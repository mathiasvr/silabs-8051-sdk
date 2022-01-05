/**************************************************************************//**
 * Copyright (c) 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include <cpt213b_oem_timer.h>
#include "si_toolchain.h"
#include "iec60730.h"
#include "lcd.h"
#include "smbus.h"
#include "cpt213b_state_machine.h"
/**************************************************************************//**
 * @addtogroup efm8_iec60730
 * @{ 
 * @addtogroup IEC60730_SYSTEM_CLOCK_Test
 * @{
 *****************************************************************************/
SI_SEGMENT_VARIABLE(iec60730_clockTestTolerance, uint8_t, SI_SEG_IDATA) = IEC60730_CLOCK_TEST_TOLERANCE;

void iec60730_TimersInit(void)
{
  DECL_PAGE;
  SET_PAGE(LEGACY_PAGE);
  // Initialize test timer, using LFO/8 as clock source

  // Initialize LFO for test timer
  SFRPAGE = 0x00;
  LFO0CN |= LFO0CN_OSCLEN__ENABLED;

  while ((LFO0CN & LFO0CN_OSCLRDY__BMASK) != LFO0CN_OSCLRDY__SET)
    ;

  // Initialize timer 4 as test timer
  SFRPAGE = TIMER4_PAGE;
  TMR4RL = -IEC_TEST_TIMER_OVERFLOW;
  TMR4 = -IEC_TEST_TIMER_OVERFLOW;
  TMR4CN0 |= TMR4CN0_T4XCLK__LFOSC_DIV_8;
  EIE2 |= EIE2_ET4__ENABLED;

  TMR4CN0 |= TMR4CN0_TR4__RUN;

  // Initialize timer 2 as test timer
  SFRPAGE = TIMER2_PAGE;
  TMR2RL = -IEC_SYSTEM_TIMER_OVERFLOW;
  TMR2 = -IEC_SYSTEM_TIMER_OVERFLOW;
  TMR2CN0 |= TMR2CN0_TR2__RUN;

  // Enable interrupts
  SFRPAGE = LEGACY_PAGE;
  IE |= IE_ET2__ENABLED;

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
  SET_PAGE(TIMER2_PAGE);
  if (iec60730_IRQExecCount[1] < 0xFF)
  {
    iec60730_IRQExecCount[1]++;
  }
  // Clear interrupt flag
  TMR2CN0 &= ~(TMR2CN0_TF2H__BMASK | TMR2CN0_TF2L__BMASK);
  iec60730_SystemClockTick();
  RESTORE_PAGE;
}

/**************************************************************************//**
 * Timer 4 interrupt
 *****************************************************************************/
SI_INTERRUPT(timer4_isr, TIMER4_IRQn)
{
  static uint8_t missedStatusPkt = 0;
  uint8_t skullFactor = 3; // used to print a skull if the CPT device goes into
    	  	  	  	  	  	  //safe state
  DECL_PAGE;
  SET_PAGE(TIMER4_PAGE);

  if (iec60730_IRQExecCount[0] < 0xFF)
  {
    iec60730_IRQExecCount[0]++;
  }
  // Clear interrupt flag
  TMR4CN0 &= ~(TMR4CN0_TF4H__BMASK | TMR4CN0_TF4L__BMASK);
  iec60730_TestClockTick();
  RESTORE_PAGE;
  // Check to see if a status packet has been received

  SET_PAGE(LEGACY_PAGE);
  if(!(cpt_state == UPDATE_SMBUS))
  {
    // if in config load or entering sensing mode
	// do not look for missing status packets
	missedStatusPkt = 0;
  }
  else
  {
	// Increment if status packets are not received
    if((!firstStatusPkt) && (cpt_state == UPDATE_SMBUS))
	{
	  missedStatusPkt++;
	}
	else
    {
	  // Reset missed packets to 0 once status packets
	  // are received
	  if(cpt_state == UPDATE_SMBUS)
	  {
	    missedStatusPkt = 0;
		firstStatusPkt = false;
	  }
	}
  }
  // Enter IEC safe state if we don't get the status packet after 300 ms
  if(missedStatusPkt > 3)
  {
	cpt_state = ENTER_SAFE_STATE;
  }
  RESTORE_PAGE;
}

/** @} (end addtogroup IEC60730_SYSTEM_CLOCK_Test) */
/** @} (end addtogroup efm8_iec60730) */

