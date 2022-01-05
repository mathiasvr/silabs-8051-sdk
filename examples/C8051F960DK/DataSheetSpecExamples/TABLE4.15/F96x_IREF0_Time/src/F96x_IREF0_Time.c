//-----------------------------------------------------------------------------
// F96x_IREF0_Time.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the startup time of the internal current reference
// (IREF0), as specified on table 4.15 of the datasheet.
//
//
// Refer to F96x_IREF0_Time_Readme.txt for more information regarding
// the functionality of this code example.
//
// How To Test (Table 4.15 - Dynamic Performance):
// ---------------------------------------------------------
// 1) Ensure the pins of VBAT CURRENT (J15) are shorted.
// 2) Power the board using the P1 power connector.
// 3) Remove all of the shorting blocks on the board except for the block(s)
//    across the following pins:
//    a) IREF0 & P0.7 on J19
// 4) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Attach oscilloscope probes to P0.7 and P0.0.
// 7) To cycle between Reference on/off, push SW2 (P0.1).
// 8) The toggle time between P0.7 and P0.0 is the output time on datasheet
//    for IREF0 in Table 4.15 
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:    None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 6 FEB 2014
//
// Release 1.0
//    - Initial Revision (MT)
//    - 04 NOV 2013
//
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include "SI_C8051F960_Register_Enums.h"

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(LED1, SFR_P0, 0);                 // '0' means ON, '1' means OFF
SI_SBIT(SW2,  SFR_P0, 1);                 // SW2 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK              20000000    // Clock speed in Hz
#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

#define LED_ON              0
#define LED_OFF             1
#define MAX_VALUE           0x3F
#define MIN_VALUE           0x00        // Set the minimum output code

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);                // Configure the system clock
void PORT_Init (void);                  // Configure the Crossbar and GPIO
void Wait_MS(uint16_t ms);          // Inserts programmable delay
void Mode_Update(void);                 // Cycle Reference mode,push SW2
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Debounce latches for SW0.2 
uint8_t switch1Latch = 0; 

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
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   

    PORT_Init ();                       // Initialize crossbar and GPIO
    SYSCLK_Init();                      // Initialize the system clock
 
    IREF0CN = MAX_VALUE;      		    // Configure IREF0 as a current source
    LED1 = LED_OFF;                                  
    while(1)
    {  
        Mode_Update();
    }

} 

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// Pinout:
//
// P0.0   digital   push-pull            LED1
// P0.1   digital   open-drain/High-Z    Switch 
// P0.7   analog    open-drain/High-Z    IREF0
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   
    // IREF0 Analog Output
    P0MDIN  &= ~0x80;                   // P0.7 is analog
    P0MDOUT &= ~0x80;                   // P0.7 is open-drain
    P0      |=  0x80;                   // Set P0.7 latch to '1' 
    P0SKIP  |=  0x80;                   // P0.7 skipped in Crossbar
   
    // Switch and LED  
    P0MDIN  |=  0x03;                   // P0.0, P0.1 are digital
    P1MDOUT |=  0x01;                   // P0.0, is push-pull
    P0MDOUT &= ~0x02;                   // P0.1 are open-drain
    P0      |=  0x03;                   // Set P0.0, P0.1 latches to '1'
    P0SKIP  |=  0x03;                   // P0.0,P0.1 skipped in Crossbar

    XBR2    = 0x40;                     // Enable crossbar and enable
                                        // weak pull-ups
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal low power
// oscillator at its maximum frequency.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
    SFRPAGE = CONFIG_PAGE;
    FLSCL     = 0x40;                   // Flash Read Timing One-Shot Bypass.
    SFRPAGE = LEGACY_PAGE;
    
    // Select Low-Power Oscillator 20 MHz as system clock source
    // Since using the precision internal oscillator as the system clock source, 
    // the precision voltage reference should not be enabled from a disabled 
    // state. 
    CLKSEL = 0x04;        
    OSCICN &= ~0x80;
    REG0CN &=~0x10;
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Mode_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function checks switch input and select IREF0 mode ,
// once each time the switch is pressed. 
// The cycling order is as follows:
//
// Switch SW2:
// To toggle Reference and LED1 on/off 
// P0.1   digital   open-drain   input   SW2
//
//-----------------------------------------------------------------------------
void Mode_Update(void)
{
    // Cycle Reference mode if P0.2 is pushed
    if( (SW2 == SW_PRESSED) && (!switch1Latch) )
    {
        if (IREF0CN > 0)
        {
            IREF0CN = 0;
             
        }
        else
        {
            IREF0CN = MAX_VALUE;
        }
        
        LED1 = ~LED1;   
        // Arm latches to prevent auto-cycling with switch held
        switch1Latch = 1;
    }

    // Reset latch once switch is released
    else if(SW2 == SW_NOT_PRESSED)
    {
        switch1Latch = 0;
    }
    Wait_MS(50);                        // Wait 50 milliseconds
}
//-----------------------------------------------------------------------------
// Wait_MS
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters:
//   1) uint16_t ms - number of milliseconds of delay
//                        range is full range of integer: 0 to 256
//
// This routine inserts a delay of <ms> milliseconds.
//-----------------------------------------------------------------------------
void Wait_MS(uint16_t ms)
{
    uint8_t i;

    TCON_TR0 = 0;                            // Stop Timer0
   
    TMOD &= ~0x0F;                      // Timer0 in 8-bit mode
    TMOD |= 0x02;
   
    CKCON &= ~0x03;                     // Timer0 uses a 1:48 prescaler
    CKCON |= 0x02;                   
    
    TH0 = -SYSCLK/48/10000;             // Set Timer0 Reload Value to 
                                        // overflow at a rate of 10kHz
   
    TL0 = TH0;                          // Init Timer0 low byte to the
                                        // reload value
   
    TCON_TF0 = 0;                            // Clear Timer0 Interrupt Flag
    IE_ET0 = 0;                            // Timer0 interrupt disabled
    TCON_TR0 = 1;                            // Timer0 on

    while(ms--)
    {
        for(i = 0; i < 10; i++)
        {
            TCON_TF0 = 0;
            while(!TCON_TF0);
        }
    }
    TCON_TF0 = 0;
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
