//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

         
// USER INCLUDES
#include <SI_EFM8BB51_Register_Enums.h>

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
extern volatile uint8_t updateDiv;

//-----------------------------------------------------------------------------
// INT0_ISR
//-----------------------------------------------------------------------------
//
// INT0 ISR Content goes here. Remember to clear flag bits:
// TCON::IE0 (External Interrupt 0)
//
// Whenever a negative edge appears on P0.2, set updateDiv flag.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(INT0_ISR, INT0_IRQn)
{
  updateDiv = 1;
}

//-----------------------------------------------------------------------------
// INT1_ISR
//-----------------------------------------------------------------------------
//
// INT1 ISR Content goes here. Remember to clear flag bits:
// TCON::IE1 (External Interrupt 1)
//
// Whenever a negative edge appears on P0.3, set updateDiv flag.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(INT1_ISR, INT1_IRQn)
{
  updateDiv = 1;
}

