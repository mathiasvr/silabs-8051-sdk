//-----------------------------------------------------------------------------
// F96x_Voltage_Monitor.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
//
// Program Description:
// --------------------
// This program demonstrates the Voltage Supply Monitor power consumption 
// listed in Table 4.6 of the datasheet. This program also demonstrates the 
// VDD Supply Monitor Threshold listed in Table 4.6 of the datasheet. The 
// supply monitor is enabled at the start of the program and switch SW2 is
// used to toggle the enable bit. The power consumption can be tested by 
// measuring the current of both modes and taking the difference. The supply
// monitor is selected as a reset source when the supply monitor is enabled. 
// When the supply monitor is disabled, it is deselected as a reset source.
//
//
// How To Test:
// ------------
//  1) Cut the trace on the bottom of the PCB between the two pins of the VBAT
//     CURRENT header (J15).
//  2) Cut the trace on the bottom of the PCB between the two pins of the header
//     J20.
//  3) Connect a current meter between the two pins on header J15 to measure
//     the current draw of the device.
//  4) Remove all other shorting blocks and select VREG on SW5.
//  5) Default is digital monitor supply, if user want to measure analog
//     monitor supply, set #define ANALOG_MODE  1
//  6) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//     board by selecting Run -> Debug from the menus, clicking the Debug button
//     in the quick menu, or pressing F11.
//  7) Run the code by selecting Run -> Resume from the menus, clicking the
//     Resume button in the quick menu, or pressing F8.
//  8) Record the current reading. This reading is the current with the voltage
//     monitor enabled.
//  9) Use switch at SW2 (P0.1) to disable the voltage monitor.
// 10) Record the current reading and subtract the value from the previously
//     recorded value. Verify the difference is within the range of the values
//     listed on the datasheet for Voltage Supply Monitor under Table 4.6.
// 11) Use SW2 to enable the voltage monitor.
// 12) Using the IDE, stop the program and disconnect the board.
// 13) Remove any and all power sources to the board including the debugger.
// 14) Remove the ammeter from Imeasure J15 and select VBAT on SW5.
// 15) Connect a power supply to VBAT and set to 3.3V (LED1 ON).
// 16) Use an oscilloscope to monitor /RST-(TP2). 
// 17) Reduce the supply voltage until RESET is pulled low (LED1 OFF). 
//     Verify the voltage at which RESET is pulled low is within the values 
//     given for VDD Supply Monitor Threshold in Table 4.6. 
//
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
// Refer to F96x_Voltage_Monitor_Readme.txt for more information. 
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F960_Register_Enums.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SW_PRESSED      0
#define SW_NOT_PRESSED  1

#define ANALOG_MODE     0
//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(SW2, SFR_P0,1);
SI_SBIT(LED1, SFR_P0,0);
uint8_t VDM0CN_Mode;
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(PM_ISR, PMATCH_IRQn);

void SYSCLK_Init (void);
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
    PCA0MD &= ~0x40;                     
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   
    // Disable the watchdog timer

    SYSCLK_Init ();                     // Initialize system clock to 24.5 MHz
    Port_Init ();                       // Initialize crossbar and GPIO
	
    IE_EA = 1;                             // Enable global interrupts

    // Loop forever
    while (1) {}
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator divided by 1 as its clock source.  Also enables missing clock
// detector reset.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
#if (ANALOG_MODE == 1)
    VDM0CN_Mode = 0x08;                 // Enable Analog VDD Monitor. 
#else
    VDM0CN_Mode = 0x80;                 // Enable Digital VDD Monitor. 
#endif
    VDM0CN |= VDM0CN_Mode;
    
    //At clock frequencies above 10 MHz, the system clock cycle becomes 
    //short enough that the one-shot timer no longer provides a power benefit.
    //Disabling the one-shot timer at higher frequencies 
    //reduces power consumption. 
    SFRPAGE = CONFIG_PAGE;
    FLSCL     = 0x40;        
    SFRPAGE = LEGACY_PAGE;
    
    REG0CN |= 0x10;                     // Enable the precision osc. bias
    OSCICN |= 0x80;                     // Enable the precision internal osc.

    // Waiting for above the VDD monitor warning threshold
#if (ANALOG_MODE == 1)
    while (!(VDM0CN & 0x02));
#else
    while (!(VDM0CN & 0x20));
#endif
    RSTSRC = 0x02;                      // Enable missing clock detector and
                                        // set VDD Monitor as reset source
    
    CLKSEL = 0x00;                      // Select precision internal osc.
                                        // divided by 1 as the system clock.
    
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the Crossbar and GPIO ports.
//
// P0.0     digital     push-pull      output   LED1 
// P0.1     digital     open-drain     input    SW2
//-----------------------------------------------------------------------------
void Port_Init (void)
{

    // Port 0 initialization
    P0MDIN |= 0x03;                     // Set P0.0 P0.1 as digital
    P0MDOUT |= 0x01;                    // Set P0.0 as output
    P0MDOUT &= ~0x02;                   // Set P0.1 as open-drain
    P0 |= 0x03;                         // Set P0.0 P0.1 latch to '1'

    P0MAT  |= 0x02;                     // Set up the port-match value on P0.1
    P0MASK |= 0x02;                     // An event will occur if
                                        // (P0 & P0MASK) != (P0MAT & P0MASK)
                                        // The switch pins are 1 if the switch
                                        // isn't pressed.

    EIE2 |= 0x02;                       // Enable Port Match interrupts
    LED1 = 0;                           // LED1 ON
    XBR2 = 0x40;                        // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port Match ISR
//-----------------------------------------------------------------------------
//
// Whenever SW2 (P0.1) is pressed, the VDD Monitor enable is toggled. When the
// VDD Monitor is enabled, the VDD Monitor is selected as a reset source. When
// the VDD Monitor is disabled, the VDD Monitor is deselected as a reset 
// source.
//
// The Port Match interrupt occurs whenever (Pn & PnMASK) != (PnMAT & PnMASK).
// We must disable the interrupt and wait until the switches are released in
// main, or else the interrupt will trigger multiple times.  It is generally
// not good practice to sit in an ISR for long periods of time.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PM_ISR, PMATCH_IRQn)
{
    uint8_t SavePage;
    SavePage = SFRPAGE;
    SFRPAGE = LEGACY_PAGE;
    if (SW2 == SW_PRESSED)
    {
        // If the Voltage Supply Monitor is enabled
        if((VDM0CN & VDM0CN_Mode) == VDM0CN_Mode)
        {
            VDM0CN &= ~VDM0CN_Mode;     // Disable VDD Monitor
            RSTSRC = 0;                 // Clear VDD Monitor as reset source
        } 
        else
        {
            VDM0CN |= VDM0CN_Mode;      // Enable VDD Monitor
            RSTSRC = 0x02;              // Set VDD Monitor as reset source
        }

        P0MAT &= ~0x02;                 // Set port match to trigger at switch
                                        // rising edge to prevent multiple 
                                        // interrupts from occurring while the
                                        // switch is pressed   
    }
    else
    {
        P0MAT |= 0x02;                  // Reset port match to trigger at switch
                                        // falling edge
    }
    SFRPAGE = SavePage;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
