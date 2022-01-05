//-----------------------------------------------------------------------------
// F35x_PCA0_High_Speed_Output.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// Use the PCA to generate a high frequency signal (KHz)
//
// This program sends a square wave out on an I/O pin, using the PCA's
// High-Speed Output Mode.
//
// In this example, PCA Module 0 is used to generate the waveform, and the
// crossbar is configured to send the CEX0 pin out on P0.1.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are installed on the following:
//    - J8: Power selection, P1 9V or J4 Debugger
// 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1. After power the board, the red LED D3
//    is on.
// 3) Compile and download code to the C8051F350-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) Connect an oscilloscope to view the configured frequency on P0.1.
//    SYSCLK may be viewed by connecting an oscilloscope to P0.0.
//
//
// Target:         C8051F35x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1 (JL)
//    - Updated descriptions
//    - 01 JUL 2014
//
// Release 1.0
//    -Initial Revision (BD / TP)
//    -2 AUG 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F350_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // Internal oscillator frequency in Hz

#define CEX0_FREQUENCY 10000           // Frequency to output in Hz
#define DIVIDE_RATIO (SYSCLK/CEX0_FREQUENCY/2) // SYSCLK cycles per interrupt

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void PCA0_Init (void);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint16_t NEXT_COMPARE_VALUE;                // Next edge to be sent out in HSO mode

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void) {

   // Disable watchdog timer

   PORT_Init ();                       // Initialize crossbar and GPIO
   OSCILLATOR_Init ();                 // Initialize oscillator
   PCA0_Init ();                       // Initialize PCA0

   IE_EA = 1;                          // Globally enable interrupts

   while (1);                          // Spin here to wait for ISR
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
// at 24.5 MHz.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   // Set internal oscillator to run at its maximum frequency
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;

   // Select the internal osc. as the SYSCLK source
   CLKSEL = CLKSEL_CLKSL__DIVIDED_HFOSC;
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
// P0.0   digital   push-pull     PCA0 CEX0
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   XBR0 |= XBR0_SYSCKE__ENABLED;       // SYSCLK to P0.0

   // Route CEX0 to P0.0, enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED |
          XBR1_XBARE__ENABLED |
          XBR1_PCA0ME__CEX0;

   // Set SYSCLK and CEX0 (P0.0) to push-pull
   P0MDOUT |= 0x03;
}

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA time base, and sets up High-Speed output
// mode for Module 0 (CEX0 pin).
//
// The frequency of the square wave generated at the CEX0 pin is defined by
// the parameter CEX0_FREQUENCY.
//
// The maximum square wave frequency output for this example is about 230 kHz.
// The minimum square wave frequency output for this example is about 190 Hz
//
// The PCA time base in this example is configured to use SYSCLK, and SYSCLK
// is set up to use the internal oscillator running at 24.5 MHz.
// Using different PCA clock sources or a different processor clock will
// result in a different frequency for the square wave, and different
// maximum and minimum options.
//
//    -------------------------------------------------------------------------
//    How "High Speed Output Mode" Works:
//
//       The PCA's High Speed Output Mode works by toggling the output pin
//    associated with the module every time the PCA0 register increments and
//    the new 16-bit PCA0 counter value matches the module's capture/compare
//    register (PCA0CPn). When initially enabled in high-speed output mode, the
//    CEXn pin associated with the module will be at a logic high state.  The
//    first match will cause a falling edge on the pin.  The next match will
//    cause a rising edge on the pin, and so on.
//
//    By loading the PCA0CPn register with the next match value every time a
//    match happens, arbitrary waveforms can be generated on the pin with high
//    levels of precision.
//    -------------------------------------------------------------------------
//
// When setting the capture/compare register for the next edge value, the low
//  byte of the PCA0CPn register (PCA0CPLn) should be written first, followed
//  by the high byte (PCA0CPHn).  Writing the low byte clears the ECOMn bit,
//  and writing the high byte will restore it.  This ensures that a match does
//  not occur until the full 16-bit value has been written to the compare
//  register.  Writing the high byte first will result in the ECOMn bit being
//  set to '0' after the 16-bit write, and the next match will not occur at
// the correct time.
//
// It is best to update the capture/compare register as soon after a match
//  occurs as possible so that the PCA counter will not have incremented past
//  the next desired edge value. This code implements the compare register
//  update in the PCA ISR upon a match interrupt.
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
   // Configure PCA time base; overflow interrupt disabled
   // Stop counter; clear all flags
   PCA0CN = PCA0CN_CF__NOT_SET |
            PCA0CN_CR__STOP |
            PCA0CN_CCF2__NOT_SET |
            PCA0CN_CCF1__NOT_SET |
            PCA0CN_CCF0__NOT_SET;

   PCA0MD = PCA0MD_CPS__SYSCLK;        // Use SYSCLK as time base

   // Module 0 = High Speed Output mode, enable Module 0 Interrupt flag, enable
   // ECOM bit
   PCA0CPM0 = PCA0CPM0_PWM16__8_BIT   |
              PCA0CPM0_ECOM__ENABLED  |
              PCA0CPM0_CAPP__DISABLED |
              PCA0CPM0_CAPN__DISABLED |
              PCA0CPM0_MAT__ENABLED   |
              PCA0CPM0_TOG__ENABLED   |
              PCA0CPM0_PWM__DISABLED  |
              PCA0CPM0_ECCF__ENABLED;

   PCA0L = 0x00;                       // Reset PCA Counter Value to 0x0000
   PCA0H = 0x00;

   PCA0CPL0 = DIVIDE_RATIO & 0x00FF;   // Set up first edge
   PCA0CPH0 = (DIVIDE_RATIO & 0xFF00) >> 8;

   // Set up the variable for the following edge
   NEXT_COMPARE_VALUE = PCA0CP0 + DIVIDE_RATIO;

   EIE1 |= EIE1_EPCA0__ENABLED;        // Enable PCA interrupts

   PCA0CN_CR = 1;                      // Start PCA

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
// This is the ISR for the PCA.  It handles the case when a match occurs on
// channel 0, and updates the PCA0CPn compare register with the value held in
// the global variable "NEXT_COMPARE_VALUE".
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
   if (PCA0CN_CCF0)                    // If Module 0 caused the interrupt
   {
      PCA0CN_CCF0 = 0;                 // Clear module 0 interrupt flag.

      PCA0CPL0 = (NEXT_COMPARE_VALUE & 0x00FF);
      PCA0CPH0 = (NEXT_COMPARE_VALUE & 0xFF00) >> 8;

      // Set up the variable for the following edge
      NEXT_COMPARE_VALUE = PCA0CP0 + DIVIDE_RATIO;
   }
   else                                // Interrupt was caused by other bits.
   {
      // Clear other interrupt flags for PCA
      PCA0CN &= ~(PCA0CN_CF__BMASK   |
                  PCA0CN_CCF2__BMASK |
                  PCA0CN_CCF1__BMASK);
   }
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------