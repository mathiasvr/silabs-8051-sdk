//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8BB1_Register_Enums.h>
#include "adc_0.h"

bool ADC_WINDOW_COMPARE = false;

//-----------------------------------------------------------------------------
// ADC0WC_ISR
//-----------------------------------------------------------------------------
//
// ADC0WC ISR Content goes here. Remember to clear flag bits:
// ADC0CN0::ADWINT (Window Compare Interrupt Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC0WC_ISR, ADC0WC_IRQn)
{
  uint16_t sample;
  ADC0_clearIntFlags(ADC0_WINDOW_COMPARE_IF);

  // Re-set the window compare values to be slightly above and below the
  // current ADC sample value.
  sample = ADC0_getResult();
  ADC0_setWindowCompare(sample - 32, sample + 32);

  ADC_WINDOW_COMPARE = true;
}
