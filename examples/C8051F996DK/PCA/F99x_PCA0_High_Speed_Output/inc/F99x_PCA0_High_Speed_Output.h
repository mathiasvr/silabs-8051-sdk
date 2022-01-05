/*
 * F99x_PCA0_High_Speed_Output.h
 *
 *  Created on: Jun 3, 2014
 *      Author: jiguo1
 */

#ifndef F99X_PCA0_HIGH_SPEED_OUTPUT_MAIN_H_
#define F99X_PCA0_HIGH_SPEED_OUTPUT_MAIN_H_

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24500000L          // Internal oscillator frequency in Hz

#define CEX0_FREQUENCY 10000L           // Frequency to output in Hz

#define DIVIDE_RATIO (SYSCLK/CEX0_FREQUENCY/2) // SYSCLK cycles per interrupt

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

extern uint16_t Next_Compare_Value;       // Next edge to be sent out in HSO mode


#endif /* F99X_PCA0_HIGH_SPEED_OUTPUT_MAIN_H_ */
