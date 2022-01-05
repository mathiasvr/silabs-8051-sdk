//-----------------------------------------------------------------------------
// F96x_Clock_Normal.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the current consumption of the 24.5 MHz High
// Frequency Oscillator, 20 MHz Low Power Oscillator, and 32.768 kHz Real
// Time Clock in Normal mode - fetching instructions from flash.
//
// Refer to F96x_Clock_Normal_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.4 - Normal Mode Current):
// -------------------------------------------------
// 1) In "Define Hardware", #define either "UDP_F960_MCU_MUX_LCD" or
//    "UDP_F960_MCU_EMIF" depending on which board you are using.
// 2) Cut the trace on the bottom of the PCB between the two pins of the VBAT
//    CURRENT header (LCD: J15, EMIF: J18).
// 3) Cut the trace on the bottom of the PCB between the two pins of the header
//    J20.
// 4) Connect a current meter between the two pins on the VBAT CURRENT header.
// 5) Set the VBAT Selector SW5 to the desired power source.
// 6) Set VIO Select (SW7) and VIORF(SW12) Select to VBAT.
// 7) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) The device will initially be running off the HFOSC (24.5 MHz) in Normal
//    mode. To cycle between HFOSC (24.5 MHz), LPOSC (20 MHz) and the RTC
//    (32.768 kHz), press SW2.
// 10) To measure the clock frequency, place an oscilloscope probe on P0.0.
// 11) To toggle the clock signal on P0.0, press SW3. Note, this will increase
//    current consumption.
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
//    - 16 OCT 2013
//
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F960_Register_Enums.h"

//-----------------------------------------------------------------------------
// Define Hardware
//-----------------------------------------------------------------------------
#define UDP_F960_MCU_MUX_LCD
//#define UDP_F960_MCU_EMIF
//-----------------------------------------------------------------------------
// Hardware Dependent definitions
//-----------------------------------------------------------------------------
#ifdef UDP_F960_MCU_MUX_LCD
SI_SBIT(SW2, SFR_P0, 1);
SI_SBIT(SW3, SFR_P0, 2);
#endif

#ifdef UDP_F960_MCU_EMIF
SI_SBIT(SW2, SFR_P3, 1);
SI_SBIT(SW3, SFR_P3, 2);
#endif

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SW_PRESSED           0
#define SW_NOT_PRESSED       1

#define CLOCK_GATE           1

//-----------------------------------------------------------------------------
// Mode Variables
//-----------------------------------------------------------------------------

// Mode type enumerations
enum CLOCK_NORMAL_MODE {HFOSC = 1,
                        LPOSC = 2,
                        RTC = 3};
						
enum CLOCK_NORMAL_MODE currentMode;
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void Oscillator_Init (void);
void Oscillator_Update(void);
void Port_Init (void);
void Port_Update (void);
void RTC_Init (void);
void RTC_Disable(void);
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
// Debounce latches for SW0.2 and SW0.3
uint8_t switch1Latch = 0; 
uint8_t switch2Latch = 0;

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

    // Initialize Oscillator (Default is 24.5 MHz Precision Oscillator)
    Oscillator_Init (); 
    Port_Init();
#if (CLOCK_GATE)
    PCLKACT = 0x0F;                     // No Clock Gating
#else 
    PCLKACT = 0x00;                    
#endif    
    // Default Mode: 24.5 MHz Precision Oscillator
    currentMode = HFOSC;

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
// This function initializes the system clock to use the internal precision 
// oscillator at the maximum frequency.
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
// P0.1   digital   open-drain   input    SW2 (Switch P0.1)
// P0.2   digital   open-drain   input    SW3 (Switch P0.2)
// P0.3   digital   push-pull    output   SYSCLK (System Clock)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    SFRPAGE = LEGACY_PAGE;
    // Oscillator Pins
#ifdef UDP_F960_MCU_MUX_LCD
    P0SKIP  |=  0x06;                  // P0.1 P0.2 skipped in the Crossbar
    P0MDOUT &= ~0x06;                  // P0.1, P0.2, are open-drain
    P0      |=  0x06;                  // Set Output Latch LOGIC HIGH
                                       // for P0.1, P0.2
#endif

#ifdef UDP_F960_MCU_EMIF
    P3MDOUT &= ~0x06;                  // P3.1, P3.2 are open-drain
    P3      |=  0x06;                  // Set Output Latch LOGIC HIGH
                                       // for P3.1, P3.2
#endif

    P1MDIN &= ~0x0C;                   // P1.2 and P1.3 (RTC Pins) are
                                       // analog

   
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
// P0.1   digital   open-drain   input   SW1 (Switch P0.1)
// P0.2   digital   open-drain   input   SW3 (Switch P0.2)
//
//-----------------------------------------------------------------------------
void Port_Update(void)
{
    // Cycle frequency mode if P0.2 is pushed

    if( (SW2 == SW_PRESSED) && (!switch1Latch) )
    {
        switch (currentMode)
        {
    
            case HFOSC:
                currentMode = LPOSC;
                break;
            case LPOSC:
                currentMode = RTC;
                break;
            case RTC:
                currentMode = HFOSC;
                break;
        }
        // Arm latches to prevent auto-cycling with switch held
        switch1Latch = 1;
        modeLatch = 1;
        while( SW2 == SW_PRESSED);
    }

    // Reset latch once switch is released
    else if(SW2 == SW_NOT_PRESSED)
    {
        switch1Latch = 0;
    }
    
    // Cycle measurement mode if P0.3 is pushed
    if( (SW3 == SW_PRESSED) && (!switch2Latch) )
    {
        
        // Toggle P0.3 between push-pull on/off
        // System Clock Output turn on/off
        P0MDOUT ^= 0x01;
        XBR0 ^= XBR0_SYSCKE__ENABLED;
        
        // Arm latches to prevent auto-cycling with switch held	  
        switch2Latch = 1;
        while( SW3 == SW_PRESSED);
    }
    
    // Reset latch once switch is released
    else if(SW3 == SW_NOT_PRESSED)
    {
        switch2Latch = 0;
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

            case HFOSC :
                SFRPAGE = CONFIG_PAGE;
                FLSCL     = 0x40;       // Flash Read Timing One-Shot Bypass.
                
                SFRPAGE = LEGACY_PAGE;
                            
                REG0CN |= 0x10;
                OSCICN |= 0x80;
                CLKSEL = 0x00;          // Enable HFOSC
                RTC_Disable();
                break;

            case LPOSC :
                CLKSEL = 0x04;          // Switch to Low-Power Oscillator
                OSCICN &= ~0x80;
                REG0CN &=~0x10;
                break;
                
            case RTC:
                RTC_Init();             // Switch to SmaRTC
                SFRPAGE = CONFIG_PAGE;
                FLSCL     = 0x00;       // Flash Read Timing One-Shot Bypass.
                 
                SFRPAGE = LEGACY_PAGE;
                 break;

        }
        // Reset latch once mode is changed
        modeLatch = 0;
    }
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
