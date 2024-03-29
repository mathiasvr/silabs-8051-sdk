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
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (LED_BLUE, SFR_P1, 5);
SI_SBIT (LED_RED, SFR_P1, 6);
SI_SBIT (S1, SFR_P0, 2);
SI_SBIT (S2, SFR_P0, 3);

//-----------------------------------------------------------------------------
// PMATCH_ISR
//-----------------------------------------------------------------------------
//
// PMATCH ISR Content goes here. Remember to clear flag bits:
//
// The Port Match interrupt occurs whenever (Pn & PnMASK) != (PnMAT & PnMASK).
// We must disable the interrupt and wait until the switches are released in
// main, or else the interrupt will trigger multiple times.  It is generally
// not good practice to sit in an ISR for int32_t periods of time.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PMATCH_ISR, PMATCH_IRQn)
{
   // If the Port Match event occurred on S1, toggle blue LED.
   if(S1 == 0)
   {
	  LED_BLUE = !LED_BLUE;
   }

   // If the Port Match event occurred on S2, toggle red LED
   if(S2 == 0)
   {
	  LED_RED = !LED_RED;
   }

   SFRPAGE = LEGACY_PAGE;              // EIE1 on SFRPAGE 0x00
   EIE1 &= ~0x02;                      // Disable Port Match interrupts to
									   // prevent multiple interrupts from
									   // occurring while the switches are
									   // pressed
}


