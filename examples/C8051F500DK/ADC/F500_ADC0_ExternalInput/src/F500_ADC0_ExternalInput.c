//-----------------------------------------------------------------------------
// F500_ADC0_ExternalInput.c
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
// The system is clocked by the internal 24 MHz oscillator. Timer 2 triggers
// a conversion on ADC0 on each overflow. The completion of this conversion
// in turn triggers an interrupt service routine (ISR).  The ISR averages
// 2048 measurements, then prints the value to the terminal via printf before
// starting another average cycle.
//
// The analog multiplexer selects P1.2 as the ADC0 input. The C8051F50x
// ADC supports single-ended inputs and so the voltage on P1.2 is compared to
// GND.
//
// P1.2 on the C8051F500-TB is connected to a potentiometer. When the
// potentiometer is set to the maximum setting, the voltage input is VIO,
// which is 5V. In order to measure this signal using the on-chip 2.25V
// voltage reference, the on-chip Gain module is used to scale the signal.
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
//    Tsettle =   ln | ---- | * 103e3 * 32e-12 = 32 us
//                   | 0.25 |
//
// In addition, one must allow at least 1.5 us after changing analog mux
// inputs or PGA settings. The settling time in this example, then, is
// dictated by the large external source resistance.
//
// The conversion is 10 periods of the SAR clock <SAR_CLK>. At 3 MHz,
// this time is 10 * 400 ns = 3.3 us.
//
//
// Tsample, minimum  = Tsettle + Tconvert
//                   = 32 us + 3.3 us
//                   = 35.3 us
//
// Timer 2 is set to start a conversion every 100 us, which is far longer
// than the minimum required.
//
// F500 Resources:
// ---------------
// ADC0  : Uses on-chip VREF
// Timer2: overflow initiates ADC conversion
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
// 4) Ensure that a shorting block is put on J20 (potentiometer) and J22 (VREF)
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
// 8) HyperTerminal will print the voltage measured by the device if
//    everything is working properly.
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

#define SYSCLK       24000000          // SYSCLK frequency in Hz
#define BAUDRATE       115200          // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void TIMER2_Init (void);
void ADC0_Init (void);
void UART0_Init (void);

SI_INTERRUPT_PROTO(ADC_ISR, INTERRUPT_ADC0_EOC);

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
   SFRPAGE = LEGACY_PAGE;              // Set for PCA0MD


   SYSCLK_Init ();                     // Initialize system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   TIMER2_Init ();                     // Init Timer2 for ADC conversion clock
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
// Return Value:  None
// Parameters:    None
//
// This routine initializes the system clock to use the internal 24 MHz
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   // Configure internal oscillator for maximum frequency
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
// P1.2 - analog               ADC0 analog
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0SKIP |= 0x01;                     // Skip P0.0 (VREF)
   P1SKIP |= 0x04;                     // Skip P1.2 (ADC input)

   P0MDOUT |= 0x10;                    // Set TX pin to push-pull
   P0MDIN  &= ~0x01;                   // Set VREF to analog

   P1MDIN  &= ~0x04;                   // Set P1.2 as an analog input

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
// Return Value:  None
// Parameters:    None
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 100 us
// intervals.  Timer 2 overflow automatically triggers ADC0 conversion.
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
   TMR2RL = 65535 - (SYSCLK / 10000);  // Init reload value for 100 us
   TMR2   = 0xFFFF;                    // Set to reload immediately
   TMR2CN_TR2 = 1;                     // Start Timer2

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures ADC0 to make single-ended analog measurements on pin P1.2
// Also enabled the gain and the voltage reference
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
   ADC0L   = 0x6C;                     // Load the upper byte of 0x6CA to 
                                       // ADC0GNH
   ADC0H   = 0x07;                     // Load the ADC0GNL address
   ADC0L   = 0xA0;                     // Load the lower nibble of 0x6CA to 
                                       // ADC0GNL
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

   ADC0MX = 0x0A;                      // Set ADC input to P1.2

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

   static uint32_t accumulator = 0;         // Accumulator for averaging
   static uint16_t measurements = 2048;     // Measurement counter
   uint32_t result = 0;
   uint32_t mV;                             // Measured voltage in mV

   ADC0CN_ADINT = 0;                   // Clear ADC0 conv. complete flag

   accumulator += ADC0;                // Add most recent sample
   measurements--;                     // Subtract counter

   if (measurements == 0)
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
      //
      // Then multiply the result by 2.27 to account for the 0.44 gain
      // applied earlier

      mV =  result * 2400 / 4095 * 227 / 100;

      // SFR page is already correct for printf (SBUF0 page)
      printf("P1.2 voltage: %ld mV\n", mV);
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------