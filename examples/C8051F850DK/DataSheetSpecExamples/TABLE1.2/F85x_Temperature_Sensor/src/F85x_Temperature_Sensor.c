//-----------------------------------------------------------------------------
// F85x_Temperature_Sensor.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// Refer to F85x_Temperature_Sensor_Readme.txt for more information 
// regarding this code example's functionality. 
//
//
// How To Measure (Table 1.2 - Power Consumption):
// -----------------------------------------------
// 1) Remove all shorting blocks on the board except for the block across S1 
//    and P1.7 on J27.
// 2) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 3) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 4) Measure the current across Imeasure (JP2).
// 5) Use switch at P1.7 (S1) to toggle the sensor modes. Skip Mode 0.
// 6) In Mode 1, the temperature sensor is activated but is not sending any
//    data to the UART. Meanwhile, in Mode 2 the temperature sensor is
//    completely disabled. Compare the Imeasure current values between
//    Mode 1 and Mode 2 to determine the amount of current added to the
//    system due to the temperature sensor.
//
//
// How To Measure (Table 1.9 - Voltage Offset):
// -----------------------------------------------
// 1) Remove all shorting blocks on the board except for the block across S1 
//    and P1.7 on J27, the JP2 (Imeasure) shorting block, and the shorting 
//    blocks on all three COM headers.
// 2) Launch your preferred UART communications application (such as PuTTY or
//    HyperTerminal) and configure your computer to initialize a serial COM
//    connection with 9600 Baud 8-N-1.
// 3) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) Use switch at P1.7 (S1) to toggle the sensor modes.
// 6) In Mode 0, the temperature sensor is activated and actively sending
//    data to the UART. To determine the offset of the temperature
//    sensor with this mode, measure the voltage reading sent to the UART
//    at a temperature of 0 degrees Celsius.
//
//
// How To Measure (Table 1.9 - Voltage Slope):
// -----------------------------------------------
// 1) Remove all shorting blocks on the board except for the block across S1 
//    and P1.7 on J27, the JP2 (Imeasure) shorting block, and the shorting 
//    blocks on all three COM headers.
// 2) Launch your preferred UART communications application (such as PuTTY or
//    HyperTerminal) and configure your computer to initialize a serial COM
//    connection with 9600 Baud 8-N-1.
// 3) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) Use switch at P1.7 (S1) to toggle the sensor modes.
// 6) In Mode 0, the temperature sensor is activated and actively sending
//    data to the UART. To determine the slope of the temperature
//    sensor with this mode, choose two different temperatures and
//    measure the voltage reading output from the MCU to the UART terminal
//    at each temperature point.
// 7) Using these two temperature-voltage readings, calculate the rate of
//    change of voltage with respect to temperature to determine slope.
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
//    - Initial Revision (BGD & PMD)
//    - 2 JUL 2013
//
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>
#include <intrins.h>                   // Keil Cx51 intrinsic code prototypes
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK                24500000 // SYSCLK frequency in Hz
#define BAUDRATE                  9600 // Baud rate of UART in bps
#define SAMPLING_NUMBER            256 // Sample Count

#define TRUE                         1
#define FALSE                        0

#define SW_PRESSED                   0
#define SW_NOT_PRESSED               1

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint32_t ADC_SUM = 0;                       // Accumulates the ADC samples
bit CONV_COMPLETE = FALSE;             // ADC_SUM result ready flag
uint32_t TEMP_SENSOR_MODE = 0;              

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(S1, SFR_P1, 7);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(ADC0_ISR, ADC0EOC_IRQn);
SI_INTERRUPT_PROTO(PM_ISR, PMATCH_IRQn);

void Oscillator_Init (void);
void Port_Init (void);
void Timer2_Init(void);
void UART0_Init(void);
void ADC0_Init(void);

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
   
   int32_t voltage = 0;                    // Formats UART voltage output
   int16_t voltageFrac = 0;
      
   // Disable the watchdog timer
   
   Oscillator_Init ();                 // Initialize system clock to 24.5 MHz
   ADC0_Init();                        // Initialize ADC to use TSENSE
   Timer2_Init ();                     // Initialize Timer 2 for periodic 
                                       // sampling of temperature sensor
									   
   Port_Init ();                       // Initialize crossbar and GPIO
   UART0_Init();                       // Initialize UART for output
	
   IE_EA = 1;                             // Enable global interrupts

   printf("\f");                       // Clear screen before output begins

   while (1) 
   {
      if( TEMP_SENSOR_MODE == 0 )      // Enable TSENSE and UART
	  {
         EIE1 |= 0x08;                 // Enable ADC0 conversion complete int.
	      TMR2CN |= 0x04;               // Enable Timer2
         P0MDOUT |= 0x10;              // Enable UART-TX push-pull
         REF0CN = 0x1C;                // Enable Temp. Sensor, VREF = 1.65 V
         ADC0CN0 |= 0x80;              // ADC enabled
		 	  
	     if( CONV_COMPLETE )
		  {

            // Convert ADC Raw Samples to mV
            voltage = (ADC_SUM * 100) / 256;
			   voltage = (voltage * 1650) / 1024;
            voltageFrac = voltage % 100;
			   voltage = voltage / 100;

            // Print current ADC/Voltage Status (from temp sensor)
            printf("\r  Voltage = %ld.%d mV   ", voltage, voltageFrac);
            CONV_COMPLETE = FALSE;

		  }
	  }

      else if( TEMP_SENSOR_MODE == 1 )      // Enable TSENSE Only
	   {
         EIE1 &= ~(0x08);              // Disable ADC0 conversion complete int.
	      TMR2CN &= ~(0x04);            // Disable Timer2
         P0MDOUT &= ~(0x10);           // Disable UART-TX push-pull
         REF0CN = 0x1C;                // Enable Temp. Sensor, VREF = 1.65 V
         ADC0CN0 &= ~0x80;             // ADC disabled
	   }

	  else if( TEMP_SENSOR_MODE == 2 ) // Disable TSENSE and UART
	  {
         EIE1 &= ~(0x08);              // Disable ADC0 conversion complete int.
	      TMR2CN &= ~(0x04);            // Disable Timer2
         P0MDOUT &= ~(0x10);           // Disable UART-TX push-pull
         REF0CN = 0x18;                // Disable Temp. Sensor
         ADC0CN0 &= ~0x80;             // ADC disabled
	  }
 
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal oscillator
// at its maximum frequency of 24.5 MHz and enables the Missing Clock Detector.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock

   RSTSRC = 0x06;                      // Enable missing clock detector
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
// P1.7   digital   open-drain   input   S1 (Switch 1)
// P0.4   digital   push-pull   output   UTX (UART Transmit)
// P0.5   digital   open-drain   input   URX (UART Receive)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{

   // Port 1 (S1) initialization
   P1MDIN |= 0x80;                     // Set P1.7 as digital
   P1MDOUT &= ~0x80;                   // Set P1.7 as open-drain
   P1 |= 0x80;                         // Set the P1.7 latch to '1'

   P1MAT  |= 0x80;                     // Set up the port-match value on P1.7
   P1MASK |= 0x80;                     // An event will occur if
                                       // (P1 & P1MASK) != (P1MAT & P1MASK)
                                       // The switch pins are 1 if the switch
                                       // isn't pressed.

   // UART Initialization
   P0MDOUT |= 0x10;                    // UART-TX push-pull
   XBR0 = 0x01;                        // Enable UART I/O
	
   // S1 uses Port Match ISR to interface while UART is running	
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
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 100 us
// intervals. Timer 2 overflows automatically trigger ADC0 conversions.
//
//
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2; use SYSCLK
                                       // as timebase, 16-bit auto-reload
   CKCON |= 0x10;                      // Select SYSCLK for timer 2 source
   TMR2RL = 65535 - (SYSCLK / 10000);  // Init reload value for 100uS
   TMR2 = 0xffff;                      // Set to reload immediately
   TMR2CN_TR2 = 1;                            // Start Timer2
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
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                            // START Timer1
   SCON0 |= 0x02;                      // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Initialize the ADC to use the temperature sensor.
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   REF0CN = 0x1C;                      // Temp. Sensor on, VREF = 1.65 V

   ADC0MX = 0x10;                      // Select Temp. Sensor

   ADC0AC = 0x20;                      // 10-bit mode, left justified
   ADC0CF |= 0x01;                     // 10-bit mode, normal tracking mode,
                                       // PGA gain = 1
   ADC0CN0 = 0x02;                     // ADC conversion starts on TMR2
                                       // overflow

   ADC0PWR = 0xC0;                     // Select bias current mode 3

   ADC0CN0 |= 0x80;                    // ADC enabled
   EIE1 |= 0x08;                       // Enable ADC0 conversion complete int.
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port Match ISR
//-----------------------------------------------------------------------------
//
// Whenever S1 (P1.7) is pressed, the Temperature Sensor enable is toggled.
//
// The Port Match interrupt occurs whenever (Pn & PnMASK) != (PnMAT & PnMASK).
// We must disable the interrupt and wait until the switches are released in
// main, or else the interrupt will trigger multiple times.  It is generally
// not good practice to sit in an ISR for int32_t periods of time.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PM_ISR, PMATCH_IRQn)
{
   if (S1 == SW_PRESSED)
   {
      TEMP_SENSOR_MODE++;
	  if(TEMP_SENSOR_MODE == 3)
	  {
	     TEMP_SENSOR_MODE = 0;         // Toggle Temperature Sensor Modes
	  }              

      P1MAT &= ~0x80;                  // Set port match to trigger at switch
                                       // rising edge to prevent multiple 
                                       // interrupts from occurring while the
                                       // switch is pressed   
   }
   else
   {
      P1MAT |= 0x80;                   // Reset port match to trigger at switch
                                       // falling edge
   }
}

//-----------------------------------------------------------------------------
// ADC0_ISR
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This ISR averages 256 samples then copies the result to ADC_SUM.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(ADC0_ISR, ADC0EOC_IRQn)
{
   static uint32_t temp = 0;
   static uint32_t accumulator = 0;                // Accumulator for averaging
   static uint16_t measurements = SAMPLING_NUMBER; // Measurement counter

   ADC0CN0 &= ~0x20;                   // Clear ADC0 conv. complete flag

   // Checks if obtained the necessary number of samples
   if(measurements == 0)
   {
      ADC_SUM = accumulator;           // Copy total into ADC_SUM
      measurements = 256;              // Reset counter
      accumulator = 0;                 // Reset accumulator
      CONV_COMPLETE = TRUE;            // Set result ready flag
   }
   else
   {
      temp = ADC0 >> 6;
      accumulator += ADC0 >> 6;        // If not, keep adding while shifting
                                       // out unused bits in ADC0
      measurements--;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
