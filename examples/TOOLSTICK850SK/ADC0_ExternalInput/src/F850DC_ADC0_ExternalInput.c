//-----------------------------------------------------------------------------
// F850DC_ADC0_ExternalInput.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This example code takes and averages 2048 analog measurements from input
// P1.2 (Potentiometer input) using ADC0. This measurement can be viewed by
// halting the example, or by connecting the device's UART to a UART receiver.
//
// The system is clocked by the internal 24.5 MHz oscillator divided by 1.
// Timer 2 triggers a conversion on ADC0 on each overflow.  The completion of
// this conversion in turn triggers an interrupt service routine (ISR).  The ISR
// averages 2048 measurements, then prints the value to the terminal via printf
// before starting another average cycle.
//
// The analog multiplexer selects P1.2 (ADC0.10) as the positive ADC0 input.
// This pin is configured as an analog input in the port initialization
// routine. The negative ADC0 input is connected via mux to ground, which
// provides for a single-ended ADC input.
//
// A 100 kohm potentiometer may be connected (J27) as a voltage divider between
// VDD (3.30 V) and GND on the terminal strip as shown below:
//
// ---------
//          |
//         o| VDD  ----|
//         o| GND   ---|<-|
//         o| P1.2     |  |
//         o|    |        |
//         o|     --------
//         o|
//          |
//----------
// C8051F850 UDP MCU Card
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
// In this application, assume a 100 kohm potentiometer as the voltage divider.
// The expression evaluates to:
//
//                   | 2^10 |
//    Tsettle =   ln | ---- | * 105e3 * 5e-12 = 4.4 us
//                   | 0.25 |
//
// In addition, one must allow at least 1.5uS after changing analog mux
// inputs or PGA settings.  The settling time in this example, then, is
// dictated by the large external source resistance.
//
// The conversion is given in the datasheet (10-bit mode, SAR clock 12.25 MHz)
// as 1 us.
//
// Tsample, minimum  = Tsettle + Tconvert
//                   = 4.4 us + 1 us
//                   = 5.4 u
//
// Timer2 is set to start a conversion every 100 us, which is far longer
// than the minimum required.
//
// F850 Resources:
// ---------------
// Timer1: clocks UART
// Timer2: overflow initiates ADC conversion
//
// How To Test:
// ------------
// 1) Connect the ToolStick850DC to a ToolStick Base Adapter.
// 2) Compile and download code to the ToolStick Daughter Card by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 3) Set a break-point at the last printf inside the ADC_ISR function
//    (line 368).
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) When the program halts, check the given voltage in the "mv" variable by
//    observing it in the Variables view. This variable stores the measured
//    voltage in millivolts: 3300 would equal 3.3V, for example.
// 6) Adjust the potentiometer and resume the program again. When it halts,
//    observe the "mv" variable again to see that the voltage has changed.
//
//
// Target:         ToolStick850DC
// Tool chain:     Simplicity Studio / Keil C51 9.51
//
// Release 1.2 (BL)
//    - Updated Description / How to Test
//    - 21 MAR 2014
//
// Release 1.1 (TP)
//    -Updated ADC0CN_ADEN, ADC0CN_ADINT, and SCON0_TI to the correct names
//    -1 AUG 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 30 APR 2013

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F850_Register_Enums.h"
#include "stdio.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // SYSCLK frequency in Hz
#define BAUDRATE       115200          // Baud rate of UART in bps

SI_SBIT(LED, SFR_P1, 0);                 // LED==0 means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(ADC_ISR, ADC0EOC_IRQn);

void Oscillator_Init (void);
void Port_Init (void);
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
   WDTCN = 0xDE;
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   // Disable the watchdog timer

   Oscillator_Init ();                 // Initialize system clock to 24.5 MHz
   Port_Init ();                       // Initialize crossbar and GPIO
   Timer2_Init();                      // Init Timer2 to generate
                                       // Overflows to trigger ADC
   UART0_Init();                       // Initialize UART0 for printf's
   ADC0_Init();                        // Initialize ADC0

   IE_EA = 1;                             // Enable global interrupts
   while (1) {}
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Oscillator_Init
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
void Oscillator_Init (void)
{
   CLKSEL = 0x00;                      // Set system clock to 24.5 MHz

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
// P0.4 - UART TX (push-pull)
// P0.5 - UART RX
//
// P1.2 - ADC0 analog input (ADC0.10)
// P1.3 - LED (push-pull)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // UART TX
   P0MDOUT |= 0x10;                    // Set TX pin to push-pull

   // LED
   P1MDOUT |= 0x01;                    // Enable LED as a push-pull output

   // Analog Input
   P1MDIN &= ~0x04;                    // Set P1.2 as an analog input
   P1 |= 0x04;
   P1SKIP |= 0x04;

   // Crossbar Configuration
   XBR0 = 0x01;                        // Enable UART0
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
// intervals.  Timer 2 overflow automatically triggers ADC0 conversion.
//
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
                                       // Use SYSCLK as timebase, 16-bit
                                       // auto-reload
   CKCON |= 0x10;                      // Select SYSCLK for timer 2 source
   TMR2RL = 65535 - (SYSCLK / 10000);  // Init reload value for 10 us
   TMR2 = 0xFFFF;                      // Set to reload immediately
   TMR2CN_TR2 = 1;                            // Start Timer2
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
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   ADC0CN0 = 0x02;                     // ADC0 disabled, conversions triggered
                                       // on Timer2 overflows

   ADC0CF = (((SYSCLK*2)/12250000)-1)<<3; // Set SAR clock to 12.25 MHz

   ADC0CF |= 0x01;                     // 1x gain, normal tracking mode,
                                       // 10-bit mode

   ADC0AC = 0x00;                      // 12-bit mode disabled, accumulation
                                       // disabled, right-justified, repeat
                                       // count set to 1

   ADC0PWR = 0x00;                     // Low power mode disabled

   ADC0MX = 0x0A;                      // ADC0 positive input = ADC0.10 or
                                       // P1.2

   REF0CN = 0x08;                      // Ground reference is the GND pin,
                                       // voltage reference is the VDD pin
                                       // (3.3 V), temperature sensor disabled

   EIE1 |= 0x08;                       // Enable ADC0 conversion complete
                                       // interrupt

   ADC0CN0_ADEN = 1;                           // Enable ADC0
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
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

   TL1 = TH1;                          // init Timer1
   TMOD &= ~0xF0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                            // START Timer1
   SCON0_TI = 1;                             // Indicate TX ready
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

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
   static uint32_t accumulator = 0;     // Accumulator for averaging
   static uint16_t measurements = 2048;  // Measurement counter
   uint32_t result = 0;
   uint32_t mV;                         // Measured voltage in mV

   ADC0CN0_ADINT = 0;                                // Clear ADC0 conv. complete flag

   accumulator += ADC0;
   measurements--;

   if(measurements == 0)
   {
      measurements = 2048;
      result = accumulator / 2048;
      accumulator = 0;

      // The 10-bit ADC value is averaged across 2048 measurements.
      // The measured voltage applied to P0.7 is then:
      //
      //                           Vref (mV)
      //   measurement (mV) =   --------------- * result (bits)
      //                       (2^10)-1 (bits)

      mV =  (result * 3300) / 1023;
      printf("P1.2 voltage: %ld mV\n",mV); // <--- Place break-point here
   }
}

//-----------------------------------------------------------------------------
// Support Functions
//-----------------------------------------------------------------------------

#if defined SDCC
void putchar (char c)
{
  if (c == '\n')
  {
    while (!SCON0_TI);
    SCON0_TI = 0;
    SBUF0 = 0x0d;                      /* output PCA0CN_CR  */
  }
  while (!SCON0_TI);
  SCON0_TI = 0;
  SBUF0 = c;
}
#endif

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
