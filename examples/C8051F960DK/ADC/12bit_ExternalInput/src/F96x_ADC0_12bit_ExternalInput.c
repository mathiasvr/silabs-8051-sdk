//-----------------------------------------------------------------------------
// F96x_ADC0_12bit_ExternalInput.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This example code uses ADC in 12-bit mode to take analog measurements from 
// input  P0.6 (Potentiometer input), then prints the results to a terminal 
// window via the UART.
//
// The system is clocked by the internal 24.5MHz oscillator.  Timer 2 triggers
// a conversion on ADC0 on each overflow.  The completion of this conversion
// in turn triggers an interrupt service routine (ISR).  
//
// The analog multiplexer selects P0.6 as the ADC0 input. All samples are 
// single-ended with respect to GND. P0.6 is configured as an analog input 
// in the port initialization routine.
//
// When J18 is shorted between P0.6 and POT, the potentiometer (R34) wiper is
// connected to P0.6.
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
// Timer 2 is set to start a conversion every 100uS
//
// Resources:
// ---------------
// Timer1: clocks UART
// Timer2: overflow initiates ADC conversion
//
// How To Test:
// ------------
// 1) In "Define Hardware", #define either "UDP_F960_MCU_MUX_LCD" or
//    "UDP_F960_MCU_EMIF" depending on which board you are using.
// 2) Ensure that jumpers are placed on the following:
//       J16 (LCD Only):
//             P0.6/POT
//       P2 (LCD) or P12 (EMIF):
//             P0.4/MCU_TX
//             P0.5/MCU_RX
// 3) Connect a USB cable from the development board J17 to a PC
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the USB port (virtual com port) at <BAUDRATE> and 8-N-1
// 5) Connect the USB Debug Adapter to J13.
// 6) Move the VBAT SEL switch (SW5) to the middle VREG position.
// 7) Move the VIO switch (SW7) and VIORF switch (SW12) to the VBAT position.
// 8) Connect a 9V power adapter to P1 of the development board.
// 9) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 10) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 11) Adjust the on-board potentiometer (POT). HyperTerminal will print the
//    voltage measured by the device if everything is working properly
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Added a newline character during printing for better formatting.
//    - 16 JAN 2014
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F960_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // SYSCLK frequency in Hz
#define BAUDRATE     115200            // Baud rate of UART in bps

#define SAMPLERATE   100               // ADC word rate in Hz
//-----------------------------------------------------------------------------
// Define Hardware
//-----------------------------------------------------------------------------
#define UDP_F960_MCU_MUX_LCD
//#define UDP_F960_MCU_EMIF
//-----------------------------------------------------------------------------
// Hardware Dependent definitions
//-----------------------------------------------------------------------------
#ifdef UDP_F960_MCU_MUX_LCD
SI_SBIT(POT_EN, SFR_P1, 4);
#endif

#ifdef UDP_F960_MCU_EMIF
SI_SBIT(POT_EN, SFR_P2, 7);
#endif

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
   
   POT_EN = 0;                         // Enable the Potentiometer

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
// This routine initializes the system clock to use the internal 24.5MHz
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   SFRPAGE = CONFIG_PAGE;
   FLSCL     = 0x40;                   // Set BYPASS bit for >12.5 MHz.

   SFRPAGE = LEGACY_PAGE;
   REG0CN |= 0x10;                     // Enable the precision osc. bias
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
// P1.4 - Potentiometer enable (open-drain)
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   SFRPAGE = LEGACY_PAGE;

#ifdef UDP_F960_MCU_MUX_LCD
   // Analog Input
   P0MDIN   &= ~0x40;                  // Set P0.6 as an analog input
   P0MDOUT  &= ~0x40;                  // Set P0.6 to open-drain
   P0       |=  0x40;                  // Set P0.6 latch to '1'
   P0SKIP   |=  0x40;                  // Skip P0.6 in the Crossbar
#endif

#ifdef UDP_F960_MCU_EMIF
   // Analog Input
   P1MDIN   &= ~0x10;                  // Set P1.4 as an analog input
   P1MDOUT  &= ~0x10;                  // Set P1.4  to open-drain
   P1       |=  0x10;                  // Set P1.4  latch to '1'
   P1SKIP   |=  0x10;                  // Skip P1.4 in the Crossbar
#endif
   
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
   SFRPAGE = LEGACY_PAGE;

   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
   CKCON  &= ~0x30;                    // use SYSCLK/12 as timebase, 
                                       // 16-bit auto-reload

   TMR2RL  = 65535 - (SYSCLK / 12 / SAMPLERATE); // init reload value 
   TMR2    = TMR2RL;                   // init timer value
   TMR2CN_TR2     = 1;                        // start Timer2
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures ADC0 to make single-ended analog measurements on pin P0.6
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   SFRPAGE = LEGACY_PAGE;

   ADC0CN = 0x42;                      // ADC0 disabled, Burst Mode enabled,
                                       // conversion triggered on TMR2 overflow
  
   REF0CN = 0x18;                      // Select internal high speed voltage
                                       // reference

#ifdef UDP_F960_MCU_MUX_LCD
   ADC0MX = 0x06;                      // Select P0.6 as the ADC input pin
#endif

#ifdef UDP_F960_MCU_EMIF
   ADC0MX = 0x0C;                      // Select P1.4 as the ADC input pin
#endif

   ADC0CF = ((SYSCLK/8300000))<<3;     // Set SAR clock to 8.3MHz

   ADC0CF |= 0x00;                     // Select Gain of 0.5
   
   ADC0AC = 0x81;                      // Right-justify results, shifted right
                                       // by 0 bits. Accumulate 4 samples for
                                       // an output word of 12-bits. 12-bit mode
                                       // is enabled

   EIE1 |= 0x08;                       // Enable ADC0 conversion complete int.

   
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
   SFRPAGE = LEGACY_PAGE;

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

   uint32_t result;
   uint32_t mV;                   // measured voltage in mV
   static uint8_t sample_count = 0;

   SFRPAGE = LEGACY_PAGE;

   ADC0CN_ADINT = 0;                         // clear ADC0 conv. complete flag
   
   result = ADC0;                      // copy ADC0 into result
 
   // The 12-bit ADC value represents the voltage applied
   // to P0.6 according to the following equation:
   //
   //                           Vref (mV)
   //   measurement (mV) =   --------------- * result (bits)
   //                       ((2^10)-1)*2^2 (bits)

   mV =  result * 3300 / 4092;
   
   // Print the results every 10 samples
   if(sample_count >= 10)
   {
      printf("\rP0.6 voltage: %4d mV\n", (uint16_t) mV);
      sample_count = 0;
   
   } else
   {
      sample_count++;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
