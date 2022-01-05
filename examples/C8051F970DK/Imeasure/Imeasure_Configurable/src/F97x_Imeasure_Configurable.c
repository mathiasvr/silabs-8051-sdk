//-----------------------------------------------------------------------------
// F97x_Imeasure_Configurable.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program reproduces the power number shown in the datasheet, Table 1.2.
// All of the possible selections are available in the F97x_Config.h file.
//
//
// How To Test:
// ------------
// 1) Select the desired options in F97x_Config.h.
// 2) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
// 3) Pull off the JP3 jumper and place a multimeter across, positive side on
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
//    - 29 MAY 2014


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"

#include "Global_Const.h"
#include "F97x_Config.h"
#include "Imeasure.h"

#include <intrins.h>

//-----------------------------------------------------------------------------
// Function Pointer
//-----------------------------------------------------------------------------

void (*Imeasure_Function_Pointer)(void);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Port_Init (void);

void Wait_us(uint16_t us);
void RTC_Init (void);
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
  SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
  PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{

   //--------------------------------------------------------------------------
   // MCU Initialization
   //--------------------------------------------------------------------------
   Port_Init();                        // Initialize Port I/O
   Oscillator_Init ();                 // Initialize Oscillator

   //--------------------------------------------------------------------------
   // Sleep or Suspend Mode
   //--------------------------------------------------------------------------
   #if ((POWER_MODE == SLEEP) || (POWER_MODE == RTCSLEEP) || (POWER_MODE == SUSPEND))
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

      while(1)
      {
         // If using a clock other than the Low Power Oscillator, switch to
         // the Low Power Oscillator and select a divide-by-1 setting before
         // going to sleep.

         if((CLOCK_SELECTION & 0x07) == SMARTCLOCK_OSC ||
            (CLOCK_SELECTION & 0x07) == EXTERNAL_OSC   )
         {
            CLKSEL = CLKSEL_CLKSL__LPOSC
                     | CLKSEL_CLKDIV__SYSCLK_DIV_1;
         }


         while ((CLKSEL & CLKSEL_CLKRDY__BMASK) == CLKSEL_CLKRDY__NOT_SET);

         FLSCL &= ~FLSCL_BYPASS__BMASK;

         #if((POWER_MODE == RTCSLEEP) && ((CLOCK_SELECTION & 0x0F) != SMARTCLOCK_OSC))
         RTC_Init();
         #endif

         PMU0CF = PMU0CF_CLEAR__ALL_FLAGS;
         PMU0CF = POWER_MODE;          // Enter the low power mode

         #if(POWER_MODE == SLEEP || POWER_MODE == RTCSLEEP)
            NOP();                     // Do not place a breakpoint on
                                       // this instruction if waking up
                                       // from Sleep Mode. Placing a
                                       // breakpoint here will cause
                                       // the IDE to disconnect.

            NOP(); NOP(); NOP();       // 4 NOPs
                                       // are required after waking up
                                       // from Sleep mode.
         #endif

         if((CLOCK_SELECTION & 0x07) == PRECISION_OSC)
         {
            REG0CN |= REG0CN_OSCBIAS__FORCE_ENABLED;    // Enable precision oscillator bias
            Wait_us(4);                // Insert 4uS  delay
         }

         CLKSEL = CLOCK_SELECTION;     // Restore selected system clock
         while ((CLKSEL & CLKSEL_CLKRDY__BMASK) == CLKSEL_CLKRDY__NOT_SET); // Wait for divider to be applied

         // Restore the oneshot setting
         #if((SYSCLK > 12500000))
         FLSCL |= FLSCL_BYPASS__SYSCLK;                // Set the BYPASS bit
         #endif

         // If the wake-up source was a falling edge on /RST
         #if(POWER_MODE == SLEEP || POWER_MODE == SUSPEND)
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
         #endif
      }
   #endif

   //--------------------------------------------------------------------------
   // Idle Mode
   //--------------------------------------------------------------------------
   #if (POWER_MODE == IDLE)

      while(1)
      {
         FLSCL &= ~FLSCL_BYPASS__BMASK;               // Clear the BYPASS bit


         PCON = IDLE;

         // Restore the oneshot setting
         #if(SYSCLK > 10000000)
         FLSCL |= FLSCL_BYPASS__SYSCLK;                // Set the BYPASS bit
         #endif

      }
   #endif

   //--------------------------------------------------------------------------
   // Stop Mode
   //--------------------------------------------------------------------------
   #if (POWER_MODE == STOP)

      if((CLOCK_SELECTION & 0x07) != PRECISION_OSC)
      {
         REG0CN |= REG0CN_OSCBIAS__FORCE_ENABLED; // Enable precision oscillator bias
         Wait_US(4);                   // Insert 4uS  delay
      }

      OSCICN |= OSCICN_IOSCEN__ENABLED;                  // Select precision oscillator as
      CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 |
             CLKSEL_CLKSL__HFOSC;                   // clock source
      while ((CLKSEL & CLKSEL_CLKRDY__BMASK) == CLKSEL_CLKRDY__NOT_SET); // Wait for divider to be applied


      PMU0CF = PMU0CF_CLEAR__ALL_FLAGS;                   // Clear wake-up flags to disable
                                       // the low power oscillator

      FLSCL &= ~FLSCL_BYPASS__BMASK;                  // Clear the BYPASS bit

      PCON = POWER_MODE;               // A reset is required to exit this mode

   #endif

   //--------------------------------------------------------------------------
   // Normal Mode
   //--------------------------------------------------------------------------
   // Infinite Loop For Measuring Current in Normal Mode
   // Note: Ensure that this loop does not bridge a 64-byte flash row
   // boundary. To ensure the while(1) statement is located on a single
   // row, manually assign its location in Flash.
   Imeasure_Function_Pointer = IMEASURE_FUNCTION_LOCATION;
   Imeasure_Function_Pointer();

   while(1);
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

   #if(ENABLE_MISSING_CLOCK_DETECTOR)

      #if (DISABLE_VDD_SUPPLY_MONITOR)
      RSTSRC = RSTSRC_MCDRSF__SET;     // Enable missing clock detector and
                                       // leave VDD Monitor disabled.
      #else

      RSTSRC = RSTSRC_PORSF__SET |
             RSTSRC_MCDRSF__SET;     // Enable missing clock detector and
                                       // leave VDD Monitor enabled.
      #endif
   #endif

   // If system clock is > 14 MHz, bypass (disable) the one-shot timer
   // Otherwise, system clock is <= 14 MHz, enable the one-shot timer
   #if(SYSCLK > 14000000)
      FLSCL |= FLSCL_BYPASS__SYSCLK;   // Set the BYPASS bit
   #else
      FLSCL &= ~FLSCL_BYPASS__BMASK;   // Clear the BYPASS bit
                                       // to enable the one-shot
   #endif


   #if  ((CLOCK_SELECTION & 0x0F) == PRECISION_OSC)

      REG0CN |= REG0CN_OSCBIAS__FORCE_ENABLED;
      OSCICN |= OSCICN_IOSCEN__ENABLED;

      while ((OSCICN & OSCICN_IFRDY__BMASK) == OSCICN_IFRDY__NOT_SET);

   #elif((CLOCK_SELECTION & 0x0F) == LOW_POWER_OSC)

      REG0CN &= ~REG0CN_OSCBIAS__BMASK; // Disable precision oscillator bias

   #elif((CLOCK_SELECTION & 0x0F) == LOW_POWER_OSC_DIV_8)

      REG0CN &= ~REG0CN_OSCBIAS__BMASK; // Disable precision oscillator bias

   #elif((CLOCK_SELECTION & 0x0F) == SMARTCLOCK_OSC)

      REG0CN &= ~REG0CN_OSCBIAS__BMASK; // Disable precision oscillator bias

      PMU0CF = PMU0CF_CLEAR__ALL_FLAGS; // Clear all wake-up source flags
                                        // This allows low power oscillator
                                        // to be turned off

      RTC_Init();

   #elif((CLOCK_SELECTION & 0x0F) == EXTERNAL_OSC)

      REG0CN &= ~REG0CN_OSCBIAS__BMASK;   // Disable precision oscillator bias

      PMU0CF = PMU0CF_CLEAR__ALL_FLAGS;   // Clear all wake-up source flags
                                          // This allows low power oscillator
                                          // to be turned off

      SFRPAGE = CONFIG_PAGE;

      P1MDIN = 0x02;                      // Set XTAL2 as a digital input

      SFRPAGE = LEGACY_PAGE;

      OSCXCN = OSCXCN_XOSCMD__CMOS;

//      OSCXCN = OSCXCN_XOSCMD__CRYSTAL | OSCXCN_XFCN__MODE6; // Start external oscillator
                                       // in crystal clock mode

     // while ((OSCXCN & OSCXCN_XCLKVLD__BMASK) == OSCXCN_XCLKVLD__NOT_SET);

   #endif

   CLKSEL = CLOCK_SELECTION;

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

   P0 = 0xFF;                          // Make all IO Analog Inputs
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

   P0MDIN &= ~0xC0;                    // P0.6 and P0.7 (RTC Pins) are
                                       // analog
   P0SKIP |= 0xC0;

   // Uncomment this section to send SYSCLK (24.5 MHz) out a port pin
   /*
   // SYSCLK to P0.1
   P0SKIP |= 0x01;  // P0.0 skipped
   P0MDIN |= 0x02;  // P0.1 is an output
   P0MDOUT |= 0x02; // P0.1 is push-pull


   XBR0 = XBR0_SYSCKE__ENABLED;        // Put SYSCLK onto Crossbar
   */
   XBR1 = XBR1_XBARE__ENABLED |
          XBR1_WEAKPUD__PULL_UPS_DISABLED;

   SFRPAGE = LEGACY_PAGE;
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
   #if (USE_SMARTCLOCK_LFO == 1)
      RTC_Write (RTC0XCN, RTC0XCN_BIASX2__DISABLED
                 | RTC0XCN_LFOEN__ENABLED);

      // Disable Auto Load Cap Stepping
      RTC_Write (RTC0XCF, RTC0XCF_AUTOSTP__DISABLED
                 | 0x07);

   #else
      RTC_Write (RTC0XCN, RTC0XCN_AGCEN__DISABLED
                 | RTC0XCN_XMODE__CRYSTAL
                 | RTC0XCN_BIASX2__ENABLED
                 | RTC0XCN_LFOEN__DISABLED);


      RTC_Write (RTC0XCF, RTC0XCF_AUTOSTP__ENABLED
                 | 0x01);

      RTC_Write (RTC0CN, RTC0CN_RTC0EN__ENABLED);


      //----------------------------------
      // Wait for smaRTClock to start
      //----------------------------------

      // Wait > 20 ms
      Wait_us(25000);

      // Wait for smaRTClock valid
      while ((RTC_Read (RTC0XCN) & RTC0XCN_CLKVLD__BMASK) == RTC0XCN_CLKVLD__NOT_SET);


      // Wait for Load Capacitance Ready
      while ((RTC_Read (RTC0XCF) & RTC0XCF_LOADRDY__BMASK) == RTC0XCF_LOADRDY__NOT_SET);

      //----------------------------------
      // smaRTClock has been started
      //----------------------------------

      RTC_Write (RTC0XCN, RTC0XCN_AGCEN__ENABLED
                 | RTC0XCN_XMODE__CRYSTAL
                 |  RTC0XCN_BIASX2__DISABLED
                 |  RTC0XCN_LFOEN__DISABLED);
   #endif


   RTC_Write (RTC0CN, RTC0CN_RTC0EN__ENABLED |
                      RTC0CN_MCLKEN__ENABLED);
   // Wait > 2 ms
   Wait_us(2500);

   PMU0CF = PMU0CF_CLEAR__ALL_FLAGS;

}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// RTC_Read
//-----------------------------------------------------------------------------
//
// Return Value : RTC0DAT
// Parameters   : 1) unsigned char reg - address of RTC register to read
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
// Insert <us> microsecond delays using an on-chip timer.
//
void Wait_us(uint16_t us)
{
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = LEGACY_PAGE;

   CKCON |= CKCON_T2ML__SYSCLK;

   // Setup Timer2 to overflow at a 1 MHz Rate
   TMR2RL = -(LOW_POWER_OSC_FREQ/1000000);
   TMR2 = TMR2RL;

   TMR2CN |= TMR2CN_TR2__RUN;

   while(us)
   {
      TMR2CN &= ~TMR2CN_TF2H__BMASK;
      while((TMR2CN & TMR2CN_TF2H__BMASK) == TMR2CN_TF2H__NOT_SET);
      us--;                            // Decrement remaining time
   }

   TMR2CN &= ~TMR2CN_TR2__BMASK;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
