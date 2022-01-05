//-----------------------------------------------------------------------------
// F35x_Comparator0_ResetSource.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// This software shows the necessary configuration to use Comparator0 as
// a reset source. The code executes the initialization routines and then
// spins in an infinite while() loop. If the button on P1.0 (on the target
// board) is pressed, then the comparators postive input (CP0+) input will drop
// below the comparator's negative input (CP0-). When this happens,
// Comparator0 will cause the device to be reset.
//
// When the code is waiting for the button to be pressed, the LED is on.
//
// Upon every device reset, the code checks the RSTSRC register to determine
// if the last reset was due to a comparator reset.  If it was a
// comparator reset, the code will blink the LED constantly.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are installed on the following:
//    - J1: P0.0 - P1.0
//    - J1: P0.1 - P0.7
//    - J3: P0.7 - P0.7_LED
//    - J3: P1.0 - P1.0_SW
//    - J8: Power selection, P1 9V or J4 Debugger
// 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1. After power the board, the red LED D3
//    is on.
// 3) Compile and download code to the C8051F350-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) The green LED D1 is on before pressing button. After press the P1.0
//    switch (this causes a comparator reset), the LED D1 is blinking.
//
//
// Target:         C8051F35x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1
//    - Updated to use with Simplicity Studio (JL)
//    - 23 JUN 2014
//
// Release 1.0
//    -Initial Revision (GP)
//    -17 MAY 2006
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F350_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000    // Clock speed in Hz
#define COMPARATOR_SETTLE  10          // Settle time in us

SI_SBIT(LED, SFR_P0, 7);                 // Green Led

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);           // Configure the system clock
void PORT_Init (void);                 // Configure the Crossbar and GPIO
void Comparator0_Init (void);          // Configure Comparator0
void ResetSRC_Init(void);              // Configure Reset Sources
void Timer0_Delay (uint16_t us);            // Used as a delay

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
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{

   OSCILLATOR_Init();                  // Initialize the system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   Comparator0_Init();                 // Initialize Comparator0

   // When checking the RSTSRC register for the source of the last reset,
   // you must first also check that the PORSF bit is also '0'.  If the PORSF
   // bit is set ('1'), then all other bits in the RSTSRC register are invalid.

   // If last reset was due to comparator reset
   if (((RSTSRC & RSTSRC_PORSF__BMASK)  == RSTSRC_PORSF__NOT_SET) &&
       ((RSTSRC & RSTSRC_C0RSEF__BMASK) == RSTSRC_C0RSEF__SET))
   {
      while (1)
      {
         Timer0_Delay (30000);         // Delay
         LED = ~LED;                   // Flip LED state
      }
   }
   // Last reset was due to a power-on reset or something else
   else
   {
      ResetSRC_Init();                 // Init comparators as reset source
      LED = 1;                         // Turn LED on

      // Infinite while loop waiting for comparator reset
      while(1){}
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the precision internal
// oscillator as its clock source.
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void)
{
   // SYSCLK is internal oscillator running at 24.5 MHz
   OSCICN |= OSCICN_IFCN__HFOSC_DIV_1;
}

//-----------------------------------------------------------------------------
// PORT_Init
//
// Return Value : None
// Parameters   : None
//
//  This function configures the crossbar and GPIO ports.
//
// P0.0   digital   push-pull     Comparator + Input
// P0.1   digital   open-drain    Comparator - Input
// P0.7   digital   push-pull     LED
// P1.0   digital   open-drain    Switch
//-----------------------------------------------------------------------------

void PORT_Init (void)
{
   // Skip P0.0 and P0.1 in crossbar assignment
   P0SKIP   = P0SKIP_B0__SKIPPED | P0SKIP_B1__SKIPPED;
   // Enable crossbar and weak pull-ups
   XBR1     = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED;

   P0MDIN  &= ~0x03;                   // Comparator 0 inputs (P0.0 and P0.1)
                                       // are analog inputs.
   P0MDOUT  = P0MDOUT_B7__PUSH_PULL;   // LED is push-pull output
}

//-----------------------------------------------------------------------------
// Comparator0_Init
//
// Return Value : None
// Parameters   : None
//
//  This function configures the comparator to pins P0.0 and P0.1
//-----------------------------------------------------------------------------

void Comparator0_Init (void)
{
   CPT0CN = CPT0CN_CPEN__ENABLED;      // Comparator enabled
   CPT0MX = 0x00;                      // P0.1 = Negative Input
                                       // P0.0 = Positive Input

   Timer0_Delay (COMPARATOR_SETTLE);   // Allow CP0 output to settle for 10 us
}

//-----------------------------------------------------------------------------
// ResetSRC_Init
//
// Return Value : None
// Parameters   : None
//
// This function configures the comparator as a reset source
//-----------------------------------------------------------------------------

void ResetSRC_Init (void)
{
   RSTSRC = 0x20;                      // Comparator 0 -> reset source
}

//-----------------------------------------------------------------------------
// Timer0_Delay
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) unsigned int us - number of microseconds of delay
//                        range is full range of integer: 0 to 65335
//
// Configure Timer0 to delay <us> microseconds before returning.
//-----------------------------------------------------------------------------

void Timer0_Delay (unsigned int us)
{
   unsigned i;                         // Millisecond counter

   // STOP Timer0 and clear overflow flag
   TCON  &= ~(TCON_TF0__BMASK | TCON_TR0__BMASK);
   TMOD  &= ~(TMOD_GATE0__BMASK | TMOD_CT0__BMASK | TMOD_T0M__FMASK);
   // Configure Timer0 to 16-bit mode
   TMOD  |=  TMOD_CT0__TIMER | TMOD_T0M__MODE1;
   CKCON |=  CKCON_T0M__SYSCLK;        // Timer0 counts SYSCLKs

   for (i = 0; i < us; i++)            // Count microseconds
   {
      TCON &= ~TCON_TR0__RUN;          // STOP Timer0
      TH0 = (-SYSCLK/1000000) >> 8;    // Set Timer0 to overflow in 1us
      TL0 = -SYSCLK/1000000;
      TCON |= TCON_TR0__RUN;           // START Timer0
      while (TCON_TF0 == 0);           // Wait for overflow
      TCON &= ~TCON_TF0__SET;          // Clear overflow indicator
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------