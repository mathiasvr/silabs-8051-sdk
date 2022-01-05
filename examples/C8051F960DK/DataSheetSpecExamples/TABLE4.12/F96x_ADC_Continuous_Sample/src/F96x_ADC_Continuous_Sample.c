//-----------------------------------------------------------------------------
// F96x_ADC_Continuous_Sample.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
//
// This example demonstrates the power consumption and throughput rate of the
// on-board Analog to Digital converter (ADC0) as specified on table 4.12 of
// the datasheet.
//
//
// Refer to F96x_ADC_Continuous_Sample_Readme.txt for more information 
// regarding the functionality of this code example.
//
//
// How To Measure (Table 4.12 - ADC Normal power mode, conversion mode current):
// ----------------------------------------------------
// 1) Place VBAT SEL(SW5) in the VREG position.
// 2) Ensure that jumpers are placed on P2 of the target board
//    that connect P0.4/TX to the RXD signal, and P0.5/RX to the TXD signal.
// 3) Cut the trace between two pins of J15, solder a jumper,  attach an ammeter 
//    between the VBAT CURRENT headers.
// 4) Set #define ADC0_MODE_SELECT to NP_CONVERSION_MODE. This will initialize the 
//    ADC to run at 10bit conversion mode and setting sample at 300 ksps.
// 5) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Measure the current across Imeasure. This reading is the current with
//    the ADC enabled.
// 8) Press the switch at P0.2 (SW3) to disable the ADC.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 Power supply current under Table 4.12.
//
//
// How To Measure (Table 4.12 - ADC Normal power mode, tracking mode current):
// ----------------------------------------------------
// 1) Place VBATT SEL(SW5) in the VREG position.
// 2) Ensure that jumpers are placed on P2 of the target board
//    that connect P0.4/TX to the RXD signal, and P0.5/RX to the TXD signal.
// 3) Cut the trace between two pins of J15, solder a jumper,  attach an ammeter 
//    between the VBAT CURRENT headers.
// 4) Set #define ADC0_MODE_SELECT to NP_TRACKING_MODE. This will initialize the 
//    ADC to run at 10bit tracking mode.
// 5) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Measure the current across Imeasure. This reading is the current with
//    the ADC enabled.
// 8) Press the switch at P0.2 (SW3) to disable the ADC.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 Power supply current under Table 4.12.
//
//
// How To Measure (Table 4.12 - ADC Low power mode, conversion mode current):
// ----------------------------------------------------
// 1) Place VBATT SEL(SW5) in the VREG position.
// 2) Ensure that jumpers are placed on P2 of the target board
//    that connect P0.4/TX to the RXD signal, and P0.5/RX to the TXD signal.
// 3) Cut the trace between two pins of J15, solder a jumper,  attach an ammeter 
//    between the Imeasure headers.
// 4) Set #define ADC0_MODE_SELECT to LP_CONVERSION_MODE. This will initialize the 
//    ADC to run at 10bit low power conversion mode and setting sample at 150 ksps.
// 5) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Measure the current across Imeasure. This reading is the current with
//    the ADC enabled.
// 8) Press the switch at P0.2 (SW3) to disable the ADC.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 Power supply current under Table 4.12.
//
//
// How To Measure (Table 4.12 - ADC Low power mode, tacking mode current):
// ----------------------------------------------------
// 1) Place VBATT SEL(SW5) in the VREG position.
// 2) Ensure that jumpers are placed on P2 of the target board
//    that connect P0.4/TX to the RXD signal, and P0.5/RX to the TXD signal.
// 3) Cut the trace between two pins of J15, solder a jumper,  attach an ammeter 
//    between the Imeasure headers.
// 4) Set #define ADC0_MODE_SELECT to LP_TRACKING_MODE. This will initialize the 
//    ADC to run at 10bit low power tracking mode.
// 5) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Measure the current across Imeasure. This reading is the current with
//    the ADC enabled.
// 8) Press the switch at P0.2 (SW3) to disable the ADC.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 Power supply current under Table 4.12.
//
//
//
//
// How To Measure (Table 4.12 - ADC Conversion Throughput Rate):
// -------------------------------------------------------------------
// 1) Set #define THROUGHPUT_EN 1
// 2) Other settings are same as above Measurements
// 3) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) Use an oscilloscope to monitor P1.5. A square wave should be seen at
//    this pin. Verify the frequency of this square wave is half the rate of
//    the sampling rate listed for ADC conversation throughput Rate 300 ksps
//    in Table 4.12.
//
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 6 FEB 2014
//
// Release 1.0
//    - Initial Revision (MT)
//    - 18 OCT 2013
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F960_Register_Enums.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK              24500000    // SYSCLK frequency in Hz

#define SARCLK              8300000     // SARCLK frequency in Hz
#define SARCLK_LP           4000000    

#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

#define ADC0_DISABLE        0

// Use the following define statement to select the ADC characteristic to test.
// 
// Available Modes:
// ----------------
// 0 - Power Consumption: Normal Power conversion mode (300 ksps)
// 1 - Power Consumption: Normal Power tracking mode (0 ksps)
// 2 - Power Consumption: Low Power conversion mode (150 ksps)
// 3 - Power Consumption: Low Power conversion mode (0 ksps)
//
#define NP_CONVERSION_MODE  0
#define NP_TRACKING_MODE    1
#define LP_CONVERSION_MODE  2
#define LP_TRACKING_MODE    3

#define ADC0_MODE_SELECT    LP_TRACKING_MODE           

//Throughput Rate: 300kHz
#define THROUGHPUT_EN       0

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(SW3, SFR_P0, 2);                 // Switch 1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(PM_ISR, PMATCH_IRQn);
SI_INTERRUPT_PROTO(TIMER2_ISR, TIMER2_IRQn);
SI_INTERRUPT_PROTO(ADC_ISR, ADC0EOC_IRQn);

void ADC0_Init(uint8_t);
void SYSCLK_Init (void);
void Port_Init (void);
void Timer2_Init (uint32_t);

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
    ADC0_Init(ADC0_MODE_SELECT);

    IE_EA = 1;                             // Enable global interrupts
    while (1) {}
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    ADC0_Mode
//
// This routine uses the parameters ADC0_Mode initialize the ADC for the test 
// being performed. 
// The ADC interrupt is enabled if the user is testing the throughput
// of the ADC. settings and a sampling rate of 300 ksps 
//
//-----------------------------------------------------------------------------
void ADC0_Init (uint8_t ADC0_Mode)
{
    SFRPAGE = LEGACY_PAGE;
    ADC0CN = 0x02;                      // ADC0 disabled, conversions triggered
                                        // on Timer2 overflows

    ADC0MX = 0x06;                      // ADC0 positive input = P0.6

    REF0CN = 0x08;                      // Ground reference is the GND pin,
                                        // voltage reference is the VDD pin
                                        // (3.3 V), temperature sensor disabled   

    switch (ADC0_Mode) {
    case NP_CONVERSION_MODE:
        // Set SAR clock to 8.3 MHz
        ADC0CF = ((SYSCLK/SARCLK)-1)<<3;
        ADC0AC = 0x00;                  // repeat count set to 1
        ADC0CF |= 0x01;                 // 1x gain, Normal tracking mode, 10 bit mode 
        ADC0CN = 0x82;                  // Enable ADC0
        Timer2_Init (300000);           // Init Timer2 to trigger ADC at 300 ksps
        break;
    case NP_TRACKING_MODE:
        // Set SAR clock to 8.3 MHz
        ADC0CF = ((SYSCLK/SARCLK)-1)<<3;
        ADC0AC = 0x51;                  // accumulation enabled, right-justified 2, repeat
                                        // count set to 4 
        ADC0CF |= 0x01;                 // 1x gain, normal tracking mode,
        ADC0CN = 0x42;                  // Burst mode enabled
        Timer2_Init (50000);            // Init Timer2 to trigger ADC at 50kHz * 4 = 200kHz
        break;
    case LP_CONVERSION_MODE:
        SFRPAGE = CONFIG_PAGE;
        ADC0PWR |= 0x80;                // Low power mode enabled
        SFRPAGE = LEGACY_PAGE;
        // Set SAR clock to 4.0 MHz
        ADC0CF = ((SYSCLK/SARCLK_LP)-1)<<3;
        ADC0AC = 0x00;                  // repeat count set to 1
        ADC0CF |= 0x01;                 // 1x gain, Normal tracking mode, 10 bit mode 
        ADC0CN = 0x82;                  // Enable ADC0
        Timer2_Init (150000);           // Init Timer2 to trigger ADC at 150 ksps
        break;
    case LP_TRACKING_MODE:
        SFRPAGE = CONFIG_PAGE;
        ADC0PWR |= 0x80;                // Low power mode enabled
        SFRPAGE = LEGACY_PAGE;
        // Set SAR clock to 4.0 MHz
        ADC0CF = ((SYSCLK/SARCLK_LP)-1)<<3;
        ADC0AC = 0x51;                  // accumulation enabled, right-justified 2, repeat
                                        // count set to 4 
        ADC0CF |= 0x01;                 // 1x gain, normal tracking mode,
        ADC0CN = 0x42;                  // Burst mode enabled
        Timer2_Init (30000);            // Init Timer2 to trigger ADC at 30kHz * 4 = 120kHz
        break;
    default:
        break;
    }

    // If in throughput testing modes
    #if THROUGHPUT_EN > 0

    EIE1 |= 0x08;                       // Enable ADC0 conversion complete
                                        // interrupt
    #endif
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
    SFRPAGE = CONFIG_PAGE;
    FLSCL     = 0x40;                   // Set BYPASS bit for >12.5 MHz.
    // Select Precision Internal Oscillator 24.5MHz as system
    // clock source
    SFRPAGE = LEGACY_PAGE;
    REG0CN |= 0x10;
    OSCICN |= 0x80;	
    CLKSEL = 0x00; 
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the Crossbar and GPIO ports based on the characteristics under 
// test.
//
// P1.5  digital  push-pull   output   
// P0.6  analog               input    AD0MX P0.6
// P0.2  digital  open-drain  input    SW3
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    SFRPAGE = LEGACY_PAGE;

    P0MDIN   &= ~0x40;                  // Set P0.6 as an analog input
    P0MDOUT  &= ~0x40;                  // Set P0.6 to open-drain
    P0       |=  0x40;                  // Set P0.6 latch to '1'
    P0SKIP   |=  0x40;                  // Skip P0.6 in the Crossbar
    
    P0MDIN |= 0x04;                     // Set P0.2 as an digital input
    P0MDOUT &= ~0x04;                   // Set P0.2 as open-drain
    P0 |= 0x04;                         // Set P0.2 latch to '1'
   
    // Set Port Match settings for S1 (P0.2)
    P0MASK |= 0x04;                     // Enable Port Match check for P0.2
    P0MAT  |= 0x04;                     // Set Port Match value to 1 for P0.2
    EIE2 = 0x02;                        // Enable port match interrupts
    // If in throughput testing modes
    #if THROUGHPUT_EN > 0
   
    P1MDIN |= 0x20;                     // Set P1.5 as an digital 
    P1MDOUT |= 0x20;                    // Set P1.5 as push-pull output
    P1 &= ~0x20;                        // Set the P1.5 latch to '0'
    #endif
    
    XBR2 = 0x40;                        // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    uint32_t ADC0_Sampling_Rate
//
// Configure Timer2 to 16 bit auto-reload and generate an interrupt at the rate
// specified by ADC0_Sampling_Rate. Timer 2 overflow automatically triggers 
// ADC0 conversion. The Timer2 interrupt is enabled and used to clear the ADC 
// interrupt when the user is not testing the throughput of the ADC.
//
//-----------------------------------------------------------------------------
void Timer2_Init (uint32_t ADC0_Sampling_Rate)
{
    SFRPAGE = LEGACY_PAGE;
    TMR2CN = 0x00;                      // Stop Timer2 and clear TF2
                                        // Use SYSCLK as timebase, 16 bit
                                        // auto-reload
    CKCON |= 0x10;                      // Select SYSCLK for Timer2 source

    // Set reload for ADC0 sampling rate
    TMR2RL = -((SYSCLK/ADC0_Sampling_Rate) + 1);

    TMR2 = 0xFFFF;                      // Set to reload immediately
   
    TMR2CN_TR2 = 1;                            // Start Timer2

    // If in Power consumption testing modes
    #if THROUGHPUT_EN == 0
    IE_ET2 = 1;                            // Timer2 interrupt enabled
    #endif
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ADC0_ISR
//-----------------------------------------------------------------------------
//
// This ISR is called after each ADC conversion which is triggered by Timer2.
// 
// NOTE: This ISR is ignored if the user is testing the power consumption of
//       the ADC.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(ADC_ISR, ADC0EOC_IRQn)
{
    SFRPAGE = LEGACY_PAGE;
    if(P1 & 0x20)
        P1 &= ~0x20;
    else
        P1 |= 0x20;
    ADC0CN_ADINT = 0;                         // Clear ADC0 conv. complete flag
}

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// The Timer 2 interrupt occurs whenever the timer overflows (TMR2 == 0xFFFF).
// 
// NOTE: This ISR is ignored if the user is testing the throughput of the ADC.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
    SFRPAGE = LEGACY_PAGE;
    TMR2CN_TF2H = 0;                           // Clear the interrupt flag
    ADC0CN_ADINT = 0;                         // Clear ADC0 conv. complete flag
}

//-----------------------------------------------------------------------------
// Port Match ISR
//-----------------------------------------------------------------------------
//
// Whenever SW3 (P0.2) is pressed, the ADC enable is toggled.
//
// The Port Match interrupt occurs whenever (Pn & PnMASK) != (PnMAT & PnMASK).
// We must disable the interrupt and wait until the switches are released in
// main, or else the interrupt will trigger multiple times.  It is generally
// not good practice to sit in an ISR for long periods of time.
//
// NOTE: This ISR is ignored if the user is testing the throughput of the ADC.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PM_ISR, PMATCH_IRQn)
{
    SFRPAGE = LEGACY_PAGE;
    if (SW3 == SW_PRESSED)
    {
        if (ADC0AC == 0)
        {
            // Conversion mode
            ADC0CN_ADEN = !ADC0CN_ADEN;             // Toggle ADC enable
        }
        else 
        {
            // Tracking mode
            ADC0CN_ADBMEN = !ADC0CN_ADBMEN;
        }
        P0MAT &= ~0x04;                 // Set port match to trigger at switch
                                        // rising edge to prevent multiple 
                                        // interrupts from occurring while the
                                        // switch is pressed   
    }
    else
    {
        P0MAT |= 0x04;                  // Reset port match to trigger at switch
                                        // falling edge
    }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
