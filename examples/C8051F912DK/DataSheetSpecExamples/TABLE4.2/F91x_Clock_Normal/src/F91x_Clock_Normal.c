//-----------------------------------------------------------------------------
// F91x_Clock_Normal.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the current consumption of the device while 
// in Normal mode when using the 24.5 MHz High Frequency Oscillator, the 20 MHz
// Low Power Oscillator, and the 32 kHz Real Time Clock.
//
// Refer to F91x_Clock_Normal_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.2 - Normal Mode Current):
// -------------------------------------------------
// 1) Ensure that shorting blocks are attached to the J11 (VBAT & WALL_PWR) 
//    and J8 (P0.2, P0.3) headers.
// 2) Remove the J17 shorting block and attach an ammeter between
//    the Imeasure headers.
// 3) Remove all other shorting blocks and select 2 Cell (3.3v) on SW4
// 4) If oscilloscope measurements are desired, attach an oscilloscope probe
//    to P0.0.
// 5) Compile and download code to a 'F91x device on a C8051F912-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 6) Disconnect the debugger. The code should now be running.
// 7) To cycle between oscillator frequencies, push SW0.2 (P0.2).
//    The frequencies are 24.5 MHz, 20 MHz, and 32 kHz.
// 8) To toggle the clock out on P0.0, push SW0.3 (P0.3). Remove the
//    oscilloscope probe to prevent extra current draw when measuring
//    current.
//
//
// Target:         C8051F91x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Fixed button debounce.
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

#include "SI_C8051F912_Register_Enums.h"

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(SW0_2, SFR_P0, 2);
SI_SBIT(SW0_3, SFR_P0, 3);

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SW_PRESSED           0
#define SW_NOT_PRESSED       1

//-----------------------------------------------------------------------------
// Mode Variables
//-----------------------------------------------------------------------------

// Mode type enumerations
enum CLOCK_NORMAL_MODE {PROSC = 1,
                        LPOSC = 2,
                        SMARTC = 3};
						
enum CLOCK_NORMAL_MODE currentMode;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Oscillator_Update (void);
void Port_Init (void);
void Port_Update (void);
void RTC_Init (void);
void RTC_Disable(void);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Debounce latch for oscillator mode
uint8_t modeLatch = 0;

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

    // Initialize Port I/O (SW0.2, SW0.3, and SYSCLK output)
    Port_Init();                        // Initialize Port I/O
 
    // Default Mode: 24.5 MHz High-Frequency Oscillator
    currentMode = PROSC;

    // Initialize Oscillator (Default is 24.5 MHz Precision Oscillator)
    Oscillator_Init ();  

    while (1)
    {

        // Detect switch inputs and adjust modes
        Port_Update();

        // Update oscillator with new modes
        Oscillator_Update();

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
void OSCILLATOR_Init (void)
{

    // Select Precision Internal Oscillator 24.5MHz as system clock source
    REG0CN |= 0x10;
    OSCICN |= 0x80;	
    CLKSEL = 0x00; 

    // At clock frequencies above 10 MHz, the system clock cycle becomes 
    // short enough that the one-shot timer no longer provides a power benefit.
    // Disabling the one-shot timer at higher frequencies reduces power 
    // consumption. 
    FLSCL = 0x40;
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
// P0.0   digital   push-pull    output   SYSCLK (System Clock)
// P0.2   digital   open-drain   input    SW0.2 (Switch P0.2)
// P0.3   digital   open-drain   input    SW0.3 (Switch P0.3)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    // Oscillator Pinsg
    P0SKIP  |=  0x0C;                  // P0.2, P0.3 skipped in the Crossbar
    P0MDOUT &= ~0x0D;                  // P0.0, P0.2, P0.3 is open-drain
    P0      |=  0x0D;                  // Set Output Latch LOGIC HIGH
                                       // for P0.0, P0.2, P0.3


    // Crossbar Initialization
    XBR2    = 0x40;                    // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Runtime Subroutines
//-----------------------------------------------------------------------------

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
// Mode 1: Precision Oscillator,  24.5 MHz
// Mode 2: Low-Power Oscillator,  20 MHz
// Mode 3: SmaRTClock Oscillator, 32.768 kHz   
//
// P0.2   digital   open-drain   input   SW0.2 (Switch P0.2)
// P0.3   digital   open-drain   input   SW0.3 (Switch P0.3)
//
//-----------------------------------------------------------------------------
void Port_Update(void)
{
    // Cycle frequency mode if P0.2 is pushed
    if (SW0_2 == SW_PRESSED)
    {
        switch (currentMode)
        {

            case PROSC:
                currentMode = LPOSC;
                break;
            case LPOSC:
                currentMode = SMARTC;
                break;
            case SMARTC:
                currentMode = PROSC;
                break;
        }

        while(SW0_2 == SW_PRESSED);

        // Arm latches to prevent auto-cycling with switch held
        modeLatch = 1;
    }


    // Cycle measurement mode if P0.3 is pushed
    if (SW0_3 == SW_PRESSED)
    {
        
        // Toggle P0.0 between push-pull on/off
        // System Clock Output turn on/off
        P0MDOUT ^= 0x01;
        XBR0 ^= 0x08;
        
        while(SW0_3 == SW_PRESSED);
    }

}

//-----------------------------------------------------------------------------
// Oscillator_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes new oscillator settings based on the operating
// mode chosen by the user. This function only changes oscillator settings
// once each time the switch is pressed.
//
//-----------------------------------------------------------------------------
void Oscillator_Update(void)
{
    if( modeLatch )
    {
        // Cycle through oscillator settings
        switch(currentMode)
        {

            case PROSC :
                 FLSCL = 0x40;         // Flash Read Timing One-Shot Bypass.
                 REG0CN |= 0x10;
                 OSCICN |= 0x80;
                 CLKSEL = 0x00;        // Enable PROSC 
                 RTC_Disable();
                 break;

            case LPOSC :
                 CLKSEL = 0x04;        // Switch to Low-Power Oscillator
                 OSCICN &= ~0x80;
                 REG0CN &=~0x10;
                 break;
                
            case SMARTC:       
                 RTC_Init();           // Switch to SmaRTC
                 break;

        }
        // Reset latch once mode is changed
        modeLatch = 0;
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
// Using a register number greater that 0x0F is not permited.
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
// Using a register number greater that 0x0F is not permited.
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
   
    RSTSRC = 0x06;                     // Enable missing clock detector and
                                       // leave VDD Monitor enabled.
   
    CLKSEL &= ~0x70;                   // Specify a clock divide value of 1
   
    while(!(CLKSEL & 0x80));           // Wait for CLKRDY to ensure the 
                                       // divide by 1 has been applied
      
    CLKSEL = 0x03;                     // Select smaRTClock oscillator 
                                       // as the system clock
    OSCICN &= ~0x80;                   // Disable internal osciallator
    
    FLSCL = 0x00;
    RTC0KEY = 0x55;                    // lock the smaRTClock interface;
    
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
