//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

         
// USER INCLUDES			
#include <SI_EFM8UB3_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SAMPLING_NUMBER            256 // Number of samples per calculation

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
extern uint32_t ADC_SUM;               // Accumulates the ADC samples
extern SI_BIT(CONV_COMPLETE);          // ADC accumulated result ready flag

//-----------------------------------------------------------------------------
// ADC0EOC_ISR
//-----------------------------------------------------------------------------
//
// ADC0EOC ISR Content goes here. Remember to clear flag bits:
// ADC0CN0::ADINT (Conversion Complete Interrupt Flag)
//
// This ISR averages 256 samples then copies the result to ADC_SUM.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC0EOC_ISR, ADC0EOC_IRQn)
{
   static uint32_t accumulator = 0;         // Accumulator for averaging
   static uint16_t measurements = SAMPLING_NUMBER; // Measurement counter

   ADC0CN0_ADINT = 0;                          // Clear ADC0 conv. complete flag

   // Checks if obtained the necessary number of samples
   if(measurements == 0)
   {
      ADC_SUM = accumulator;           // Copy total into ADC_SUM
      measurements = SAMPLING_NUMBER;  // Reset counter
      accumulator = 0;                 // Reset accumulator
      CONV_COMPLETE = 1;               // Set result ready flag
   }
   else
   {
      accumulator += ADC0 >> 6;        // If not, keep adding while shifting
                                       // out unused bits in ADC0
      measurements--;
   }
}


