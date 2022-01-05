//-----------------------------------------------------------------------------
// F70x_External_Interrupts.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This software shows the necessary configuration to use External Interrupts 0
// and 1 as interrupt sources.  The code executes the initialization 
// routines and then spins in an infinite while() loop.  
// If the button on P1.1 (on the target board) is pressed, then the
// edge-triggered /INT0 input on P0.0 and P0.1 will cause an interrupt and toggle
// the LED.
//
// Pinout:
//
// P0.0 - /INT0
// P0.1 - /INT1
// P1.0 - LED
// P1.1 - SW (Switch)
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J8: P1.0 - DS3
//	  - J15: VDD - power source
// 2) Use an external wire to connect SW1 to P0.0 for /INT0 or P0.1 for /INT1.
// 3) Connect the USB Debug Adapter to P3. Connect a USB cable to P4.
// 4) Compile and download code to the C8051F70x development board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Press the switch. Every time the switch is pressed, the P1.0 LED will toggle.
//
// Target:         C8051F70x
// Tool chain:     Simplicity Studio / Keil C51 9.53
// Command Line:   None
//
//
// Release 1.0
//    -Initial Revision (PD)
//    -07 JUL 2008
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include "si_toolchain.h"             // compiler declarations
#include <SI_C8051F700_defs.h>         // SFR declarations

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000    // Clock speed in Hz

SI_SBIT(LED,    SFR_P1, 0);              // '0' means ON, '1' means OFF
SI_SBIT(SW,     SFR_P1, 1);              // '0' means ON, '1' means OFF

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

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
   WDTCN = 0xDE;                       // Disable Watchdog timer
   WDTCN = 0xAD;
}
 
void Oscillator_Init (void);           // Configure the system clock
void Port_Init (void);                 // Configure the Crossbar and GPIO
void Ext_Interrupt_Init (void);        // Configure External Interrupts (/INT0
                                       // and /INT1)
SI_INTERRUPT_PROTO(Ext_Interrup0t_ISR, INTERRUPT_INT0);
SI_INTERRUPT_PROTO(Ext_Interrupt1_ISR, INTERRUPT_INT1);
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{

   Oscillator_Init();                  // Initialize the system clock
   Port_Init ();                       // Initialize crossbar and GPIO
   Ext_Interrupt_Init();               // Initialize External Interrupts

   IE_EA = 1;

   while(1);                           // Infinite while loop waiting for
                                       // an interrupt from /INT0 or /INT1
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

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
// This function initializes the system clock to use the internal
// oscillator.
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
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// Pinout:
//
// P0.0 - digital   open-drain  /INT0
// P0.1 - digital   open-drain  /INT1
//
// P1.0 - digital   open-drain  SW
// P1.1 - digital   push-pull   LED
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   XBR1     = 0x40;                    // Enable crossbar and weak pullups

   P1MDOUT  = 0x01;                    // LED1 and LED2 are push-pull outputs

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Ext_Interrupt_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures and enables /INT0 (External Interrupt)
// as negative edge-triggered.
//
//-----------------------------------------------------------------------------
void Ext_Interrupt_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   TCON = 0x05;                        // /INT 0, /INT1 are edge triggered

   IT01CF = 0x10;                      // /INT0 active low; /INT0 on P0.0;
                                       // /INT1 active low; /INT1 on P0.1

   IE_EX0 = 1;                         // Enable /INT0 interrupts
   IE_EX1 = 1;
   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// /Ext_Interrupt0_ISR
//-----------------------------------------------------------------------------
//
// Whenever a negative edge appears on P0.0, LED is toggled.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Ext_Interrupt0_ISR, INT0_IRQn)
{
   LED = !LED;
}

//-----------------------------------------------------------------------------
// /Ext_Interrupt1_ISR
//-----------------------------------------------------------------------------
//
// Whenever a negative edge appears on P0.1, LED is toggled.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Ext_Interrupt1_ISR, INT1_IRQn)
{
   LED = !LED;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
