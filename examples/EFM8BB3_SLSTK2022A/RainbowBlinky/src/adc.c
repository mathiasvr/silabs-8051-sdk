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

uint8_t joystickDirection = JOYSTICK_NONE;

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
  uint32_t mV;

  ADC0CN0_ADINT = 0; // Clear ADC0 conv. complete flag

  mV = (ADC0 * (uint32_t) 3300) / 1023;

  joystickDirection = JOYSTICK_convert_mv_to_direction(mV);
}
