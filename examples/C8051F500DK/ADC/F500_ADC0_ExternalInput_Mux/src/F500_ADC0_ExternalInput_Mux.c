//-----------------------------------------------------------------------------
// F500_ADC0_ExternalInput_Mux.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This code example illustrates using the internal analog multiplexer to
// measure analog voltages on up to 8 different analog inputs. Results are
// printed to a PC terminal program via the UART.
//
// The inputs are sequentially scanned, beginning with input 0 (P2.0), up
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
// conversion begins. This program takes advantage of this to increase the
// settling time above the minimum required. In other words, when
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
//    Tsettle =   ln | ---- | * 103e3 * 32e-12 = 32 us
//                   | 0.25 |
//
// In addition, one must allow at least 1.5 us after changing analog MUX
// inputs or PGA settings. The settling time in this example, then, is
// dictated by the large external source resistance.
//
// The conversion is 16 periods of the SAR clock.  At 2.5 MHz,
// this time is 16 * 400 ns = 6.4 us.
//
//
// Tsample, minimum  = Tsettle + Tconvert
//                   = 32 us + 6.4 us
//                   = 38.4 us
//
// Timer2 is set to change the MUX input and start a conversion every 40 us.
//
// General:
// --------
//
// The system is clocked using the internal 24 MHz oscillator. Results are
// printed to the UART from a loop with the rate set by a delay based on
// Timer0. This loop periodically reads the ADC value from a global array,
// <RESULT>.
//
// The ADC makes repeated measurements at 40 us intervals based on Timer2.
// The end of each ADC conversion initiates an interrupt which calls an
// averaging function. <INT_DEC> samples are averaged, then the Result
// values updated.
//
// For each power of 4 of <INT_DEC>, you gain 1 bit of effective resolution.
// For example, <INT_DEC> = 256 gain you 4 bits of resolution: 4^4 = 256.
//
// The ADC input multiplexer is set for a single-ended input.  The example
// sequentially scans through the inputs, starting at P2.0.  <ANALOG_INPUTS>
// inputs are read.  The amplifier is set for 0.44 gain so a voltage range of
// 0 to 5V (2.2V) may be measured.  Although voltages up to 5V may be
// applied without damaging the device, only the range 0 to VREF may be
// measured by the ADC.
//
// A 100 kohm potentiometer may be connected as a voltage divider between
// VREF and AGND as shown below:
//
// ---------
//          |
//         o| AGND ----|
//         o| VREF ----|<-|
//         o| P2.x     |  |
//         o|    |        |
//         o|     --------
//         o|
//         o|
//         o|
//          |
// ---------
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F500-TB are placed on the following:
//    J17: P0.4 - UART_TX
//    J17: P0.5 - UART_RX
//    J7: Power selection, P4 9V (+5VREG) or P2 Debugger (SER_PWR)
// 2) Connect the USB Debug Adapter to P2. If +5VREG is selected on J7,
//    connect the 9V power adapter to P2.
// 3) Connect a USB cable from the USB port P5 on development board to a PC.
//    Specify the target baudrate in the constant <BAUDRATE>.
// 4) Connect a variable voltage source (between 0 and Vref)
//    to Port2 pins, or a potentiometer voltage divider as shown above.
// 5) On the PC, open Hyperterminal, or a similar program, and connect to the
//    target board's serial port using <BAUDRATE> and 8-N-1 (CP210x USB-to-UART
//    VCP drivers must be installed).
// 6) Compile and download code to C8051F500 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 8) HyperTerminal will print the voltages measured by the device if
//    everything is working properly. Note that some of the analog inputs are
//    floating and will return nonzero values.
//
//
// Target:         C8051F50x/1x (Side A of a C8051F500-TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.4 / 11 AUG 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.3 / 02 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.2 / 04 NOV 2008 (GP)
//    -Fixed VREF initialization to use 2.25V reference
//
// Release 1.1 / 10 JUN 2008 (ADT)
//    -Edited formatting
//
// Release 1.0 / 03 MAR 2008 (GP)
//    -Initial Revision


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F500_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK         24000000        // SYSCLK frequency in Hz
#define BAUDRATE         115200        // Baud rate of UART in bps
#define ANALOG_INPUTS         8        // Number of AIN pins to measure,
                                       // skipping the UART0 pins

#define INT_DEC             256        // Integrate and decimate ratio

#define TIMER0_RELOAD_HIGH    7        // Timer0 High register
#define TIMER0_RELOAD_LOW   208        // Timer0 Low register

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void TIMER2_Init (void);
void ADC0_Init (void);
void UART0_Init (void);

void Timer0_wait (int ms);

SI_INTERRUPT_PROTO(ADC_ISR, INTERRUPT_ADC0_EOC);
SI_INTERRUPT_PROTO(TIMER2_ISR, INTERRUPT_TIMER2);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// ADC0 decimated value, one for each analog input
SI_SEGMENT_VARIABLE(RESULT[ANALOG_INPUTS], uint32_t, xdata);       

// ADC0 input matrix.  Bit-wise OR with 0x10 to get the ADC0MX setting for P2
uint8_t PIN_TABLE[ANALOG_INPUTS] = { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07 };

uint8_t AMUX_INPUT = 0;          // Index of analog MUX inputs


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
   uint8_t  i;                              // Loop counter
   uint32_t measurement;                    // Voltage measurement

   SFRPAGE = LEGACY_PAGE;              // Set for PCA0MD and for printf()


   OSCILLATOR_Init ();                 // Initialize system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   TIMER2_Init ();                     // Init Timer2 to trigger ADC
   UART0_Init ();                      // Initialize UART0 for printf's
   ADC0_Init ();                       // Initialize ADC0

   IE_EA = 1;                          // Enable global interrupts

   while (1)
   {
      IE_EA = 0;                       // Disable interrupts
      printf ("\f");
      for (i = 0; i < ANALOG_INPUTS; i++)
      { 
         // The 12-bit ADC value is averaged across INT_DEC measurements.
         // The result is then stored in RESULT, and is right-justified
         // The measured voltage applied to the port pins is then:
         //
         //                           Vref (mV)
         //   measurement (mV) =   --------------- * Result (bits)
         //                        (2^12)-1 (bits)
         //
         // Then multiply the result by 2.27 to account for the 0.44 gain
         // applied earlier

         measurement =  RESULT[i] * 2400 / 4095 * 227 / 100;
         printf("P2.%bu voltage: %4ld mV\n", PIN_TABLE[i], measurement);
      }
      IE_EA = 1;                       // Re-enable interrupts

      Timer0_wait(25);                 // Wait before displaying new values
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24 MHz
// oscillator as its clock source. Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   // Configure internal oscillator for its maximum frequency
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;

   SFRPAGE = LEGACY_PAGE;
   // enable missing clock detector
   RSTSRC = 0x04;

   SFRPAGE = SFRPAGE_save;
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
// P0.0 - analog               VREF
// P0.4 - digital  push-pull   UART TX
// P0.5 - digital  open-drain  UART RX
//
// P2.0 - P2.7     analog      ADC inputs
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0SKIP |= 0x01;                     // Skip P0.0 (VREF)
   P0MDOUT|= 0x10;                     // Set TX pin to push-pull
   P0MDIN &= ~0x01;                    // Set VREF to analog

   P2SKIP  = 0xFF;                     // Skip all of P2 for analog inputs
   P2MDIN  = 0x00;                     // Configure all of P2 as analog inputs

   XBR0 = XBR0_URT0E__ENABLED;         // Enable UART0
   // Enable crossbar and weak pull-ups
   XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED |
          XBR2_XBARE__ENABLED;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// TIMER2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 40 us
// intervals. Timer2 overflows automatically triggers ADC0 conversion.
//
//-----------------------------------------------------------------------------
void TIMER2_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
                                       // use SYSCLK as timebase, 16-bit
                                       // auto-reload
   CKCON |= CKCON_T2ML__SYSCLK;        // Select SYSCLK for Timer 2 source
   TMR2RL = 65535 - (SYSCLK / 40);     // Init reload value for 20 us
   TMR2   = 0xFFFF;                    // Set to reload immediately
   IE_ET2 = 1;                         // Enable Timer2 interrupts
   TMR2CN_TR2 = 1;                     // Start Timer2

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configures ADC0 to make single-ended analog measurements on Port 0 according
// to the values of <ANALOG_INPUTS> and <PIN_TABLE>.
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   // Initialize the Gain to account for a 5V input and 2.25 VREF
   // Solve the equation provided in Section 6.3.1 of the Datasheet

   // The 5V input is scaled by a factor of 0.44 so that the maximum input
   // voltage seen by the pin is 2.2V

   // 0.44 = (GAIN/4096) + GAINADD * (1/64)

   // Set GAIN to 0x6CA and GAINADD to 1
   // GAIN = is the 12-bit word formed by ADC0GNH[7:0] ADC0GNL[7:4]
   // GAINADD is bit ADC0GNA.0

   ADC0CF |= ADC0CF_GAINEN__ENABLED;   // Set GAINEN = 1
   ADC0H   = 0x04;                     // Load the ADC0GNH address
   ADC0L   = 0x6C;                     // Load the upper byte of 0x6CA
   ADC0H   = 0x07;                     // Load the ADC0GNL address
   ADC0L   = 0xA0;                     // Load the lower nibble of 0x6CA
   ADC0H   = 0x08;                     // Load the ADC0GNA address
   ADC0L   = 0x01;                     // Set the GAINADD bit
   ADC0CF &= ~ADC0CF_GAINEN__BMASK;    // Set GAINEN = 0

   // ADC0 disabled, normal tracking, conversion triggered on TMR2 overflow
   // Output is right-justified
   ADC0CN = ADC0CN_ADLJST__RIGHT |
            ADC0CN_ADCM__TIMER2;

   // Enable on-chip VREF and buffer, set voltage reference to 2.2V
   REF0CN = REF0CN_ZTCEN__FORCE_ENABLED |
            REF0CN_REFLV__2P2 |
            REF0CN_BIASE__ENABLED |
            REF0CN_REFBE__ENABLED;

   ADC0MX = 0x10 | PIN_TABLE[AMUX_INPUT];  // Set ADC input to initial setting

   ADC0CF = ((SYSCLK / 3000000) - 1) << 3; // Set SAR clock to 3MHz

   EIE1 |= EIE1_EADC0__ENABLED;        // Enable ADC0 conversion complete int.

   ADC0CN_ADEN = 1;                    // Enable ADC0

   SFRPAGE = SFRPAGE_save;
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
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       // clear RI0 and TI0 bits

   // Baud Rate = [BRG Clock / (65536 - (SBRLH0:SBRLL0))] x 1/2 x 1/Prescaler

#if   ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 1)
      SBRL0 = -(SYSCLK / BAUDRATE / 2);
      SBCON0 |= 0x03;                  // Set prescaler to 1
#elif ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 4)
      SBRL0 = -(SYSCLK / BAUDRATE / 2 / 4);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x01;                  // Set prescaler to 4
#elif ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 12)
      SBRL0 = -(SYSCLK / BAUDRATE / 2 / 12);
      SBCON0 &= ~0x03;                 // Set prescaler to 12
#else
      SBRL0 = -(SYSCLK / BAUDRATE / 2 / 48);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x02;                  // Set prescaler to 48
#endif

   SBCON0 |= SBCON0_SBRUN__ENABLED;    // Enable baud rate generator
   SCON0_TI = 1;                       // Indicate TX0 ready

   SFRPAGE = SFRPAGE_save;
}

#ifdef SDCC

// SDCC does not include a definition for putchar(), which is used in printf()
// and so it is defined here.  The prototype does not need to be explicitly
// defined because it is provided in stdio.h

//-----------------------------------------------------------------------------
// putchar
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : character to send to UART
//
// This function outputs a character to the UART.
//-----------------------------------------------------------------------------
void putchar (int8_t output)
{
   if (output == '\n')
   {
      while (!TI0);
      TI0 = 0;
      SBUF0 = 0x0D;
   }
   while (!TI0);
   TI0 = 0;
   SBUF0 = output;
}

#endif

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
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                    // Clear Timer2 interrupt flag

   // Set up the AMUX for the next ADC input
   if (AMUX_INPUT == (ANALOG_INPUTS - 1))
   {
      ADC0MX = PIN_TABLE[0] | 0x10;
   }
   else
   {
      ADC0MX = PIN_TABLE[AMUX_INPUT+1] | 0x10;
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
SI_INTERRUPT(ADC0_ISR, ADC0EOC_IRQn)
{
   static uint16_t int_dec = INT_DEC;       // Integrate/decimate counter
                                       // A new result is posted when
                                       // int_dec is 0

   // Integrate accumulator for the ADC samples from input pins
   static int32_t accumulator[ANALOG_INPUTS] = {0x00000000};

   uint8_t i;                               // Loop counter


   ADC0CN_ADINT = 0;                   // Clear ADC conversion complete
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
// Timer0_wait
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) int ms - number of milliseconds to wait
//               range is positive range of an int: 0 to 32767
//
// This function configures the Timer0 as a 16-bit timer, interrupt enabled.
// Using the internal osc. at 24.5MHz with a prescaler of 1:8 and reloading the
// T0 registers with TIMER0_RELOAD_HIGH/LOW, it will wait for <ms>
// milliseconds.
// Note: The Timer0 uses a 1:12 prescaler
//-----------------------------------------------------------------------------
void Timer0_wait(int16_t ms)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   TH0 = TIMER0_RELOAD_HIGH;           // Init Timer0 High register
   TL0 = TIMER0_RELOAD_LOW ;           // Init Timer0 Low register
   TMOD |= TMOD_T0M__MODE1;            // Timer0 in 16-bit mode
   CKCON &= 0xFC;                      // Timer0 uses a 1:12 prescaler
   TCON_TR0  = 1;                      // Timer0 ON

   while(ms)
   {
      TCON_TF0 = 0;                    // Clear flag to initialize
      while(!TCON_TF0);                // Wait until timer overflows
      ms--;                            // Decrement ms
   }

   TCON_TR0 = 0;                       // Timer0 OFF

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------