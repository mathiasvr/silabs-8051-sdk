//-----------------------------------------------------------------------------
// F97x_PCA0_Software_Timer_Blinky.c
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
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) LED05 will blink.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 30 APR 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK          24500000       // SYSCLK frequency in Hz

#define LED_FREQUENCY   5L             // Frequency to blink LED at in Hz
#define T0_CLOCKS       200L           // Use 200 clocks per T0 Overflow

// SYSCLK cycles per interrupt
#define PCA_TIMEOUT ((SYSCLK/T0_CLOCKS)/LED_FREQUENCY/2)

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P0, 5);              // '0' means ON, '1' means OFF

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(PCA0_ISR, PCA0_IRQn);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint16_t Next_Compare_Value;             // Next edge to be sent out in HSO mode

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
// main() Routine
//-----------------------------------------------------------------------------

void main(void)
{
  enter_DefaultMode_from_RESET();
  while (1)
    ;
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
// the global variable "Next_Compare_Value".
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
  uint8_t SFRPAGE_save = SFRPAGE;
  SFRPAGE = PCA0_PAGE;

  if (PCA0CN_CCF0 != 0)                      // If Module 0 caused the interrupt
      {
    PCA0CN_CCF0 = 0;                        // Clear module 0 interrupt flag.

    PCA0CPL0 = (Next_Compare_Value & 0x00FF);
    PCA0CPH0 = (Next_Compare_Value & 0xFF00) >> 8;

    LED = !LED;                             // Toggle LED05

    // Set up the variable for the following edge
    Next_Compare_Value = PCA0CP0 + PCA_TIMEOUT;
  } else // Interrupt was caused by other bits.
  {
    PCA0CN &= ~(PCA0CN_CF__BMASK |       // Clear other interrupt flags for PCA
        PCA0CN_CCF2__BMASK |
        PCA0CN_CCF1__BMASK);
  }

  SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
