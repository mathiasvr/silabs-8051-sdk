//-----------------------------------------------------------------------------
// F96x_Sleep_Wakeup.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the wake-up times from Idle, Suspend, and Sleep
// modes, as specified on table 4.7 of the datasheet.
//
// Refer to F96x_Sleep_Wakeup_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.7 - Sleep Wakeup Time):
// -----------------------------------------------
// 1) Ensure the pins of the VBAT CURRENT header (J15) are shorted, either by
//    the un-cut trace between the pins underneath the header, or by a shorting
//    block.
// 2) Remove all other shorting blocks and select VREG on SW5.
// 3) Set #define ENTER_MODE to select the mode to wake from.
// 4) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Use an oscilloscope to monitor SW2 (P0.1) and LED1(P0.0).
// 7) Push SW2. The delay between SW2 being pulled low and LED1 being driven 
//    low is the sleep wakeup time on datasheet Table 4.7.
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
//    - 04 NOV 2013
//
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F960_Register_Enums.h"

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(SW2,  SFR_P0, 1);
SI_SBIT(LED1, SFR_P0, 0);
//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

#define PORT_MATCH_WAKEUP   0x02
#define SUSPEND_MODE        0x40
#define SLEEP_MODE          0x80

#define ENTER_SLEEP         0
#define ENTER_SUSPEND       1
#define ENTER_IDLE          2

// Select enter mode 
// 0 Sleep mode
// 1 Suspend mode
// 2 Idle mode
#define ENTER_MODE          0
//-----------------------------------------------------------------------------
// Mode Variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(PM_ISR,PMATCH_IRQn);

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
    PCA0MD &= ~0x40;                     // WDTE = 0 (clear watchdog timer
}
 
void Oscillator_Init (void);
void Oscillator_Cycle (void);
void Port_Init (void);
void Enter_Sleep (void);
void Enter_Suspend (void);
void Enter_Idle (void);
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

                                         // enable)
    // Prevent processor into sleep, when SW2 pressed after reset.
    while(SW2 == 0);

    // Initialize Port I/O (SW2)
    Port_Init();             
   
    // Initialize Oscillator (Default is 20.0 MHz LPOSC)
    Oscillator_Init();  
    PMU0CF = 0x20;                      // Clean all Wakeup flag
    LED1 = 0;                           // Turn on LED1.
    while (1)
    {
		
        // Wait until user press SW2 
        while( SW2 == SW_PRESSED );
        LED1 = 1;                       // Turn off LED1 
        
        switch (ENTER_MODE)
        {
            case ENTER_SLEEP:
                Enter_Sleep();
                break;
                
            case ENTER_SUSPEND:
                Enter_Suspend();
                break;   
                
            case ENTER_IDLE:
                Enter_Idle();
                break;
                
            default:
                break;
        }
        
    } 
}

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to Low Power frequency
// oscillator at the maximum frequency.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{    
    CLKSEL = 0x04;          // Switch to Low-Power Oscillator 20Mhz
    OSCICN &= ~0x80;
    REG0CN &=~0x10;
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins. The switch used
// is configured to wake up from sleep mode and change the CPU Mode
//
// P0.0   digital   push-pull    output  LED1
// P0.1   digital   open-drain   input   SW2
//
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    SFRPAGE = LEGACY_PAGE;
    // SW2 (P0.1) Input - Oscillator Clock Cycle Switch
    // LED1(P0.0) Output - Measure Sleep wake up time
    P0MDIN |= 0x03;                     // P0.0 P0.1 is digital
    P0MDOUT |= 0x01;                    // P0.0 is push-pull
    P0MDOUT &= ~0x02;                   // P0.1 is open-drain
    P0 |= 0x03;                         // Set P0.0 P0.1 latch to '1'
   
    // Set Port Match settings for S1 (P0.2)
    P0MASK |= 0x02;                     // Enable Port Match check for P0.1
    P0MAT  |= 0x02;                     // Set Port Match value to 1 for P0.1
 
    // Crossbar Initialization
    XBR2 = 0x40;                        // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Runtime Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Enter_Sleep
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Puts the processor into sleep mode, in order to allow the user
// to measure Suspend / Sleep mode current draw. 
// System wake up when SW2 (P0.1) is pressed.
//
//-----------------------------------------------------------------------------
void Enter_Sleep (void)
{ 
    // Go into sleep mode
    PMU0CF = SLEEP_MODE | PORT_MATCH_WAKEUP;
  
    //--sleep mode--//
    //= = = = = = = = = = = = = = =
    //= = = = = = = = = = = = = = =
    //--wake on port match event--//
    LED1 = 0;  
    PMU0CF = 0x20;
}
//-----------------------------------------------------------------------------
// Enter_Suspend
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Puts the processor into suspend mode, in order to allow the user
// to measure Suspend mode current draw. 
// System wake up when SW2 (P0.1) is pressed.
//
//-----------------------------------------------------------------------------
void Enter_Suspend (void)
{ 
    // Go into suspend mode
    PMU0CF = SUSPEND_MODE | PORT_MATCH_WAKEUP; 
  
    //--Suspend mode--//
    //= = = = = = = = = = = = = = =
    //= = = = = = = = = = = = = = =
    //--wake on port match event--//
    LED1 = 0;                           // Turn on LED1.
    PMU0CF = 0x20;
}

//-----------------------------------------------------------------------------
// Enter_Idle
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Puts the processor into Idle mode, in order to allow the user
// to measure Idle mode current draw. 
// System wake up when SW0_2 (P0.2) is pressed.
//
//-----------------------------------------------------------------------------
void Enter_Idle (void)
{ 
    uint8_t IE_save;
    uint8_t EIE2_save;
    
    IE_EA = 0;                             // Clear global interrupt enable
   
    // Save interrupt state
    IE_save = IE;                       // Preserve IE register
    EIE2_save = EIE2;                   // Preserve EIE1 register
    IE = 0x00;                          // Disable interrupts
    
    EIE2 = 0x02;                        // Enable port match interrupts
    IE_EA = 1;                             // Enable global interrupts
    
    // Go into idle mode
    PCON |= 0x01;                       // Idle mode
    PCON = PCON;                        // Follow the instruction to enter idle
    // with a 3 cycle dummy instruction
    
    //--Idle mode--//
    //--wake on port match event--//
    EIE2 = EIE2_save;                   // Restore ext interrupts
    IE = IE_save;                       // Restore interrupts
    IE_EA = 1;                             // Set global interrupt enable
}

//-----------------------------------------------------------------------------
// Port Match ISR
//-----------------------------------------------------------------------------
//
// This interrupt wakes the processor from idle mode, and proceeds to continue
// to the Oscillator_Cycle() function.
//
// The Port Match interrupt occurs whenever (Pn & PnMASK) != (PnMAT & PnMASK).
// We must disable the interrupt and wait until the switches are released in
// main, or else the interrupt will trigger multiple times.  It is generally
// not good practice to sit in an ISR for long periods of time.
//
// P0.2   digital   open-drain   input   SW0.2 - Port Match Trigger
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PM_ISR,PMATCH_IRQn)
{

    LED1 = 0;                           // Turn on LED1.
    EIE2 &= ~0x02;                      // Disable Port Match interrupts to
                                        // prevent multiple interrupts from
                                        // occurring while the switches are
                                        // pressed
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
