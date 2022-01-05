/**************************************************************************//**
 * @file    buttons.c
 * @brief   Support for mapping joystick presses.
 * @author  Silicon Laboratories
 *
 *******************************************************************************
 * @section License
 * (C) Copyright 2016 Silicon Labs Inc,
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *
 ******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

// -----------------------------------------------------------------------------
// Includes

#include "bsp.h"
#include "retargetserial.h"
#include "button.h"
#include "joystick.h"

//------------------------------------------------------------------------------
// Variables

extern volatile uint8_t JoystickDirection;

//------------------------------------------------------------------------------
// Functions

/**************************************************************************//**
 * @brief  Gets joystick status.
 *
 * @return
 *   Which direction on the joystick has been pushed as well as which
 *   buttons have been pressed.
 *
 *****************************************************************************/
uint8_t Joystick_GetStatus(void)
{
  uint8_t joyStatus = 0;

  switch (JoystickDirection)
  {
    case JOYSTICK_N:
      joyStatus |= JOY_UP;
      break;

    case JOYSTICK_NE:
      joyStatus |= (JOY_UP | JOY_RIGHT);
      break;

    case JOYSTICK_E:
      joyStatus |= JOY_RIGHT;
      break;

    case JOYSTICK_SE:
      joyStatus |= (JOY_DOWN | JOY_RIGHT);
      break;

    case JOYSTICK_S:
      joyStatus |= JOY_DOWN;
      break;

    case JOYSTICK_SW:
      joyStatus |= (JOY_DOWN | JOY_LEFT);
      break;

    case JOYSTICK_W:
      joyStatus |= JOY_LEFT;
      break;

    case JOYSTICK_NW:
      joyStatus |= (JOY_UP | JOY_LEFT);
      break;
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
