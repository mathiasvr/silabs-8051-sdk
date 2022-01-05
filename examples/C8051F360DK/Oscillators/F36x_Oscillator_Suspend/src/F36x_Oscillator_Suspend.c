//-----------------------------------------------------------------------------
// F36x_Oscillator_Suspend.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// The program demonstrates the power saving SUSPEND mode operation on the
// F360. The MCU will sleep in low power suspend mode and wait until a switch
// is pressed. When the MCU wakes up it will switch to a 24.5 MHz internal
// oscillator & turn on an LED. After 1 second, it will go back into SUSPEND
// until the switch is pressed again.
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: P3.0 - SW3.0
//    J12: P3.2 - P3.2_LED
//    J4:  P2.0 - J5: P3.0
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
// 5) Verify the LED turns on when the switch is pressed.
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

#define SYSCLK    24500000

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Port_Init (void);

void configSleep (void);
void configWake (void);
void delay (uint16_t ms);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P3, 2);                  // Port pin used for LED

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
// main () Routine
//-----------------------------------------------------------------------------
void main (void)
{

   Oscillator_Init ();                 // Initialize 24.5 MHz oscillator
   Port_Init ();                       // Initialize IO ports

   IE_EA = 1;                          // Set global interrupt enable

   SFRPAGE = LEGACY_PAGE;

   while (1)
   {
      configSleep ();                  // Configure MCU for low power sleep

      SFRPAGE = CONFIG_PAGE;

      // Set suspend mode bit
      OSCICN |= OSCICN_SUSPEND__ENABLED;

      // Wait for a switch press

      SFRPAGE = LEGACY_PAGE;

      configWake ();                   // Configure MCU for wake mode

      SFRPAGE = LEGACY_PAGE;

      LED = 1;                         // Turn on LED

      delay (1000);                    // Wait 1000 ms (1 second)

      LED = 0;                         // Turn off LED
   }
}

//-----------------------------------------------------------------------------
// Port_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P2.0   open-drain   input    port-match input
//
// P3.0   open-drain   input    SW1
//
// P3.2   push-pull    output   LED
//
//-----------------------------------------------------------------------------
void Port_Init ()
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   P2MDOUT &= ~0x01;                   // Enable P2.0 as open-drain

   P3MDOUT = 0x04;                     // Enable P3.0 as open-drain
                                       // Enable P3.2 as push-pull

   XBR1 |= XBR1_XBARE__ENABLED;        // Enable crossbar

   SFRPAGE = LEGACY_PAGE;

   P2MAT = 0x01;                       // Set up the port-match value on P2
   P2MASK = 0x01;                      // An event will occur if
                                       // (P2 & P2MASK) != (P2MAT & P2MASK)
                                       // The SW1 pin = 1 if the switch isn't
                                       // pressed

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Oscillator_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the internal oscillator to 24.5 MHz.
//
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
// configSleep ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the MCU for low power sleep by disabling the
// crossbar, enabling weak pull-ups, and turning off MCU peripherals. The
// lowest possible sleep current can be obtained by turning off the Voltage
// monitor.
//
//-----------------------------------------------------------------------------
void configSleep (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   XBR1   &= ~(XBR1_WEAKPUD__BMASK |
               XBR1_XBARE__BMASK);     // Disable crossbar, enabled weak pull ups

   // Disable all peripherals for low power sleep

   SCON0 &= ~SCON0_REN__BMASK;         // Disable UART0
   TCON &= ~TCON_TR0__BMASK;           // Disable T0
   TCON &= ~TCON_TR1__BMASK;           // Disable T1
   TMR2CN &= ~TMR2CN_TR2__BMASK;       // Disable T2

   // Disable any additional peripherals here

   VDM0CN &= ~VDM0CN_VDMEN__BMASK;     // Disable voltage monitor

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// configWake ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will configure the MCU for wake mode operation.
// Enable additional peripherals as needed.
//
//-----------------------------------------------------------------------------
void configWake (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   VDM0CN |= VDM0CN_VDMEN__ENABLED;    // Enable voltage monitor
   TCON |= TCON_TR0__RUN;              // Enable T0

   // enable any additional peripherals here

   XBR1 |= XBR1_XBARE__ENABLED;        // Enable crossbar

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// delay ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1)  unsigned int ms - number of milliseconds to delay
//                    range is entire range of unsigned integer: 0 to 65535
//
// This function will implement a delay using T0.
// A value of 1000 for <ms> is equal to one second.
//
//-----------------------------------------------------------------------------
void delay (uint16_t ms)
{
   TCON &= ~(TCON_TF0__BMASK |
             TCON_TR0__BMASK);         // Stop T0 & clear flag
   TMOD &= ~0x0F;                      // Clear T0 nibble of TMOD
   TMOD |= TMOD_T0M__MODE1;            // T0 16 bit counter mode
   CKCON |= CKCON_T0M__SYSCLK;         // T0 uses SYSCLK

   // set T0 using time
   TL0 = (uint8_t) (-(SYSCLK / 1000) & 0x00FF);
   TH0 = (uint8_t) ((-(SYSCLK / 1000) & 0xFF00) >> 8);

   TCON |= TCON_TR0__RUN;              // Enable T0

   while (ms > 0)
   {
      while (!TCON_TF0);               // Wait for TOF

      TCON_TF0 = 0;                    // Clear the timer flag

      // Reload the Timer0 registers
      TL0 = (uint8_t) (-(SYSCLK / 1000) & 0x00FF);
      TH0 = (uint8_t) ((-(SYSCLK / 1000) & 0xFF00) >> 8);

      ms--;
   }
}

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------