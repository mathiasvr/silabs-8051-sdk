//-----------------------------------------------------------------------------
// F36x_Timer2_Two_8bitTimers.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program presents an example of use of the Timer2 of the C8051F36x's in
// two 8-bit reload counter/timer mode. It uses the 'F360DK as HW platform.
//
// It sets Timer2 to be two 8-bit timers that interrupt every millisecond.
//
// When the Timer2 low interrupts a counter (low_counter) increments and upon
// reaching the value defined by LED1_TOGGLE_RATE toggles LED1
// When the Timer2 high interrupts a counter (high_counter) increments and upon
// reaching the value defined by LED2_TOGGLE_RATE toggles LED2
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: P3.2 - P3.2_LED (toggled by TMR2L)
//    J12: P3.3 - P3.3_LED (toggled by TMR2H)
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
// 5) Verify that LED1 and LED2 are blinking.
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

#define TIMER_PRESCALER            12  // Based on Timer2 CKCON and TMR2CN
                                       // settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

// Note: TIMER_TICKS_PER_MS should not exceed 255 (0xFF) for the 8-bit timers

#define AUX1     TIMER_TICKS_PER_MS
#define AUX2     -AUX1

#define LED1_TOGGLE_RATE          100  // LED1 toggle rate in milliseconds
                                       // if LED1_TOGGLE_RATE = 1, LED1 will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

#define LED2_TOGGLE_RATE         30    // LED2 toggle rate in milliseconds
                                       // if LED2_TOGGLE_RATE = 1, LED2 will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

#define TIMER2_RELOAD_HIGH       AUX2  // Reload value for Timer2 high byte
#define TIMER2_RELOAD_LOW        AUX2  // Reload value for Timer2 low byte

SI_SBIT(LED1, SFR_P3, 2);                 // LED1='1' means ON
SI_SBIT(LED2, SFR_P3, 3);                 // LED2='1' means ON

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Port_Init (void);                 // Port initialization routine
void Timer2_Init (void);               // Timer2 initialization routine

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

   Timer2_Init ();                     // Initialize the Timer2
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
// Pinout:
//
//    P3.2 -> LED1 (toggled by TMR2L)
//
//    P3.3 -> LED2 (toggled by TMR2H)
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   XBR1 = XBR1_XBARE__ENABLED;         // Enable crossbar

   P3MDOUT = 0x0C;                     // Set LED1 and LED2 to be push-pull

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures Timer2 as two 8-bit reload timers with interrupt.
// Using the internal osc. at 24.5MHz/8 with a timer prescaler of 1:12
// reloading the TMR2H register with TIMER2_RELOAD_HIGH and TMR2L with
// TIMER2_RELOAD_HIGH.
//
// Note: The Timer2 uses a 1:12 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void Timer2_Init(void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   CKCON &= ~0x60;                     // Timer2 uses SYSCLK/12
   TMR2CN &= ~0x01;

   TMR2RLH = TIMER2_RELOAD_HIGH;       // Init Timer2 Reload High register
   TMR2RLL = TIMER2_RELOAD_LOW;        // Init Timer2 Reload Low register

   TMR2H = TIMER2_RELOAD_HIGH;         // Init Timer2 High register
   TMR2L = TIMER2_RELOAD_LOW;          // Init Timer2 Low register

   TMR2CN = TMR2CN_TF2LEN__ENABLED |   // Low byte interrupt enabled
            TMR2CN_T2SPLIT__8_BIT_RELOAD | // and Split-Mode enabled
            TMR2CN_TR2__RUN;           // Timer2 enabled


   IE_ET2 = 1;                         // Timer2 interrupt enabled

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This function processes the Timer2 interrupt. It checks both counters for
// matches and, upon a match, toggles LED1 and LED2.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   static uint16_t low_counter=0;  // Define counter variables
   static uint16_t high_counter=0;

   if(TMR2CN_TF2L == 1)
   {
      if((low_counter++) == LED1_TOGGLE_RATE)
      {
         low_counter = 0;              // Reset interrupt counter
         LED1 = ~LED1;                 // Toggle LED1
      }
      TMR2CN_TF2L = 0;
   }
   if(TMR2CN_TF2H == 1)
   {
      if((high_counter++) == LED2_TOGGLE_RATE)
      {
         high_counter = 0;             // Reset interrupt counter
         LED2 = ~LED2;                 // Toggle LED2
      }
      TMR2CN_TF2H = 0;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//----------------------------------------------------------------------------