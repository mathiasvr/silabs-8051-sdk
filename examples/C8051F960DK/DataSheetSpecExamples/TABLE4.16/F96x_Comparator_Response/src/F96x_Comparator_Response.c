//-----------------------------------------------------------------------------
// F96x_Comparator_Response.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the four response-time modes of the comparator
// (CP0), as specified on table 4.16 of the datasheet.
//
//
// Refer to F96x_Comparator_Response_Readme.txt for more information regarding
// the functionality of this code example.
//
// How To Test (Table 4.16 - Response Time):
// ---------------------------------------------------------
// 1) Ensure the pins of VBAT CURRENT (J15) are shorted.
// 2) Power the board using the P1 power connector.
// 3) Remove all of the shorting blocks on the board. 
// 4) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) To cycle between Comparator modes, push SW3 (P0.2). LED1 and LED2
//    will indicate the comparator mode as below:
//
//     LED2  |  LED1 |  CP0 Mode 
//    -------------------------------------
//      OFF  |  OFF  |   Mode 0
//      OFF  |  ON   |   Mode 1
//      ON   |  OFF  |   Mode 2
//      ON   |  ON   |   Mode 3
//
// 7) Attach oscilloscope probes to P0.6 and P0.3.
// 8) To cycle between CP0 Positive Input VDD/GND, push SW4 (P0.3).The length
//    from P0.3 toggled to P0.6 toggled is the response time as shown on table
//    4.16 of the datasheet.
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:    None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 6 FEB 2014
//
// Release 1.0
//    - Initial Revision (MT)
//    - 05 NOV 2013
//
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include "SI_C8051F960_Register_Enums.h"

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(LED1,   SFR_P0, 0);               // '0' means ON, '1' means OFF
SI_SBIT(LED2,   SFR_P0, 1);               // '0' means ON, '1' means OFF
SI_SBIT(SW3,    SFR_P0, 2);               // SW3 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK              20000000    // Clock speed in Hz
#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

#define LED_ON              0
#define LED_OFF             1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);                // Configure the system clock
void PORT_Init (void);                  // Configure the Crossbar and GPIO
void Wait_MS(uint16_t ms);          // Inserts programmable delay
void Mode_Update(void);                 // Cycle CP0 mode or turn on/off CP0
                                        // when push SW0_2 or SW0_3
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Debounce latches for SW0.2 
uint8_t switch1Latch = 0; 

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
 
// Mode type enumerations
enum COMPARATOR_MODE {MODE0 = 0,
                      MODE1 = 1,
                      MODE2 = 2,
                      MODE3 = 3};
						
enum COMPARATOR_MODE comparatorMode;
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   

    PORT_Init ();                       // Initialize crossbar and GPIO
    SYSCLK_Init();                      // Initialize the system clock
 
    // Default Comparator0 enabled as Mode0
    comparatorMode = MODE0;
    //CPT0CN = 0x80;
    CPT0MD = MODE0;

    // Comparator0 Negative Input P0.3
    // Comparator0 Positive Input 1/2 VDD
    CPT0MX = 0x1D;
    // CP0 enable
    CPT0CN = 0x80;
    while(1)
    {  
        Mode_Update();
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
// P0.1   digital   push-pull           LED2
// P0.2   digital   open-drain          SW3
// P0.3   analog    Comparator0         Negative Input
// P0.6   digital   push-pull           CP0 Output
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
    SFRPAGE = LEGACY_PAGE;
    // Analog CP0 negative input 
    P0MDIN  &= ~0x08;                   // P0.3 is analog
    P0MDOUT &= ~0x08;                   // P0.3 is  open-drain
    P0      |=  0x08;                   // Set P0.3 latches to '1'
    P0SKIP  |=  0x08;                   // P0.3 skipped in Crossbar
    
    // Switch, LED, CP0 output 
    P0MDIN  |=  0x4F;                   // P0.0,P0.1,P0.2, P0.3, P0.6 digital
    P0MDOUT &= ~0x04;                   // P0.2 is  open-drain
    P0      |=  0x4F;                   // Set P0.0 to P0.3 latches to '1'
    P0SKIP  |=  0x3F;                   // Enable crossbar P0.6 
    
    XBR0 = 0x10;                        // CP0 output available at P0.6
    XBR2 = 0x40;                        // Enable crossbar and enable
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
    FLSCL     = 0x40;                   // Flash Read Timing One-Shot Bypass.
    SFRPAGE = LEGACY_PAGE;
    
    // Select Low-Power Oscillator 20 MHz as system clock source
    // Since using the precision internal oscillator as the system clock source, 
    // the precision voltage reference should not be enabled from a disabled 
    // state. 
    CLKSEL = 0x04;        
    OSCICN &= ~0x80;
    REG0CN &=~0x10;
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Mode_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function checks switch input and select CP0 mode or turn on/off CP0
// once each time the switch is pressed. 
// The cycling order is as follows:
//
// Switch Sw3:
// Mode 0: CP0 Mode 0
// Mode 1: CP0 Mode 1
// Mode 2: CP0 Mode 2
// Mode 3: CP0 Mode 3
//
// Switch SW4:
// Toggle CP0 Negative input VDD/GND 
//
// P0.2   digital   open-drain   input   SW3
//
//-----------------------------------------------------------------------------
void Mode_Update(void)
{
    // Cycle CP0 mode if P0.2 is pushed
    if( (SW3 == SW_PRESSED) && (!switch1Latch) )
    {
        switch (comparatorMode)
        {
            case MODE0:
                comparatorMode = MODE1;
                
                CPT0MD = MODE1;
                LED1 = LED_ON;           // Turn ON LED1
                LED2 = LED_OFF;          // Turn OFF LED2
                break;
                
            case MODE1:
                comparatorMode = MODE2;
               
                CPT0MD = MODE2;
                LED1 = LED_OFF;          // Turn OFF LED1
                LED2 = LED_ON;           // Turn ON LED2
                break;

            case MODE2:
                comparatorMode = MODE3;
               
                CPT0MD = MODE3;
                LED1 = LED_ON;           // Turn ON LED1
                LED2 = LED_ON;           // Turn ON LED2
                break;

            case MODE3:
                comparatorMode = MODE0;
                
                CPT0MD = MODE0;
                LED1 = LED_OFF;          // Turn OFF LED1    
                LED2 = LED_OFF;          // Turn OFF LED2
                break;
        }
        CPT0CN = 0x80;
        
        // Arm latches to prevent auto-cycling with switch held
        switch1Latch = 1;
    }
    
    // Reset latch once switch is released
    else if(SW3 == SW_NOT_PRESSED)
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
