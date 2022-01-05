/******************************************************************************
 * Copyright (c) 2020 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "pwr.h"
#include <SI_EFM8BB52_Register_Enums.h>

// Mask of all wake flags in PSTAT0 (used for reading PSTAT0)
#define PWR_PSTAT0_WAKE_FLAGS (PWR_CPT0_WF | PWR_PMAT_WF | PWR_TMR4_WF | PWR_CL0_WF) //| PWR_I2C0_WF | PWR_SPI0_WF

// Mask of all reset values for wake flags in PSTAT0 (used for clearing PSTAT0)
#define PWR_PSTAT0_WAKE_CLEARS (~PWR_PSTAT0_WAKE_FLAGS) 

#if (EFM8PDL_PWR_AUTO_PAGE == 1)
#define SAVE_SFRPAGE uint8_t save_sfrpage = SFRPAGE
#define RESTORE_SFRPAGE SFRPAGE = save_sfrpage
#else
#define SAVE_SFRPAGE
#define RESTORE_SFRPAGE
#endif

// Reads and clears all PSTAT0 wake flags.
uint8_t PWR_readAndClearWakeFlags(void)
{
  uint8_t save_ie, snapshot_pstat0;
  SAVE_SFRPAGE;
  SFRPAGE = 0x10;

  // Disable interrupts to minimize time between reading and clearing the flags
  save_ie = IE;
  IE_EA = 0;

  // Snapshot the wake-up source flags, then clear them
  snapshot_pstat0 = PSTAT0 & PWR_PSTAT0_WAKE_FLAGS;
  PSTAT0 &= PWR_PSTAT0_WAKE_CLEARS;

  // Now restore the interrupt enable
  IE = save_ie;
  RESTORE_SFRPAGE;

  // Return snapshot
  return snapshot_pstat0;
}

void PWR_enterIdle(void)
{
  SAVE_SFRPAGE;
  SFRPAGE = 0;

  // Set the IDLE bit followed by a 3-cycle dummy instruction to ensure the
  // CPU wakes under all conditions (see ref manual)
  PCON0 |= PCON0_CPUIDLE__BMASK;
  PCON0 = PCON0;

  RESTORE_SFRPAGE;
}

#if (EFM8PDL_PWR_USE_STOP == 1)
void PWR_enterStop(void)
{
  // No need to service interrupts or save the SFRPAGE since the device
  // will be stopped
  IE_EA = 0;
  SFRPAGE = 0x10;
  
  // Clock source must be HFOSC0 for lowest power (see ref manual)
  // Enable HFOSC0 then source the system clock from it
  HFO0CN |= HFO0CN_HFOOSCEN__BMASK;
  CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__HFOSC0_clk49;
  while((CLKSEL & CLKSEL_DIVRDY__BMASK) == 0);
  
  // Set the STOP bit
  PCON0 |= PCON0_CPUSTOP__BMASK;

  // NOTE: Reset is required to exit stop mode. Program execution
  // will not continue to this point.
}
#endif // EFM8PDL_PWR_USE_STOP

void PWR_enterSnooze(void)
{
  uint8_t save_ie, save_clksel;
  SAVE_SFRPAGE;
  SFRPAGE = 0;

  // Disable interrupts until the system is awake and restored
  save_ie = IE;
  IE_EA = 0;
  
  // Enable LFO0OSC oscillator
  LFO0CN |=  LFO0CN_OSCLEN__ENABLED | LFO0CN_LFODIV__DIVIDE_BY_1;
  while ((LFO0CN & LFO0CN_LFORDY__BMASK) != LFO0CN_LFORDY__SET);

  // Set the system clock to LFO0OSC / 1
  save_clksel = CLKSEL;
  CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__LFOSC;
  while((CLKSEL & CLKSEL_DIVRDY__BMASK) == 0);

  // Enter suspend mode
  PCON1 |= PCON1_SNOOZE__SNOOZE;
  
  // Next restore the system clock source and divider
  CLKSEL = save_clksel;
  while((CLKSEL & CLKSEL_DIVRDY__BMASK) == 0);

  // Finally restore the interrupt enable
  IE = save_ie;
  RESTORE_SFRPAGE;
}

void PWR_enterShutdown(void)
{
  // No need to service interrupts or save the SFRPAGE since the device
  // will be stopped
  IE_EA = 0;
  SFRPAGE = 0;

  // Set the STOPCF bit to shutdown regulator 
  REG0CN |= REG0CN_STOPCF__SHUTDOWN;

  // Set the STOP bit
  PCON0 |= PCON0_CPUSTOP__BMASK;

  // NOTE: Reset is required to exit stop mode. Program execution
  // will not continue to this point.
}
