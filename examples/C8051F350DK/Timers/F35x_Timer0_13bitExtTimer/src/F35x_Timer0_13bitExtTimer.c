//-----------------------------------------------------------------------------
// F35x_Timer0_13bitExtTimer.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// Use Timer0 as a single 13-bit timer with an external clock source
//
// This program presents an example of use of the Timer0 of the C8051F35x's in
// 13-bit counter/timer in counter mode from an external pin. It uses two I/O
// pins; one to create the input pulses (SIGNAL) to be counted and another one
// to enable the counting process (GTE).
//
// This code uses the 'F350DK as HW platform.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are installed on the following:
//    - J3 : P0.6 - P0.6_LED
//    - J3 : P1.0 - P1.0_SW
//    - J10: P0.3
//    - J1 : P0.0 - P0.3 (T0 with SIGNAL)
//    - J1 : P0.1 - P0.5 (/INT0 with GTE)
//    - J8 : Power selection, P1 9V or J4 Debugger
// 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1. After power the board, the red LED D3
//    is on.
// 3) Compile and download code to the C8051F350-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) To enable the counting, press and hold BUTTON1 (switch), which will be
//    polled to enable the timer.
// 6) The LED will blink and SIGNAL can be observed on an oscilloscope.
//
//
// Target:         C8051F35x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1 (JL)
//    - Updated descriptions
//    - 26 JUN 2014
//
// Release 1.0
//    -Initial Revision (CG)
//    -14 DEC 2005


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F350_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000/8  // SYSCLK in Hz (24.5 MHz internal
                                       // oscillator / 8)
                                       // the internal oscillator has a
                                       // tolerance of +/- 2%

#define PULSES_PER_TOGGLE       1000   // Arbitrary number of pulses in the
                                       // input pin necessary to create an
                                       // interrupt.
                                       // Limited to 0x1FFF or 8191d for a
                                       // 13-bit timer

#define SOFTWARE_DELAY  SYSCLK/100000  // Software timer to generate the
                                       // SIGNAL output
                                       // Generate a signal in the kHz range

#define AUX0 0x1FFF-PULSES_PER_TOGGLE+1
#define AUX1 AUX0&0x001F               // 5 LSBs of timer value in TL0[4:0]
#define AUX2 ((AUX0&0x1FFF)>>5)        // High 8 bits of timer in TH0

#define TIMER0_RELOAD_HIGH      AUX2   // Reload value for Timer0 high byte
#define TIMER0_RELOAD_LOW       AUX1   // Reload value for Timer0 5 LSBs

SI_SBIT(SIGNAL,  SFR_P0, 3);             // SIGNAL is used to input pulses into
                                       // T0 pin
SI_SBIT(GTE,     SFR_P0, 5);             // Gate control SIGNAL for Timer0
SI_SBIT(LED,     SFR_P0, 6);             // LED='1' means ON
SI_SBIT(BUTTON1, SFR_P1, 0);             // Button that enables counting

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Port_Init (void);                 // Port initialization routine
void Timer0_Init (void);               // Timer0 initialization routine

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
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   uint16_t counter;


   Timer0_Init ();                     // Initialize the Timer0
   Port_Init ();                       // Init Ports

   LED = 0;
   IE_EA = 1;                          // Enable global interrupts

   while (1)
   {
      if (BUTTON1 == 0)                // If button pressed, enable counting
      {
         GTE = 1;
      }
      else
      {
         GTE = 0;
      }

      // Wait a certain time before toggling signal
      for (counter=0; counter < SOFTWARE_DELAY; counter++);

      SIGNAL = ~SIGNAL;                // Toggle the SIGNAL pin
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
// Pinout:
//
//    P0.0 -> T0 (Timer0 External Input)
//    P0.1 -> /INT0
//
//    P0.3 -> SIGNAL (digital, push-pull)
//
//    P0.5 -> GTE (digital, push-pull)
//    P0.6 -> LED1 (digital, push-pull)
//
//    P1.0 -> BUTTON1 (switch)
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // Enable crossbar and weak pull-ups, T0 available at the I/O pins
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED |
          XBR1_XBARE__ENABLED            |
          XBR1_T0E__ENABLED;

   P0MDOUT = 0x68;                     // Set I/Os to be used as push-pull
}

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Timer0 as a 13-bit timer, interrupt enabled.
// Using an external SIGNAL as clock source 1:8 and reloading the
// TH0 and TL0 registers it will interrupt and then toggle the LED upon roll-
// over of the timer.
//
// Note: In this example the GATE0 gate control is used.
//-----------------------------------------------------------------------------
void Timer0_Init(void)
{
   // Timer0 in 13-bit mode ext. counter, gated counting T0 input
   TMOD = TMOD_T0M__MODE0     |
          TMOD_GATE0__ENABLED |
          TMOD_CT0__COUNTER;

   IE_ET0 = 1;                         // Timer0 interrupt enabled
   // Timer0 ON with INT0 edge active
   TCON = TCON_TR0__RUN | TCON_IT0__EDGE;
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
// Here we process the Timer0 interrupt and toggle the LED1
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer0_ISR, TIMER0_IRQn)
{
   LED = ~LED;                         // Toggle the LED
   TH0 = TIMER0_RELOAD_HIGH;           // Reinit Timer0 High register
   TL0 = TIMER0_RELOAD_LOW;            // Reinit Timer0 Low register
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------