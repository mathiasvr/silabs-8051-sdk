//-----------------------------------------------------------------------------
// F53xA_Watchdog.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program helps the user to learn about operating the Watchdog Timer.
// The WDT is used to generate resets if the times between writes to the WDT
// update register (PCA0CPH2) exceed a specified limit. The WDT can be disabled
// and enabled in the software as needed. When enabled the PCA Module 2 acts as
// the WDT. This program resets the MCU when P1.4 switch is pressed. Also upon
// reset the LED blinks approximately five times faster when compared to before
// The reset is caused due to a WDT overflow and can be confirmed by checking
// the value of the RSTSRC register where bit 3 is set to indicate a reset
// caused by WDT.
//
// Pinout:
//
// P1.3 - LED
// P1.4 - Switch
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F530A TB are placed on the following:
//    "A" SIDE
//    HDR4: P1.3_A - P1.3_LED_A
//    HDR4: P1.4_A - P1.4_SW_A
//    "B" SIDE
//    HDR3: P1.3_B - P1.3_LED_B
//    HDR3: P1.4_B - P1.4_SW_B
// 2) Connect the USB Debug Adapter to HDR2 for "A" SIDE or HDR1 for "B" SIDE.
//    Connect the 9V power adapter to P5.
// 3) Compile and download code to C8051F530A target board
//    (either "A" SIDE or "B" SIDE) with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) The test will blink the LED at a rate of 10Hz until the switch
//    (P1.4) is pressed. Once the the switch is pressed and held for a long
//    enough time, it will prevent the WDT from being touched and the WDT will
//    cause a reset. Upon reset the code checks for a WDT reset and blinks the
//    LED five times faster than before to indicate the same.
//
//
// Target:         C8051F52xA/F53xA (C8051F530A TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.3 / 09 SEP 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.2 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.1
//    -Updated for 'F530A TB (TP)
//    -31 JAN 2008
//
// Release 1.0
//    -Initial Revision (SM)
//    -10 JULY 2007


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F530_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000 / 8      // SYSCLK frequency in Hz

SI_SBIT(LED, SFR_P1, 3);                 // LED='1' means ON
SI_SBIT(SW,  SFR_P1, 4);                 // SW='0' means switch pressed

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void PCA_Init (void);
void Timer2_Init (uint16_t counts);
SI_INTERRUPT_PROTO(TIMER2_ISR, TIMER2_IRQn);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
//
// The MAIN routine performs all the intialization, and then loops until the
// switch is pressed. When SW (P1.4) is pressed the code checks the RSTSRC
// register to make sure if the last reset is because of the Watchdog Timer.
//-----------------------------------------------------------------------------
void main (void)
{
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable the watchdog timer

   OSCILLATOR_Init ();                 // Initialize the system clock
   PCA_Init();                         // Intialize the PCA
   PORT_Init();                        // Initialize crossbar and GPIO

   if ((RSTSRC & 0x02) == 0x00)        // First check the PORSF bit. if PORSF
   {                                   // is set, all other RSTSRC flags are
                                       // invalid
      // Check if the last reset was due to the Watchdog Timer
      if (RSTSRC == RSTSRC_WDTRSF__SET)
      {
         Timer2_Init (SYSCLK / 12 / 50); // Make LED blink at 50Hz

         IE_EA = 1;                    // Enable global interrupts
         while(1);                     // Wait forever
      }
      else
      {
         // Init Timer2 to generate interrupts at a 10Hz rate.
         Timer2_Init (SYSCLK / 12 / 10);
      }

   }
   // Calculate Watchdog Timer Timeout
   // Offset calculated in PCA clocks
   // Offset = ( 256 x PCA0CPL2 ) + 256 - PCA0L
   //        = ( 256 x 255(0xFF)) + 256 - 0
   // Time   = Offset * (12/SYSCLK)
   //        = 255 ms ( PCA uses SYSCLK/12 as its clock source)

   PCA0MD  &= ~0x40;                   // WDTE = 0 (clear watchdog timer
                                       // enable)
   PCA0L = 0x00;                       // Set lower byte of PCA counter to 0
   PCA0H = 0x00;                       // Set higher byte of PCA counter to 0
   PCA0CPL2 = 0x0F;                    // Write offset for the WDT
   PCA0MD |= 0x40;                     // Enable the WDT
   IE_EA = 1;                          // Enable global interrupts


   //--------------------------------------------------------------------------
   // Main Application Loop/Task Scheduler
   //--------------------------------------------------------------------------

   while (1)
   {
      //-----------------------------------------------------------------------
      // Task #1 - Check Port I/O
      //-----------------------------------------------------------------------
      while(!SW);                      // Force the MCU to stay in this task as
                                       // long as SW2 is pressed. This task
                                       // must finish before the watchdog timer
                                       // timeout expires.

      //-----------------------------------------------------------------------
      // Task #2 - Reset Watchdog Timer
      //-----------------------------------------------------------------------
      PCA0CPH2 = 0x00;                 // Write a 'dummy' value to the PCA0CPH2
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
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24.5MHz / 8
// oscillator as its clock source.  Also enables missing clock detector reset.
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   // Configure internal oscillator for 24.5 MHz / 8
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_8;
   RSTSRC = 0x04;                      // Enable missing clock detector
}

//-----------------------------------------------------------------------------
// PCA_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the PCA to use the SYSCLK / 12
// as its clock source.  It also sets the offset value by writing to PCA0CPL2.
//-----------------------------------------------------------------------------
void PCA_Init()
{
   PCA0CN =  0x40;                     // PCA counter enable
   PCA0MD &= ~0x40 ;                   // Watchdog timer disabled
   PCA0MD &=  0xF1;                    // Timebase selected - System clock / 12
   PCA0CPL2 = 0xFF;                    // Offset value
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Crossbar and GPIO ports.
//
// P1.3   digital   push-pull     LED
// P1.3   digital   open-drain    Switch (SW)
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P1MDOUT |= 0x08;                    // Enable LED as a push-pull output

   // Enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED |
          XBR1_XBARE__ENABLED;
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1)  uint16_t counts - calculated Timer overflow rate
//                    range is positive range of integer: 0 to 65535
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/48 as its time base.
//-----------------------------------------------------------------------------
void Timer2_Init (uint16_t counts)
{
   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2
                                       // Use SYSCLK/12 as timebase
   CKCON &= ~0x60;                     // Timer2 clocked based on T2XCLK

   TMR2RL = -counts;                   // Init reload values
   TMR2 = 0xffff;                      // Set to reload immediately
   IE_ET2 = 1;                         // Enable Timer2 interrupts
   TMR2CN_TR2 = 1;                     // Start Timer2
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
// This routine changes the state of the LED whenever Timer2 overflows.
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                    // Clear Timer2 interrupt flag
   LED = ~LED;                         // Change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------