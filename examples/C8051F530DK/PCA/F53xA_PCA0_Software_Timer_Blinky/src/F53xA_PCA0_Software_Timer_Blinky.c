//-----------------------------------------------------------------------------
// F53xA_PCA0_Software_Timer_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use the PCA as a software timer
//
// This program uses the PCA's Software Timer Mode to periodically schedule
// an interrupt.  Inside the ISR, a port pin is toggled to blink an LED on the
// target board.
//
// In this example, PCA Module 0 is used to generate the interrupt, and the
// port pin driving the LED is configured for push-pull mode.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F530A TB are placed on the following:
//    "A" SIDE
//    HDR4: P1.3_A - P1.3_LED_A
//    "B" SIDE
//    HDR3: P1.3_B - P1.3_LED_B
// 2) Connect the USB Debug Adapter to HDR2 for "A" SIDE or HDR1 for "B" SIDE.
//    Connect the 9V power adapter to P5.
// 3) Compile and download code to C8051F530A target board
//    (either "A" SIDE or "B" SIDE) with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) The LED should blink at 5 Hz.
//
//
// Target:         C8051F52xA/F53xA (C8051F530A TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.3 / 09 SEP 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.2 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.1
//    -Updated for 'F530A TB (TP)
//    -30 JAN 2008
//
// Release 1.0
//    -Initial Revision (SM)
//    -11 JUN 2007


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F530_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // Internal oscillator frequency in Hz

#define LED_FREQUENCY       5          // Frequency to blink LED at in Hz
#define T0_CLOCKS         245          // Use 245 clocks per T0 Overflow

// SYSCLK cycles per interrupt
#define PCA_TIMEOUT ((SYSCLK/T0_CLOCKS)/LED_FREQUENCY/2)

SI_SBIT(LED, SFR_P1, 3);                 // LED='1' means ON

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Port_Init (void);
void PCA0_Init (void);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint16_t Next_Compare_Value;       // Next edge to be sent out in HSO mode

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
   PCA0MD = 0x00;                      // Disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   Port_Init ();                       // Initialize crossbar and GPIO
   Oscillator_Init ();                 // Initialize oscillator
   PCA0_Init ();                       // Initialize PCA0

   IE_EA = 1;                          // Globally enable interrupts

   while (1);
}


//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at 24.5 MHz.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   // Set internal oscillator to run at its maximum frequency
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// No crossbar peripherals are used for this example.
//
// P1.3 (LED)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1MDOUT |= 0x08;                    // Set LED (P1.3) to push-pull

   // Enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED |
          XBR1_XBARE__ENABLED;
}

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA time base to use T0 Overflows as a clock
// source.  Timer0 is also configured by this routine for 8-bit auto-reload
// mode, where T0_CLOCKS is the number of System clocks per T0 Overflow.
// The code then sets up Software Timer mode for Module 0 (CEX0 pin).
//
// On every interrupt from Module 0, software toggles the port I/O pin for the
// LED. The frequency of the LED toggling is determined by the parameter
// CEX0_FREQUENCY.
//
// The maximum frequency output for this example is about 50 kHz.
// The minimum frequency output for this example is about 1 Hz
//
// The PCA time base in this example is configured to use T0 overflows, and
// SYSCLK is set up for the internal oscillator running at 24.5 MHz.
// Using different PCA clock sources, different T0 reload values, or a
// different processor clock will result in a different frequency for the LED
// square wave, and different maximum and minimum options.
//
//    -------------------------------------------------------------------------
//    How "Software Timer Mode" Works:
//
//       The PCA's Software Timer Mode works by generating an interrupt for the
//    associated module every time the PCA0 register increments and the new
//    16-bit PCA0 counter value matches the module's capture/compare
//    register (PCA0CPn).
//
//    By loading the PCA0CPn register with the next match value every time a
//    match happens, arbitrarily timed interrupts can be generated.
//    -------------------------------------------------------------------------
//
// When setting the capture/compare register for the next match value, the low
// byte of the PCA0CPn register (PCA0CPLn) should be written first, followed
// by the high byte (PCA0CPHn).  Writing the low byte clears the ECOMn bit,
// and writing the high byte will restore it.  This ensures that a match does
// not occur until the full 16-bit value has been written to the compare
// register.  Writing the high byte first will result in the ECOMn bit being
// set to '0' after the 16-bit write, and the next match will not occur at
// the correct time.
//
// It is best to update the capture/compare register as soon after a match
// occurs as possible so that the PCA counter will not have incremented past
// the next desired edge value. This code implements the compare register
// update in the PCA ISR upon a match interrupt.
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
   // Configure Timer 0 for 8-bit auto-reload mode, using SYSCLK as time base
   TMOD &= 0xF0;                       // Clear all T0 control bits
   TMOD |= 0x02;                       // 8-bit auto-reload timer
   CKCON |= 0x04;                      // T0 uses SYSCLK
   TH0 = -T0_CLOCKS;                   // Set up reload value
   TL0 = -T0_CLOCKS;                   // Set up initial value

   // Configure PCA time base; overflow interrupt disabled
   PCA0CN = 0x00;                      // Stop counter; clear all flags
   PCA0MD = 0x04;                      // Use Timer 0 as time base

   PCA0CPM0 = 0x49;                    // Module 0 = Software Timer Mode,
                                       // Enable Module 0 Interrupt flag,
                                       // Enable ECOM bit

   PCA0L = 0x00;                       // Reset PCA Counter Value to 0x0000
   PCA0H = 0x00;

   PCA0CPL0 = PCA_TIMEOUT & 0x00FF;    // Set up first match
   PCA0CPH0 = (PCA_TIMEOUT & 0xFF00) >> 8;

   // Set up the variable for the following match
   Next_Compare_Value = PCA0CP0 + PCA_TIMEOUT;

   EIE1 |= EIE1_EPCA0__ENABLED;        // Enable PCA interrupts

   // Start PCA counter
   PCA0CN_CR = 1;
   TCON_TR0 = 1;                       // Start Timer 0
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// This is the ISR for the PCA.  It handles the case when a match occurs on
// channel 0, and updates the PCA0CPn compare register with the value held in
// the global variable "Next_Compare_Value".
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
   if (PCA0CN_CCF0)                    // If Module 0 caused the interrupt
   {
      PCA0CN_CCF0 = 0;                 // Clear module 0 interrupt flag.

      PCA0CPL0 = (Next_Compare_Value & 0x00FF);
      PCA0CPH0 = (Next_Compare_Value & 0xFF00)>>8;

      LED = ~LED;                      // Invert the LED pin

      // Set up the variable for the following edge
      Next_Compare_Value = PCA0CP0 + PCA_TIMEOUT;
   }
   else
   {
      // Clear other interrupt flags for PCA
      PCA0CN &= ~(PCA0CN_CF__BMASK |
                  PCA0CN_CCF2__BMASK |
                  PCA0CN_CCF1__BMASK);
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------