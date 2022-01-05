//-----------------------------------------------------------------------------
// F97x_UART0_Interrupt.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to configure the C8051F97x to write to and read
// from the UART interface. The program reads a word using the UART interrupts
// and outputs that word to the screen, with all characters in uppercase
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP1: VCP RX <> P0.1/UART0.TX
//    - JP2: VCP TX <> P0.2/UART0.RX
//    - JP3:    VDD <> VMCU
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Connect a USB cable from J9 of the development board to a PC,
// 5) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the virtual COM port at <BAUDRATE> (115200) and 8-N-1.
// 6) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 7) Type up to 64 characters into the Terminal and press Enter.  The MCU
//    will then print back the characters that were typed
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 30 APR 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);


//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(UART0_ISR, INTERRUPT_UART0);


//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

#define UART_BUFFERSIZE 64
uint8_t UART_Buffer[UART_BUFFERSIZE];

uint8_t UART_Buffer_Size = 0;
uint8_t UART_Input_First = 0;
uint8_t UART_Output_First = 0;
uint8_t TX_Ready = 1;
static uint8_t Byte;


//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
  SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
  PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
   enter_DefaultMode_from_RESET();

   while(1)
   {
      // If the complete word has been entered via the terminal followed by
      // carriage return

      if(TX_Ready == 1 && UART_Buffer_Size != 0 && (Byte == '\r' || Byte == '\n'))
      {
         TX_Ready = 0;                 // Set the flag to zero

         SFRPAGE = UART0_PAGE;
         SCON0 |= SCON0_TI__SET;       // Indicate TX0 ready
      }
   }
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// UART0_Interrupt
//-----------------------------------------------------------------------------
//
// This routine is invoked whenever a character is entered or displayed on the
// Hyperterminal.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(UART0_ISR, UART0_IRQn)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = UART0_PAGE;

   if ((SCON0 & SCON0_RI__BMASK) == SCON0_RI__SET)
   {

      if( UART_Buffer_Size == 0)
      {  // If new word is entered
         UART_Input_First = 0;
      }

      SCON0 &= ~SCON0_RI__BMASK;         // Clear interrupt flag

      Byte = SBUF0;                      // Read a character from UART

      if (UART_Buffer_Size < UART_BUFFERSIZE)
      {
         UART_Buffer[UART_Input_First] = Byte; // Store in array

         UART_Buffer_Size++;                   // Update array's size

         UART_Input_First++;                   // Update counter
      }
   }

   if ((SCON0 & SCON0_TI__BMASK) == SCON0_TI__SET) // Check if transmit flag is set
   {

      SCON0 &= ~SCON0_TI__BMASK;                   // Clear interrupt flag

      if (UART_Buffer_Size != 1)                   // If buffer not empty
      {
         // If a new word is being output
         if ( UART_Buffer_Size == UART_Input_First )
         {
              UART_Output_First = 0;
         }

         // Store a character in the variable byte
         Byte = UART_Buffer[UART_Output_First];

         if ((Byte >= 0x61) && (Byte <= 0x7A))
         {  // If upper case letter
            Byte -= 32;
         }

         SBUF0 = Byte;                   // Transmit to Hyperterminal

         UART_Output_First++;            // Update counter

         UART_Buffer_Size--;             // Decrease array size

      }
      else
      {
         UART_Buffer_Size = 0;           // Set the array size to 0
         TX_Ready = 1;                   // Indicate transmission complete
      }
   }

   SFRPAGE = SFRPAGE_save;               // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
