/**************************************************************************//**
 * Copyright (c) 2021 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "dac_0.h"

#if EFM8PDL_DAC0_AUTO_PAGE == 1
// declare variable needed for autopage enter/exit
#define DECL_PAGE uint8_t savedPage
// enter autopage section
#define SET_PAGE(p)     do                                                    \
                        {                                                     \
                          savedPage = SFRPAGE;  /* save current SFR page */   \
                          SFRPAGE = (p);        /* set SFR page */            \
                        } while(0)
// exit autopage section
#define RESTORE_PAGE    do                                                    \
                        {                                                     \
                          SFRPAGE = savedPage;  /* restore saved SFR page */  \
                        } while(0)
#else
#define DECL_PAGE
#define SET_PAGE(p)
#define RESTORE_PAGE
#endif

#define SFRPAGE_DAC0 (0x30)

#if EFM8PDL_DAC0_USE_INIT == 1
void DAC0_init(DAC0_UpdateTrigger_t trigger)
{
  DECL_PAGE;
  SET_PAGE(SFRPAGE_DAC0);

  // DAC0 VREF == VDD; Gain = 0
  DAC0CF1 = DAC0CF1_SRC__DACALT | DAC0CF1_AMEN__FALSE | DAC0CF1_VREFSEL__VDD
      | DAC0CF1_GAINADJ__FALSE;

  // DAC0 Right justified; Normal reset (all resets will reset DAC0 and
  //   associated registers); Update trigger source = trigger
  DAC0CF0 = DAC0CF0_EN__DISABLE | trigger
      | DAC0CF0_LJST__RIGHT_JUSTIFY | DAC0CF0_RSTMD__NORMAL;

  DAC0_enable();
  RESTORE_PAGE;
}
#endif

void DAC0_enable(void)
{
  DECL_PAGE;
  SET_PAGE(SFRPAGE_DAC0);
  DAC0CF0 |=  DAC0CF0_EN__ENABLE;
  RESTORE_PAGE;
}

void DAC0_disable(void)
{
  DECL_PAGE;
  SET_PAGE(SFRPAGE_DAC0);
  DAC0CF0 &= ~DAC0CF0_EN__BMASK;
  RESTORE_PAGE;
}

void DAC0_setOutput(uint16_t value)
{
  DECL_PAGE;
  SET_PAGE(SFRPAGE_DAC0);
  // MUST load DAC0L first!
  DAC0L = value & 0xFF;
  DAC0H = value >> 8;
  RESTORE_PAGE;
}

uint16_t DAC0_getOutput(void)
{
  uint16_t value;
  DECL_PAGE;
  SET_PAGE(SFRPAGE_DAC0);
  value = (DAC0H << 8) | (DAC0L & 0xFF);
  RESTORE_PAGE;
  return value;
}
