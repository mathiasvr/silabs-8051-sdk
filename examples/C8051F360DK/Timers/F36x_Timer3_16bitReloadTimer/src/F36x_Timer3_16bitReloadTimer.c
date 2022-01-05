//-----------------------------------------------------------------------------
// F36x_Timer3_16bitReloadTimer.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program presents an example of use of the Timer3 of the C8051F36x's in
// 16-bit reload counter/timer mode. It uses the 'F36xDK as HW platform.
//
// The timer reload registers are initialized with a certain value so that
// the timer counts from that value up to FFFFh and when it overflows an
// interrupt is generated and the timer is automatically reloaded.
// In this interrupt the ISR toggles the LED.
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
// 5) Check that the LED is blinking
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
//    -23 OCT 2005


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

#define TIMER_PRESCALER            12  // Based on Timer3 CKCON and TMR3CN
                                       // settings

#define LED_TOGGLE_RATE            50  // LED toggle rate in milliseconds
                                       // if LED_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

// Note: LED_TOGGLE_RATE*TIMER_TICKS_PER_MS should not exceed 65535 (0xFFFF)
// for the 16-bit timer

#define AUX1     TIMER_TICKS_PER_MS*LED_TOGGLE_RATE
#define AUX2     -AUX1

#define TIMER3_RELOAD            AUX2  // Reload value for Timer3

SI_SBIT(LED, SFR_P3, 2);                  // LED='1' means ON

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Port_Init (void);                 // Port initialization routine
void Timer3_Init (void);               // Timer3 initialization routine

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Clear watchdog timer enable
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   Timer3_Init ();                     // Initialize the Timer3
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
   P3MDOUT = 0x04;                     // Set LED to push-pull

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Timer3_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures Timer3 as a 16-bit reload timer, interrupt enabled.
// Using the SYSCLK at 16MHz/8 with a 1:12 prescaler.
//
// Note: The Timer3 uses a 1:12 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void Timer3_Init(void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   CKCON &= ~0x60;                     // Timer3 uses SYSCLK/12
   TMR3CN &= ~0x01;

   TMR3RL = TIMER3_RELOAD;             // Reload value to be used in Timer3
   TMR3 = TMR3RL;                      // Init the Timer3 register

   TMR3CN = TMR3CN_TR3__RUN;           // Enable Timer3 in auto-reload mode
   EIE1 |= EIE1_ET3__ENABLED;          // Timer3 interrupt enabled

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer3_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer3 interrupt and toggle the LED
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer3_ISR, TIMER3_IRQn)
{
   LED = ~LED;                         // Toggle the LED
   TMR3CN &= ~TMR3CN_TF3H__BMASK;      // Reset Interrupt
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------