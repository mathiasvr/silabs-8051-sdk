//-----------------------------------------------------------------------------
// F53xA_PCA0_Capture_Input.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use the PCA to time external IO events
//
// This program uses the PCA's capture mode to time events on an input pin.
// The difference between subsequent captures is calculated and stored in
// memory.
//
// In this example, PCA Module 0 is used to capture rising edges of an
// incoming digital waveform, and the crossbar is configured to receive the
// CEX0 pin on on P0.0.  The PCA is configured to clock once every 10 us.
//
// Timer2 overflows are used to toggle pin P0.1 at an overflow rate specified
// by the definition T2_RELOAD_CLOCKS. This signal at P0.1 is provided simply
// as a test method for the code.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F530A TB are placed on the following:
//    "A" SIDE
//    J2: P0.0_A - P0.1_A
//    "B" SIDE
//    J1: P0.0_B - P0.1_B
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
// 5) Run the program for a few seconds, and then halt.
// 6) Verify that the variable "capture_period" is equal to 50 (=500us).
//
// Alternately:
// 1) Run a separate signal generator into the P0.0 pin.
// 2) Run the program for a few seconds, and then halt.
// 3) Verify that the variable "capture_period" matches the expected time
//    between rising edges (in units of 10us).
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

#define T0_CLOCKS         245          // Use 245 clocks per T0 Overflow
                                       //    (245 = 10 us)
#define T2_RELOAD_CLOCKS 6125          // SYSCLKs per 1/2 cycle square wave
                                       //    (245 = 10 us)

SI_SBIT(TEST_OUT, SFR_P0, 1);            // Output for testing purposes

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void PCA0_Init (void);

void TestTimerInit (void);

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

   PORT_Init ();                       // Initialize crossbar and GPIO
   OSCILLATOR_Init ();                 // Initialize oscillator
   PCA0_Init ();                       // Initialize PCA0

   TestTimerInit ();                   // Configure Timer2 for test signal

   IE_EA = 1;

   while (1)
   {
      // Check to see if Timer2 overflowed
      if (TMR2CN_TF2H)
      {
         TEST_OUT = ~TEST_OUT;         // Toggle test pin
         TMR2CN_TF2H = 0;              // Clear T2 overflow
      }
   }
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
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P0.0   digital   open-drain     PCA0 CEX0
// P0.1   digital   push-pull
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P0MDOUT |= 0x02;                    // Set TEST_OUT (P0.1) to push-pull

   // Enable PCA0 CEX0 output, enable crossbar and weak pull-ups
   XBR1 = XBR1_XBARE__ENABLED |
          XBR1_PCA0ME__CEX0;
}

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA time base, and sets up capture mode for
// Module 0 (CEX0 pin).
//
// The PCA will capture rising edges on the CEX0 pin, and interrupt the
// processor.  Software subtracts the previous capture value from the most
// recent caputre value to set the "captured_clocks" variable.
//
// The PCA time base in this example is configured to use Timer 0, overflowing
// every 10 us. The PCA clock rate will determine the maximum time between
// edges that can be reliably detected (this is equal to 65535 * the PCA clock).
//
// The time range that can be measured using this example is 10us (MIN) to
// 655.350 ms (MAX) when the clocks are set up as they are in this example.
// The precision is limited to the PCA clock rate (in this example, 10 us).
// Using different PCA clock sources or a different processor clock will allow
// for different time capture ranges or levels of precision.
//
//    -------------------------------------------------------------------------
//    How "Capture Mode" Works:
//
//    The PCA's Capture Mode works by making a copy of the current PCA timer
//    value (PCA0) into the capture/compare register for the module (PCA0CPn).
//    The module can be configured to capture rising edges, falling edges, or
//    both rising and falling edges.  When a capture occurs, the CCFn flag is
//    set, and an interrupt is (optionally) generated by the PCA module.
//
//    To detect an edge (and not reject it as noise) the new logic level must
//    be present at the pin for at least two SYSCLK cycles.
//
//    With a constantly-running PCA timer, the time between two edges on a
//    signal can be accurately measured to within 1 PCA clock cycle.
//    Multiple PCA channels can be used in capture mode to measure timing
//    between different signals.
//    -------------------------------------------------------------------------
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


   PCA0CPM0 = 0x21;                    // Module 0 = Rising Edge Capture Mode
                                       // enable CCF flag.


   EIE1 |= EIE1_EPCA0__ENABLED;        // Enable PCA interrupts

   // Start PCA counter
   PCA0CN_CR = 1;
   TCON_TR0 = 1;                       // Start Timer 0
}

//-----------------------------------------------------------------------------
// TestTimerInit
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine simply sets up Timer 2 in 16-bit auto-reload mode, to be used
// as a rudimentary source for testing.
//
// The T2_RELOAD_CLOCKS value can be set up to generate multiples of 10us for
// testing, by using the formula:
//
// T2_RELOAD_CLOCKS = 245 * (Time_between_rising_edges / 10us)/2
//
//-----------------------------------------------------------------------------
void TestTimerInit (void)
{
   CKCON |= 0x10;                      // Use SYSCLK to clock Timer2

   TMR2RL = -(int)T2_RELOAD_CLOCKS;    // Set up Timer 2 reload rate
   TMR2 = -(int)T2_RELOAD_CLOCKS;      // Preload timer

   TMR2CN = TMR2CN_TR2__RUN;           // Timer 2 run in 16b auto-reload
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// This is the ISR for the PCA.  It handles the case when a capture occurs on
// channel 0, and updates the variables to reflect the new capture information.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
   static uint16_t current_capture_value, previous_capture_value;
   static uint16_t capture_period;

   if (PCA0CN_CCF0)                    // If Module 0 caused the interrupt
   {
      PCA0CN_CCF0 = 0;                 // Clear module 0 interrupt flag.

      // Store most recent capture value
      current_capture_value = PCA0CP0;

      // Calculate capture period from last two values.
      capture_period = current_capture_value - previous_capture_value;

      // Update previous capture value with most recent info.
      previous_capture_value = current_capture_value;

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