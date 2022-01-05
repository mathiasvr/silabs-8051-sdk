//-----------------------------------------------------------------------------
// F96x_Clock_Normal_DC.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// Refer to F96x_Clock_Normal_DC_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.3 - Normal Mode Current):
// -------------------------------------------------
// 1) Remove the J15 shorting block and make a DC Power Supply and ammeter
//    serial to attach VBAT and GND.
// 3) Remove all other shorting blocks and select VBAT on SW5
// 4) Compile and download the code to target board. Run the code.
// 5) To cycle between system clock output on/off, push SW3 (P0.2).
// 6) Adjust the voltage output of DC Power supply, measure the supply current .
// 7) If system clock measurements are desired, attach an oscilloscope probe
//    to P0.3 and set #define SYSTEM_CLOCK_OUTPUT  1.
// 8) Compile and download the code to your target board. Run the code.
//
//
// Target:         C8051F96x
// Target Board:   UDP-0014-002
// Tool chain:     Generic
// Command Line:   None
//
//
// Revision History:
// -------------------
// Release 1.0
//    - Initial Revision (MT)
//    - 14 OCT 2013
//
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F960_Register_Enums.h>

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------
SI_SBIT(SW3, SFR_P0, 2);
//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SW_PRESSED           0
#define SW_NOT_PRESSED       1

#define SYSTEM_CLOCK_OUTPUT  0
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Port_Init (void);

#if (SYSTEM_CLOCK_OUTPUT)
void Port_Update (void);
#endif
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
// Debounce latches for SW3
uint8_t switchLatch = 0; 

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
                                        // enable)


   
    // Initialize Oscillator (Default is 24.5 MHz Precision Oscillator)
    Oscillator_Init (); 
    Port_Init();
    DC0MD = 1;                          // DC0 enable.
    while (1)
    {
#if (SYSTEM_CLOCK_OUTPUT)
        Port_Update();
#endif	
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
// This function initializes the system clock to use the internal high 
// frequency oscillator at the maximum frequency.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
    SFRPAGE = CONFIG_PAGE;
    FLSCL     = 0x40;
        
    SFRPAGE = LEGACY_PAGE;
    REG0CN |= 0x10;                     // Enable the precision osc. bias
    OSCICN |= 0x80;                     // Enable the precision internal osc.
        
    RSTSRC = 0x06;                      // Enable missing clock detector and
    // leave VDD Monitor enabled.
        
    CLKSEL = 0x00;                      // Select precision internal osc.
    // divided by 1 as the system clock
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
// P0.3   digital   push-pull    output   SYSCLK (System Clock)
// P0.2   digital   open-drain   input    SW3 (Switch P0.2)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    SFRPAGE = LEGACY_PAGE;
    // Oscillator Pinsg
    P0SKIP  |=  0x07;                  // P0.2 skipped in the Crossbar
    P0MDOUT &= ~0x0C;                  // P0.3, P0.2is open-drain
    P0      |=  0x0C;                  // Set Output Latch LOGIC HIGH
                                       // for P0.2, P0.3

    // Buffered System Clock Output
    //P0MDOUT |= 0x08;                   // P0.3 is push-pull

    // Crossbar Initialization
    //XBR0    = 0x08;                    // Route /SYSCLK to first available pin
    XBR2    = 0x40;                    // Enable Crossbar and weak pull-ups
}
//-----------------------------------------------------------------------------
// Runtime Subroutines
//-----------------------------------------------------------------------------

#if (SYSTEM_CLOCK_OUTPUT)
//-----------------------------------------------------------------------------
// Port_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function checks switch input and cycles through oscillator
// modes, once each time the switch is pressed. The cycling order 
// is as follows:
//
// Mode 1: Enable system clock output 24.5 MHz
// Mode 2: Disable system clock output
//
// P0.2   digital   open-drain   input   SW3 (Switch P0.2)
//
//-----------------------------------------------------------------------------
void Port_Update(void)
{

    // Cycle measurement mode if P0.2 is pushed
    if( (SW3 == SW_PRESSED) && (!switchLatch) )
    {
        
        // Toggle P0.3 between push-pull on/off
        // System Clock Output turn on/off
        P0MDOUT ^= 0x08;
        XBR0 ^= 0x08;
        
        // Arm latches to prevent auto-cycling with switch held	  
        switchLatch = 1;
    }

    // Reset latch once switch is released
    else if(SW3 == SW_NOT_PRESSED)
    {
        switchLatch = 0;
    }
}
#endif
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
