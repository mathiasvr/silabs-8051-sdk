//-----------------------------------------------------------------------------
// F96x_ADC_Performance.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the performance characteristics (SNR, SINADR, THD
// SFDR, Offset Error) of the internal Analog to Digital converter (ADC0) as
// specified on table 4.12 of the datasheet.
//
//
// Refer to F96x_ADC_Performance_Readme.txt for more information 
// regarding this code example's functionality. 
//
//
// How to Measure (Table 4.12 - SNR, SINADR, THD, SFDR):
// ----------------------------------------------------
// 1) Ensure that shorting blocks are attached to the J15 header.
// 2) Remove all other shorting blocks and select VREG on SW5
// 3) Ensure that jumpers are placed on P2 of the target board
//    that connect P0.4/MCU_TX to the RXD signal, and P0.5/MCU_RX to the TXD
//    signal.
// 4) Ensure that you have connected a USB cable between J17 and your computer.
// 5) Launch your preferred UART communications application (such as PuTTY or
//    HyperTerminal) and configure your computer to initialize a serial COM
//    connection with 115200 Baud, 8-N-1.
// 6) Using a waveform generator, attach a sinusoidal signal with a
//    frequency of 3.5 kHz (10-Bit Samples) or 1.5 kHz (12-Bit Samples),
//    with an amplitude of 1.6 Vpp, and an offset of +800 mV to the ADC 
//    Input Pin 0.0). These frequencies ensure that the sample window
//    fits one period of waveform data.
// 7) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) Follow the instructions printed out in the UART terminal to take
//    10-bit or 12-bit sampling data.
// 10) Open F96x_ADC_Performance_SNR.xlsx and follow the directions inside
//    to analyze the sampling data.
//
// How to Measure (Table 4.12 - Offset Error):
// ----------------------------------------------------
// 1) Ensure that shorting blocks are attached to the J15 header .
// 2) Ensure that jumpers are placed on P2 of the target board
//    that connect P0.4/MCU_TX to the RXD signal, and P0.5/MCU_RX to the TXD 
//    signal.
// 3) Ensure that you have connected a USB cable between J17 and your computer.
// 4) Launch your preferred UART communications application (such as PuTTY or
//    HyperTerminal) and configure your computer to initialize a serial COM
//    connection with 115200 Baud, 8-N-1.
// 5) Attach a DC voltage source with a voltage of 1V to the ADC Input J11(P0.0)
// 7) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) Follow the instructions printed out in the UART terminal to take
//    10-bit or 12-bit sampling data.
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.2 (BL)
//    - Updated Description / How to Test
//    - 6 FEB 2014
//
// Release 1.0
//    - Initial Revision (MT) 
//    - 19 OCT 2013
//
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F960_Register_Enums.h"
#include "stdio.h"

//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

#define SYSCLK          20000000        // SYSCLK frequency in Hz
#define SARCLK          8300000         // SARCLK frequency in Hz
#define BAUDRATE        115200          // Baud rate of UART in bps
#define DATA_SIZE       64              // Number of samples to take

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint16_t xdata ADCBuffer[DATA_SIZE];

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART0_Init (void);
void ADC0_Init (void);
void ADC0_12Bit (void);
void ADC0_10Bit (void);
void ADC0_SampleWindow (void);
void UART_OutputADC0Samples (void);
void UART_OutputADC0Average (void);
void Timer2_Init (void);
void Port_Init (void);

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
   
    uint8_t inputChar;
    uint8_t inputLatch;

                                        // enable)

    // Initialize all components and peripherals
    SYSCLK_Init();
    UART0_Init();
    ADC0_Init();  
    Timer2_Init();
    Port_Init();             
 
    // Print opening text
    printf("F96x_ADC_Performance.c \n");
    printf("---------------------------------------------- \n");      
    printf("Copyright 2014 Silicon Laboratories, Inc.\n");
    printf("\n");
     
    // Disable global interrupts
    IE_EA = 0;
    while (1)
    {

        // inputLatch prevents program from progression until input is given
        inputLatch = 0;
        printf("\n");

        while( !inputLatch )
        {
            // Print directions to user.
            printf("---------------------------------------------- \n");
            printf("To take a sample using 12-bit Mode, Press 1. \n");
            printf("To take a sample using 10-bit Mode, Press 2. \n");
            printf("To run the 12-bit offset error test, Press 3. \n");
            printf("To run the 10-bit offset error test, Press 4. \n");
            printf("Enter your choice:");
            inputChar = getchar();

            printf("\n");

            // Checks if user has input an appropriate option, 
            // configures ADC mode.
            switch (inputChar){
            case '1':
                REF0CN = 0x08;      // Default Reference VDD                     
                ADC0_12Bit();        // Enable 12-Bit Mode Settings
                inputLatch = 1;
                break;
            case '2':
                REF0CN = 0x08;      // Default Reference VDD 
                ADC0_10Bit();       // Enable 10-Bit Mode Settings
                inputLatch = 1;
                break;
            case '3':
                REF0CN = 0x18;      // Use 1.65V Reference 
                ADC0_12Bit();       // Enable 12-Bit Mode Settings
                inputLatch = 1;
                break;
            case '4':
                REF0CN = 0x18;      // use 1.65V Reference 
                ADC0_10Bit();       // Enable 10-Bit Mode Settings
                inputLatch = 1;
            default:
                break;
            }
        }
      
        switch(inputChar){
            // Test 1/2:
            // Perform ADC sample. Samples are captured with a sampling 
            // rate that achieves maximum throughput. 
        case '1':
	    case '2':
            ADC0_SampleWindow();
	        UART_OutputADC0Samples();
            break;

            // Test 3/4:
            // Perform ADC Offset Error test. a full set of ADC samples are
            // taken and then averaged. This average is compared to the 
            // expected ADC value, after which error is calculated. 
        case '3':
	    case '4':
            ADC0_SampleWindow();
            UART_OutputADC0Average();
            break;

	    default:
            break;
        }

        // Prints directions to user, data must be transferred to the Excel file.
        printf("Copy the above samples and paste into the excel document, \n");
        printf("F96x_ADC_Performance_SNR.xlsx to measure the SNR of the data. \n");
        printf("\n");
   } 
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
// P0.4   digital   push-pull   UTX (UART Transmit)
// P0.5   digital   push-pull   URX (UART Receive)
// P0.0   analog               input    AD0MX P0.0
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    SFRPAGE = LEGACY_PAGE;

    P0MDIN   &= ~0x01;                  // Set P0.0 as an analog input
    P0MDOUT  &= ~0x01;                  // Set P0.0 to open-drain
    P0       |=  0x01;                  // Set P0.0 latch to '1'
    P0SKIP   |=  0x01;                  // Skip P0.0 in the Crossbar
    
    // UART TX (P0.4) and RX (P0.5) Output
    P0MDOUT |= 0x10;                    // Enable UTX as push-pull output
    XBR0 = 0x01;                        // Enable UART on P0.4(TX) and P0.5(RX)

    // Crossbar Initialization
    XBR2 = 0x40;                        // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the Low power 20 MHz
// oscillator divided by 1 as its clock source.  Also enables missing clock
// detector reset.
//
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
    SFRPAGE = CONFIG_PAGE;
    FLSCL     = 0x40;               // Set BYPASS bit for >12.5 MHz.
    // Select Precision Internal Oscillator 24.5MHz as system
    // clock source
    SFRPAGE = LEGACY_PAGE; 
   
    CLKSEL = 0x04;                  // Select Low power 20 MHz osc.
                                    // divided by 1 as the system clock
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
    SFRPAGE = LEGACY_PAGE;
    SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                        //        level of STOP bit is ignored
                                        //        RX enabled
                                        //        ninth bits are zeros
                                        //        clear SCON0_RI and SCON0_TI bits
   
    if (SYSCLK/BAUDRATE/2/256 < 1) 
    {
        TH1 = -(SYSCLK/BAUDRATE/2);
        CKCON &= ~0x0B;                 // T1M = 1; SCA1:0 = xx
        CKCON |=  0x08;
    } 
   
    else if (SYSCLK/BAUDRATE/2/256 < 4) 
    {
        TH1 = -(SYSCLK/BAUDRATE/2/4);
        CKCON &= ~0x0B;                 // T1M = 0; SCA1:0 = 01
        CKCON |=  0x01;
    } 
   
    else if (SYSCLK/BAUDRATE/2/256 < 12) 
    {
        TH1 = -(SYSCLK/BAUDRATE/2/12);
        CKCON &= ~0x0B;                 // T1M = 0; SCA1:0 = 00
    } 
   
    else 
    {
        TH1 = -(SYSCLK/BAUDRATE/2/48);
        CKCON &= ~0x0B;                 // T1M = 0; SCA1:0 = 10
        CKCON |=  0x02;
    }

    TL1 = TH1;                          // Init Timer1
    TMOD &= ~0xF0;                      // TMOD: timer 1 in 8-bit autoreload
    TMOD |=  0x20;
    TCON_TR1 = 1;                            // START Timer1
    SCON0_TI = 1;                            // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures ADC0 to make single-ended analog measurements on the P0.0.
//
// P0.0   analog   I/O   ADC0 (ADC Input)
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
    SFRPAGE = LEGACY_PAGE;
 
    ADC0CF = ((SYSCLK/SARCLK)-1)<<3;    // Set SAR clock to 8.3 MHz

    ADC0AC = 0x00;                      // 12-bit mode disabled, accumulation
                                        // disabled, right-justified, repeat
                                        // count set to 1

    ADC0MX = 0;                         // ADC0 positive input P0.0

    REF0CN = 0x18;                      // Use Internal Reference
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 3 us
// intervals.  Timer 2 overflow automatically triggers ADC0 conversion.
//
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
    SFRPAGE = LEGACY_PAGE;
    TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
                                        // Use SYSCLK as timebase, 16-bit
                                        // auto-reload
    CKCON |= 0x10;                      // Select SYSCLK for timer 2 source
    TMR2RL = -( (SYSCLK / 300000) + 1); // Configure for 300 kHz rate
                                      
    TMR2 = 0xFFFF;                      // Set to reload immediately
    TMR2CN_TR2 = 1;                            // Start Timer2
}

//-----------------------------------------------------------------------------
// Runtime Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ADC0_8Bit
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Switches ADC0 to a 12-Bit configuration (burst mode ). Used
// when switching between different sampling and testing modes.
//
//-----------------------------------------------------------------------------
void ADC0_12Bit (void)
{  
    SFRPAGE = LEGACY_PAGE;
    ADC0AC = 0x81;                      // 12 bit enabled accumulation disabled, 
                                        // right-justified, repeat
                                        // count set to 1
    ADC0CF |= 0x01;                     // 1x gain, normal tracking mode,
                                        // 
    TMR2RL = -( (SYSCLK / 75000) + 1);  // Configure for 75 kHz rate
    ADC0CN = 0x42;                      // Enable ADC0
}

//-----------------------------------------------------------------------------
// ADC0_10Bit
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Switches ADC0 to a 10-Bit configuration (burst mode enabled with repeat
// count set to four). Used when switching between different sampling and 
// testing modes.
//
//-----------------------------------------------------------------------------
void ADC0_10Bit (void)
{
    SFRPAGE = LEGACY_PAGE;
    ADC0AC = 0;                         // 10-bit mode enabled, accumulation
                                        // disabled, right-justified , repeat
                                        // count set to 1
    ADC0CF |= 0x01;                     // 1x gain, normal tracking mode,
                                        // 10-bit mode
    TMR2RL = -( (SYSCLK / 300000) + 1);  // Configure for 300 kHz rate
    ADC0CN = 0x82;                       // Enable ADC0
}

//-----------------------------------------------------------------------------
// ADC0_SampleWindow
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Initializes and waits for the ADC to take a set of samples and stores those
// samples into a buffer.
//
//-----------------------------------------------------------------------------
void ADC0_SampleWindow (void)
{

    static uint16_t totalADCSamples;

    // Reset ADC status and buffer status
    totalADCSamples = 0;
    SFRPAGE = LEGACY_PAGE;
    ADC0CN_ADINT = 0;

    // Stay within loop until <DATA_SIZE> samples are collected.
    while(totalADCSamples < DATA_SIZE)
    {
        // Poll interrupt flag
        while( !ADC0CN_ADINT ){}

        // Store each new data sample upon conversion completion
        ADC0CN_ADINT = 0;
        // Determine expected ADC value from user settings
       
        ADCBuffer[totalADCSamples++] = ADC0;
        ADC0 = 0;
    }
}

//-----------------------------------------------------------------------------
// UART_OutputADC0Samples
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Outputs the current buffer of data through UART to the user.
//
//-----------------------------------------------------------------------------
void UART_OutputADC0Samples (void)
{

   uint8_t ADCIndex;

   // Output formatting
   printf("ADC Sample Window, Max Throughput: \n");
   printf("------------------------------------ \n");

   // Prints ADC value buffer (form is friendly for Copy-paste operation)
   for(ADCIndex = 0; ADCIndex < DATA_SIZE; ADCIndex++)
   {
      printf("%i \n", ADCBuffer[ADCIndex]);
   }

   printf("\n");

}

//-----------------------------------------------------------------------------
// UART_OutputADC0Average
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Calculates the average of the sampled data, compares it to the expected
// value, and then finds the offset error. All results are displayed via
// UART.
//
//-----------------------------------------------------------------------------
void UART_OutputADC0Average (void)
{
    uint8_t ADCIndex;
    static uint16_t ADCAverage;
    uint16_t ADCExpectedValue;
    uint32_t ADCSum;

    // Output formatting
    printf("Average of ADC Samples, Offset Error: \n");
    printf("------------------------------------ \n");
 
    // Reset previous values
    ADCAverage = 0;
    ADCSum = 0;

    // Determines average of sample data.
    for(ADCIndex = 0; ADCIndex < DATA_SIZE; ADCIndex++)
    {
        ADCSum += ADCBuffer[ADCIndex];
    }
   
    // Determine expected ADC value from user settings
    if( ADC0AC & 0x80 )
    {
        // Expected ADC Value for 1V, 12-Bit Sample
        // ADCExpectedValue = 409600 / 165; // 2482
        ADCExpectedValue = 2507;
    }
    else
    {
        // Expected ADC Value for 1V, 10-Bit Sample 
        //ADCExpectedValue = 102400 / 165; // 620
        ADCExpectedValue = 625;
    }

    ADCAverage = ADCSum / DATA_SIZE;

    // Output results, including error calculation.
    printf(" - Average Value: %i \n", ADCAverage);
    printf(" - Expected Value: %i \n", ADCExpectedValue);
    printf(" - Offset Error: %i \n", ADCExpectedValue - ADCAverage);
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
