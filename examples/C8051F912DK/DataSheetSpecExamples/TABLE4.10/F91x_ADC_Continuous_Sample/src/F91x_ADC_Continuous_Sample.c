//-----------------------------------------------------------------------------
// F91x_ADC_Continuous_Sample.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the power consumption and throughput rate of
// the Analog to Digital Converter (ADC) in datasheet Table 4.10.
//
// Refer to F91x_ADC_Continuous_Sample_Readme.txt for more information
// regarding the functionality of this code example.
//
//
// How To Measure (Table 4.10 - ADC 10bit Conversion Power Consumption):
// ----------------------------------------------------
//  1) Ensure that shorting blocks on a C8051F912-TB are attached to the
//     J11 (VBAT & WALL_PWR) and J8 (P0.2H) headers.
//  2) Place VDD Select (SW4) in the 2 CELL position and power the board
//     using the P2 power connector.
//  3) Remove all shorting blocks and attach an ammeter between
//     the J17 (IMEASURE) headers.
//  4) Set ADC0_MODE_SELECT to 0 to select ADC 10-bit conversion mode.
//  5) Set LOW_POWER_MODE to 0 to for Normal mode with 300 ksps throughput.
//     If LOW_POWER_MODE is set to 1, the ADC works as Low power mode with
//     200 ksps throughput.
//  6) Download and run the code on the target board.
//  7) Measure the current across IMEASURE. This reading is the current when
//     ADC is enabled.
//  8) Use the switch at P0.2 (SW0_2) to disable the ADC.
//  9) Record the current reading and subtract the value from the previously
//     recorded value. Verify the difference is within the range of the values
//     listed on the datasheet for ADC0 Power supply current under Table 4.10.

// How To Measure (Table 4.10 - ADC 10bit Tracking Power Consumption):
// ----------------------------------------------------
//  1) Ensure that shorting blocks are attached to the J11 (VBAT & WALL_PWR)
//     and J8 (P0.2H) headers. 
//  2) Place VDD Select (SW4) in the 2 CELL position and power the board
//     using the P2 power connector.
//  3) Remove all shorting blocks and attach an ammeter between
//     the J17 (IMEASURE) headers.
//  4) Set ADC0_MODE_SELECT to 1 to select ADC 10-bit Tracking mode.
//  5) Set LOW_POWER_MODE to 0 to select Normal mode with 280ksps throughput.
//     If LOW_POWER_MODE is set to 1, the ADC works as low power mode with 200
//     ksps throughput.
//  6) Download and run the code on the target board.
//  7) Measure the current across IMEASURE. This reading is the current with
//     the ADC enabled.
//  8) Use the switch at P0.2 (SW0_2) to disable the ADC.
//  9) Record the current reading and subtract the value from the previously
//     recorded value. Verify the difference is within the range of the values
//     listed on the datasheet for ADC0 Power supply current under Table 4.10.
//
//
// How To Measure (Table 4.10 - ADC 12bit Tracking Power Consumption):
// ----------------------------------------------------
//  1) Ensure that shorting blocks are attached to the J11 (VBAT & WALL_PWR) 
//     and J8 (P0.2H) headers. 
//  2) Place VDD Select (SW4) in the 2 CELL position and power the board
//     using the P2 power connector.
//  3) Remove all shorting blocks and attach an ammeter between
//     the J17 (IMEASURE) headers.
//  4) Set ADC0_MODE_SELECT to 2 to select ADC 12-bit Tracking mode.
//  5) Set LOW_POWER_MODE to 0 to select Normal mode with 75 ksps throughput.
//     If LOW_POWER_MODE is set to 1, the ADC works as Low power mode with
//     55 ksps throughput.
//  5) Download and run the code on the target board.
//  6) Measure the current across IMEASURE. This reading is the current with 
//     the ADC enabled.
//  7) Use the switch at P0.2 (SW0_2) to disable the ADC.
//  8) Record the current reading and subtract the value from the previously
//     recorded value. Verify the difference is within the range of the values
//     listed on the datasheet for ADC0 Power supply current under Table 4.10.
//
//
// How To Measure (Table 4.10 - ADC Conversion Throughput Rate):
// -------------------------------------------------------------------
//  1) Set THROUGHPUT_EN to 1
//  2) Other settings are same as above Measurements
//  3) Compile the code.
//  4) Download and run the code on the target board.
//  5) Use an oscilloscope to monitor P1.5. A square wave should be seen at 
//     this pin. Verify the frequency of this square wave is half the rate of
//     the sampling rate listed for ADC conversion throughput Rate 
//     in Table 4.10.
//
//
// Target:         C8051F93x/92x
// Tool Chain:     Generic
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (MT)
//    - 23 SEP 2013
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK              20000000    // SYSCLK frequency in Hz

#define NP10_SARCLK         7000000     // SARCLK frequency in Hz
#define NP12_SARCLK         6000000
#define LP_SARCLK           4000000

#define SW_PRESSED          0
#define SW_NOT_PRESSED      1


#define ADC0_DISABLE        0

// Use the following define statement to select the ADC characteristic to test.
// 
// Available Modes:
// ----------------
// 0 - Power Consumption: Conversion 10 bit mode (300 ksps)
// 1 - Power Consumption: Tracking 10 bit mode
// 2 - Power Consumption: Tracking 12 bit mode
//
#define ADC0_MODE_SELECT    0           

//  Low Power Mode Switch
#define LOW_POWER_MODE      0

#define CONVERSION_MODE     0
#define TRACKING_MODE       1

#define ADC0_10_BIT         0
#define ADC0_12_BIT         1

#define LOW_POWER           0x80
#define NORMAL_POWER        0

//Throughput Rate: 300kHz
#define THROUGHPUT_EN       0

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(SW0_2, SFR_P0, 2);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void ADC0_Init(uint8_t, uint8_t);
void SYSCLK_Init (void);
void Port_Init (void);
void Timer2_Init (uint32_t);

SI_INTERRUPT_PROTO(ADC_ISR, ADC0EOC_IRQn);
SI_INTERRUPT_PROTO(TIMER2_ISR, TIMER2_IRQn);
SI_INTERRUPT_PROTO(PM_ISR, PMATCH_IRQn);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint8_t powerMode = 0;

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

    SYSCLK_Init ();                     // Initialize system clock to 20 MHz
    Port_Init ();                       // Initialize crossbar and GPIO

#if (LOW_POWER_MODE == 1)
    powerMode = LOW_POWER;
#else
    powerMode = NORMAL_POWER;
#endif
    
    // ADC0 power mode and resolution initialization selection
    switch (ADC0_MODE_SELECT)
    {
        // Setup ADC0 for power consumption measurement in conversion
        // 10 bit mode
        case 0:
            ADC0_Init (CONVERSION_MODE, ADC0_10_BIT);
            break;
            
        // Setup ADC0 for power consumption measurement in tracking
        // 10 bit mode
        case 1:
            ADC0_Init (TRACKING_MODE, ADC0_10_BIT);
            break;

        // Setup ADC0 for power consumption measurement in tracking
        // 12 bit mode
        case 2:
            ADC0_Init (TRACKING_MODE, ADC0_12_BIT);
            break;

        default:
            // Error: ADC initialization mode selection not recognized.
            while(1);
   }

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
// Parameters:    ADC0_Power_Mode
//                ADC0_Resolution
//
// This routine uses the parameters ADC0_Power_Mode and ADC0_Resolution to 
// initialize the ADC for the test being performed. ADC0_Power_Mode can be 
// either conversion or tracking mode. ADC0_Resolution can be either 10 bit or 
// 12 bit. The ADC interrupt is enabled if the user is testing the throughput
// of the ADC. settings and a sampling rate of different model. 
//
//-----------------------------------------------------------------------------
void ADC0_Init (uint8_t ADC0_Power_Mode, uint8_t ADC0_Resolution)
{
    ADC0CN = 0x02;                      // ADC0 disabled, conversions triggered
                                        // on Timer2 overflows

    ADC0MX = 0x00;                      // ADC0 positive input = P0.0

    REF0CN = 0x08;                      // Ground reference is the GND pin,
                                        // voltage reference is the VDD pin
                                        // (3.3 V), temperature sensor disabled   

    SFRPAGE = CONFIG_PAGE;
    ADC0PWR = powerMode;                // Set power mode.
    SFRPAGE = LEGACY_PAGE;
    
    // Initialize ADC0 with Normal tracking mode settings
    if(ADC0_Power_Mode == CONVERSION_MODE)
    {
        // ADC0 10 bit resolution initialization settings
        
        // 1x gain, Normal tracking mode,repeat count set to 1
        // 10 bit mode
        ADC0AC = 0;
        if (powerMode == 0x80)
        {   // Low Power mode
            ADC0CF = ((SYSCLK/LP_SARCLK)-1)<<3;
            Timer2_Init (200000);       // Init Timer2 to generate overflows
                                        // to trigger ADC at 200 kHz
        }
        else
        {
            // Normal mode
            ADC0CF = ((SYSCLK/NP10_SARCLK)-1)<<3;
            Timer2_Init (300000);       // Init Timer2 to generate overflows
                                        // to trigger ADC at 300 kHz
        }
        ADC0CF |= 0x01;
        ADC0CN = 0x82;                  // Enable ADC0
    }

    // ADC0 TRACKING_MODE (burst)initialization settings
    else
    { 
        // ADC0 10 bit resolution initialization settings
        if(ADC0_Resolution == ADC0_10_BIT)
        {
            ADC0AC = 0x51;              // burst mode enabled, accumulation
                                        // enabled, right-justified 2, repeat
                                        // count set to 4
            // 1x gain, Normal tracking mode,repeat count set to 1
            // 10 bit mode
            if (powerMode == 0x80) 
            {   // Low Power mode
                ADC0CF = ((SYSCLK/LP_SARCLK)-1)<<3;
                Timer2_Init (50000);    // Init Timer2 to generate overflows
                                        // to trigger ADC at 50*4 = 200kHz
            }
            else
            {
                // Normal mode
                ADC0CF = ((SYSCLK/NP10_SARCLK)-1)<<3;
                Timer2_Init (70000);    // Init Timer2 to generate overflows
                                        // to trigger ADC at 70*4 = 280kHz
            }
        }

        // ADC0 12 bit resolution initialization settings
        else
        {
            ADC0AC = 0x81;              // burst mode enabled, accumulation
                                        // disabled, right-justified 0, repeat
                                        // count set to 4
            // 1x gain, Normal tracking mode,repeat count set to 1
            // 12 bit mode 
            if (powerMode == 0x80)
            {   // Low Power mode
                ADC0CF = ((SYSCLK/LP_SARCLK)-1)<<3;
                Timer2_Init (55000);    // Init Timer2 to generate overflows
                                        // to trigger ADC at 55kHz
            }
            else
            {
                // Normal mode
                ADC0CF = ((SYSCLK/NP12_SARCLK)-1)<<3;
                Timer2_Init (75000);    // Init Timer2 to generate overflows
                                        // to trigger ADC at 75kHz
            }
        }
        ADC0CF |= 0x01;
        ADC0CN = 0x42;                  // Enable ADC0
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
// This routine initializes the system clock to use the internal 20 MHz
// oscillator divided by 1 as its clock source.  Also enables missing clock
// detector reset.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
    CLKSEL = 0x04;        // Switch to Low-Power Oscillator
    OSCICN &= ~0x80;
    REG0CN &=~0x10;
 
    //At clock frequencies above 10 MHz, the system clock cycle becomes 
    //short enough that the one-shot timer no longer provides a power benefit.
    //Disabling the one-shot timer at higher frequencies 
    //reduces power consumption. 
    FLSCL = 0x40;			   
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
// P0.0  analog               input    AD0MX P0.0
// P0.2  digital  open-drain  input    SW0_2
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    P0MDIN &= ~0x01;                    // Set P0.0 as an analog input
    P0SKIP |= 0x01;                     // Skip P0.0 on crossbar
    P0MDIN |= 0x04;                     // Set P0.2 as an digital input
    P0MDOUT &= ~0x04;                   // Set P0.2 as open-drain
    P0 |= 0x05;                         // Set P0.0, P0.2 latch to '1'
    
    // Set Port Match settings for S1 (P0.2)
    P0MASK |= 0x04;                     // Enable Port Match check for P0.2
    P0MAT  |= 0x04;                     // Set Port Match value to 1 for P0.2
    EIE2 = 0x02;                        // Enable port match interrupts
    // If in throughput testing modes
    #if THROUGHPUT_EN > 0
   
    P1MDIN |= 0x20;                     // Set P1.5 as an digital input
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
    P1 ^= 0x20;                         // Invert the P1.5 latch
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
    TMR2CN_TF2H = 0;                           // Clear the interrupt flag
    ADC0CN_ADINT = 0;                         // Clear ADC0 conv. complete flag
}

//-----------------------------------------------------------------------------
// Port Match ISR
//-----------------------------------------------------------------------------
//
// Whenever SW0_2 (P0.2) is pressed, the ADC enable is toggled.
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
    if (SW0_2 == SW_PRESSED)
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
