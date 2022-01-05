//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8UB1_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
#define UART_BUFFERSIZE        64

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
uint8_t UART_Buffer[UART_BUFFERSIZE];
uint8_t UART_Buffer_Size = 0;
uint8_t UART_Input_First = 0;
uint8_t UART_Output_First = 0;
uint8_t TX_Ready = 1;
uint8_t Byte = 0;
SI_SBIT (S1, SFR_P0, 2);
SI_SBIT (S2, SFR_P0, 3);
uint8_t TX_Char = 61;

//-----------------------------------------------------------------------------
// UART1_ISR
//-----------------------------------------------------------------------------
//
// UART1 ISR Content goes here. Remember to clear flag bits:
// SCON1::RI (Receive Interrupt Flag)
// SCON1::TI (Transmit Interrupt Flag)
// UART1FCN1::RFRQ (Receive FIFO Request)
// UART1FCN1::TFRQ (Transmit FIFO Request)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(UART1_ISR, UART1_IRQn)
{
	if (SCON1_RI == 1)
	{
		if( UART_Buffer_Size == 0) {    // If new word is entered
			UART_Input_First = 0;}

		SCON1_RI = 0;                          // Clear interrupt flag

		Byte = SBUF1;// Read a character from UART

		if (UART_Buffer_Size < UART_BUFFERSIZE)
		{
			UART_Buffer[UART_Input_First] = Byte; // Store in array

			UART_Buffer_Size++;// Update array's size

			UART_Input_First++;// Update counter
		}
	}
	if(UART_Buffer_Size != 0)
	{	SCON1_TI = 1;}
	if (SCON1_TI == 1)                      // Check if transmit flag is set
	{
		SCON1_TI = 0;                          // Clear interrupt flag

		if (UART_Buffer_Size != 0)// If buffer not empty
		{
			// If a new word is being output
			if ( UART_Buffer_Size == UART_Input_First ) {
				UART_Output_First = 0;}

			// Store a character in the variable byte
			Byte = UART_Buffer[UART_Output_First];

			if ((Byte >= 0x61) && (Byte <= 0x7A))// If lower case letter
			{
				Byte -= 32; //convert to upper case
			}

			SBUF1 = Byte;                 // Transmit to Hyperterminal

			UART_Output_First++;// Update counter

			UART_Buffer_Size--;// Decrease array size

		}
		else
		{
			UART_Buffer_Size = 0;         // Set the array size to 0
			TX_Ready = 1;// Indicate transmission complete
		}
	}
}

