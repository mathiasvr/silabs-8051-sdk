/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// button.c
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "button.h"
#include "joystick.h"

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

static volatile uint8_t JoystickDirection;

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Joystick_GetStatus() Routine
// ----------------------------------------------------------------------------
//
// Description - Get joystick status.
//
// Return - Which direction on the joystick has been pushed as well as which
//          buttons have been pressed
//-----------------------------------------------------------------------------
uint8_t Joystick_GetStatus(void)
{
  uint8_t joyStatus = 0;

	switch (JoystickDirection)
  {
  case JOYSTICK_N:    joyStatus |= JOY_UP;                break;
  case JOYSTICK_NE:   joyStatus |= JOY_UP | JOY_RIGHT;    break;
  case JOYSTICK_E:    joyStatus |= JOY_RIGHT;             break;
  case JOYSTICK_SE:   joyStatus |= JOY_DOWN | JOY_RIGHT;  break;
  case JOYSTICK_S:    joyStatus |= JOY_DOWN;              break;
  case JOYSTICK_SW:   joyStatus |= JOY_DOWN | JOY_LEFT;   break;
  case JOYSTICK_W:    joyStatus |= JOY_LEFT;              break;
  case JOYSTICK_NW:   joyStatus |= JOY_UP | JOY_LEFT;     break;
  }

    if (!LEFT_BUTTON_PIN)
    {
        joyStatus |= LEFT_BUTTON;
    }
    if (!RIGHT_BUTTON_PIN)
    {
        joyStatus |= RIGHT_BUTTON;
    }
    if (JoystickDirection == JOYSTICK_C)
    {
        joyStatus |= CENTER_BUTTON;
    }

  return joyStatus;
}

//-----------------------------------------------------------------------------
// ADC Interrupt Routine
// ----------------------------------------------------------------------------
//
// - ADC input = P1.7
// - SARCLK = 48 MHz / 8 = 6 MHz
// - SOC = T3 overflow
// - 10-bit mode
//
SI_INTERRUPT(ADC_ISR, ADC0EOC_IRQn)
{
    uint16_t sample;
    uint32_t mv;

    ADC0CN0_ADINT = 0;

    sample = ADC0;
    mv = ((uint32_t)sample) * 3300 / 1024;

    JoystickDirection = JOYSTICK_convert_mv_to_direction(mv);
}
