//=========================================================
// src/Interupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

         
// USER INCLUDES			
#include <SI_EFM8BB2_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
uint16_t CEX0_Compare_Value;           // Holds current PCA compare value

//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// PCA0 ISR Content goes here. Remember to clear flag bits:
// PCA0CN::CCF0 (PCA Module 0 Capture/Compare Flag)
// PCA0CN::CCF1 (PCA Module 1 Capture/Compare Flag)
// PCA0CN::CCF2 (PCA Module 2 Capture/Compare Flag)
// PCA0CN::CF (PCA Counter/Timer Overflow Flag)
// PCA0PWM::COVF (Cycle Overflow Flag)
//
// This is the ISR for the PCA.  It handles the case when a match occurs on
// channel 0, and updates the PCA0CPn compare register with the value held in
// the global variable "CEX0_Compare_Value".
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
   PCA0PWM &= ~PCA0PWM_COVF__BMASK;    // Clear cycle overflow flag

   PCA0PWM |= PCA0PWM_ARSEL__BMASK;    // Target Auto-reload registers

   PCA0CPL0 = (CEX0_Compare_Value & 0x00FF);
   PCA0CPH0 = (CEX0_Compare_Value & 0xFF00)>>8;

   PCA0PWM &= ~PCA0PWM_ARSEL__BMASK;   // Target PCA0CPH/L registers
}


