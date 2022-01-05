//-----------------------------------------------------------------------------
// F91x_Clock_Idle.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the current consumption of the device while 
// in Idle mode when using the 24.5 MHz High Frequency Oscillator, the 20 MHz
// Low Power Oscillator, and the 32 kHz Real Time Clock.
//
// Refer to F91x_Clock_Idle_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.2 - Idle Mode Current):
// -----------------------------------------------
// 1) Ensure that shorting blocks are attached to the J11(VBAT & WALL_PWR)and 
//    J8(P0.2H & SW0.2)headers.
// 2) Remove the J17 shorting block and attach an ammeter between
//    the Imeasure headers.
// 3) Remove all other shorting blocks and select 2 Cell (3.3v) on SW4
// 4) Compile and download code to a 'F99x device on a C8051F996-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 5) Disconnect the debugger. The code should now be running.
// 6) To cycle between oscillator frequencies, push SW0.2.
//    The frequencies are 24.5 MHz, 20 MHz, and 32 kHz.
//
// Target:         C8051F99x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 6 FEB 2014
//
// Release 1.0
//    - Initial Revision (MT)
//    - 21 OCT 2013
//
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F990_Register_Enums.h"

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(SW0_2, SFR_P0, 2);

//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

#define SW_PRESSED           0
#define SW_NOT_PRESSED       1

//-----------------------------------------------------------------------------
// Mode Variables
//-----------------------------------------------------------------------------

// Mode type enumerations
enum CLOCK_IDLE_MODE {PROSC = 1,
                      LPOSC = 2,
                      SMARTC = 3};

enum CLOCK_IDLE_MODE nextMode;

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
void Enter_Idle (void);
void RTC_Init (void);
void RTC_Disable(void);
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

                                        // enable)

   // Starting Mode: 24.5 MHz Percision Oscillator
   // Next Mode is Low Power Oscillator
   nextMode = LPOSC;

   // Initialize Port I/O (SW0.2)
   Port_Init();             
      
   // Initialize Oscillator (Default is 24.5 MHz PROSC)
   Oscillator_Init();  
     
   while (1)
   {
   
        // Wait until user lets go of SW0_2 before sleeping
        while( SW0_2 == SW_PRESSED );
         
        // Idle mode, ends upon SW0_2 Port Match event
        Enter_Idle();

        // Cycle to next oscillator frequency after wake up
        Oscillator_Cycle();

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

    // Select Precision Internal Oscillator 24.5MHz as system
    // clock source
    REG0CN |= 0x10;
    OSCICN |= 0x80;	
    CLKSEL = 0x00; 
    //At clock frequencies above 10 MHz, the system clock cycle becomes 
    //short enough that the one-shot timer no longer provides a power benefit.
    //Disabling the one-shot timer at higher frequencies 
    //reduces power consumption. 
    FLSCL = 0x40;			   
	
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins. The switch used
// is configured to both wake up from sleep mode and change the oscillator
// operating frequency.
//
// P0.2   digital   open-drain   input   SW0.2
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // SW0_2 (P0.2) Input - Oscillator Clock Cycle Switch
   P0MDIN |= 0x04;                      // P0.2 is digital
   P0MDOUT &= ~0x04;                    // P0.2 is open-drain
   P0 |= 0x40;                          // Set the P0.2 latch to '1'
   
   // Set Port Match settings for S1 (P0.2)
   P0MASK |= 0x04;                      // Enable Port Match check for P0.2
   P0MAT  |= 0x04;                      // Set Port Match value to 1 for P0.2

   
   P1MDIN &= ~0xC0;                     // P1.6 and P1.7 (RTC Pins) are
                                        // analog
   // Crossbar Initialization
   XBR2 = 0x40;                         // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Runtime Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Enter_Idle
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Puts the processor into Idle mode, in order to allow the user
// to measure idle mode current draw. Idle mode ends when SW0_2 (P0.2) is
// pushed.
//
//-----------------------------------------------------------------------------
void Enter_Idle (void)
{
   uint8_t IE_save;
   uint8_t EIE2_save;

   IE_EA = 0;                             // Clear global interrupt enable
   FLSCL = 0;		
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
// Oscillator_Cycle
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function sets the internal oscillator to the next frequency setting.
// The process occurs once each time the processor is woken up from sleep
// mode after a port match interrupt.
//
// Mode 1: Precision Oscillator,  24.5 MHz
// Mode 3: Low-Power Oscillator,  20 MHz
// Mode 3: SmaRTClock Oscillator, 32.768 kHz   
//
//-----------------------------------------------------------------------------
void Oscillator_Cycle(void)
{
    // Cycle through oscillator settings
    switch(nextMode)
    {
        case PROSC :
            FLSCL = 0x40;               //Flash Read Timing One-Shot Bypass.
            REG0CN |= 0x10;
            OSCICN |= 0x80;
            CLKSEL = 0x00;              // Enable PROSC
            RTC_Disable();
            nextMode = LPOSC;
            break;
      
        case LPOSC :
            CLKSEL = 0x04;              // Switch to LPOSC
            OSCICN &= ~0x80;
            REG0CN &=~0x10;
            nextMode = SMARTC;
            break;
                  
        case SMARTC:
            RTC_Init();                 // Switch to SmaRTC
            nextMode = PROSC;
            break;
    }
   
}
//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

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

   EIE2 &= ~0x02;                       // Disable Port Match interrupts to
                                        // prevent multiple interrupts from
                                        // occurring while the switches are
                                        // pressed
}

//-----------------------------------------------------------------------------
// RTC_Read ()
//-----------------------------------------------------------------------------
//
// Return Value : RTC0DAT
// Parameters   : 1) uint8_t reg - address of RTC register to read
//
// This function will read one byte from the specified RTC register.
// Using a register number greater that 0x0F is not permitted.
//
//-----------------------------------------------------------------------------

uint8_t RTC_Read (uint8_t reg)
{
    reg &= 0x0F;                        // mask low nibble
    RTC0ADR  = reg;                     // pick register
    RTC0ADR |= 0x80;                    // set BUSY bit to read
    while ((RTC0ADR & 0x80) == 0x80);   // poll on the BUSY bit
    return RTC0DAT;                     // return value
}

//-----------------------------------------------------------------------------
// RTC_Write ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) uint8_t reg - address of RTC register to write
//                2) uint8_t value - the value to write to <reg>
//
// This function will Write one byte to the specified RTC register.
// Using a register number greater that 0x0F is not permitted.
//-----------------------------------------------------------------------------
void RTC_Write (uint8_t reg, uint8_t value)
{
    reg &= 0x0F;                        // mask low nibble
    RTC0ADR  = reg;                     // pick register
    RTC0DAT = value;                    // write data
    while ((RTC0ADR & 0x80) == 0x80);   // poll on the BUSY bit
}

//-----------------------------------------------------------------------------
// RTC_Disable ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will Disable RTC. 
//-----------------------------------------------------------------------------
void RTC_Disable(void)
{
    RTC0KEY = 0xA5;                     // Unlock the smaRTClock interface
    RTC0KEY = 0xF1;
    
    RTC_Write (RTC0XCN, 0x00);          // Configure the smaRTClock
                                        // oscillator for crystal mode
                                        // Bias Doubling, AGC Disabled
    RTC_Write (RTC0XCF, 0x00); 
    RTC_Write (RTC0CN, 0x00);           // Disable smaRTClock oscillator
    RTC0KEY = 0x55;                     // lock the smaRTClock interface;
}

//-----------------------------------------------------------------------------
// RTC_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will initialize the RTC. First it unlocks the RTC interface,
// enables the RTC, clears ALRMn and CAPTUREn bits. Then it sets up the RTC
// to operate using a 32.768khZ crystal. Lastly it enables the alarm and
// interrupt. This function uses the RTC primitive functions to access 
// the internal RTC registers.
//
//-----------------------------------------------------------------------------
void RTC_Init (void)
{
    uint16_t i;

    uint8_t CLKSEL_SAVE = CLKSEL;   

    RTC0KEY = 0xA5;                     // Unlock the smaRTClock interface
    RTC0KEY = 0xF1;

    RTC_Write (RTC0XCN, 0x60);          // Configure the smaRTClock
                                        // oscillator for crystal mode
                                        // Bias Doubling Enabled
                                        // AGC Disabled
   
    RTC_Write (RTC0XCF, 0x81);          // Enable Automatic Load Capacitance
                                        // stepping and set the desired 
                                        // load capacitance to 4.5pF plus
                                        // the stray PCB capacitance

    RTC_Write (RTC0CN, 0x80);           // Enable smaRTClock oscillator
      
    //----------------------------------
    // Wait for smaRTClock to start
    //----------------------------------
    CLKSEL    =  0x74;                  // Switch to 156 kHz low power
                                        // internal oscillator
    // Wait > 20 ms

    for (i=0x150;i!=0;i--);  

    // Wait for smaRTClock valid
    while ((RTC_Read (RTC0XCN) & 0x10)== 0x00); 
   
    // Wait for Load Capacitance Ready
    while ((RTC_Read (RTC0XCF) & 0x40)== 0x00); 
   
    //----------------------------------
    // smaRTClock has been started
    //----------------------------------
 
    RTC_Write (RTC0XCN, 0xC0);          // Enable Automatic Gain Control
                                        // and disable bias doubling

    RTC_Write (RTC0CN, 0xC0);           // Enable missing smaRTClock detector
                                        // and leave smaRTClock oscillator
                                        // enabled. 
    // Wait > 2 ms
    for (i=0x340;i!=0;i--);  
   
    PMU0CF = 0x20;                      // Clear PMU wake-up source flags

    CLKSEL = CLKSEL_SAVE;               // Restore system clock
    while(!(CLKSEL & 0x80));            // Poll CLKRDY
   
    RSTSRC = 0x06;                      // Enable missing clock detector and
                                        // leave VDD Monitor enabled.
   
    CLKSEL &= ~0x70;                    // Specify a clock divide value of 1
   
    while(!(CLKSEL & 0x80));            // Wait for CLKRDY to ensure the 
                                        // divide by 1 has been applied
      
    CLKSEL = 0x03;                      // Select smaRTClock oscillator 
                                        // as the system clock
    OSCICN &= ~0x80;                    // Disable internal oscillator
    
    FLSCL = 0x00;
    RTC0KEY = 0x55;                     // lock the smaRTClock interface;
    
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
