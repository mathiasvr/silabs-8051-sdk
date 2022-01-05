//-----------------------------------------------------------------------------
// F580_PCA1_Capture_Input.c
//-----------------------------------------------------------------------------
// Copyright 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program uses the PCA's capture mode to time events on an input pin.
// The difference between subsequent captures is calculated and stored in
// memory.
//
// In this example, PCA1 Module 6 is used to capture rising edges of an
// incoming digital waveform, and the crossbar is configured to receive the
// CEX6 pin on on P0.0.  The PCA is configured to clock once every 10 us.
//
// Timer2 overflows are used to toggle pin P0.1 at an overflow rate specified
// by the definition T2_RELOAD_CLOCKS. This signal at P0.1 is provided simply
// as a test method for the code.
//
// How To Test:
//
// 1) Download code to a 'F580 device with P0.0 and P0.1 tied together.
// 2) Click "Debug F580_PCA1_Capture_Input ..." button in the toolbar.
// 3) Place the cursor on the line that reads "<-- break here".
// 4) Click the menu Run > Toggle Breakpoint,  Right-click the blue dot in the
//    left margin > Breakpoint Properties, set "Ignore count" to 100.
// 5) Click Resume in the toolbar.
// 6) Click the Variables view. "capture_period" should be 50.
//
// Alternately:
//
// 1) Run a separate signal generator into the P0.0 pin.
// 2) Click "Debug F580_PCA1_Capture_Input ..." button in the toolbar.
// 3) Place the cursor on the line that reads "<-- break here".
// 4) Click the menu Run > Toggle Breakpoint,  Right-click the blue dot in the
//    left margin > Breakpoint Properties, set "Ignore count" to 100.
// 5) Click Resume in the toolbar.
// 6) Click the Variables view. Verify that the variable "capture_period"
//    matches the expected time between rising edges (in units of 10us).
//
// Target:         C8051F580 (C8051F580-TB)
// Tool chain:     Keil C51 8.0 / Keil EVAL C51
// Command Line:   None
//
// Release 1.2 / 10 JUL 2015 (SG)
//    - Revised for Simplicity Studio
//
// Release 1.1 / 14 APR 2009 (GP)
//    -Fixed clearing of PCA flags in PCA1 ISR
//
// Release 1.0 / 21 JUL 2008 (ADT)
//    - Initial Revision
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F580_Register_Enums.h>         // SFR declarations

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24000000          // Internal oscillator frequency in Hz

#define T0_CLOCKS         240          // Use 240 clocks per T0 Overflow
                                       //    (240 = 10 us)

#define T2_RELOAD_CLOCKS 6000          // SYSCLKs per 1/2 cycle square wave
                                       //    (6000 = 250 us)

SI_SBIT(TEST_OUT, SFR_P0, 1);            // Output for testing purposes

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void PCA1_Init (void);
void TestTimerInit (void);

SI_INTERRUPT_PROTO(PCA1_ISR, PCA1_IRQn);

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
   PCA0MD &= ~0x40;                    // Disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SFRPAGE = ACTIVE1_PAGE;             // Set SFR Page for PCA0MD and Timer


   PORT_Init ();                       // Initialize crossbar and GPIO
   OSCILLATOR_Init ();                 // Initialize oscillator
   PCA1_Init ();                       // Initialize PCA1

   TestTimerInit ();                   // Configure Timer2 for test signal

   IE_EA = 1;                          // Enable Global Interrupts

   while (1)
   {
      // check to see if Timer2 overflowed
      if (TMR2CN_TF2H)
      {
         TEST_OUT = !TEST_OUT;         // Toggle test pin
         TMR2CN_TF2H = 0;              // Clear T2 overflow
      }

   };
}


//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at 24 MHz.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0x87;                      // Set internal oscillator to run
                                       // at its maximum frequency

   CLKSEL = 0x00;                      // Select the internal osc. as
                                       // the SYSCLK source

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
// P0.0   digital   open-drain     PCA1 CEX6
// P0.1   digital   push-pull      Test Out
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x01;                    // Set CEX6 (P0.0) to push-pull
   P0MDOUT |= 0x02;                    // Set TEST_OUT (P0.1) to push-pull

   XBR3 = 0x01;                        // Enable CEX6 on crossbar
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PCA1_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA1 time base, and sets up capture mode for
// Module 6 (CEX6 pin).
//
// The PCA will capture rising edges on the CEX6 pin, and interrupt the
// processor.  Software subtracts the previous capture value from the most
// recent caputre value to set the "captured_clocks" variable.
//
// The PCA time base in this example is configured to use Timer 0, overflowing
// every 10 us. The PCA clock rate will determine the maximum time between
// edges that can be reliably detected (this is equal to 65535 * PCA clock).
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
//    value (PCA1) into the capture/compare register for the module (PCA1CPn).
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
void PCA1_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE2_PAGE;

   // Configure Timer 0 for 8-bit auto-reload mode, using SYSCLK as time base
   TMOD &= 0xF0;                       // Clear all T0 control bits
   TMOD |= 0x02;                       // 8-bit auto-reload timer
   CKCON |= 0x04;                      // T0 uses SYSCLK
   TH0 = -(int8_t)T0_CLOCKS;               // Set up reload value
   TL0 = -(int8_t)T0_CLOCKS;               // Set up initial value

   // Configure PCA time base; overflow interrupt disabled
   PCA1CN = 0x00;                      // Stop counter; clear all flags
   PCA1MD = 0x04;                      // Use Timer 0 as time base


   PCA1CPM6 = 0x21;                    // Module 6 = Rising Edge Capture Mode
                                       // enable CCF flag.


   EIE2 |= 0x10;                       // Enable PCA1 interrupts

   // Start PCA counter
   PCA1CN_CR = 1;
   TCON_TR0 = 1;                       // Start Timer 0

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// TestTimerInit
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine simply sets up Timer 2 in 16-bit auto-reload mode, to be used
// as a rudimentary source for testing
//
// The T2_RELOAD_CLOCKS value can be set up to generate multiples of 10us for
// testing, by using the formula:
//
// T2_RELOAD_CLOCKS = 240 * (Time_between_rising_edges / 10us)/2
//
//-----------------------------------------------------------------------------
void TestTimerInit (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE;

   CKCON |= 0x10;                      // Use SYSCLK to clock Timer2

   TMR2RL = -(int16_t)T2_RELOAD_CLOCKS;    // Set up Timer 2 reload rate
   TMR2 = -(int16_t)T2_RELOAD_CLOCKS;      // Preload timer

   TMR2CN = 0x04;                      // Timer 2 run in 16b auto-reload

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCA1_ISR
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This is the ISR for PCA1.  It handles the case when a capture occurs on
// CEX6, and updates the variables to reflect the new capture information.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PCA1_ISR, PCA1_IRQn)
{
   static uint16_t current_capture_value, previous_capture_value;
   static uint16_t capture_period;

   if (PCA1CN_CCF6)                    // If Module 6 caused the interrupt
   {
      PCA1CN_CCF6 = 0;                 // Clear module 6 interrupt flag.

      // Store most recent capture value
      current_capture_value = PCA1CP6;

      // Calculate capture period from last two values.
      capture_period = current_capture_value - previous_capture_value;

      // <-- break here and view the contents of "capture_period"

      // Update previous capture value with most recent info.
      previous_capture_value = current_capture_value;

   }
   else                                // Interrupt was caused by other bits.
   {
      PCA1CN &= ~0xBE;                 // Clear other interrupt flags for PCA1
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
