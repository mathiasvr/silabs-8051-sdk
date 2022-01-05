//-----------------------------------------------------------------------------
// F91x_DC0.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the Output Voltage range, Current, Power
// consumption and clocking frequency on datasheet for the DC-DC in Table 4.16.
//
// Refer to F91x_DC0_Readme.txt for more information regarding
// the functionality of this code example.
//
// How To Test (Table 4.16 - Output Voltage):
// ---------------------------------------------------------
// 1) Ensure a shorting block is placed the J11 (VBAT & WALL_PWR) .
// 2) Place VDD Select 1 CELL (1.8v) in the SW4 position and power the board
//    using the P2 power connector.
// 3) Attach an voltmeter between VDD/DC+ and GND.
// 4) Remove all of the shorting blocks on the board except for the block(s)
//    across the following pins:
//    a) Imeasure on J17
//    b) LED1.5 and P1.5 on J8
//    c) SW0.2 and P0.2H on J8
// 5) Download and run the code on the target board.
// 6) Default DC0 output voltage leve0 1.8v, DS3 turn on.
// 7) To push SW0.2 (P0.2) once the output voltage increase a level, DS3 turn 
//    off.When the voltage reached the highest 3.3V , push SW0.2 once the output
//    voltage reduce a level till level0 then DS3 turn on.
// 8) Measure the voltage on VDD/DC+ each time. The voltage should be read between
//    the minimum and maximum values that provided on the datasheet for the DC-DC 
//    Converter in Table 4.16
//
// How To Test (Table 4.16 - Output Current):
// ---------------------------------------------------------
// 1) Ensure a shorting block is placed the J11 (VBAT & WALL_PWR) .
// 2) Place VDD Select 1 CELL (1.8v) in the SW4 position and power the board
//    using the P2 power connector.
// 3) Attach an voltmeter between VDD/DC+ and GND.
// 4) Remove all of the shorting blocks on the board except SW0.2 and P0.2H 
//    on J8 and the Imeasure on J17.
// 5) Download and run the code on the target board.
// 6) Default DC0 output voltage leve0 1.8v.
// 7) To push SW0.2 (P0.2) once the output voltage increase a level.When the 
//    voltage reached the highest 3.3V , push SW0.2 once the output
//    voltage reduce a level till level0.
// 8) Attach a ammeter and resistance series between VDD/DC+ and GND.
// 9) Change the corresponding resistance (such as 50ohm, 1.8v)and Measure the 
//    current that should be read between the minimum and maximum values that 
//    provided on the datasheet for the DC-DC Converter in Table 4.16
//
// How To Test (Table 4.16 - Output Power):
// ---------------------------------------------------------
//    Output Power = Output voltage * Output current.
// 1) The output power should be between the minimum and maximum values provided  
//    on the datasheet for the DC-DC Converter in Table 4.16
//
// How To Test (Table 4.16 - Clocking frequency):
// ---------------------------------------------------------
// 1) Ensure a shorting block is placed the J11 (VBAT & WALL_PWR) .
// 2) Place VDD Select 1 CELL (1.8v) in the SW4 position and power the board
//    using the P2 power connector.
// 3) Remove all of the shorting blocks on the board except the Imeasure on J17.
// 4) By default DC-DC is clocked from its internal oscillator, if user want to
//    clock from system clock source, set #define SYSTEM_CLOCK  1.
// 5) Download and run the code on the target board.
// 6) Use a probe detect the output frequency on DCEN pin.
// 7) The frequency should be read between the minimum and maximum values provided 
//    on the datasheet for the DC-DC Converter in Table 4.16
//
//
//
// Target:         C8051F91x/90x
// Tool Chain:     Generic
// Command Line:   None
//
// Release 1.1 (JL)
//    - Updated description
//    - 15 Feb 2014
//
// Release 1.0
//    - Initial Revision (MT)
//    - 02 NOV 2013
//
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(DS3,   SFR_P1, 5);                // '0' means ON, '1' means OFF
SI_SBIT(SW0_2, SFR_P0, 2);                // SW0_2 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK              20000000    // Clock speed in Hz
#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

#define LED_ON              0
#define LED_OFF             1

#define SYSTEM_CLOCK        0
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);                // Configure the system clock
void PORT_Init (void);                  // Configure the Crossbar and GPIO
void Wait_MS(unsigned int ms);          // Inserts programmable delay
void Voltage_Update(void);              // Cycle DC0 output level, push SW0_2 
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Debounce latches for SW0.2 
uint8_t switch1Latch = 0; 

// DC0 output voltage changing direction
// 1: Increase level
// 0: Decrease level
bit dc0Direction = 0;

// Mode type enumerations
enum DC0_LEVEL {LEVEL0 = 0,
                LEVEL1 = 1,
                LEVEL2 = 2,
                LEVEL3 = 3,
                LEVEL4 = 4,
                LEVEL5 = 5,
                LEVEL6 = 6,
                LEVEL7 = 7};
						
enum DC0_LEVEL dc0Level;

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
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   

    PORT_Init ();                       // Initialize crossbar and GPIO
    SYSCLK_Init();                      // Initialize the system clock
 
    // Default DC0 enabled as level 0
    // Default internal oscillator frequency 2.4MHz
    dc0Level = LEVEL0;
    DC0CN = LEVEL0;
    DS3 = LED_ON;                       // Turn ON DS3
    
#if (SYSTEM_CLOCK == 1)    
    // DC-DC clock from system clock divided 8.
    DC0CF = 0x61;
#endif
    
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
// P0.2   digital   open-drain          Switch 1
// P0.3   digital   open-drain          Switch 2
// P1.5   digital   push-pull           DS3
// P1.6   digital   push-pull           DS4
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
    // Switch   
    P0MDIN  |=  0x0C;                   // P0.2, P0.3 are digital
    P0MDOUT &= ~0x0C;                   // P0.2, P0.3 are open-drain
    P0      |=  0x0C;                   // Set P0.2, P0.3 latches to '1'
    P0SKIP  |=  0x0C;                   // P0.2, P0.3 skipped in Crossbar

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
    
    // Select Low-Power Oscillator 20 MHz as system clock source
    
    CLKSEL = 0x04;        
    OSCICN &= ~0x80;
    REG0CN &=~0x10;
    
    // At clock frequencies above 10 MHz, the system clock cycle becomes 
    // short enough that the one-shot timer no longer provides a power benefit.
    // Disabling the one-shot timer at higher frequencies reduces power 
    // consumption. 
    FLSCL = 0x40;
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
// P0.2   digital   open-drain   input   SW0_2
//
//-----------------------------------------------------------------------------
void Voltage_Update(void)
{
    // Cycle DC0 level if P0.2 is pushed
    if( (SW0_2 == SW_PRESSED) && (!switch1Latch) )
    {
        
        if (DS3 == LED_ON)
        {
            DS3 = LED_OFF;              // Turn OFF DS3
        }
            
        if (dc0Direction)
        {
            if (dc0Level-- > LEVEL1)
            {
                DC0CN = dc0Level;
            }
            else
            {
                DC0CN = dc0Level;
                dc0Direction = 0;
                DS3 = LED_ON;           // Turn ON DS3
            }
            
        }
        else
        {
            
            if (dc0Level++ < LEVEL7)
            {
                DC0CN = dc0Level;
            }
            else
            {
                dc0Level = LEVEL6;
                DC0CN = dc0Level;
                dc0Direction = 1;
            }
        }
        // Wait 50 milliseconds
        Wait_MS(50);  
        // Arm latches to prevent auto-cycling with switch held
        switch1Latch = 1;
    }
    
    // Reset latch once switch is released
    else if(SW0_2 == SW_NOT_PRESSED)
    {
        switch1Latch = 0;
    }
}
//-----------------------------------------------------------------------------
// Wait_MS
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters:
//   1) unsigned int ms - number of milliseconds of delay
//                        range is full range of integer: 0 to 256
//
// This routine inserts a delay of <ms> milliseconds.
//-----------------------------------------------------------------------------
void Wait_MS(unsigned int ms)
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
