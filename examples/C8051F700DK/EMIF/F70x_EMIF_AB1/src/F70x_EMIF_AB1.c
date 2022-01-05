//-----------------------------------------------------------------------------
// F70x_EMIF_AB1.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program configures the external memory interface to read and write
// to an external SRAM (AB1).
//
// Pinout:
//
//    P0.4 - UART1 TX
//    P0.5 - UART1 RX
//
//    P4.4 - RAM_CS (RAM Chip Select)
//    P4.5 - RAM_BANK (RAM Bank Select)
//    P4.6 - /RD (EMIF)
//    P4.7 - /WR (EMIF)
//
//    P3, P4, P5, P6 used for EMIF
//
//    all other port pins unused
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J12: P0.4 - RXD
//           P0.5 - TXD
//	  - J15: VDD - power source
// 2) Connect the USB Debug Adapter to P3.
// 3) Connect the USB cable to P4.
// 4) Connect the AB1 board to the Co8051F70x development board and ensure
//    the jumper is in place on the AB1.
// 5) Specify the target baudrate in the constant <BAUDRATE>.
// 6) Open Hyperterminal, or a similar program, and connect to the target
//    board's serial port.
// 7) Compile and download code to the C8051F70x development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) Verify the UART displays the correct results.
//
// Target:         C8051F70x
// Tool chain:     Simplicity Studio / Keil C51 9.53
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (TP)
//    -16 OCT 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "si_toolchain.h"             // compiler declarations
#include <SI_C8051F700_defs.h>         // SFR declarations
#include <stdio.h>
#include <intrins.h>
//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F70x
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // SYSCLK frequency in Hz
#define BAUDRATE     230400            // Baud rate of UART in bps

#define RAM_SIZE     1024

SI_SBIT(LED,    SFR_P1, 0);              // '0' means ON, '1' means OFF

//-----------------------------------------------------------------------------
// Global Macros
//-----------------------------------------------------------------------------
#define ASSERT_CS()   P6 &= ~0x08
#define DEASSERT_CS() P6 |= 0x08

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
bit TX_Ready;

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void UART0_Init (void);
void EMIF_Init (void);

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

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
   WDTCN = 0xDE;                       // Disable Watchdog timer
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void) 
{
   // Memory access pointer
   SI_VARIABLE_SEGMENT_POINTER(pchar, uint8_t, xdata);
   unsigned char byte;

   uint32_t i;


   PORT_Init();                        // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   UART0_Init ();                      // Initialize UART0
   EMIF_Init ();                       // Initialize memory interface

   SFRPAGE = CONFIG_PAGE;              // P4 is on SFR Page F

   // RAM_BANK = 0;                    // Select lower bank
   ASSERT_CS();

   SFRPAGE = LEGACY_PAGE;

   // Clear xdata space
   pchar = 0x4000;
   for (i = 0; i < RAM_SIZE; i++) 
   {
      *pchar = 0;
      pchar++;
      // Print status to UART0
      if ((i % 16) == 0) 
      {
         printf ("\nwriting 0x%04x: %02x ", (unsigned) i, (unsigned) 0);
      } else 
      {
         printf ("%02x ", (unsigned) 0);
      }
   }

   // Verify all are zero
   pchar = 0x4000;
   for (i = 0; i < RAM_SIZE; i++) 
   {
      byte = *pchar;
         if (byte != 0) {
            printf ("Erase error!\n");
            while (1);
         }
         // Print status to UART0
         if ((i % 16) == 0) 
         {
            printf ("\nverifying 0x%04x: %02x ", (unsigned) i, (unsigned) *pchar);
         } else {
            printf ("%02x ", (unsigned) *pchar);
         }
      pchar++;
   }

   // Write xdata space
   pchar = 0x4000;
   for (i = 0; i < RAM_SIZE; i++) 
   {
      *pchar = ~i;
      // Print status to UART0
      if ((i % 16) == 0) {
         printf ("\nwriting 0x%04x: %02x ", (unsigned) i, (unsigned) ((~i) & 0xff));
      } else {
         printf ("%02x ", (unsigned) ((~i) & 0xff));
      }
      pchar++;
   }

   // Verify
   pchar = 0x4000;
   for (i = 0; i < RAM_SIZE; i++) 
   {
      if (*pchar != ((~i) & 0xff)) 
      {
         printf ("Verify error!\n");
      }
      // Print status to UART0
      if ((i % 16) == 0) 
      {
         printf ("\nverifying 0x%04x: %02x ", (unsigned) i, (unsigned) *pchar);
      } else 
      {
         printf ("%02x ", (unsigned) *pchar);
      }
      pchar++;
   }

  printf ("\n\nTest complete.");

  while (1);
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
//    P0.0 - UART1 TX (push-pull)
//    P0.1 - UART1 RX
//
//    P6.0 - /WR (EMIF) (push-pull)
//    P6.1 - /RD (EMIF) (push-pull)
//    P6.2 - RAM_ALE (RAM Chip Select) (push-pull)
//    P6.3 - RAM_CS (RAM Chip Select) (push-pull)
//
//    P3, P4, P5 used for EMIF (push-pull)
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   XBR0    = 0x01;                     // Enable UART1 on P0.4(TX) and P0.5(RX)
   XBR1    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups

   P0MDOUT |= 0x10;                    // Enable UTX as push-pull output
   P1MDOUT |= 0x01;                    // Enable LED as push-pull output

   // EMIF Initializations
   P6MDOUT |= 0x3F;                    // ALE, /WR, /RD, CS are push-pull
   P5MDOUT |= 0xFF;
   P4MDOUT |= 0xFF;
   P3MDOUT |= 0xFF;

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
//
// Also enables the Missing Clock Detector and VDD monitor as reset sources.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   OSCICN = 0x83;                      // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock
   OSCICN |= 0x80;                     // Enable the precision internal osc.

      RSTSRC = 0x06;                      // Enable missing clock detector and
                                          // leave VDD Monitor enabled.

      CLKSEL = 0x00;                      // Select precision internal osc.
                                          // divided by 1 as the system clock

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// UART1_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART1 using Timer1, for <baudrate> and 8-N-1.
//
//-----------------------------------------------------------------------------
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
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits
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
   TCON_TR1 = 1;                       // START Timer1
   SCON0_TI = 1;                       // Indicate TX0 ready

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// EMIF_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the external memory interface to non-multiplexed split-mode
// with bank select.
//
//-----------------------------------------------------------------------------
void EMIF_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   EMI0CF = 0x38;                      // Non-muxed mode; split mode with bank
                                       // select

   SFRPAGE = LEGACY_PAGE;
   EMI0CN = 0x40;                      // Access addresses on page 0x4000 for
                                       // 8-bit XRAM moves

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// putchar
//-----------------------------------------------------------------------------
//
//  Copyright KEIL ELEKTRONIK GmbH 1990 - 2002
//
// putchar (basic version): expands '\n' into CR LF
//
//-----------------------------------------------------------------------------
/*
char putchar (int8_t c)
{
   uint8_t SFRPAGE_SAVE = SFRPAGE;

   SFRPAGE = LEGACY_PAGE;

   if (c == '\n')                      // If carriage return
   {
      while (!TI0);
      TI0 = 0;
      SBUF0 = 0x0d;                    // Output CR
   }

   while (!TI0);                       // Wait for transmit complete
   TI0 = 0;

   SBUF0 = c;                          // Send character

   SFRPAGE = SFRPAGE_SAVE;

   return c;
}
*/
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
