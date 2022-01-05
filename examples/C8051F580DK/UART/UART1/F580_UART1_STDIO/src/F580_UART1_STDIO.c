//-----------------------------------------------------------------------------
// F580_UART1_STDIO.c
//-----------------------------------------------------------------------------
// Copyright 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates how to configure the C8051F580 to use routines
// in STDIO.h to write to and read from the UART interface.  The program
// reads a character using the STDIO routine getkey(), outputs that character
// to the screen, and then outputs the ASCII hex value of that character.
//
//
// How To Test:
//
// 1) Ensure that jumpers are placed on J? of the C8051F580 target board
//    that connect P0.4 to the TX signal, and P0.5 to the RX field.
// 2) Connect USB cable from the development board to a PC
// 3) Specify the target baudrate in the constant <BAUDRATE>. Disable any
//    option like "Append LF to transmission" or "Append CR to transmission",
//    if present.
// 4) Open Hyperterminal, or a similar program, and connect to the target
//    board's USB port (virtual com port).
// 5) Download and execute code to an 'F580 target board.
//
//
// Target:         C8051F580
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

#define SYSCLK      24000000           // SYSCLK frequency in Hz
#define BAUDRATE      115200           // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART1_Init (void);
void PORT_Init (void);

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
   uint8_t inputcharacter;                  // Used to store character from UART

   SFRPAGE = ACTIVE1_PAGE;

   PORT_Init();                        // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   UART1_Init();

   SFRPAGE = ACTIVE2_PAGE;             // Switch page for UART1 communication

   while (1)
   {
      printf ("\nEnter character: ");
      inputcharacter = getkey ();
      printf ("\nCharacter entered : %c",inputcharacter);
      printf ("\n      Value in Hex: %bx",inputcharacter);
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
                                       //        clear SCON0_RI and SCON0_TI bits
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

   SCON1_TI = 1;                       // Indicate TX0 ready (SCON1)

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
