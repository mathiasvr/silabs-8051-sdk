//-----------------------------------------------------------------------------
// F96x_IREF0_Power.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the power consumption of the internal current
// reference (IREF0), as specified on table 4.15 of the datasheet.
//
// Refer to F96x_IREF0_Power_Readme.txt for more information regarding
// the functionality of this code example.
//
// How To Test (Table 4.15 - Power Supply Current):
// ---------------------------------------------------------
// 1) Place VDD Select VREG in the SW5 position and power the board
//    using the P1 power connector.
// 2) Cut the trace between two pins of J15, solder a jumper, attach an ammeter
//    between the Imeasure headers.
// 3) Place a shorting block on P0.3 - LED/SW4 of J16.
// 4) To change between current sink / current source modes, change
//    #define SINK_MODE to 1.
// 5) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) To cycle between Current Reference mode, push SW3 (P0.2).LED1 and LED2
//    could indicate current reference mode as below :
//
//     LED2  |  LED1 |  Reference Mode 
//    -------------------------------------
//      OFF  |  OFF  |   Low  Minimum   (IREF0CN = 00000001)
//      OFF  |  ON   |   Low  Maximum   (IREF0CN = 00111111)
//      ON   |  OFF  |   High Minimum   (IREF0CN = 01000001)
//      ON   |  ON   |   High Maximum   (IREF0CN = 01111111)
//
// 8) To cycle between Reference ON / OFF, push SW4 (P0.3).Record the
//    current each time. The difference is the power supply current on 
//    datasheet for the IREF0 in Table 4.15
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
//    - 19 OCT 2013
//
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F960_Register_Enums.h>

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(LED1,   SFR_P0, 0);                // '0' means ON, '1' means OFF
SI_SBIT(LED2,   SFR_P0, 1);                // '0' means ON, '1' means OFF
SI_SBIT(SW3,    SFR_P0, 2);                // SW3 == 0 means switch pressed
SI_SBIT(SW4,    SFR_P0, 3);                // SW4 == 0 means switch pressed

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
void Mode_Update(void);                 // Cycle Reference mode,push SW3
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Debounce latches for SW3 
uint8_t switch1Latch = 0; 
// Debounce latches for SW4 
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
// P0.0   digital   push-pull     LED1
// P0.1   digital   push-pull     LED2
//
// P0.2   digital   open-drain    SW3
// P0.3   digital   open-drain    SW4
// P0.7   analog    open-drain    IREF0
//
//
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
   P0MDIN  |= 0x03;                    // P0.0, P0.1 are digital
   P0MDOUT |= 0x03;                    // P0.0, P0.1 are push-pull
   P0      |= 0x03;                    // Set P0.0, P0.1 latches to '1'
   P0SKIP  |= 0x03;                    // P0.0, P0.0 skipped in Crossbar


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
    FLSCL     = 0x40;               // Set BYPASS bit for >12.5 MHz.
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
// This function checks switch input and select IREF0 mode ,
// once each time the switch is pressed. 
// The cycling order is as follows:
//
// Switch Sw3:
// Mode 1: Low current minimum
// Mode 2: Low current maximum
// Mode 3: High current minimum
// Mode 4: High current maximum
//
// Switch SW4:
// Toggle Reference ON / OFF 
//
// P0.2   digital   open-drain   input   SW3
// P0.3   digital   open-drain   input   SW4
//
//-----------------------------------------------------------------------------
void Mode_Update(void)
{
    // Cycle Reference mode if P0.2 is pushed
    if( (SW3 == SW_PRESSED) && (!switch1Latch) )
    { 
        switch (referenceMode)
        {
            case LOW_CURRENT_MIN:
                
                referenceMode = LOW_CURRENT_MAX;
                // Low current maximum value 
                IREF0CN = MAX_VALUE;
                LED1 = LED_ON;          // Turn ON  LED1
                LED2 = LED_OFF;         // Turn OFF LED2
                break;
                
            case LOW_CURRENT_MAX:
                
                referenceMode = HIGH_CURRENT_MIN;
                // High current minimum value 
                IREF0CN = MIN_VALUE | 0x40;
                LED1 = LED_OFF;
                LED2 = LED_ON;          // Turn ON LED2
                break;
            
            case HIGH_CURRENT_MIN:
                referenceMode = HIGH_CURRENT_MAX;
                
                // High current minimum value 
                IREF0CN = MAX_VALUE | 0x40;
                LED1 = LED_ON;          // Turn ON LED1
                LED1 = LED_ON;          // Turn ON LED2
                break;
                
            case HIGH_CURRENT_MAX:
                referenceMode = LOW_CURRENT_MIN;
                
                // Low current minimum value 
                IREF0CN = MIN_VALUE;
                LED1 = LED_OFF;
                LED2 = LED_OFF;         // Turn ON LED2
                break;    
        }
#if (SINK_MODE == 1) 
        IREF0CN |= 0x80;                // Set Current Sink mode
#endif            
        iref0Save = IREF0CN;
        // Arm latches to prevent auto-cycling with switch held
        switch1Latch = 1;
    }

    // Reset latch once switch is released
    else if(SW3 == SW_NOT_PRESSED)
    {
        switch1Latch = 0;
    }
    
    // Cycle Reference ON / OFF if P0.3 is pushed
    if( (SW4 == SW_PRESSED) && (!switch2Latch) )
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
           
        switch2Latch = 1;
    }  
    
    // Reset latch once switch is released
    else if(SW4 == SW_NOT_PRESSED)
    {
        switch2Latch = 0;
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
