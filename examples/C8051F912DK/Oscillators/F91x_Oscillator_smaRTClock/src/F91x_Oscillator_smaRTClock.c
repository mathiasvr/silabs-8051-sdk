//-----------------------------------------------------------------------------
// F91x_Oscillator_smaRTClock.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to use the smaRTClock oscillator as a low
// frequency system clock.
//
//
// How To Test:
//
// 1) Ensure that a 32.768 kHz crystal is installed at XTAL3 and XTAL4, or
//    crystal Y2 is populated.
// 2) Ensure that jumpers are placed on the following:
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 3) Connect the USB Debug Adapter to J4.
// 4) Turn the POWER switch (SW5) to the "ON" position.
// 5) Specify the crystal frequency in the constant <CRYSTAL_FREQUENCY>.
// 6) Compile and download code to a 'F91x device on a C8051F91x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) Measure the frequency output on P0.0.
//
//
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
// Release 1.1a
//    - Compile and test C8051F912_defs.h on 912 board (JH)
//    - 07 JULY 2009
//
// Release 1.1
//    - Updated SmaRTClock Init Procedure (FB)
//    - 15 AUG 2008
//
// Release 1.0
//    - Initial Revision (FB)
//    - 15 JAN 2007

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void PORT_Init (void);
void RTC_Init (void);
void OSCILLATOR_Init (void);

uint8_t RTC_Read (uint8_t);
void RTC_Write (uint8_t, uint8_t);

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

   PORT_Init();                        // Initialize Port I/O
   RTC_Init ();                        // Initialize RTC
   OSCILLATOR_Init ();                 // Initialize Oscillator
   

   while (1);                          // Infinite Loop

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
//
// P0.0   digital    push-pull      /SYSCLK
//-----------------------------------------------------------------------------
void PORT_Init (void)
{

   // Buffered System Clock Output
   P0MDOUT |= 0x03;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0    = 0x08;                     // Route /SYSCLK to first available pin
   XBR2    = 0x40;                     // Enable Crossbar and weak pull-ups
   
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
   for (i=0x40;i!=0;i--);  
   
   PMU0CF = 0x20;                      // Clear PMU wake-up source flags

   CLKSEL = CLKSEL_SAVE;               // Restore system clock
   while(!(CLKSEL & 0x80));            // Poll CLKRDY
  
}

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the smaRTClock oscillator.
// 
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{

      
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.
   
   CLKSEL &= ~0x70;                    // Specify a clock divide value of 1
   
   while(!(CLKSEL & 0x80));            // Wait for CLKRDY to ensure the 
                                       // divide by 1 has been applied

   CLKSEL = 0x03;                      // Select smaRTClock oscillator 
                                       // as the system clock

}


//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

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
// End Of File
//-----------------------------------------------------------------------------
