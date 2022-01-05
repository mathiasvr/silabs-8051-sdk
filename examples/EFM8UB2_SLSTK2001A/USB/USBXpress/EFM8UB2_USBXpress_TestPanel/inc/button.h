/**************************************************************************//**
 * @file    button.h
 * @brief   External constant declarations for button.c
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

#ifndef __BUTTON_H__
#define __BUTTON_H__

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// Constants

#define LEFT_BUTTON         (1 << 0)
#define RIGHT_BUTTON        (1 << 1)
#define CENTER_BUTTON       (1 << 2)
#define JOY_UP		          (1 << 3)
#define JOY_DOWN	          (1 << 4)
#define JOY_LEFT	          (1 << 5)
#define JOY_RIGHT	          (1 << 6)

#define JOY_MASK            (JOY_UP | JOY_DOWN | JOY_LEFT | JOY_RIGHT)
#define BUTTON_MASK         (LEFT_BUTTON | RIGHT_BUTTON | CENTER_BUTTON)

// P0.2 and P0.3
#define LEFT_BUTTON_PIN		P0_B3
#define RIGHT_BUTTON_PIN	P0_B2


// -----------------------------------------------------------------------------
// Function Prototypes

uint8_t Joystick_GetStatus(void);

#ifdef __cplusplus
}
#endif

#endif  // __BUTTON_H__
