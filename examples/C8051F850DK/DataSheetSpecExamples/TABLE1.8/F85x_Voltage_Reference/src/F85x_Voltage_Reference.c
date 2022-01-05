//-----------------------------------------------------------------------------
// F85x_Voltage_Reference.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
//
// Refer to F85x_Voltage_Reference_Readme.txt for more information regarding
// the functionality of this code example.
//
//
// How To Test (Table 1.8 - Output Voltage at 2.4V Setting):
// ---------------------------------------------------------
// 1) Ensure a shorting block is placed across Imeasure (JP2).
// 2) Place VDD Select (SW1) in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) Remove all of the shorting blocks on the board except for the block(s)
//    across the following pins:
//    a) Imeasure on JP2
//    b) DS5 and P1.0 on J27
//    c) DS6 and P1.1 on J27
//    d) S1 and P1.7 on J27
//    e) POT and P1.2 on J27
// 6) Adjust the potentiometer so that the measured voltage at P1.2 on J25 is
//    as close to GND as possible. Please note that the voltage at this pin may
//    not display as 0 due to a 1k resistor placed between P1.2 and the wiper
//    of the potentiometer.
// 7) Adjust the potentiometer up from GND until DS5 turns on. DS5 will turn on
//    when the ADC reaches the maximum 10 bit value (0x03FF).
// 8) Measure the voltage at P1.2 on J25. The voltage should read between the
//    minimum and maximum values provided on the datasheet for the 2.4V setting
//    of the internal voltage reference in Table 1.8.
//
//
// How To Test (Table 1.8 - Output Voltage at 1.65V Setting):
// ----------------------------------------------------------
// 1) Ensure a shorting block is placed across Imeasure (JP2).
// 2) Place VDD Select (SW1) in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) Remove all of the shorting blocks on the board except for the block(s)
//    across the following pins:
//    a) Imeasure on JP2
//    b) DS5 and P1.0 on J27
//    c) DS6 and P1.1 on J27
//    d) S1 and P1.7 on J27
//    e) POT and P1.2 on J27
// 6) Adjust the potentiometer so that the measured voltage at P1.2 on J25 is
//    as close to GND as possible. Please note that the voltage at this pin may
//    not display as 0 due to a 1k resistor placed between P1.2 and the wiper
//    of the potentiometer.
// 7) Use the switch at P1.7 (S1) to set the internal ADC reference level to
//    1.65V. DS6 should turn on indicating the reference voltage level has been
//    successfully set to 1.65V.
// 8) Adjust the potentiometer up from GND until DS5 turns on. DS5 will turn on
//    when the ADC reaches the maximum 10 bit value (0x03FF).
// 9) Measure the voltage at P1.2 on J25. The voltage should read between the
//    minimum and maximum values provided on the datasheet for the 1.65V setting
//    of the internal voltage reference in Table 1.8.
//
//
// Target:         C8051F85x
// Target Board:   UPMU-F850 MCU-100
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 5 FEB 2014
//
// Release 1.0
//    - Initial Revision (BGD)
//    - 16 JUL 2013
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

SI_SBIT(S1, SFR_P1, 7);                   // Switch 1
SI_SBIT(DS5, SFR_P1, 0);
SI_SBIT(DS6, SFR_P1, 1);

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
   ADC0_Init ();                       // Initialize ADC0
   Timer2_Init ();                     // Initialize Timer 2

   IE_EA = 1;                             // Enable global interrupts

   // Loop forever
   while (1)
   {
      // If the ADC is at a max reading (10 bit)
      if(ADC0 == 0x03FF)
         DS5 = 0;                      // Turn ON DS5
      else
         DS5 = 1;                      // Turn OFF DS5

      // If the internal ADC reference level is set to 1.65V
      if((REF0CN & 0x80) == 0x00)
         DS6 = 0;                      // Turn ON DS6
      else
         DS6 = 1;                      // Turn OFF DS6
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// This routine initializes the ADC to take 10-bit measurements at a rate of
// 800 ksps using the internal reference. The reference level is set to 2.4V
// default but can be set to 1.65V by pressing S1.
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   ADC0CN0 = 0x02;                     // ADC0 disabled, conversions triggered
                                       // on Timer2 overflows

   ADC0CN1 |= 0x01;                    // Enable common mode buffer

   ADC0CF = ((SYSCLK/12250000)-1)<<3;  // Set SAR clock to 12.25 MHz

   ADC0CF |= 0x01;                     // 1x gain, normal tracking mode,
                                       // 10-bit mode

   ADC0AC = 0x00;                      // 12-bit mode disabled, accumulation
                                       // disabled, right-justified, repeat
                                       // count set to 1

   ADC0PWR = 0x00;                     // Low power mode disabled

   ADC0MX = 0x0A;                      // ADC0 positive input = ADC0.10 or
                                       // P1.2

   REF0CN = 0x98;                      // Ground reference is the GND pin,
                                       // voltage reference is the internal
                                       // reference at 2.4V, temperature
                                       // sensor disabled

   ADC0CN0_ADEN = 1;                           // Enable ADC0
}

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
// P1.0  digital  push-pull   DS5
// P1.1  digital  push-pull   DS5
// P1.2  analog               ADC0.10
// P1.7  digital  open-drain  S1 (Switch 1)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // Analog Pin Setup
   P1MDIN &= ~0x04;                    // Set P1.2 as an analog input
   P1 |= 0x04;                         // Set the P1.2 latch to high
   P1SKIP |= 0x04;                     // Skip P1.2 on crossbar

   // Digital Pin Setup
   P1MDIN |= 0x83;                     // Set P1.0, P1.1, & P1.7 as digital
   P1MDOUT &= ~0x80;                   // Set P1.7 as open-drain
   P1MDOUT |= 0x03;                    // Set P1.0 and P1.1 as push-pull
   P1 |= 0x83;                         // Set P1.0, P1.1, & P1.7 latch to high

   P1MAT |= 0x80;                      // Set up the port-match value on P1.7
   P1MASK |= 0x80;                     // An event will occur if
                                       // (P1 & P1MASK) != (P1MAT & P1MASK)
                                       // The switch pins are 1 if the switch
                                       // isn't pressed.

   EIE1 |= 0x02;                       // Enable Port Match interrupts

   XBR2 =  0x40;                       // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    uint16_t ADC0_Sampling_Rate
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 1.25 us
// intervals. Timer 2 overflow automatically triggers ADC0 conversion.
//
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
                                       // Use SYSCLK as time base, 16-bit
                                       // auto-reload
   CKCON |= 0x10;                      // Select SYSCLK for timer 2 source
   TMR2RL = -((SYSCLK/800000) + 1);    // Set reload for ADC0 sampling rate
   TMR2 = 0xFFFF;                      // Set to reload immediately
   TMR2CN_TR2 = 1;                            // Start Timer2
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port Match ISR
//-----------------------------------------------------------------------------
//
// Whenever S1 (P1.7) is pressed, the internal ADC voltage reference level is
// toggled between 2.4V and 1.65V.
//
// The Port Match interrupt occurs whenever (Pn & PnMASK) != (PnMAT & PnMASK).
// We must disable the interrupt and wait until the switches are released in
// main, or else the interrupt will trigger multiple times.  It is generally
// not good practice to sit in an ISR for int32_t periods of time.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PM_ISR, PMATCH_IRQn)
{
   int16_t i;                              // For loop variable initialization

   if(S1 == SW_PRESSED)
   {
      REF0CN ^=  0x80;                 // Toggle internal ADC voltage reference
                                       // level between 2.4V and 1.65V

      P1MAT &= ~0x80;                  // Set port match to trigger at switch
                                       // rising edge to prevent multiple
                                       // interrupts from occurring while the
                                       // switch is pressed
   }
   else
   {
      P1MAT |= 0x80;                   // Reset port match to trigger at switch
                                       // falling edge

      for(i = 0; i < 10000; i++);      // Delay for debouncing
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
