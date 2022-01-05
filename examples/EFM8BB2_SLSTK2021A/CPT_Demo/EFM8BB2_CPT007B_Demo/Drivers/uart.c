/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
// Standard library
#include <si_toolchain.h>
#include <string.h>
#include "retargetserial.h"
#include <stdlib.h>

// project library
#include "project_config.h"
#include "uart.h"

/******************************************************************************
 * update UART output contents
 *****************************************************************************/
void updateUART(void)
{
  uint8_t i;
  uint16_t shiftOne = 0x0001;

#ifdef SI_EFM8SB1_DEFS_H // SB1 demo supports CS00-03
  for (i = 0; i < 4; i++)
#else
  for (i = 0; i < TOTAL_CAPSENSE_PIN; i++)
#endif
  {
    if (!(capsensePrevious & shiftOne) && (capsenseCurrent & shiftOne))
    {
      RETARGET_PRINTF("CS%d pressed\n", (int16_t) i);
    }
    else if ((capsensePrevious & shiftOne) && !(capsenseCurrent & shiftOne))
    {
      RETARGET_PRINTF("CS%d released\n", (int16_t) i);
    }
    else
    {
      // nothing
    }
    shiftOne <<= 1;
  }
}
