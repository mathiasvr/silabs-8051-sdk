//-----------------------------------------------------------------------------
// F560_Timer2_16bitReloadTimer.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program presents an example of use of the Timer2 of the C8051F560 in
// 16-bit reload counter/timer mode.
//
// The timer reload registers are initialized with a certain value so that
// the timer counts from that value up to FFFFh and when it overflows an
// interrupt is generated and the timer is automatically reloaded.
// In this interrupt the ISR toggles the LED.
//
// Pinout:
//
//    P1.3 -> LED
//
//    all other port pins unused
//
// How To Test:
//
// 1) Open the F560_Timer2_16bitReloadTimer.c file in the Silicon Labs IDE.
// 2) If a change in the interrupt period is required, change the value of
//    LED_TOGGLE_RATE.
// 3) Compile and download the code.
// 4) Verify the LED pins of J19 are populated.
// 5) Run the code.
// 6) Check that the LED is blinking at the specified rate.
//
//
// Target:         C8051F56x (Side A of the C8051F560TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.2 / 8  MAY 2015 (DL)
//    -Updated to use with Simplicity Studio
//
// Release 1.1 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0 / 15 JAN 2009 (GP)
//    -Initial Revision
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F550_Defs.h>
#include <SI_C8051F550_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24000000/8  // SYSCLK in Hz (20 MHz internal
                                       // oscillator / 8)

#define LED_TOGGLE_RATE           100  // LED toggle rate in milliseconds
                                       // if LED_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
// An interrupt occurs whenever Timer 2 overflows from 0xFFFF to 0x0000
// so we want the reload value to be 1 ms in timer ticks before 0xFFFF.
#define TIMER_PRESCALER            12  // Based on Timer2 CKCON and TMR2CN
                                       // settings

// Reload value for Timer2
#define TIMER2_RELOAD  (uint16_t)(0xFFFF - (SYSCLK/TIMER_PRESCALER/1000))


SI_SBIT(LED, SFR_P1, 3);                 // LED==0 means ON

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);           // Oscillator initialization
void Port_Init (void);                 // Port initialization routine
void Timer2_Init (void);               // Timer2 initialization routine
SI_INTERRUPT_PROTO(TIMER2_ISR, INTERRUPT_TIMER2);

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
   PCA0MD &= ~0x40;                    // Clear watchdog timer enable
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SFRPAGE = LEGACY_PAGE;

   OSCILLATOR_Init ();                 // Initialize Oscillator
   Timer2_Init ();                     // Initialize the Timer2
   Port_Init ();                       // Initialize Ports
   IE_EA = 1;                          // Enable global interrupts

   while (1);                          // Loop forever
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the internal oscillator to maximum internal frequency / 8, which
// is 3 MHz
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0xC4;

   SFRPAGE = SFRPAGE_save;
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
// P1.3  digital  push-pull  LED
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   XBR2 = 0x40;                        // Enable crossbar
   P1MDOUT = 0x08;                     // Set LED to push-pull

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures Timer2 as a 16-bit reload timer, interrupt enabled.
// Using the SYSCLK at 24MHz/8 with a 1:12 prescaler.
//
// Note: The Timer2 uses a 1:12 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void Timer2_Init(void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   CKCON &= ~0x30;                     // Timer2 uses SYSCLK/12
   TMR2CN &= ~0x01;

   TMR2RL = TIMER2_RELOAD;             // Reload value to be used in Timer2
   TMR2 = TMR2RL;                      // Init the Timer2 register

   TMR2CN = 0x04;                      // Enable Timer2 in auto-reload mode
   IE_ET2 = 1;                         // Timer2 interrupt enabled

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer2 interrupt and toggle the LED
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   static uint16_t i = 0;

   i++;
   if (i == LED_TOGGLE_RATE)
   {
      LED = !LED;                         // Toggle the LED
      i = 0;
   }

   TMR2CN_TF2H = 0;                       // Reset Interrupt

}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
