/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/*
 * 			GPIO data structure
 * 	For CP27000 there are 7 capsense outputs, each pin
 * 	corresponds to a capsenseCurrent bit showing whether a
 * 	cap-sense button pressed or not, and capsensePrevious
 * 	bit showing the status of previous cycle.
 *
 * 	If capsenseCurrent==1 && capsensePrevious==0, detects a "press"
 * 	If capsenseCurrent==0 && capsensePrevious==1, detects a "release"
 *
 * 	capsenseCurrent bit-map byte:
 * 	|unused_bit       |capsenseCurrent6 |capsenseCurrent5 |capsenseCurrent4 |
 * 	|capsenseCurrent3 |capsenseCurrent2 |capsenseCurrent1 |capsenseCurrent0 |
 *
 * 	capsensePrevious bit-map byte:
 * 	|unused_bit       |capsensePrevious6|capsensePrevious5|capsensePrevious4|
 * 	|capsensePrevious3|capsensePrevious2|capsensePrevious1|capsensePrevious0|
 *
 */

#include "project_config.h"

// GPIO data structure declaration
uint16_t capsenseCurrent;
uint16_t capsensePrevious;

/******************************************************************************
 * GPIO data structure initialization
 *****************************************************************************/
void initGPIO(void)
{
  capsenseCurrent = 0;
  capsensePrevious = 0;
}

/******************************************************************************
 * update GPIO data structure current level of seven GPIO pins
 *****************************************************************************/
void updateGPIO(void)
{
  // get previous states of Cap-sense button array
  capsensePrevious = capsenseCurrent;
  
  // update current button states if a cap-sense button is pressed,
  // set its corresponding bit on capsenseCurrent to high using the mask.
  // Otherwise set to 0.
  capsenseCurrent = (CS00 ? 0 : CS00_MASK)
                    | (CS01 ? 0 : CS01_MASK)
                    | (CS02 ? 0 : CS02_MASK)
                    | (CS03 ? 0 : CS03_MASK)
                    | (CS04 ? 0 : CS04_MASK)
                    | (CS05 ? 0 : CS05_MASK)
                    | (CS06 ? 0 : CS06_MASK);
}
