//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

         
// USER INCLUDES			
#include <SI_EFM8UB1_Register_Enums.h>

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------
SI_SBIT (LED_GREEN, SFR_P1, 4);           // green LED
SI_SBIT (LED_BLUE,  SFR_P1, 5);           // blue LED

//-----------------------------------------------------------------------------
// INT0_ISR
//-----------------------------------------------------------------------------
//
// INT0 ISR Content goes here. Remember to clear flag bits:
// TCON::IE0 (External Interrupt 0)
//
// Whenever a negative edge appears on P0.2, toggle LED_GREEN.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(INT0_ISR, INT0_IRQn)
{
   LED_GREEN = !LED_GREEN;
}

//-----------------------------------------------------------------------------
// INT1_ISR
//-----------------------------------------------------------------------------
//
// INT1 ISR Content goes here. Remember to clear flag bits:
// TCON::IE1 (External Interrupt 1)
//
// Whenever a negative edge appears on P0.3, toggle LED_BLUE.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(INT1_ISR, INT1_IRQn)
{
   LED_BLUE = !LED_BLUE;
}


