//-----------------------------------------------------------------------------
// F96x_Timer0_13bitExtTimer.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program presents an example of use of the Timer0 in
// 13-bit counter/timer in counter mode from an external pin. It uses two I/O
// pins; one to create the input pulses (SIGNAL) to be counted and another one
// to enable the counting process (GTE).
//
//
// Pinout:
//
//    P0.0 -> LED (digital, open drain)
//    P0.1 -> /INT0
//    P0.2 -> BUTTON3 (switch)
//    P0.3 -> T0 (Timer0 External Input)
//    P1.4 -> SIGNAL (digital, push-pull)
//    P1.5 -> GTE (digital, push-pull)

//
//    all other port pins unused
//
// How To Test:
//
// 1) In "Define Hardware", #define either "UDP_F960_MCU_MUX_LCD" or
//    "UDP_F960_MCU_EMIF" depending on which board you are using.
// 2) To change the number of input pulses/interrupt, modify
//    PULSES_PER_TOGGLE
// 3) To change the speed of the SIGNAL waveform, modify
//    SOFTWARE_DELAY
// 4) Connnect the following pins:
//                   P0.3 <--> P1.4 (T0 with SIGNAL)
//                   P0.1 <--> P1.5 (/INT0 with GTE)
//
// 5) Connect the USB Debug Adapter to J13.
// 6) Move the VBAT SEL switch (SW5) to the middle VREG position.
// 7) Move the VIO switch (SW7) and VIORF switch (SW12) to the VBAT position.
// 8) Connect a 9V power adapter to P1 of the development board.
// 9) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 10) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 11) To enable the counting, press and hold SW3 (BUTTON3), which will be
//    polled to enable the timer.
// 12) LED1 will blink and SIGNAL can be observed on an oscilloscope on P1.4.
//
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Added support for the EMIF board.
//    - 20 JAN 2014
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F960_Register_Enums.h>

//-----------------------------------------------------------------------------
// Define Hardware
//-----------------------------------------------------------------------------
//#define UDP_F960_MCU_MUX_LCD
#define UDP_F960_MCU_EMIF
//-----------------------------------------------------------------------------
// Hardware Dependent definitions
//-----------------------------------------------------------------------------
#ifdef UDP_F960_MCU_MUX_LCD
SI_SBIT(LED1, SFR_P0, 0);
SI_SBIT(BUTTON3, SFR_P0, 2);             // Button that enables counting
#endif

#ifdef UDP_F960_MCU_EMIF
SI_SBIT(LED1, SFR_P3, 0);
SI_SBIT(BUTTON3, SFR_P3, 2);             // Button that enables counting
#endif

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK           (20000000L/8L) // SYSCLK in Hz (20 MHz internal
                                       // low power oscillator / 8)
                                       // the low power oscillator has a
                                       // tolerance of +/- 10%
                                       // the precision oscillator has a
                                       // tolerance of +/- 2%

#define PULSES_PER_TOGGLE       100    // Arbitrary number of pulses in the
                                       // input pin necessary to create an
                                       // interrupt.
                                       // Limited to 0x1FFF or 8191d for a
                                       // 13-bit timer

#define SOFTWARE_DELAY  (SYSCLK/100000L) // Software timer to generate the
                                       // SIGNAL output
                                       // Generate a signal in the kHz range

#define AUX0 0x1FFF-PULSES_PER_TOGGLE+1
#define AUX1 AUX0&0x001F               // 5 LSBs of timer value in TL0[4:0]
#define AUX2 ((AUX0&0x1FFF)>>5)        // High 8 bits of timer in TH0

#define TIMER0_RELOAD_HIGH      AUX2   // Reload value for Timer0 high byte
#define TIMER0_RELOAD_LOW       AUX1   // Reload value for Timer0 5 LSBs

SI_SBIT(GTE, SFR_P1, 5);                 // Gate control signal for Timer0
SI_SBIT(SIGNAL, SFR_P1, 4);              // SIGNAL is used to input pulses into
                                       // T0 pin


//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);



//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Port_Init (void);                 // Port initialization routine
void Timer0_Init (void);               // Timer0 initialization routine
SI_INTERRUPT_PROTO(TIMER0_ISR, TIMER0_IRQn);

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
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   volatile uint8_t counter = 0;

                                       // enable)

   Timer0_Init ();                     // Initialize the Timer0
   Port_Init ();                       // Init Ports

   LED1 = 1;                            // Turn off LED
   IE_EA = 1;                             // Enable global interrupts
   while (1)
   {
      if (BUTTON3 == 0)                // If button pressed, enable counting
      {
         GTE = 1;
      }
      else
      {
         GTE = 0;
      }

      // Wait a certain time before toggling signal
      for(counter = 0; counter < SOFTWARE_DELAY; counter++);

      SIGNAL = !SIGNAL;                // Toggle the SIGNAL pin
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P0.0   digital   push-pull    LED
// P0.1   digital   open-drain   /INT0
// P0.2   digital   open-drain   BUTTON3
// P0.3   digital   open-drain   T0 (Timer0 External Input)

// P1.4   digital   push-pull    SIGNAL
// P1.5   digital   push-pull    GTE

//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   SFRPAGE = LEGACY_PAGE;

   IT01CF = 0x01;                      // /INT0 available at P0.1.
   XBR1 = 0x10;                        // T0 available at the I/O pins
   XBR2 = 0x40;                        // Enable crossbar
   P0MDOUT = 0x01;
   P0SKIP = 0x07;
   P1MDOUT = 0x30;                     // Set P1.4/5 to be used as push-pull
}

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Timer0 as a 13-bit timer, interrupt enabled.
// Using an external signal as clock source 1:8 and reloading the
// TH0 and TL0 registers it will interrupt and then toggle the LED upon roll-
// over of the timer.
//
// Note: In this example the GATE0 gate control is used.
//-----------------------------------------------------------------------------
void Timer0_Init(void)
{
   SFRPAGE = LEGACY_PAGE;

   TMOD = 0x0C;                        // Timer0 in 13-bit mode ext. counter
                                       // gated counting T0 input
   IE_ET0 = 1;                            // Timer0 interrupt enabled
   TCON = 0x11;                        // Timer0 ON with INT0 edge active
   TH0 = TIMER0_RELOAD_HIGH;           // Reinit Timer0 High register
   TL0 = TIMER0_RELOAD_LOW;            // Reinit Timer0 Low register
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer0 interrupt and toggle the LED
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER0_ISR, TIMER0_IRQn)
{
   SFRPAGE = LEGACY_PAGE;

   LED1 = !LED1;                         // Toggle the LED
   TH0 = TIMER0_RELOAD_HIGH;           // Reinit Timer0 High register
   TL0 = TIMER0_RELOAD_LOW;            // Reinit Timer0 Low register
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
