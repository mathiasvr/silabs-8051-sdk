//-----------------------------------------------------------------------------
// F53xA_Timer0_8bitReloadTimer.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// Use Timer0 as a single 8-bit timer
//
// This program presents an example of use of the Timer0 of the C8051F53xA's in
// 8-bit counter/timer with reload mode. It uses the 'F530A TB as the HW
// platform. It uses the Timer0 to create an interrupt at a certain rate and
// when the user's interrupt counter reaches the selected value the LED is
// toggle.
//
// Pinout:
//
//    P1.3 -> LED
//
//    all other port pins unused
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F530A TB are placed on the following:
//    "A" SIDE
//    HDR4: P1.3_A - P1.3_LED_A
//    "B" SIDE
//    HDR3: P1.3_B - P1.3_LED_B
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
// 5) Check the toggling of the LED (P1.3).
//
//
// Target:         C8051F52xA/F53xA (C8051F530A TB)
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
//    -11 Jun. 2007
//
// Note: All time estimates are for SYSCLK freq ~ 24.5MHz

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F530_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000/8  // Internal oscillator frequency in Hz

#define BAUDRATE                 9600  // Baud rate of UART in bps

#define TIMER_PRESCALER            48  // Based on Timer CKCON settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

// Note: TIMER_TICKS_PER_MS should not exceed 255 (0xFF) for the 8-bit timer

#define AUX1     TIMER_TICKS_PER_MS
#define AUX2     -AUX1

#define LED_TOGGLE_RATE           100  // LED toggle rate in milliseconds
                                       // if LED_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

#define TIMER0_RELOAD_HIGH       AUX2  // Reload value for Timer0 high byte

SI_SBIT(LED, SFR_P1, 3);                 // LED

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Port_Init (void);
void Timer0_Init(void);

void Delay(void);

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

   Oscillator_Init ();
   Timer0_Init ();                     // Initialize the Timer0
   Port_Init ();                       // Init Ports
   IE_EA = 1;                          // Enable global interrupts

   while (1);                          // Loop forever

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
//    P1.3 -> LED
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1MDOUT = 0x08;                     // Set LED to push-pull

   // Enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED |
          XBR1_XBARE__ENABLED;
}

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Timer0 as a 8-bit timer with reload, interrupt
// enabled.
// Using the internal osc. at 24.5MHz with a prescaler of 1:8 and reloading
// TL0 register with TH0 it will interrupt aproximately one time every 1msec.
//
// Note: The Timer0 uses a 1:48 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void Timer0_Init(void)
{
   TH0 = TIMER0_RELOAD_HIGH;           // Init Timer0 High register
   TL0 = TH0;                          // Set the intial Timer0 value

   TMOD = TMOD_T0M__MODE2;             // Timer0 in 8-bit reload mode
   CKCON = CKCON_SCA__SYSCLK_DIV_48;   // Timer0 uses a 1:48 prescaler
   IE_ET0 = 1;                         // Timer0 interrupt enabled
   TCON = TCON_TR0__RUN;               // Timer0 ON
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer0 interrupt and toggle the LED when appropriate
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer0_ISR, TIMER0_IRQn)
{
   static int16_t counter = 0;

   if((counter++) == LED_TOGGLE_RATE)
   {
      LED = ~LED;                      // Toggle the LED
      counter = 0;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------