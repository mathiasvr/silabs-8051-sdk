//-----------------------------------------------------------------------------
// F85x_LF_Oscillator.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
//
// Refer to F85x_LF_Oscillator_Readme.txt for more information regarding the
// functionality of this code example.
//
//
// How To Measure (Table 1.2 - Low Frequency Oscillator Current):
// --------------------------------------------------------------
// 1) Ensure a shorting block is placed across Imeasure (JP2).
// 2) Place VDD Select (SW1) in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Comment the line #define SYSCLK_OUTPUT_ENABLE under the section labeled
//    "Global Constants".
// 4) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Remove all shorting blocks on the board except for the block(s) across
//    the following pins:
//    a) S1 and P1.7 on J27
// 7) Measure the current across Imeasure. This reading is the current with the
//    low frequency oscillator enabled.
// 8) Use switch at P1.7 (S1) to disable the low frequency oscillator.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. The difference should be close to the typical value
//    listed on the datasheet for Low Frequency Oscillator under Table 1.2.
//
//
// How To Measure (Table 1.5 - Low Frequency Oscillator Frequency):
// ----------------------------------------------------------------
// 1) Ensure a shorting block is placed across Imeasure (JP2).
// 2) Place VDD Select (SW1) in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Uncomment the line #define SYSCLK_OUTPUT_ENABLE under the section labeled
//    "Global Constants".
// 4) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
//    NOTE: A warning may appear stating Enter_Idle() is an uncalled segment.
//    This warning is expected and may be safely ignored as Enter_Idle() is
//    not used in the following test.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Using the IDE, stop the program and disconnect the board.
// 7) Remove any and all power sources to the board including the debugger.
// 8) Connect a power supply to VDD (TP5) set to 3.6V.
// 9) Use an oscilloscope to monitor P0.0. The low frequency oscillator is
//    routed to this pin for this program.
// 10) Sweep the supply voltage down to 2.2V. During the sweep, the frequency
//    of the low frequency oscillator should remain within the limits listed
//    in Table 1.5. Verify with the oscilloscope.
//
//
// Target:        C8051F85x
// Target Board:   UPMU-F850 MCU-100
// Tool chain:    Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 5 FEB 2014
//
// Release 1.0
//    - Initial Revision (BGD)
//    - 17 JUL 2013
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(S1, SFR_P1, 7);

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SW_PRESSED         0
#define SW_NOT_PRESSED     1

// Uncomment the following #define statement to route the low frequency
// oscillator to an external pin. Leave commented when testing the power
// consumption specifications as enabling this settings will skew test results.
#define SYSCLK_OUTPUT_ENABLE

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(PM_ISR, PMATCH_IRQn);

void Port_Init (void);
void Oscillator_Init (void);
void Enter_Idle (void);

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
   WDTCN = 0xDE;
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   // Disable the watchdog timer

   Port_Init ();                       // Initialize Port I/O
   Oscillator_Init ();                 // Initialize Oscillator

   // If sysclk output is enabled
   #ifdef SYSCLK_OUTPUT_ENABLE

      // Loop forever
      while(1) {}

   // If sysclk output is disabled
   #else

      IE_EA = 1;                          // Enable global interrupts

      // Loop forever
      while (1)
      {
         while(S1 == SW_PRESSED);      // Block until switch is not pressed

         Enter_Idle();                 // Enter idle mode
                                       // Exit on port match interrupt

         OSCLCN ^= 0x80;               // Toggle LFOSC enable

         // Poll until the LFOSC is ready
         while ((OSCLCN & 0x40) != 0x40);

      }

   #endif
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P1.7  digital  open-drain  S1 (Switch 1)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // If sysclk output is enabled
   #ifdef SYSCLK_OUTPUT_ENABLE

      P0MDIN |= 0x01;                  // Set P0.0 as digital
      P0MDOUT |= 0x01;                 // Set P0.0 to push-pull

      XBR0 = 0x80;                     // Enable SYSCLK on the crossbar

   // If sysclk output is disabled
   #else

      P1MDIN |= 0x80;                  // Set P1.7 as digital
      P1MDOUT &= ~0x80;                // Set P1.7 as open-drain
      P1 |= 0x80;                      // Set the P1.7 latch to '1'

      P1MAT |= 0x80;                   // Set up the port-match value on P1.7
      P1MASK |= 0x80;                  // An event will occur if
                                       // (P1 & P1MASK) != (P1MAT & P1MASK)
                                       // The switch pins are 1 if the switch
                                       // isn't pressed.

      EIE1 |= 0x02;                    // Enable Port Match interrupts

   #endif

   XBR2 = 0x40;                        // Enable crossbar and enable
                                       // weak pull-ups
}

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to high frequency oscillator
// divided by 128 and enables the missing clock detector. This function also
// initializes the low frequency oscillator.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.
   OSCLCN |= 0x83;                     // Enable the LFOSC and divide by 1

   // If sysclk output is enabled
   #ifdef SYSCLK_OUTPUT_ENABLE

      CLKSEL = 0x02;                   // Select low frequency oscillator as
                                       // the system clock

   // If sysclk output is disabled
   #else

      CLKSEL = 0x70;                   // Select internal high frequency osc.
                                       // divided by 128 as the system clock

   #endif
}

//-----------------------------------------------------------------------------
// Runtime Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Enter_Idle
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Puts the processor into sleep mode, in order to allow the user to measure
// idle mode current draw. Sleep mode ends when SW1 (P1.7) is pressed.
//
//-----------------------------------------------------------------------------
void Enter_Idle (void)
{
   uint8_t IE_save;
   uint8_t EIE1_save;

   IE_EA = 0;                             // Clear global interrupt enable

   // Save interrupt state
   IE_save = IE;                     // Preserve IE register
   EIE1_save = EIE1;                   // Preserve EIE1 register

   // Disable all interrupts
   IE = 0x00;                        // Disable interrupts
   EIE1 = 0x02;                        // Enable port match interrupts
   IE_EA = 1;                           // Enable global interrupts

   // Go into idle mode
   PCON |= 0x01;                       // Set Idle mode
   PCON = PCON;                        // Follow the instruction to enter idle
                                       // with a 3 cycle dummy instruction

   //--wake on port match event--//

   EIE1 = EIE1_save;                   // Restore EIE1 register
   IE = IE_save;                       // Restore IE register
   IE_EA = 1;                             // Set global interrupt enable

}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port Match ISR
//-----------------------------------------------------------------------------
//
// Whenever S1 (P1.7) is pressed, wake up from Idle to toggle the LFOSC enable.
//
// The Port Match interrupt occurs whenever (Pn & PnMASK) != (PnMAT & PnMASK).
// We must disable the interrupt and wait until the switches are released in
// main, or else the interrupt will trigger multiple times.  It is generally
// not good practice to sit in an ISR for int32_t periods of time.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PM_ISR, PMATCH_IRQn)
{
   EIE1 &= ~0x02;                      // Disable Port Match interrupts to
                                       // prevent multiple interrupts from
                                       // occurring while the switches are
                                       // pressed
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
