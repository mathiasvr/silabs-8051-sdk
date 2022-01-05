//-----------------------------------------------------------------------------
// F580_Blinky.c
//-----------------------------------------------------------------------------
// Copyright (C) 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program flashes the green LED on the C8051F580 target board about
// five times a second using the interrupt handler for Timer2.
//
//
// How To Test:
//
// 1) Project > Properties, expand C/C++ Build, click Settings, under
//    "Tool Settings" tab, "Keil 8051 Linker" side-menu, click the page
//    Banking, and uncheck "Enable code banking"
// 2) Project > Properties, expand C/C++ Build, click Settings, under
//    "Tool Settings" tab, "Keil 8051 Assembler" side-menu, click the page
//    "Control Symbols", click the row containing the variable "USE_BANKING",
//    click the edit button, and set that row to "USE_BANKING = 0"
// 3) Download code to the 'F580 target board
// 4) Ensure that the P1.3 pins are shorted together on the J19 header
// 5) Run the program.  If the LED flashes, the program is working
//
//
// Target:         C8051F580 (C8051F580-TB)
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
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F580_Register_Enums.h>         // SFR declarations

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24000000/8        // SYSCLK frequency in Hz

SI_SBIT(LED, SFR_P1, 3);                 // LED = 1 means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void TIMER2_Init (uint16_t counts);

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
   PCA0MD &= ~0x40;                    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   SFRPAGE = ACTIVE1_PAGE;


   OSCILLATOR_Init ();                 // Configure system clock
   PORT_Init ();                       // Initialize crossbar
   TIMER2_Init (SYSCLK / 12 / 10);     // Init Timer2 to generate interrupts
                                       // at a 10 Hz rate.

   IE_EA = 1;                          // Enable global interrupts

   while (1);                          // Spin forever
}


//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the internal oscillator to maximum internal frequency / 8
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0x84;

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the Crossbar and GPIO ports.
//
// P1.3 - digital  push-pull   LED
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P1MDOUT |= 0x08;                    // Enable LED as a push-pull output
   P1SKIP |= 0x08;                     // Skip the LED pin on the crossbar

   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// TIMER2_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    uint16_t counts - number of system clock cycles between interrupts
//                             range is 0 to 65535
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/12 as its time base.
//
//-----------------------------------------------------------------------------

void TIMER2_Init (uint16_t counts)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE;

   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON &= ~0x60;                     // Timer2 clocked based on T2XCLK;

   TMR2RL = -counts;                   // Init reload values
   TMR2 = 0xFFFF;                      // Set to reload immediately
   IE_ET2 = 1;                         // Enable Timer2 interrupts
   TMR2CN_TR2 = 1;                     // Start Timer2

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED whenever Timer2 overflows.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                    // Clear Timer2 interrupt flag
   LED = !LED;                         // Change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
