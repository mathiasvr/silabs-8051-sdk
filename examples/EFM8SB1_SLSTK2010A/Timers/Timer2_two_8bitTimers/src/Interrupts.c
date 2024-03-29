//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================


// USER INCLUDES
#include <SI_EFM8SB1_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define LED0_TOGGLE_RATE          100  // LED0 toggle rate in milliseconds
                                       // if LED0_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

#define TEST_OUT_TOGGLE_RATE      250  // TEST_OUT pin toggle rate in milliseconds
                                       // if TEST_OUT_TOGGLE_RATE = 1, the
                                       // SIGNAL output will be on for 1
                                       // millisecond and off for 1
                                       // millisecond

SI_SBIT (LED0, SFR_P1, 1);             // LED0 == 1 means ON
SI_SBIT (TEST_OUT, SFR_P0, 1);


//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// TIMER2 ISR Content goes here. Remember to clear flag bits:
// TMR2CN0::TF2H (Timer # High Byte Overflow Flag)
// TMR2CN0::TF2L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
	static uint16_t low_counter=0;     // Define counter variables
	static uint16_t high_counter=0;

	if(TMR2CN0_TF2L == 1)
	{
	  if((low_counter++) == LED0_TOGGLE_RATE)
	  {
		 low_counter = 0;                 // Reset interrupt counter
		 LED0 = !LED0;        // Toggle the yellow LED
	  }
	  TMR2CN0_TF2L = 0;
	}
	if(TMR2CN0_TF2H == 1)
	{
	  if((high_counter++) == TEST_OUT_TOGGLE_RATE)
	  {
		 high_counter = 0;                // Reset interrupt counter
		 TEST_OUT = !TEST_OUT;              // Toggle the Red LED
	  }
	  TMR2CN0_TF2H = 0;
	}
}



