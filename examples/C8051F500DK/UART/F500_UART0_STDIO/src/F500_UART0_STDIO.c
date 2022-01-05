//-----------------------------------------------------------------------------
// F500_UART0_STDIO.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to configure the C8051F500 to use routines
// in STDIO.h to write to and read from the UART interface. The program
// reads a character using the STDIO routine getchar(), outputs that character
// to the screen, and then outputs the ASCII hex value of that character.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F500-TB are placed on the following:
//    J17: P0.4 - UART_TX
//    J17: P0.5 - UART_RX
//    J7: Power selection, P4 9V (+5VREG) or P2 Debugger (SER_PWR)
// 2) Connect the USB Debug Adapter to P2. If +5VREG is selected on J7,
//    connect the 9V power adapter to P2.
// 3) Connect a USB cable from the USB port P5 on development board to a PC.
//    Specify the target baudrate in the constant <BAUDRATE>
// 4) On the PC, open Hyperterminal, or a similar program, and connect to the
//    target board's serial port using <BAUDRATE> and 8-N-1 (CP210x USB-to-UART
//    VCP drivers must be installed).
// 5) Compile and download code to C8051F500 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 7) Type a character in your terminal program.
//
//
// Target:         C8051F50x/1x (Side A of a C8051F500-TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.2 / 11 AUG 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.1 / 02 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0 / 10 MAR 2008 (GP)
//    -Initial Revision


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F500_Register_Enums.h>
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
void UART0_Init (void);
void PORT_Init (void);

#ifdef SDCC
 uint8_t getchar (void);
 void putchar (int8_t output);
#endif

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
   uint8_t inputcharacter;                  // Used to store character from UART

   SFRPAGE = LEGACY_PAGE;              // Change for PCA0MD and SBUF0

   PORT_Init ();                       // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   UART0_Init ();

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
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x10;                    // Enable UTX as push-pull output
   XBR0    = XBR0_URT0E__ENABLED;      // Enable UART on P0.4(TX) and P0.5(RX)
   // Enable crossbar and weak pull-ups
   XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED |
          XBR2_XBARE__ENABLED;

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

   // Configure internal oscillator for its maximum frequency
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;

   SFRPAGE = LEGACY_PAGE;
   RSTSRC  = 0x04;                     // Enable missing clock detector

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

   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       // clear RI0 and TI0 bits

   // Baud Rate = [BRG Clock / (65536 - (SBRLH0:SBRLL0))] x 1/2 x 1/Prescaler

#if   ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 1)
      SBRL0 = -(SYSCLK / BAUDRATE / 2);
      SBCON0 |= 0x03;                  // Set prescaler to 1
#elif ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 4)
      SBRL0 = -(SYSCLK / BAUDRATE / 2 / 4);
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

   SBCON0 |= SBCON0_SBRUN__ENABLED;    // Enable baud rate generator
   SCON0_TI = 1;                       // Indicate TX0 ready

   SFRPAGE = SFRPAGE_save;
}

#ifdef SDCC

// SDCC does not include a definition for putchar(), which is used in printf()
// and so it is defined here.  The prototype does not need to be explicitly
// defined because it is provided in stdio.h

//-----------------------------------------------------------------------------
// putchar
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : character to send to UART
//
// This function outputs a character to the UART.
//-----------------------------------------------------------------------------
void putchar (int8_t output)
{
   if (output == '\n')
   {
      while (!SCON0_TI);
      SCON0_TI = 0;
      SBUF0 = 0x0D;
   }
   while (!SCON0_TI);
   SCON0_TI = 0;
   SBUF0 = output;
}

// SDCC does not include a definition for getchar(), and so it is defined here

//-----------------------------------------------------------------------------
// getchar
//-----------------------------------------------------------------------------
//
// Return Value : character received from UART
// Parameters   : None
//
// This function returns a character received from the UART
// Assumes SFR page has been set correctly by the application
//-----------------------------------------------------------------------------
uint8_t getchar (void)
{
   uint8_t received;

   while (!SCON0_RI);                  // Wait for character to be received
   received = SBUF0;                   // Read it
   SCON0_RI = 0;                       // Clear received data indicator

   return received;
}

#endif

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------