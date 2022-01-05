//-----------------------------------------------------------------------------
// F91x_IREF0_Power.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the Power consumption of different Current Reference
// modes and different IREF0DAT on datasheet for the IREF0 in Table 4.13.
//
// Refer to F91x_IREF0_Power_Readme.txt for more information regarding
// the functionality of this code example.
//
// How To Test (Table 4.13 - Power Supply Current):
// ---------------------------------------------------------
// 1) Ensure a shorting block is placed the J11 (VBAT & WALL_PWR) .
// 2) Place VDD Select (2 CELL) in the SW4 position and power the board
//    using the P2 power connector.
// 3) Remove the J17 shorting block and attach an ammeter between
//    the Imeasure headers.
// 4) Remove all of the shorting blocks on the board except for the block(s)
//    across the following pins:
//    a) LED1.5 and P1.5 on J8
//    b) LED1.6 and P1.6 on J8
//    c) SW0.2 and P0.2H on J8
//    d) SW0.3 and P0.3H on J8
// 5) Default is current source mode, if user want to measure current sink 
//    mode, set #define SINK_MODE 1
// 6) Download and run the code on the target board.
// 7) To cycle between Current Reference mode, push SW0.2 (P0.2).DS3 and DS4
//    could indicate current reference mode as below :
//
//      DS4  |  DS3  |  Reference Mode 
//    -------------------------------------
//      OFF  |  OFF  |   Low  Minimum   (IREF0CN = 00000001)
//      OFF  |  ON   |   Low  Maximum   (IREF0CN = 00111111)
//      ON   |  OFF  |   High Minimum   (IREF0CN = 01000001)
//      ON   |  ON   |   High Maximum   (IREF0CN = 01111111)
//
// 8) To cycle between Reference ON / OFF, push SW0.3 (P0.3).Record the 
//    current each time. The difference is the power supply current on 
//    datasheet for the IREF0 in Table4.13
//
// Target:          C8051F91x/90x
// Tool Chain:      Generic
// Command Line:    None
//
//
// Release 1.0
//    - Initial Revision (MT)
//    - 26 OCT 2013
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
SI_SBIT(DS4,   SFR_P1, 6);                // '0' means ON, '1' means OFF
SI_SBIT(SW0_2, SFR_P0, 2);                // SW2 == 0 means switch pressed
SI_SBIT(SW0_3, SFR_P0, 3);                // SW3 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK              20000000    // Clock speed in Hz
#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

#define LED_ON              0
#define LED_OFF             1
#define MAX_VALUE           0x3F
#define MIN_VALUE           0x01        // Set the minimum output code

#define SINK_MODE           0
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);                // Configure the system clock
void PORT_Init (void);                  // Configure the Crossbar and GPIO
void Wait_MS(unsigned int ms);          // Inserts programmable delay
void Mode_Update(void);                 // Cycle Reference mode,push SW0_2
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Debounce latches for SW0.2 
uint8_t switch1Latch = 0; 
// Debounce latches for SW0.3 
uint8_t switch2Latch = 0; 

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
 
uint8_t iref0Save;
// Mode type enumerations
enum REFERENCE_MODE {LOW_CURRENT_MIN  = 1,
                     LOW_CURRENT_MAX  = 2,
                     HIGH_CURRENT_MIN = 3,
                     HIGH_CURRENT_MAX = 4};
						
enum REFERENCE_MODE referenceMode;
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   

    PORT_Init ();                       // Initialize crossbar and GPIO
    SYSCLK_Init();                      // Initialize the system clock
 
    referenceMode = LOW_CURRENT_MIN;
    IREF0CN = MIN_VALUE;                // Configure IREF0 as a current source
                                        // in High Power mode

#if (SINK_MODE == 1) 
    IREF0CN |= 0x80;                    // Set Current Sink mode
#endif    
    
    iref0Save = IREF0CN;
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
// P0.2   digital   open-drain    Switch 1
// P0.3   digital   open-drain    Switch 2
// P0.7   analog    open-drain    IREF0

// P1.5   digital   push-pull     DS3
// P1.6   digital   push-pull     DS4
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   
   // IREF0 Analog Output
   P0MDIN  &= ~0x80;                   // P0.7 is analog
   P0MDOUT &= ~0x80;                   // P0.7 is open-drain
   P0      |=  0x80;                   // Set P0.7 latch to '1' 
   P0SKIP  |=  0x80;                   // P0.7 skipped in Crossbar
   
   // Switch   
   P0MDIN  |=  0x0C;                   // P0.2, P0.3 are digital
   P0MDOUT &= ~0x0C;                   // P0.2, P0.3 are open-drain
   P0      |=  0x0C;                   // Set P0.2, P0.3 latches to '1'
   P0SKIP  |=  0x0C;                   // P0.2, P0.3 skipped in Crossbar

   // LEDs
   P1MDIN  |= 0x60;                    // P1.5, P1.6 are digital
   P1MDOUT |= 0x60;                    // P1.5, P1.6 are push-pull
   P1      |= 0x60;                    // Set P1.5, P1.6 latches to '1'
   P1SKIP  |= 0x60;                    // P1.5, P1.6 skipped in Crossbar


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
    // Since using the precision internal oscillator as the system clock source, 
    // the precision voltage reference should not be enabled from a disabled 
    // state. 
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
// Mode_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function checks switch input and select IREF0 mode ,
// once each time the switch is pressed. 
// The cycling order is as follows:
//
// Switch Sw0_2:
// Mode 1: Low current minimum
// Mode 2: Low current maximum
// Mode 3: High current minimum
// Mode 4: High current maximum
//
// Switch SW0_3:
// Toggle Reference ON / OFF 
//
// P0.2   digital   open-drain   input   SW0_2
// P0.3   digital   open-drain   input   SW0_3
//
//-----------------------------------------------------------------------------
void Mode_Update(void)
{
    // Cycle Reference mode if P0.2 is pushed
    if( (SW0_2 == SW_PRESSED) && (!switch1Latch) )
    { 
        switch (referenceMode)
        {
            case LOW_CURRENT_MIN:
                
                referenceMode = LOW_CURRENT_MAX;
                // Low current maximum value 
                IREF0CN = MAX_VALUE;
                DS3 = LED_ON;           // Turn ON  DS3
                DS4 = LED_OFF;          // Turn OFF DS4
                break;
                
            case LOW_CURRENT_MAX:
                
                referenceMode = HIGH_CURRENT_MIN;
                // High current minimum value 
                IREF0CN = MIN_VALUE | 0x40;
                DS3 = LED_OFF;
                DS4 = LED_ON;           // Turn ON DS4
                break;
            
            case HIGH_CURRENT_MIN:
                referenceMode = HIGH_CURRENT_MAX;
                
                // High current minimum value 
                IREF0CN = MAX_VALUE | 0x40;
                DS3 = LED_ON;           // Turn ON DS3
                DS4 = LED_ON;           // Turn ON DS4
                break;
                
            case HIGH_CURRENT_MAX:
                referenceMode = LOW_CURRENT_MIN;
                
                // Low current minimun value 
                IREF0CN = MIN_VALUE;
                DS3 = LED_OFF;
                DS4 = LED_OFF;          // Turn ON DS4
                break;    
        }
#if (SINK_MODE == 1) 
        IREF0CN |= 0x80;                // Set Current Sink mode
#endif          
        iref0Save = IREF0CN;
        Wait_MS(50);                    // Wait 50 milliseconds
        // Arm latches to prevent auto-cycling with switch held
        switch1Latch = 1;
    }

    // Reset latch once switch is released
    else if(SW0_2 == SW_NOT_PRESSED)
    {
        switch1Latch = 0;
    }
    
    // Cycle Reference ON / OFF if P0.3 is pushed
    if( (SW0_3 == SW_PRESSED) && (!switch2Latch) )
    {
        if (IREF0CN)
        {
            // Output voltage be decreasing to 0  
            IREF0CN = 0;
        }
        else
        {
            IREF0CN = iref0Save;
        }
        // Wait 50 milliseconds
        Wait_MS(50);                   
        switch2Latch = 1;
    }  
    
    // Reset latch once switch is released
    else if(SW0_3 == SW_NOT_PRESSED)
    {
        switch2Latch = 0;
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
   char i;

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
