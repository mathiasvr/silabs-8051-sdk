//-----------------------------------------------------------------------------
// F36x_Oscillator_Capacitor.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to configure the oscillator for use with an
// external capacitor.
//
//
// How To Test:
// ------------
// 1) Populate a 33 pF capacitor (C19)  between P0.6 (XTAL2) and GND and
//    specify the desired XFCN setting by modifying the constant <XFCN>.
// 2) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J1: Power selection, P2 9V, P4 USB or J9 Debugger
// 3) Connect the USB Debug Adapter to J9. If 9V power is selected on J1,
//    connect the 9V power adapter to P2. If VBUS is selected on J1, connect
//    USB cable to P4.
// 4) Compile and download code to C8051F360 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 6) Place a scope probe on P0.0 and observe the frequency of SYSCLK output.
//
// The oscillation frequency and the required External Oscillator Frequency
// Control value (XFCN) can be determined by the following equation:
//
//       Clock frequency (MHz) = KF(XFCN) / (C * VDD)
//
//       where
//       KF(XFCN) is a factor based on XFCN.
//       C = capacitor value on the XTAL2 pin in pF
//       Vdd = power supply voltage in Volts
//
//       XFCN | K Factor
//       ---------------
//         0  |    0.87
//         1  |    2.6
//         2  |    7.7
//         3  |   22
//         4  |   65
//         5  |  180
//         6  |  664
//         7  | 1590
//
// The actual frequency is best determined by measurement. In this example,
// a 33pF capacitor was used with VDD = 3.3V. The calculated frequency is
// 202 kHz. The actual measured SYSCLK frequency on P0.0 was 134 kHz for this
// example when XFCN is set to 3.
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
//    -Initial Revision (FB / TP)
//    -13 OCT 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
// XFCN Setting Macro
#define XFCN      3

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void ExtCapOsc_Init (void);
void Port_Init (void);

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
   PCA0MD  &= ~PCA0MD_WDTE__ENABLED;   // Disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   Port_Init();                        // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   ExtCapOsc_Init ();                  // Initialize External Capacitor
                                       // Oscillator
   SFRPAGE = LEGACY_PAGE;

   while (1) {};                       // Loop forever
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
// This function configures the crossbar and ports pins.
//
// The oscillator pins should be configured as analog inputs when using the
// external oscillator in capacitor mode.
//
// P0.0   digital    push-pull     /SYSCLK
// P0.6   analog     don't care    XTAL2
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // Oscillator Pins
   P0MDIN &= ~0x40;                    // P0.6 is analog
   P0SKIP |= 0x40;                     // P0.6 skipped in the Crossbar

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0    = XBR0_SYSCKE__ENABLED;     // Route /SYSCLK to first available pin
   XBR1    = XBR1_XBARE__ENABLED;      // Enable Crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   OSCICN |= OSCICN_IFCN__HFOSC_DIV_1; // Initialize internal oscillator to
                                       // highest frequency

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// ExtCapOsc_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes for and switches to an external capacitor for the
// External Oscillator.
//
//-----------------------------------------------------------------------------
void ExtCapOsc_Init (void)
{
   uint8_t i;
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   OSCXCN = OSCXCN_XOSCMD__C;          // External Oscillator is an external
                                       // capacitor

   OSCXCN |= XFCN;

   // RC and C modes typically require no startup time.

   for (i = 0; i < 255; i++);          // Wait for C osc. to start

   CLKSEL = CLKSEL_CLKSL__EXTOSC;      // Select external oscillator as system
                                       // clock source

   OSCICN = OSCICN_IOSCEN__DISABLED;   // Disable the internal oscillator.

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------