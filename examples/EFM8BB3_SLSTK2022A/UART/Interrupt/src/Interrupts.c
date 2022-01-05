//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

         
// USER INCLUDES			
#include <SI_EFM8BB3_Register_Enums.h>

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
uint8_t TX_Ready =1;
uint8_t Byte = 0;

//-----------------------------------------------------------------------------
// UART0_ISR
//-----------------------------------------------------------------------------
//
// UART0 ISR Content goes here. Remember to clear flag bits:
// SCON0::RI (Receive Interrupt Flag)
// SCON0::TI (Transmit Interrupt Flag)
//
// This routine is invoked whenever a character is entered or displayed on the
// Hyperterminal.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(UART0_ISR, UART0_IRQn)
{
	   if (SCON0_RI == 1)
	   {
	      if( UART_Buffer_Size == 0)  {    // If new word is entered
	         UART_Input_First = 0;    }

	      SCON0_RI = 0;                          // Clear interrupt flag

	      Byte = SBUF0;                    // Read a character from UART

	      if (UART_Buffer_Size < UART_BUFFERSIZE)
	      {
	         UART_Buffer[UART_Input_First] = Byte; // Store in array

	         UART_Buffer_Size++;           // Update array's size

	         UART_Input_First++;           // Update counter
	      }
	   }

	   if (SCON0_TI == 1)                        // Check if transmit flag is set
	   {
	      SCON0_TI = 0;                          // Clear interrupt flag

	      if (UART_Buffer_Size != 1)       // If buffer not empty
	      {
	         // If a new word is being output
	         if ( UART_Buffer_Size == UART_Input_First ) {
	              UART_Output_First = 0;  }

	         // Store a character in the variable byte
	         Byte = UART_Buffer[UART_Output_First];

	         if ((Byte >= 0x61) && (Byte <= 0x7A)) { // If upper case letter
	            Byte -= 32; }

	         SBUF0 = Byte;                 // Transmit to Hyperterminal

	         UART_Output_First++;          // Update counter

	         UART_Buffer_Size--;           // Decrease array size

	      }
	      else
	      {
	         UART_Buffer_Size = 0;         // Set the array size to 0
	         TX_Ready = 1;                 // Indicate transmission complete
	      }
	   }
}


