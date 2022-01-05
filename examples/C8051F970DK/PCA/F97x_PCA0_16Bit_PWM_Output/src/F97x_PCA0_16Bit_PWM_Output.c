//-----------------------------------------------------------------------------
// F97x_PCA0_8Bit_PWM_Output.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use the PCA to generate a 16-bit PWM waveform
//
// This program sends a PWM waveform out of an I/O pin, using the PCA's
// 16-bit PWM Output Mode.  The duty cycle of the waveform is modified
// periodically within the main loop.
//
// In this example, PCA Module 0 is used to generate the waveform, and the
// crossbar is configured to send the CEX0 pin out on P0.5.
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
//      or pressing F8..
// 5) Observe the triangle wave on LED05.
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
SI_SBIT(LED, SFR_P0, 5);                 // LED=0 means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

SI_INTERRUPT_PROTO(PCA0_ISR, PCA0_IRQn);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint16_t CEX0_Compare_Value;                // Holds current PCA compare value

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
void main(void)
{
  uint16_t delay_count;                    // Used to implement a delay
  bit duty_direction = 0;             // 0 = Decrease; 1 = Increase

  enter_DefaultMode_from_RESET();

  while (1) {
    // Wait a little while
    for (delay_count = 10; delay_count > 0; delay_count--)
      ;

    SFRPAGE = PCA0_PAGE;
    if (duty_direction == 1)         // Increase Duty Cycle
        {
      // First, check the ECOM0 bit
      if ((PCA0CPM0 & PCA0CPM0_ECOM__BMASK) == PCA0CPM0_ECOM__DISABLED) {
        PCA0CPM0 |= PCA0CPM0_ECOM__ENABLED; // Set ECOM0
      } else                          // Increase duty cycle otherwise
      {

        CEX0_Compare_Value--;      // Increase duty cycle

        if (CEX0_Compare_Value == 0x00) {
          duty_direction = 0;     // Change direction for next time
        }
      }
    } else                             // Decrease Duty Cycle
    {
      if (CEX0_Compare_Value == (65536 - 1)) {
        PCA0CPM0 &= ~PCA0CPM0_ECOM__BMASK; // Clear ECOM0
        duty_direction = 1;        // Change direction for next time
      } else {
        CEX0_Compare_Value++;      // Decrease duty cycle
      }
    }
  }
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This is the ISR for the PCA.  It handles the case when a match occurs on
// channel 0, and updates the PCA0CPn compare register with the value held in
// the global variable "CEX0_Compare_Value".
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
  SFRPAGE = PCA0_PAGE;

  PCA0CN &= ~PCA0CN_CCF0__BMASK;      // Clear module 0 interrupt flag.

  // Update Capture/Compare Register
  PCA0CPL0 = (CEX0_Compare_Value & 0x00FF);
  PCA0CPH0 = (CEX0_Compare_Value & 0xFF00) >> 8;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
