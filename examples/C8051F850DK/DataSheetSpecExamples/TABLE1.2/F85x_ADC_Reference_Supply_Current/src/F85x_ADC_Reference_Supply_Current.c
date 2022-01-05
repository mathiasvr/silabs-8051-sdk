//-----------------------------------------------------------------------------
// F85x_ADC_Reference_Supply_Current.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
//
// Refer to F85x_ADC_Reference_Supply_Current_Readme.txt for more information
// regarding the functionality of this code example.
//
//
// How To Measure (Table 1.2 - Normal Power Mode):
// -----------------------------------------------
// 1) Ensure a shorting block is placed across Imeasure (JP2).
// 2) Place VDD Select (SW1) in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) Remove all shorting blocks on the board except for the block(s) across
//    the following pins:
//    a) S1 and P1.7 on J27
//    b) S2 and P2.1 on J27
// 6) Measure the current across Imeasure. This reading is the current with
//    the internal reference enabled.
// 7) Use the switch at P1.7 (S1) to disable the internal reference.
// 8) Measure the current across Imeasure and subtract the value from the
//    previous current reading. Verify the difference is within the range of
//    values listed on the datasheet for Internal ADC0 Reference Normal Power
//    Mode.
//
//
// How To Measure (Table 1.2 - Low Power Mode):
// --------------------------------------------
// 1) Ensure a shorting block is placed across Imeasure (JP2).
// 2) Place VDD Select (SW1) in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) Remove all shorting blocks on the board except for the block(s) across
//    the following pins:
//    a) S1 and P1.7 on J27
//    b) S2 and P2.1 on J27
// 6) Use the switch at P2.1 (S2) to set the internal ADC reference to a low
//    power mode.
// 7) Measure the current across Imeasure. This reading is the current with
//    the internal reference enabled.
// 8) Use the switch at P1.7 (S1) to disable the internal reference.
// 9) Measure the current across Imeasure and subtract the value from the
//    previous current reading. Verify the difference is within the range of
//    values listed on the datasheet for Internal ADC0 Reference Low Power
//    Mode.
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
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000    // SYSCLK frequency in Hz

#define SW_PRESSED         0
#define SW_NOT_PRESSED     1

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(S1, SFR_P1, 7);
SI_SBIT(S2, SFR_P2, 1);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(PM_ISR, PMATCH_IRQn);

void ADC0_Init (void);
void SYSCLK_Init (void);
void Port_Init (void);
void Timer2_Init (void);

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

   SYSCLK_Init ();                     // Initialize system clock to 24.5 MHz
   Port_Init ();                       // Initialize crossbar and GPIO
   Timer2_Init ();                     // Initialize Timer 2
   ADC0_Init ();                       // Initialize ADC0

   IE_EA = 1;                             // Enable global interrupts

   // Loop forever
   while (1)
   {
      int16_t i;                           // For loop variable initializer

      if (S2 == SW_PRESSED)
      {
         ADC0PWR ^= 0x20;              // Invert internal ADC reference low
                                       // power bit

         while (S2 == SW_PRESSED);     // Block until switch is let go to
                                       // prevent multiple events from occurring
                                       // while the switch is pressed

         for(i = 0; i < 10000; i++);   // Debouncing delay
      }
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator divided by 1 as its clock source.  Also enables missing clock
// detector reset.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.
   CLKSEL = 0x00;                      // Set system clock to 24.5 MHz
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the Crossbar and GPIO ports.
//
// P1.7  digital  open-drain  S1 (Switch 1)
// P2.1  digital  open-drain  S2 (Switch 2)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // Port 1 Initialization
   P1MDIN |= 0x80;                     // Set P1.7 as digital
   P1MDOUT &= ~0x80;                   // Set P1.7 as open-drain
   P1 |= 0x80;                         // Set the P1.7 latch to '1'

   P1MAT |= 0x80;                      // Set up the port-match value on P1.7
   P1MASK |= 0x80;                     // An event will occur if
                                       // (P1 & P1MASK) != (P1MAT & P1MASK)
                                       // The switch pins are 1 if the switch
                                       // isn't pressed.

   // Port 2 Initialization
   P2MDOUT &= ~0x02;                   // Set P2.1 as open-drain
   P2 |= 0x02;                         // Set the P2.1 latch to '1'

   EIE1 |= 0x02;                       // Enable Port Match interrupts

   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 1.25 us
// intervals. Timer 2 overflow automatically triggers ADC0 conversion.
//
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
                                       // Use SYSCLK as timebase, 16-bit
                                       // auto-reload
   CKCON |= 0x10;                      // Select SYSCLK for timer 2 source
   TMR2RL = -((SYSCLK/250000) + 1);    // Set reload for 250 ksps
   TMR2 = 0xFFFF;                      // Set to reload immediately
   TMR2CN_TR2 = 1;                            // Start Timer2
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures ADC0 to make single-ended analog measurements on GND.
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   ADC0CN0 = 0x02;                     // ADC0 disabled, conversions triggered
                                       // on Timer2 overflows

   ADC0CF = ((SYSCLK/4000000)-1)<<3;   // Set SAR clock to 4 MHz

   ADC0CF |= 0x01;                     // 1x gain, normal tracking mode,
                                       // 10-bit mode

   ADC0AC = 0x00;                      // 12-bit mode disabled, accumulation
                                       // disabled, right-justified, repeat
                                       // count set to 1

   ADC0PWR = 0xC0;                     // Disable Low Power settings, set bias
                                       // current to mode 3

   ADC0MX = 0x13;                      // ADC0 positive input = GND

   REF0CN = 0x98;                      // Ground reference is the GND pin,
                                       // voltage reference is the internal
                                       // reference set to 2.4V, temperature
                                       // sensor disabled

   ADC0CN0_ADEN = 1;                           // Enable ADC0
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port Match ISR
//-----------------------------------------------------------------------------
//
// Whenever S1 (P1.7) is pressed, the ADC voltage reference is toggled between
// the internal reference and VDD.
//
// The Port Match interrupt occurs whenever (Pn & PnMASK) != (PnMAT & PnMASK).
// We must disable the interrupt and wait until the switches are released in
// main, or else the interrupt will trigger multiple times.  It is generally
// not good practice to sit in an ISR for int32_t periods of time.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PM_ISR, PMATCH_IRQn)
{
   int16_t i;                              // For loop variable initializer

   // Do not allow S1 events while S2 is pressed
   if(S2 != SW_PRESSED)
   {
      if(S1 == SW_PRESSED)
      {
         REF0CN ^= 0x10;               // Toggle voltage reference selection
                                       // between VDD and internal reference

         P1MAT &= ~0x80;               // Set port match to trigger at switch
                                       // rising edge to prevent multiple
                                       // interrupts from occurring while the
                                       // switch is pressed
      }
      else
      {
         P1MAT |= 0x80;                // Reset port match to trigger at switch
                                       // falling edge

         for(i = 0; i < 10000; i++);   // Debouncing delay
      }
   }

}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
