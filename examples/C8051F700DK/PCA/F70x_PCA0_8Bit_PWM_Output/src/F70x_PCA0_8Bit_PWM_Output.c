//-----------------------------------------------------------------------------
// F70x_PCA0_8Bit_PWM_Output.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program sends a PWM waveform out of an I/O pin, using the PCA's
// 8-bit PWM Output Mode.  The duty cycle of the waveform is modified
// periodically within the main loop.
//
// In this example, PCA Module 0 is used to generate the waveform, and the
// crossbar is configured to send the CEX0 pin out on P0.0.
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//	  - J15: VDD - power source
// 2) Connect the USB Debug Adapter to P3.
// 3) Connect the USB cable to P4.
// 4) Connect an oscilloscope probe to P0.0.
// 5) Compile and download code to the C8051F70x development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) A waveform should be visible on the oscilloscope. Verify that the duty
//    cycle of the waveform varies smoothly between minimum and maximum values.
//
// Target:         C8051F70x
// Tool chain:     Simplicity Studio / Keil C51 9.53
// Command Line:   None
//
//
// Release 1.0
//    -Initial Revision (PD)
//    -07 JUL 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "si_toolchain.h"
#include <SI_C8051F700_defs.h>         // SFR declaration

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // Internal oscillator frequency in Hz

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
   WDTCN = 0xDE;                       // Disable watchdog timer
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   uint16_t delay_count;                    // Used to implement a delay
   bit duty_direction = 0;             // 0 = Decrease; 1 = Increase


   PORT_Init ();                       // Initialize crossbar and GPIO
   OSCILLATOR_Init ();                 // Initialize oscillator
   PCA0_Init ();                       // Initialize PCA0

   while (1)
   {
      // Wait a little while
      for (delay_count = 10000; delay_count > 0; delay_count--);

      SFRPAGE = LEGACY_PAGE;

      if (duty_direction == 1)         // Direction = Increase
      {
         SFRPAGE = CONFIG_PAGE;
         // First, check the ECOM0 bit
         if ((PCA0CPM0 & 0x40) == 0x00)
         {
            PCA0CPM0 |= 0x40;          // Set ECOM0 if it is '0'
         }
         else                          // Increase duty cycle otherwise
         {
            SFRPAGE = LEGACY_PAGE;

            PCA0CPH0--;                // Increase duty cycle

            if (PCA0CPH0 == 0x00)
            {
               duty_direction = 0;     // Change direction for next time
            }
         }
      }
      else                             // Direction = Decrease
      {
         if (PCA0CPH0 == 0xFF)
         {
            PCA0CPM0 &= ~0x40;         // Clear ECOM0
            duty_direction = 1;        // Change direction for next time
         }
         else
         {
            PCA0CPH0++;                // Decrease duty cycle
         }
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
// This function initializes the system clock to use the internal
// oscillator.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   OSCICN |= 0x80;                     // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock

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
// P0.0   digital   push-pull     PCA0 CEX0
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{

   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   XBR0    = 0x00;           
   XBR1    = 0x41;                     // Enable crossbar and weak pull-ups,
                                       // Route CEX0 to P0.0

   P0MDOUT |= 0x01;                    // Set CEX0 (P0.0) to push-pull

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA time base, and sets up 8-bit PWM output
// mode for Module 0 (CEX0 pin).
//
// The frequency of the PWM signal generated at the CEX0 pin is equal to the
// PCA main timebase frequency divided by 256.
//
// The PCA time base in this example is configured to use SYSCLK, and SYSCLK
// is set up to use the internal oscillator running at 24.5 MHz.  Therefore,
// the frequency of the PWM signal will be 24.5 MHz / 256 = 95.7 kHz.
// Using different PCA clock sources or a different processor clock will
// result in a different frequency for the PWM signal.
//
//    -------------------------------------------------------------------------
//    How "8-Bit PWM Mode" Works:
//
//       The PCA's 8-bit PWM Mode works by setting an output pin low every
//    time the main PCA counter low byte (PCA0L) overflows, and then setting
//    the pin high whenever a specific match condition is met.
//
//    Upon a PCA0L overflow (PCA0L incrementing from 0xFF to 0x00), two things
//    happen:
//
//    1) The CEXn pin will be set low.
//    2) The contents of the PCA0CPHn register for the module are copied into
//       the PCA0CPLn register for the module.
//
//    When the PCA0L register increments and matches the PCA0CPLn register for
//    the selected module, the CEXn pin will be set high, except when the
//    ECOMn bit in PCA0CPMn is cleared to '0'.  By varying the value of the
//    PCA0CPHn register, the duty cycle of the waveform can also be varied.
//
//    When ECOMn = '1', the duty cycle of the PWM waveform is:
//
//       8-bit PWM Duty Cycle = (256 - PCA0CPLn) / 256
//
//    To set the duty cycle to 100%, a value of 0x00 should be loaded into the
//    PCA0CPHn register for the module being used (with ECOMn set to '1').
//    When the value of PCA0CPLn is equal to 0x00, the pin will never be
//    set low.
//
//    To set the duty cycle to 0%, the ECOMn bit in the PCA0CPMn register
//    should be cleared to 0.  This prevents the PCA0CPLn match from occuring,
//    which results in the pin never being set high.
//    -------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   // Configure PCA time base; overflow interrupt disabled
   PCA0CN = 0x00;                      // Stop counter; clear all flags
   PCA0MD = 0x08;                      // Use SYSCLK as time base

   PCA0CPM0 = 0x42;                    // Module 0 = 8-bit PWM mode

   SFRPAGE = LEGACY_PAGE;
   // Configure initial PWM duty cycle = 50%
   PCA0CPH0 = 256 - (256 * 0.5);

   // Start PCA counter
   PCA0CN_CR = 1;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
