/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// adc.c
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "adc.h"
#include "joystick.h"


/////////////////////////////////////////////////////////////////////////////
// Globals
/////////////////////////////////////////////////////////////////////////////

uint32_t v100u = 33000; // Measured voltage in 100uV (10^-4 V)

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

uint32_t ADC_GetVoltage(void)
{
  uint32_t voltage;
  EIE1 &= ~EIE1_EADC0__BMASK; // Disable ADC end of conversion interrupts
  voltage = v100u;
  EIE1 |= EIE1_EADC0__BMASK;  // Enable ADC end of conversion interrupts
  return voltage;
}

/////////////////////////////////////////////////////////////////////////////
// Interrupt Service Handlers
/////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// ADC0EOC_ISR
//-----------------------------------------------------------------------------
//
// ADC0EOC ISR Content goes here. Remember to clear flag bits:
// ADC0CN0::ADINT (Conversion Complete Interrupt Flag)
//
// This ISR averages ADC_MEASUREMENTS_TO_AVG samples then prints the result
// to the terminal.  The ISR is called after each ADC conversion which is
// triggered by Timer0.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(ADC0EOC_ISR, ADC0EOC_IRQn)
{
   static uint32_t accumulator = 0; // Accumulator for averaging
   static uint16_t measurements = ADC_MEASUREMENTS_TO_AVG; // Measurement counter
   uint32_t result = 0;

   ADC0CN0_ADINT = 0; // Clear ADC0 conv. complete flag

   accumulator += ADC0;
   measurements--;

   if(measurements == 0)
   {
      measurements = ADC_MEASUREMENTS_TO_AVG;
      result = accumulator / ADC_MEASUREMENTS_TO_AVG;
      accumulator = 0;

      // The 14-bit ADC value is averaged across
      // ADC_NUM_MEASUREMENTS_TO_AVG measurements.
      // The measured voltage applied to input is then:
      //
      //                             Vref (10^-4 V)
      //   measurement (10^-4 V) =  ---------------- * result (bits)
      //                            (2^14)-1 (bits)

      v100u =  result * ADC_VREF_MV * 10 / 16383;
   }
}



