//-----------------------------------------------------------------------------
// F96x_Oscillator_smaRTClock.c
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
// 1) Ensure the 32.768 kHz quartz crystal is installed on the F960 Pico Card.
// 2) Connect the USB Debug Adapter to J13.
// 3) Move the VBAT SEL switch (SW5) to the middle VREG position.
// 4) Move the VIO switch (SW7) and VIORF switch (SW12) to the VBAT position.
// 5) Connect a 9V power adapter to P1 of the development board.
// 5) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) Measure the frequency output on P0.0 to be approximately 32.768 kHz.
//
//
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 20 JAN 2014
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F960_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);

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
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   P1MDIN &= ~0x0C;                    // P1.2 and P1.3 (RTC Pins) are
                                       // analog
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


   RTC_Write (RTC0XCF, 0x8A);          // Enable Automatic Load Capacitance
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
// RTC_Write ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : reg, value
//
// This function will Write one byte from the specified RTC register.
// Using a register number greater that 0x0F is not permited,
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
// Using a register number greater that 0x0F is not permited,
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
// End Of File
//-----------------------------------------------------------------------------
