//-----------------------------------------------------------------------------
// F36x_Oscillator_Crystal.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to configure the oscillator for use with an
// external crystal.
//
//
// How To Test:
// ------------
// 1) Populate Y1 with a crystal (32.768 kHz), R1 with a 10 MOhm resistor and
//    two 22 pF load capacitances on C19 and C20, also update <Crystal_Clock>
//    with the crystal frequency in Hz.
// 2) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J1: Power selection, P2 9V, P4 USB or J9 Debugger
// 3) Connect the USB Debug Adapter to J9. If 9V power is selected on J1,
//    connect the 9V power adapter to P2. If VBUS is selected on J1, connect
//    USB cable to P4.
// 4) Compile and download code to C8051F360 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 6) Place a scope probe on P0.0 and observe the frequency of SYSCLK output.
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
//    -Initial Revision (FB / TP)
//    -25 OCT 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>

//-----------------------------------------------------------------------------
// External Oscillator Crystal Frequency Selection
//-----------------------------------------------------------------------------
//
// Please initialize the following constant to the crystal frequency
//
#define Crystal_Clock 32768L           // External Crystal oscillator
                                       // frequency (Hz)
                                       // NOTE: This value will determine the
                                       // XFCN bits in ExtCrystalOsc_Init ()
// XFCN bits setting based on the crystal frequencY
#if (Crystal_Clock <= 32000)
   #define XFCN 0
#elif (Crystal_Clock <= 84000L)
   #define XFCN 1
#elif (Crystal_Clock <= 225000L)
   #define XFCN 2
#elif (Crystal_Clock <= 590000L)
   #define XFCN 3
#elif (Crystal_Clock <= 1500000L)
   #define XFCN 4
#elif (Crystal_Clock <= 4000000L)
   #define XFCN 5
#elif (Crystal_Clock <= 10000000L)
   #define XFCN 6
#elif (Crystal_Clock <= 30000000L)
   #define XFCN 7
#else
   #error "Defined Crystal Frequency outside allowable range!"
   #define XFCN 0
#endif

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void ExtCrystalOsc_Init (void);
void Port_Init (void);

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
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   Port_Init ();                       // Initialize crossbar and GPIO

   SYSCLK_Init ();                     // Initialize system clock to 24.5MHz

   ExtCrystalOsc_Init ();              // Initialize for and switch to the
                                       // external Crystal oscillator

   SFRPAGE = LEGACY_PAGE;

   while (1) {};                       // Loop forever
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
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
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   OSCICN |= OSCICN_IFCN__HFOSC_DIV_1; // Configure internal oscillator for
                                       // its highest frequency (24.5 MHz)

   RSTSRC = 0x04;                      // Enable missing clock detector

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// ExtCrystalOsc_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes for and switches to the External Crystal
// Oscillator.
//
void ExtCrystalOsc_Init (void)
{
   uint16_t i;
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // Step 1. Enable the external oscillator.
   OSCXCN = OSCXCN_XOSCMD__CRYSTAL;    // External Oscillator is an external
                                       // crystal (no divide by 2 stage)
   OSCXCN |= XFCN;

   // Step 2. Wait at least 1 ms.
   for (i = 12250; i > 0; i--);        // At 24.5 MHz, 1 ms = 24500 SYSCLKs
                                       // DJNZ = 2 SYSCLKs

   // Step 3. Poll for XTLVLD => ‘1’.
   while ((OSCXCN & OSCXCN_XTLVLD__BMASK) != OSCXCN_XTLVLD__SET);

   // Step 4. Switch the system clock to the external oscillator.
   CLKSEL = CLKSEL_CLKSL__EXTOSC;

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// The oscillator pins should be configured as analog inputs when using the
// external oscillator in crystal mode.
//
// P0.0   digital    push-pull      /SYSCLK
//
// P0.5   analog     don't care     XTAL1
// P0.6   analog     don't care     XTAL2
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // Oscillator Pins
   P0MDIN &= ~0x60;                    // P0.5, P0.6 are analog
   P0SKIP |= 0x60;                     // P0.5, P0.6 skipped in the Crossbar

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0 = XBR0_SYSCKE__ENABLED;        // Route /SYSCLK to a port pin
   XBR1 = XBR1_XBARE__ENABLED;         // Enable crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------