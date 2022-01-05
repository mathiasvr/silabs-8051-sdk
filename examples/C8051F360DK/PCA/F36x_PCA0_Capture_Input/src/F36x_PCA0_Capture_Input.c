//-----------------------------------------------------------------------------
// F36x_PCA0_Capture_Input.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// Use the PCA to time external IO events
//
// This program uses the PCA's capture mode to time events on an input pin.
// The difference between subsequent captures is calculated and stored in
// memory.
//
// In this example, PCA Module 0 is used to capture rising edges of an
// incoming digital waveform, and the crossbar is configured to receive the
// CEX0 pin on on P0.0. The PCA is configured to clock once every 10 us.
//
// Timer2 overflows are used to toggle pin P0.1 at an overflow rate specified
// by the definition T2_RELOAD_CLOCKS. This signal at P0.1 is provided simply
// as a test method for the code.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J1: Power selection, P2 9V, P4 USB or J9 Debugger
// 2) Connect the USB Debug Adapter to J9. If 9V power is selected on J1,
//    connect the 9V power adapter to P2. If VBUS is selected on J1, connect
//    USB cable to P4.
// 3) SELF TEST:
//     - Add a jumper from P0.1 to P0.0 (T2 Output - PCA0 CEX0)
//    EXTERNAL SOURCE:
//     - Connect an external signal to P0.0 (PCA0 CEX0)
// 4) Compile and download code to C8051F360 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 6) Run the program for a few seconds, and then halt.
// 7) SELF TEST:
//     - Verify that "capture_period" == 50 (500us)
//    EXTERNAL SOURCE:
//     - Verify that the variable "Capture_Period" matches the expected time
//    between rising edges (in units of 10us).
//
//
// Target:         C8051F36x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1
//    - Updated to use with Simplicity Studio (JL)
//    - 29 JUL 2014
//
// Release 1.0
//    -Initial Revision (BD / TP)
//    -16 OCT 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>


//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // Internal oscillator frequency in Hz

#define T0_CLOCKS    245               // Use 245 clocks per T0 Overflow
                                       //    (245 = 10 us)
#define T2_RELOAD_CLOCKS 6125          // SYSCLKs per 1/2 cycle square wave
                                       //    (245 = 10 us)

SI_SBIT(TEST_OUT, SFR_P0, 1);            // Output for testing purposes

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Port_Init (void);
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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void) 
{

   Port_Init ();                       // Initialize crossbar and GPIO
   Oscillator_Init ();                 // Initialize oscillator
   PCA0_Init ();                       // Initialize PCA0

   TestTimerInit ();                   // Configure Timer2 for test signal

   SFRPAGE = LEGACY_PAGE;

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
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // Set internal oscillator to run at its maximum frequency
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;

   // Select the internal osc. as the SYSCLK source
   CLKSEL = CLKSEL_CLKSL__DIVIDED_HFOSC;

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
// P0.0   digital   open-drain     PCA0 CEX0
// P0.1   digital   push-pull      TEST_OUT
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // Route CEX0 to P0.0, enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED |
          XBR1_XBARE__ENABLED |
          XBR1_PCA0ME__CEX0;

   P0MDOUT |= 0x02;                    // Set TEST_OUT (P0.1) to push-pull

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

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

   TCON_TR0  = 1;                      // Start Timer 0

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
// T2_RELOAD_CLOCKS = 245 * (Time_between_rising_edges / 10us)/2
//
//-----------------------------------------------------------------------------
void TestTimerInit (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   CKCON |= CKCON_T2ML__SYSCLK;        // Use SYSCLK to clock Timer2

   TMR2RL = -(int)T2_RELOAD_CLOCKS;    // Set up Timer 2 reload rate
   TMR2 = -(int)T2_RELOAD_CLOCKS;      // preload timer

   TMR2CN = TMR2CN_TR2__RUN;           // Timer 2 run in 16b auto-reload

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This is the ISR for the PCA.  It handles the case when a capture occurs on
// channel 0, and updates the variables to reflect the new capture information.
//
//-----------------------------------------------------------------------------
static uint16_t Capture_Period;

SI_INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
   static uint16_t Current_Capture_Value, Previous_Capture_Value;


   if (PCA0CN_CCF0)                    // If Module 0 caused the interrupt
   {
      PCA0CN_CCF0 = 0;                 // Clear module 0 interrupt flag.

      // Store most recent capture value
      Current_Capture_Value = PCA0CP0;

      // Calculate capture period from last two values.
      Capture_Period = Current_Capture_Value - Previous_Capture_Value;

      // Update previous capture value with most recent info.
      Previous_Capture_Value = Current_Capture_Value;

   }
   else                                // Interrupt was caused by other bits.
   {
      // Clear other interrupt flags for PCA
      PCA0CN &= ~(PCA0CN_CF__BMASK   |
                  PCA0CN_CCF5__BMASK |
                  PCA0CN_CCF4__BMASK |
                  PCA0CN_CCF3__BMASK |
                  PCA0CN_CCF2__BMASK |
                  PCA0CN_CCF1__BMASK);
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------