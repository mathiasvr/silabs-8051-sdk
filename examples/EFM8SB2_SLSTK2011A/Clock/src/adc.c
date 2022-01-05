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

static volatile uint8_t JoystickDirection = JOYSTICK_NONE;

/////////////////////////////////////////////////////////////////////////////
// Global Functions
/////////////////////////////////////////////////////////////////////////////

uint8_t GetJoystickDirection(void)
{
    return JoystickDirection;
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
   static SI_SEGMENT_VARIABLE(mV, uint32_t, SI_SEG_XDATA);

   ADC0CN0_ADINT = 0; // Clear ADC0 conv. complete flag

   mV = ADC0;

   // The 10-bit ADC value is averaged across
   // ADC_NUM_MEASUREMENTS_TO_AVG measurements.
   // The measured voltage applied to P1.2 is then:
   //
   //                           Vref (mV)
   //   measurement (mV) =   --------------- * result (bits)
   //                       (2^10)-1 (bits)

   mV =  (mV * 3300) / 1023;

   JoystickDirection = JOYSTICK_convert_mv_to_direction(mV);
}
