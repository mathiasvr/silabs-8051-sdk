//-----------------------------------------------------------------------------
// F97x_Imeasure_Sleep_RTC_LF_Oscillator.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program reproduces the power number shown in the datasheet, Table 1.2,
// in the Digital Supply Current - Suspend and Sleep Mode section.
//
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
// 2) Pull off the JP3 jumper and place a multimeter across, positive side on
//    the pin labeled VDD.
// 3) Connect the USB Debug Adapter to H8.
// 4) Place the VDD SELECT switch (SW1) in the VREG position and power the board.
// 5) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 6) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 7) Measure the supply current using the multimeter.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (TP/JL)
//    - 27 MAY 2014


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F970_Register_Enums.h"
#include <intrins.h>

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Port_Init (void);

void RTC_Init (void);
void RTC_Write (uint8_t, uint8_t);
void Wait_us(uint16_t us);

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
  SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
  PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{

   //--------------------------------------------------------------------------
   // Low Power Startup Delay
   //--------------------------------------------------------------------------
   // Decreasing the MCU power consumption for a short period after Power-On
   // Reset allows the decoupling capacitors time to recharge. This allows the
   // MCU to successfully start up at lower voltages when the source impedance
   // is very high. Examples of high source impedance include a battery which
   // is near its end of life or when starting up while being connected to a
   // current meter.

   if((RSTSRC & RSTSRC_PORSF__BMASK) == RSTSRC_PORSF__SET ||
      (PMU0CF & PMU0CF_RSTWK__BMASK) == PMU0CF_RSTWK__SET)
   {
      // Wait 100 ms
      Wait_us(65000);
      Wait_us(35000);
   }

   Port_Init();                        // Initialize Port I/O
   RTC_Init();
   Oscillator_Init ();                 // Initialize Oscillator

   while(1)
   {
      // If using a clock other than the Low Power Oscillator, switch to
      // the Low Power Oscillator and select a divide-by-1 setting before
      // going to sleep.

      CLKSEL = CLKSEL_CLKSL__LPOSC |
               CLKSEL_CLKDIV__SYSCLK_DIV_1;

      // Wait for divider to be applied
      while ((CLKSEL & CLKSEL_CLKRDY__BMASK) == CLKSEL_CLKRDY__NOT_SET);

      FLSCL &= ~FLSCL_BYPASS__BMASK;

      PMU0CF = PMU0CF_CLEAR__ALL_FLAGS;
      PMU0CF = PMU0CF_SLEEP__START;

      NOP();                        // Do not place a breakpoint on
                                    // this instruction if waking up
                                    // from Sleep Mode. Placing a
                                    // breakpoint here will cause
                                    // the IDE to disconnect.

      NOP(); NOP(); NOP();          // 4 NOPs
                                    // are required after waking up
                                    // from Sleep mode.

      FLSCL |= FLSCL_BYPASS__SYSCLK;// Set the BYPASS bit

      // If the wake-up source was a falling edge on /RST
      if((PMU0CF & PMU0CF_RSTWK__BMASK) == PMU0CF_RSTWK__SET)
      {
         Wait_us(15);               // Wait at least 15 us before
                                    // allowing the MCU to go back
                                    // into the low power mode.
                                    // This gives the IDE/Debug
                                    // Adapter time to synchronize
                                    // with the MCU and allows the MCU
                                    // to respond to a Pin Reset.
      }
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
// This function initializes the system clock to use the selected oscillator
// and divide value.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = LEGACY_PAGE;

   // If system clock is > 14 MHz, bypass (disable) the one-shot timer
   FLSCL |= FLSCL_BYPASS__SYSCLK;      // Set the BYPASS bit

   REG0CN &= ~REG0CN_OSCBIAS__BMASK;   // Disable precision oscillator bias

   CLKSEL = CLKSEL_CLKSL__LPOSC |
            CLKSEL_CLKDIV__SYSCLK_DIV_1;

   // Wait for divider to be applied
   while ((CLKSEL & CLKSEL_CLKRDY__BMASK) == CLKSEL_CLKRDY__NOT_SET);

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   SFRPAGE = CONFIG_PAGE;

   P0 = 0xFF;                          // Set all I/O to analog inputs
   P1 = 0xFF;                          // to save power
   P2 = 0xFF;
   P3 = 0xFF;
   P4 = 0xFF;
   P5 = 0xFF;

   P0MDIN = 0x00;
   P1MDIN = 0x00;
   P2MDIN = 0x00;
   P3MDIN = 0x00;
   P4MDIN = 0x00;
   P5MDIN = 0x00;

   // Uncomment this section to send SYSCLK out a port pin
   /*
   // SYSCLK to P0.1
   P0SKIP |= 0x01;  // P0.0 skipped
   P0MDIN |= 0x02;  // P0.1 is an output
   P0MDOUT |= 0x02; // P0.1 is push-pull


   XBR0 = XBR0_SYSCKE__ENABLED;            // Put SYSCLK onto Crossbar
   */
   XBR1 = XBR1_XBARE__ENABLED |            // Enable Crossbar and
          XBR1_WEAKPUD__PULL_UPS_DISABLED; // disable Weak pullups.
                                           // Less power consumption with
                                           // crossbar enabled...

   SFRPAGE = LEGACY_PAGE;
}


//-----------------------------------------------------------------------------
// RTC_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will initialize the RTC to use the LFO.
//
//-----------------------------------------------------------------------------
void RTC_Init (void)
{
   RTC_Write (RTC0XCN, RTC0XCN_BIASX2__DISABLED
              | RTC0XCN_LFOEN__ENABLED);

   // Disable Auto Load Cap Stepping
   RTC_Write (RTC0XCF, RTC0XCF_AUTOSTP__DISABLED
              | 0x07);


   RTC_Write (RTC0CN, RTC0CN_RTC0EN__ENABLED
              | RTC0CN_MCLKEN__ENABLED);

   // Wait > 2 ms
   Wait_us(2500);

   PMU0CF = PMU0CF_CLEAR__ALL_FLAGS;

}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// RTC_Write
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) unsigned char reg - address of RTC register to write
//                2) unsigned char value - the value to write to <reg>
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
// Wait_us
//-----------------------------------------------------------------------------
//
// Insert <us> microsecond delays using an on-chip timer.
//
void Wait_us(uint16_t us)
{
   CKCON |= CKCON_T2ML__SYSCLK;        // Timer2 uses SYSCLK

   // Setup Timer2 to overflow at a 1 MHz Rate
   TMR2RL = -(20000000/1000000);
   TMR2 = TMR2RL;

   TMR2CN |= TMR2CN_TR2__RUN;          // Enable Timer2 in auto-reload mode

   while(us)
   {
      TMR2CN &= ~TMR2CN_TF2H__BMASK;
      while((TMR2CN & TMR2CN_TF2H__BMASK) == TMR2CN_TF2H__NOT_SET);
      us--;
   }

   TMR2CN &= ~TMR2CN_TR2__BMASK;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
