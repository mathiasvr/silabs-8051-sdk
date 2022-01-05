//-----------------------------------------------------------------------------
// F36x_Oscillator_CMOS.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This example demonstrates how to initialize for and switch to an external
// CMOS Oscillator.
// - Pinout:
//    P0.0 -> /SYSCLK
//
//    P0.6 -> XTAL2 - External CMOS Oscillator input
//
//    P3.2 -> LED
//
//    all other port pins unused
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: P3.2 - P3.2_LED
//    J1: Power selection, P2 9V, P4 USB or J9 Debugger
// 2) Connect the USB Debug Adapter to J9. If 9V power is selected on J1,
//    connect the 9V power adapter to P2. If VBUS is selected on J1, connect
//    USB cable to P4.
// 3) Define the input CMOS clock frequency using <CMOS_clock>
// 4) Connect the CMOS input clock to XTAL2 / P0.6 (or J11 on the TB)
// 5) Compile and download code to C8051F360 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
//      - the test will blink an LED at a frequency based on the CMOS
//         Oscillator
//      - the 'F36x will also output the SYSCLK to a port pin (P0.0) for
//         observation
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
//    -27 NOV 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define CMOS_clock 3062500             // CMOS oscillator frequency

// Timer2 using SYSCLK/12 as its time base
// Timer2 counts 65536 SYSCLKs per Timer2 interrupt
// LED target flash rate = 10 Hz (may be slower, depending on CMOS_clock)
//
// If CMOS_clock is too slow to divide into a number of counts,
// <count> will always remain 0. In this case, the LED Flash rate will be
// slower than 10 Hz.
#define LED_interrupt_count CMOS_clock/12/65536/10

SI_SBIT(LED, SFR_P3, 2);                  // LED='1' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void ExtCMOSOsc_Init (void);
void Port_Init (void);
void Timer2_Init (void);

SI_INTERRUPT_PROTO(Timer2_ISR, TIMER2_IRQn);

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
   PCA0MD  &= ~PCA0MD_WDTE__ENABLED;   // Disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, switches to the external CMOS
// oscillator, and loops forever, blinking the LED.
//
//-----------------------------------------------------------------------------
void main (void)
{

   SYSCLK_Init ();                     // Initialize system clock to 24.5MHz

   ExtCMOSOsc_Init ();                 // Initialize for and switch to the
                                       // external CMOS oscillator

   Port_Init ();                       // Initialize crossbar and GPIO

   Timer2_Init ();                     // Init Timer2 to generate
                                       // interrupts for the LED.

   IE_EA = 1;                          // Enable global interrupts

   SFRPAGE = LEGACY_PAGE;

   while (1) {                         // Spin forever
   }
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
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // Configure internal oscillator for its highest frequency (24.5 MHz)
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;

   RSTSRC = 0x04;                      // Enable missing clock detector

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
//-----------------------------------------------------------------------------
void ExtCMOSOsc_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   P0MDIN |= 0x40;                     // In CMOS clock mode, the associated
                                       // pin should be configured as a
                                       // digital input.

   OSCXCN = OSCXCN_XOSCMD__CMOS;       // External Oscillator is a CMOS clock
                                       // (no divide by 2 stage)
                                       // XFCN bit settings do not apply to a
                                       // CMOS clock

   CLKSEL = CLKSEL_CLKSL__EXTOSC;      // Switch to the external CMOS clock

   OSCICN = OSCICN_IOSCEN__DISABLED;   // Disable the internal oscillator.

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
// Pinout:
//
// P0.0   digital   push-pull     /SYSCLK
//
// P0.6   digital   open-drain    XTAL2 - External CMOS Oscillator input
//
// P3.2   digital   push-pull     LED
//
// all other port pins unused
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   P0MDOUT |= 0x01;                    // Enable /SYSCLK as a push-pull output

   P3MDOUT |= 0x04;                    // Enable LED as a push-pull output

   P0SKIP |= 0x40;                     // Skip the XTAL2 pin
   P3SKIP |= 0x04;                     // Skip the LED pin

   XBR0 = XBR0_SYSCKE__ENABLED;        // Route /SYSCLK to a port pin
   XBR1 = XBR1_XBARE__ENABLED;         // Enable crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase

   CKCON &= ~0x30;                     // Timer2 clocked based on T2XCLK;

   TMR2RL = 0x0000;                    // Init reload value
   TMR2 = 0xffff;                      // Set to reload immediately

   IE_ET2 = 1;                         // Enable Timer2 interrupts

   TMR2CN_TR2 = 1;                     // Start Timer2

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
//-----------------------------------------------------------------------------
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