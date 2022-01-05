//-----------------------------------------------------------------------------
// F85x_ADC_Continuous_Sample.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
//
// Refer to F85x_ADC_Continuous_Sample_Readme.txt for more information
// regarding the functionality of this code example.
//
//
// How To Measure (Table 1.2 - ADC High Power Current):
// ----------------------------------------------------
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
//    a) POT and P1.2 on J27
//    b) S1 and P1.7 on J27
//    c) S2 and P2.1 on J27
// 6) Measure the current across Imeasure. This reading is the current with
//    the ADC enabled.
// 7) Use the switch at P2.1 (S2) to disable the ADC.
// 8) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 always-on sampling at 800 ksps under
//    Table 1.2.
//
//
// How To Measure (Table 1.2 - ADC Low Power Current):
// ---------------------------------------------------
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
//    a) POT and P1.2 on J27
//    b) S1 and P1.7 on J27
//    c) S2 and P2.1 on J27
// 6) Press the switch at P1.7 (S1) once. This will initialize the ADC to run
//    at the lowest power settings and sample at 250 ksps.
// 7) Measure the current across Imeasure. This reading is the current with
//    the ADC enabled.
// 8) Use the switch at P2.1 (S2) to disable the ADC.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 always-on sampling at 250 ksps under
//    Table 1.2.
//
//
// How To Measure (Table 1.7 - ADC High Speed 12 Bit Throughput Rate):
// -------------------------------------------------------------------
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
//    a) POT and P1.2 on J27
//    b) S1 and P1.7 on J27
//    c) S2 and P2.1 on J27
// 6) Press the switch at P1.7 (S1) twice. This will initialize the ADC to run
//    at the High Speed 12 Bit mode.
// 7) Use an oscilloscope to monitor P1.0. A square wave should be seen at
//    this pin. Verify the frequency of this square wave is half the rate of
//    the sampling rate listed for ADC Throughput Rate in High Speed 12 Bit
//    mode in Table 1.7.
//
//
// How To Measure (Table 1.7 - ADC High Speed 10 Bit Throughput Rate):
// -------------------------------------------------------------------
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
//    a) POT and P1.2 on J27
//    b) S1 and P1.7 on J27
//    c) S2 and P2.1 on J27
// 6) Press the switch at P1.7 (S1) twice.
// 7) Press the switch at P2.1 (S2) once. This will initialize the ADC to run
//    at the High Speed 10 Bit mode.
// 8) Use an oscilloscope to monitor P1.0. A square wave should be seen at
//    this pin. Verify the frequency of this square wave is half the rate of
//    the sampling rate listed for ADC Throughput Rate in High Speed 10 Bit
//    mode in Table 1.7.
//
//
// How To Measure (Table 1.7 - ADC Low Power 12 Bit Throughput Rate):
// ------------------------------------------------------------------
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
//    a) POT and P1.2 on J27
//    b) S1 and P1.7 on J27
//    c) S2 and P2.1 on J27
// 6) Press the switch at P1.7 (S1) twice.
// 7) Press the switch at P2.1 (S2) three times. This will initialize the ADC
//    to run at the Low Power 12 Bit mode.
// 8) Use an oscilloscope to monitor P1.0. A square wave should be seen at
//    this pin. Verify the frequency of this square wave is half the rate of
//    the sampling rate listed for ADC Throughput Rate in Low Power 12 Bit
//    mode in Table 1.7.
//
//
// How To Measure (Table 1.7 - ADC Low Power 10 Bit Throughput Rate):
// ------------------------------------------------------------------
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
//    a) POT and P1.2 on J27
//    b) S1 and P1.7 on J27
//    c) S2 and P2.1 on J27
// 6) Press the switch at P1.7 (S1) twice.
// 7) Press the switch at P2.1 (S2) four times. This will initialize the ADC
//    to run at the Low Power 10 Bit mode.
// 8) Use an oscilloscope to monitor P1.0. A square wave should be seen at
//    this pin. Verify the frequency of this square wave is half the rate of
//    the sampling rate listed for ADC Throughput Rate in Low Power 10 Bit
//    mode in Table 1.7.
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
//    - 22 JUL 2013
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

#define SARCLK_HIGH_SPEED  12250000    // High speed SARCLK frequency in Hz
#define SARCLK_LOW_POWER   4000000     // Low power SARCLK frequency in Hz

#define SW_PRESSED         0
#define SW_NOT_PRESSED     1

#define ADC0_HIGH_SPEED    0
#define ADC0_LOW_POWER     1

#define ADC0_10_BIT        0
#define ADC0_12_BIT        1

#define CURRENT_HIGH_POWER 0
#define CURRENT_LOW_POWER  1
#define THROUGHPUT_RATE    2

#define HIGH_SPEED_12_BIT  0
#define HIGH_SPEED_10_BIT  1
#define LOW_POWER_12_BIT   2
#define LOW_POWER_10_BIT   3

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(S1, SFR_P1, 7);                   // Switch 1
SI_SBIT(S2, SFR_P2, 1);                   // Switch 2

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(ADC_ISR, ADC0EOC_IRQn);

void ADC0_Init(uint8_t, uint8_t);
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
   WDTCN = 0xDE;
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   // Local variables
   uint8_t currTestMode = CURRENT_HIGH_POWER;
   uint8_t nextTestMode = CURRENT_LOW_POWER;
   uint8_t nextThroughputRate = HIGH_SPEED_10_BIT;
   uint16_t debounceCounter;

   // Disable the watchdog timer

   ADC0_Init (ADC0_HIGH_SPEED,         // Initialize ADC0 to high speed/power,
              ADC0_10_BIT);            // 10 bit conversions
   SYSCLK_Init ();                     // Initialize system clock to 24.5 MHz
   Port_Init ();                       // Initialize crossbar and GPIO

   IE_EA = 1;                             // Enable global interrupts

   // Loop forever
   while (1)
   {
      if ((S1 == SW_PRESSED) && (S2 != SW_PRESSED))
      {
         switch (nextTestMode)
         {
            case CURRENT_HIGH_POWER:
               // Setup ADC0 for power consumption measurement at highest power
               // mode settings and a sampling rate of 800 ksps
               ADC0_Init (ADC0_HIGH_SPEED, ADC0_10_BIT);

               EIE1 &= ~0x08;          // Disable ADC0 conv. complete interrupt

               currTestMode = CURRENT_HIGH_POWER;
               nextTestMode = CURRENT_LOW_POWER;
               break;

            case CURRENT_LOW_POWER:
               // Setup ADC0 for power consumption measurement at lowest power
               // mode settings and a sampling rate of 250 ksps
               ADC0_Init (ADC0_LOW_POWER, ADC0_10_BIT);

               EIE1 &= ~0x08;          // Disable ADC0 conv. complete interrupt

               currTestMode = CURRENT_LOW_POWER;
               nextTestMode = THROUGHPUT_RATE;
               break;

            case THROUGHPUT_RATE:
               // Setup ADC0 for throughput measurement in high speed 12 bit
               // mode
               ADC0_Init (ADC0_HIGH_SPEED, ADC0_12_BIT);

               EIE1 |= 0x08;           // Enable ADC0 conv. complete interrupt

               currTestMode = THROUGHPUT_RATE;
               nextTestMode = CURRENT_HIGH_POWER;

               break;

            default:
               // Error: Undefined state for nextTestMode
               while (1);
         }

         while (S1 == SW_PRESSED);     // Block until switch 1 is released

         for(debounceCounter = 0; debounceCounter < 20000; debounceCounter++);
      }

      else if ((S1 != SW_PRESSED) && (S2 == SW_PRESSED))
      {
         // If measuring power consumption
         if((currTestMode == CURRENT_HIGH_POWER)
               || (currTestMode == CURRENT_LOW_POWER))
         {
            ADC0CN0_ADEN = !ADC0CN0_ADEN;              // Toggle ADC enable
         }
         // If measuring throughput rate
         else // currTestMode == THROUGHPUT_RATE
         {
            //
            switch (nextThroughputRate)
            {
               case HIGH_SPEED_12_BIT:
                  // Setup ADC0 for throughput measurement in high speed 12 bit
                  // mode
                  ADC0_Init (ADC0_HIGH_SPEED, ADC0_12_BIT);

                  nextThroughputRate = HIGH_SPEED_10_BIT;

                  break;

               case HIGH_SPEED_10_BIT:
                  // Setup ADC0 for throughput measurement in high speed 10 bit
                  // mode
                  ADC0_Init (ADC0_HIGH_SPEED, ADC0_10_BIT);

                  nextThroughputRate = LOW_POWER_12_BIT;

                  break;

               case LOW_POWER_12_BIT:
                  // Setup ADC0 for throughput measurement in low power 12 bit
                  // mode
                  ADC0_Init (ADC0_LOW_POWER, ADC0_12_BIT);

                  nextThroughputRate = LOW_POWER_10_BIT;

                  break;

               case LOW_POWER_10_BIT:
                  // Setup ADC0 for throughput measurement in low power 10 bit
                  // mode
                  ADC0_Init (ADC0_LOW_POWER, ADC0_10_BIT);

                  nextThroughputRate = HIGH_SPEED_12_BIT;

                  break;

               default:
                  // Error: Undefined state for nextThroughputRate
                  while (1);
            }
         }

         while (S2 == SW_PRESSED);     // Block until switch 2 is released

         for(debounceCounter = 0; debounceCounter < 20000; debounceCounter++);
      }
      // If both or none of the switches are pressed
      else
      {
         // Preserve state variables
         currTestMode = currTestMode;
         nextTestMode = nextTestMode;
         nextThroughputRate = nextThroughputRate;
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
// Parameters:    powerMode
//                resolution
//
// This routine uses the parameters powerMode and resolution to initialize the
// ADC for the test being performed. The power mode can be either high speed or
// low power. The resolution can be either 10 bit or 12 bit.
//
//-----------------------------------------------------------------------------
void ADC0_Init (uint8_t powerMode, uint8_t resolution)
{
   ADC0CN0 = 0x02;                     // ADC0 disabled, conversions triggered
                                       // on Timer2 overflows

   ADC0CN1 |= 0x01;                    // Enable common mode buffer

   ADC0MX = 0x0A;                      // ADC0 positive input = ADC0.10 or
                                       // P1.2

   REF0CN = 0x08;                      // Ground reference is the GND pin,
                                       // voltage reference is the VDD pin
                                       // (3.3 V), temperature sensor disabled

   // Initialize ADC0 with Low Power settings
   if(powerMode == ADC0_LOW_POWER)
   {
      // Set SAR clock to 4 MHz
      ADC0CF = ((SYSCLK/SARCLK_LOW_POWER)-1)<<3;

      ADC0CF |= 0x03;                  // 1x gain, delayed tracking mode,
                                       // 10 bit/12 bit mode

      ADC0PWR = 0xF0;                  // Enable Low Power settings

      // ADC0 12 bit resolution initialization settings
      if(resolution == ADC0_12_BIT)
      {
         ADC0CN0 |= 0x40;              // Burst mode enabled

         ADC0AC = 0x81;                // 12-bit mode enabled, right-
                                       // justified, repeat count set to 1

         ADC0TK = 0xBF;                // Setup tracking time for 12 bit

         Timer2_Init (62500);          // Init Timer2 to generate overflows
                                       // to trigger ADC at 62.5 kHz
      }

      // ADC0 10 bit resolution initialization settings
      else
      {
         ADC0AC = 0x00;                // 12 bit mode disabled, accumulation
                                       // disabled, right-justified, repeat
                                       // count set to 1

         Timer2_Init (250000);         // Init Timer2 to generate overflows
                                       // to trigger ADC at 250 kHz
      }
   }

   // ADC0 High Speed initialization settings
   else
   {
      // Set SAR clock to 12.25 MHz
      ADC0CF = ((SYSCLK/SARCLK_HIGH_SPEED)-1)<<3;

      ADC0CF |= 0x01;                  // 1x gain, normal tracking mode,
                                       // 10 bit/12 bit mode

      ADC0PWR = 0x40;                  // Low power mode disabled, bias
                                       // current mode 2

      // ADC0 12 bit resolution initialization settings
      if(resolution == ADC0_12_BIT)
      {
         ADC0CN0 |= 0x40;              // Burst mode enabled

         ADC0AC = 0x81;                // 12 bit mode enabled, right-
                                       // justified, repeat count set to 1

         ADC0TK = 0xBF;                // Setup tracking time for 12 bit

         Timer2_Init (200000);         // Init Timer2 to generate overflows
                                       // to trigger ADC at 200 kHz
      }

      // ADC0 10 bit resolution initialization settings
      else
      {
         ADC0AC = 0x00;                // 12 bit mode disabled, accumulation
                                       // disabled, right-justified, repeat
                                       // count set to 1

         Timer2_Init (800000);         // Init Timer2 to generate overflows
                                       // to trigger ADC at 800 kHz
      }
   }

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
// Configure the Crossbar and GPIO ports based on the characteristics under
// test.
//
// P1.0  digital  push-pull   Throughput Toggle Pin
// P1.2  analog               ADC0.10
// P1.7  digital  open-drain  S1 (Switch 1)
// P2.1  digital  open-drain  S2 (Switch 2)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // Port 1 Initialization
   P1MDIN &= ~0x04;                    // Set P1.2 as an analog input
   P1SKIP |= 0x04;                     // Skip P1.2 on crossbar
   P1MDOUT |= 0x01;                    // Set P1.0 as push-pull output
   P1MDOUT &= ~0x80;                   // Set P1.7 as open-drain
   P1 |= 0x85;                         // Set the P1.0, P1.2 & P1.7 latch to '1'

   // Port 2 Initialization
   P2MDOUT &= ~0x02;                   // Set P2.1 as open-drain
   P2 |= 0x02;                         // Set the P2.1 latch to '1'

   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    samplingRate
//
// Configure Timer2 to 16 bit auto-reload and generate an interrupt at the rate
// specified by samplingRate. Timer 2 overflow automatically triggers ADC0
// conversion. The Timer2 interrupt is enabled and used to clear the ADC
// interrupt when the user is not testing the throughput of the ADC.
//
//-----------------------------------------------------------------------------
void Timer2_Init (uint32_t samplingRate)
{
   TMR2CN = 0x00;                      // Stop Timer2 and clear TF2
                                       // Use SYSCLK as time base, 16 bit
                                       // auto-reload
   CKCON |= 0x10;                      // Select SYSCLK for Timer2 source

   // Set reload for ADC0 sampling rate
   TMR2RL = -((SYSCLK/samplingRate) + 1);

   TMR2 = 0xFFFF;                      // Set to reload immediately

   TMR2CN_TR2 = 1;                            // Start Timer2
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
   P1 ^= 0x01;                         // Invert the P1.0 latch
   ADC0CN0_ADINT = 0;                          // Clear ADC0 conv. complete flag
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
