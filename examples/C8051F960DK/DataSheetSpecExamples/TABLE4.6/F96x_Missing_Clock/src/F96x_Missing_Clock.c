//-----------------------------------------------------------------------------
// F96x_Missing_Clock.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the electrical characteristics for the missing
// clock detector reset source, as specified on table 4.6 of the datasheet.
//
// Refer to F96x_Missing_Clock_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.6 - Reset):
// -----------------------------------------------
//  1) Ensure the pins of the VBAT CURRENT header (J15) are shorted, either by
//     a shorting block, or the un-cut trace underneath the header pins.
//  2) Place VDD Select SW5 in the VREG position and power the board
//     using the P1 power connector.
//  3) Remove all shorting blocks on the board 
//  4) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//     board by selecting Run -> Debug from the menus, clicking the Debug button
//     in the quick menu, or pressing F11.
//  5) Run the code by selecting Run -> Resume from the menus, clicking the
//     Resume button in the quick menu, or pressing F8.
//  6) Press SW3. LED2 will be lighted if  missing Clock  Detector timeout 
//     caused the last reset.
//  7) Attach an oscilloscope probe to P0.1 (J11). 
//  8) Using the oscilloscope measure the low level length of P0.1 after 
//     press SW3 (P0.2) .the length is missing clock timeout .
//  9) Since power on reset cause the value read from all other bits in 
//     RSTSRC register are indeterminate.So the first missing clock timeout
//     measured result should be ignored.
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
//    - 17 OCT 2013
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
SI_SBIT(LED2, SFR_P0, 1);
SI_SBIT(SW3, SFR_P0, 2);
//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

#define SW_PRESSED           0
#define SW_NOT_PRESSED       1
#define LED_ON               0
#define LED_OFF              1

#define SMART_CLK_DIV4       0x20
#define SMART_CLK_DIV8       0x30

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
    PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
}
 
void Oscillator_Init (void);
void Oscillator_Cycle (void);
void Port_Init (void);
void Enter_Idle (void);
void RTC_Init (void);
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

                                        // enable)

    // Initialize Port I/O (SW3)
    Port_Init();
    
    // Initialize Oscillator 32khz SmaRTClock
    // 32kHz SmaRTClock can be divide to less than 10 kHz to system clock 
    RTC_Init();                         // Switch to SmaRTClock
    if (RSTSRC & 0x04)
    {
        // Missing clock caust a last rest
        LED2 = LED_ON;                  // LED2 ON        
    }
    RSTSRC = 0x06;                      // Enable Missing Clock detector 
    
    // Wait until user press SW0_2 
    while( SW3 == SW_NOT_PRESSED );
  
    // SmaRTClock crystal 32kHz divided to low frequency ;
    LED1 =  LED_ON;                     // LED1 ON
    CLKSEL |= SMART_CLK_DIV8;           // Switch to 4kHz low frequency
   
    while (RSTSRC & 0x04)
    LED1 = LED_OFF;                     // LED1 OFF       
      
    while(1);
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

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
// P0.0   digital   push-pull    output  LED1
// P0.1   digital   push-pull    output  LED2
// P0.2   digital   open-drain   input   SW3
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    // SW3(P0.2) Input - Oscillator Clock Cycle Switch
    // LED1(P0.0) LED2(P0.1) Output
    P0MDIN |= 0x07;                     // P0.0 P0.2 P0.2 is digital
    P0MDOUT |= 0x03;                    // P0.0,P0.1 push pull output
    P0MDOUT &= ~0x04;                   // P0.2 is open-drain
    P0 |= 0x07;                         // Set the P0.0 P0.1 P0.2 latch to '1'
    
    P1MDIN &= ~0x0C;                    // P1.2 and P1.3 (RTC Pins) are
                                        // analog
    // Crossbar Initialization
    XBR2 = 0x40;                        // Enable Crossbar and weak pull-ups
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
