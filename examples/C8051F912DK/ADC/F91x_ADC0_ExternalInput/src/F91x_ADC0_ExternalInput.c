//-----------------------------------------------------------------------------
// F91x_ADC0_ExternalInput.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This example code takes and averages 2048 analog measurements from input
// P0.6 (Potentiometer input) using ADC0, then prints the results to a terminal 
// window via the UART.
//
// The system is clocked by the internal 24.5MHz oscillator.  Timer 2 triggers
// a conversion on ADC0 on each overflow.  The completion of this conversion
// in turn triggers an interrupt service routine (ISR).  The ISR averages
// 2048 measurements, then prints the value to the terminal via printf before
// starting another average cycle.
//
// The analog multiplexer selects P0.6 as the ADC0 input. All samples are 
// single-ended with respect to GND. P0.6 is configured as an analog input 
// in the port initialization routine.
//
// When J16 is shorted, the potentiometer (R14) wiper is connected to P0.6.
// When J16 is open, P0.6 may also be supplied with a voltage from the H1 
// Terminal block. 
//
// Terminal output is done via printf, which directs the characters to
// UART0.  A UART initialization routine is therefore necessary.
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
//                   | 2^10 |
//    Tsettle =   ln | ---- | * 105e3 * 5e-12 = 4.4uS
//                   | 0.25 |
//
// In addition, one must allow at least 1.7uS after changing analog mux
// inputs or PGA settings.  The settling time in this example, then, is
// dictated by the large external source resistance.
//
// The conversion is 10 periods of the SAR clock <SAR_CLK>.  At 8.33 MHz,
// this time is 10 * 120nS = 1.2 uS.
//
//
// Tsample, minimum  = Tsettle + Tconvert
//                   = 4.4uS + 1.2uS
//                   = 5.6 uS
//
// Timer 2 is set to start a conversion every 100uS, which is far longer
// than the minimum required.
//
// F93x Resources:
// ---------------
// Timer1: clocks UART
// Timer2: overflow initiates ADC conversion
//
// How To Test:
// ------------
// 1) Ensure that jumpers are placed on the following:
//       J12:  P0.4/TX
//             P0.5/RX
//             VDD/VIO
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect a USB cable from the development board P3 to a PC
// 3) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the USB port (virtual com port) at <BAUDRATE> and 8-N-1
// 4) Connect a variable voltage source to P0.6 or verify that J15 and J16 are
//    installed. J15 should be connected to the side labeled GND.
// 5) Connect the USB Debug Adapter to J4.
// 6) Turn the POWER switch (SW5) to the "ON" position.
// 7) Compile and download code to a 'F91x device on a C8051F91x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) HyperTerminal will print the voltage measured by the device if
//    everything is working properly
//
// Target:         C8051F90x-C8051F91x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.4 (BL)
//    - Updated Description / How to Test
//    - 13 JAN 2014
//
// Release 1.3 
//    - Updated to be compatible with Raisonance (FB)
//    - 17 MAY 10
//
// Release 1.2
//    - Compile and test C8051F912_defs.h on 912 board (JH)
//    - Port to C8051F912-TB from C8051F930-TB (JH)
//    - 06 JULY 09
//
// Release 1.1 
//    - Updated to be compatible with SDCC (FB)
//    - 14 NOV 07
//
// Release 1.0
//    - Initial Revision (FB)
//    - 16 AUG 07

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // SYSCLK frequency in Hz
#define BAUDRATE     230400            // Baud rate of UART in bps

SI_SBIT(R15_ENABLE, SFR_P1, 4);          // Open-Drain output. Write '0' to 
                                       // enable the potentiometer.

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void Timer2_Init(void);
void ADC0_Init(void);
void UART0_Init (void);

SI_INTERRUPT_PROTO(ADC_ISR, ADC0EOC_IRQn);


//-----------------------------------------------------------------------------
// Generic UART definitions to support both Keil and SDCC
//-----------------------------------------------------------------------------

#ifdef SDCC
#include <sdcc_stdio.h>
#endif

char *GETS (char *, uint16_t);

#ifdef __C51__
#define GETS gets
#endif

#ifdef SDCC
#include <sdcc_stdio.c>
#endif

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
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
                                       // enable)
   
   PORT_Init ();                       // Initialize crossbar and GPIO
   SYSCLK_Init ();                     // Initialize system clock to
                                       // 24.5MHz
   
   Timer2_Init();                      // Init Timer2 to generate
                                       // overflows to trigger ADC
   UART0_Init();                       // Initialize UART0 for printf's
   ADC0_Init();                        // Initialize ADC0
   
   R15_ENABLE = 0;                     // Enable the Potentiometer

   IE_EA = 1;                             // enable global interrupts
   while (1) {                         // spin forever
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// This routine initializes the system clock to use the internal 24.5MHz / 8
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   OSCICN |= 0x80;                     // Enable the precision internal osc.
   
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc. 
                                       // divided by 1 as the system clock
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the Crossbar and GPIO ports.
//
// P0.4 - UART TX (push-pull)
// P0.5 - UART RX
//
// P0.6 - ADC0 analog input
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Analog Input
   P0MDIN   &= ~0x40;                  // Set P0.6 as an analog input
   P0MDOUT  &= ~0x40;                  // Set P0.6 to open-drain
   P0       |=  0x40;                  // Set P0.6 latch to '1'
   P0SKIP   |=  0x40;                  // Skip P0.6 in the Crossbar
   
   // UART TX 
   P0MDOUT |= 0x10;                    // Set TX pin to push-pull
   
   // Crossbar Configuration
   XBR0     = 0x01;                    // Enable UART0
   XBR2     = 0x40;                    // Enable crossbar and weak pull-ups
      
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 100uS
// intervals.  Timer 2 overflow automatically triggers ADC0 conversion.
//
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSCLK as timebase, 16-bit
                                       // auto-reload
   CKCON  |= 0x10;                     // select SYSCLK for timer 2 source
   TMR2RL  = 65535 - (SYSCLK / 10000); // init reload value for 100uS
   TMR2    = 0xffff;                   // set to reload immediately
   TMR2CN_TR2     = 1;                        // start Timer2
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures ADC0 to make single-ended analog measurements on pin P1.4
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   ADC0CN = 0x02;                      // ADC0 disabled, normal tracking,
                                       // conversion triggered on TMR2 overflow

   REF0CN = 0x18;                      // Select internal high speed voltage
                                       // reference

   ADC0MX = 0x06;                      // Select P0.6 as the ADC input pin

   ADC0CF = ((SYSCLK/8300000))<<3;     // Set SAR clock to 8.3MHz

   ADC0CF |= 0x00;                     // Select Gain of 0.5
   
   ADC0AC = 0x00;                      // Right-justify results, no shifting
                                       // applied

   EIE1 |= 0x08;                       // Enable ADC0 conversion complete int.

   ADC0CN_ADEN = 1;                          // Enable ADC0
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear SCON0_RI and SCON0_TI bits
   #if (SYSCLK/BAUDRATE/2/256 < 1) 
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   #elif (SYSCLK/BAUDRATE/2/256 < 4) 
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x01;
   #elif (SYSCLK/BAUDRATE/2/256 < 12) 
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   #else 
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   #endif

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                            // START Timer1
   SCON0_TI = 1;                            // Indicate TX0 ready
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
SI_INTERRUPT(ADC_ISR, ADC0EOC_IRQn)
{

   static uint32_t accumulator = 0;    // accumulator for averaging
   static uint16_t measurements = 2048; // measurement counter
   uint32_t result=0;
   uint32_t mV;                        // measured voltage in mV

   ADC0CN_ADINT = 0;                              // clear ADC0 conv. complete flag

   accumulator += ADC0;
   measurements--;

   if(measurements == 0)
   {
      measurements = 2048;
      result = accumulator / 2048;
      accumulator=0;

      // The 10-bit ADC value is averaged across 2048 measurements.
      // The measured voltage applied to P1.4 is then:
      //
      //                           Vref (mV)
      //   measurement (mV) =   --------------- * result (bits)
      //                       (2^10)-1 (bits)

      mV =  result * 3300 / 1023;
      printf("\f\nP0.6 voltage: %d mV", (uint16_t) mV);
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
