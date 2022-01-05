//-----------------------------------------------------------------------------
// F96x_Clock_Sleep.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the current consumption of the device when in
// Suspend mode, Sleep mode (with an external 32.768 kHz RTC running), and
// Sleep mode (with no external RTC running).
//
// Refer to F96x_Clock_Sleep_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.4 - Sleep Mode Current):
// -----------------------------------------------
// 1) In "Define Hardware", #define either "UDP_F960_MCU_MUX_LCD" or
//    "UDP_F960_MCU_EMIF" depending on which board you are using.
// 2) Cut the trace on the bottom of the PCB between the two pins of the VBAT
//    CURRENT header (LCD: J15, EMIF: J18).
// 3) Cut the trace on the bottom of the PCB between the two pins of the header
//    J20.
// 4) If using the EMIF board, connect P0.1 to P3.1.
// 5) Connect a current meter between the two pins on the VBAT CURRENT header.
// 6) Set the VBAT Selector SW5 to the desired power source.
// 7) Set VIO Select (SW7) and VIORF Select (SW12) to VBAT.
// 8) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 9) Disconnect the debugger in order to reduce current draw. Apply the
//    the desired power source to run the code.
// 10) The device will initially be running in Suspend mode. Press SW2 to
//    cycle between Suspend, Sleep (with RTC), and Sleep (without RTC)
//    modes.
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
//    - 09 NOV 2013
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
#endif

#ifdef UDP_F960_MCU_EMIF
SI_SBIT(SW2, SFR_P3, 1);
#endif

//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

#define DISABLED 0
#define ENABLED 1

// Set the constant below to 'DISABLED' to disable the VBAT supply monitor on
// 'F96x devices, 'ENABLED' to enable.
#define VBAT_MONITOR                DISABLED

// Set the constant below to use the low frequency oscillator (instead of
// an external 32.768 kHz crystal). On devices that do not have an LFO, the
// SmaRTClock oscillator will be configured into self oscillate mode
#define RTC_LFO                     DISABLED


#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

#define PORT_MATCH_WAKEUP   0x02
#define SUSPEND_MODE        0x40
#define SLEEP_MODE          0x80

//-----------------------------------------------------------------------------
// Mode Variables
//-----------------------------------------------------------------------------

// Mode type enumerations
enum CLOCK_SLEEP_MODE {SUSPEND = 1,
                       SLEEP_RTC = 2,
                       SLEEP = 3};

enum CLOCK_SLEEP_MODE nextMode;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

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
   PCA0MD &= ~0x40;                     // WDTE = 0 (clear watchdog timer
}
 
void Oscillator_Init (void);
void Oscillator_Cycle (void);
void Port_Init (void);
void Enter_Sleep (void);
void RTC_Init (void);
void RTC_Disable(void);
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

                                        // enable)
   // Prevent into sleep, when SW2 pressed after reset processor.
   while(SW2 == 0);

   // Starting Mode: LPOSC suspend mode
   nextMode = SUSPEND;

   // Initialize Port I/O (SW2)
   Port_Init();             
   
   // Initialize Oscillator (Default is 20.0 MHz LPOSC)
   Oscillator_Init();  
   Enter_Sleep();
     
   while (1)
   {
      SFRPAGE = LEGACY_PAGE;
      // Wait until SW2 is pressed and released before going into Idle mode.
      while (SW2 != SW_PRESSED);

        // Cycle to next CPU Mode after System Wake up
        Oscillator_Cycle();

        // Sleep mode, ends upon SW2 Port Match event
        Enter_Sleep();

   } 
}

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to Low Power frequency
// oscillator at the maximum frequency.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
 
#if (VBAT_MONITOR == 1)
    // Enable VBAT Supply monitor
    VDM0CN = 0x08;
    // waiting for VBAT above the VBAT warn monitor threshold
    while (!(VDM0CN & 0x02));
    
    RSTSRC = 0x02;                      // Enable missing clock detector and
                                        // set VBAT Monitor as reset source
#endif

    RTC_Init();                         // Enable SmaRTClock

    OSCICN &= ~0x80;
    REG0CN &=~0x10;
    PMU0CF = 0x20;                      // Clean all Wakeup flag
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins. The switch used
// is configured to wake up from sleep mode and change the CPU Mode
//
// P0.1   digital   open-drain   input   SW2
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    // SW2 (P0.1) Input - Oscillator Clock Cycle Switch
    P0MDIN |= 0x02;                     // P0.1 is digital
    P0MDOUT &= ~0x02;                   // P0.1 is open-drain
    P0 |= 0x02;                         // Set the P0.1 latch to '1'
   
    // Set Port Match settings for S1 (P0.2)
    P0MASK |= 0x02;                     // Enable Port Match check for P0.2
    P0MAT  |= 0x02;                     // Set Port Match value to 1 for P0.2
   
    P1MDIN &= ~0x0C;                    // P1.2 and P1.3 (RTC Pins) are
                                        // analog
    // Crossbar Initialization
    XBR2 = 0x40;                        // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Runtime Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Enter_Sleep
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Puts the processor into sleep mode, in order to allow the user
// to measure Suspend / Sleep mode current draw. 
// System wake up when SW2 (P0.1) is pressed.
//
//-----------------------------------------------------------------------------
void Enter_Sleep (void)
{ 
    // Go into  mode
   switch (nextMode)
   {
        case SUSPEND :
            // Go into suspend mode
            PMU0CF = SUSPEND_MODE | PORT_MATCH_WAKEUP; 
            break;
        
        default:
            // Go into sleep mode
            PMU0CF = SLEEP_MODE | PORT_MATCH_WAKEUP;
            break;
   }
  
   //--sleep mode--//
   //= = = = = = = = = = = = = = =
   //= = = = = = = = = = = = = = =
   //--wake on port match event--//
   PMU0CF = 0x20;
}

//-----------------------------------------------------------------------------
// Oscillator_Cycle
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function sets the CPU Mode to the next setting.
// The process occurs once each time the processor is woken up from sleep
// mode after a port match event happen.
//
// Mode 1: Suspend
// Mode 2: Sleep, SmaRTClock running 
// Mode 3: Sleep, SmaRTClock disable   
//
//-----------------------------------------------------------------------------
void Oscillator_Cycle(void)
{
    // Cycle through oscillator settings
    switch(nextMode)
    {
        case SUSPEND :
            nextMode = SLEEP_RTC;
            break;
      
        case SLEEP_RTC :
            RTC_Disable();
            nextMode = SLEEP;
            break;
                  
        case SLEEP:
            RTC_Init();                 // Enable SmaRTClock.
            nextMode = SUSPEND;
            break;
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
    
    RTC_Write (RTC0XCN, 0x00);          
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
// to operate using a 32.768khZ crystal or low frequency oscillator. Lastly
// it enables the alarm and interrupt. This function uses the RTC primitive 
// functions to access the internal RTC registers.
//
//-----------------------------------------------------------------------------
void RTC_Init (void)
{
    uint16_t i;

    uint8_t CLKSEL_SAVE = CLKSEL;

    RTC0KEY = 0xA5;                     // Unlock the smaRTClock interface
    RTC0KEY = 0xF1;

#if (RTC_LFO == 1)
    RTC_Write (RTC0XCN, 0x08);          // Using the Low Frequency Oscillator
    RTC_Write (RTC0CN, 0x80);           // Enable smaRTClock oscillator
#else
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
#endif    
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
    
    //CLKSEL = 0x03;                    // Select smaRTClock oscillator
                                        // as the system clock
    //OSCICN &= ~0x80;                  // Disable internal oscillator
    
    RTC0KEY = 0x55;                     // Lock smaRTClock interface;
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
