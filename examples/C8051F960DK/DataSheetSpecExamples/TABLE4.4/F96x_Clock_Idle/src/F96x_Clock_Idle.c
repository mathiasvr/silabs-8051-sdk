//-----------------------------------------------------------------------------
// F96x_Clock_Idle.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the current consumption of the High Frequency
// Oscillator, Low Power Oscillator, and Real Time Clock in Idle mode.
//
// Refer to F96x_Clock_Idle_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.4 - Idle Mode Current):
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
// 7) Set VIO Select (SW7) and VIORF(SW12) Select to VBAT.
// 8) To demonstrate Low Power Idle mode, set the constant <LOW_POWER_IDLE>
//    to '1'. To enable peripherals while in Low Power Idle mode, set the
//    constant <PERIPHERALS_ENABLE> to '1'.
//       Note: This example leaves the PMU active to allow interrupts to wake
//             the device. This will consume more current, however. To disable
//             the PMU, set the constant <PMU_ENABLED> to '0'. The example will
//             no longer be able to cycle through power modes, the desired
//             power mode must then be set by changing the constant
//             <INITIAL_MODE>.
//
// 9) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 10) Disconnect the debugger in order to reduce current draw. Apply the
//    the desired power source to run the code.
// 11) The device will initially be running off the HFOSC (24.5 MHz) in Idle
//    mode. To cycle between HFOSC (24.5 MHz), LPOSC (20 MHz) and the RTC
//    (32.768 kHz), press SW2.
//
//
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Added support for the EMIF board.
//    - 12 FEB 2014
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
#endif

#ifdef UDP_F960_MCU_EMIF
SI_SBIT(SW2, SFR_P3, 1);
#endif

//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

//Set to '1' to enable Low Power Idle mode.
#define LOW_POWER_IDLE       0

//Set to '1' to enable Peripherals while in Low Power Idle mode.
#define PERIPHERALS_ENABLED  0

//Set to '0' to disable the PMU during Low Power Idle mode.
// WARNING: This will remove the device's ability to cycle through power modes.
// Set <INITIAL_MODE> in this case to the desired mode.
#define PMU_ENABLED          1

// Mode type enumerations
enum CLOCK_IDLE_MODE
{
   HFOSC,   //24.5 MHz High-Frequency Oscillator
   LPOSC,   //20 MHz Low-Power Oscillator
   RTC      //32.768 kHz Real Time Clock
};

#define INITIAL_MODE         HFOSC

#define SW_PRESSED           0
#define SW_NOT_PRESSED       1
//-----------------------------------------------------------------------------
// Mode Variables
//-----------------------------------------------------------------------------
enum CLOCK_IDLE_MODE nextMode = INITIAL_MODE;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(PortMatch_ISR,PMATCH_IRQn);

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
  // Disable the watchdog here
}
 
void Oscillator_Init (void);
void Oscillator_Cycle (void);
void Port_Init (void);
void Enter_Idle (void);
void RTC_Init (void);
void RTC_Disable(void);
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   // Disable watchdog timer
   PCA0MD = 0x00;
   VDM0CN = 0x00;                      // disable both VDD monitors

   // Initialize Port I/O (SW0.2)
   Port_Init();

   // Initialize Oscillator (Default is 24.5 MHz PROSC)
   Oscillator_Init();
   Oscillator_Cycle();

   // Idle mode, ends upon SW0_2 Port Match event
   Enter_Idle();

   while (1) {
      SFRPAGE = LEGACY_PAGE;
      // Wait until SW2 is pressed and released before going into Idle mode.
      while (SW2 != SW_PRESSED);
      while (SW2 == SW_PRESSED);

      // Cycle to next oscillator frequency after wake up
      Oscillator_Cycle();

      // Idle mode, ends upon SW0_2 Port Match event
      Enter_Idle();
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
// This function configures the crossbar and ports pins. The switch used
// is configured to both wake up from sleep mode and change the oscillator
// operating frequency.
//
// P0.1   digital   open-drain   input   SW2
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
#ifdef UDP_F960_MCU_MUX_LCD
    // SW2 (P0.1) Input - Oscillator Clock Cycle Switch
    P0MDIN |= 0x02;                     // P0.1 is digital
    P0MDOUT &= ~0x02;                   // P0.1 is open-drain
    P0 |= 0x02;                         // Set the P0.1 latch to '1'
#endif

#ifdef UDP_F960_MCU_EMIF
    // SW2 (P3.1) Input - Oscillator Clock Cycle Switch
    P3MDIN |= 0x02;                     // P3.1 is digital
    P3MDOUT &= ~0x02;                   // P3.1 is open-drain
    P3 |= 0x02;                         // Set the P3.1 latch to '1'
#endif

    // Set Port Match settings for P0.1
    P0MASK |= 0x02;                     // Enable Port Match check for P0.1
    P0MAT  |= 0x02;                     // Set Port Match value to 1 for P0.1
   
    P1MDIN &= ~0x0C;                    // P1.2 and P1.3 (RTC Pins) are
                                        // analog
    // Crossbar Initialization
    XBR2 = 0x40;                        // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Runtime Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Enter_Idle
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Puts the processor into Idle mode, in order to allow the user
// to measure idle mode current draw. Idle mode ends when SW2 (P0.1) is
// pushed.
//
//-----------------------------------------------------------------------------
void Enter_Idle (void)
{
    uint8_t IE_save;
    uint8_t EIE2_save;
    SFRPAGE = CONFIG_PAGE;
    FLSCL     = 0;        
    SFRPAGE = LEGACY_PAGE;
   
    IE_EA = 0;                         // Clear global interrupt enable
    PMU0CF = 0x20;
    // Save interrupt state
    IE_save = IE;                      // Preserve IE register
    EIE2_save = EIE2;                  // Preserve EIE1 register
    IE = 0x00;                         // Disable interrupts
   
    EIE2 |= EIE2_EMAT__ENABLED;        // Enable port match interrupts
    IE_EA = 1;                         // Enable global interrupts

    // Go into idle mode
#if (LOW_POWER_IDLE == 1) 
    SFRPAGE = CONFIG_PAGE;
    
#if (PERIPHERALS_ENABLED == 1)
    PCLKEN = 0x0F;
#else
   #if (PMU_ENABLED == 1)
       PCLKEN = 0x08;
   #else
       PCLKEN = 0x00;
   #endif
#endif
    
    CLKMODE = 0x04;                     // Low Power Idle mode
    SFRPAGE = LEGACY_PAGE;
#endif
   
    PCON |= 0x01;                       // Idle mode
    PCON = PCON;                        // Follow the instruction to enter idle
                                        // with a 3 cycle dummy instruction
 					
    //--Idle mode--//
    //--wake on port match event--//

    EIE2 = EIE2_save;                   // Restore ext interrupts
    IE = IE_save;                       // Restore interrupts
    CLKMODE = 0x00;
    PMU0CF = 0x20;
    IE_EA = 1;                             // Set global interrupt enable

}

//-----------------------------------------------------------------------------
// Oscillator_Cycle
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function sets the internal oscillator to the next frequency setting.
// The process occurs once each time the processor is woken up from sleep
// mode after a port match interrupt.
//
// Mode 1: Precision Oscillator,  24.5 MHz
// Mode 3: Low-Power Oscillator,  20 MHz
// Mode 3: SmaRTClock Oscillator, 32.768 kHz   
//
//-----------------------------------------------------------------------------
void Oscillator_Cycle(void)
{
    // Cycle through oscillator settings
    switch(nextMode)
    {
        case HFOSC :
            SFRPAGE = CONFIG_PAGE;
            FLSCL     = 0x40;           // Flash Read Timing One-Shot Bypass.            
            SFRPAGE = LEGACY_PAGE;
            
            REG0CN |= 0x10;
            OSCICN |= 0x80;
            CLKSEL = 0x00;              // Enable PROSC
            RTC_Disable();
            nextMode = LPOSC;
            break;
      
        case LPOSC :
            CLKSEL = 0x04;              // Switch to LPOSC
            OSCICN &= ~0x80;
            REG0CN &=~0x10;
            nextMode = RTC;
            break;
                  
        case RTC:
            RTC_Init();                 // Switch to SmaRTC
            nextMode = HFOSC;
            break;
    }
   
}
//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port Match ISR
//-----------------------------------------------------------------------------
//
// This interrupt wakes the processor from idle mode, and proceeds to continue
// to the Oscillator_Cycle() function.
//
// The Port Match interrupt occurs whenever (Pn & PnMASK) != (PnMAT & PnMASK).
// We must disable the interrupt and wait until the switches are released in
// main, or else the interrupt will trigger multiple times.  It is generally
// not good practice to sit in an ISR for int32_t periods of time.
//
// P0.1   digital   open-drain   input   SW2 - Port Match Trigger
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PortMatch_ISR,PMATCH_IRQn)
{

   EIE2 &= ~0x02;                       // Disable Port Match interrupts to
                                        // prevent multiple interrupts from
                                        // occurring while the switches are
                                        // pressed
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
