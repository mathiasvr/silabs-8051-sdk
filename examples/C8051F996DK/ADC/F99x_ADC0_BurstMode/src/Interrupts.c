//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================


// USER INCLUDES
#include <SI_C8051F990_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Generic UART definitions to support both Keil and SDCC
//-----------------------------------------------------------------------------

#ifdef SDCC
#include <sdcc_stdio.h>
#endif

char *GETS (char *, uint16_t);

#ifdef __C51__
#define GETS gets
#endif

#ifdef SDCC
#include <sdcc_stdio.c>
#endif

//-----------------------------------------------------------------------------
// ADC0EOC_ISR
//-----------------------------------------------------------------------------
//
// ADC0EOC ISR Content goes here. Remember to clear flag bits:
// ADC0CN::ADINT (Conversion Complete Interrupt Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC0EOC_ISR, ADC0EOC_IRQn)
{
	uint32_t result;
	uint32_t mV;                   // measured voltage in mV
	static uint8_t sample_count = 0;

	ADC0CN_ADINT = 0;                         // clear ADC0 conv. complete flag

	result = ADC0;                      // copy ADC0 into result

	// The 12-bit ADC value represents the voltage applied
	// to P1.4 according to the following equation:
	//
	//                           Vref (mV)
	//   measurement (mV) =   --------------- * result (bits)
	//                       ((2^10)-1)*2^2 (bits)

	mV =  result * 3300 / 4092;

	// Print the results every 10 samples
	if(sample_count >= 10)
	{
	  printf("\f\nP0.6 voltage: %d mV", (uint16_t) mV);
	  sample_count = 0;

	} else
	{
	  sample_count++;
	}
}



