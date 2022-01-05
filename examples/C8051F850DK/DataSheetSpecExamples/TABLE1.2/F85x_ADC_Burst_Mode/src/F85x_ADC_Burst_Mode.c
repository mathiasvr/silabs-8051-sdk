//-----------------------------------------------------------------------------
// F85x_ADC_Burst_Mode.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
//
// Refer to F85x_ADC_Bust_Mode_Readme.txt for more information regarding the
// functionality of this code example.
//
//
// How To Measure (Table 1.2 - ADC0 Burst Mode, 10 Bit, Ext Ref, 200 ksps):
// ------------------------------------------------------------------------
// 1) Ensure a shorting block is placed across Imeasure (JP2).
// 2) Place VDD Select (SW1) in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 4) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Remove all of the shorting blocks on the board except for the block(s)
//    across the following pins:
//    a) POT and P1.2 on J27
//    b) S1 and P1.7 on J27
//    c) S2 and P2.1 on J27
// 7) Measure the current across Imeasure. This is the current with the ADC
//    burst mode enabled.
// 8) Use the switch at P2.1 (S2) to disable the ADC.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 Burst Mode, 10 bit single conversions,
//    external reference sampling at 200 ksps under Table 1.2.
//
//
// How To Measure (Table 1.2 - ADC0 Burst Mode, 10 Bit, Ext Ref, 100 ksps):
// ------------------------------------------------------------------------
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
// 6) Press the switch at P1.7 (S1) once. This will initialize the ADC for 10
//    bit burst mode using the external reference and sampling rate of 100 ksps.
// 7) Measure the current across Imeasure. This reading is the current with
//    the ADC burst mode enabled.
// 8) Use the switch at P2.1 (S2) to disable the ADC.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 Burst Mode, 10 bit single conversions,
//    external reference sampling at 100 ksps under Table 1.2.
//
//
// How To Measure (Table 1.2 - ADC0 Burst Mode, 10 Bit, Ext Ref, 10 ksps):
// -----------------------------------------------------------------------
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
// 6) Press the switch at P1.7 (S1) twice. This will initialize the ADC for 10
//    bit burst mode using the external reference and sampling rate of 10 ksps.
// 7) Measure the current across Imeasure. This reading is the current with
//    the ADC burst mode enabled.
// 8) Use the switch at P2.1 (S2) to disable the ADC.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 Burst Mode, 10 bit single conversions,
//    external reference sampling at 10 ksps under Table 1.2.
//
//
// How To Measure (Table 1.2 - ADC0 Burst Mode, 10 Bit, Int Ref, 200 ksps):
// ------------------------------------------------------------------------
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
// 6) Press the switch at P1.7 (S1) three times. This will initialize the ADC
//    for 10 bit burst mode using the internal reference and sampling rate of
//    200 ksps.
// 7) Measure the current across Imeasure. This reading is the current with
//    the ADC burst mode enabled.
// 8) Use the switch at P2.1 (S2) to disable the ADC.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 Burst Mode, 10 bit single conversions,
//    internal reference sampling at 200 ksps under Table 1.2.
//
//
// How To Measure (Table 1.2 - ADC0 Burst Mode, 10 Bit, Int Ref, 100 ksps):
// ------------------------------------------------------------------------
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
// 6) Press the switch at P1.7 (S1) four times. This will initialize the ADC
//    for 10 bit burst mode using the internal reference and sampling rate of
//    100 ksps.
// 7) Measure the current across Imeasure. This reading is the current with
//    the ADC burst mode enabled.
// 8) Use the switch at P2.1 (S2) to disable the ADC.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 Burst Mode, 10 bit single conversions,
//    internal reference sampling at 100 ksps under Table 1.2.
//
//
// How To Measure (Table 1.2 - ADC0 Burst Mode, 10 Bit, Int Ref, 10 ksps):
// -----------------------------------------------------------------------
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
// 6) Press the switch at P1.7 (S1) five times. This will initialize the ADC
//    for 10 bit burst mode using the internal reference and sampling rate of
//    10 ksps.
// 7) Measure the current across Imeasure. This reading is the current with
//    the ADC burst mode enabled.
// 8) Use the switch at P2.1 (S2) to disable the ADC.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 Burst Mode, 10 bit single conversions,
//    internal reference sampling at 10 ksps under Table 1.2.
//
//
// How To Measure (Table 1.2 - ADC0 Burst Mode, 12 Bit, Ext Ref, 100 ksps):
// ------------------------------------------------------------------------
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
// 6) Press the switch at P1.7 (S1) six times. This will initialize the ADC
//    for 12 bit burst mode using the external reference and sampling rate of
//    100 ksps.
// 7) Measure the current across Imeasure. This reading is the current with
//    the ADC burst mode enabled.
// 8) Use the switch at P2.1 (S2) to disable the ADC.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 Burst Mode, 12 bit single conversions,
//    external reference sampling at 100 ksps under Table 1.2.
//
//
// How To Measure (Table 1.2 - ADC0 Burst Mode, 12 Bit, Ext Ref, 50 ksps):
// -----------------------------------------------------------------------
// 1) Ensure a shorting block is placed across Imeasure (JP2).
// 2) Place VDD Select (SW1) in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 4) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Remove all of the shorting blocks on the board except for the block(s)
//    across the following pins:
//    a) POT and P1.2 on J27
//    b) S1 and P1.7 on J27
//    c) S2 and P2.1 on J27
// 7) Press the switch at P1.7 (S1) seven times. This will initialize the ADC
//    for 12 bit burst mode using the external reference and sampling rate of
//    50 ksps.
// 8) Measure the current across Imeasure. This reading is the current with
//    the ADC burst mode enabled.
// 9) Use the switch at P2.1 (S2) to disable the ADC.
// 10) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 Burst Mode, 12 bit single conversions,
//    external reference sampling at 50 ksps under Table 1.2.
//
//
// How To Measure (Table 1.2 - ADC0 Burst Mode, 12 Bit, Ext Ref, 10 ksps):
// -----------------------------------------------------------------------
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
// 6) Press the switch at P1.7 (S1) eight times. This will initialize the ADC
//    for 12 bit burst mode using the external reference and sampling rate of
//    10 ksps.
// 7) Measure the current across Imeasure. This reading is the current with
//    the ADC burst mode enabled.
// 8) Use the switch at P2.1 (S2) to disable the ADC.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 Burst Mode, 12 bit single conversions,
//    external reference sampling at 10 ksps under Table 1.2.
//
//
// How To Measure (Table 1.2 - ADC0 Burst Mode, 12 Bit, Int Ref, 100 ksps):
// ------------------------------------------------------------------------
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
// 6) Press the switch at P1.7 (S1) nine times. This will initialize the ADC
//    for 12 bit burst mode using the internal reference and sampling rate of
//    100 ksps.
// 7) Measure the current across Imeasure. This reading is the current with
//    the ADC burst mode enabled.
// 8) Use the switch at P2.1 (S2) to disable the ADC.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 Burst Mode, 12 bit single conversions,
//    internal reference sampling at 100 ksps under Table 1.2.
//
//
// How To Measure (Table 1.2 - ADC0 Burst Mode, 12 Bit, Int Ref, 50 ksps):
// -----------------------------------------------------------------------
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
// 6) Press the switch at P1.7 (S1) ten times. This will initialize the ADC
//    for 12 bit burst mode using the internal reference and sampling rate of
//    50 ksps.
// 7) Measure the current across Imeasure. This reading is the current with
//    the ADC burst mode enabled.
// 8) Use the switch at P2.1 (S2) to disable the ADC.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 Burst Mode, 12 bit single conversions,
//    internal reference sampling at 50 ksps under Table 1.2.
//
//
// How To Measure (Table 1.2 - ADC0 Burst Mode, 12 Bit, Int Ref, 10 ksps):
// -----------------------------------------------------------------------
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
// 6) Press the switch at P1.7 (S1) eleven times. This will initialize the ADC
//    for 12 bit burst mode using the internal reference and sampling rate of
//    10 ksps.
// 7) Measure the current across Imeasure. This reading is the current with
//    the ADC burst mode enabled.
// 8) Use the switch at P2.1 (S2) to disable the ADC.
// 9) Record the current reading and subtract the value from the previously
//    recorded value. Verify the difference is within the range of the values
//    listed on the datasheet for ADC0 Burst Mode, 12 bit single conversions,
//    internal reference sampling at 10 ksps under Table 1.2.
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
//    - 25 JUL 2013
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

#define ADC0_EXT_REF       0
#define ADC0_INT_REF       1

#define ADC0_10_BIT        0
#define ADC0_12_BIT        1

#define ADC0_BURST_10_BIT_EXT_REF_200_KSPS   0x00
#define ADC0_BURST_10_BIT_EXT_REF_100_KSPS   0x01
#define ADC0_BURST_10_BIT_EXT_REF_10_KSPS    0x02
#define ADC0_BURST_10_BIT_INT_REF_200_KSPS   0x03
#define ADC0_BURST_10_BIT_INT_REF_100_KSPS   0x04
#define ADC0_BURST_10_BIT_INT_REF_10_KSPS    0x05
#define ADC0_BURST_12_BIT_EXT_REF_100_KSPS   0x06
#define ADC0_BURST_12_BIT_EXT_REF_50_KSPS    0x07
#define ADC0_BURST_12_BIT_EXT_REF_10_KSPS    0x08
#define ADC0_BURST_12_BIT_INT_REF_100_KSPS   0x09
#define ADC0_BURST_12_BIT_INT_REF_50_KSPS    0x0A
#define ADC0_BURST_12_BIT_INT_REF_10_KSPS    0x0B

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
   uint8_t nextTestMode = ADC0_BURST_10_BIT_EXT_REF_100_KSPS;
   uint16_t debounceCounter;

   // Disable the watchdog timer

   ADC0_Init (ADC0_EXT_REF,            // Initialize ADC0 to use external ref,
              ADC0_10_BIT);            // 10 bit conversions
   SYSCLK_Init ();                     // Initialize system clock to 24.5 MHz
   Port_Init ();                       // Initialize crossbar and GPIO
   Timer2_Init (200000);               // Init Timer2 to generate overflows
                                       // to trigger ADC at 200 kHz

   IE_EA = 1;                             // Enable global interrupts

   // Loop forever
   while (1)
   {
      if ((S1 == SW_PRESSED) && (S2 != SW_PRESSED))
      {
         switch (nextTestMode)
         {
            case ADC0_BURST_10_BIT_EXT_REF_200_KSPS:
               // Setup ADC0 to use external reference, 10 bit conversions
               ADC0_Init (ADC0_EXT_REF, ADC0_10_BIT);

               // Init Timer2 to generate overflows to trigger ADC at 200 kHz
               Timer2_Init (200000);

               nextTestMode = ADC0_BURST_10_BIT_EXT_REF_100_KSPS;
               break;

            case ADC0_BURST_10_BIT_EXT_REF_100_KSPS:
               // Init Timer2 to generate overflows to trigger ADC at 100 kHz
               Timer2_Init (100000);

               nextTestMode = ADC0_BURST_10_BIT_EXT_REF_10_KSPS;
               break;

            case ADC0_BURST_10_BIT_EXT_REF_10_KSPS:
               // Init Timer2 to generate overflows to trigger ADC at 10 kHz
               Timer2_Init (10000);

               nextTestMode = ADC0_BURST_10_BIT_INT_REF_200_KSPS;
               break;

            case ADC0_BURST_10_BIT_INT_REF_200_KSPS:
               // Setup ADC0 to use internal reference at 1.65V, 10 bit
               // conversions
               ADC0_Init (ADC0_INT_REF, ADC0_10_BIT);

               // Init Timer2 to generate overflows to trigger ADC at 200 kHz
               Timer2_Init (200000);

               nextTestMode = ADC0_BURST_10_BIT_INT_REF_100_KSPS;
               break;

            case ADC0_BURST_10_BIT_INT_REF_100_KSPS:
               // Init Timer2 to generate overflows to trigger ADC at 100 kHz
               Timer2_Init (100000);

               nextTestMode = ADC0_BURST_10_BIT_INT_REF_10_KSPS;
               break;

            case ADC0_BURST_10_BIT_INT_REF_10_KSPS:
               // Init Timer2 to generate overflows to trigger ADC at 10 kHz
               Timer2_Init (10000);

               nextTestMode = ADC0_BURST_12_BIT_EXT_REF_100_KSPS;
               break;

            case ADC0_BURST_12_BIT_EXT_REF_100_KSPS:
               // Setup ADC0 to use external reference, 12 bit conversions
               ADC0_Init (ADC0_EXT_REF, ADC0_12_BIT);

               // Init Timer2 to generate overflows to trigger ADC at 100 kHz
               Timer2_Init (100000);

               nextTestMode = ADC0_BURST_12_BIT_EXT_REF_50_KSPS;
               break;

            case ADC0_BURST_12_BIT_EXT_REF_50_KSPS:
               // Init Timer2 to generate overflows to trigger ADC at 50 kHz
               Timer2_Init (50000);

               nextTestMode = ADC0_BURST_12_BIT_EXT_REF_10_KSPS;
               break;

            case ADC0_BURST_12_BIT_EXT_REF_10_KSPS:
               // Init Timer2 to generate overflows to trigger ADC at 10 kHz
               Timer2_Init (10000);

               nextTestMode = ADC0_BURST_12_BIT_INT_REF_100_KSPS;
               break;

            case ADC0_BURST_12_BIT_INT_REF_100_KSPS:
               // Setup ADC0 to use internal reference at 1.65V
               REF0CN = 0x18;

               // Init Timer2 to generate overflows to trigger ADC at 100 kHz
               Timer2_Init (100000);

               nextTestMode = ADC0_BURST_12_BIT_INT_REF_50_KSPS;
               break;

            case ADC0_BURST_12_BIT_INT_REF_50_KSPS:
               // Setup ADC0 to use internal reference at 1.65V, 12 bit
               // conversions
               ADC0_Init (ADC0_INT_REF, ADC0_12_BIT);

               // Init Timer2 to generate overflows to trigger ADC at 50 kHz
               Timer2_Init (50000);

               nextTestMode = ADC0_BURST_12_BIT_INT_REF_10_KSPS;
               break;

            case ADC0_BURST_12_BIT_INT_REF_10_KSPS:
               // Init Timer2 to generate overflows to trigger ADC at 10 kHz
               Timer2_Init (10000);

               nextTestMode = ADC0_BURST_10_BIT_EXT_REF_200_KSPS;
               break;

            default:
               // Error: Undefined state for nextTestMode
               while (1);
         }

         ADC0CN0_ADBMEN = 1;                   // Enable burst mode

         while (S1 == SW_PRESSED);     // Block until switch 1 is released

         for(debounceCounter = 0; debounceCounter < 20000; debounceCounter++);
      }

      else if ((S1 != SW_PRESSED) && (S2 == SW_PRESSED))
      {
         ADC0CN0_ADBMEN = !ADC0CN0_ADBMEN;             // Toggle ADC burst mode enable

         while (S2 == SW_PRESSED);     // Block until switch 2 is released

         for(debounceCounter = 0; debounceCounter < 20000; debounceCounter++);
      }

      // If both or none of the switches are pressed
      else
      {
         // Preserve state variables
         nextTestMode = nextTestMode;
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
// Parameters:    reference
//                resolution
//
// This routine uses the parameters reference and resolution to initialize the
// ADC for the test being performed. The reference can be either external or
// internal. The resolution can be either 10 bit or 12 bit.
//
//-----------------------------------------------------------------------------
void ADC0_Init (uint8_t reference, uint8_t resolution)
{
   ADC0CN0 = 0x42;                     // ADC0 disabled, burst mode enabled,
                                       // conversions triggered on Timer2
                                       // overflows

   ADC0CN1 |= 0x01;                    // Enable common mode buffer

   ADC0MX = 0x0A;                      // ADC0 positive input = ADC0.10 or
                                       // P1.2

   // Setup ADC0 for 12 bit conversions
   if(resolution == ADC0_12_BIT)
   {
      ADC0AC = 0x81;                   // 12-bit mode enabled, right-
                                       // justified, repeat count set to 1

      // ADC0 reference is internal
      if(reference == ADC0_INT_REF)
      {
         // Set SAR clock to 4 MHz
         ADC0CF = ((SYSCLK/SARCLK_LOW_POWER)-1)<<3;

         ADC0PWR = 0xF4;               // Enable Low Power settings

         ADC0TK = 0x34;                // Setup tracking time for 12 bit burst
                                       // mode using internal reference

         REF0CN = 0x18;                // Ground reference is the GND pin,
                                       // voltage reference is the internal
                                       // reference at 1.65V, temperature
                                       // sensor disabled
      }

      // ADC0 reference is external
      else
      {
         // Set SAR clock to 12.25 MHz
         ADC0CF = ((SYSCLK/SARCLK_HIGH_SPEED)-1)<<3;

         ADC0PWR = 0x44;               // Low power mode disabled, bias
                                       // current mode 2

         ADC0TK = 0x3A;                // Setup tracking time for 12 bit burst
                                       // mode using external reference

         REF0CN = 0x08;                // Ground reference is the GND pin,
                                       // voltage reference is the VDD pin,
                                       // temperature sensor disabled
      }
   }

   // Setup ADC0 for 10 bit conversions
   else
   {
      ADC0AC = 0x00;                   // 12 bit mode disabled, accumulation
                                       // disabled, right-justified, repeat
                                       // count set to 1

      // ADC0 reference is internal
      if(reference == ADC0_INT_REF)
      {
         // Set SAR clock to 4 MHz
         ADC0CF = ((SYSCLK/SARCLK_LOW_POWER)-1)<<3;

         ADC0PWR = 0xF4;               // Enable Low Power settings

         ADC0TK = 0x34;                // Setup tracking time for 10 bit burst
                                       // mode using internal reference

         REF0CN = 0x18;                // Ground reference is the GND pin,
                                       // voltage reference is the internal
                                       // reference at 1.65V, temperature
                                       // sensor disabled
      }

      // ADC0 reference is external
      else
      {
         // Set SAR clock to 12.25 MHz
         ADC0CF = ((SYSCLK/SARCLK_HIGH_SPEED)-1)<<3;

         ADC0PWR = 0x44;               // Low power mode disabled, bias
                                       // current mode 2

         ADC0TK = 0x3A;                // Setup tracking time for 10 bit burst
                                       // mode using external reference

         REF0CN = 0x08;                // Ground reference is the GND pin,
                                       // voltage reference is the VDD pin,
                                       // temperature sensor disabled
      }
   }

   ADC0CF |= 0x01;                     // 1x gain, normal tracking mode,
                                       // 10 bit/12 bit mode

   EIE1 |= 0x08;                       // Enable ADC0 conversion complete
                                       // interrupt
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
   P1MDOUT &= ~0x80;                   // Set P1.7 as open-drain
   P1 |= 0x84;                         // Set the P1.2 and P1.7 latch to '1'

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
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC_ISR, ADC0EOC_IRQn)
{
   ADC0CN0_ADINT = 0;                          // Clear ADC0 conv. complete flag
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
