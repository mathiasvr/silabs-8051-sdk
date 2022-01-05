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
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// PCA0 ISR Content goes here. Remember to clear flag bits:
// PCA0CN0::CCF0 (PCA Module 0 Capture/Compare Flag)
// PCA0CN0::CCF1 (PCA Module 1 Capture/Compare Flag)
// PCA0CN0::CCF2 (PCA Module 2 Capture/Compare Flag)
// PCA0CN0::CF (PCA Counter/Timer Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
	static uint16_t current_capture_value, previous_capture_value;
	static uint16_t capture_period;


	if (PCA0CN0_CCF0)                           // If Module 0 caused the interrupt
	{
	  PCA0CN0_CCF0 = 0;                        // Clear module 0 interrupt flag.

	  // Store most recent capture value
	  current_capture_value = PCA0CP0;

	  // Calculate capture period from last two values.
	  capture_period = current_capture_value - previous_capture_value;

	  // Update previous capture value with most recent info.
	  previous_capture_value = current_capture_value;

	}
	else                                // Interrupt was caused by other bits.
	{
	  PCA0CN0 &= ~0x86;                 // Clear other interrupt flags for PCA
	}
}

