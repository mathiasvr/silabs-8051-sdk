//-----------------------------------------------------------------------------
// F41x_ADC0_ExternalInput_Mux.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// This code example illustrates using the internal analog multiplexer to
// measure analog voltages on up to 8 different analog inputs.  Results are
// printed to a PC terminal program via the UART.
//
// The inputs are sequentially scanned, beginning with P1.0, up
// to input number <ANALOG_INPUTS>-1 based on the values in <PIN_TABLE>.
//
//
// ADC Settling Time Requirements, Sampling Rate:
// ----------------------------------------------
//
// The total sample time per input is comprised of an input setting time
// (Tsettle), followed by a conversion time (Tconvert):
//
// Tsample  = Tsettle + Tconvert
//
// Settling and conversion times may overlap, as the ADC holds the value once
// conversion begins.  This program takes advantage of this to increase the
// settling time above the minimum required.  In other words, when
// converting the value from analog input Ain(n), the input mux is switched
// over to the next input Ain(n+1) to begin settling.
//
// |---Settling Ain(n)---|=Conversion Ain(n)=|
//                       |---Settling Ain(n+1)---|=Conversion Ain(n+1)=|
//                                               |---Settling Ain(n+2)---|
// ISR:  Timer 2         ^                       ^                       ^
// ISR:  ADC0                                ^                         ^
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
//    Tsettle =   ln | ---- | * 105e3 * 10e-12 = 10.2uS
//                   | 0.25 |
//
// In addition, one must allow at least 1.5 us after changing analog MUX
// inputs or PGA settings.  The settling time in this example, then, is
// dictated by the large external source resistance.
//
// The conversion is 16 periods of the SAR clock.  At 2.5 MHz,
// this time is 16 * 400nS = 6.4 uS.
//
//
// Tsample, minimum  = Tsettle + Tconvert
//                   = 10.2uS + 6.4uS
//                   = 16.6 uS
//
// Timer2 is set to change the MUX input and start a conversion every 20 us.
//
// General:
// --------
//
// The system is clocked using the internal 24.5MHz oscillator. Results are
// printed to the UART from a loop with the rate set by a delay based on
// Timer0. This loop periodically reads the ADC value from a global array,
// <RESULT>.
//
// The ADC makes repeated measurements at 20 us intervals based on Timer2.
// The end of each ADC conversion initiates an interrupt which calls an
// averaging function. <INT_DEC> samples are averaged, then the Result
// values updated.
//
// For each power of 4 of <INT_DEC>, you gain 1 bit of effective resolution.
// For example, <INT_DEC> = 256 gain you 4 bits of resolution: 4^4 = 256.
//
// The ADC input multiplexer is set for a single-ended input.  The example
// sequentially scans through the inputs, starting at P1.0.  <ANALOG_INPUTS>
// inputs are read.  The amplifier is set for unity gain so a voltage range of
// 0 to Vref (2.2V) may be measured.  Although voltages up to Vdd may be
// applied without damaging the device, only the range 0 to Vref may be
// measured by the ADC.
//
// A 100 kohm potentiometer may be connected as a voltage divider between
// VREF and AGND as shown below:
//
// ---------
//          |
//         o| AGND ----|
//         o| VREF ----|<-|
//         o| P1.x     |  |
//         o|    |        |
//         o|     --------
//         o|
//         o|
//         o|
//          |
// ---------
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    J9:   VUNREG (9V adapter) or 5VEC3 (Debugger) 
//    J10:  +3VD/+3VD
//    J12:  +3VD/VREGIN
//          VREGIN_EN
//    J17:  5VEC3/VIO or +3VD/VIO
//          VIO_EN
//    J27:  P0.4/TX
// 2) Connect the USB Debug Adapter to J4. If VUNREG is selected on J9,
//    connect the 9V power adapter to P1.
// 3) Connect serial cable from the development board's J8 to a PC.
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the COM port at <BAUDRATE> and 8-N-1
// 5) Connect a variable voltage source (between 0 and Vref) to the Port1 pins,
//    or a potentiometer voltage divider as shown above.
// 6) Compile and download code to the C8051F410-TB development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in the
//    quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) HyperTerminal will print the voltages measured by the device if
//    everything is working properly.  Note that some of the analog inputs are
//    floating and will return nonzero values.
//
// Target:         C8051F41x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.2 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 specific code with compiler agnostic code from 
//      compiler_defs.h.
//    - 21 JAN 2014
//
// Release 1.1 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0
//    -Initial Revision (SM / TP)
//    -25 OCT 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F410_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK        24500000         // SYSCLK frequency in Hz
#define BAUDRATE      115200           // Baud rate of UART in bps
#define ANALOG_INPUTS 6                // Number of AIN pins to measure,
                                       // skipping the UART0 pins
#define INT_DEC       256              // Integrate and decimate ratio

#define TIMER0_RELOAD_HIGH  0          // Timer0 High register
#define TIMER0_RELOAD_LOW 255          // Timer0 Low register

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(ADC0_ISR, 10);
SI_INTERRUPT_PROTO(Timer2_ISR, 5);

void Oscillator_Init (void);
void Port_Init (void);
void Timer2_Init(void);
void ADC0_Init(void);
void UART0_Init (void);

void Timer0_wait(int16_t ms);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

int32_t RESULT[ANALOG_INPUTS];            // ADC0 decimated value, one for each
                                       // analog input

// The <PIN_MUX_TABLE> values are the values to be written to the AMX0P
// register to select the P1.<PIN_TABLE> port pins.
// For the 'F410, the AMX0P settings correspond to the following port pins:
//
//    AMX0P      Port Pin
//    0x08         P1.0
//    0x09         P1.1
//    0x0B         P1.3
//    0x0C         P1.4
//    0x0D         P1.5
//    0x0E         P1.6
//
// Skip the VREF pin (P1.2)
uint8_t idata PIN_TABLE[ANALOG_INPUTS] = {0,1,3,4,5,6};
uint8_t idata PIN_MUX_TABLE[ANALOG_INPUTS] = {8,9,11,12,13,14};

uint8_t AMUX_INPUT = 0;          // Index of analog MUX inputs

// Integrate accumulator for the ADC samples from input pins
uint32_t accumulator[ANALOG_INPUTS];

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
   uint8_t i;
   uint32_t measurement;

                                       // enable)

   Oscillator_Init ();                 // Initialize system clock to
                                       // 24.5MHz
   Port_Init ();                       // Initialize crossbar and GPIO
   Timer2_Init();                      // Init Timer2 to generate
                                       // overflows to trigger ADC
   UART0_Init();                       // Initialize UART0 for printf's
   ADC0_Init();                        // Initialize ADC0

   // Initialize array used to accumulate samples
   for (i = 0; i < ANALOG_INPUTS; i++) {
      accumulator[i] = 0; }

   IE_EA = 1;                             // Enable global interrupts
   while (1)
   {
      IE_EA = 0;                          // Disable interrupts
      printf("\f");
      for(i = 0; i < ANALOG_INPUTS; i++)
      {
         // The 12-bit ADC value is averaged across INT_DEC measurements.
         // The result is then stored in RESULT, and is right-justified
         // The measured voltage applied to the port pins is then:
         //
         //                           Vref (mV)
         //   measurement (mV) =   --------------- * Result (bits)
         //                        (2^12)-1 (bits)

         measurement =  RESULT[i] * 2200 / 4095;
         printf("P1.%bu voltage: %4ld mV\n",PIN_TABLE[i],measurement);
      }
      IE_EA = 1;                          // Re-enable interrupts

      Timer0_wait(25);                 // Wait before displaying new values
   }
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
// This routine initializes the system clock to use the internal 24.5MHz
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   OSCICN = 0x87;                      // Configure internal oscillator for
                                       // its highest frequency
   RSTSRC = 0x04;                      // Enable missing clock detector
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports.
//
// P0.4 - UART TX (push-pull)
// P0.5 - UART RX
//
// P1.0 - analog input (ADC0)
// P1.1 - analog input (ADC0)
// P1.2 - analog output (VREF)
// P1.3 - analog input (ADC0)
// P1.4 - analog input (ADC0)
// P1.5 - analog input (ADC0)
// P1.6 - analog input (ADC0)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1SKIP = 0x7F;                      // Skip all analog pins

   XBR0 = 0x01;                        // UART0 TX and RX pins enabled
   XBR1 = 0x40;                        // Enable crossbar and weak pull-ups

   P0MDOUT |= 0x10;                    // Enable TX0 as a push-pull output

   P1MDIN &= ~0x7F;                    // Set desired pins as analog inputs
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 10 us
// intervals.  Timer2 overflows automatically triggers ADC0 conversion.
//
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
                                       // use SYSCLK as timebase, 16-bit
                                       // auto-reload
   CKCON |= 0x10;                      // Select SYSCLK for timer 2 source
   TMR2RL = 65535 - (SYSCLK / 10000);  // Init reload value for 10 us
   TMR2 = 0xffff;                      // Set to reload immediately
   IE_ET2 = 1;                            // Enable Timer2 interrupts
   TMR2CN_TR2 = 1;                            // Start Timer2
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configures ADC0 to make single-ended analog measurements on Port 1 according
// to the values of <ANALOG_INPUTS> and <PIN_TABLE>.
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   ADC0CN = 0x03;                      // ADC0 disabled, burst-mode disabled,
                                       // conversion triggered on TMR2 overflow

   ADC0TK = 0xFB;                      // Pre-tracking mode enabled, keep
                                       // default timing settings

   REF0CN = 0x13;                      // VREF = 2.2 V, VREF avail on VREF pin,
                                       // VREF and Bias Generator enabled

   ADC0MX = PIN_MUX_TABLE[0];          // ADC0 initial positive input = P1.0

   ADC0CF = 0x00;                      // Repeat count = 1

   ADC0CF |= ((SYSCLK/3000000)-1)<<3;  // Set SAR clock to 3MHz

   EIE1 |= 0x08;                       // Enable ADC0 EOC interrupt

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

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xF0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                            // START Timer1
   SCON0_TI = 1;                            // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This routine changes to the next Analog MUX input whenever Timer2 overflows
// for the next ADC sample.  This allows the ADC to begin setting on the new
// input while converting the old input.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer2_ISR, 5)
{
   TMR2CN_TF2H = 0;                           // Clear Timer2 interrupt flag

   // Set up the AMUX for the next ADC input
   // ADC0 positive input = P1.<PIN_TABLE[AMUX_INPUT+1]>
   // ADC0 negative input = GND
   // i.e., single ended mode
   if (AMUX_INPUT == (ANALOG_INPUTS - 1))
   {
      ADC0MX = PIN_MUX_TABLE[0];
   }
   else
   {
      ADC0MX = PIN_MUX_TABLE[AMUX_INPUT+1];
   }
}

//-----------------------------------------------------------------------------
// ADC0_ISR
//-----------------------------------------------------------------------------
//
// This ISR averages <INT_DEC> samples for each analog MUX input then prints
// the results to the terminal.  The ISR is called after each ADC conversion,
// which is triggered by Timer2.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC0_ISR, 10)
{
   static unsigned int_dec = INT_DEC;  // Integrate/decimate counter
                                       // A new result is posted when
                                       // int_dec is 0

   uint8_t i;                    // Loop counter

   ADC0CN_ADINT = 0;                         // Clear ADC conversion complete
                                       // overflow

   accumulator[AMUX_INPUT] += ADC0;    // Read the ADC value and add it to the
                                       // running total

   // Reset sample counter <int_dec> and <AMUX_INPUT> if the final input was
   // just read
   if(AMUX_INPUT == (ANALOG_INPUTS - 1))
   {
      int_dec--;                       // Update decimation counter
                                       // when the last of the analog inputs
                                       // is sampled

      if (int_dec == 0)                // If zero, then post the averaged
      {                                // results
         int_dec = INT_DEC;            // Reset counter

         // Copy each averaged ADC0 value into the RESULT array
         for(i = 0; i < ANALOG_INPUTS; i++)
         {
            // Copy averaged values into RESULT
            RESULT[i] = accumulator[i] / int_dec;

            // Reset accumulators
            accumulator[i] = 0x00000000;
         }
      }

      AMUX_INPUT = 0;                  // Reset input index back to P2.0
   }
   // Otherwise, increment the AMUX channel counter
   else
   {
      AMUX_INPUT++;                    // Step to the next analog mux input
   }
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) int16_t ms - number of milliseconds to wait
//                        range is positive range of an int: 0 to 32767
//
// This function configures the Timer0 as a 16-bit timer, interrupt enabled.
// Using the internal osc. at 12MHz with a prescaler of 1:8 and reloading the
// T0 registers with TIMER0_RELOAD_HIGH/LOW, it will wait for <ms>
// milliseconds.
// Note: The Timer0 uses a 1:12 prescaler
//-----------------------------------------------------------------------------
void Timer0_wait(int16_t ms)
{
   TH0 = TIMER0_RELOAD_HIGH;           // Init Timer0 High register
   TL0 = TIMER0_RELOAD_LOW ;           // Init Timer0 Low register
   TMOD |= 0x01;                       // Timer0 in 16-bit mode
   CKCON &= 0xFC;                      // Timer0 uses a 1:12 prescaler
   TCON_TR0  = 1;                           // Timer0 ON

   while(ms)
   {
      TCON_TF0 = 0;                         // Clear flag to initialize
      while(!TCON_TF0);                     // Wait until timer overflows
      ms--;                            // Decrement ms
   }

   TCON_TR0 = 0;                            // Timer0 OFF
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
