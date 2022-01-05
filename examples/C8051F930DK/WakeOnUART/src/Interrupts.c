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
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK          24500000L      // SYSCLK frequency in Hz
#define TIMEOUT_MS         1           // Wait time in ms

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


