//-----------------------------------------------------------------------------
// F33x_ADC0_ExternalInput.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This example code takes and averages 2048 analog measurements from input
// P1.4 using ADC0, then prints the results to a terminal window via the UART.
//
// The system is clocked by the internal 24.5MHz oscillator.  Timer 2 triggers
// a conversion on ADC0 on each overflow.  The completion of this conversion
// in turn triggers an interrupt service routine (ISR).  The ISR averages 
// 2048 measurements, then prints the value to the terminal via printf before
// starting another average cycle.
//
// The analog multiplexer selects P1.4 as the positive ADC0 input.  This 
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
//         o| P1.4     |  |
//         o|    |        |
//         o|     -------- 
//         o|
//          |
//----------   
// C8051F330-TB
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
// In addition, one must allow at least 1.5uS after changing analog mux 
// inputs or PGA settings.  The settling time in this example, then, is 
// dictated by the large external source resistance.
//
// The conversion is 10 periods of the SAR clock <SAR_CLK>.  At 3 MHz, 
// this time is 10 * 400nS = 3.3 uS.
//
// 
// Tsample, minimum  = Tsettle + Tconvert
//                   = 4.4uS + 3.3uS 
//                   = 7.7 uS
// 
// Timer 2 is set to start a conversion every 100uS, which is far longer
// than the minimum required.
//
// F330 Resources:
// ---------------
// Timer1: clocks UART
// Timer2: overflow initiates ADC conversion
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J8: Power - 9V or Debugger
//    - J3: UART TX - P0.4/TX
// 2) Connect the USB Debug Adapter to J4. If 9V power selected on J8,
//    connect the 9V power adapter to P1.
// 3) Compile and download code to the C8051F330-TB development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in the
//    quick menu, or pressing F11.
// 4) Connect serial cable from the J5 to a PC.
// 5) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the COM port at <BAUDRATE> (default 115200) and 8-N-1
// 6) Connect a variable voltage source (between 0 and Vref)
//    to P1.4, or a potentiometer voltage divider as shown above.
//		Vref is, by default, 2.44V.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) HyperTerminal will print the voltage measured by the device if
//    everything is working properly
//
// Target:         C8051F330
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated How To Test with Studio specific instructions
//    - Modified the ADC Interrupt to use compiler-agnostic declaration
//    - 6 JAN 2014
//
// Release 1.0
//    -Initial Revision (clm)
//    -13-Jun-06

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F330_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F33x
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // SYSCLK frequency in Hz
#define BAUDRATE     115200            // Baud rate of UART in bps

SI_SBIT(LED, SFR_P1, 3);                      // LED='1' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(ADC0_ISR, ADC0EOC_IRQn);

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
   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer 
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void) 
{
                                       // enable)

   SYSCLK_Init ();                     // Initialize system clock to 
                                       // 24.5MHz
   PORT_Init ();                       // Initialize crossbar and GPIO
   Timer2_Init();                      // Init Timer2 to generate 
                                       // overflows to trigger ADC
   UART0_Init();                       // Initialize UART0 for printf's
   ADC0_Init();                        // Initialize ADC0

   IE_EA = 1;							         // enable global interrupts
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
   OSCICN = 0x83;                      // configure internal oscillator for
                                       // 24.5MHz 
   RSTSRC = 0x04;                      // enable missing clock detector
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the Crossbar and GPIO ports.
// P0.4 - UART TX (push-pull)
// P0.5 - UART RX 
// P1.4 - ADC0 analog input
// P1.3 - LED (push-pull)
// 
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   XBR0     = 0x01;                    // Enable UART0
   XBR1     = 0x40;                    // Enable crossbar and weak pull-ups
   P0MDOUT |= 0x10;                    // Set TX pin to push-pull
   P1MDOUT |= 0x08;                    // enable LED as a push-pull output
   P1MDIN &= ~0x10;                    // set P1.4 as an analog input
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
   TMR2CN  = 0x00;                      // Stop Timer2; Clear TF2;
                                        // use SYSCLK as timebase, 16-bit 
                                        // auto-reload
   CKCON  |= 0x10;                      // select SYSCLK for timer 2 source
   TMR2RL  = 65535 - (SYSCLK / 10000);  // init reload value for 10uS
   TMR2    = 0xffff;                    // set to reload immediately
   TMR2CN_TR2     = 1;                         // start Timer2
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

   REF0CN = 0x03;                      // Enable on-chip VREF and buffer

   AMX0P = 0x0C;                       // ADC0 positive input = P1.4
   AMX0N = 0x11;                       // ADC0 negative input = GND
                                       // i.e., single ended mode

   ADC0CF = ((SYSCLK/3000000)-1)<<3;   // set SAR clock to 3MHz

   ADC0CF |= 0x00;                     // right-justify results 

   EIE1 |= 0x08;                       // enable ADC0 conversion complete int.

   ADC0CN_ADEN = 1;                          // enable ADC0
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
SI_INTERRUPT(ADC0_ISR, ADC0EOC_IRQn)
{

   static uint32_t accumulator = 0;     // accumulator for averaging
   static uint16_t measurements = 2048;  // measurement counter
   uint32_t result=0;
   uint32_t mV;                         // measured voltage in mV

   ADC0CN_ADINT = 0;                               // clear ADC0 conv. complete flag

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

      mV =  result * 2440 / 1023;   
      printf("P1.4 voltage: %ld mV\n",mV);
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
