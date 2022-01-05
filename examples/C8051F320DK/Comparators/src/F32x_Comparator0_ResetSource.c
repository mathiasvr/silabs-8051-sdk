//-----------------------------------------------------------------------------
// F32x_Comparator0_ResetSource.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Silicon Laboratories MCUs can use different mechanisms to reset the device.
//
// This software shows the necessary configuration to use Comparator0 as
// a reset source.  The code executes the initialization routines and then
// spins in an infinite while() loop.  If the button on P2.0 (on the target
// board) is pressed, then the comparators postive input (CP0+) input will drop
// below the comparator's negative input (CP0-).  When this happens,
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
//    J1: P1.0 - P2.0
//    J1: P1.1 - P2.2
//    J3: P2.0 - P2.0_SW
//    J3: P2.2 - P2.2_LED
// 2) If power the C8051F320-TB from the power adapter, please connect the 9V
//    power adapter to P1, ensure J8 Pin2 is connected to Pin3, and J2 is
//    installed.
//    If power the target board from USB, please connect a USB cable to PC, and
//    ensure J11 is installed.
//    After power the board, the red PWR LED D3 is on.
// 3) Connect the USB Debug Adapter to J4.
// 4) Compile and download code to the C8051F320-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 5) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 6) Verify that the LED is on. And then press the P2.0 switch
//    (this causes a comparator reset), verify that the LED is blinking.
//
//
// Target:         C8051F320/1
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1 (JL)
//    - Updated descriptions
//    - 28 SEP 2014
//
// Release 1.0
//    -Initial Revision (GP)
//    -17 MAY 2006


//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F320_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             12000000    // Clock speed in Hz
#define COMPARATOR_SETTLE  10          // Settle time in us

SI_SBIT(LED,    SFR_P2, 2);              // Green Led
SI_SBIT(SWITCH, SFR_P2, 0);              // Push-button switch on board

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
   PCA0MD &= ~PCA0MD_WDTE__BMASK;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   // Disable Watchdog timer

   OSCILLATOR_Init();                  // Initialize the system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   Comparator0_Init();                 // Initialize Comparator0

   // When checking the RSTSRC register for the source of the last reset,
   // you must first also check that the PORSF bit is also '0'.  If the PORSF
   // bit is set ('1'), then all other bits in the RSTSRC register are invalid.

   // If last reset was due to comparator reset
   if (((RSTSRC & RSTSRC_PORSF__BMASK)  == RSTSRC_PORSF__NOT_SET ) &&
       ((RSTSRC & RSTSRC_C0RSEF__BMASK) == RSTSRC_C0RSEF__SET ))
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

      while(1){}                       // Infinite while loop waiting for
                                       // comparator reset
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
   OSCICN |= OSCICN_IFCN__HFOSC_DIV_1; // SYSCLK is internal osc.
}                                      // running at 12 MHz

//-----------------------------------------------------------------------------
// PORT_Init
//
// Return Value : None
// Parameters   : None
//
//  This function configures the crossbar and GPIO ports.
//
// P1.0   digital   push-pull     Comparator + Input
// P1.1   digital   open-drain    Comparator - Input
// P2.0   digital   open-drain    Switch
// P2.2   digital   push-pull     LED
//-----------------------------------------------------------------------------

void PORT_Init (void)
{
   P1SKIP   = 0x03;                    // Skip P1.0 and P1.1 in
                                       // crossbar assignment

   // Enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED;

   P1MDIN  &= ~0x03;                   // Comparator 0 inputs (P1.0 and P1.1)
                                       // are analog inputs.
   P2MDOUT  = 0x04;                    // LED is push-pull output
}

//-----------------------------------------------------------------------------
// Comparator0_Init
//
// Return Value : None
// Parameters   : None
//
//  This function configures the comparator to pins P1.0 and P1.1
//-----------------------------------------------------------------------------

void Comparator0_Init (void)
{
   CPT0CN = CPT0CN_CPEN__ENABLED;      // Comparator enabled
   CPT0MX = 0x00;                      // P1.1 = Negative Input
                                       // P1.0 = Positive Input

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
   RSTSRC = RSTSRC_C0RSEF__SET;        // Comparator 0 -> reset source
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

void Timer0_Delay (uint16_t us)
{
   uint16_t i;                              // Millisecond counter

   TCON  &= ~0x30;                     // STOP Timer0 and clear overflow flag
   TMOD  &= ~0x0f;                     // Configure Timer0 to 16-bit mode
   TMOD  |=  0x01;
   CKCON |=  0x04;                     // Timer0 counts SYSCLKs

   for (i = 0; i < us; i++)            // Count microseconds
   {
      TCON &= ~0x10;                   // STOP Timer0
      TH0 = (-SYSCLK / 1000000 ) >> 8; // Set Timer0 to overflow in 1us
      TL0 = -SYSCLK / 1000000;
      TCON |= 0x10;                    // START Timer0
      while (TCON_TF0 == 0);           // Wait for overflow
      TCON &= ~0x20;                   // Clear overflow indicator
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------