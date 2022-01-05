//-----------------------------------------------------------------------------
// F36x_LFO_Idle.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// The program demonstrates the Low Frequency internal oscillator and power
// savings mode operation on the F360. The MCU will sleep in low power idle
// mode with only the low frequency oscillator and timer T0 running. When
// the MCU wakes up it will switch to a 24.5 MHz clock and turn on an LED
// and wait 100 ms. The 100 ms delay also uses T0 and the low frequency
// oscillator but does not use idle mode or interrupts.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: P3.2 - P3.2_LED
//    J1: Power selection, P2 9V, P4 USB or J9 Debugger
// 2) Connect the USB Debug Adapter to J9. If 9V power is selected on J1,
//    connect the 9V power adapter to P2. If VBUS is selected on J1, connect
//    USB cable to P4.
// 3) Compile and download code to C8051F360 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) The P3.2 LED should blink once a second.
//
//
// Target:         C8051F36x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1
//    - Updated to use with Simplicity Studio (JL)
//    - 29 JUL 2014
//
// Release 1.0
//    -Initial Revision (KAB / TP)
//    -13 OCT 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SLEEP_TIME 10000               // Sleep 1 second
#define BLINK_TIME  1000               // Blink 100 ms

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Port_Init (void);

void lowPowerSleep (uint16_t);
void configMCU_Sleep(void);
void configMCU_Wake(void);
void delay (uint16_t);
SI_INTERRUPT_PROTO(Timer0_ISR, TIMER0_IRQn);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P3, 2);

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
   PCA0MD  &= ~PCA0MD_WDTE__ENABLED;   // Disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{

   Oscillator_Init ();
   Port_Init ();

   IE_EA = 1;                          // Set global interrupt enable

   SFRPAGE = LEGACY_PAGE;

   while (1)
   {
      lowPowerSleep (SLEEP_TIME);
      LED = 1;
      delay (1000);
   }
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
// P3.2 push-pull output LED
//-----------------------------------------------------------------------------
void Port_Init ()
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   P3MDOUT = 0x04;

   XBR1   |= XBR1_XBARE__ENABLED;      // Enable crossbar

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Oscillator_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the internal 24.5 MHz oscillator.
//-----------------------------------------------------------------------------
void Oscillator_Init ()
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // Enable 24.5 MHz int osc
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// lowPowerSleep ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : time - sleep time (10000 = 1 second)
//
// This function will put the MCU into low power sleep mode using a T0
// interrupt to wake up the MCU. In sleep mode the MCU will be running off
// the internal low frequency oscillator running at 10 kHz with all of the
// peripherals disabled except T0.
//-----------------------------------------------------------------------------
void lowPowerSleep (uint16_t time)
{
   uint8_t restoreIE;
   uint8_t restoreEIE1;
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   IE_EA = 0;                          // Clear global interrupt enable

   // save interrupt state
   restoreIE = IE;                     // Preserve IE register
   restoreEIE1 = EIE1;                 // Preserve EIE1 register

   // disable all interrupts
   IE       = 0x00;                    // Disable interrupts
   EIE1     = 0x00;                    // Disable extended interrupts

   configMCU_Sleep ();                 // Configure for Sleep

   //init T0 to wake up MCU

   TCON    &= ~(TCON_TF0__BMASK |
                TCON_TR0__BMASK);      // Stop T0 & clear flag
   TMOD    &= ~0x0F;                   // Clear T0 nibble of TMOD
   TMOD    |= TMOD_T0M__MODE1;         // T0 16 bit counter mode
   CKCON   |= CKCON_T0M__SYSCLK;       // T0 uses SYSCLK

   // set T0 using time
   time = -time;
   TL0      = (uint8_t)(time & 0x00FF);
   TH0      = (uint8_t)(time >> 8);

   // Comment out this line to measure stead state idle current
   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   IE      |= 0x82;                    // Enable T0 & global interrupts
   //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

   //switch to low freq oscillator
   OSCLCN   = OSCLCN_OSCLEN__ENABLED;  // LFO is 10 kHz
   CLKSEL   = CLKSEL_CLKSL__LFOSC;     // Switch to LFO
   OSCICN   = OSCICN_IOSCEN__DISABLED; // Stop internal osc to save power

   TCON    |= TCON_TR0__RUN;           // Enable T0

   // go to IDLE mode
   PCON    |= PCON_IDLE__IDLE;         // Idle mode
   PCON     = PCON;

   //--wake--//

   configMCU_Wake ();

   // restore MCU pin configuration to portInit state & restore LEDs
   EIE1     = restoreEIE1;             // Restore ext interrupts
   IE       = restoreIE;               // Restore interrupts
   IE_EA = 1;                          // Set global interrupt enable

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// configMCU_Sleep ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the MCU for low power sleep disabling the crossbar,
// enabling weak pull-ups, and explicitly turning off all of the peripherals.
//
//-----------------------------------------------------------------------------
void configMCU_Sleep (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   XBR1    &= ~(XBR1_WEAKPUD__BMASK |
                XBR1_XBARE__BMASK);    // Disable crossbar, enabled weak pull ups

   // disable all peripherals for low power sleep
   SCON0   &= ~SCON0_REN__BMASK;       // Disable UART
// TCON    &= ~0x10;                   // Don't disable T0
   TCON    &= ~TCON_TR1__BMASK;        // Disable T1
   TMR2CN  &= ~TMR2CN_TR2__BMASK;      // Disable T2
   TMR3CN  &= ~TMR3CN_TR3__BMASK;      // Disable T3
   PCA0CN  &= ~PCA0CN_CR__BMASK;       // Disable PCA0
   SMB0CF  &= ~SMB0CF_ENSMB__BMASK;    // Disable SMBUS
   SPI0CN  &= ~SPI0CN_SPIEN__BMASK;    // Disable SPI
   CPT0CN  &= ~CPT0CN_CPEN__BMASK;     // Disable comparator
   REF0CN  &= ~(REF0CN_TEMPE__BMASK|
                REF0CN_BIASE__BMASK |
                REF0CN_REFBE__BMASK);  // Disable voltage reference
   IDA0CN  &= ~IDA0CN_IDAEN__BMASK;    // Disable iDAC
   VDM0CN  &= ~VDM0CN_VDMEN__BMASK;    // Disable voltage monitor

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// configMCU_Wake ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will configure the MCU for wake mode operation. Un-comment
// the peripheral enables for the ones that you are using.
//-----------------------------------------------------------------------------
void configMCU_Wake (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

// enable peripherals as needed
// SCON0   |= 0x10;                    // Enable UART
// TCON    |= 0x10;                    // Enable T0
// TCON    |= 0x40;                    // Enable T1
// TMR2CN  |= 0x04;                    // Enable T2
// TMR3CN  |= 0x04;                    // Enable T3
// PCA0CN  |= 0x40;                    // Enable PCA0
// SMB0CF  |= 0x80;                    // Enable SMBUS
// SPI0CN  |= 0x05;                    // Enable SPI
// CPT0CN  |= 0x80;                    // Enable comparator
// REF0CN  |= 0x07;                    // Enable voltage reference
// IDA0CN  |= 0x80;                    // Enable iDAC
// VDM0CN  |= 0x80;                    // Enable voltage monitor

   XBR1    |= XBR1_XBARE__ENABLED;     // Enable crossbar

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// delay ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : time - delay time (10000 = 1 second)
//
// This function will implement a delay using T0 and the LFO 10 kHz clock.
// A delay time of 10000 counts is equal to one second. This function modifies
// the system clock and exits with a 24.5 MHz clock.
//-----------------------------------------------------------------------------
void delay(uint16_t time)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   TCON    &= ~0x30;                   // Stop T0 & clear flag
   TMOD    &= ~0x0F;                   // Clear T0 nibble of TMOD
   TMOD    |= TMOD_T0M__MODE1;         // T0 16 bit counter mode
   CKCON   |= CKCON_T0M__SYSCLK;       // T0 uses SYSCLK
   // set T0 using time
   time = -time;
   TL0      = (uint8_t)(time & 0x00FF);
   TH0      = (uint8_t)(time >> 8);
   //switch to low freq oscillator
   OSCLCN   = OSCLCN_OSCLEN__ENABLED;  // LFO is 10 kHz
   CLKSEL   = CLKSEL_CLKSL__LFOSC;     // Switch to LFO
   TCON    |= TCON_TR0__RUN;           // Enable T0
   while(!TCON_TF0);                   // Wait for TOF
   TCON    &= ~(TCON_TF0__BMASK |
                TCON_TR0__BMASK);      // Stop T0 & clear flag
   // Enable 24.5 MHz int osc
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;

   // Use int osc
   CLKSEL   = CLKSEL_CLKSL__DIVIDED_HFOSC;

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------
// T0ISR ()
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer0_ISR, TIMER0_IRQn)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // Enable 24.5 MHz int osc
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;

   // Wait for lock bit
   while ((OSCICN & OSCICN_IFRDY__BMASK) == OSCICN_IFRDY__NOT_SET);
   // Use int osc
   CLKSEL  = CLKSEL_CLKSL__DIVIDED_HFOSC;

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE

   TCON   &= ~TCON_TR0__BMASK;         // Stop timer T0
   IE     &= ~IE_ET0__BMASK;           // Disable T0 interrupt
}

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------