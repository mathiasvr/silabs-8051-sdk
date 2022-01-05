//-----------------------------------------------------------------------------
// F500_Comparator0_ResetSource.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Silicon Laboratories MCUs can use different mechanisms to reset the device.
// This software shows the necessary configuration to use Comparator0 as
// a reset source. The code executes the initialization routines and then
// spins in an infinite while() loop. If the button on P1.4 (on the target
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
//  How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F500-TB are placed on the following:
//    J1:  P0.0 - J2: P1.4
//    J1:  P0.1 - J2: P1.3
//    J19: P1.3 LED
//    J19: P1.4 - P1.4_SW
//    J7: Power selection, P4 9V (+5VREG) or P2 Debugger (SER_PWR)
// 2) Connect the USB Debug Adapter to P2. If +5VREG is selected on J7,
//    connect the 9V power adapter to P2.
// 3) Compile and download code to C8051F500 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
// 5) Verify that the LED is on. Then press the P1.4 switch
//    (this causes a comparator reset), verify that the LED is blinking.
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
// Release 1.1 / 10 JUN 2008 (ADT)
//    -Edited formatting
//
// Release 1.0 / 03 MAR 2008 (GP)
//    -Initial Revision


//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F500_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24000000    // Clock speed in Hz
#define COMPARATOR_SETTLE        10    // Settle time in us

SI_SBIT(LED, SFR_P1, 3);                 // LED == 1 means ON
SI_SBIT(SW1, SFR_P1, 4);                 // SW1 == 0 means switch depressed

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);           // Configure the system clock
void PORT_Init (void);                 // Configure the Crossbar and GPIO
void COMPARATOR0_Init (void);          // Configure Comparator0
void ResetSRC_Init (void);             // Configure Reset Sources

void TIMER0_Delay (uint16_t us);            // Used as a delay

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
   SFRPAGE = LEGACY_PAGE;              // Set SFR Page for PCA0MD and RSTSRC


   OSCILLATOR_Init ();                 // Initialize the system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   COMPARATOR0_Init ();                // Initialize Comparator0

   // When checking the RSTSRC register for the source of the last reset,
   // you must first also check that the PORSF bit is also '0'. If the PORSF
   // bit is set ('1'), then all other bits in the RSTSRC register are invalid.

   // If last reset was due to comparator reset
   if (((RSTSRC & RSTSRC_PORSF__BMASK) == RSTSRC_PORSF__NOT_SET) &&
       ((RSTSRC & RSTSRC_C0RSEF__BMASK) == RSTSRC_C0RSEF__SET))
   {
      while (1)
      {
         TIMER0_Delay (30000);         // Delay
         LED = ~LED;                   // Flip LED state
      }
   }
   // Last reset was due to a power-on reset or something else
   else
   {
      ResetSRC_Init ();                // Init comparators as reset source
      LED = 1;                         // Turn LED on

      while (1);                       // Infinite while loop waiting for
                                       // comparator reset
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
// This routine initializes the system clock to use the precision internal
// oscillator as its clock source.
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   // Set oscillator divide to /8
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_8;

   SFRPAGE = LEGACY_PAGE;

   RSTSRC = 0x04;                      // Enable missing clock detector

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P0.0   digital   open-drain    Comparator + Input
// P0.1   digital   open-drain    Comparator - Input
// P1.3   digital   push-pull     LED
// P1.4   digital   open-drain    Switch
//-----------------------------------------------------------------------------

void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0SKIP   = 0x03;                    // Skip P0.0 and P0.1 in
                                       // crossbar assignment

   P0MDIN  &= ~0x03;                   // Comparator 0 inputs (P0.0 and P0.1)
                                       // are analog inputs.
   P1MDOUT  = 0x08;                    // LED is push-pull output

   // Enable crossbar and weak pull-ups
   XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED |
          XBR2_XBARE__ENABLED;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// COMPARATOR0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the comparator to pins P0.0 and P0.1
//-----------------------------------------------------------------------------

void COMPARATOR0_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   CPT0CN = 0x80;                      // Comparator enabled
   CPT0MX = 0x00;                      // P0.1 = Negative Input
                                       // P0.0 = Positive Input

   TIMER0_Delay (COMPARATOR_SETTLE);   // Allow CP0 output to settle for 10 us

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// ResetSRC_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the comparator as a reset source
//-----------------------------------------------------------------------------

void ResetSRC_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   RSTSRC = 0x20;                      // Comparator 0 -> reset source

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// TIMER0_Delay
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) unsigned int us - number of microseconds of delay
//                        range is full range of integer: 0 to 65335
//
// Configure Timer0 to delay <us> microseconds before returning.
//-----------------------------------------------------------------------------

void TIMER0_Delay (uint16_t us)
{
   uint16_t i;                              // Millisecond counter

   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   TCON  &= ~0x30;                     // STOP Timer0 and clear overflow flag
   TMOD  &= ~0x0f;                     // Configure Timer0 to 16-bit mode
   TMOD  |=  0x01;
   CKCON |=  0x04;                     // Timer0 counts SYSCLKs

   for (i = 0; i < us; i++)            // Count microseconds
   {
      TCON_TR0 =0;                     // STOP Timer0
      TH0 = (-SYSCLK / 1000000) >> 8;  // Set Timer0 to overflow in 1us
      TL0 = -SYSCLK / 1000000;
      TCON |= 0x10;                    // START Timer0
      while (TCON_TF0 == 0);           // Wait for overflow
      TCON_TF0 = 0;                    // Clear overflow indicator
   }

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------