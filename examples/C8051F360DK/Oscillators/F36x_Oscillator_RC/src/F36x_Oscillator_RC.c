//-----------------------------------------------------------------------------
// F36x_Oscillator_RC.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to configure the oscillator for use with an
// external RC Oscillator.
//
//
// How To Test:
// ------------
// 1) Connect a 33pF capacitor (C19) between P0.6 (XTAL2) and GND, and a
//    330kOhm resistor (R5) between P0.3 (XTAL2) and Vcc (3.3V). And
//    define the capacitor and pull-up resistor values using <Cap_Value> and
//    <Resistor_Value>.
// 2) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J1: Power selection, P2 9V, P4 USB or J9 Debugger.
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
//    -13 OCT 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>

//-----------------------------------------------------------------------------
// External Oscillator RC Frequency Selection
//-----------------------------------------------------------------------------
//
// Note: The Cap_Value and Resistor_Value numbers are formatted specifically
// to keep all numbers whole (not decimals) and within a representable range
// (32 bits).
//

#define Cap_Value 33                   // Value of the external capacitor, in
                                       // pF
                                       // The capacitor should be no greater
                                       // than 100 pF.

#define Resistor_Value 330000L         // Value of the external pull-up
                                       // resistor, in ohms

                                       // Note: Debugging cannot occur when the
                                       // SYSCLK is less than 32 kHz.
// Error checks on the input values
// The capacitor should be no greater than 100 pF.
#if (Cap_Value > 100)
   #error "Defined Capacitor Value outside allowable range!"
   #define XFCN 0
#endif

// XFCN bits setting based on the oscillator frequency
#define Output_Freq 1230000000L/(Resistor_Value * Cap_Value)

#if (Output_Freq <= 25L)
   #define XFCN 0
#elif (Output_Freq <= 50L)
   #define XFCN 1
#elif (Output_Freq <= 100L)
   #define XFCN 2
#elif (Output_Freq <= 200L)
   #define XFCN 3
#elif (Output_Freq <= 400L)
   #define XFCN 4
#elif (Output_Freq <= 800L)
   #define XFCN 5
#elif (Output_Freq <= 1600L)
   #define XFCN 6
#elif (Output_Freq <= 3200L)
   #define XFCN 7
#else
   #define XFCN 0
   #error "Resistor_Value and Cap_Value produce out-of-bounds frequency!"
#endif

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void ExtRCOsc_Init (void);
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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Clear watchdog timer enable
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{

   SYSCLK_Init();                      // Initialize SYSCLK
   Port_Init();                        // Initialize Port I/O
   ExtRCOsc_Init ();                   // Initialize External Oscillator

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
// P0.0   digital    push-pull     /SYSCLK
// P0.6   analog     don't care    XTAL2
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // Oscillator Pins
   P0MDIN &= ~0x40;                    // P0.6 is analog
   P0SKIP |= 0x40;                     // P0.6 skipped in the Crossbar

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0 = XBR0_SYSCKE__ENABLED;        // Route /SYSCLK to a port pin
   XBR1 = XBR1_XBARE__ENABLED;         // Enable crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// ExtRCOsc_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the external oscillator
// in RC mode.
//
//-----------------------------------------------------------------------------
void ExtRCOsc_Init (void)
{
   int16_t i;                              // Delay counter
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   
   OSCXCN = OSCXCN_XOSCMD__RC;         // External Oscillator is an external
                                       // resistor-capacitor

   OSCXCN |= XFCN;

   // RC and C modes typically require no startup time.

   for (i=0; i < 256; i++);            // Wait for RC osc. to start

   CLKSEL = CLKSEL_CLKSL__EXTOSC;      // Select external oscillator as system
                                       // clock source

   OSCICN = OSCICN_IOSCEN__DISABLED;   // Disable the internal oscillator.

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------