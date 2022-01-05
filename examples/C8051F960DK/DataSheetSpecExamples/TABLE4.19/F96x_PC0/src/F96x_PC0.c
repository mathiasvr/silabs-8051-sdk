//-----------------------------------------------------------------------------
// F96x_PC0.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// Refer to F96x_PC0_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.19 - Supply Current):
// -----------------------------------------------
// 1) Cut the trace on the bottom of the PCB between the two pins of the VBAT
//    CURRENT header J15.
// 2) Cut the trace on the bottom of the PCB between the two pins of the header
//    J20.
// 3) Connect a current meter between the two pins on header J15 to measure
//    the current draw during sleep mode.
// 4) Connect P1.0 (J11) and SW4 (J16).
// 5) Remove all other shorting blocks and select VBAT on SW5.
// 6) Set #define PC0_EN 1 to enable the PC0 during Sleep mode.
// 7) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) Power supply output voltage refer to Table4.19.
// 10) Record the current draw of the device. Pressing SW4 5 times will toggle
//    LED1 using PC0.
// 11) Set #define PC0_EN 0 to disable PC0 during Sleep mode.
// 12) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 13) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 14) Record the new current, the difference is PC0 supply current.
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
//    - 08 NOV 2013
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

SI_SBIT(LED1, SFR_P0, 0);
SI_SBIT(SW4,  SFR_P1, 0);
//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

#define SLEEP_MODE          0x80
#define PC_PAGE             2

#define PC0_EN              1
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(PC0_ISR,PC0_IRQn);

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
 
void PC0_Init (void);
void Port_Init (void);
void Enter_Sleep (void);
void RTC_Init (void);
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

                                        // enable)
    // Prevent processor into sleep, when SW2 pressed.
    while(SW4 == 0);

    // Initialize Port I/O (SW4)
    Port_Init();             
    
    // Initialize RTC as the system clock 
    RTC_Init();                         // Enable SmaRTClock 
    LED1 = 1;                           // LED1 turn OFF
    
    PMU0CF = 0x20;                      // Clean all Wakeup flag

    // Enable PC0 interrupt.
    EIE2 = 0x10;
    IE_EA = 1;
    while (1)
    {
#if (PC0_EN == 1)
        PC0_Init();
#endif          
        // Sleep mode, ends upon PC0 comparator match event
        Enter_Sleep();
   } 
}

//-----------------------------------------------------------------------------
// PC0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the PC0 Single mode, sample rate 2ms.PC0 comparator
// match event wakes processor from sleep mode.
//
//-----------------------------------------------------------------------------
void PC0_Init (void)
{
    SFRPAGE = PC_PAGE;
    // Single Counter mode,2ms sample rate. Pull-up resistors close.
    PC0MD = 0x70;
    PC0PCF = 0;
    // PC0 Debounce time 10 * 2ms = 20ms
    PC0DCH = 0x0A;
    PC0DCL = 0x0A;
    // PC0 threshold 5 pulse.
    PC0CMP0H = 0;
    PC0CMP0M = 0; 
    PC0CMP0L = 5;
    // PC0 Comparator Interrupt/Wake-up Source Enable
    PC0INT0 = 0x10;
    SFRPAGE = LEGACY_PAGE;
    // Enable wake up on PC0 event. 
    PMU0FL = 0x01;
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Switch and Led. The switch(PC0,P1.0) match event
// is configured to wake up processor from sleep mode. 
//
// P0.0   digital   push-pull    output   LED1
// P1.0   analog    open-drain   input    SW4
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    // lED1 Output, Indicate PC0 work state .
    P0MDIN |= 0x01;                     // P0.0 is digital
    P0MDOUT |= 0x01;                    // P0.0 is open-drain
    P0 |= 0x01;                         // Set the P0.0 latch to '1'
   
    P1MDIN &= ~0x01;                    // P1.0 is analog
    P1MDOUT &= ~0x01;                   // P1.0 is open-drain
    P1 |= 0x01;                         // Set the P1.0 latch to '1'
    P1SKIP  |=  0x01;                   // Skip P1.0 on crossbar
    
    P1MDIN &= ~0x0C;                    // P1.2 and P1.3 (RTC Pins) are
                                        // analog
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
    PMU0CF = SLEEP_MODE;

    //--sleep mode--//
    //= = = = = = = = = = = = = = =
    //= = = = = = = = = = = = = = =
    //--wake on PC0 comparator match event--//
    PMU0CF = 0x20;
}
//-----------------------------------------------------------------------------
// PC0 Comparator match ISR
//-----------------------------------------------------------------------------
//
// This interrupt wakes the processor from Sleep mode and toggle LED1. 
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PC0_ISR,PC0_IRQn)
{
    SFRPAGE = PC_PAGE;
    // reset PC0.
    PC0MD = 0;
    SFRPAGE = LEGACY_PAGE;
    LED1 = ~LED1;
}

//-----------------------------------------------------------------------------
// RTC_Write ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : reg, value
//
// This function will Write one byte from the specified RTC register.
// Using a register number greater that 0x0F is not permitted,
//
// This function uses the new F960 fast write mode.
//
//-----------------------------------------------------------------------------
void RTC_Write (uint8_t reg, uint8_t value)
{
    uint8_t  restoreSFRPAGE;
    restoreSFRPAGE = SFRPAGE;

    SFRPAGE = LEGACY_PAGE;
    reg &= 0x0F;                        // mask low nibble
    RTC0ADR  = reg;                     // pick register
    RTC0DAT = value;                    // write data

    SFRPAGE= restoreSFRPAGE;
}
//-----------------------------------------------------------------------------
// RTC_Read ()
//-----------------------------------------------------------------------------
//
// Return Value : RTC0DAT
// Parameters   : reg
//
// This function will read one byte from the specified RTC register.
// Using a register number greater that 0x0F is not permitted,
//
//
// This function uses the new F960 fast read mode.
//
//-----------------------------------------------------------------------------
uint8_t RTC_Read (uint8_t reg)
{
    uint8_t value;
    uint8_t  restoreSFRPAGE;

    restoreSFRPAGE = SFRPAGE;
    SFRPAGE = LEGACY_PAGE;
    
    reg &= 0x0F;                        // mask low nibble
    RTC0ADR  = (reg |0x80);             // write address setting READ bit
    value = RTC0DAT;
    SFRPAGE= restoreSFRPAGE;

    return value;                       // return value
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

    RTC_Write (RTC0XCF, 0x8A);          // 8a Enable Automatic Load Capacitance
    // stepping and set the desired
    // load capacitance

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

    RTC_Write (RTC0XCN, 0x40);          // Disable bias doubling

    // Wait > 20 ms
    for (i=0x150;i!=0;i--);

    RTC_Write (RTC0XCN, 0x40);          // Enable Automatic Gain Control
    // and disable bias doubling

    RTC_Write (RTC0CN, 0xC0);           // Enable missing smaRTClock detector
    // and leave smaRTClock oscillator
    // enabled.
    // Wait > 2 ms
    for (i=0x40;i!=0;i--);

    PMU0CF = 0x20;                      // Clear PMU wake-up source flags

    CLKSEL = CLKSEL_SAVE;               // Restore system clock
    while(!(CLKSEL & 0x80));            // Poll CLKRDY
    
    // Initializes the system clock to use the smaRTClock oscillator.
    RSTSRC = 0x06;                      // Enable missing clock detector and
    // leave VDD Monitor enabled.
    
    CLKSEL &= ~0x70;                    // Specify a clock divide value of 1
    
    while(!(CLKSEL & 0x80));            // Wait for CLKRDY to ensure the
    // divide by 1 has been applied
    
    CLKSEL = 0x03;                      // Select smaRTClock oscillator
                                        // as the system clock
    OSCICN &= ~0x80;                    // Disable internal oscillator
    
    RTC0KEY = 0x55;                     // Lock smaRTClock interface;
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
