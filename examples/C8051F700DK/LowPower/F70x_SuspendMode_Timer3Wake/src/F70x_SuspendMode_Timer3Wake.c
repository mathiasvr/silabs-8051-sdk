//-----------------------------------------------------------------------------
// F70x_SuspendMode_Timer3Wake.c
//-----------------------------------------------------------------------------
// Copyright (C) 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program blinks the C8051F700 target board's LED using Timer 3 and
// suspend mode.  The device is placed in suspend mode with timer 3 running
// from an external source.  When timer 3 overflows, the device wakes up,
// toggles the LED on or off, and then puts itself back to suspend mode.
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//	  - J15: VDD - power source
//    - J8:  P1.0 - DS3
// 2) Connect the USB Debug Adapter to P3.
// 3) Connect the USB cable to P4.
// 4) Connect an external clock source to port pin 0.3.  The code is
//    currently configured for a CMOS clock running at 50kHz.
// 5) Compile and download code to the C8051F70x development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Verify that the LED is blinking.
//
// Target:         C8051F70x
// Tool chain:     Simplicity Studio / Keil C51 9.53
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (PD)
//    -10 JUL 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"             // compiler declarations
#include <SI_C8051F700_defs.h>         // SFR declarations


//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(GREEN_LED,    SFR_P1, 0);        // '0' means ON, '1' means OFF

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK             24500000L
#define EXTCLK                50000L

#define LED_TOGGLE_RATE          200   // LED toggle rate in milliseconds
                                       // if LED_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond/ 8(Ext.Osc Circuit).
#define TIMER_TICKS_PER_MS  EXTCLK/1000/8

// Note: LED_TOGGLE_RATE*TIMER_TICKS_PER_MS should not exceed 65535 (0xFFFF)
// for the 16-bit timer

#define AUX1     TIMER_TICKS_PER_MS*LED_TOGGLE_RATE
#define AUX2     -AUX1

#define TIMER3_RELOAD            AUX2  // Reload value for Timer2

#define LED_ON           0
#define LED_OFF          1

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void);
void PORT_Init (void);
void Timer3_Init (void);

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
   WDTCN = 0xDE;                       // Disable watchdog timer
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{


   PORT_Init ();                       // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator
   Timer3_Init();                      // Initialize Timer 3 to operate
                                       // as a suspend mode wake-up source

   SFRPAGE = CONFIG_PAGE;              // Page contains OSCICN, which is the
                                       // only SFR accessed in the application
                                       // loop that is not available on all
                                       // pages
   IE_EA = 1;                             // Enable global interrupts

   //----------------------------------
   // Main Application Loop
   //----------------------------------
   while (1)
   {
      GREEN_LED = !GREEN_LED;          // Toggle LED on and off

      SFRPAGE = CONFIG_PAGE;
      OSCICN |= 0x20;                  // Place MCU in suspend mode
      SFRPAGE = LEGACY_PAGE;
      // MCU remains in suspend mode until timer 3 overflows

   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal high power
// oscillator.  The function also configures the external clock FR to
// use an external CMOS oscillator, which will be used to clock Timer 3.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   OSCICN |= 0x80;                     // Enable the precision internal osc.

   OSCXCN = 0x20;
   CLKSEL = 0x00;                      // Select low power internal osc.
                                       // divided by 1 as the system clock

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P1.0   digital   push-pull     LED
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   P1MDIN |= 0x01;                     // P1.0 is digital

   P1MDOUT |= 0x01;                    // P1.0 is push-pull

   XBR1    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Timer3_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures Timer3 as a 16-bit reload timer, interrupt enabled.
// Using the SYSCLK at 20MHz/8 with a 1:12 prescaler.
//
// Note: The Timer3 uses a 1:12 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void Timer3_Init(void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;
   CKCON &= ~0x40;                     // Timer3 uses external clock
   TMR3CN &= ~0x01;

   SFRPAGE = LEGACY_PAGE;
   TMR3 = TIMER3_RELOAD;               // Init the Timer3 register
   TMR3RL = TIMER3_RELOAD;             // Reload value to be used in Timer3
   TMR3CN    = 0x05;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
