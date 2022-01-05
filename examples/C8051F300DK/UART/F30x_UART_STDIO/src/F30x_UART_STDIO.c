//-----------------------------------------------------------------------------
// F30x_UART_STDIO.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the C8051F300 to use routines
// in STDIO.h to write to and read from the UART interface.  The program
// reads a character using the STDIO routine getkey(), outputs that character
// to the screen, and then outputs the ASCII hex value of that character.
//
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J3: P0.4 - TX
//          P0.5 - RX
// 2) Connect the USB Debug Adapter to J4.
// 3) Connect the 9V power adapter to P1.
// 4) Connect a serial cable from the RS232 connector on the target board to a 
//    PC.
// 5) Specify the target baudrate in the constant <BAUDRATE>.
// 6) Compile and download code to a 'F300 device on a C8051F300-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) Open Hyperterminal, or a similar program, and connect to the target
//    board's serial port. 
// 9) Follow the prompts on the terminal.
//   
//
// Target:         C8051F300
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.2 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 code with compiler agnostic code
//    - 10 JAN 2014
//
// Release 1.1
//    -Corrected errors in UART setup routine. (BD)
//    -02 AUG 2007
//
// Release 1.0
//    -Initial Revision (PD)
//    -16 AUG 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F300_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK      24500000           // SYSCLK frequency in Hz
#define BAUDRATE        9600           // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART0_Init (void);
void PORT_Init (void);
void Timer2_Init (int16_t);

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
   uint8_t inputcharacter;       // Used to store character from UART

                                       // enable)
   PORT_Init();                        // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   UART0_Init();

   while (1)
   {
      printf ("\nEnter character: ");
      inputcharacter = getchar ();
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
// P0.4   digital   push-pull    UART TX
// P0.5   digital   open-drain   UART RX
// 
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P0MDOUT |= 0x10;                    // set UART TX to push-pull output              
   XBR1    = 0x03;                     // Enable UTX, URX as push-pull output
   XBR2    = 0x40;                     // Enable crossbar, weak pull-ups 
                                       // disabled
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
   OSCICN |= 0x03;                     // Configure internal oscillator for
                                       // its maximum frequency
   RSTSRC  = 0x04;                     // Enable missing clock detector
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
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear SCON0_RI and SCON0_TI bits
   if (SYSCLK/BAUDRATE/2/256 < 1) {
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON |= 0x10;                   // T1M = 1; SCA1:0 = xx
   } else if (SYSCLK/BAUDRATE/2/256 < 4) {
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON |=  0x01;                  // T1M = 0; SCA1:0 = 01
      CKCON &= ~0x12;
   } else if (SYSCLK/BAUDRATE/2/256 < 12) {
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x13;                  // T1M = 0; SCA1:0 = 00
   } else {
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON |=  0x02;                  // T1M = 0; SCA1:0 = 10
      CKCON &= ~0x11;
   }

   TL1 = 0xff;                         // set Timer1 to overflow immediately
   TMOD |= 0x20;                       // TMOD: timer 1 in 8-bit autoreload
   TMOD &= ~0xD0;                      // mode
   TCON_TR1 = 1;                            // START Timer1
   SCON0_TI = 1;                            // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------