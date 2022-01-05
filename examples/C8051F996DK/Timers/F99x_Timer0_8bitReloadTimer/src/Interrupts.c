//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================


// USER INCLUDES
#include <SI_C8051F990_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define LED_TOGGLE_RATE           100  // LED toggle rate in milliseconds
                                       // if LED_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond


SI_SBIT(LED, SFR_P1, 3);                 // LED==1 means ON

//-----------------------------------------------------------------------------
// TIMER0_ISR
//-----------------------------------------------------------------------------
//
// TIMER0 ISR Content goes here. Remember to clear flag bits:
// TCON::TF0 (Timer 0 Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER0_ISR, TIMER0_IRQn)
{
	static int16_t counter = 0;

	if((counter++) == LED_TOGGLE_RATE)
	{
	  LED = !LED;                      // Toggle the LED
	  counter = 0;
	}
}


