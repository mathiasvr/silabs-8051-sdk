//-----------------------------------------------------------------------------
// F326_UART0_Interrupt.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the C8051F326 to write to and
// read from the UART interface. The program reads a word using the UART
// interrupts and outputs that word to the screen, with all characters in
// uppercase.
//
//
// How To Test:
//
// 1) Ensure shorting blocks on C8051F326-TB are placed on the following:
//    J2: VDD  - +3VD
//    J3: P0.4 - TX
//    J3: P0.5 - RX
//    J7: VDD  - VIO
//    J8: Power selection, P1 9V or J4 Debugger
// 2) Ensure that the serial cable is connected to the RS232 connector
//    on the target board.
// 3) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1.
// 4) Open Hyperterminal, or a similar program, and connect to the target
//    board's serial port at a baud rate of <BAUDRATE1>.
// 5) Compile and download code to C8051F326 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 7) Type up to 64 characters into the Terminal and press Enter. The MCU
//    will then print back the characters that were typed
//
//
// Target:         C8051F326/7
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1 (JL)
//    - Updated descriptions
//    - 02 APR 2014
//
// Release 1.0
//    -Initial Revision (SM)
//    -16 JULY 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F326_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK             12000000    // Clock speed in Hz
#define BAUDRATE1          115200      // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F326
//-----------------------------------------------------------------------------
SI_SFR16(SBRL1, 0x93);

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART0_Init (void);
void PORT_Init (void);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

#define UART_BUFFERSIZE 64
unsigned char UART_Buffer[UART_BUFFERSIZE];
unsigned char UART_Buffer_Size = 0;
unsigned char UART_Input_First = 0;
unsigned char UART_Output_First = 0;
unsigned char TX_Ready =1;
static char Byte;

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
  // Disable the watchdog here
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
   PORT_Init();                        // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   UART0_Init();                       // Initialize UART0

   IE_EA = 1;

   while(1)
   {
      // If the complete word has been entered via the terminal followed by
      // carriage return
      if(TX_Ready == 1 && UART_Buffer_Size != 0 && Byte == 13)
      {
         TX_Ready = 0;                 // Set the flag to zero
         SCON0_TI = 1;                 // Set transmit flag to 1
      }
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the GPIO ports.
//
// P0.4   digital   push-pull    UART TX
// P0.5   digital   open-drain   UART RX
//
//-----------------------------------------------------------------------------

void PORT_Init (void)
{
   P0MDOUT = 0x10;                   // Enabling TX as push-pull

   GPIOCN  = 0x040;                  // Enable input path
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal oscillator
// at its maximum frequency(12 Mhz).
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
   OSCICN = 0x83;                      // Set internal oscillator to run
                                       // at its maximum frequency
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Baudrate generator, for <BAUDRATE1> and 8-N-1.
//-----------------------------------------------------------------------------

void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       // clear SCON0_RI and SCON0_TI bits

   // Baud Rate = [BRG Clock / (65536 - (SBRLH0:SBRLL0))] x 1/2 x 1/Prescaler

   if (SYSCLK/BAUDRATE1/2/0xFFFF < 1) {
      SBRL1 = -(SYSCLK/BAUDRATE1/2);
      SBCON0 |= 0x03;                  // set prescaler to 1
   } else if (SYSCLK/BAUDRATE1/2/0xFFFF < 4) {
      SBRL1 = -(SYSCLK/BAUDRATE1/2/4);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x01;                  // set prescaler to 4

   } else if (SYSCLK/BAUDRATE1/2/0xFFFF < 12) {
      SBRL1 = -(SYSCLK/BAUDRATE1/2/12);
      SBCON0 &= ~0x03;                 // set prescaler to 12
   } else {
      SBRL1 = -(SYSCLK/BAUDRATE1/2/48);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x02;                  // set prescaler to 4
   }

   SBCON0 |= 0x40;                     // enable baud rate generator
   TX_Ready = 1;                       // Flag showing that UART can transmit
   IE = 0x10;                          // Enable UART0 interrupts

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

void UART0_Interrupt (void) interrupt 4
{
   if (SCON0_RI == 1)
   {
      if( UART_Buffer_Size == 0)  {            // If new word is entered
         UART_Input_First = 0; }

      SCON0_RI = 0;                            // Clear interrupt flag

      Byte = SBUF0;                            // Read a character from UART

      if (UART_Buffer_Size < UART_BUFFERSIZE)
      {
         UART_Buffer[UART_Input_First] = Byte; // Store in array

         UART_Buffer_Size++;                   // Update array's size

         UART_Input_First++;                   // Update counter
      }
   }

   if (SCON0_TI == 1)                          // Check if transmit flag is set
   {
      SCON0_TI = 0;                            // Clear interrupt flag

      if (UART_Buffer_Size != 1)               // If buffer not empty
      {
         // If a new word is being output
         if ( UART_Buffer_Size == UART_Input_First ) {
              UART_Output_First = 0; }

         // Store a character in the variable byte
         Byte = UART_Buffer[UART_Output_First];

         if ((Byte >= 0x61) && (Byte <= 0x7A)) { // If upper case letter
            Byte -= 32; }

         SBUF0 = Byte;                           // Transmit to Hyperterminal

         UART_Output_First++;                    // Update counter

         UART_Buffer_Size--;                     // Decrease array size

      }
      else
      {
         // Set the array size to 0
         UART_Buffer_Size = 0;

         // Indicate transmission complete
         TX_Ready = 1;
      }
   }

}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
