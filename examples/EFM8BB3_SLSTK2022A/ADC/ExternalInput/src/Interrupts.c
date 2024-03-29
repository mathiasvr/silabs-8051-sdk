//=========================================================
// src/Interupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

         
// USER INCLUDES			
#include <SI_EFM8BB3_Register_Enums.h>
#include "retargetserial.h"

//-----------------------------------------------------------------------------
// ADC0EOC_ISR
//-----------------------------------------------------------------------------
//
// ADC0EOC ISR Content goes here. Remember to clear flag bits:
// ADC0CN0::ADINT (Conversion Complete Interrupt Flag)
//
// This ISR averages 2048 samples then prints the result to the terminal.  The
// ISR is called after each ADC conversion which is triggered by Timer2.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC0EOC_ISR, ADC0EOC_IRQn)
{
   static uint32_t accumulator = 0;     // Accumulator for averaging
   static uint16_t measurements = 2048; // Measurement counter
   uint32_t result = 0;
   uint32_t mV;                         // Measured voltage in mV
   uint8_t SFRPAGE_save;


   ADC0CN0_ADINT = 0;              // Clear ADC0 conv. complete flag

   accumulator += ADC0;
   measurements--;

   if(measurements == 0)
   {
	  measurements = 2048;
	  result = accumulator / 2048;
	  accumulator = 0;

	  // The 14-bit ADC value is averaged across 2048 measurements.
	  // The measured voltage applied to P1.7 is then:
	  //
	  //                           Vref (mV)
	  //   measurement (mV) =   --------------- * result (bits)
	  //                       (2^12)-1 (bits)

	  mV =  (result * 3300) / 4095;

	  SFRPAGE_save = SFRPAGE;
	  SFRPAGE = LEGACY_PAGE;
	  RETARGET_PRINTF ("\nP1.7 voltage: %ld mV\n", mV);
	  SFRPAGE = SFRPAGE_save;
   }
}


