//-----------------------------------------------------------------------------
// F36x_ADC0_ExternalInput_Mux.c
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
// conversion is made. This settling depends on the external source
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
//    Tsettle =   ln | ---- | * 105e3 * 10e-12 = 8.8 us
//                   | 0.25 |
//
// In addition, one must allow at least 1.5 us after changing analog MUX
// inputs or PGA settings. The settling time in this example, then, is
// dictated by the large external source resistance.
//
// The conversion is 16 periods of the SAR clock.  At 2.5 MHz,
// this time is 16 * 400nS = 6.4 us.
//
//
// Tsample, minimum  = Tsettle + Tconvert
//                   = 8.8 us + 6.4 us
//                   = 15.2 us
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
// sequentially scans through the inputs, starting at P2.0.  <ANALOG_INPUTS>
// inputs are read.  The amplifier is set for unity gain so a voltage range of
// 0 to Vref (2.43V) may be measured.  Although voltages up to Vdd may be
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
//         o| P2.x     |  |
//         o|    |        |
//         o|     --------
//         o|
//         o|
//         o|
//          |
// ---------
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: P0.1 - TX
//    J12: P0.2 - RX
//    J1: Power selection, P2 9V, P4 USB or J9 Debugger
// 2) Connect the USB Debug Adapter to J9. If 9V power is selected on J1,
//    connect the 9V power adapter to P2. If VBUS is selected on J1, connect
//    USB cable to P4.
// 3) Ensure that a USB cable is connected to the USB connector
//    on the target board.
// 4) Connect a variable voltage source (between 0 and Vref)
//    to Port2 pins, or a potentiometer voltage divider as shown above.
// 5) Specify the target baudrate in the constant <BAUDRATE>.
// 6) Open Hyperterminal, or a similar program, and connect to the target
//    board's serial port using <BAUDRATE> and 8-N-1 (CP210x USB-to-UART
//    VCP drivers must be installed).
// 7) Compile and download code to C8051F360 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 9) HyperTerminal will print the voltages measured by the device if
//    everything is working properly. Note that some of the analog inputs are
//    floating and will return nonzero values.
//
//
// Target:         C8051F36x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.2
//    - Updated to use with Simplicity Studio (JL)
//    - 29 JUL 2014
//
// Release 1.1
//    -01 MAR 2010 (BD)
//    -Added curly braces around "accumulator" array init in ADC ISR
//
// Release 1.0
//    -Initial Revision (GP)
//    -29 AUG 2007


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK        24500000         // SYSCLK frequency in Hz
#define BAUDRATE      115200           // Baud rate of UART in bps
#define ANALOG_INPUTS 6                // Number of AIN pins to measure,

#define INT_DEC       256              // Integrate and decimate ratio

#define TIMER0_RELOAD_HIGH  0          // Timer0 High register
#define TIMER0_RELOAD_LOW 255          // Timer0 Low register

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Port_Init (void);
void Timer2_Init(void);
void ADC0_Init(void);
void UART0_Init (void);

void Timer0_wait(int ms);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint32_t RESULT[ANALOG_INPUTS];             // ADC0 decimated value, one for each
                                       // analog input

// The <PIN_MUX_TABLE> values are the values to be written to the AMX0P
// register to select the P2.<PIN_TABLE> port pins.
// For the 'F360, the AMX0P settings correspond to the following port pins:
//
//    AMX0P      Port Pin
//    0x08         P2.0
//    0x09         P2.1
//    0x0A         P2.2
//    0x0B         P2.3
//    0x0C         P2.4
//    0x0D         P2.5
//
uint8_t idata PIN_TABLE[ANALOG_INPUTS] = {0,1,2,3,4,5};
uint8_t idata PIN_MUX_TABLE[ANALOG_INPUTS] = {8,9,10,11,12,13};

uint8_t AMUX_INPUT = 0;                     // Index of analog MUX inputs

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   uint8_t i;
   uint32_t measurement;


   Oscillator_Init ();                 // Initialize system clock 
   Port_Init ();                       // Initialize crossbar and GPIO
   Timer2_Init();                      // Init Timer2 to generate
                                       // overflows to trigger ADC
   UART0_Init();                       // Initialize UART0 for printf's
   ADC0_Init();                        // Initialize ADC0

   IE_EA = 1;                          // Enable global interrupts
   while (1)
   {
      IE_EA = 0;                       // Disable interrupts
      printf("\f");
      for(i = 0; i < ANALOG_INPUTS; i++)
      {
         // The 10-bit ADC value is averaged across INT_DEC measurements.
         // The result is then stored in RESULT, and is right-justified
         // The measured voltage applied to the port pins is then:
         //
         //                           Vref (mV)
         //   measurement (mV) =   --------------- * Result (bits)
         //                        (2^10)-1 (bits)

         measurement =  RESULT[i] * 2430 / 1023;
         printf("P2.%bu voltage: %4ld mV\n",PIN_TABLE[i],measurement);
      }
      IE_EA = 1;                        // Re-enable interrupts

      Timer0_wait(25);                  // Wait before displaying new values
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
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;  // Configure internal oscillator for
                                       // 24.5MHz
   RSTSRC = 0x04;                      // Enable missing clock detector

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
// P0.1 - digital   push-pull    UART TX
// P0.2 - digital   open-drain   UART RX
// P0.3 - analog                 VREF
//
// P2.0 - analog input (ADC0)
// P2.1 - analog input (ADC0)
// P2.2 - analog input (ADC0)
// P2.3 - analog input (ADC0)
// P2.4 - analog input (ADC0)
// P2.5 - analog input (ADC0)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   P2SKIP = 0x3F;                      // Skip all analog pins

   XBR0 = XBR0_URT0E__ENABLED;         // Enable UART0
   XBR1 = XBR1_XBARE__ENABLED;         // Enable crossbar and weak pull-ups

   P0MDOUT |= 0x10;                    // Enable TX0 as a push-pull output

   P0MDIN &= ~0x08;                    // P0.3 in analog mode

   P2MDIN &= ~0x3F;                    // Set desired pins as analog inputs

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 10 us
// intervals. Timer2 overflows automatically triggers ADC0 conversion.
//
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
                                       // use SYSCLK as timebase, 16-bit
                                       // auto-reload
   CKCON |= 0x10;                      // Select SYSCLK for timer 2 source
   TMR2RL = 65535 - (SYSCLK / 10000);  // Init reload value for 10 us
   TMR2 = 0xffff;                      // Set to reload immediately
   IE_ET2 = 1;                         // Enable Timer2 interrupts
   TMR2CN_TR2 = 1;                     // Start Timer2

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configures ADC0 to make single-ended analog measurements on Port 2 according
// to the values of <ANALOG_INPUTS> and <PIN_TABLE>.
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE  
  
   ADC0CN = ADC0CN_ADCM__TIMER2;       // ADC0 disabled, normal tracking,
                                       // conversion triggered on TMR2 overflow

   REF0CN = REF0CN_BIASE__ENABLED |
            REF0CN_REFBE__ENABLED;     // Enable internal VREF

   AMX0P = PIN_MUX_TABLE[0];           // ADC0 initial positive input = P2.0
   AMX0N = 0x1F;                       // ADC0 negative input = GND
                                       // i.e., single ended mode

   ADC0CF = ((SYSCLK/3000000)-1)<<3;   // Set SAR clock to 3MHz

   ADC0CF |= 0x00;                     // Right-justify results

   EIE1 |= EIE1_EADC0__ENABLED;        // Enable ADC0 EOC interrupt

   ADC0CN_ADEN = 1;                    // Enable ADC0

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits
   CKCON &= ~(CKCON_T1M__BMASK |
              CKCON_SCA__FMASK);
   #if (SYSCLK/BAUDRATE/2/256 < 1)
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON |= CKCON_T1M__SYSCLK;
   #elif (SYSCLK/BAUDRATE/2/256 < 4)
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON |= CKCON_SCA__SYSCLK_DIV_4;
   #elif (SYSCLK/BAUDRATE/2/256 < 12)
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON |= CKCON_SCA__SYSCLK_DIV_12;
   #else
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON |= CKCON_SCA__SYSCLK_DIV_48;
   #endif

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xF0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  TMOD_T1M__MODE2;
   TCON_TR1 = 1;                       // START Timer1
   SCON0_TI = 1;                       // Indicate TX0 ready

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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

SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                    // Clear Timer2 interrupt flag

   // Set up the AMUX for the next ADC input
   // ADC0 positive input = P2.<PIN_TABLE[AMUX_INPUT+1]>
   // ADC0 negative input = GND
   // i.e., single ended mode
   if (AMUX_INPUT == (ANALOG_INPUTS - 1))
   {
      AMX0P = PIN_MUX_TABLE[0];
   }
   else
   {
      AMX0P = PIN_MUX_TABLE[AMUX_INPUT+1];
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
// Timer0_Init
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

void Timer0_wait(int ms)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

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

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------