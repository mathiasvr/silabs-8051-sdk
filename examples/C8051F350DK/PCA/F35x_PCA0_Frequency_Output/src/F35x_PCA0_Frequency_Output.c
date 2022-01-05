//-----------------------------------------------------------------------------
// F35x_PCA0_Frequency_Output.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// Use the PCA to generate a high frequency signal (MHz)
//
// This program sends a square wave out of an I/O pin, using the PCA's
// Frequency Output Mode.
//
// In this example, PCA Module 0 is used to generate the waveform, and the
// crossbar is configured to send the CEX0 pin out on P0.1. For comparison
// SYSCLK is routed to P0.0.
//
//
//  How To Test:
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
//    -Initial Revision (BD)
//    -11 JAN 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F350_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // Internal oscillator frequency in Hz

#define CEX0_FREQUENCY 50000L          // Frequency to output on CEX0 (Hz)

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void PCA0_Init (void);


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

   PORT_Init ();                       // Initialize crossbar and GPIO
   OSCILLATOR_Init ();                 // Initialize oscillator
   PCA0_Init ();                       // Initialize PCA0

   while (1);
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
// This function configures the PCA time base, and sets up frequency output
// mode for Module 0 (CEX0 pin).
//
// The frequency generated at the CEX0 pin is equal to CEX0_FREQUENCY Hz,
// which is defined in the "Global Constants" section at the beginning of the
// file.
//
// The PCA time base in this example is configured to use SYSCLK / 12.
// The frequency range that can be generated using this example is ~4 kHz to
// ~1 MHz when the processor clock is 24.5 MHz.  Using different PCA clock
// sources or a different processor clock will generate different frequency
// ranges.
//
//    -------------------------------------------------------------------------
//    How "Frequency Output Mode" Works:
//
//       The PCA's Frequency Output Mode works by toggling an output pin every
//    "N" PCA clock cycles.  The value of "N" should be loaded into the PCA0CPH
//    register for the module being used (in this case, module 0).  Whenever
//    the register PCA0L (PCA counter low byte) and the module's PCA0CPL value
//    are equivalent, two things happen:
//
//    1) The port pin associated with the PCA module toggles logic state
//    2) The value stored in PCA0CPH is added to PCA0CPL.
//
//    Using this mode, a square wave is produced at the output port pin of the
//    PCA module. The speed of the waveform can be changed by changing the
//    value of the PCA0CPH register.
//    -------------------------------------------------------------------------
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

   PCA0MD = PCA0MD_CPS__SYSCLK_DIV_12; // Use SYSCLK/12 as time base

   // Module 0 = Frequency Output mode
   PCA0CPM0 = PCA0CPM0_PWM16__8_BIT   |
              PCA0CPM0_ECOM__ENABLED  |
              PCA0CPM0_CAPP__DISABLED |
              PCA0CPM0_CAPN__DISABLED |
              PCA0CPM0_MAT__DISABLED  |
              PCA0CPM0_TOG__ENABLED   |
              PCA0CPM0_PWM__ENABLED   |
              PCA0CPM0_ECCF__DISABLED;

   // Configure frequency for CEX0
   // PCA0CPH0 = (SYSCLK/12)/(2*CEX0_FREQUENCY), where:
   // SYSCLK/12 = PCA time base
   // CEX0_FREQUENCY = desired frequency
   PCA0CPH0 = (SYSCLK/12)/(2*CEX0_FREQUENCY);

   // Start PCA counter
   PCA0CN_CR = 1;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------