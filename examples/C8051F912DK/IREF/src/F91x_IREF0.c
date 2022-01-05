//-----------------------------------------------------------------------------
// F91x_IREF0.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This software shows how to enable and configure the current reference 
// to source current in Low Power Mode, where each step is 1uA and the
// maximum current is 63uA. When R14 is populated with a 10K Ohm resistor,
// the voltage on P0.7/IREF0 should vary between 0V and 0.63V. To keep the 
// output voltage/current constant, press the P0.2 switch.
//
// The compile option <CURRENT_MODE> may be set to "HIGH_CURRENT" to place
// IREF0 in High Current mode, where each step is 8uA and the maximum 
// current is 504uA. In this mode, the voltage should vary between 0V and 0.95V
// when a 10K Ohm resistor is used at the IREF0 output.
//
// Pinout:
//
// P0.2 - Switch
// P0.7 - IREF0 output
// P1.5 - Red LED
// P1.6 - Yellow LED
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J8:   P0.2/SW0.2
//       J7:   P0.7/IREF
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Ensure that C19 and R14 are populated.
// 2) Connect the USB Debug Adapter to J4.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F91x device on a C8051F91x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 4) Place voltmeter or oscilloscope on P0.7/IREF0.
// 5) To hold the voltage/current at its current value, press the
//    P0.2 Switch.
//
// Target:         C8051F90x-C8051F91x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 14 JAN 2014
//
// Release 1.2 
//    - Compiled and tested for Raisonance Toolchain (FB)
//    - 17 MAY 10
//
// Release 1.1
//    - Compile and test C8051F912_defs.h on 912 board (JH)
//    - Port to C8051F912-TB from C8051F930-TB (JH)
//    - 06 JULY 2009
//
// Release 1.0
//    -Initial Revision (FB)
//    -15 AUG 2007
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(RED_LED,    SFR_P1, 5);          // '0' means ON, '1' means OFF
SI_SBIT(YELLOW_LED, SFR_P1, 6);          // '0' means ON, '1' means OFF
SI_SBIT(SW2,        SFR_P0, 2);          // SW2 == 0 means switch pressed
SI_SBIT(SW3,        SFR_P0, 3);          // SW3 == 0 means switch pressed


//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             20000000    // Clock speed in Hz

#define LED_ON                0
#define LED_OFF               1

#define LOW_POWER             0
#define HIGH_CURRENT          1

#define CURRENT_MODE       LOW_POWER   // "LOW_POWER" or "HIGH_CURRENT"

#if(CURRENT_MODE == HIGH_CURRENT)      // Set the maximum output code depending
   #define MAX_VALUE  0x0C             // on the currently selected current
#else                                  // mode
   #define MAX_VALUE  0x3F
#endif 

#define MIN_VALUE     0x00             // Set the minimum output code

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);           // Configure the system clock
void PORT_Init (void);                 // Configure the Crossbar and GPIO
void IREF0_Init (void);                // Configure IREF0
void Wait_MS(uint16_t ms);         // Inserts programmable delay

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
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   bit rising = 1;
   

   PORT_Init ();                       // Initialize crossbar and GPIO
   OSCILLATOR_Init();                  // Initialize the system clock
   IREF0_Init();                       // Initialize IREF0

   while(1)
   {  
      // If P0.2 Switch is not pressed
      if(SW2)
      {  
         //----------------------------------------
         // Set LED State
         //----------------------------------------
         
         // Turn off the Yellow LED
         YELLOW_LED = LED_OFF;
         
         //----------------------------------------
         // Control output voltage/current
         //----------------------------------------

         // If output voltage should be increasing
         if(rising)
         {
            // Increment the output current
            IREF0CN++;
            
            // If we have reached the maximum value
            if((IREF0CN & 0x3F) == MAX_VALUE)
            {
               // Change Direction
               rising = 0;
            }
         } else
         
         // Output voltage should be decreasing 
         {  
            // Decrement the output current
            IREF0CN--;
            
            // If we have reached the minimum value
            if((IREF0CN & 0x3F) == MIN_VALUE)
            {
               // Change Direction
               rising = 1;
            }
         }

         //----------------------------------------
         // Insert Delay
         //----------------------------------------

         Wait_MS(50);                  // Wait 50 milliseconds
      
      } else
      
      // P0.2 switch is pressed
      {

         //----------------------------------------
         // Set LED State
         //----------------------------------------

         // Turn on the Yellow LED
         YELLOW_LED = LED_ON;

         //----------------------------------------
         // Control output voltage/current
         //----------------------------------------
         
         // Keep the current value as-is.

         //----------------------------------------
         // Insert Delay
         //----------------------------------------

         // There is no need to insert a delay.

      }

   
   } // end while(1)

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
// P0.2   digital   open-drain/High-Z    Switch 2
// P0.3   digital   open-drain/High-Z    Switch 3
// P0.7   analog    open-drain/High-Z    IREF0

// P1.5   digital   push-pull            Red LED
// P1.6   digital   push-pull            Yellow LED
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   
   // IREF0 Analog Output
   P0MDIN  &= ~0x80;                   // P0.7 is analog
   P0MDOUT &= ~0x80;                   // P0.7 is open-drain
   P0      |=  0x80;                   // Set P0.7 latch to '1' 
   P0SKIP  |=  0x80;                   // P0.7 skipped in Crossbar
   
   // Switches   
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
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal low power
// oscillator at its maximum frequency.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   CLKSEL = 0x04;                     // Select low power oscillator with a 
                                      // clock divider value of 1 (20MHz)
}



//-----------------------------------------------------------------------------
// IREF0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the current reference to source current in 
// Low Power Mode, where each step is 1uA and the maximum current is 63uA.
//
//-----------------------------------------------------------------------------
void IREF0_Init (void)
{
  
   #if(CURRENT_MODE == HIGH_CURRENT)
   
      IREF0CN = 0x40;                  // Configure IREF0 as a current source
                                       // in High Current mode

   #else
      
      IREF0CN = 0x00;                  // Configure IREF0 as a current source
                                       // in Low Power mode
   #endif
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Wait_MS
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters:
//   1) uint16_t ms - number of milliseconds of delay
//                        range is full range of integer: 0 to 65335
//
// This routine inserts a delay of <ms> milliseconds.
//-----------------------------------------------------------------------------
void Wait_MS(uint16_t ms)
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
