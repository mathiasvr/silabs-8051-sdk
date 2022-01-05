/**************************************************************************//**
 * Copyright (c) 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

// Disable warnings on unused labels.  Labels are used to set breakpoints for
// automated testing.
#pragma warninglevel(1)

#include "iec60730.h"
#include "iec60730_oem_plausibility.h"

/**************************************************************************//**
 * @addtogroup efm8_iec60730
 * @{ 
 * @addtogroup IEC60730_GPIO_PLAUSIBILTY_Test
 * @{
 *****************************************************************************/

#message "This function must be filled out properly by the OEM!"
void iec60730_GpioInit()
{
  XBR2 |= XBR2_XBARE__ENABLED;
}

#message "This function must be filled out properly by the OEM!"
void iec60730_GpioCheck()
{
  iec60730_programmeCounterCheck |= IEC60730_GPIO_COMPLETE;
  return;
}

/** @} (end addtogroup IEC60730_GPIO_PLAUSIBILTY_Test) */

/**************************************************************************//**
 * @addtogroup IEC60730_ADC_PLAUSIBILTY_Test
 * @{
 *****************************************************************************/

#message "This function must be filled out properly by the OEM!"
void iec60730_AnalogCheck()
{
  iec60730_programmeCounterCheck |= IEC60730_ANALOG_COMPLETE;
  return;
}

/** @} (end addtogroup IEC60730_ADC_PLAUSIBILTY_Test) */
/** @} (end addtogroup efm8_iec60730) */

