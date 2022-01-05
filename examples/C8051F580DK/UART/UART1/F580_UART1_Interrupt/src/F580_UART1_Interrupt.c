//-----------------------------------------------------------------------------
// F580_UART1_Interrupt.c
//-----------------------------------------------------------------------------
// Copyright 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates how to configure the C8051F580 to write and read
// from the UART1 interface. The program reads a word using the UART interrupts
// and outputs that word to the screen, with all characters in uppercase
//
// How To Test:
//
// 1) Ensure that jumpers are placed on J17 of the C8051F580 target board
//    that connect the P0.4 pin to the TX signal, and P0.5 to the RX signal
// 2) Connect USB cable from the development board to a PC
// 3) Specify the target baudrate in the constant <BAUDRATE> Enable the
//    "Append CR to transmission", or manually send a carriage return when
//    done typing.
// 4) Open Hyperterminal, or a similar program, and connect to the target
//    board's USB port (virtual com port)
// 5) Download and execute code on an 'F580 target board
// 6) Type up to 64 characters into the Terminal and press Enter. The MCU
//    will then print back the characters that were typed
//
// Target:         C8051F580
// Tool chain:     Keil C51 8.0 / Keil EVAL C51
// Command Line:   None
//
// Release 1.1 / 10 JUL 2015 (SG)
//    - Revised for Simplicity Studio
//
// Release 1.0 / 21 JUL 2008
//    - Initial Revision (ADT)
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
void UART1_Init (void);
void PORT_Init (void);
SI_INTERRUPT_PROTO(UART1_ISR, UART1_IRQn);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

#define UART1_BUFFERSIZE 64
uint8_t UART1_Buffer[UART1_BUFFERSIZE];
uint8_t UART1_Buffer_Size = 0;
uint8_t UART1_Input_First = 0;
uint8_t UART1_Output_First = 0;
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
   SFRPAGE = ACTIVE1_PAGE;

   PORT_Init();                        // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   UART1_Init();

   SFRPAGE = ACTIVE2_PAGE;

   IE_EA = 1;

   while(1)
   {
      // If the complete word has been entered via the terminal followed by
      // carriage return

      if(TX_Ready == 1 && UART1_Buffer_Size != 0 && Byte == 13)
      {
         TX_Ready = 0;                 // Set the flag to zero
         SCON1_TI = 1;                 // Set transmit flag to 1
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
// P0.4   digital   push-pull    UART1 TX
// P0.5   digital   open-drain   UART1 RX
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x10;                    // Enable UART1 TX as push-pull output
   P0SKIP = 0x0F;                      // Skip to P0.4

   XBR2 = 0x42;                        // Enable UART1 on P0.4(TX) and P0.5(RX)
                                       // Enable crossbar and weak pull-ups
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
// UART1_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART1 using Timer1, for <BAUDRATE> and 8-N-1.
//-----------------------------------------------------------------------------
void UART1_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE2_PAGE;

   SCON1 = 0x10;                       // SCON1: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear SCON1_RI and SCON1_TI bits
#if   ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 1)
      TH1 = -(SYSCLK / BAUDRATE / 2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |= 0x08;
#elif ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 4)
      TH1 = -(SYSCLK / BAUDRATE / 2 / 4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |= 0x01;
#elif ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 12)
      TH1 = -(SYSCLK / BAUDRATE / 2 / 12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
#else
      TH1 = -(SYSCLK / BAUDRATE / 2 / 48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |= 0x02;
#endif

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xF0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |= 0x20;
   TCON_TR1 = 1;                       // START Timer1

   EIE2 |= 0x08;                       // Enable UART1 interrupts

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// UART1_Interrupt
//-----------------------------------------------------------------------------
//
// This routine is invoked whenever a character is entered or displayed on the
// Hyperterminal.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(UART1_ISR, UART1_IRQn)
{
   if (SCON1_RI == 1)
   {
      if( UART1_Buffer_Size == 0)      // If new word is entered
      {
         UART1_Input_First = 0;
      }

      SCON1_RI = 0;                    // Clear interrupt flag

      Byte = SBUF1;                    // Read a character from UART

      if (UART1_Buffer_Size < UART1_BUFFERSIZE)
      {
         UART1_Buffer[UART1_Input_First] = Byte; // Store in array

         UART1_Buffer_Size++;          // Update array's size

         UART1_Input_First++;          // Update counter
      }
   }

   if (SCON1_TI == 1)                  // Check if transmit flag is set
   {
      SCON1_TI = 0;                    // Clear interrupt flag

      if (UART1_Buffer_Size != 1)      // If buffer not empty
      {
         // If a new word is being output
         if ( UART1_Buffer_Size == UART1_Input_First ) {
              UART1_Output_First = 0;  }

         // Store a character in the variable byte
         Byte = UART1_Buffer[UART1_Output_First];

         if ((Byte >= 0x61) && (Byte <= 0x7A)) { // If upper case letter
            Byte -= 32; }

         SBUF1 = Byte;                 // Transmit to Hyperterminal

         UART1_Output_First++;         // Update counter

         UART1_Buffer_Size--;          // Decrease array size

      }
      else
      {
         UART1_Buffer_Size = 0;        // Set the array size to 0
         TX_Ready = 1;                 // Indicate transmission complete
      }
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
