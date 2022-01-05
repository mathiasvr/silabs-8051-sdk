//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================


// USER INCLUDES
#include <SI_EFM8BB1_Register_Enums.h>



//-----------------------------------------------------------------------------
// ADC0EOC_ISR
//-----------------------------------------------------------------------------
//
// ADC0EOC ISR Content goes here. Remember to clear flag bits:
// ADC0CN0::ADINT (Conversion Complete Interrupt Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC0EOC_ISR, ADC0EOC_IRQn)
{
	ADC0CN0_ADINT = 0;
}



//-----------------------------------------------------------------------------
// ADC0WC_ISR
//-----------------------------------------------------------------------------
//
// ADC0WC ISR Content goes here. Remember to clear flag bits:
// ADC0CN0::ADWINT (Window Compare Interrupt Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT_USING(ADC0WC_ISR, ADC0WC_IRQn, 2)
{
    ADC0CN0_ADWINT = 0;


}

