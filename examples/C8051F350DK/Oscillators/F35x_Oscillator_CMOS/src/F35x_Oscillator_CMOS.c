//-----------------------------------------------------------------------------
// F35x_Oscillator_CMOS.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This example demonstrates how to initialize for and switch to an external
// CMOS Oscillator.
//
//
// How To Test:
// ------------
// 1) Connect the CMOS input clock to XTAL2 / P0.3 (or J10 on the TB), and
//    specify the desired frequency by changing the constant <CMOS_clock>
// 2) Ensure shorting blocks are installed on the following:
//    - J3: P0.6 - P0.6_LED
//    - J8: Power selection, P1 9V or J4 Debugger
// 3) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1. After power the board, the red LED D3
//    is on.
// 4) Compile and download code to the C8051F350-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 5) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 6) Measure the frequency output on P0.0.
//    This test will blink an LED at a frequency based on the CMOS Oscillator.
//
//
// Target:         C8051F35x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1
//    - Updated to use with Simplicity Studio (JL)
//    - 03 JUN 2014
//
// Release 1.0
//    -Initial Revision (TP)
//    -31 MAY 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F350_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define CMOS_clock 20000000             // CMOS oscillator frequency

// Timer2 using SYSCLK/12 as its time base
// Timer2 counts 65536 SYSCLKs per Timer2 interrupt
// LED target flash rate = 10 Hz (may be slower, depending on CMOS_clock)
//
// If CMOS_clock is too slow to divide into a number of counts,
// <count> will always remain 0. In this case, the LED Flash rate will be
// slower than 10 Hz.
#define LED_interrupt_count CMOS_clock/12/65536/10

SI_SBIT(LED, SFR_P0, 6);                 // LED='1' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void ExtCMOSOsc_Init (void);
void Port_Init (void);
void Timer2_Init (void);

void Timer2_ISR (void);

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, switches to the external CMOS
// oscillator, and loops forever, blinking the LED.
//
void main (void) {

   // Disable watchdog timer

   SYSCLK_Init ();                     // Initialize system clock to 24.5MHz

   ExtCMOSOsc_Init ();                 // Initialize for and switch to the
                                       // external CMOS oscillator

   Port_Init ();                       // Initialize crossbar and GPIO

   Timer2_Init ();                     // Init Timer2 to generate
                                       // interrupts for the LED.

   IE_EA = 1;                          // Enable global interrupts

   while (1) {}                        // Spin forever
}

//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSCLK_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator as its clock source.  Also enables missing clock detector reset.
//
void SYSCLK_Init (void)
{
   // Configure internal oscillator for its highest frequency (24.5 MHz)
   OSCICN = OSCICN_IOSCEN__ENABLED | OSCICN_IFCN__HFOSC_DIV_1;

   RSTSRC = 0x04;                      // Enable missing clock detector
}

//-----------------------------------------------------------------------------
// ExtCMOSOsc_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes for and switches to the External CMOS Oscillator.
//
// Note: In RC, capacitor, or CMOS clock configuration, the clock source
// should be wired to the XTAL2 pin as shown in Option 2, 3, or 4 in the
// Oscillators section of the datasheet.
//
void ExtCMOSOsc_Init (void)
{
   P0MDIN |= 0x08;                     // In CMOS clock mode, the associated
                                       // pin should be configured as a
                                       // digital input.

   OSCXCN = OSCXCN_XOSCMD__CMOS;       // External Oscillator is a CMOS clock
                                       // (no divide by 2 stage)
                                       // XFCN bit settings do not apply to a
                                       // CMOS clock

   CLKSEL = CLKSEL_CLKSL__EXTOSC;      // Switch to the external CMOS clock
}

//-----------------------------------------------------------------------------
// Port_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports.
//
// P0.0   digital   push-pull     /SYSCLK
// P0.3   digital   open-drain    XTAL2 - External CMOS Oscillator input
// P0.6   digital   push-pull     LED
//
void PORT_Init (void)
{
   P0MDOUT |= 0x01;                    // Enable /SYSCLK as a push-pull output

   P0MDOUT |= 0x40;                    // Enable LED as a push-pull output

   // Crossbar Initialization
   XBR0 = XBR0_SYSCKE__ENABLED;        // Route /SYSCLK to first available pin
   // Enable Crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED |
          XBR1_XBARE__ENABLED;
}

//-----------------------------------------------------------------------------
// Timer2_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt after the
// maximum possible time (TMR2RL = 0x0000).
//
void Timer2_Init (void)
{
   // Stop Timer2; Clear TF2, use SYSCLK/12 as timebase
   TMR2CN = TMR2CN_TF2H__NOT_SET |
            TMR2CN_TF2L__NOT_SET |
            TMR2CN_TF2LEN__DISABLED |
            TMR2CN_T2SPLIT__16_BIT_RELOAD |
            TMR2CN_TR2__STOP |
            TMR2CN_T2XCLK__SYSCLK_DIV_12;

   CKCON &= ~(CKCON_T2MH__BMASK |
              CKCON_T2ML__BMASK);      // Timer2 clocked based on T2XCLK;

   TMR2RL = 0x0000;                    // Init reload value
   TMR2 = 0xffff;                      // Set to reload immediately

   IE_ET2 = 1;                         // Enable Timer2 interrupts

   TMR2CN_TR2 = 1;                     // Start Timer2
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED whenever Timer2 overflows.
//
SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   static uint16_t count = 0;

   TMR2CN_TF2H = 0;                    // Clear Timer2 interrupt flag

   if (count == LED_interrupt_count)
   {
      LED = ~LED;                      // Change state of LED

      count = 0;
   }
   else
   {
      count++;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------