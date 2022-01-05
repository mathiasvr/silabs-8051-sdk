//-----------------------------------------------------------------------------
// F96x_Clock_External.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This program demonstrates the current consumption of a 1 MHz external clock
// in Normal, Idle, and Low Power Idle modes.
//
// Refer to F96x_Clock_External_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.4 - External Mode Current):
// -----------------------------------------------
// 1) In "Define Hardware", #define either "UDP_F960_MCU_MUX_LCD" or
//    "UDP_F960_MCU_EMIF" depending on which board you are using.
// 2) Cut the trace on the bottom of the PCB between the two pins of the VBAT
//    CURRENT header (LCD: J15, EMIF: J18).
// 3) Cut the trace on the bottom of the PCB between the two pins of the header
//    J20.
// 4) If using the LCD board, Remove SW3/P0.2/XTAL1 pull-up resistor R40.
// 5) If using the EMIF board, connect P0.0 to P3.0.
// 6) Connect a current meter between the two pins on the VBAT CURRENT header.
// 7) Set the VBAT Selector SW5 to the desired power source.
// 8) Set VIO Select (SW7) and VIORF(SW12) Select to VBAT.
// 9) Attach a crystal oscillator network to XTAL1 (P0.2) and XTAL2 (P0.3) pins.
//    A 1 MHz crystal is used for the measurements in Table 4.4.
// 10) Set the constant <CRYSTAL_FREQUENCY> to match the frequency of the installed
//    crystal oscillator.
// 11) To demonstrate Low Power Idle clock gating, set the constant
//    <PERIPHERALS_ENABLED> to '1' to have Timers0-3 and the ADC enabled during
//    Normal and Idle modes, but disabled during Low Power Idle mode.
// 12) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 13) Disconnect the debugger in order to reduce current draw. Apply the
//    the desired power source to run the code.
// 14) The device will initially be running in NORMAL mode (CPU running,
//    fetching instructions from flash). To cycle between Normal, Idle, and
//    Low Power Idle modes, press SW1.
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Added mode cycling functionality
//    - Added support for the EMIF board
//    - 12 FEB 2014
//
// Release 1.0
//    - Initial Revision (MT)
//    - 18 OCT 2013
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
SI_SBIT(SW1, SFR_P0, 0);
#endif

#ifdef UDP_F960_MCU_EMIF
SI_SBIT(SW1, SFR_P3, 0);
#endif

//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

// Please initialize the following constant to the crystal frequency
#define CRYSTAL_FREQUENCY    1000000    //  Crystal Frequency in Hz

//Set this to '1' to enable Timers0/1/2/3 and the ADC.
#define PERIPHERALS_ENABLED 0

//Set this to '1' to enable peripherals during Low-power Idle mode.
//Normally, peripherals would only be enabled during Normal and Idle modes.
#define PERIPHERALS_ENABLED_DURING_LOW_POWER_IDLE       0

// XFCN Setting Macro
#if  (CRYSTAL_FREQUENCY <= 20000)
    #define XFCN 0
#elif(CRYSTAL_FREQUENCY <= 58000)
    #define XFCN 1
#elif(CRYSTAL_FREQUENCY <= 155000)
    #define XFCN 2
#elif(CRYSTAL_FREQUENCY <= 415000)
    #define XFCN 3
#elif(CRYSTAL_FREQUENCY <= 1100000)
    #define XFCN 4
#elif(CRYSTAL_FREQUENCY <= 3100000)
    #define XFCN 5
#elif(CRYSTAL_FREQUENCY <= 8200000)
    #define XFCN 6
#elif(CRYSTAL_FREQUENCY <= 40000000)
    #define XFCN 7
#else
    #error "Crystal Frequency must be less than 40MHz"
#endif

#define CLK_DIV                 0x00

enum CLOCK_MODE{
   NORMAL,
   IDLE,
   LOW_POWER_IDLE
};

enum CLOCK_MODE NEXT_CLOCK_MODE = NORMAL;

#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(PortMatch_ISR,PMATCH_IRQn);
void Oscillator_Init (void);
void Port_Init (void);
void Peripherals_Enable(void);
void Idle(void);

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
                                        
    // Initialize Port 
    Port_Init();   

    // Disable precision oscillator regulator
    REG0CN &=~0x10;
    
    //Enable Peripherals to highlight the difference between IDLE and
    //Low_POWER_IDLE modes.
#if (PERIPHERALS_ENABLED == 1)
    Peripherals_Enable();
#endif

    // Starting Mode: External oscillator 1 MHz Normal Mode
    Oscillator_Init();
    
    SFRPAGE = CONFIG_PAGE;
    
#if (PERIPHERALS_ENABLED_DURING_LOW_POWER_IDLE == 1)
    //Leave all peripherals enabled when entering Low-power idle mode.
    PCLKEN = 0x0F;
#else
    //We must keep the PMU active (PCLKEN[3]), otherwise we may not be able to
    //leave low-power idle mode. Disable all other peripherals.
    //To enable the lowest-power mode, set PCLKEN to 0x00.
    PCLKEN = 0x08;
#endif 

    while (1) {
      switch (NEXT_CLOCK_MODE) {

      case NORMAL:
         //Stay in Normal mode until the switch is pressed and released
         while(SW1 != SW_PRESSED);
         while(SW1 == SW_PRESSED);

         NEXT_CLOCK_MODE = IDLE;
         break;

      case IDLE:
         //Go into Idle mode.
         Idle();

         NEXT_CLOCK_MODE = LOW_POWER_IDLE;
         break;

      case LOW_POWER_IDLE:
         // Enable Low-power idle mode
         CLKMODE = CLKMODE_LPMEN__ENABLED;
         //Go into Idle mode.
         Idle();
         // Disable Low-power idle mode
         CLKMODE = CLKMODE_LPMEN__DISABLED;

         NEXT_CLOCK_MODE = NORMAL;
         break;
      }
   }
}

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the external oscillator
// in crystal mode.the system clock is 1 MHz.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
    SFRPAGE = LEGACY_PAGE;
    OSCXCN = (0x70 | XFCN);             // start external oscillator with
                                        // the appropriate XFCN setting
                                        // based on crystal frequency
    
    while (!(OSCXCN & 0x80));           // Wait for crystal osc. to settle
    
    RSTSRC = 0x06;                      // Enable missing clock detector and
                                        // leave VDD Monitor reset enabled
    
    CLKSEL = 0x01 | CLK_DIV;            // Select external oscillator as system
                                        // clock source, 
	
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
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   SFRPAGE = LEGACY_PAGE;
    // P0.2, P0.3 Oscillator Pins
    P0MDIN  &= ~0x0C;                  // P0.2, P0.3 are analog
    
    P0SKIP  |=  0x0C;                  // P0.2, P0.3 skipped in the Crossbar
    P0MDOUT &= ~0x0C;                  // PP0.2, P0.3 output mode is open-drain
    P0      |=  0x0C;                  // P0.2, P0.3 output drivers are OFF

#ifdef UDP_F960_MCU_MUX_LCD
    // SW1 (P0.0) Input - Oscillator Clock Cycle Switch
    P0MDIN |= 0x01;                     // P0.0 is digital
    P0MDOUT &= ~0x01;                   // P0.0 is open-drain
    P0 |= 0x01;                         // Set the P0.0 latch to '1'
#endif

#ifdef UDP_F960_MCU_EMIF
    // SW1 (P3.0) Input - Oscillator Clock Cycle Switch
    P3MDIN |= 0x01;                     // P3.0 is digital
    P3MDOUT &= ~0x01;                   // P3.0 is open-drain
    P3 |= 0x01;                         // Set the P3.0 latch to '1'
#endif

    // Set Port Match settings for P0.0
    P0MASK |= 0x01;                     // Enable Port Match check for P0.1
    P0MAT  |= 0x01;                     // Set Port Match value to 1 for P0.1

    //Enable port match interrupts
    EIE2 |= EIE2_EMAT__ENABLED;

    //Enable global interrupts
    IE_EA = 1;

    XBR2    = 0x40;                     // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Peripherals_Enable
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function enables several peripherals to highlight the power savings
// when the CPU enters Low Power Idle mode, which disables the clock output
// to these peripherals.
//
//-----------------------------------------------------------------------------
void Peripherals_Enable (void)
{

   //Enable Timer0
   TCON_TR0 = 1;

   //Enabled Timer1
   TCON_TR1 = 1;

   //Enabled Timer2
   TMR2CN_TR2 = 1;

   //Enable Timer3
   TMR3CN = TMR3CN_TR3__RUN;

   //Enabled ADC
   ADC0CN_ADEN = 1;
}

//-----------------------------------------------------------------------------
// Idle
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function places the device in Idle mode. The device will wake from idle
// upon an interrupt. In this case, the interrupt will be generated by a
// port-match event on SW1 when it is pressed.
//
//-----------------------------------------------------------------------------
void Idle(void)
{
   PCON |= PCON_IDLE__IDLE;    // Idle Mode
   PCON = PCON;                // Follow the instruction to enter idle
                               // with a 3 cycle dummy instruction

   //--------------------------------------------------------------------------
   // Device is in Idle Mode
   //
   // Device will wake upon any interrupt. In this case, a port-match interrupt
   // generated by pressing SW1.
   //
   //--------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------
// PortMatch_ISR
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This interrupt is called whenever SW1 is pressed.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(PortMatch_ISR, PMATCH_IRQn)
{
   //Wait for the switch to be released before exiting the ISR
   //Otherwise, the ISR will continue to fire.
   while(SW1 == SW_PRESSED);
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------


