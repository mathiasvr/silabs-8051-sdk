/**************************************************************************//**
 * Copyright (c) 2014 by Silicon Laboratories Inc. All rights reserved.
 *
 * (C) Copyright 2014 Silicon Labs Inc,
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef _RTC_DRIVER_H_
#define _RTC_DRIVER_H_
#include "si_toolchain.h"
#include "RTC_driverEnums.h"

/**************************************************************************//**
 * Sets RTC alarm period
 * @param period time in ms before next alarm
 *
 * Function configures alarm to fire a wake-up event.
 *
 *****************************************************************************/
void RTC_setAlarmPeriod(uint16_t period);

/**************************************************************************//**
 * Initialize RTC peripheral
 *
 * Function checks to see if RTC peripheral has already been configured
 * using a one-shot variable.  If it hasn't been configured, the RTC is initialized
 * to use either a crystal or the LFO.
 *
 *****************************************************************************/
void RTC_init(void);

/**************************************************************************//**
 * Clear RTC wake-up bits
 *
 * @param bits 1-byte bit array of wake-up source bits
 *
 * Clears wake-up bits in the mask.
 *
 *****************************************************************************/
void RTC0CN_setBits(uint8_t bits);


void RTC0CN_clearBits(uint8_t bits);

#endif
