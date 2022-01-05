//-----------------------------------------------------------------------------
// F91x_Temperature_Sensor.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the Temperature Sensor power consumption and
// performance, such as Offset , Offset Error, temperature. A UART connection
// from the MCU is required to print test result in UART terminal.
//
// Refer to F91x_Temperature_Sensor_Readme.txt for more information 
// regarding this code example's functionality. 
//
//
// How to Measure (Table 4.11):
// ----------------------------------------------------
// 1) Ensure the shorting block is attached to the J11 (VBAT & WALL_PWR) .
// 2) Ensure that jumpers are placed on J8 of the target board
//    that connect P0.4/TX to the RXD signal, and P0.5/RX to the TXD signal.
// 3) Remove the J17 shorting block and attach an ammeter between
//    the Imeasure headers.
// 4) Ensure that you have connected the COM Port USB Cable between your board
//    and your computer.
// 5) Launch your preferred UART communications application (such as PuTTY or
//    HyperTerminal) and configure your computer to initialize a serial COM
//    connection with 115200 Baud, 8-N-1.
// 6) Measure the ambient temperature (this temperature must be known)
//    and set #define AMBIENT_TC
// 7) Compile and download the code to the target board.
// 8) Run the code.
// 9) Follow the instructions printed out in the UART terminal to 
//    measure offset, offset error, temperature and supply current.
//    
//
//
//
// Target:          C8051F91x/90x
// Tool chain:      Generic
// Command Line:    None
//
//
// Revision History:
// -----------------
// Release 1.1 (JL)
//    - Updated UART printing information.
//    - 14 Feb 2014
//
// Release 1.0
//    - Initial Revision (MT) 
//    - 26 OCT 2013
//
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

#define SYSCLK          24500000        // SYSCLK frequency in Hz
#define SARCLK          7000000         // SARCLK frequency in Hz
#define BAUDRATE        115200          // Baud rate of UART in bps
#define DATA_SIZE       64              // Number of samples to take

#define DF_OFFSET       1024            // Default offset
#define VREF            1650            // VREF 1.65V
#define SLOPE           34              // Slope 3.4 * 10
#define AMBIENT_TC      25              // Ambient temperature
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint16_t xdata ADCBuffer[DATA_SIZE];
int16_t xdata offset;
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART0_Init (void);
void ADC0_Init (void);
void ADC0_SampleWindow (void);
void UART_Output (uint8_t inputChar);
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
    printf("F91x_Temperature_Sensor.c \n");
    printf("---------------------------------------------- \n");      
    printf("Copyright 2014 Silicon Laboratories, Inc.\n");
    printf("\n");
    
    offset = DF_OFFSET;

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
            printf("To measure offset , Press 1. \n");
            printf("To measure offset error,Press 2. \n");
            printf("To measure temperature, Press 3. \n");
            printf("To measure supply current, Press 4. \n");
            printf("Enter your choice: ");
            inputChar = getchar();

            //printf("\f");
            
            switch (inputChar)
            {
                case '1':
                case '2':
                case '3':
                    ADC0MX = 0x1B;          // ADC0 input Temperature sensor
                    REF0CN = 0x1C;          // Enable temperature sensor
                                            // VREF = 1.65V
                    inputLatch = 1;
                    ADC0_SampleWindow();
                    // Discharge any voltage on the ADC sampling capacitor 
                    // from the previous channel selection. 
                    ADC0MX = 0x1F;         
                    break;

                case '4':
                    if (REF0CN & 0x04)
                    {
                        REF0CN &= ~0x04;
                        printf("\nTemperature Sensor,  Disable : \n");
                    }
                    else 
                    {
                        REF0CN |= 0x04;
                        printf("\nTemperature Sensor,  Enable : \n");
                    }
                    break;

                default:
                    break;
		      
            }
            
        }
        // Printed test result to the UART terminal
        UART_Output (inputChar);
   
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
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
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
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator divided by 1 as its clock source.  Also enables missing clock
// detector reset.
//
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
    // Select Precision Internal Oscillator 24.5MHz as system
    // clock source
    REG0CN |= 0x10;
    OSCICN |= 0x80;	
    CLKSEL = 0x00; 
    //At clock frequencies above 10 MHz, the system clock cycle becomes 
    //short enough that the one-shot timer no longer provides a power benefit.
    //Disabling the one-shot timer at higher frequencies 
    //reduces power consumption. 
    FLSCL = 0x40;			  
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
// Configures ADC0 to make single-ended analog measurements on the Temperature
// Sensor.
//
// P0.0   analog   I/O   ADC0 (ADC Input)
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
    ADC0CF = ((SYSCLK/SARCLK)-1)<<3;    // Set SAR clock to 7 MHz
    ADC0CF |= 0x01;                     // 1x gain, normal tracking mode,
                                        // 10-bit mode
    
    // ADC0 select GND Channel to discharge
    // any voltage on the ADC sampling capacitor 
    // fromthe previous channel selection
    ADC0MX = 0x1F;    
 
    ADC0AC = 0x51;                      // 10-bit mode enabled, accumulation
                                        // enabled, right-justified 2, repeat
                                        // count set to 4

    TMR2RL = -( (SYSCLK / 10000) + 1);  // Configure for 10 kHz rate
     
    EIP1 |= 0x08;                       // Set ADC0 to high priority

    EIE1 |= 0x08;                       // Enable ADC0 conversion complete
                                        // interrupt
    ADC0CN = 0x42;                      // Enable ADC0 burst mode
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
   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
                                       // Use SYSCLK as timebase, 16-bit
                                       // auto-reload
   CKCON |= 0x10;                      // Select SYSCLK for timer 2 source
   TMR2RL = -( (SYSCLK / 10000) + 1); // Configure for 10 kHz rate
                                      
   TMR2 = 0xFFFF;                      // Set to reload immediately
   TMR2CN_TR2 = 1;                            // Start Timer2
}

//-----------------------------------------------------------------------------
// Runtime Subroutines
//-----------------------------------------------------------------------------
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
    ADC0CN_ADINT = 0;

    // Stay within loop until <DATA_SIZE> samples are collected.
    while(totalADCSamples < DATA_SIZE)
    {
        // Poll interrupt flag
        while( !ADC0CN_ADINT ){}

        // Store each new data sample upon conversion completion
        ADC0CN_ADINT = 0;
        // Determine expected ADC value from user settings
       
        ADCBuffer[totalADCSamples] = ADC0;
        ADC0 = 0;
        
        totalADCSamples++;
    }
}

//-----------------------------------------------------------------------------
// UART_Output
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// All results are displayed via UART.
//-----------------------------------------------------------------------------
void UART_Output (uint8_t inputChar)
{
    uint8_t ADCIndex;
    static uint16_t ADCAverage;
    int16_t Value;
    uint32_t ADCSum;
    int16_t Vtemp;

    // Output formatting
    printf("\nExpected Value ,     Output : \n");
    printf("------------------------------------- \n");
 
    // Reset previous values
    ADCAverage = 0;
    ADCSum = 0;

   
    // Determines average of sample data.
    for(ADCIndex = 0; ADCIndex < DATA_SIZE; ADCIndex++)
    {
        ADCSum += ADCBuffer[ADCIndex];
    }

    ADCAverage = ADCSum / DATA_SIZE;
    // Vtemp = ADC * VREF / 1024 = ADC * 1.64 mV 
    //       = ADC * 16.4 /10
    //       = ADC * 16 / 10 + ADC * 4 / 100 
    
    Vtemp = ADCAverage * 8 / 5 ;
    Vtemp += ADCAverage / 25;
    switch (inputChar)
    {
        case '1':
            // Calculate offset with new ambient temperature 
            offset = Vtemp - SLOPE * (AMBIENT_TC - 25) / 10; 
            printf(" - calibration Offset Value: %i \n", offset);
            break;
        case '2':
            Value = Vtemp - SLOPE * (AMBIENT_TC - 25) /10; 
            printf(" - Offset Error: %i \n", Value - offset);
            break;
        case '3':
            Value = 25 + ((Vtemp - offset) * 10 / SLOPE);
            printf(" - Temperature Value: %i \n", Value);                    
            break;
    }
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
