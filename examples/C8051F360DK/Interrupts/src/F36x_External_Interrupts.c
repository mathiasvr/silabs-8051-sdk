//-----------------------------------------------------------------------------
// F36x_External_Interrupts.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Interrupt driven IO using Buttons and LEDs
//
// This software shows the necessary configuration to use External Interrupt 0
// (/INT0) or External Interrupt 1 (/INT1) as an interrupt source.  The code
// executes the initialization routines and then spins in an infinite while()
// loop. If the button P3.0 (on the target board) is pressed, then the
// edge-triggered /INT0 input on P0.0 will cause an interrupt and toggle the
// LED.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: SW3.0 - J2: P0.0 (/INT0)
//    J12: SW3.1 - J2: P0.1 (/INT1)
//    J12: P3.2 - P3.2_LED
//    J12: P3.3 - P3.3_LED
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
// 5) Press the switches. Every time a switch is pressed, the P2.2 or P2.3
//    LED should toggle.
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
//    -Initial Revision (SM)
//    -20 JUN 2007


//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             12000000    // Clock speed in Hz

SI_SBIT(SW1,  SFR_P0, 0);                 // Push-button switch on board
SI_SBIT(SW2,  SFR_P0, 1);                 // Push-button switch on board
SI_SBIT(LED1, SFR_P3, 2);                 // Green LED
SI_SBIT(LED2, SFR_P3, 3);                 // Green LED

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
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   OSCICN |= OSCICN_IFCN__HFOSC_DIV_1; // Initialize internal oscillator to
                                       // highest frequency

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
// P0.0 - digital   open-drain  /INT0 (Switch 1)
// P0.1 - digital   open-drain  /INT1 (Switch 2)
//
// P3.2 - digital   push-pull   LED1
// P3.3 - digital   push-pull   LED2
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // LED Pins
   P3MDOUT |= 0x0C;                    // P3.2,P3.3 is push-pull

   // Crossbar Initialization
   XBR1    = XBR1_XBARE__ENABLED;      // Enable Crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   TCON = TCON_IT1__EDGE |
          TCON_IT0__EDGE;              // /INT 0 and /INT 1 are edge triggered

   IT01CF = IT01CF_IN1SL__P0_1 |       // /INT0 active low; /INT0 on P0.0;
            IT01CF_IN0SL__P0_0;        // /INT1 active low; /INT1 on P0.1

   IE_EX0 = 1;                         // Enable /INT0 interrupts
   IE_EX1 = 1;                         // Enable /INT1 interrupts

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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