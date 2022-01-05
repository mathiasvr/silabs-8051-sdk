//-----------------------------------------------------------------------------
// F580_External_Interrupts.c
//-----------------------------------------------------------------------------
// Copyright 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This software shows the necessary configuration to use External Interrupt 0
// (/INT0) or External Interrupt 1 (/INT1) as an interrupt source.  The code
// executes the initialization routines and then spins in an infinite while()
// loop.  If the button on P1.4 (on the target board) is pressed, then the
// edge-triggered /INT0 input on P1.0 will cause an interrupt and toggle the
// LED.
//
// Pinout:
// P1.0 - /INT0
// P1.1 - /INT1
// P1.3 - LED
// P1.4 - SW1 
//
// Resources Used:
// * Internal Oscillator: MCU clock source
//
// How To Test:
//
// 1) Compile and download code to a 'F580 target board.
// 2) On the target board, connect P1.4 to P1.0 for /INT0 or P1.1 for /INT1.
// 3) Remove the LIN_TX and LIN_RX jumpers from J17.
// 3) Press the switch.  Every time a switch is pressed, the P1.3
//    LED should toggle.
//
// Target:         C8051F580 (Side A of a C8051F580-TB)
// Tool chain:     Raisonance / Keil / SDCC
// Command Line:   None
//
// Release 1.2 / 10 JUL 2015 (SG)
//    - Revised for Simplicity Studio
//
// Release 1.1 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0 / 21 JUL 2008 (ADT)
//    - Initial Revision
//
//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F580_defs.h>         // SFR declarations

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK           240000000/8   // Clock speed in Hz

SI_SBIT(LED, SFR_P1, 3);                 // LED == 1 means ON
SI_SBIT(SW1, SFR_P1, 4);                 // SW1 == 0 means switch depressed

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);           // Configure the system clock
void PORT_Init (void);                 // Configure the Crossbar and GPIO
void EXTERNAL_INTERRUPT_Init (void);   // Configure External Interrupts 

SI_INTERRUPT_PROTO(INT0_ISR, INT0_IRQn);
SI_INTERRUPT_PROTO(INT1_ISR, INT1_IRQn);

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
   PCA0MD &= ~0x40;                    // Disable Watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   SFRPAGE = ACTIVE1_PAGE;             // Set SFR Page for PCA0MD


   OSCILLATOR_Init ();                 // Initialize the system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   EXTERNAL_INTERRUPT_Init ();         // Initialize External Interrupts

   IE_EA = 1;

   while(1);                           // Infinite while loop waiting for
                                       // an interrupt from /INT0 or /INT1
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24 MHz / 8
// oscillator as its clock source.  Also enables missing clock detector reset.
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0x84;                      // Configure internal oscillator for
                                       // 24 MHz / 8

   SFRPAGE = ACTIVE1_PAGE;

   RSTSRC = 0x04;                      // Enable missing clock detector

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// Pinout:
//
// P1.0 - digital   open-drain      /INT0
// P1.1 - digital   open-drain      /INT1
// P1.3 - digital   push-pull       LED
// P1.4 - digital   open-drain      SW1 (Switch 1)
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P1SKIP = 0x03;                      // Skip P1.0 and P1.1
   P1MDOUT = 0x08;                     // LED is a push-pull output; all other
                                       // P1 pins are open-drain

   XBR2 = 0x40;                        // Enable crossbar and weak pullups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// EXTERNAL_INTERRUPT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures and enables /INT0 and /INT1 (External Interrupts)
// as negative edge-triggered.
//
//-----------------------------------------------------------------------------
void EXTERNAL_INTERRUPT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;              // Configured for IT01CF

   TCON = 0x05;                        // /INT0 and /INT1 are edge triggered

   IT01CF = 0x10;                      // /INT0 active low; /INT0 on P1.0;
                                       // /INT1 active low; /INT1 on P1.1

   IE_EX0 = 1;                         // Enable /INT0 interrupts
   IE_EX1 = 1;                         // Enable /INT1 interrupts

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// /INT0 ISR
//-----------------------------------------------------------------------------
//
// Whenever a negative edge appears on P1.0, LED is toggled.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(INT0_ISR, INT0_IRQn)
{
   LED = !LED;
}

//-----------------------------------------------------------------------------
// /INT1 ISR
//-----------------------------------------------------------------------------
//
// Whenever a negative edge appears on P1.1, LED is toggled.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(INT1_ISR, INT1_IRQn)
{
   LED = !LED;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
