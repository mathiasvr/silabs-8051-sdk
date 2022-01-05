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
extern uint32_t ADC_AVG;               // Accumulates the ADC samples
extern bool CONV_COMPLETE;              // ADC accumulated result ready flag

//-----------------------------------------------------------------------------
// ADC0EOC_ISR
//-----------------------------------------------------------------------------
//
// ADC0EOC ISR Content goes here. Remember to clear flag bits:
// ADC0CN0::ADINT (Conversion Complete Interrupt Flag)
//
// This ISR triggers when the ADC has accumulated 32 14-bit samples into
// the ADC0 register. This result is shifted in hardware to the right by 3,
// effectively dividing it by 8, giving a 16-bit result. This is then
// divided again by 4 to give a 14-bit average.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC0EOC_ISR, ADC0EOC_IRQn)
{
   ADC0CN0_ADINT = 0;               // Clear ADC0 conv. complete flag
   ADC_AVG = ADC0/4;
   CONV_COMPLETE = 1;               // Set result ready flag
}


