//-----------------------------------------------------------------------------
// F36x_Oscillator_PLL.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to configure the internal PLL in the 'F36x
// to produce a 98 MHz clock derived from the internal oscillator.
//
// Pinout:
//
//    P0.0 - SYSCLK
//
//    all other port pins unused
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: P3.0 - SW3.0
//    J12: P3.2 - P3.2_LED
//    J4:  P2.0 - J5: P3.0
//    J1: Power selection, P2 9V, P4 USB or J9 Debugger
// 2) Connect the USB Debug Adapter to J9. If 9V power is selected on J1,
//    connect the 9V power adapter to P2. If VBUS is selected on J1, connect
//    USB cable to P4.
// 3) Compile and download code to C8051F360 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) Measure the frequency output on P0.0. The frequency should be 98 MHz.
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
//    -Initial Revision (TP)
//    -26 OCT 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK  98000000               // SYSCLK frequency in Hz

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void PLL_Init (void);
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

   Oscillator_Init ();                 // Initialize Oscillator
   PLL_Init ();
   Port_Init ();                       // Initialize Port I/O

   SFRPAGE = LEGACY_PAGE;

   while (1) {};                       // Loop forever

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
// This function initializes the internal oscillator to its maximum frequency
// of 24.5 MHz.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   OSCICN |= OSCICN_IFCN__HFOSC_DIV_1;

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// PLL_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the external oscillator
// in RC mode.
//
//-----------------------------------------------------------------------------
void PLL_Init (void)
{
   int8_t i;
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // Step 2. Set the PLLSRC bit (PLL0CN.2) to select the desired
   // clock source for the PLL.
   PLL0CN &= ~PLL0CN_PLLSRC__BMASK;    // Internal oscillator

   // Step 3. Program the Flash read timing bits, FLRT (FLSCL.5-4) to the
   // appropriate value for the new clock rate (see Section 15. Flash Memory
   // on page 199).
   SFRPAGE = LEGACY_PAGE;
   FLSCL |= FLSCL_FLRT__SYSCLK_BELOW_100_MHZ;// >= 100 MHz
   SFRPAGE = CONFIG_PAGE;

   // Step 4. Enable power to the PLL by setting PLLPWR (PLL0CN.0) to ‘1’.
   PLL0CN |= PLL0CN_PLLPWR__ENABLED;

   // Step 5. Program the PLL0DIV register to produce the divided reference
   // frequency to the PLL.
   PLL0DIV = 0x01;

   // Step 6. Program the PLLLP3-0 bits (PLL0FLT.3-0) to the appropriate
   // range for the divided reference frequency.
   PLL0FLT |= 0x01;

   // Step 7. Program the PLLICO1–0 bits (PLL0FLT.5–4) to the appropriate
   // range for the PLL output frequency.
   PLL0FLT &= ~0x30;

   // Step 8. Program the PLL0MUL register to the desired clock multiplication
   // factor.
   PLL0MUL = 0x04;

   // Step 9. Wait at least 5 µs, to provide a fast frequency lock.
   for (i = 100; i > 0; i--);

   // Step 10. Enable the PLL by setting PLLEN (PLL0CN.1) to ‘1’.
   PLL0CN |= PLL0CN_PLLEN__ENABLED;

   // Step 11. Poll PLLLCK (PLL0CN.4) until it changes from ‘0’ to ‘1’.
   while ((PLL0CN & PLL0CN_PLLLCK__BMASK) != PLL0CN_PLLLCK__LOCKED);

   // Step 12. Switch the System Clock source to the PLL using the CLKSEL
   // register.
   CLKSEL = CLKSEL_CLKSL__PLL;

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// Pinout:
//
// P0.0   digital    push-pull     /SYSCLK
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0    = XBR0_SYSCKE__ENABLED;     // Route /SYSCLK to first available pin
   XBR1    = XBR1_XBARE__ENABLED;      // Enable Crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------