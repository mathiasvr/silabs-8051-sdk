//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

         
// USER INCLUDES			
#include <SI_EFM8LB1_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
int LEDCOUNT = 0;
//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
SI_SBIT(LED0, SFR_P1, 4);                  // P1.4 LED0
SI_SBIT(LED1, SFR_P1, 5);                  // P1.5 LED1
SI_SBIT(LED2, SFR_P1, 6);                  // P1.6 LED2

//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// TIMER2 ISR Content goes here. Remember to clear flag bits:
// TMR2CN::TF2H (Timer # High Byte Overflow Flag)
// TMR2CN::TF2L (Timer # Low Byte Overflow Flag)
//
// This routine changes the state of the LED whenever Timer2 overflows.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
	TMR2CN0_TF2H = 0;                  // Clear Timer2 interrupt flag

	switch (LEDCOUNT)
	{
		case 0:
		   LED0 = 1;
		   LED1 = 0;
		   LED2 = 0;
		   break;
		case 1:
		   LED0 = 0;
		   LED1 = 1;
		   LED2 = 0;
		   break;
		case 2:
		   LED0 = 0;
		   LED1 = 0;
		   LED2 = 1;
		   break;
		case 3:
		   LED0 = 1;
		   LED1 = 1;
		   LED2 = 0;
		   break;
		case 4:
		   LED0 = 0;
		   LED1 = 1;
		   LED2 = 1;
		   break;
		case 5:
		   LED0 = 1;
		   LED1 = 0;
		   LED2 = 1;
		   break;
		case 6:
		   LED0 = 1;
		   LED1 = 1;
		   LED2 = 1;
		   break;
	}

	if (LEDCOUNT <= 6) LEDCOUNT++;
	else LEDCOUNT = 0;
}


