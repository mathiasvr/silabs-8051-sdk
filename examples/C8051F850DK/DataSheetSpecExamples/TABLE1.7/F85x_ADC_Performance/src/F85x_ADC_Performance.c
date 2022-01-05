//-----------------------------------------------------------------------------
// F85x_ADC_Performance.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// Refer to F85x_ADC_Performance_Readme.txt for more information 
// regarding this code example's functionality. 
//
//
// How to Measure (Table 1.7 - SNR, SINAD):
// ----------------------------------------------------
// 1) Ensure that shorting blocks are attached to the JP2 (Imeasure) header 
//    as well as all three J7 (COM) headers.
// 2) Ensure that the ADC0.10-POT (J27) shorting block is removed.
// 3) Ensure that you have connected the COM Port USB Cable between your board
//    and your computer.
// 4) Launch your preferred UART communications application (such as PuTTY or
//    HyperTerminal) and configure your computer to initialize a serial COM
//    connection with 230400 Baud, 8-N-1. Ensure that the terminal connection
//    is configured to print a log file of all printed output.
// 5) Using a waveform generator, attach a sinusoidal signal with a
//    frequency below 1 kHz (the sampling rate is 2 kHz) to the ADC Input
//    (ADC0.10, P1.2). Configure the signal to possess an amplitude of 3.3V
//    peak-to-peak and an offset of 1600 mV.
// 6) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) Follow the instructions printed out in the UART terminal to take
//    10-bit or 12-bit sampling data.
// 9) Open F85x_ADC_Performance_SNR.xlsx and follow the directions inside
//    to analyze the sampling data.
//
// How to Measure (Table 1.7 - Offset Error):
// ----------------------------------------------------
// 1) Ensure that shorting blocks are attached to the JP2 (Imeasure) header 
//    as well as all three J7 (COM) headers.
// 2) Ensure that the ADC0.10-POT (J27) shorting block is removed.
// 3) Ensure that you have connected the COM Port USB Cable between your board
//    and your computer.
// 4) Launch your preferred UART communications application (such as PuTTY or
//    HyperTerminal) and configure your computer to initialize a serial COM
//    connection with 230400 Baud, 8-N-1.
// 5) Attach a variable DC voltage source with a to the ADC Input
//    (ADC0.10, P1.2).
// 6) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) Follow the instructions printed out in the UART terminal to take
//    10-bit or 12-bit sampling data.
//
// Target:        C8051F85x
// Target Board:  UPMU-F850 MCU-100
// Tool chain:    Simplicity Studio / Keil C51 9.51
// Command Line:  None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 5 FEB 2014
//
// Release 1.0
//    - Initial Revision (PMD)
//    - 2 AUG 2013
//
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

#define SYSCLK          24500000         // SYSCLK frequency in Hz
#define BAUDRATE        230400           // Baud rate of UART in bps
#define DATA_SIZE       4096             // Number of samples to take
                                      
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART0_Init (void);

void ADC0_Init (void);
void ADC0_10Bit (void);
void ADC0_12Bit (void);
void ADC0_SampleData (void);
void ADC0_OffsetTest (void);

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
   WDTCN = 0xDE;
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   
   uint8_t inputChar;
   
   // Disable the watchdog timer

   // Initialize all components and peripherals
   SYSCLK_Init();
   UART0_Init();
   ADC0_Init();  
   Timer2_Init();
   Port_Init();             
 
   // Initialize Burst Mode at least once in initialization
   ADC0_12Bit(); 
   ADC0_10Bit();

   // Print opening text
   printf("F85x_ADC_Performance.c \n");
   printf("---------------------------------------------- \n");      
   printf("Copyright 2014 Silicon Laboratories, Inc.\n");
   printf("\n");
     
   // Disable global interrupts
   IE_EA = 0;
   while (1)
   {

	  printf("\n");

	  // Print directions to user.
      printf("---------------------------------------------- \n");
      printf("To take a sample using 10-bit Mode, Press 1. \n");
	  printf("To take a sample using 12-bit Mode, Press 2. \n");
      printf("To run the 10-bit offset error test, Press 3. \n");
	  printf("To run the 12-bit offset error test, Press 4. \n");
      printf("Enter your choice:");
      inputChar = getchar();

      printf("\f");

      // Checks if user has input an appropriate option, configures ADC mode.
      switch (inputChar)
      {
         // Test 1/2:
         // Perform ADC sample. Samples are captured with a sampling rate that
         // achieves maximum throughput. 
         case '1':                       
            ADC0_10Bit();              // Enable 10-Bit Mode Settings
			   ADC0_SampleData();
		   	break;

         case '2':
			   ADC0_12Bit();              // Enable 12-Bit Mode Settings
		      ADC0_SampleData();
			   break;

         // Test 3/4:
         // Perform ADC Offset Error test. a full set of ADC samples are
	      // taken and then averaged. This average is compared to the expected
		   // ADC value, after which error is calculated.
         case '3':  
			   ADC0_10Bit();              // Enable 10-Bit Mode Settings
			   ADC0_OffsetTest();
            break;

         case '4':
			   ADC0_12Bit();              // Enable 12-Bit Mode Settings
		      ADC0_OffsetTest();
			   break;

         default:
			   break;
		      
	  }
	  
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
   CLKSEL = 0x00;                      // Set system clock to 24.5 MHz
   RSTSRC = 0x06;                      // Enable missing clock detector
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
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   } 
   
   else if (SYSCLK/BAUDRATE/2/256 < 4) 
   {
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x01;
   } 
   
   else if (SYSCLK/BAUDRATE/2/256 < 12) 
   {
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   } 
   
   else 
   {
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   }

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xF0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                            // START Timer1
   SCON0_TI = 1;                             // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures ADC0 to make single-ended analog measurements on the ADC0.10
// input (P1.2).
//
// P1.2   analog         ADC0 (ADC Input)
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   ADC0CN0 = 0x02;                     // ADC0 disabled, conversions triggered
                                       // on Timer2 overflows

   ADC0CF = ((SYSCLK/12250000)-1)<<3;  // Set SAR clock to 12.25 MHz

   ADC0CF |= 0x01;                     // 1x gain, normal tracking mode,
                                       // 10-bit mode

   ADC0AC = 0x00;                      // 12-bit mode disabled, accumulation
                                       // disabled, right-justified, repeat
                                       // count set to 1

   ADC0MX = 0x0A;                      // ADC0 positive input = ADC0.10 or
                                       // P1.2

   REF0CN = 0x08;                      // Default Reference 

   ADC0CN1 |= 0x01;                    // Enable Common-Mode Buffer

   ADC0CN0_ADEN = 1;                           // Enable ADC0
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 0.5 ms
// intervals.  Timer 2 overflow automatically triggers ADC0 conversion.
//
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
                                       // Use SYSCLK as timebase, 16-bit
                                       // auto-reload
   CKCON |= 0x10;                      // Select SYSCLK for timer 2 source
   TMR2RL = -( (SYSCLK / 2000) + 1);   // Configure for 2 kHz rate
   TMR2 = 0xFFFF;                      // Set to reload immediately
   TMR2CN_TR2 = 1;                            // Start Timer2
}

//-----------------------------------------------------------------------------
// Runtime Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ADC0_10Bit
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Switches ADC0 to a 10-Bit configuration (no burst mode or repeats). Used
// when switching between different sampling and testing modes.
//
//-----------------------------------------------------------------------------
void ADC0_10Bit (void)
{

   ADC0AC = 0x00;                      // 12-bit mode disabled, accumulation
                                       // disabled, right-justified, repeat
                                       // count set to 1

   ADC0PWR = 0x44;
   ADC0TK = 0x3A;                      // Use default Tracking Time
   ADC0CN0 &= ~(0x40);                 // Disable Burst Mode

   TMR2RL = -( (SYSCLK / 1000) + 1);   // Configure for 800 kHz rate

}

//-----------------------------------------------------------------------------
// ADC0_12Bit
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Switches ADC0 to a 12-Bit configuration (burst mode enabled with repeat
// count set to four). Used when switching between different sampling and 
// testing modes.
//
//-----------------------------------------------------------------------------
void ADC0_12Bit (void)
{

   ADC0AC = 0x81;                      // 12-bit mode enabled, accumulation
                                       // disabled, right-justified, repeat
                                       // count set to 4

   ADC0PWR = 0x44;
   ADC0TK = 0x3A;                      // Use default Tracking Time
   ADC0CN0 |= 0x40;                    // Enable Burst Mode

   TMR2RL = -( (SYSCLK / 1000) + 1);   // Configure for 3.2 kHz rate

}

//-----------------------------------------------------------------------------
// ADC0_SampleData
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Initializes and waits for the ADC to sample data. As samples are acquired,
// they are immediately sent out to the terminal via UART.
//
//-----------------------------------------------------------------------------
void ADC0_SampleData (void)
{
   uint32_t totalADCSamples = 0;
  
   // Reset ADC status and buffer status
   totalADCSamples = 0;
   ADC0CN0_ADINT = 0;

   printf("\n");
   printf("ADC Sample Data: \n");  

   // Stay within loop until <DATA_SIZE> samples are collected.
   while(totalADCSamples < DATA_SIZE)
   {
      // Poll interrupt flag
      while( !ADC0CN0_ADINT ){}

	  // Send sample out via UART
      ADC0CN0_ADINT = 0;
      printf("%i \n", ADC0);
      totalADCSamples++;

   }

   // Prints directions to user, data must be transferred to the Excel file.
   printf("Locate the terminal's log file and copy the samples into \n");
   printf("F85x_ADC_Performance_SNR.xlsx to analyze the data. \n");
   printf("\n");

}

//-----------------------------------------------------------------------------
// ADC0_OffsetTest
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Initializes and waits for the ADC to sample data. As samples are acquired,
// they are accumulated into a running total and averaged. This average value
// is then compared to an expected value, and test results are output to
// the terminal.
//
//
//-----------------------------------------------------------------------------
void ADC0_OffsetTest (void)
{
   uint32_t totalADCSamples;
   uint32_t sumADCSamples;
   uint16_t avgADCSamples;
   uint16_t avgADCExpected;

   // Reset ADC status and buffer status
   totalADCSamples = 0;
   avgADCSamples = 0;
   sumADCSamples = 0;
   ADC0CN0_ADINT = 0;

   // Prints directions to user, data must be transferred to the Excel file.
   printf("Set the DC input voltage to 0 Volts. Once ready, press Return. \n");
   getchar();
   printf("\n");
   printf("Collecting samples for offset error test... \n");

   avgADCExpected = 0;  // Expected ADC Value for offset

   // Stay within loop until <DATA_SIZE> samples are collected.
   while(totalADCSamples < DATA_SIZE)
   {
      // Poll interrupt flag
      while( !ADC0CN0_ADINT ){}

	  // Send sample out via UART
      ADC0CN0_ADINT = 0;
      sumADCSamples += ADC0;
      totalADCSamples++;

   }

   avgADCSamples = sumADCSamples / DATA_SIZE;

   // Output results, including error calculation.
   printf("Offset Error Test Results: \n");
   printf(" - Average Value: %i \n", avgADCSamples);
   printf(" - Expected Value: %i \n", avgADCExpected);
   printf(" - Offset Error: %i \n", avgADCExpected - avgADCSamples);

}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
