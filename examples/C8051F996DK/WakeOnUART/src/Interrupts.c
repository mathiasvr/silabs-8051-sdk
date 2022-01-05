//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================


// USER INCLUDES
#include <SI_C8051F990_Register_Enums.h>
#include "F99x-98x_WakeOnUART.h"

//-----------------------------------------------------------------------------
// PMATCH_ISR
//-----------------------------------------------------------------------------
//
// PMATCH ISR Content goes here. Remember to clear flag bits:

//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PMATCH_ISR, PMATCH_IRQn)
{
	TMR2CN_TR2 = 0;
	TMR2CN_TF2H = 0;
	TMR2 = -(TIMEOUT_MS*SYSCLK/12/1000);
	TMR2CN_TR2 = 1;
}

