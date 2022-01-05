//-----------------------------------------------------------------------------
// F96x_DC0.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the output voltage and current characteristics
// of the DC-DC converter, as specified on table 4.20 of the datasheet.
//
// Refer to F96x_DC0_Readme.txt for more information regarding
// the functionality of this code example.
//
// How To Test (Table 4.20 - Output Voltage):
// ---------------------------------------------------------
// 1) Remove the J15 shorting block and place VDD Select VBAT in the SW5 
//    position.
// 2) Attach an DC supply between VBAT and GND, set DC supply output 4.2V.
// 3) Attach an voltmeter between VDC and GND
// 4) Place VIO/VIORF switches (SW7/SW12) in the VBAT position.
// 5) Remove all of the shorting blocks on the board. 
// 6) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) LED1 is on by default to indicate the default 1.8V voltage level.
// 9) Press SW2(P0.2) to increase the output voltage level. LED1 will turn off.
//    Once the output voltage reaches the highest level (3.5V), press SW2 to
//    decrease the voltage level. When the voltage reaches 1.8V again, LED1
//    should once again turn on.
// 10) Measure the voltage on VDC each time. The voltage should read between
//    the minimum and maximum values provided on the datasheet for the DC-DC 
//    Converter in Table 4.20
//
// How To Test (Table 4.20 - Output Current):
// ---------------------------------------------------------
// 1)  Remove the J15 shorting block and place VDD Select VBAT in the SW5 
//     position .
// 2)  Attach an power supply between VBAT and GND, set power supply output 4.2V.
// 3) Place VIO/VIORF switches (SW7/SW12) in the VBAT position.
// 4)  Remove all of the shorting blocks on the board. 
// 5) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) LED1 is on by default to indicate the default 1.8V voltage level.
// 8) Press SW2(P0.2) to increase the output voltage level. LED1 will turn off.
//    Once the output voltage reaches the highest level (3.5V), press SW2 to
//    decrease the voltage level. When the voltage reaches 1.8V again, LED1
//    should once again turn on.
// 9) Attach a ammeter and resistance series between VDC and GND.
// 10) Change the corresponding resistance (such as 35ohm, 3v)and Measure the
//    current. This value should read between the minimum and maximum values
//    that are provided on the datasheet for the DC-DC Converter in Table 4.20
//
// How To Test (Table 4.20- Output Power):
// ---------------------------------------------------------
//    Output Power = Output voltage * Output current.
// 1) The output power should be between the minimum and maximum values provided  
//    on the datasheet for the DC-DC Converter in Table 4.20
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
//    - 06 NOV 2013
//
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include "SI_C8051F960_Register_Enums.h"

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(LED1,   SFR_P0, 0);                // '0' means ON, '1' means OFF
SI_SBIT(SW2,    SFR_P0, 1);                // SW2 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK              20000000    // Clock speed in Hz
#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

#define LED_ON              0
#define LED_OFF             1

#define LEVEL0              0
#define lEVEL15             15

#define DC0_PAGE            2
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);                // Configure the system clock
void PORT_Init (void);                  // Configure the Crossbar and GPIO
void Wait_MS(uint16_t ms);          // Inserts programmable delay
void Voltage_Update(void);              // Cycle DC0 output level, push SW2 
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Debounce latches for SW2 
uint8_t switch1Latch = 0; 

// DC0 output voltage changing direction
// 1: Increase level
// 0: Decrease level
bit dc0Direction = 0;

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
 
uint8_t dc0Level;
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   

    PORT_Init ();                       // Initialize crossbar and GPIO
    SYSCLK_Init();                      // Initialize the system clock
     
    SFRPAGE = DC0_PAGE; 
    // DCDC Converter Bias Output Enable
    // To same bias generator is shared by the DCDC Converter and LCD0. 
    LCD0MSCN = 0x20;
    // Default DC0 enabled as level 0
    dc0Level = LEVEL0;
    DC0CF = dc0Level << 3;
    DC0MD |= 0x05;                      // DC0 enabled with 
                                        // automatic bypass mode
    SFRPAGE = LEGACY_PAGE;
    
    LED1 = LED_ON;                      // Turn ON LED1
    
    while(1)
    {  
        Voltage_Update();
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
// P0.0   digital   push-pull           LED1
// P0.1   digital   open-drain          SW2
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
    // Switch and LED 
    P0MDIN  |=  0x03;                   // P0.0, P0.1 are digital
    P0MDOUT &= ~0x02;                   // P0.1 is open-drain
    P0MDOUT |=  0x01;                   // P0.0 is open-drain
    P0      |=  0x03;                   // Set P0.0, P0.1 latches to '1'
    P0SKIP  |=  0x03;                   // P0.0, P0.1 skipped in Crossbar

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
    // At clock frequencies above 10 MHz, the system clock cycle becomes 
    // short enough that the one-shot timer no longer provides a power benefit.
    // Disabling the one-shot timer at higher frequencies reduces power 
    // consumption. 
    FLSCL     = 0x40;        
    SFRPAGE = LEGACY_PAGE;
      
    // Select Low-Power Oscillator 20 MHz as system clock source
    
    CLKSEL = 0x04;        
    OSCICN &= ~0x80;
    REG0CN &=~0x10;
 
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Voltage_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function checks switch input and select DC0 output voltage level
// once the switch is pressed. The level changing direction from low 
// to high growth. when it reach highest level then direction changed from high 
// to low till level0.
//
// P0.1   digital   open-drain   input   SW2
//
//-----------------------------------------------------------------------------
void Voltage_Update(void)
{     
    // Cycle DC0 level if P0.2 is pushed
    if( (SW2 == SW_PRESSED) && (!switch1Latch) )
    {
        SFRPAGE = DC0_PAGE;  
        if (LED1 == LED_ON)
        {
            LED1 = LED_OFF;              // Turn OFF LED1
        }
            
        if (dc0Direction)
        {
            if (--dc0Level == LEVEL0)
            {
                dc0Direction = 0;
                LED1 = LED_ON;           // Turn ON LED1
            }
            DC0CF = dc0Level << 3;
            
        }
        else
        {
            
            if (dc0Level++ < lEVEL15)
            {
                DC0CF = dc0Level << 3;
            }
            else
            {
                dc0Level = lEVEL15 - 1;
                DC0CF = dc0Level << 3;
                dc0Direction = 1;
            }
        }
        SFRPAGE = LEGACY_PAGE;
        // Arm latches to prevent auto-cycling with switch held
        switch1Latch = 1;
    }
    
    // Reset latch once switch is released
    else if(SW2 == SW_NOT_PRESSED)
    {
        switch1Latch = 0;
    }
    // Wait 50 milliseconds
    Wait_MS(50);  
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
