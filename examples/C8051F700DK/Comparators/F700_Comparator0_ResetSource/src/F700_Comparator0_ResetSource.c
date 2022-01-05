//-----------------------------------------------------------------------------
// F700_Comparator0_ResetSource.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Silicon Laboratories MCUs can use different mechanisms to reset the device.
//
// This software shows the necessary configuration to use Comparator0 as
// a reset source.  The code executes the initialization routines and then
// spins in an infinite while() loop.  If the button on P1.1 (on the target
// board) is pressed, then the comparators positive input (CP0+) input will drop
// below the comparator's negative input (CP0-).  When this happens,
// Comparator0 will cause the device to be reset.
//
// When the code is waiting for the button to be pressed, the LED is on.
//
// Upon every device reset, the code checks the RSTSRC register to determine
// if the last reset was due to a comparator reset.  If it was a
// comparator reset, the code will blink the LED constantly.
//
// How To Test:
//
// 1) Compile and download code to a 'F700 target board
// 2) On the target board, connect P1.3 to the potentiometer R8 and P1.2 to the
//    switch labeled SW1.
// 3) Run the code and verify that the LED is on
// 5) Press the SW1 switch (this causes a comparator reset)
// 6) Verify that the LED is blinking
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J8: P1.0 - DS3
//	  - J15: VDD - power source
// 2) Use an external wire to connect the following:
//    - P1.3 - Potentiometer R8 set to mid voltage
//    - P1.2 - SW1
// 2) Connect the USB Debug Adapter to P3.
// 3) Connect the USB cable to P4.
// 4) Compile and download code to the C8051F70x development board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8. Verify that the LED is
//    On.
// 6) Press the P1.1 switch (this causes a comparator reset)
// 7) Verify that the LED is blinking.
//
//
// Target:         C8051F700
// Tool chain:     Simplicity Studio / Keil C51 9.53
// Command Line:   None
//
//
// Release 1.0
//    -Initial Revision (PD)
//    -31 JUL 2008
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include "si_toolchain.h"
#include <SI_C8051F700_Defs.h>         // SFR declarations

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK 24500000                // Clock speed in Hz
#define COMPARATOR_SETTLE  10          // Settle time in us

SI_SBIT(LED, SFR_P1, 0);                 // LED==1 means ON
SI_SBIT(SW1, SFR_P1, 2);                 // SW1==0 means switch depressed

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);               // Configure the system clock
void PORT_Init (void);                 // Configure the Crossbar and GPIO
void Comparator0_Init (void);          // Configure Comparator0
void ResetSRC_Init(void);              // Configure Reset Sources
void Timer0_Delay (unsigned int us);   // Used as a delay
void OSCILLATOR_Init (void);

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
   WDTCN = 0xDE;                       // Disable the watchdog timer
   WDTCN = 0xAD;   
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{

   SYSCLK_Init();                      // Initialize the system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   Comparator0_Init();                 // Initialize Comparator0
   OSCILLATOR_Init ();

   // When checking the RSTSRC register for the source of the last reset,
   // you must first also check that the PORSF bit is also '0'.  If the PORSF
   // bit is set ('1'), then all other bits in the RSTSRC register are invalid.

   // If last reset was due to comparator reset
   if (((RSTSRC & 0x02) == 0x00) && ((RSTSRC & 0x20)==0x20))
   {
      while (1)
      {
         Timer0_Delay (30000);         // Delay
         LED = !LED;                   // Flip LED state
      }
   }
   // Last reset was due to a power-on reset or something else
   else
   {
      ResetSRC_Init();                 // Init comparators as reset source
      LED = 1;                         // Turn LED on

      while(1){}                       // Infinite while loop waiting for
                                       // comparator reset
   }
}

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at 24.5 MHz.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   OSCICN |= 0x80;                     // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// This routine initializes the system clock to use the internal 24.5MHz / 8
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0x83;                      // Enable the precision internal osc.
   
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc. 
                                       // divided by 1 as the system clock

   SFRPAGE = SFRPAGE_save;
}                                      // running at 24.5 MHz

//-----------------------------------------------------------------------------
// PORT_Init
//
// Return Value : None
// Parameters   : None
//
//  This function configures the crossbar and GPIO ports.
//
// P1.1   digital   push-pull     Comparator - Input
// P1.2   digital   open-drain    Comparator + Input
// P1.0   digital   push-pull     LED
// P1.1   digital   open-drain    Switch

//-----------------------------------------------------------------------------

void PORT_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;
   
   SFRPAGE = CONFIG_PAGE;

   P1SKIP   = 0x06;                    // Skip P1.1 and P1.2 in
    // P1SKIP = 0x0C;                             // crossbar assignment
   XBR0     = 0x00;
   XBR1     = 0x40;                    // Enable crossbar and weak pull-ups
   P1MDIN  &= ~0x06;                   // Comparator 0 inputs (P1.1 and P1.2)
  // P1MDIN  &= ~0x0C;                        // are analog inputs.
   P1MDOUT  = 0x01;                    // LED is push-pull output

   SFRPAGE = SFRPAGE_save;
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
   uint8_t i;
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = LEGACY_PAGE;   
   CPT0CN = 0x80;                     // Comparator enabled

   SFRPAGE = 0x00;

   for (i = 0; i < 35; i++);           // Wait 10us for initialization
   CPT0MX = 0x01;                      // P1.1 = Negative Input
        //CPT0MX = 0x11;                           // P1.2 = Positive Input

   Timer0_Delay (COMPARATOR_SETTLE);   // Allow CP0 output to settle for 10 us

   SFRPAGE = SFRPAGE_save;
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
   Timer0_Delay (COMPARATOR_SETTLE);
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

   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   TCON  &= ~0x30;                     // STOP Timer0 and clear overflow flag
   TMOD  &= ~0x0f;                     // Configure Timer0 to 16-bit mode
   TMOD  |=  0x01;
   CKCON |=  0x04;                     // Timer0 counts SYSCLKs

   for (i = 0; i < us; i++)            // Count microseconds
   {
      TCON &= ~0x10;                   // STOP Timer0
      TH0 = (-SYSCLK/1000000) >> 8;    // Set Timer0 to overflow in 1us
      TL0 = -SYSCLK/1000000;
      TCON |= 0x10;                    // START Timer0
      while (TCON_TF0 == 0);           // Wait for overflow
      TCON &= ~0x20;                   // Clear overflow indicator
   }

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
