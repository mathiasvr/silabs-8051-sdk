//-----------------------------------------------------------------------------
// F93x_Voltage_Reference.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the internal Voltage Reference's electrical
// characteristics as specified on table 4.11 of the datasheet.
//
// Refer to F93x_Voltage_Reference_Readme.txt for more information regarding
// the functionality of this code example.
//
//
// How To Test (Table 4.11 - Output Voltage):
// ---------------------------------------------------------
// 1) Ensure a shorting block is placed the J11 (VBAT & WALL_PWR) .
// 2) Place VDD Select (2 CELL) in the SW4 position and power the board
//    using the P2 power connector.
// 3) Download and run the code on C8051F930-TB target board.
// 4) Remove all of the shorting blocks on the board except for the block(s)
//    across the following pins:
//    a) Imeasure on J17
//    b) LED1.5 and P1.5 on J8
//    c) LED1.6 and P1.6 on J8
//    d) SW0.2 and P0.2H on J8
//    e) SW0.3 and P0.3H on J8
//    f) P0.6 on J16
//    g) GND on J15
// 5) Adjust the potentiometer so that the measured voltage at P0.6 on J2 is
//    as close to GND as possible.
// 6) Adjust the potentiometer up from GND until DS4 turns on. DS4 will turn on
//    when the ADC reaches the maximum 10 bit value (0x03FF).
// 7) Measure the voltage at P0.6 on J2. The voltage should read between the
//    minimum and maximum values provided on the datasheet for the
//    internal voltage reference in Table 4.11.
// 8) To cycle between Reference mode, push SW0.2 (P0.2).
//    default is High Speed Reference 1.65V mode. DS3 will turn on when
//    Precision Reference 1.68V mode be selected.
//
//
// How To Test (Table 4.11 - Supply Current):
// ----------------------------------------------------------
// 1) Ensure that shorting blocks are attached to the J11(VBAT & WALL_PWR)and
//    J8(P0.2H & SW0.2, P0.3H & SW0.3)headers.
// 2) Remove the J17 shorting block and attach an ammeter between
//    the Imeasure headers.
// 3) Place VDD Select 2 CELL in the SW4 position and power the board
//    using the P2 power connector.
// 4) Download and run the code on the target board.
// 5) Remove all of the shorting blocks on the board.
// 6) Default is High Speed Reference 1.65V enabled .
// 7) Record the current then push SW0.3 (P0.3) to diable Reference.
// 8) Read the new current to subtract previous current,
//    the difference is the High Speed Reference current.
// 9) Push SW0.2 (P0.2).toggle Reference to Precision mode.
// 10) Record the current then push SW0.3 (P0.3) to diable Reference.
// 11) Read the new current to subtract previous current,
//     the difference is the Precision Reference current.
//
//
//
// Target:         C8051F93x/92x
// Tool Chain:     Generic
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (MT)
//    - 10 OCT 2013
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F930_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK          20000000        // SYSCLK frequency in Hz
#define SARCLK          7000000         // SARCLK frequency in Hz

#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

#define LED_ON              0
#define LED_OFF             1
// Mode type enumerations
enum REFERENCE_MODE {HIGH_SPEED = 1,
                     PRECISION  = 2,};
						
enum REFERENCE_MODE referenceMode;
//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(DS3, SFR_P1, 5);
SI_SBIT(DS4, SFR_P1, 6);
SI_SBIT(SW0_2, SFR_P0, 2);
SI_SBIT(SW0_3, SFR_P0, 3);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void ADC0_Init (void);
void SYSCLK_Init (void);
void Port_Init (void);
void Timer2_Init (void);
void Port_Update(void);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Debounce latches for SW0.2 and SW0.3
uint8_t switch1Latch = 0;
uint8_t switch2Latch = 0;

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
    PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
                                        // enable)

    SYSCLK_Init ();                     // Initialize system clock to 24.5 MHz
    Port_Init ();                       // Initialize crossbar and GPIO
    ADC0_Init ();                       // Initialize ADC0
    Timer2_Init ();                     // Initialize Timer 2

    IE_EA = 1;                          // Enable global interrupts
    referenceMode = HIGH_SPEED;
    // Loop forever
    while (1)
    {
        // Detect switch inputs and adjust modes
        Port_Update();
        // If the ADC is at a max reading (10 bit)
        if(ADC0 == 0x03FF)
        {
            DS4 = LED_ON;               // Turn ON DS4
        }
        else
        {
            DS4 = LED_OFF;              // Turn OFF DS4
        }

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
// Configures ADC0 10-Bit configuration mode to make single-ended analog
// measurements on the P1.4.
//
// P1.4   analog   I/O   (ADC Input)
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
    ADC0CF = ((SYSCLK/SARCLK)-1)<<3;    // Set SAR clock to 7 MHz
    ADC0CF |= 0x01;                     // 1x gain, normal tracking mode,
                                        // 10-bit mode

    ADC0AC = 0x00;                      // No right-justified , repeat
                                        // count set to 1
    ADC0MX = 0x06;                      // ADC0 positive input P0.6

    REF0CN = 0x18;                      // Use Internal Reference

    ADC0CN = 0x82;                      // Enable ADC0
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// This routine initializes the system clock to use the internal Low Power
// 20 MHz oscillator divided by 1 as its clock source.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
    // Select Low-Power Oscillator 20 MHz as system clock source
    // Since using the precision internal oscillator as the system clock source,
    // the precision voltage reference should not be enabled from a disabled
    // state.
    CLKSEL = 0x04;
    OSCICN &= ~0x80;
    REG0CN &=~0x10;

    // At clock frequencies above 10 MHz, the system clock cycle becomes
    // short enough that the one-shot timer no longer provides a power benefit.
    // Disabling the one-shot timer at higher frequencies reduces power
    // consumption.
    FLSCL = 0x40;
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
// P1.5  digital  push-pull   output   DS3
// P1.6  digital  push-pull   output   DS4
// P0.0  analog               input    ADC vref
// P0.6  analog               input    ADC input
// P0.2  digital  open-drain  input    SW0.2
// P0.3  digital  open-drain  input    SW0.3
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    // Analog Pin Setup
    P0MDIN  &= ~0x41;                   // Set P0.0, P0.6 as an analog input
    P0      |=  0x41;                   // Set the P0.0, P0.6 latch to high
    P0SKIP  |=  0x41;                   // Skip P0.0, P0.6 on crossbar

    // Digital Pin Setup
    P1MDIN  |=  0x60;                   // Set P1.5, P1.6 as digital
    P1MDOUT |=  0x60;                   // Set P1.5, P1.6 as push-pull
    P1      |=  0x60;                   // Set P1.5, P1.6 latch to high

    P0MDIN  |=  0x0C;                   // Set P0.2, P0.3 as digital
    P0MDOUT &=  0x0C;                   // Set P0.2  P0.3 as open-drain

    XBR2     =  0x40;                   // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Port_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function checks switch input and cycles through Voltage Reference
// modes and ON/OFF state, once each time the switch is pressed.
// The cycling order is as follows:
//
// Switch Sw0_2:
// Mode 1: High Speed Reference 1.65 V
// Mode 2: Precision Reference 1.68 V
//
// Switch SW0_3:
// Toggle Reference ON / OFF
//
// P0.2   digital   open-drain   input   SW0_2
// P0.3   digital   open-drain   input   SW0_3
//
//-----------------------------------------------------------------------------
void Port_Update(void)
{
    // Cycle Reference output mode if P0.2 is pushed
    if( (SW0_2 == SW_PRESSED) && (!switch1Latch) )
    {
        switch (referenceMode)
        {
            case HIGH_SPEED:
                referenceMode = PRECISION;

                // Select internal 1.65 V high speed voltage reference.
                REF0CN = 0x01;
                DS3 = LED_ON;               // Turn ON DS3
                break;

            case PRECISION:
                referenceMode = HIGH_SPEED;

                // Precision Voltage Reference enabled and connected to
                // P0.0/VREF.
                REF0CN = 0x18;
                DS3 = LED_OFF;               // Turn OFF DS3
                break;
        }
        ADC0CN_ADINT = 0;
        ADC0 = 0;
        // Arm latches to prevent auto-cycling with switch held
        switch1Latch = 1;
    }

    // Reset latch once switch is released
    else if(SW0_2 == SW_NOT_PRESSED)
    {
        switch1Latch = 0;
    }

    // Cycle Reference ON / OFF if P0.3 is pushed
    if( (SW0_3 == SW_PRESSED) && (!switch2Latch) )
    {
        // Toggle SW0_3 between push-pull on/off
        // Reference between on/off
        if (referenceMode == HIGH_SPEED)
        {
            if(REF0CN & 0x18)
            {
                REF0CN = 0;
            }
            else
            {
                // Select internal 1.65 V high speed voltage reference.
                REF0CN = 0x18;
            }
        }
        else
        {
            // Reference in Precision Mode
            if(REF0CN & 0x01)
            {
                REF0CN = 0;
            }
            else
            {
                // Precision Voltage Reference enabled and connected to
                // P0.0/VREF.
                REF0CN = 0x01;
            }
        }

        // Arm latches to prevent auto-cycling with switch held	
        switch2Latch = 1;
    }

    // Reset latch once switch is released
    else if(SW0_3 == SW_NOT_PRESSED)
    {
        switch2Latch = 0;
    }
}


//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    uint16_t ADC0_Sampling_Rate
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 3.3 us
// intervals. Timer 2 overflow automatically triggers ADC0 conversion.
//
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // Use SYSCLK as time base, 16-bit
                                       // auto-reload
   CKCON  |= 0x10;                     // Select SYSCLK for timer 2 source
   TMR2RL  = -((SYSCLK/300000) + 1);   // Set reload for ADC0 sampling rate
   TMR2    = 0xFFFF;                   // Set to reload immediately
   TMR2CN_TR2     = 1;                        // Start Timer2
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
