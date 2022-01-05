//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================


// USER INCLUDES
#include "bsp.h"
#include "smb_0.h"
#include "SMBus_Lib_Buffered_Slave_Multibyte.h"

//-----------------------------------------------------------------------------
// TIMER3_ISR
//-----------------------------------------------------------------------------
//
// TIMER3 ISR Content goes here. Remember to clear flag bits:
// TMR3CN0::TF3H (Timer # High Byte Overflow Flag)
// TMR3CN0::TF3L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER3_ISR, TIMER3_IRQn)
{
  // Clear Timer 3 high overflow flag
  TMR3CN0 &= ~TMR3CN0_TF3H__SET;

  numErrors++;

  // Abort the transfer and set rx/tx bytes remaining to 0
  SMB0_abortTransfer();

  SMB0CF &= ~SMB0CF_ENSMB__ENABLED;// Disable SMBus
  SMB0CF |= SMB0CF_ENSMB__ENABLED;// Re-enable SMBus

  SMB0CN0_STA = 0;
}
