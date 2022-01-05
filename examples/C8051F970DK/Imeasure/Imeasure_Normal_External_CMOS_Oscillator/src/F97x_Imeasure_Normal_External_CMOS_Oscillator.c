//-----------------------------------------------------------------------------
// F97x_Imeasure_Normal_External_CMOS_Oscillator.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demostrates the power number of the device when the oscillator
// is using an external CMOS clock.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
// 2) Pull off the JP3 jumper and place a multimeter across, positive side on
//    the pin labeled VDD.
// 3) Connect the USB Debug Adapter to H8.
// 4) Place the VDD SELECT switch (SW1) in the VREG position and power the board.
// 5) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 6) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 7) Measure the supply current using the multimeter.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (TP/JL)
//    - 29 MAY 2014


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h>
#include "Imeasure.h"

//-----------------------------------------------------------------------------
// Function Pointer
//-----------------------------------------------------------------------------

void (*Imeasure_Function_Pointer)(void);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
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
  SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
  PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{

   Port_Init();                        // Initialize Port I/O
   Oscillator_Init ();                 // Initialize Oscillator

   // Infinite Loop For Measuring Current in Normal Mode
   // Note: Ensure that this loop does not bridge a 64-byte flash row
   // boundary. To ensure the while(1) statement is located on a single
   // row, manually assign its location in Flash.
   Imeasure_Function_Pointer = IMEASURE_FUNCTION_LOCATION;
   Imeasure_Function_Pointer();

   while(1);
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
// This function initializes the system clock to use the selected oscillator
// and divide value.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = LEGACY_PAGE;

   REG0CN &= ~REG0CN_OSCBIAS__BMASK;   // Disable precision oscillator bias

   PMU0CF = PMU0CF_CLEAR__ALL_FLAGS;   // Clear all wake-up source flags
                                       // This allows low power oscillator
                                       // to be turned off

   SFRPAGE = CONFIG_PAGE;

   P1MDIN = 0x02;                      // Set XTAL2 as a digital input

   SFRPAGE = LEGACY_PAGE;

   OSCXCN = OSCXCN_XOSCMD__CMOS;

   CLKSEL = CLKSEL_CLKSL__EXTOSC |
            CLKSEL_CLKDIV__SYSCLK_DIV_1;

   // Wait for divider to be applied
   while ((CLKSEL & CLKSEL_CLKRDY__BMASK) == CLKSEL_CLKRDY__NOT_SET);

   SFRPAGE = SFRPAGE_save;
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
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   SFRPAGE = CONFIG_PAGE;

   P0 = 0xFF;                          // Set all I/O to analog inputs
   P1 = 0xFF;                          // to save power
   P2 = 0xFF;
   P3 = 0xFF;
   P4 = 0xFF;
   P5 = 0xFF;

   P0MDIN = 0x00;
   P1MDIN = 0x00;
   P2MDIN = 0x00;
   P3MDIN = 0x00;
   P4MDIN = 0x00;
   P5MDIN = 0x00;

   // Uncomment this section to send SYSCLK out a port pin
   /*
   // SYSCLK to P0.1
   P0SKIP |= 0x01;  // P0.0 skipped
   P0MDIN |= 0x02;  // P0.1 is an output
   P0MDOUT |= 0x02; // P0.1 is push-pull


   XBR0 = XBR0_SYSCKE__ENABLED;            // Put SYSCLK onto Crossbar
   */
   XBR1 = XBR1_XBARE__ENABLED |            // Enable Crossbar and
          XBR1_WEAKPUD__PULL_UPS_DISABLED; // disable Weak pullups.
                                           // Less power consumption with
                                           // crossbar enabled...

   SFRPAGE = LEGACY_PAGE;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------