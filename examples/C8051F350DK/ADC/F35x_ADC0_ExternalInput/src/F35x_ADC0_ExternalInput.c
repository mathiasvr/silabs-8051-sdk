//-----------------------------------------------------------------------------
// F35x_ADC0_ExternalInput.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This example code for the C8051F350 takes measurements from input A1N0.2
// using ADC0 then prints the results to a terminal window via the UART.
//
// The system is clocked by the internal 24.5MHz oscillator. The completion of
// this conversion in turn triggers an interrupt service routine (ISR). The ISR
// calculates the ADC0 result into the equivalent mV and then prints the value
// to the terminal via printf before starting another conversion.
//
// The analog multiplexer selects A1N2 as the positive ADC0 input.  This
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
//         o| AIN2     |  |
//         o|    |        |
//         o|     --------
//         o|
//          |
//----------
// C8051F350-TB
//
// Terminal output is done via printf, which directs the characters to
// UART0.
//
// F350 Delta-Sigma ADC
// --------------------
// Please see Silicon Labs Applicaton Note AN217 for more information
// on the C8051F35x Delta-Sigma ADC.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt/products/mcu/Pages/ApplicationNotes.aspx
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are installed on the following:
//    - J3 : P0.4 - TX
//    - J3 : P0.5 - RX
//    - J8 : Power selection, P1 9V or J4 Debugger
// 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1. After power the board, the red LED D3
//    is on.
// 3) Connect serial cable from the RS232 connector J5 to a PC
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the COM port at <BAUDRATE> (115200) and 8-N-1
// 5) Connect a variable voltage source (between 0 and Vref)
//    to AIN2, or a potentiometer voltage divider as shown above.
// 6) Compile and download code to the C8051F350-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 7) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 8) The terminal program will print the voltage measured by the device.
//
//
// Target:         C8051F350/1
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1
//    - Updated to use with Simplicity Studio (JL)
//    - 24 JUN 2014
//
// Release 1.0
//    -Initial Revision (SM / TP)
//    - 8 NOV 2006

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F350_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F35x
//-----------------------------------------------------------------------------

SI_SFR16(ADC0DEC, 0x9A);                 // ADC0 Decimation Ratio Register

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // SYSCLK frequency in Hz

#define MDCLK         2457600          // Modulator clock in Hz (ideal is
                                       // (2.4576 MHz)
#define OWR                20          // Desired Output Word Rate in Hz

#define BAUDRATE       115200          // Baud rate of UART in bps

typedef union LONGDATA{                // Access LONGDATA as an
   uint32_t result;                         // unsigned long variable or
   uint8_t Byte[4];                         // 4 unsigned byte variables
}LONGDATA;

// With the Keil compiler and union byte addressing:
// [0] = bits 31-24, [1] =  bits 23-16, [2] = bits 15-8, [3] = bits 7-0
#define Byte3 0
#define Byte2 1
#define Byte1 2
#define Byte0 3

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Port_Init (void);
void UART0_Init (void);
void ADC0_Init(void);

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // WDTE = 0 (clear watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
                                       // enable)

   Oscillator_Init();                  // Initialize system clock
   Port_Init();                        // Initialize Crossbar and GPIO
   UART0_Init();                       // Initialize UART0 for printf's
   ADC0_Init();                        // Initialize ADC0

   ADC0STA_ADINT = 0;
   // Start continuous conversions
   ADC0MD = ADC0MD_ADEN__ENABLED |
            ADC0MD_ADSM__CONT_CONVERSION;
   IE_EA = 1;                          // Enable global interrupts

   while (1);                          // Spin forever
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Oscillator_Init
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
   // enable internal high frequency
   OSCICN = OSCICN_IOSCEN__ENABLED | OSCICN_IFCN__HFOSC_DIV_1;

   // Enable missing clock detector
   RSTSRC = 0x04;
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the GPIO and the Crossbar
//
// Pinout:
//
//   P0.4 - UART TX (digital, push-pull)
//   P0.5 - UART RX (digital, open-drain)
//
//   AIN0.2 - ADC0 input
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   XBR0 = XBR0_URT0E__ENABLED;         // UART0 Selected

   // Enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED;

   P0MDOUT |= 0x10;                    // TX, Push-pull
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
   SCON0 = SCON0_SMODE__8_BIT         |   // 8-bit variable bit rate
           SCON0_MCE__MULTI_DISABLED  |   // level of STOP bit is ignored
           SCON0_REN__RECEIVE_ENABLED |   // RX enabled
           SCON0_TB8__CLEARED_TO_0    |   // ninth bits are zeros
           SCON0_RB8__CLEARED_TO_0    |
           SCON0_TI__NOT_SET          |   // clear RI0 and TI0 bits
           SCON0_RI__NOT_SET;

   CKCON &= ~(CKCON_T1M__BMASK |
              CKCON_SCA__FMASK);
   #if (SYSCLK/BAUDRATE/2/256 < 1)
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON |=  CKCON_T1M__SYSCLK;        // T1M = 1; SCA1:0 = xx
   #elif (SYSCLK/BAUDRATE/2/256 < 4)
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON |=  CKCON_SCA__SYSCLK_DIV_4;  // T1M = 0; SCA1:0 = 01
   #elif (SYSCLK/BAUDRATE/2/256 < 12)
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON |= CKCON_SCA__SYSCLK_DIV_12;  // T1M = 0; SCA1:0 = 00
   #else
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON |=  CKCON_SCA__SYSCLK_DIV_48; // T1M = 0; SCA1:0 = 10
   #endif

   TL1 = TH1;                             // Init Timer1
   TMOD &= ~(TMOD_GATE1__BMASK |
             TMOD_CT1__BMASK   |
             TMOD_T1M__FMASK);
   // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  TMOD_T1M__MODE2;
   TCON_TR1 = 1;                          // START Timer1
   SCON0_TI = 1;                          // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Initialize the ADC to use the temperature sensor. (non-differential)
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   // Enable internal Vref
   REF0CN |= REF0CN_BIASE__ENABLED | REF0CN_REFBE__ENABLED;
   // Gain = 1, Unipolar mode
   ADC0CN = ADC0CN_ADPOL__UNIPOLAR | ADC0CN_ADGN__GAIN_1;
   // Interrupts upon SINC3 filter output
   ADC0CF = ADC0CF_ADISEL__SINC3   | ADC0CF_ADVREF__INTERNAL;
                                       // and uses internal VREF

   ADC0CLK = (SYSCLK/MDCLK)-1;         // Generate MDCLK for modulator.
                                       // Ideally MDCLK = 2.4576MHz

   // Program decimation rate for desired OWR
   ADC0DEC = ((unsigned long) MDCLK / (unsigned long) OWR /
              (unsigned long) 128) - 1;

   ADC0BUF = 0x00;                     // Turn off Input Buffers
   ADC0MUX = 0x28;                     // Select AIN0.2

   // Start internal calibration
   ADC0MD = ADC0MD_ADEN__ENABLED | ADC0MD_ADSM__FULL_INTERNAL_CAL;
   while(ADC0STA_ADCALC != 1);         // Wait until calibration is complete

   EIE1   |= EIE1_EADC0__ENABLED;      // Enable ADC0 Interrupts
   // Enable the ADC0 (IDLE Mode)
   ADC0MD  = ADC0MD_ADEN__ENABLED | ADC0MD_ADSM__IDLE;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ADC0_ISR
//-----------------------------------------------------------------------------
//
// This ISR prints the result to the UART. The ISR is called after each ADC
// conversion.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC0_ISR, ADC0_IRQn)
{
   static LONGDATA rawValue;
   unsigned long mV;

   while(!ADC0STA_ADINT);              // Wait till conversion complete
   ADC0STA_ADINT = 0;                  // Clear ADC0 conversion complete flag

   // Copy the output value of the ADC
   rawValue.Byte[Byte3] = 0x00;
   rawValue.Byte[Byte2] = (unsigned char)ADC0H;
   rawValue.Byte[Byte1] = (unsigned char)ADC0M;
   rawValue.Byte[Byte0] = (unsigned char)ADC0L;

   //                           Vref (mV)
   //   measurement (mV) =   --------------- * result (bits)
   //                       (2^24)-1 (bits)
   //
   //   measurement (mV) =  result (bits) / ((2^24)-1 (bits) / Vref (mV))
   //
   //
   //   With a Vref (mV) of 2500:
   //
   //   measurement (mV) =  result (bits) / ((2^24)-1 / 2500)
   //
   //   measurement (mV) =  result (bits) / ((2^24)-1 / 2500)
   //
   //   measurement (mV) =  result (bits) / (16777215 / 2500)
   //
   //   measurement (mV) =  result (bits) / (6710)

   mV = rawValue.result / 6710;        // Because of bounds issues, this
                                       // calculation has been manipulated as
                                       // shown above
                                       // (i.e. 2500 (VREF) * 2^24 (ADC result)
                                       // is greater than 2^32)

   printf("AIN0.2 voltage: %4ld mV\n",mV);
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
