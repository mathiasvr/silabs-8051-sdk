//-----------------------------------------------------------------------------
// F91x_WakeOnUART.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program implements Low-Power Wake-on-UART. The MCU will be in sleep mode
// until a high-to-low transition is detected on the UART RX pin. It will then
// wake up and receive the character. The received character is echoed back.
//
// How To Test:
//
// Setup:
// 1) Ensure that jumpers are placed on the following:
//       J12:  P0.4/TX
//             P0.5/RX
//             P1.5/LED1.5
//             VDD/VIO
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect a USB cable from the development board P3 to a PC
// 3) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the USB port (virtual com port) at <BAUDRATE> and 8-N-1
// 4) Connect the USB Debug Adapter to J4.
// 5) Turn the POWER switch (SW5) to the "ON" position.
// 6) Compile and download code to a 'F91x device on a C8051F91x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) Type characters into the terminal and click the "Send Data" button. The
//    MCU will echo them back. When the MCU is awake, the Red LED (P1.5) will
//    be lit.
//
//
// Target:         C8051F90x-C8051F91x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 14 JAN 2014
//
// Release 1.2 
//    - Compiled and tested for Raisonance Toolchain (FB)
//    - 17 MAY 10
//
// Release 1.1
//    - Port to C8051F912 from C8051F930 (JH)
//    - Compile and test C8051F912_defs.h on 912 board (JH)
//    - 06 JULY 2009
//
// Release 1.0
//    - Initial Revision (PKC)
//    - 20 MAY 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F912_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(RED_LED,      SFR_P1, 5);        // '0' means ON, '1' means OFF
SI_SBIT(YELLOW_LED,   SFR_P1, 6);        // '0' means ON, '1' means OFF
SI_SBIT(SW2,          SFR_P0, 2);        // SW2 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK          24500000L      // SYSCLK frequency in Hz
#define RTCCLK          32768          // smaRTClock frequency in Hz

#define BAUDRATE        57600L         // Baud rate of UART in bps
                                       // Up to 38400 bps with INDUCTOR_PEAK_500=0
                                       // Up to 57600 bps with INDUCTOR_PEAK_500=1

#define TIMEOUT_MS         1           // Wait time in ms

                                       // Set this to 1 to support up to 57600 baud
#define INDUCTOR_PEAK_500  1           // 0 = DC-DC Converter inductor peak current limit = 125mA
                                       // 1 = DC-DC Converter inductor peak current limit = 500mA
                                       
#define WAKE_LED_ACTIVE    1           // 0 = Red LED will be always off
                                       // 1 = Red LED will be on if MCU is awake

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void PORT_IO_Init (void);
void SYSCLK_Init (void);
void TIMER2_Init (void);
void UART0_Init (void);

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
   uint8_t rx_char;
   uint16_t temp;

                                       // enable)

   SYSCLK_Init ();
   PORT_IO_Init ();
   TIMER2_Init ();
   UART0_Init ();

   for (temp = 0; temp < 0xffff; temp++);    // Delay before sending to ToolStick
      
   printf ("\f\f");
   printf ("\nHello! This program demonstrates low-power Wake-on-UART\n");
   printf ("MCU will enter sleep mode now, and will wake up on receiving a UART RX transition\n");

   EIE2 |= 0x02;                       // Enable Port Match interrupt
   IE_EA = 1;                             // Enable global interrupts

   while (1)
   {
      if (TMR2CN_TF2H == 1 && SCON0_TI == 1)       // If timeout has occured AND last character was transmitted:
      {
         RED_LED = 1;                  // Turn off LED

         PMU0CF |= 0x20;               // Clear flags
         PMU0CF = 0x82;                // Enter Sleep mode with Port Match Wake Up

         #if WAKE_LED_ACTIVE == 1
            RED_LED = 0;               // Turn On LED 
         #endif

         TMR2CN_TR2 = 0;                      // Set timeout
         TMR2CN_TF2H = 0;
         TMR2 = -(TIMEOUT_MS*SYSCLK/12/1000);
         TMR2CN_TR2 = 1;
      }

      while (SCON0_RI == 0 && TMR2CN_TF2H == 0); // Wait till byte fully received or timeout

      if (SCON0_RI == 1)
      {
         SCON0_RI = 0;
         rx_char = SBUF0;              // Read received character
         printf ("%c", SBUF0);         // Echo received character
      }
   }
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

   OSCICN |= 0x80;                     // Enable the precision internal osc.
   while((OSCICN & 0x40) == 0);        // Wait till IFRDY is set

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   FLSCL  = 0x40;                      // Configre Flash read timing for
                                       // SYSCLK >10 MHz.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock

}

//-----------------------------------------------------------------------------
// Port_IO_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will initialize the Port I/O pins.
//
//-----------------------------------------------------------------------------
void PORT_IO_Init (void)
{
   // P0.0  -  Unassigned,  Open-Drain, Digital
   // P0.1  -  Unassigned,  Open-Drain, Digital
   // P0.2  -  Skipped,     Open-Drain, Digital (Push Button Switch, S2)
   // P0.3  -  Unassigned,  Open-Drain, Digital
   // P0.4  -  TX0 (UART0), Pull-Pull,  Digital
   // P0.5  -  RX0 (UART0), Open-Drain, Digital
   // P0.6  -  Unassigned,  Open-Drain, Digital
   // P0.7  -  Unassigned,  Open-Drain, Digital

   // P1.0  -  Unassigned,  Open-Drain, Digital
   // P1.1  -  Unassigned,  Open-Drain, Digital
   // P1.2  -  Unassigned,  Open-Drain, Digital
   // P1.3  -  Unassigned,  Open-Drain, Digital
   // P1.4  -  Unassigned,  Open-Drain, Digital
   // P1.5  -  Skipped,     Open-Drain, Digital (Red LED)
   // P1.6  -  Skipped,     Open-Drain, Digital (Yellow LED)
   // P1.7  -  Unassigned,  Open-Drain, Digital

   // P2.0  -  Unassigned,  Open-Drain, Digital
   // P2.1  -  Unassigned,  Open-Drain, Digital
   // P2.2  -  Unassigned,  Open-Drain, Digital
   // P2.3  -  Unassigned,  Open-Drain, Digital
   // P2.4  -  Unassigned,  Open-Drain, Digital
   // P2.5  -  Unassigned,  Open-Drain, Digital
   // P2.6  -  Unassigned,  Open-Drain, Digital
   // P2.7  -  Skipped,     Open-Drain, Digital

   DC0CN = 0x06;                       // Set Supply Voltage to 3.0 Volts

#if INDUCTOR_PEAK_500 == 1
   DC0CF = 0x04;                       // Peak inductor current limit = 500 mA
#endif

   P0MDOUT   = 0x10;
   P0SKIP    = 0x04;
   P1SKIP    = 0x60;

   P0MASK    = 0x24;                   // Port match interrupt when TX0=0 or SW2=0

   XBR0      = 0x01;
   XBR2      = 0x40;
}

//-----------------------------------------------------------------------------
// TIMER2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes Timer2 as 16-bit auto-reload timer clocked by
// SYSCLK/12.
//
//-----------------------------------------------------------------------------

void TIMER2_Init (void)
{
   TMR2CN = 0x00;
   TMR2RL = -(TIMEOUT_MS*SYSCLK/12/1000);
   TMR2 = 0xFFFF;                      // Set to overflow immediately
   TMR2CN_TR2 = 1;                            // Start timer
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
// PORTMATCH_ISR
//-----------------------------------------------------------------------------
//
// This ISR code is executed on a port match event. 
// It resets Timer2, which is used as a timeout to go back into sleep mode.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PORTMATCH_ISR, PMATCH_IRQn)
{
   TMR2CN_TR2 = 0;
   TMR2CN_TF2H = 0;
   TMR2 = -(TIMEOUT_MS*SYSCLK/12/1000);
   TMR2CN_TR2 = 1;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------