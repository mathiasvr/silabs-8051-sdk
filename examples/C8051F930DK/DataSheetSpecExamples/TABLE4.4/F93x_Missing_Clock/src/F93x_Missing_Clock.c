//-----------------------------------------------------------------------------
// F93x_Missing_Clock.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the Missing clock reset detecting and Missing
// clock reset detector timeout. Pressing a switch to change system clock
// from 32kHz to less than 10kHz that will cause the processor to reset
// triggered by the Missing Clock Detector.
//
// Refer to F93x_Missing_Clock_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.4 - Reset):
// -----------------------------------------------
//  1) Ensure a shorting block is placed across VBAT & WALL_PWR (J11) and
//     Imeausre (J17). 
//  2) Place VDD Select SW4 in the 2 Cell (3.3v) position and power the board
//     using the P2 power connector.
//  3) Remove all shorting blocks on the board except for the block across 
//     the following pins:
//     a) LED1.6 and P1.6 on J8
//     b) P0.2H and SW0.2 on J8
//  4) Compile and download the code to target board.
//  5) Run the code.
//  6) Press SW0.2. DS4 will be lighted if  missing Clock  Detector timeout 
//     caused the last reset.
//  7) Attach an oscilloscope probe to P1.5 (J8). 
//  8) Using the oscilloscope measure the low level length of P1.5 after 
//     press SW0.2 .the length is missing clock timeout .
//  9) Since power on reset cause the value read from all other bits in 
//     RSTSRC register are indeterminate.So the first missing clock timeout
//     measured result should be ignored. user can press SW0_2 to make more test.
//    
// Target:         C8051F93x/92x
// Tool chain:     Generic
// Command Line:   None
//
//
// Revision History:
// ------------------
// Release 1.0
//    - Initial Revision (MT)
//    - 10 SEP 2013
//
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F930_Register_Enums.h>

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(SW0_2, SFR_P0, 2);
SI_SBIT(DS3, SFR_P1, 5);
SI_SBIT(DS4, SFR_P1, 6);

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

    // Initialize Port I/O (SW0.2)
    Port_Init();
    
    // Initialize Oscillator 32khz SmaRTClock
    // 32kHz SmaRTClock can be divide to less than 10 kHz to system clock 
    RTC_Init();                         // Switch to SmaRTClock
    if (RSTSRC & 0x04)
    {
        // Missing clock cause a last rest
        DS4 = LED_ON;                   // LED ON        
    }
    RSTSRC = 0x06;                      // Enable Missing Clock detector 
    
    // Wait until user press SW0_2 
    while( SW0_2 == SW_NOT_PRESSED );
  
    // SmaRTClock crystal 32kHz divided to low frequency ;
    DS3 =  LED_ON;                      // LED ON
    CLKSEL |= SMART_CLK_DIV8;           // Switch to 4kHz low frequency
   
    while (RSTSRC & 0x04)
    DS3 = LED_OFF;                      // LED OFF       
      
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
// P0.2   digital   open-drain   input   SW0.2
// P1.5   digital   push-pull    output  DS3
// P1.6   digital   push-pull    output  DS3
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // SW0_2 (P0.2) Input - Oscillator Clock Cycle Switch
   P0MDIN |= 0x04;                      // P0.2 is digital
   P0MDOUT &= ~0x04;                    // P0.2 is open-drain
   P0 |= 0x40;                          // Set the P0.2 latch to '1'
  
   // DS3(P1.5), DS4(P1.6) Output
   P1MDIN |= 0xC0;                      // P1.5,P1.6 digital
   P1MDOUT |= 0xC0;                     // P1.5,P1.6 push pull output
   P1 |= 0xC0;                          // Set P1.5, P1.6 latch to "1"
   
   // Crossbar Initialization
   XBR2 = 0x40;                         // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// RTC_Read ()
//-----------------------------------------------------------------------------
//
// Return Value : RTC0DAT
// Parameters   : 1) unsigned char reg - address of RTC register to read
//
// This function will read one byte from the specified RTC register.
// Using a register number greater that 0x0F is not permitted.
//
//-----------------------------------------------------------------------------

unsigned char RTC_Read (unsigned char reg)
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
// Parameters   : 1) unsigned char reg - address of RTC register to write
//                2) unsigned char value - the value to write to <reg>
//
// This function will Write one byte to the specified RTC register.
// Using a register number greater that 0x0F is not permitted.
//-----------------------------------------------------------------------------
void RTC_Write (unsigned char reg, unsigned char value)
{
    reg &= 0x0F;                        // mask low nibble
    RTC0ADR  = reg;                     // pick register
    RTC0DAT = value;                    // write data
    while ((RTC0ADR & 0x80) == 0x80);   // poll on the BUSY bit
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
    unsigned int i;

    unsigned char CLKSEL_SAVE = CLKSEL;   


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
