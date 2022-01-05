//-----------------------------------------------------------------------------
// F96x_PR_Oscillator_P.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the power consumption of the Internal Precision 
// oscillator.
//
// How to Measure (Table 4.9 - Internal Precision Oscillator):
// -------------------------------------------------
// 1) Cut the trace on the bottom of the PCB between the two pins of the VBAT
//    CURRENT header
// 2) Cut the trace on the bottom of the PCB between the two pins of the header
//    J20.
// 3) Connect a current meter between the two pins on header J15 to measure
//    the current draw of the device.
// 4) Remove all other shorting blocks and select VREG on SW4
// 5) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) The Internal Precision Oscillator is enabled initially, record this
//    initial current draw.
// 8) Use SW1 (P0.0) to disable the Internal Precision Oscillator. Record the
//    new current draw.
// 9) The difference betwen the two measurements is the Internal Precision 
//    Oscillator current.
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 6 FEB 2014
//
// Release 1.0
//    - Initial Revision (MT)
//    - 19 OCT 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F960_Register_Enums.h"

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------
SI_SBIT(SW1, SFR_P0,0);
//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SW_PRESSED           0
#define SW_NOT_PRESSED       1
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
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
    PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
    uint8_t switch1Latch = 0; 
    
                                        // enable)

    Port_Init();                        // Initialize Port I/O
    
    // Default enable 24.5 MHz Internal precision Oscillator
    // Select  Low power oscillator as system clock source.
    Oscillator_Init ();                 
    
    while (1)
    {
        // waiting for user press SW3 KEY
        if(SW1 == SW_PRESSED)
        {
            switch1Latch = 1;
        }
        
        if ((SW1 == SW_NOT_PRESSED) && (switch1Latch == 1))
        {
            if (OSCICN & 0x80)
            {
                // Disable Precision Internal Oscillator 
                REG0CN &= ~0x10;
                OSCICN &= ~0x80;
            }
            else
            {
                // Enable Precision Internal Oscillator 
                REG0CN |= 0x10;
                OSCICN |= 0x80;
            }
            switch1Latch = 0;
        }
    }
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
// This function initializes the system clock to use the internal precision
// oscillator at the maximum frequency.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
    
    SFRPAGE = CONFIG_PAGE;    
    // At clock frequencies above 10 MHz, the system clock cycle becomes 
    // short enough that the one-shot timer no longer provides a power benefit.
    // Disabling the one-shot timer at higher frequencies reduces power 
    // consumption. 
    FLSCL     = 0x40;        
    SFRPAGE = LEGACY_PAGE;
    
    // Internal Precision Oscillator 24.5MHz enabled
    REG0CN |= 0x10;
    OSCICN |= 0x80;	
    
    // Select Low-Power Oscillator as system clock source
    CLKSEL = 0x04; 
 
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
// P0.0   digital    push-pull             Precision Oscillator Clock ON/OFF
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    // SW1 (P0.0) Input - Oscillator Clock Cycle Switch
    P0MDIN |= 0x01;                      // P0.0 is digital
    P0MDOUT &= ~0x01;                    // P0.0 is open-drain
    P0 |= 0x01;                          // Set the P0.0 latch to '1'
 
    // Crossbar Initialization
    XBR2 = 0x40;                         // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
