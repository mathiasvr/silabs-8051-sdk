//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================


// USER INCLUDES
#include <SI_EFM8SB1_Register_Enums.h>

extern volatile bool Blink;
SI_SEGMENT_VARIABLE(halfSecondCounter, uint16_t, SI_SEG_XDATA) = 0; //counts to half of a second in units of 100 us

//-----------------------------------------------------------------------------
// TIMER0_ISR
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER0_ISR, TIMER0_IRQn)
{
    halfSecondCounter++;
    if(halfSecondCounter == 50)
    {
        halfSecondCounter = 0;
        Blink ^= 1;
    }
}

SI_INTERRUPT(RTC0FAIL_ISR, RTC0FAIL_IRQn)
{
  while (1);
}

#if 0
//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// TIMER2 ISR Content goes here. Remember to clear flag bits:
// TMR2CN0::TF2H (Timer # High Byte Overflow Flag)
// TMR2CN0::TF2L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{

}


//-----------------------------------------------------------------------------
// SPI0_ISR
//-----------------------------------------------------------------------------
//
// SPI0 ISR Content goes here. Remember to clear flag bits:
// SPI0CN0::MODF (Mode Fault Flag)
// SPI0CN0::RXOVRN (Receive Overrun Flag)
// SPI0CN0::SPIF (SPI# Interrupt Flag)
// SPI0CN0::WCOL (Write Collision Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(SPI0_ISR, SPI0_IRQn)
{

}


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

}


//-----------------------------------------------------------------------------
// TIMER3_ISR
//-----------------------------------------------------------------------------
//
// TIMER3 ISR Content goes here. Remember to clear flag bits:
// TMR3CN0::TF3H (Timer # High Byte Overflow Flag)
// TMR3CN0::TF3L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER3_ISR, TIMER3_IRQn)
{

}


#endif
