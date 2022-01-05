//-----------------------------------------------------------------------------
// F96x_Voltage_Reference.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the Output Voltage accuracy and the supply current
// draw of the internal High Speed 1.65V Voltage Reference, as specified on
// table 4.14 of the datasheet.
//
//
// Refer to F96x_Voltage_Reference_Readme.txt for more information regarding
// the functionality of this code example.
//
//
// How To Test (Table 4.14 - Output Voltage):
// ---------------------------------------------------------
// 1) Cut the trace between two pins of J15, solder a jumper,  attach an ammeter
//    between the VBAT CURRENT header.
// 2) Power the board using the P2 power connector.
// 3) Set #define OUTPUT_VOLTAGE  1.
// 4) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Remove all of the shorting blocks on the board
// 7) Attach a power supply at P0.6 on J18, output voltage is as close 0V.
// 8) Increase the output voltage from 0V until LED4 turns on. LED4 will turn on
//    when the ADC reaches the maximum 10 bit value (0x03FF).
// 9) The voltage should read between the minimum and maximum values provided on
//    the datasheet for the internal voltage reference in Table 4.14.
//
//
//
// How To Test (Table 4.14 - Supply Current):
// ----------------------------------------------------------
// 1) Cut the trace between two pins of J15, solder a jumper,  attach an ammeter
//    between the VBAT CURRENT header.
// 2) Power the board using the P1 power connector.
// 3) Set #define OUTPUT_VOLTAGE  0.
// 4) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Remove all of the shorting blocks on the board.
// 7) Default is Normal mode.
// 8) Record the current then push SW2 (P0.1) to disable Reference.
// 9) Read the new current to subtract previous current, the difference is the
//    Normal mode Reference current.
// 10) Push SW1 (P0.0).toggle Reference to Low Power mode,LED3 on.
// 11) Record the current then push SW2(P0.1) to disable Reference.
// 12) Read the new current to subtract previous current,the difference is the
//     Low Power mode Reference current. 
//
//
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:    None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 6 FEB 2014
//
// Release 1.0
//    - Initial Revision (MT)
//    - 05 NOV 2013
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F960_Register_Enums.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK          20000000        // SYSCLK frequency in Hz
#define SARCLK          6000000         // SARCLK frequency in Hz

#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

#define LED_ON              0
#define LED_OFF             1

#define OUTPUT_VOLTAGE      0
// Mode type enumerations
enum REFERENCE_MODE {NORMAL = 1,
                     LOW_POWER  = 2,};
						
enum REFERENCE_MODE referenceMode;
//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(SW1,   SFR_P0,0);
SI_SBIT(SW2,   SFR_P0,1);
SI_SBIT(LED3,    SFR_P0,2);
SI_SBIT(LED4,    SFR_P0,3);

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

    IE_EA = 1;                             // Enable global interrupts
    referenceMode = NORMAL;

    // Loop forever
    while (1)
    {
#if (OUTPUT_VOLTAGE == 1)        
        // If the ADC is at a max reading (10 bit)
        if(ADC0 == 0x03FF)
        {
            LED4 = LED_ON;               // Turn ON LED2
        }
        else
        {
            LED4 = LED_OFF;              // Turn OFF LED2
        }
#else
        // Detect switch inputs and adjust modes
        Port_Update();
#endif        
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
// measurements on the P0.6.
//
// P0.6   analog   I/O   (ADC Input)
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
    ADC0CF = ((SYSCLK/SARCLK)-1)<<3;    // Set SAR clock to 6 MHz
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
    SFRPAGE = CONFIG_PAGE;
    FLSCL     = 0x40;                   // Flash Read Timing One-Shot Bypass.
    SFRPAGE = LEGACY_PAGE;
    
    // Select Low-Power Oscillator 20 MHz as system clock source
    // Since using the precision internal oscillator as the system clock source, 
    // the precision voltage reference should not be enabled from a disabled 
    // state. 
    CLKSEL = 0x04;        
    OSCICN &= ~0x80;
    REG0CN &=~0x10;
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
// P0.0  digital  open-drain   output   SW1
// P0.1  digital  open-drain   output   SW2
// P0.2  digital  push-pull    input    LED3
// P0.3  digital  push-pull    input    LED4
// P0.6  analog                input    ADC input 

//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    SFRPAGE = LEGACY_PAGE;
    // Analog Pin Setup
    P0MDIN  &= ~0x40;                   // Set P0.6 as an analog input
    P0MDOUT &= ~0x40;
    P0      |=  0x40;                   // Set the P0.6 latch to high
    P0SKIP  |=  0x40;                   // Skip P0.6 on crossbar
   
    P0MDIN  |=  0x0F;                   // Set P0.1,P0.1,P0.2,P0.3 as digital
    P0MDOUT |=  0x0C;                   // Set P0.2, P0,3 as push-pull
    P0MDOUT &= ~0x03;                   // Set P0.0  P0.1 as open-drain
    P0      |=  0x0F;
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
// Switch Sw1:
// Mode 1: Low Power Mode
// Mode 2: Normal Mode
//
// Switch SW2: 
// Toggle Reference ON / OFF
//
// P0.0   digital   open-drain   input   SW1 
// P0.1   digital   open-drain   input   SW2 
//
//-----------------------------------------------------------------------------
void Port_Update(void)
{
    // Cycle Reference output mode if P0.2 is pushed
    if( (SW1 == SW_PRESSED) && (!switch1Latch) )
    {
        SFRPAGE = CONFIG_PAGE;
        switch (referenceMode)
        {

            case NORMAL:
                referenceMode = LOW_POWER;
               
                // Select Low Power mode.
                ADC0PWR |= 0x80;
                LED3 = LED_ON;               // Turn ON LED1
                break;
                
            case LOW_POWER:
                referenceMode = NORMAL;
               
                // Select Normal mode.
                ADC0PWR &= ~0x80;
                LED3 = LED_OFF;               // Turn OFF LED1
                break;
        }
        SFRPAGE = LEGACY_PAGE;
        ADC0CN_ADINT = 0;
        ADC0 = 0;
        // Arm latches to prevent auto-cycling with switch held
        switch1Latch = 1;
    }

    // Reset latch once switch is released
    else if(SW1 == SW_NOT_PRESSED)
    {
        switch1Latch = 0;
    }

    // Cycle Reference ON / OFF if P0.3 is pushed
    if( (SW2 == SW_PRESSED) && (!switch2Latch) )
    {
        // Toggle SW3 between push-pull on/off
        // Reference between on/off
        if(REF0CN == 0x18)
        {
            REF0CN = 0x00;
        }
        else 
        {
            // Select internal 1.65 V high speed voltage reference.
            REF0CN = 0x18;
        }
        // Arm latches to prevent auto-cycling with switch held	  
        switch2Latch = 1;
    }

    // Reset latch once switch is released
    else if(SW2 == SW_NOT_PRESSED)
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
