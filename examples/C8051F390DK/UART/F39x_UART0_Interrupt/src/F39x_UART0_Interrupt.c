//-----------------------------------------------------------------------------
// F39x_UART0_Interrupt.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the C8051F390 to write to and read
// from the UART interface. The program reads a word using the UART interrupts
// and outputs that word to the screen, with all characters in uppercase
//
// How To Test:
//
// 1) Ensure that jumpers are placed on J14 of the C8051F390/F370 UDP MCU
//    board that connect P0.4 to the MCU_TX, and P0.5 to the MCU_RX.
// 2) Connect USB cable from the development board to a PC
// 3) Specify the target baudrate in the constant <BAUDRATE>.
// 4) Open Hyperterminal, or a similar program, and connect to the target
//    board's USB port (virtual com port).
// 5) Download and execute code to an 'F390/F370 target board.
// 6) Type up to 64 characters into the Terminal and press Enter. The MCU
//    will then print back the characters that were typed.
//
//
// Target:         C8051F39x/37x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.1 (JL)
//    - Updated description
//    - 25 JAN 2014
//
// Release 1.0
//    - Initial Revision (ST)
//    - 16 JAN 2012
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F390_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSTEMCLOCK    49000000/2      // SYSCLK frequency in Hz
#define BAUDRATE             9600      // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART0_Init (void);
void PORT_Init (void);
void Timer2_Init (int);
SI_INTERRUPT_PROTO(UART0_ISR, UART0_IRQn);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

#define UART_BUFFERSIZE 64
uint8_t UART_Buffer[UART_BUFFERSIZE];
uint8_t UART_Buffer_Size = 0;
uint8_t UART_Input_First = 0;
uint8_t UART_Output_First = 0;
uint8_t TX_Ready =1;
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
   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
                                       // enable)
   PORT_Init();                        // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   UART0_Init();

   IE_EA = 1;

   while(1)
   {
      // If the complete word has been entered via the terminal followed by
      // carriage return

      if(TX_Ready == 1 && UART_Buffer_Size != 0 && Byte == 13)
      {
         TX_Ready = 0;                 // Set the flag to zero
         SCON0_TI = 1;                      // Set transmit flag to 1
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
// Configure the Crossbar and GPIO ports.
//
// P0.4   digital   push-pull    UART TX
// P0.5   digital   open-drain   UART RX
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P0MDOUT |= 0x10;                    // Enable UTX as push-pull output
   XBR0    = 0x01;                     // Enable UART on P0.4(TX) and P0.5(RX)
   XBR1    = 0x40;                     // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 49 MHz
// oscillator divided by 2 as its clock source.  Also enables missing clock 
// detector reset.
//
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
   OSCICN = 0x83;                     // Set system clock to 24.5 MHz
   RSTSRC = 0x04;                     // Enable missing clock detector
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear SCON0_RI and SCON0_TI bits
   if (SYSTEMCLOCK/BAUDRATE/2/256 < 1) {
      TH1 = -(SYSTEMCLOCK/BAUDRATE/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   } else if (SYSTEMCLOCK/BAUDRATE/2/256 < 4) {
      TH1 = -(SYSTEMCLOCK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x01;
   } else if (SYSTEMCLOCK/BAUDRATE/2/256 < 12) {
      TH1 = -(SYSTEMCLOCK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   } else {
      TH1 = -(SYSTEMCLOCK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   }

   TL1 = TH1;                          // init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                            // START Timer1
   TX_Ready = 1;                       // Flag showing that UART can transmit
   IP |= 0x10;                         // Make UART high priority
   IE_ES0 = 1;                            // Enable UART0 interrupts
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
   if (SCON0_RI == 1)
   {
      if( UART_Buffer_Size == 0)  {    // If new word is entered
         UART_Input_First = 0;    }

      SCON0_RI = 0;                         // Clear interrupt flag

      Byte = SBUF0;                    // Read a character from UART

      if (UART_Buffer_Size < UART_BUFFERSIZE)
      {
         UART_Buffer[UART_Input_First] = Byte; // Store in array

         UART_Buffer_Size++;           // Update array's size

         UART_Input_First++;           // Update counter
      }
   }

   if (SCON0_TI == 1)                       // Check if transmit flag is set
   {
      SCON0_TI = 0;                         // Clear interrupt flag

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

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
