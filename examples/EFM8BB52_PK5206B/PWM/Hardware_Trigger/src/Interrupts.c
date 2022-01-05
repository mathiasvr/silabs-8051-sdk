//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

         
// USER INCLUDES			
#include <SI_EFM8BB52_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PWM0_ISR
//-----------------------------------------------------------------------------
//
// This routine arms the PWM for the next hardware trigger after being halted
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PWM0_ISR, PWM_IRQn)
{
  uint8_t SFRPAGE_save = SFRPAGE;
  SFRPAGE = PG2_PAGE;
  PWMIF = 0;
  PWMCFG1 &= ~PWMCFG1_PWMEN__ENABLE;
  PWMCFG1 |= PWMCFG1_PWMEN__ENABLE;
  SFRPAGE = SFRPAGE_save;
}
