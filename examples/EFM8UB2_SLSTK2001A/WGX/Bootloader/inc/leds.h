/******************************************************************************
 * Copyright (c) 2018 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef __LEDS_H__
#define __LEDS_H__

#include "hboot_config.h"

#ifdef ENABLE_LEDS
SI_SBIT(LED0, SFR_P1, 6); // P1.6 LED
SI_SBIT(LED1, SFR_P1, 7); // P1.7 LED
SI_SBIT(LED2, SFR_P2, 0); // P2.0 LED
#endif

#endif