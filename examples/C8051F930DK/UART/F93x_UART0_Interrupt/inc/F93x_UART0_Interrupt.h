/*
 * F93x_UART0_Interrupt.h
 *
 *  Created on: Jun 4, 2014
 *      Author: jiguo1
 */

#ifndef F93X_UART0_INTERRUPT_MAIN_H_
#define F93X_UART0_INTERRUPT_MAIN_H_

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

#define UART_BUFFERSIZE 64
extern uint8_t UART_Buffer[UART_BUFFERSIZE];
extern uint8_t UART_Buffer_Size;
extern uint8_t UART_Input_First;
extern uint8_t UART_Output_First;
extern uint8_t TX_Ready;
extern uint8_t Byte;

#endif /* F93X_UART0_INTERRUPT_MAIN_H_ */
