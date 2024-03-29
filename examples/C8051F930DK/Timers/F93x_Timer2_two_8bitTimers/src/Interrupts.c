//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================


// USER INCLUDES
#include <SI_C8051F930_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define YELLOW_TOGGLE_RATE         100    // Yellow LED toggle rate in milliseconds
                                          // if YELLOW_TOGGLE_RATE = 1, the yellow LED will
                                          // be on for 1 millisecond and off for
                                          // 1 millisecond

#define RED_TOGGLE_RATE            30     // Red LED toggle rate in milliseconds
                                          // if RED_TOGGLE_RATE = 1, the
                                          // red LED will be on for 1 millisecond
                                          // and off for 1 millisecond

SI_SBIT(YELLOW_LED, SFR_P1, 6);             // YELLOW_LED == 0 means ON
SI_SBIT(RED_LED, SFR_P1, 5);                // Red LED to be used also by Timer2

//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// TIMER2 ISR Content goes here. Remember to clear flag bits:
// TMR2CN::TF2H (Timer # High Byte Overflow Flag)
// TMR2CN::TF2L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
	static uint16_t low_counter=0;     // Define counter variables
	static uint16_t high_counter=0;

	if(TMR2CN_TF2L == 1)
	{
	  if((low_counter++) == YELLOW_TOGGLE_RATE)
	  {
		 low_counter = 0;                 // Reset interrupt counter
		 YELLOW_LED = !YELLOW_LED;        // Toggle the yellow LED
	  }
	  TMR2CN_TF2L = 0;
	}
	if(TMR2CN_TF2H == 1)
	{
	  if((high_counter++) == RED_TOGGLE_RATE)
	  {
		 high_counter = 0;                // Reset interrupt counter
		 RED_LED = !RED_LED;              // Toggle the Red LED
	  }
	  TMR2CN_TF2H = 0;
	}
}



