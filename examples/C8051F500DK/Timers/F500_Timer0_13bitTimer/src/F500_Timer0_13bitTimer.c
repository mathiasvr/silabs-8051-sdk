//-----------------------------------------------------------------------------
// F500_Timer0_13bitReloadTimer.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// Use Timer0 as a single 13-bit timer
//
// This program flashes the green LED on the C8051F500 target board about
// once a second using the interrupt handler for Timer0.
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
// 5) If the LED flashes, the program is working.
//
//
// Target:         C8051F50x/1x (Side A of a C8051F500-TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.3 / 11 AUG 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.2 / 02 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.1 / 11 JUN 2008 (ADT)
//    -Edited Formatting
//
// Release 1.0 / 18 APR 2008 (GP)
//    -Initial Revision


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F500_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24000000/8        // SYSCLK frequency in Hz

#define BLINK_RATE   8                 // Number of 131.072 ms intervals 
                                       // between toggling the LED pin
                                       // 8 intervals is about 1 per second

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P1, 3);                 // LED==1 means ON

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);           
void PORT_Init (void);                 
void TIMER0_Init (void);               

SI_INTERRUPT_PROTO(Timer0_ISR, TIMER0_IRQn);

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
   SFRPAGE = LEGACY_PAGE;              // Set SFRPAGE for PCA0MD


   OSCILLATOR_Init ();                 // Configure system clock
   PORT_Init ();                       // Initialize crossbar
   TIMER0_Init ();                     // Initialize Timer0

   IE_EA = 1;                          // Enable global interrupts

   while (1);                          // Spin forever
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
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the Crossbar and GPIO ports.
//
// P1.3 - digital  push-pull   LED
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P1MDOUT |= 0x08;                    // Enable LED as a push-pull output

   // Enable crossbar and weak pull-ups
   XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED |
          XBR2_XBARE__ENABLED;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// TIMER0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure Timer0 to 13-bit mode (mode 0) and generate an interrupt
// every 8192 Timer0 cycles using SYSCLK/48 as the Timer0 time base.
//
// With the system clock at 3 MHz, the Timer0 time base is 62.5 kHz.  Timer0
// will overflow every 131.072 ms
//
//-----------------------------------------------------------------------------
void TIMER0_Init(void)
{
   // No need to set SFRPAGE as all registers accessed in this function
   // are available on all pages

   TH0 = 0x00;                         // Init Timer0 reload register
   TL0 = 0x00;                         // Set the intial Timer0 value
   
   TMOD  = TMOD_T0M__MODE0;            // Timer0 in 13-bit mode
   CKCON = CKCON_SCA__SYSCLK_DIV_48;   // Timer0 uses a 1:48 prescaler
   IE_ET0 = 1;                         // Timer0 interrupt enabled
   TCON  = TCON_TR0__RUN;              // Timer0 ON
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED whenever Timer0 overflows.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(Timer0_ISR, TIMER0_IRQn)
{
   // The Timer0 interrupt flag is automatically cleared by vectoring to
   // the Timer0 ISR

   static uint8_t count = 0;
   count++;                            // Increment when an interrupt occurs

   if (count == BLINK_RATE)
   {
      LED = !LED;                      // Change state of LED
      count = 0;                       // Clear the counter
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------