/**************************************************************************//**
 * Copyright (c) 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef __IEC60335_OEM_PLAUSIBILITY_H__
#define __IEC60335_OEM_PLAUSIBILITY_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**************************************************************************//**
 * @addtogroup efm8_iec60730
 * @{ 
 * @addtogroup IEC60730_GPIO_PLAUSIBILTY_Test
 * @{
 *****************************************************************************/

extern void iec60730_GpioInit();
extern void iec60730_GpioCheck();
/** @} (end addtogroup IEC60730_GPIO_PLAUSIBILTY_Test) */

/**************************************************************************//**
 * @addtogroup IEC60730_ADC_PLAUSIBILTY_Test
 * @{
 *****************************************************************************/

void iec60730_AnalogCheck();
/** @} (end addtogroup IEC60730_ADC_PLAUSIBILTY_Test) */
/** @} (end addtogroup efm8_iec60730) */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

