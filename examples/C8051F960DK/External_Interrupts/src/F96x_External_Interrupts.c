//-----------------------------------------------------------------------------
// F96x_External_Interrupts.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This software shows the necessary configuration to use External Interrupt 0
// (/INT0) or External Interrupt 1 (/INT1) as an interrupt source.  The code
// executes the initialization routines and then spins in an infinite while()
// loop.  If the button SW1 (on the target board) is pressed, then the
// edge-triggered /INT0 input on P0.0 will cause an interrupt and toggle
// LED3. If the button SW2 is pressed, then the edge-triggered
// /INT1 input on P0.1 will cause an interrupt and toggle LED4.
//
// Pinout:
//
// UDP F960 MCU MUX LCD:
// P0.0 - SW1/INT0
// P0.1 - SW2/INT1
// P0.2 - LED3
// P0.3 - LED4
//
// UDP F960 MCU EMIF:
// P0.0 - /INT0
// P0.1 - /INT1
// P3.0 - SW1
// P3.1 - SW2
// P3.2 - LED3
// P3.3 - LED4
//
//
// How To Test:
// 1) In "Define Hardware", #define either "UDP_F960_MCU_MUX_LCD" or
//    "UDP_F960_MCU_EMIF" depending on which board you are using.
// 2) Ensure that jumpers are placed on the following:
//       P2 (LCD) or P12 (EMIF):
//             P0.4/MCU_TX
//             P0.5/MCU_RX
//       J16 (LCD only):
//             P0.3/LED4/SW4
// 3) If using the EMIF board, make the following connections:
//             P0.0 <-> P3.0
//             P0.1 <-> P3.1
// 4) Connect the USB Debug Adapter to J13.
// 5) Move the VBAT SEL switch (SW5) to the middle VREG position.
// 6) Move the VIO switch (SW7) and VIORF switch (SW12) to the VBAT position.
// 7) Connect a 9V power adapter to P1 of the development board.
// 8) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 9) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 10) Press the switches SW1 or SW2. LED3 and LED4 should toggle on and off
//    with each respective switch press.
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Added support for the EMIF dev board.
//    - 17 JAN 2014
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F960_Register_Enums.h>

//-----------------------------------------------------------------------------
// Define Hardware
//-----------------------------------------------------------------------------
//#define UDP_F960_MCU_MUX_LCD
#define UDP_F960_MCU_EMIF
//-----------------------------------------------------------------------------
// Hardware Dependent definitions
//-----------------------------------------------------------------------------
#ifdef UDP_F960_MCU_MUX_LCD
SI_SBIT(SW1, SFR_P0, 0);
SI_SBIT(SW2, SFR_P0, 1);
SI_SBIT(LED3,  SFR_P0, 2);
SI_SBIT(LED4,  SFR_P0, 3);
#endif

#ifdef UDP_F960_MCU_EMIF
SI_SBIT(SW1, SFR_P3, 0);
SI_SBIT(SW2, SFR_P3, 1);
SI_SBIT(LED3,  SFR_P3, 2);
SI_SBIT(LED4,  SFR_P3, 3);
#endif

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             20000000    // Clock speed in Hz

#define LED_ON  0
#define LED_OFF 1

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);           // Configure the system clock
void Port_Init (void);                 // Configure the Crossbar and GPIO
void Ext_Interrupt_Init (void);        // Configure External Interrupts (/INT0
                                       // and /INT1)


SI_INTERRUPT_PROTO(INT0_ISR, INT0_IRQn);
SI_INTERRUPT_PROTO(INT1_ISR, INT1_IRQn);

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
   PCA0MD &= ~0x40;                    // Disable Watchdog timer
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
// This function initializes the system clock to use the internal low power
// oscillator at its maximum frequency.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   SFRPAGE = LEGACY_PAGE;
   CLKSEL = 0x04;                     // Select low power oscillator with a
                                      // clock divider value of 1 (20MHz)
}


//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   XBR2    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
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
   SFRPAGE = LEGACY_PAGE;

   TCON = 0x05;                        // /INT 0 and /INT 1 are edge triggered

   IT01CF = 0x10;                      // /INT0 active low; /INT0 on P0.0;
                                       // /INT1 active low; /INT1 on P0.1

   IE_EX0 = 1;                            // Enable /INT0 interrupts
   IE_EX1 = 1;                            // Enable /INT1 interrupts
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// /INT0 ISR
//-----------------------------------------------------------------------------
//
// Whenever a negative edge appears on SW1, LED3 is toggled.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(INT0_ISR, INT0_IRQn)
{
   SFRPAGE = LEGACY_PAGE;
   LED3 = !LED3;
}

//-----------------------------------------------------------------------------
// /INT1 ISR
//-----------------------------------------------------------------------------
//
// Whenever a negative edge appears on P0.3, LED4 is toggled.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(INT1_ISR, INT1_IRQn)
{
   SFRPAGE = LEGACY_PAGE;
   LED4 = !LED4;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
