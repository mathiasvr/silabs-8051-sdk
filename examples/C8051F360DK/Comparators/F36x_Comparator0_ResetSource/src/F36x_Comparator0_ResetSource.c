//-----------------------------------------------------------------------------
// F36x_Comparator0_ResetSource.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Silicon Laboratories MCUs can use different mechanisms to reset the device.
//
// This software shows the necessary configuration to use Comparator0 as
// a reset source. The code executes the initialization routines and then
// spins in an infinite while() loop. If the button SW3.0 (on the target
// board) is pressed, then the comparators postive input (CP0+) input will drop
// below the comparator's negative input (CP0-). When this happens,
// Comparator0 will cause the device to be reset.
//
// When the code is waiting for the button to be pressed, the LED is on.
//
// Upon every device reset, the code checks the RSTSRC register to determine
// if the last reset was due to a comparator reset. If it was a
// comparator reset, the code will blink the LED constantly.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: P3.0 - SW3.0
//    J12: P3.2 - P3.2_LED
//    J3 : P1.4 - J5: P3.0
//    J3 : P1.5 - J5: P3.2
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
// 5) Verify that the LED is on. Press the P3.0 switch (this causes a
//    comparator reset), and then verify that the LED is blinking.
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
//    -Initial Revision (TP)
//    -13 OCT 2006


//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK               24500000  // Clock speed in Hz
#define COMPARATOR_SETTLE    10        // Settle time in us

SI_SBIT(LED, SFR_P3, 2);                  // Green LED

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);           // Configure the system clock
void Port_Init (void);                 // Configure the Crossbar and GPIO
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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{

   Oscillator_Init();                  // Initialize the system clock
   Port_Init ();                       // Initialize crossbar and GPIO
   Comparator0_Init();                 // Initialize Comparator0

   SFRPAGE = LEGACY_PAGE;

   // When checking the RSTSRC register for the source of the last reset,
   // you must first also check that the PORSF bit is also '0'.  If the PORSF
   // bit is set ('1'), then all other bits in the RSTSRC register are invalid.

   // If last reset was due to comparator reset
   if (((RSTSRC & RSTSRC_PORSF__BMASK) == RSTSRC_PORSF__NOT_SET) &&
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

      while(1){}                       // Infinite while loop waiting for
                                       // comparator reset
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Oscillator_Init
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the precision internal
// oscillator as its clock source.
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   OSCICN |= OSCICN_IFCN__HFOSC_DIV_1; // SYSCLK is internal osc.
                                       // running at 24.5 MHz

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Port_Init
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// Pinout:
//
// P1.4   analog    push-pull     Comparator + Input
// P1.5   analog    open-drain    Comparator - Input
//
// P3.0   digital   open-drain    Switch
//
// P3.2   digital   push-pull     LED
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   P1SKIP  = 0x30;                     // Skip P1.4 and P1.5 in
                                       // crossbar assignment
   XBR1    = XBR1_XBARE__ENABLED;      // Enable crossbar and weak pull-ups

   P1MDIN  &= ~0x30;                   // Comparator 0 inputs (P1.4 and P1.5)
                                       // are analog inputs.
   P3MDOUT  = 0x04;                    // LED is push-pull output

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Comparator0_Init
//
// Return Value : None
// Parameters   : None
//
// This function configures the comparator to pins P1.4 and P1.5
//-----------------------------------------------------------------------------
void Comparator0_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   CPT0CN = 0x80;                      // Comparator enabled
   CPT0MX = 0x00;                      // P1.5 = Negative Input
                                       // P1.4 = Positive Input

   Timer0_Delay (COMPARATOR_SETTLE);   // Allow CP0 output to settle for 10 us

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   RSTSRC = 0x20;                      // Comparator 0 -> reset source

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
   TMOD  |=  TMOD_T0M__MODE1;
   CKCON |=  0x04;                     // Timer0 counts SYSCLKs

   for (i = 0; i < us; i++)            // Count microseconds
   {
      TCON &= ~TCON_TR0__BMASK;        // STOP Timer0
      TH0 = (-SYSCLK/1000000) >> 8;    // Set Timer0 to overflow in 1us
      TL0 = -SYSCLK/1000000;
      TCON |= TCON_TR0__RUN;           // START Timer0
      while (TCON_TF0 == 0);           // Wait for overflow
      TCON &= ~TCON_TF0__BMASK;        // Clear overflow indicator
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------