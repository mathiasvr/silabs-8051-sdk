//-----------------------------------------------------------------------------
// F580_UART0_Interrupt.c
//-----------------------------------------------------------------------------
// Copyright 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates how to configure the C8051F580 to write to and 
// read from the UART interface. The program reads a word using the UART 
// interrupts and outputs that word to the screen, with all characters in 
// uppercase.
//
// How To Test:
//
// 1) Ensure that jumpers are placed on J17 of the C8051F580 target board
//    that connect the P0.4 pin to the TX signal, and P0.5 to the RX signal
// 2) Connect USB cable from the development board to a PC
// 3) Specify the target baudrate in the constant <BAUDRATE>
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the USB port (virtual com port) at <BAUDRATE>, 8 data bits, no parity,
//    1 stop bit and no flow control. Enable the "Append CR to transmission",
//    or manually append a carriage return when done typing.
// 5) Download and execute code on an 'F580 target board
// 6) Type up to 64 characters into the Terminal and press Enter. The MCU
//    will then print back the characters that were typed
//
// Target:         C8051F580 (C8051F580-TB)
// Tool chain:     Keil C51 8.0 / Keil EVAL C51
// Command Line:   None
//
// Release 1.1 / 10 JUL 2015 (SG)
//    - Revised for Simplicity Studio
//
// Release 1.0 / 21 JUL 2008 (ADT)
//    - Initial Revision
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F580_Register_Enums.h>         // SFR declarations
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK           24000000      // SYSCLK frequency in Hz
#define BAUDRATE           115200      // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART0_Init (void);
void PORT_Init (void);

SI_INTERRUPT_PROTO(UART0_ISR, UART0_IRQn);

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
   PCA0MD &= ~0x40;                    // Disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SFRPAGE = ACTIVE1_PAGE;             // Change for PCA0MD


   PORT_Init ();                       // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   UART0_Init ();                      // Initialize UART

   IE_EA = 1;                          // Enable global interrutpts

   while (1)
   {
      // If the complete word has been entered via the terminal followed by
      // carriage return

      if((TX_Ready == 1) && (UART_Buffer_Size != 0) && (Byte == 13))
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
// Configure the Crossbar and GPIO ports.
//
// P0.4   digital   push-pull    UART TX
// P0.5   digital   open-drain   UART RX
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE; 

   P0MDOUT |= 0x10;                    // Enable UTX as push-pull output
   XBR0 = 0x01;                        // Enable UART on P0.4(TX) and P0.5(RX)
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal oscillator
// at its maximum frequency.
// Also enables the Missing Clock Detector.
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE; 

   OSCICN |= 0x87;                     // Configure internal oscillator for
                                       // its maximum frequency
   RSTSRC = 0x04;                      // Enable missing clock detector

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE; 

 // Baud Rate = [BRG Clock / (65536 - (SBRLH0:SBRLL0))] x 1/2 x 1/Prescaler

#if   ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 1)
      SBRL0 = -(SYSCLK / BAUDRATE / 2);
      SBCON0 |= 0x03;                  // Set prescaler to 1
#elif ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 4)
      SBRL0 = -(SYSCLK / BAUDRATE / 2/ 4);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x01;                  // Set prescaler to 4
#elif ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 12)
      SBRL0 = -(SYSCLK / BAUDRATE / 2 / 12);
      SBCON0 &= ~0x03;                 // Set prescaler to 12
#else
      SBRL0 = -(SYSCLK / BAUDRATE / 2 / 48);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x02;                  // Set prescaler to 48
#endif

   SBCON0 |= 0x40;                     // Enable baud rate generator
   IE_ES0 = 1;                         // Enable UART0 interrupts

   SFRPAGE = ACTIVE1_PAGE;   

   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       // clear SCON0_RI and SCON0_TI bits

   TX_Ready = 1;                       // Flag showing that UART can transmit

   SFRPAGE = SFRPAGE_save;
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
      if (UART_Buffer_Size == 0)       // If new word is entered
      {
         UART_Input_First = 0;
      }

      SCON0_RI = 0;                    // Clear interrupt flag

      Byte = SBUF0;                    // Read a character from UART

      if (UART_Buffer_Size < UART_BUFFERSIZE)
      {
         UART_Buffer[UART_Input_First] = Byte; // Store in array

         UART_Buffer_Size++;           // Update array's size

         UART_Input_First++;           // Update counter
      }
   }

   if (SCON0_TI == 1)                  // Check if transmit flag is set
   {
      SCON0_TI = 0;                    // Clear interrupt flag

      if (UART_Buffer_Size != 1)       // If buffer not empty
      {
         // If a new word is being output
         if (UART_Buffer_Size == UART_Input_First) 
         {
              UART_Output_First = 0;  
         }

         // Store a character in the variable byte
         Byte = UART_Buffer[UART_Output_First];

         if ((Byte >= 0x61) && (Byte <= 0x7A)) // If upper case letter
         { 
            Byte -= 32; 
         }

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
