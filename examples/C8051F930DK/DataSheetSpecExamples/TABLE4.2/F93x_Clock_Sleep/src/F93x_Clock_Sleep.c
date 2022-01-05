//-----------------------------------------------------------------------------
// F93x_Clock_Sleep.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the current consumption of the device while 
// in Sleep mode with and without an external Real Time Clock running.
//
// Refer to F93x_Clock_Sleep_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.2 - Sleep Mode Current):
// -----------------------------------------------
// 1) Ensure that shorting blocks are attached to the J11(VBAT & WALL_PWR)and 
//    J8(P0.2H & SW0.2)headers.
// 2) Remove the J17 shorting block and attach an ammeter between
//    the Imeasure headers.
// 3) Remove all other shorting blocks and select 2 Cell (3.3v) on SW4
// 4) Compile and download code to a 'F91x device on a C8051F930-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 5) Disconnect the debugger. The code should now be running.
// 6) To cycle between CPU power modes, press SW0.2. Initially, the device
//    will be in Suspend Mode. The device should draw approximately 77uA.
// 7) Pressing SW0.2 once will put the device in Sleep mode, with an external
//    32 kHz RTC running. The device should draw approximately .75 uA.
// 8) Pressing SW0.2 twice will put the device in Sleep mode, with no RTC.
//    The device should draw approximately .12 uA.
//
// Target:         C8051F93x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 6 FEB 2014
//
// Release 1.0
//    - Initial Revision (MT)
//    - 29 AUG 2013
//
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F930_Register_Enums.h"

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(SW0_2, SFR_P0, 2);

//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

#define PORT_MATCH_WAKEUP   0x02
#define SUSPEND_MODE        0x40
#define SLEEP_MODE          0x80
//-----------------------------------------------------------------------------
// Mode Variables
//-----------------------------------------------------------------------------

// Mode type enumerations
enum CLOCK_SLEEP_MODE {SUSPEND = 1,
                       SLEEP_RTC = 2,
                       SLEEP = 3};

enum CLOCK_SLEEP_MODE nextMode;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

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
void RTC_Init (void);
void RTC_Disable(void);
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

                                        // enable)
										//
   // Prevent processor into sleep, when SW0_2 pressed after reset.
   while(SW0_2 == 0);

   // Starting Mode: LPOSC suspend mode
   nextMode = SUSPEND;

   // Initialize Port I/O (SW0_2)
   Port_Init();             
      
   // Initialize Oscillator (Default is 20.0 MHz LPOSC)
   Oscillator_Init();  
     
   while (1)
   {
        // Wait until user presses SW0_2 
        while( SW0_2 != SW_PRESSED );
         
        // Sleep mode, ends upon SW0_2 Port Match event
        Enter_Sleep();

        // Cycle to next CPU Mode after System Wake up
        Oscillator_Cycle();
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
    
    // Enable VDD Supply monitor
    VDM0CN = 0x80;
    // waiting for VDD above the VDD warn monitor threshold
    while (!(VDM0CN & 0x20));
    RTC_Init();                         // Enable SmaRTClock
    
    CLKSEL = 0x04;                      // Switch to LPOSC
    OSCICN &= ~0x80;
    REG0CN &=~0x10;
    PMU0CF = 0x20;                      // Clean all Wakeup flag
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
// P0.2   digital   open-drain   input   SW0_2
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // SW0_2 (P0.2) Input - CPU Mode Cycle Switch
   P0MDIN |= 0x04;                      // P0.2 is digital
   P0MDOUT &= ~0x04;                    // P0.2 is open-drain
   P0 |= 0x40;                          // Set the P0.2 latch to '1'
   
   // Set Port Match settings for SW0_2 (P0.2)
   P0MASK |= 0x04;                      // Enable Port Match check for P0.2
   P0MAT  |= 0x04;                      // Set Port Match value to 1 for P0.2

   // Crossbar Initialization
   XBR2 = 0x40;                         // Enable Crossbar and weak pull-ups
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
// System wake up when SW0_2 (P0.2) is pressed.
//
//-----------------------------------------------------------------------------
void Enter_Sleep (void)
{ 
    // Go into  mode
   switch (nextMode)
   {
        case SUSPEND :
            // Go into suspend mode
            PMU0CF = SUSPEND_MODE | PORT_MATCH_WAKEUP; 
            break;
        
        default:
            // Go into sleep mode
            PMU0CF = SLEEP_MODE | PORT_MATCH_WAKEUP;
            break;
   }
  
   //--sleep mode--//
   //= = = = = = = = = = = = = = =
   //= = = = = = = = = = = = = = =
   //--wake on port match event--//
   PMU0CF = 0x20;
}

//-----------------------------------------------------------------------------
// Oscillator_Cycle
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function sets the CPU Mode to the next setting.
// The process occurs once each time the processor is woken up from sleep
// mode after a port match event happen.
//
// Mode 1: Suspend
// Mode 2: Sleep, SmaRTClock running 
// Mode 3: Sleep, SmaRTClock disable   
//
//-----------------------------------------------------------------------------
void Oscillator_Cycle(void)
{
    // Cycle through oscillator settings
    switch(nextMode)
    {
        case SUSPEND :
            nextMode = SLEEP_RTC;
            break;
      
        case SLEEP_RTC :
            RTC_Disable();
            nextMode = SLEEP;
            break;
                  
        case SLEEP:
            RTC_Init();                 // Enable SmaRTClock
            nextMode = SUSPEND;
            break;
    }
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

    RTC_Write (RTC0XCN, 0x60);

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
      
    //CLKSEL = 0x03;                    // Select smaRTClock oscillator 
                                        // as the system clock
    //OSCICN &= ~0x80;                  // Disable internal oscillator
    
    //FLSCL = 0x00;
    RTC0KEY = 0x55;                     // lock the smaRTClock interface;
    
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
