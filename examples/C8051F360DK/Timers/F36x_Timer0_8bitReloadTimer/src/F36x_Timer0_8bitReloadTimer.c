//-----------------------------------------------------------------------------
// F36x_Timer0_8bitReloadTimer.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program presents an example of use of the Timer0 of the C8051F36x's in
// 8-bit counter/timer with reload mode. It uses the 'F360DK as HW platform.
// It uses the Timer0 to create an interrupt at a certain rate and when the
// user's interrupt counter reaches the selected value the LED is toggled.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: P3.2 - P3.2_LED
//    J1: Power selection, P2 9V, P4 USB or J9 Debugger
// 2) Connect the USB Debug Adapter to J9. If 9V power is selected on J1,
//    connect the 9V power adapter to P2. If VBUS is selected on J1, connect
//    USB cable to P4.
// 3) Compile and download code to C8051F360 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) Check the toggling of the LED (P3.2)
//
//
// Target:         C8051F36x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1
//    - Updated to use with Simplicity Studio (JL)
//    - 29 JUL 2014
//
// Release 1.0
//    -Initial Revision (CG)
//    -16 OCT 2005


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000/8  // SYSCLK in Hz (24.5 MHz internal
                                       // oscillator / 8)
                                       // the internal oscillator has a
                                       // tolerance of +/- 2%

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

SI_SBIT(LED, SFR_P3, 2);                  // LED

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   Timer0_Init ();                     // Initialize the Timer0
   Port_Init ();                       // Init Ports
   IE_EA = 1;                          // Enable global interrupts

   SFRPAGE = LEGACY_PAGE;

   while (1);                          // Loop forever
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
//    P3.2 -> LED
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   XBR1 = XBR1_XBARE__ENABLED;         // Enable crossbar
   P3MDOUT = 0x04;                     // Set LED as push-pull

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   TH0 = TIMER0_RELOAD_HIGH;           // Init Timer0 High register
   TL0 = TH0;                          // Set the intial Timer0 value

   TMOD = TMOD_T0M__MODE2;             // Timer0 in 8-bit reload mode
   CKCON = CKCON_SCA__SYSCLK_DIV_48;   // Timer0 uses a 1:48 prescaler
   IE_ET0 = 1;                         // Timer0 interrupt enabled
   TCON = TCON_TR0__RUN;               // Timer0 ON

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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