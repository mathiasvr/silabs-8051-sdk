/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
#ifndef GPIO_H_
#define GPIO_H_

extern uint16_t capsenseCurrent;
extern uint16_t capsensePrevious;

/******************************************************************************
 * GPIO data structure initialization
 *****************************************************************************/
void initGPIO(void);

/******************************************************************************
 * update GPIO data structure from current values of seven GPIO pins
 *****************************************************************************/
void updateGPIO(void);

#endif /* GPIO_H_ */
