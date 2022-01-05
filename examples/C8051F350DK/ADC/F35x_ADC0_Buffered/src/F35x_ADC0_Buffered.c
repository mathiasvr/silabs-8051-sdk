//-----------------------------------------------------------------------------
// F35x_ADC0_Buffered.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// This program demonstrates taking measurements using the 24-bit ADC on the
// C8051F350/51 devices, and then prints the results to a terminal window via
// the UART.
//
// Input pin configuration shown in ADC0_Init().
//
// For a Noise measurement, connect AIN0 and AIN1 to AGND at the terminal
// block.  Set "USE_FLOAT" to '1', "PRINT_STATISTICS" to '1',
// "PRINT_SAMPLES" to '0', and "PRINT_VOLTAGES" to '0'.
//
// This software configures the ADC to use an external VREF.  Therefore,
// on the 'F350 target board, J13 and J14 should have their shorting blocks
// installed.
//
// The standard deviation (Sigma) of a sample set is equivalent to the
// effective RMS noise of the conversion system.  "Sigma", when converted
// to Volts, is equivalent to the input-referred noise floor of the
// sampling system.
//
// Typical values of Sigma from the C8051F350 rev B target board with
// AIN0 and AIN1 grounded at the terminal block are around 9 to 11 LSBs
// in bipolar mode.  For a DC measurement, this is equivalent to a Signal-
// to-Noise ratio of about 117dB, or about 20 bits of effective dynamic
// range.
//
// 117dB = 20 log10 ( 11 / 2^23)
// 20 bits = 117dB / 6dB/bit
//
// Another parameter of note for integrating converters is the number of
// Noise-Free bits.  For a Gaussian-distributed noise floor, this number
// can be obtained by multiplying Sigma by 6, evaluating the number of
// bits required to contain the result, and subtracting this number of
// bits from the 24 available bits, as follows:
//
// 10 LSBs * 6 = 60 LSBs, which can be contained in 6 bits.  Noise-free
// resolution is 24bits - 6 bits = 18 bits.
//
// Refer to 'F350 datasheet tables "ADC0 Electrical Characteristics" and
// "Absolute Maximum Ratings" for the MIN/MAX voltage range on input pins.
//
// If using the eval version of the Keil compiler, set "USE_FLOAT" to '0'
// and calculate the standard deviation by taking the square root
// of "variance".
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are installed on the following:
//    - J3 : P0.4 - TX
//    - J3 : P0.5 - RX
//    - J3 : P0.6 - P0.6_LED
//    - J3 : P0.7 - P0.7_LED
//    - J8 : Power selection, P1 9V or J4 Debugger
//    - J13: VREF+, using external VREF
//    - J14: VREF-
// 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1. After power the board, the red LED D3
//    is on.
// 3) Connect serial cable from the RS232 connector J5 to a PC.
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the virtual COM port at <BAUDRATE> (115200) and 8-N-1.
// 5) Compile and download code to the C8051F350-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 6) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 7) The terminal program will print the samples measured by the device.
//    Also green LED D1 and D2 will toggle between each ADC conversion.
//
//
// Target:         C8051F350/1
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1 (JL)
//    - Updated descriptions
//    - 24 JUN 2014
//
// Release 1.0
//    - Initial Revision (BD / PC / BW)
//    - 26 MAY 2004
//

// set USE_FLOAT to '0' to use EVAL version of Keil compiler

#define USE_FLOAT 1
#define PRINT_STATISTICS 1
#define PRINT_SAMPLES 0
#define PRINT_VOLTAGES 0

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F350_Register_Enums.h>  // SFR declarations
#include <stdio.h>                        // Standard I/O Library
#include <math.h>

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F35x
//-----------------------------------------------------------------------------

SI_SFR16(ADC0DEC, 0x9A);

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       49000000             // SYSCLK frequency (Hz)
#define BAUDRATE     115200               // UART0 Baudrate (bps)

#define MDCLK        2457600              // Modulator Clock (Hz)
#define OWR          10                   // desired Output Word Rate in Hz

#define VREF         250L                 // External VREF (x 10^-2 V)
/*
#define VREF         243UL                // Internal VREF (x 10^-2 V)
*/

SI_SBIT(LED0, SFR_P0, 6);                   // LED0='1' means ON
SI_SBIT(LED1, SFR_P0, 7);                   // LED1='1' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void SYSCLK_Init (void);
void PORT_Init (void);
void ADC0_Init (void);
void IDA0_Init (void);
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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void) {

   volatile long ADC_OutputVal=0;         // Concatenated ADC output value
   long xdata sample_array[128];
   unsigned i;
   long min;
   long max;
   long l_temp;
   long l_average;
   long l_variance;
   long l_owr;

#if (USE_FLOAT == 1)
   float temp;
   float average;
   float variance;
   float stdev;
   float owr;
#endif // USE_FLOAT


   // disable watchdog timer

   SYSCLK_Init();                         // Initialize system clock to 49 MHz

   PORT_Init();                           // Initialize crossbar and GPIO

   LED0 = 0;
   LED1 = 0;

   ADC0_Init();                           // Initialize ADC0

   UART0_Init();                          // Initialize UART0

   IE_EA = 1;                             // enable global interrupts

   printf("\nMeasurements using the 24-bit ADC in C8051F350\n");
   printf("\nCalibrating ...\n");

   EIE1 &= ~EIE1_EADC0__ENABLED;          // Disable ADC0 interrupts

   // Full internal calibration (offset and gain)
   ADC0MD |= ADC0MD_ADSM__FULL_INTERNAL_CAL;

   while (!ADC0STA_ADCALC);               // Wait for calibration complete

   ADC0MD &= ~ADC0MD_ADSM__FMASK;         // clear bits (put ADC0 in IDLE
                                          // mode)

   printf("Calibration complete\n\n");

   ADC0STA_ADINT = 0;                     // clear pending sample indication
   ADC0MD = ADC0MD_ADEN__ENABLED |
            ADC0MD_ADSM__CONT_CONVERSION; // Start continuous conversions

   while(1)
   {

      // capture 128 samples
      printf ("Collecting 128 samples...\n");
      LED0 = 1;
      for (i = 0; i < 128; i++)
      {
         while(!ADC0STA_ADINT);           // wait till conversion complete
         ADC0STA_ADINT = 0;               // clear AD0 interrupt flag

         // concatenate ADC0 data bytes to form the 24-bit value
         ADC_OutputVal = (char)ADC0H;
         ADC_OutputVal <<= 16;
         ADC_OutputVal += (long)ADC0L + ((long)ADC0M << 8);

         sample_array[i] = ADC_OutputVal;
      }
      LED0 = 0;

      // calculate mean, min, and max

#if (USE_FLOAT == 1)
      average = 0;
#endif // USE_FLOAT

      l_average = 0L;
      min = 0x7fffffffL;
      max = 0x80000000L;
      for (i = 0; i < 128; i++)
      {
         ADC_OutputVal = sample_array[i];
         l_average = l_average + ADC_OutputVal;
         if (ADC_OutputVal < min)
            min = ADC_OutputVal;

         if (ADC_OutputVal > max)
            max = ADC_OutputVal;

#if (USE_FLOAT == 1)
         average = average + (float) ADC_OutputVal;
#endif // USE_FLOAT

      }

      l_average = l_average / 128;

#if (USE_FLOAT == 1)
      average = average / 128;
#endif // USE_FLOAT

      // calculate variance
      l_variance = 0L;

#if (USE_FLOAT == 1)
      variance = 0;
#endif // USE_FLOAT

      for (i = 0; i < 128; i++)
      {
         ADC_OutputVal = sample_array[i];

         l_temp = ADC_OutputVal;
         l_temp = l_temp - l_average;
         l_temp = l_temp * l_temp;
         l_variance = l_variance + l_temp;

#if (USE_FLOAT == 1)
         temp = (float) ADC_OutputVal;
         temp = temp - average;
         temp = temp * temp;
         variance = variance + temp;
#endif // USE_FLOAT

      }

      l_variance = l_variance / 127;   // unbiased variance

      l_owr = (long) SYSCLK / (long)(ADC0CLK + 1);
      l_owr = (long) l_owr / (long)(ADC0DEC + 1);
      l_owr = (long) l_owr / (long)128;

#if (USE_FLOAT == 1)
      variance = variance / 127;       // unbiased variance
      stdev = sqrt (variance);

      owr = SYSCLK /(ADC0CLK + 1);
      owr = owr / (ADC0DEC + 1);
      owr = owr / 128;
#endif // USE_FLOAT

      // print statistics
#if (PRINT_STATISTICS == 1)
      LED1 = 1;
      printf ("SYSCLK = %lu\n", (unsigned long) SYSCLK);
      printf ("ADC0CLK = 0x%02x\n", (unsigned) ADC0CLK);
      printf ("ADC0DEC = 0x%02x%02x\n", (unsigned) ADC0DECH,
              (unsigned) ADC0DECL);
      printf ("min = %ld\n", min);
      printf ("max = %ld\n", max);

#if (USE_FLOAT == 1)
      printf ("average = %.2f\n", average);
      printf ("stdev = %.2f\n", stdev);
      printf ("variance = %.2f\n", variance);
      printf ("OWR = %.2f Hz\n", owr);
#else
      printf ("average = %ld\n", l_average);
      printf ("variance = %ld\n", l_variance);
      printf ("OWR = %ld Hz\n", l_owr);
#endif // USE_FLOAT

      printf ("\n");

      LED1 = 0;
#endif // PRINT_STATISTICS

      // print samples
#if (PRINT_SAMPLES == 1)
      for (i = 0; i < 128; i++)
      {
         ADC_OutputVal = sample_array[i];
         printf ("%6ld\n", ADC_OutputVal);
//      printf ("0x%06lx\n", ADC_OutputVal);

      }
#endif // PRINT_SAMPLES

      // print voltages
#if (PRINT_VOLTAGES == 1)
      for (i = 0; i < 128; i++)
      {
         long Calculated_uV;              // Measured voltage in uV
         ADC_OutputVal = sample_array[i];

         // Caculate measured voltage in uV:
         // V (in uV) = ADCcode * VREF * 10 / 2^24
         // Note1: Multiplying by 10 because VREF is in 10^-2 V
         // Note2: Shifting by 4 before multiplying 10 to prevent overflow
         //        of unsigned long variable (32 bits)

         Calculated_uV = ((((((ADC_OutputVal*2*VREF)/16)*10)/1024)*1000)/1024);

         // Output result:
         printf("ADC Output Code = %6ld [Calculated voltage = %+07ld uV]\n",
                  ADC_OutputVal, Calculated_uV);
      }
#endif // PRINT_VOLTAGES


   }// end while(1)
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// This routine initializes the system clock to use the internal 24.5MHz
// oscillator as its clock source, with x 2 multiply for
// 49 MHz operation. Also enables missing clock detector reset.
//
void SYSCLK_Init (void)
{
   unsigned i;

   // enable internal high frequency oscillator
   OSCICN = OSCICN_IOSCEN__ENABLED;
   // select internal high frequency oscillator as sysclk source
   CLKSEL = CLKSEL_CLKSL__DIVIDED_HFOSC;

   // INTOSC configure
   OSCICN = OSCICN_IOSCEN__ENABLED | OSCICN_IFCN__HFOSC_DIV_1;

   // Clock Multiplier configure
   CLKMUL = 0x00;                         // Reset Clock Multiplier

   CLKMUL &= ~CLKMUL_MULSEL__FMASK;       // select INTOSC / 2 as PLL source

   CLKMUL |= CLKMUL_MULEN__ENABLED;       // Enable 4x Multipler (MULEN = 1)

   for (i = 0; i < 125; i++);             // Delay for at least 5us

   // Initialize Multiplier
   CLKMUL |= CLKMUL_MULEN__ENABLED | CLKMUL_MULINIT__SET;

   while (!(CLKMUL & CLKMUL_MULRDY__SET));// Poll for Multiply Ready

// SYSCLK configure

   VDM0CN = VDM0CN_VDMEN__ENABLED;        // enable VDD monitor
   RSTSRC = 0x06;                         // enable missing clock detector
                                          // and VDD monitor reset sources
   CLKSEL = CLKSEL_CLKSL__CLKMUL;         // select PLL as clock source
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Configure the Crossbar and GPIO ports.
// P0.4 - TX0 (push-pull)
// P0.5 - RX0
// P0.6 - LED1 (push-pull)
// P0.7 - LED2 (push-pull)
//
void PORT_Init (void)
{
   XBR0     = XBR0_URT0E__ENABLED;        // UART0 Selected
   // Enable crossbar and weak pull-ups
   XBR1     = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED;
   P0MDOUT |= 0xD0;                       // TX, LEDs = Push-pull
}

//-----------------------------------------------------------------------------
// ADC0_Init extVREF Bipolar AIN0.1-AIN0.0
//-----------------------------------------------------------------------------
//
// This function initializes the ADC to measure across AIN0.1 and AIN0.0
// on the Target Board (Differential measurements, Bipolar codes)
//
void ADC0_Init (void)
{
   unsigned ADC0_decimation;

   REF0CN &= ~REF0CN_REFBE__BMASK;        // disable internal vref
/*
   REF0CN |= REF0CN_REFBE__ENABLED;       // (enable if using internal vref)
*/

   ADC0CN = ADC0CN_ADPOL__BIPOLAR;        // Bipolar output codes, GAIN=1

/*
   // interrupts upon SINC3 filter output and uses internal VREF
   ADC0CF = ADC0CF_ADISEL__SINC3 | ADC0CF_ADVREF__INTERNAL;
*/
   // interrupts upon SINC3 filter output and uses external VREF
   ADC0CF = ADC0CF_ADISEL__SINC3 | ADC0CF_ADVREF__EXTERNAL;

   // Generate MDCLK for modulator.
   // Ideally MDCLK = 2.4576
   ADC0CLK = (SYSCLK/MDCLK)-1;
                                          // Ideally, MDCLK = 2.4576 MHz
//   ADC0DEC = 0x7FF;                     // set slowest OWR



   // program decimation rate for desired OWR

   ADC0_decimation = (unsigned long) SYSCLK/ (unsigned long) OWR /
                     (unsigned long) (ADC0CLK+1)/(unsigned long)128;

   ADC0_decimation--;

   ADC0DEC = ADC0_decimation;


   ADC0BUF = 0x00;                        // Turn off Input Buffers


   // Select Mux inputs


//   ADC0MUX = 0x08;                        // Input pin selection:
                                          // Setup for differential measurements
                                          // AIN+ => AIN0.0
                                          // AIN- => AGND

//   ADC0MUX = 0x00;                        // Input pin selection:
                                          // Setup for differential measurements
                                          // AIN+ => AIN0.0
                                          // AIN- => AIN0.0

   ADC0MUX = ADC0MUX_ADPSEL__ADC0P0 |
             ADC0MUX_ADNSEL__ADC0N1;      // Input pin selection:
                                          // Setup for differential measurements
                                          // AIN+ => AIN0.0
                                          // AIN- => AIN0.1

//   ADC0MUX = 0x10;                        // Input pin selection:
                                          // Setup for differential measurements
                                          // AIN+ => AIN0.1
                                          // AIN- => AIN0.0

//   ADC0MUX = 0x32;                        // Input pin selection:
                                          // Setup for differential measurements
                                          // AIN+ => AIN0.3
                                          // AIN- => AIN0.2

//   ADC0MUX = 0x54;                        // Input pin selection:
                                          // Setup for differential measurements
                                          // AIN+ => AIN0.5
                                          // AIN- => AIN0.4

//   ADC0MUX = 0x76;                        // Input pin selection:
                                          // Setup for differential measurements
                                          // AIN+ => AIN0.7
                                          // AIN- => AIN0.6

//   ADC0MUX = 0xff;                        // Input pin selection:
                                          // Setup for differential measurements
                                          // AIN+ => Temp+
                                          // AIN- => Temp-

//   ADC0MUX = 0x88;                        // Input pin selection:
                                          // Setup for differential measurements
                                          // AIN+ => AGND
                                          // AIN- => AGND

   ADC0MD = ADC0MD_ADEN__ENABLED;         // Enable the ADC0 (IDLE Mode)
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//
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

   TL1 = TH1;                             // init Timer1
   TMOD &= ~(TMOD_GATE1__BMASK |
             TMOD_CT1__BMASK   |
             TMOD_T1M__FMASK);
   // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  TMOD_T1M__MODE2;
   TCON_TR1 = 1;                          // START Timer1
   SCON0_TI = 1;                          // Indicate TX0 ready
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------