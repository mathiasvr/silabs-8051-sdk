//-----------------------------------------------------------------------------
// F96x_Timer2_two_8bitTimers.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program presents an example of use of the Timer2 in
// two 8-bit reload counter/timer mode. .
//
// It sets Timer2 to be two 8-bit timers that interrupt every millisecond.
//
// When the Timer2 low interrupts a counter (low_counter) increments and upon
// reaching the value defined by LED1_TOGGLE_RATE toggles LED1.
// When the Timer2 high interrupts a counter (high_counter) increments and upon
// reaching the value defined by LED2_TOGGLE_RATE toggles LED2.
//
// Pinout:
//
//    P0.0(LCD)/P3.0(EMIF) -> LED1 (toggled by TL0)
//    P0.1(LCD)/P3.1(EMIF) -> LED2 (toggled by TH0)
//
//    all other port pins unused
//
// How To Test:
//
// 1) In "Define Hardware", #define either "UDP_F960_MCU_MUX_LCD" or
//    "UDP_F960_MCU_EMIF" depending on which board you are using.
// 2) If a different LED blink rate is required change the following constants:
//       -> LED1_TOGGLE_RATE in milliseconds
//       -> LED2_TOGGLE_RATE in milliseconds
// 3) Connect the USB Debug Adapter to J13.
// 4) Move the VBAT SEL switch (SW5) to the middle VREG position.
// 5) Move the VIO switch (SW7) and VIORF switch (SW12) to the VBAT position.
// 6) Connect a 9V power adapter to P1 of the development board.
// 7) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) LED1 and LED2 should blink rapidly.
//
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
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
#define UDP_F960_MCU_MUX_LCD
//#define UDP_F960_MCU_EMIF
//-----------------------------------------------------------------------------
// Hardware Dependent definitions
//-----------------------------------------------------------------------------
#ifdef UDP_F960_MCU_MUX_LCD
SI_SBIT(LED1, SFR_P0, 0);
SI_SBIT(LED2, SFR_P0, 1);
#endif

#ifdef UDP_F960_MCU_EMIF
SI_SBIT(LED1, SFR_P3, 0);
SI_SBIT(LED2, SFR_P3, 1);
#endif

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             (20000000L/8L) // SYSCLK in Hz (20 MHz internal
                                          // low power oscillator / 8)
                                          // the low power oscillator has a
                                          // tolerance of +/- 10%
                                          // the precision oscillator has a
                                          // tolerance of +/- 2%

#define TIMER_PRESCALER            12L    // Based on Timer2 CKCON and TMR2CN
                                          // settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  (SYSCLK/TIMER_PRESCALER/1000L)

// Note: TIMER_TICKS_PER_MS should not exceed 255 (0xFF) for the 8-bit timers

#define AUX1     TIMER_TICKS_PER_MS
#define AUX2     -AUX1

#define LED1_TOGGLE_RATE        100    // LED1 toggle rate in milliseconds
                                       // if LED1_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

#define LED2_TOGGLE_RATE           30  // LED2 toggle rate in milliseconds
                                       // if LED2_TOGGLE_RATE = 1, the
                                       // SIGNAL output will be on for 1
                                       // millisecond and off for 1
                                       // millisecond

#define TIMER2_RELOAD_HIGH       AUX2     // Reload value for Timer2 high byte
#define TIMER2_RELOAD_LOW        AUX2     // Reload value for Timer2 low byte


//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);


//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Port_Init (void);                    // Port initialization routine
void Timer2_Init (void);                  // Timer2 initialization routine
SI_INTERRUPT_PROTO(TIMER2_ISR, TIMER2_IRQn);

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
   PCA0MD &= ~0x40;                       // Clear watchdog timer enable
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   Timer2_Init ();                        // Initialize the Timer2
   Port_Init ();                          // Init Ports

   IE_EA = 1;                                // Enable global interrupts

   while (1);                             // Loop forever
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
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   SFRPAGE = LEGACY_PAGE;

   XBR2 = 0x40;                           // Enable crossbar
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
   SFRPAGE = LEGACY_PAGE;

   CKCON &= ~0x30;                        // Timer2 uses SYSCLK/12
   TMR2CN &= ~0x01;

   TMR2RLH = (uint8_t) TIMER2_RELOAD_HIGH;     // Init Timer2 Reload High register
   TMR2RLL = (uint8_t) TIMER2_RELOAD_LOW;      // Init Timer2 Reload Low register

   TMR2H = (uint8_t) TIMER2_RELOAD_HIGH;       // Init Timer2 High register
   TMR2L = (uint8_t) TIMER2_RELOAD_LOW;        // Init Timer2 Low register

   TMR2CN = 0x2C;                         // Low byte interrupt enabled
                                          // Timer2 enabled
                                          // and Split-Mode enabled
   IE_ET2 = 1;                               // Timer2 interrupt enabled
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This function processes the Timer2 interrupt. It checks both counters for
// matches and, upon a match, toggles the LEDs.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
   static uint16_t low_counter=0;     // Define counter variables
   static uint16_t high_counter=0;

   SFRPAGE = LEGACY_PAGE;

   if(TMR2CN_TF2L == 1)
   {
      if((low_counter++) == LED1_TOGGLE_RATE)
      {
         low_counter = 0;                 // Reset interrupt counter
         LED2 = !LED2;                    // Toggle LED2
      }
      TMR2CN_TF2L = 0;
   }
   if(TMR2CN_TF2H == 1)
   {
      if((high_counter++) == LED2_TOGGLE_RATE)
      {
         high_counter = 0;                // Reset interrupt counter
         LED1 = !LED1;                    // Toggle LED1
      }
      TMR2CN_TF2H = 0;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
