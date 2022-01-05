//-----------------------------------------------------------------------------
// F50x_Timer2_16bitReloadTimer.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program presents an example of use of the Timer2 of the C8051F50x in
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
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F500-TB are placed on the following:
//    J19: P1.3 LED
//    J7: Power selection, P4 9V (+5VREG) or P2 Debugger (SER_PWR)
// 2) Connect the USB Debug Adapter to P2. If +5VREG is selected on J7,
//    connect the 9V power adapter to P2.
// 3) Compile and download code to C8051F500 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) Check that the LED is blinking at the specified rate.
//
//
// Target:         C8051F50x/1x (Side A of the C8051F500TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.2 / 11 AUG 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.1 / 02 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0
//    -Initial Revision (ADT)
//    -18 JUN 2008


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F500_Register_Enums.h>

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable the watchdog timer
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

   // Configure internal oscillator for 24 MHz / 8
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_8;

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

   P1MDOUT = 0x08;                     // Set LED to push-pull

   // Enable crossbar and weak pull-ups
   XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED |
          XBR2_XBARE__ENABLED;

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

   // Enable Timer2 in auto-reload mode
   TMR2CN = TMR2CN_T2SPLIT__16_BIT_RELOAD |
            TMR2CN_TR2__RUN;

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
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
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