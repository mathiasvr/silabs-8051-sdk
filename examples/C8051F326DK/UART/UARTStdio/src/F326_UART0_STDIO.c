//-----------------------------------------------------------------------------
// F326_UART0_STDIO.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the C8051F326 to use routines
// in STDIO.h to write to and read from the UART interface.  The program
// reads a character using the STDIO routine getkey(), outputs that character
// to the screen, and then outputs the ASCII hex value of that character.
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
// 7) Type up to 64 characters into the Terminal. The MCU  will print back the
//    character that was typed.
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
   unsigned char inputcharacter;

   PORT_Init();                        // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   UART0_Init();                       // Initialize UART0

   while (1)
   {
      printf ("\nEnter character: ");
      inputcharacter = getchar ();
      printf ("\nCharacter entered : %c",inputcharacter);
      printf ("\nValue in Hex: %bx",inputcharacter);

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
   P0MDOUT   = 0x10;                   // Enabling TX as push-pull

   GPIOCN = 0x40;                      // Enable input path
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

   SCON0_TI = 1;                       // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
