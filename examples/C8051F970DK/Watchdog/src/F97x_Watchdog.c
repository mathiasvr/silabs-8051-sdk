//-----------------------------------------------------------------------------
// F97x_Watchdog.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use the Watchdog instead of disabling it!
//
// This program helps the user to learn about operating the Watch Dog Timer.
// The WDT is used to generate resets if the times between writes to the WDT
// update register (PCA0CPH2) exceed a specified limit. The WDT can be disabled
// and enabled in the software as needed. When enabled the PCA Module 2 acts as
// the WDT. This program resets the MCU when P0.2 switch is pressed. Also upon
// reset the LED blinks approximately five times faster when compared to before.
// The reset is caused due to a WDT overflow and can be confirmed by checking
// the value of the RSTSRC register where bit 3 is set to indicate a reset
// caused by WDT.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) LED05 will blink at a rate of 5Hz. Once BTN03 is pressed and held,
//    the WDT will cause a reset, and LED05 will blink at 20Hz.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 30 APR 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       20000000/8        // SYSCLK frequency in Hz

SI_SBIT(LED05, P0, 5);                     // LED='0' means ON
SI_SBIT(BTN03, P0, 3);                     // SW0='0' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void PCA_Init (void);
void TIMER2_Init (uint16_t counts);

SI_INTERRUPT_PROTO(Timer2_ISR, TIMER2_IRQn);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   SFRPAGE = PCA0_PAGE;                // Disable the watchdog timer
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;
  XBR1 = XBR1_XBARE__ENABLED; while(!(P0 & P0_B3__BMASK)); // While BTN03 pressed    // Disable watchdog timer

   SYSCLK_Init ();                     // Configure system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   PCA_Init ();                        // Initialize PCA


   SFRPAGE = LEGACY_PAGE;
   if ((RSTSRC & 0x02) == 0x00)        // First check the PORSF bit. if PORSF
   {                                   // is set, all other RSTSRC flags are
                                       // invalid
      // Check if the last reset was due to the Watch Dog Timer
      if (RSTSRC == 0x08)
      {
         TIMER2_Init (SYSCLK / 12 / 20); // Make LED blink at 20Hz

         IE_EA = 1;                      // Enable global interrupts

         while(1);                       // wait forever
      }
      else
      {
         // Init Timer2 to generate interrupts at a 5Hz rate.
         TIMER2_Init (SYSCLK / 12 / 5);
      }

   }

   // Calculate Watchdog Timer Timeout
   // Offset calculated in PCA clocks
   // Offset = ( 256 x PCA0CPL5 ) + 256 - PCA0L
   //        = ( 256 x 255(0xFF)) + 256 - 0
   // Time   = Offset * (12/SYSCLK)
   //        = 255 ms ( PCA uses SYSCLK/12 as its clock source)

   PCA0MD &= ~PCA0MD_WDTE__ENABLED;
  XBR1 = XBR1_XBARE__ENABLED; while(!(P0 & P0_B3__BMASK)); // While BTN03 pressed    // Disable watchdog timer
                                       // enable)
   PCA0L    = 0x00;                    // Set lower byte of PCA counter to 0
   PCA0H    = 0x00;                    // Set higher byte of PCA counter to 0

   PCA0CPL2 = 0xFF;                    // Write offset for the WDT

   PCA0MD |= PCA0MD_WDTE__ENABLED;     // Disable watchdog timer


   IE_EA = 1;                          // Enable global interrupts



   //--------------------------------------------------------------------------
   // Main Application Loop/Task Scheduler
   //--------------------------------------------------------------------------

   while (1)
   {
       //----------------------------------------------------------------------
       // Task #1 - Check Port I/O
       //----------------------------------------------------------------------
       while(!BTN03);                  // Force the MCU to stay in this task as
                                       // long as SW2 is pressed. This task must
                                       // finish before the watchdog timer
                                       // timeout expires.

       //----------------------------------------------------------------------
       // Task #2 - Reset Watchdog Timer
       //----------------------------------------------------------------------
       PCA0CPH2 = 0x00;                // Write a 'dummy' value to the PCA0CPH2
                                       // register to reset the watchdog timer
                                       // timeout. If a delay longer than the
                                       // watchdog timer delay occurs between
                                       // successive writes to this register,
                                       // the device will be reset by the watch
                                       // dog timer.
    }
}


//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// This routine initializes the system clock to use the internal 20 MHz
// oscillator as its clock source divided by 8.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;             // Save the current SFRPAGE
   SFRPAGE = LEGACY_PAGE;

   CLKSEL = CLKSEL_CLKSL__LPOSC |         // Use Low Power Int Osc
            CLKSEL_CLKDIV__SYSCLK_DIV_8;  // 20MHz/8 = 2.5MHz

   SFRPAGE = SFRPAGE_save;                // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the Crossbar and GPIO ports.
//
// P0.5 - digital  push-pull   LED
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x20;                    // Enable LED as a push-pull

   XBR1    = XBR1_XBARE__ENABLED |     // Enable crossbar
             XBR1_WEAKPUD__PULL_UPS_ENABLED; // with weak pullups

   SFRPAGE = SFRPAGE_save;             // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// PCA_Init
//-----------------------------------------------------------------------------
// Return Value : None
// Parameters   : None
//
// This routine initializes the PCA to use the SYSCLK / 12
// as its clock source.  It also sets the offset value by writing to PCA0CPL5.
//-----------------------------------------------------------------------------
void PCA_Init()
{
   uint8_t SFRPAGE_save = SFRPAGE;           // Save the current SFRPAGE
   SFRPAGE = PCA0_PAGE;

   PCA0CN_CR = 1;                       // PCA counter enable

   PCA0MD &= ~PCA0MD_WDTE__ENABLED;
  XBR1 = XBR1_XBARE__ENABLED; while(!(P0 & P0_B3__BMASK)); // While BTN03 pressed     // Disable watchdog timer

   PCA0MD &= ~PCA0MD_CPS__FMASK;        // Select Timebase
   PCA0MD |= PCA0MD_CPS__SYSCLK_DIV_12; // System clock / 12

   PCA0CPL2 = 0xFF;                     // Offset value

   SFRPAGE = SFRPAGE_save;              // Restore saved SFRPAGE
}

//-----------------------------------------------------------------------------
// TIMER2_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    uint16_t counts - number of system clock cycles between interrupts
//                             range is 0 to 65535
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/12 as its time base.
//
//-----------------------------------------------------------------------------

void TIMER2_Init (uint16_t counts)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE
   SFRPAGE = TIMER2_PAGE;

   TMR2CN_TR2 = TMR2CN_TR2__STOP;      // Stop Timer2; Clear TF2
   TMR2CN &= ~TMR2CN_T2XCLK__FMASK;    // Timebase = SYSCLK/12
   TMR2CN |= TMR2CN_T2XCLK__SYSCLK_DIV_12;

   CKCON &= ~(CKCON_T2ML__BMASK |      // Timer2 clocked based on T2XCLK
              CKCON_T2MH__BMASK);
   CKCON |= CKCON_T2MH__EXTERNAL_CLOCK |
            CKCON_T2ML__EXTERNAL_CLOCK;

   TMR2RL = -counts;                   // Init reload values
   TMR2 = 0xFFFF;                      // Set to reload immediately

   IE_ET2 = 1;                         // Enable Timer2 interrupt
   TMR2CN_TR2 = TMR2CN_TR2__RUN;       // Start Timer2

   SFRPAGE = SFRPAGE_save;             // Restore saved SFRPAGE
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED whenever Timer2 overflows.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                    // Clear Timer2 interrupt flag
   LED05 = !LED05;                     // Change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
