//-----------------------------------------------------------------------------
// F53xA_Timer0_13bitExtTimer.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// Use Timer0 as a single 13-bit timer with an external clock source
//
// This program presents an example of use of the Timer0 of the C8051F53xA's in
// 13-bit counter/timer in counter mode from an external pin. It uses two I/O
// pins; one to create the input pulses (SIGNAL) to be counted and another one
// to enable the counting process (GTE).
//
// This code uses the 'F530A TB as the HW platform.
//
// Pinout:
//
//    P0.0 -> T0 (Timer0 External Input)
//    P0.1 -> /INT0
//
//    P1.3 -> LED (digital, push-pull)
//    P1.4 -> BUTTON1 (switch)
//    P1.5 -> GTE (digital, push-pull)
//
//    P1.7 -> SIGNAL (digital, push-pull)
//
//    all other port pins unused
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F530A TB are placed on the following:
//    "A" SIDE
//    HDR4: P1.3_A - P1.3_LED_A
//    HDR4: P1.4_A - P1.4_SW_A
//    J2:   P0.0_A - P1.7_A (T0 with SIGNAL)
//    J2:   P0.1_A - P1.5_A (/INT0 with GTE)
//    J4:   P1.7_A
//    "B" SIDE
//    HDR3: P1.3_B - P1.3_LED_B
//    HDR3: P1.4_B - P1.4_SW_B
//    J1:   P0.0_B - P1.7_B (T0 with SIGNAL)
//    J1:   P0.1_B - P1.5_B (/INT0 with GTE)
// 2) Connect the USB Debug Adapter to HDR2 for "A" SIDE or HDR1 for "B" SIDE.
//    Connect the 9V power adapter to P5.
// 3) Compile and download code to C8051F530A target board
//    (either "A" SIDE or "B" SIDE) with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) Press and hold BUTTON1 (switch), which will be polled to enable the
//    timer. The LED will blink and SIGNAL can be observed on an oscilloscope.
//
//
// Target:          C8051F52xA/F53xA (C8051F530A TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.3 / 09 SEP 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.2 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.1
//    -Updated for 'F530A TB (TP)
//    -31 JAN 2008
//
// Release 1.0
//    -Initial Revision (SM)
//    -11 JUN 2007


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F530_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000/8  // SYSCLK in Hz (24.5 MHz internal
                                       // oscillator / 8)
                                       // the internal oscillator has a
                                       // tolerance of +/- 2%

#define PULSES_PER_TOGGLE        1000  // Arbitrary number of pulses in the
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

SI_SBIT(GTE,    SFR_P1, 5);              // Gate control signal for Timer0
SI_SBIT(LED,    SFR_P1, 3);              // LED='1' means ON
SI_SBIT(SIGNAL, SFR_P1, 7);              // SIGNAL is used to input pulses into
                                       // T0 pin
SI_SBIT(BUTTON1, SFR_P1, 4);             // Button that enables counting

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Port_Init (void);                 // Port initialization routine
void Timer0_Init (void);               // Timer0 initialization routine
void Oscillator_Init (void);           // Internal Oscillator initialization

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable the watchdog timer;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   uint16_t counter;


   Oscillator_Init();
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
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at 24.5/8 MHz.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   // Set internal oscillator to run at its maximum frequency
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_8;
}

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
//    P1.3 -> LED (digital, push-pull)
//    P1.4 -> BUTTON1 (switch)
//    P1.5 -> GTE (digital, push-pull)
//
//    P1.7 -> SIGNAL (digital, push-pull)
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1MDOUT = 0xA8;                     // Set P1.3/5/7 to be used as push-pull

   // Enable crossbar and weak pull-ups, T0 routed to Port pin.
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED |
          XBR1_XBARE__ENABLED |
          XBR1_T0E__ENABLED;
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
// Note: In this example, the GATE0 gate control is used.
//-----------------------------------------------------------------------------
void Timer0_Init(void)
{
   TMOD = 0x0C;                        // Timer0 in 13-bit mode ext. counter
                                       // gated counting T0 input
   IE_ET0 = 1;                         // Timer0 interrupt enabled
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
// Toggle the LED and reset the Timer0 registers.
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