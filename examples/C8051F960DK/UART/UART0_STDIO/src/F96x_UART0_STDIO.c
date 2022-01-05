//-----------------------------------------------------------------------------
// F96x_UART0_STDIO.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the C8051F960 to use routines
// in STDIO.h to write to and read from the UART interface.  The program
// reads a character using the STDIO routine getkey(), outputs that character
// to the screen, and then outputs the ASCII hex value of that character.
//
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       P2 (LCD) or P12 (EMIF):
//             P0.4/MCU_TX
//             P0.5/MCU_RX
// 2) Connect a USB cable from the development board J17 to a PC
// 3) Connect the USB Debug Adapter to J13.
// 4) Move the VBAT SEL switch (SW5) to the middle VREG position.
// 5) Move the VIO switch (SW7) and VIORF switch (SW12) to the VBAT position.
// 6) Connect a 9V power adapter to P1 of the development board.
// 7) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the USB port (virtual com port) at <BAUDRATE> and 8-N-1
// 10) Follow the prompts in the terminal.
//
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 20 JAN 2014
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F960_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK      24500000           // SYSCLK frequency in Hz
#define BAUDRATE      115200           // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART0_Init (void);
void PORT_Init (void);

//-----------------------------------------------------------------------------
// Generic UART definitions to support both Keil and SDCC
//-----------------------------------------------------------------------------

#ifdef SDCC
#include <sdcc_stdio.h>
#endif

char *GETS (char *, uint16_t);

#ifdef __C51__
#define GETS gets
#endif

#ifdef SDCC
#include <sdcc_stdio.c>
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
   UART0_Init();                       // Initialize UART0

   while (1)
   {
      printf ("\nEnter character: ");
      inputcharacter = getchar();
      printf ("\nCharacter entered : %c", inputcharacter);
      printf ("\n      Value in Hex: %x\n",  (int16_t) inputcharacter);
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
   SFRPAGE = LEGACY_PAGE;

   P0MDOUT |= 0x10;                    // Enable UTX as push-pull output
   XBR0    = 0x01;                     // Enable UART on P0.4(TX) and P0.5(RX)
   XBR2    = 0x40;                     // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal precision
// oscillator at its maximum frequency and enables the missing clock
// detector.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   SFRPAGE = CONFIG_PAGE;
   FLSCL     = 0x40;                   // Set BYPASS bit for >12.5 MHz.

   SFRPAGE = LEGACY_PAGE;
   REG0CN |= 0x10;                     // Enable the precision osc. bias
   OSCICN |= 0x80;                     // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock
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
   SFRPAGE = LEGACY_PAGE;

   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear SCON0_RI and SCON0_TI bits
   #if (SYSCLK/BAUDRATE/2/256 < 1)
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   #elif (SYSCLK/BAUDRATE/2/256 < 4)
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x01;
   #elif (SYSCLK/BAUDRATE/2/256 < 12)
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   #else
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   #endif

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                            // START Timer1
   SCON0_TI = 1;                            // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------