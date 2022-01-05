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
// Global Constants
//-----------------------------------------------------------------------------
#define LED1_TOGGLE_RATE         100   // LED1 toggle rate in milliseconds
                                       // If LED1_TOGGLE_RATE = 1, LED1 will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (LED1, SFR_P1, 4);                // green LED

//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// TIMER2 ISR Content goes here. Remember to clear flag bits:
// TMR2CN::TF2H (Timer # High Byte Overflow Flag)
// TMR2CN::TF2L (Timer # Low Byte Overflow Flag)
//
// Here we process the Timer2 interrupt and toggle the LED.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
	static uint16_t count_ms = 0;
	count_ms += 1;

	if (count_ms == LED1_TOGGLE_RATE)
	{
		LED1 = !LED1;  // Toggle the LED
		count_ms = 0;
	}

	TMR2CN0_TF2H = 0;  // Clear the interrupt flag
}


