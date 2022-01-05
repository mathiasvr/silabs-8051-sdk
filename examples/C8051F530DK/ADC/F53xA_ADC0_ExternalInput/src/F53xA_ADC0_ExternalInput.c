//-----------------------------------------------------------------------------
// F53xA_ADC0_ExternalInput.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This example code takes and averages 2048 analog measurements from input
// P1.2 using ADC0, then prints the results to a terminal window via the UART.
//
// The system is clocked by the internal 24.5MHz oscillator. Timer 2 triggers
// a conversion on ADC0 on each overflow. The completion of this conversion
// in turn triggers an interrupt service routine (ISR). The ISR averages
// 2048 measurements, then prints the value to the terminal via printf before
// starting another average cycle.
//
// The analog multiplexer selects P1.2 as the positive ADC0 input.  This
// port is configured as an analog input in the port initialization routine.
// The negative ADC0 input is connected via mux to ground, which provides
// for a single-ended ADC input.
//
// A 100kohm potentiometer may be connected as a voltage divider between
// VREF and AGND on the terminal strip as shown below:
//
// ---------
//          |
//         o| VREF ----|
//         o| GND   ---|<-|
//         o| P1.2     |  |
//         o|    |        |
//         o|     --------
//         o|
//          |
//----------
// C8051F530A-TB
//
// Terminal output is done via printf, which directs the characters to
// UART0. A UART initialization routine is therefore necessary.
//
// ADC Settling Time Requirements, Sampling Rate:
// ----------------------------------------------
//
// The total sample time per input is comprised of an input setting time
// (Tsettle), followed by a conversion time (Tconvert):
//
// Tsample  = Tsettle + Tconvert
//
// |--------Settling-------|==Conversion==|----Settling--- . . .
// Timer 2 overflow        ^
// ADC0 ISR                               ^
//
// The ADC input voltage must be allowed adequate time to settle before the
// conversion is made.  This settling depends on the external source
// impedance, internal mux impedance, and internal capacitance.
// Settling time is given by:
//
//                   | 2^n |
//    Tsettle =   ln | --- | * Rtotal * Csample
//                   | SA  |
//
// In this application, assume a 100kohm potentiometer as the voltage divider.
// The expression evaluates to:
//
//                   | 2^12 |
//    Tsettle =   ln | ---- | * 105e3 * 12e-12 = 12.2us
//                   | 0.25 |
//
// In addition, one must allow at least 1.5us after changing analog mux
// inputs or PGA settings.  The settling time in this example, then, is
// dictated by the large external source resistance.
//
// The conversion is 10 periods of the SAR clock <SAR_CLK>.  At 3 MHz,
// this time is 10 * 400nS = 3.3 us.
//
//
// Tsample, minimum  = Tsettle + Tconvert
//                   = 12.2us + 3.3us
//                   = 15.5 us
//
// Timer 2 is set to start a conversion every 100us, which is far longer
// than the minimum required.
//
//
// How to Test:
// ------------
// 1) Ensure shorting blocks on C8051F530A TB are placed on the following:
//    "A" SIDE
//    HDR4: TX_A - P0.4_A
//    HDR4: RX_A - P0.5_A
// 2) Connect the potentiometer to P1.2 by populating J6 and J8.
// 3) Connect the USB Debug Adapter to HDR2 for "A" SIDE.
//    Connect the 9V power adapter to P5.
// 4) Connect a USB cable from P1 on the TB to the PC. (The USB ACTIVE
//    LED should light up if the CP210x drivers are properly installed).
//    Specify the target baudrate in the constant <BAUDRATE>.
// 5) Open Hyperterminal, or a similar terminalprogram, and connect to the
//    target board's serial port using <BAUDRATE> 8-N-1. (CP210x USB-to-UART
//    VCP drivers must be installed).
// 6) Compile and download code to C8051F530A target board "A" SIDE with
//    Simplicity IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 8) HyperTerminal will print the voltage measured by the device if
//    everything is working properly.
//
//
// Target:         C8051F52xA/F53xA (C8051F530A TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.3 / 09 SEP 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.2 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.1
//    -Updated for C8051F530A TB (TP)
//    -29 JAN 2008
//
// Release 1.0
//    -Initial Revision (SM)
//    -14-Jun-07


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F530_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // SYSCLK frequency in Hz
#define BAUDRATE     115200            // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void Timer2_Init(void);
void ADC0_Init(void);
void UART0_Init (void);

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{

   SYSCLK_Init ();                     // Initialize system clock to
                                       // 24.5MHz
   PORT_Init ();                       // Initialize crossbar and GPIO
   Timer2_Init( );                     // Init Timer2 to generate
                                       // overflows to trigger ADC
   UART0_Init ();                      // Initialize UART0 for printf's
   ADC0_Init ();                       // Initialize ADC0

   IE_EA = 1;                          // Enable global interrupts
   while (1);                          // Spin forever
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at 24.5 MHz.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   // Set internal oscillator to run at its maximum frequency
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the Crossbar and GPIO ports.
// P0.0 - VREF (analog)
//
// P0.4 - UART TX (push-pull)
// P0.5 - UART RX
//
// P1.2 - Potentiometer ADC0 analog input
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P0MDOUT |= 0x10;                    // Set TX pin to push-pull

   P0MDIN &= ~0x01;                    // Set P0.0 as an analog pin
   P1MDIN &= ~0x04;                    // Set P1.2 as an analog input

   P0SKIP = 0x01;                      // Skip P0.0
   P1SKIP = 0x04;                      // Skip P1.2

   XBR0 = XBR0_URT0E__ENABLED;         // Enable UART0
   // Enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED |
          XBR1_XBARE__ENABLED;
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 100us
// intervals.  Timer 2 overflow automatically triggers ADC0 conversion.
//
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSCLK as timebase; 16-bit
                                       // auto-reload
   CKCON  |= 0x10;                     // Select SYSCLK for Timer2 source

   TMR2RL = -(SYSCLK / 10000);         // Initialize reload value for 100us
   TMR2 = TMR2RL;
   TMR2CN_TR2 = 1;                     // Start Timer2
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures ADC0 to make single-ended analog measurements on pin P1.2
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   ADC0CN = 0x03;                      // ADC0 disabled, normal tracking,
                                       // conversion triggered on TMR2 overflow
   REF0CN = 0x13;                      // Use internal voltage reference (2.2V)

   ADC0MX = 0x0A;                      // P1.2 selected as input

   // Set SAR clock to 3MHz
   ADC0CF = ((SYSCLK/3000000)-1)<<3;

   EIE1 |= EIE1_EADC0__ENABLED;        // Enable ADC0 conversion complete int.

   ADC0CN_ADEN = 1;                    // Enable ADC0
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the UART0 using Timer1 for <BAUDRATE> and 8-N-1.
//
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits
   if (SYSCLK/BAUDRATE/2/256 < 1) {
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON |=  0x08;                  // T1M = 1; SCA1:0 = xx
   } else if (SYSCLK/BAUDRATE/2/256 < 4) {
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x01;
   } else if (SYSCLK/BAUDRATE/2/256 < 12) {
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   } else if (SYSCLK/BAUDRATE/2/256 < 48) {
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   } else {
      while (1);                       // Error.  Unsupported baud rate
   }

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                       // START Timer1
   SCON0_TI = 1;                       // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// ADC0_ISR
//-----------------------------------------------------------------------------
//
// This ISR averages 2048 samples then prints the result to the terminal.  The
// ISR is called after each ADC conversion which is triggered by Timer2.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC0_ISR, ADC0EOC_IRQn)
{
   static uint32_t accumulator = 0;               // Accumulator for averaging
   static uint16_t measurements = 2048;           // Measurement counter
   uint32_t result=0;
   uint32_t mV;                                   // Measured voltage in mV

   ADC0CN_ADINT = 0;                         // Clear ADC0 conv. complete flag

   accumulator += ADC0;
   measurements--;

   if(measurements == 0)
   {
      measurements = 2048;
      result = accumulator / 2048;
      accumulator = 0;

      // The 12-bit ADC value is averaged across 2048 measurements.
      // The measured voltage applied to P1.2 is then:
      //
      //                           Vref (mV)
      //   measurement (mV) =   --------------- * result (bits)
      //                       (2^12)-1 (bits)

      mV =  result * 2200 / 4096;
      printf("P1.2 voltage: %ld mV\n",mV);
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------