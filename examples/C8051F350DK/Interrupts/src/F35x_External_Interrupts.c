//-----------------------------------------------------------------------------
// F35x_External_Interrupts.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// Interrupt driven IO using Buttons and LEDs
//
// This software shows the necessary configuration to use External Interrupt 0
// (/INT0) or External Interrupt 1 (/INT1) as an interrupt source. The code
// executes the initialization routines and then spins in an infinite while()
// loop. If the button on P1.0 (on the target board) is pressed, then the
// edge-triggered /INT0 input on P0.0 will cause an interrupt and toggle the
// LED.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are installed on the following:
//    - J8 : Power selection, P1 9V or J4 Debugger
//    - J3 : P0.6 - P0.6_LED
//    - J3 : P0.7 - P0.7_LED
//    - J3 : P1.0_SW - J1 : P0.0 (/INT0) or
//    - J3 : P1.0_SW - J1 : P0.0 (/INT1)
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
// 5) Press the switche S2. The green LED D1 or D2 should toggle.
//
//
// Target:         C8051F35x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1 (JL)
//    - Updated descriptions
//    - 24 JUN 2014
//
// Release 1.0
//    -Initial Revision (SM)
//    -19 JUN 2007
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F350_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000    // Clock speed in Hz

SI_SBIT(LED1, SFR_P0, 6);                // LED='1' means ON
SI_SBIT(LED2, SFR_P0, 7);                // LED='1' means ON
SI_SBIT(SW1,  SFR_P1, 0);                // Push-button switch on board

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);           // Configure the system clock
void Port_Init (void);                 // Configure the Crossbar and GPIO
void Ext_Interrupt_Init (void);        // Configure External Interrupts (/INT0
                                       // and /INT1)

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable Watchdog timer
}
 
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
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the precision internal
// oscillator as its clock source.
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   // Set clock to 24.5 MHz
   OSCICN  = OSCICN_IOSCEN__ENABLED | OSCICN_IFCN__HFOSC_DIV_1;
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
// P0.6 - digital   push-pull   LED1
// P0.7 - digital   push-pull   LED2
//
// P1.0 - digital   open-drain  SW1 (Switch 1)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // Enable crossbar and weak pull-ups
   XBR1    = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED;

   P0MDOUT = 0xC0;                    // LED1 and LED2 are push-pull outputs
}

//-----------------------------------------------------------------------------
// Ext_Interrupt_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures and enables /INT0 and /INT1 (External Interrupts)
// as negative edge-triggered.
//
//-----------------------------------------------------------------------------
void Ext_Interrupt_Init (void)
{
   // /INT 0 and /INT 1 are edge triggered
   TCON = TCON_IT1__EDGE | TCON_IT0__EDGE;

   // /INT0 active low; /INT0 on P0.0;
   // /INT1 active low; /INT1 on P0.1
   IT01CF = IT01CF_IN1PL__ACTIVE_LOW |
            IT01CF_IN1SL__P0_1       |
            IT01CF_IN0PL__ACTIVE_LOW |
            IT01CF_IN0SL__P0_0;


   IE_EX0 = 1;                         // Enable /INT0 interrupts
   IE_EX1 = 1;                         // Enable /INT1 interrupts
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// /INT0 ISR
//-----------------------------------------------------------------------------
//
// Whenever a negative edge appears on P0.0, LED1 is toggled.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(INT0_ISR, INT0_IRQn)
{
   LED1 = !LED1;
}

//-----------------------------------------------------------------------------
// /INT1 ISR
//-----------------------------------------------------------------------------
//
// Whenever a negative edge appears on P0.1, LED2 is toggled.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(INT1_ISR, INT1_IRQn)
{
   LED2 = !LED2;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------