//-----------------------------------------------------------------------------
// F500_Watchdog.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program helps the user to learn about operating the Watch Dog Timer.
// The WDT is used to generate resets if the times between writes to the WDT
// update register (PCA0CPH5) exceed a specified limit. The WDT can be disabled
// and enabled in the software as needed. When enabled the PCA Module 5 acts as
// the WDT. This program resets the MCU when P1.4 switch is pressed. Also upon
// reset the LED blinks approximately five times faster when compared to
// before.The reset is caused due to a WDT overflow and can be confirmed by 
// checking the value of the RSTSRC register where bit 3 is set to indicate a 
// reset caused by WDT.
//
// Resources Used:
// * Internal Oscillator: MCU clock source
// * Timer 2: Blinks LED
// * PCA Module 5 : Watchdog Timer
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F500-TB are placed on the following:
//    J19: P1.3 LED
//    J19: P1.4 - P1.4_SW
//    J7: Power selection, P4 9V (+5VREG) or P2 Debugger (SER_PWR)
// 2) Connect the USB Debug Adapter to P2. If +5VREG is selected on J7,
//    connect the 9V power adapter to P2.
// 3) Compile and download code to C8051F500 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) The test will blink the LED at a rate of 10Hz until the switch SW1
//    (P1.4) is pressed. Once the the switch is pressed and held for a long
//    enough time, it will prevent the WDT from being touched and the WDT will
//    cause a reset. Upon reset the code checks for a WDT reset and blinks the
//    LED five times faster than before to indicate the same.
//
//
// Target:         C8051F50x/1x (Side A of a C8051F500-TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.3 / 11 AUG 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.2 / 02 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.1 / 11 JUN 2008 (ADT)
//    -Edited Formatting
//
// Release 1.0 / 03 MAR 2008 (GP)
//    -Initial Revision


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F500_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24000000 / 8      // SYSCLK frequency in Hz

SI_SBIT(LED, SFR_P1, 3);                 // LED == 1 means ON
SI_SBIT(SW1, SFR_P1, 4);                 // SW1 == 0 means switch depressed

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void PCA_Init (void);
void TIMER2_Init (uint16_t counts);

SI_INTERRUPT_PROTO(TIMER2_ISR, TIMER2_IRQn);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
//
// The MAIN routine performs all the intialization, and then loops until the
// switch is pressed. When SW2 (P0.7) is pressed the code checks the RSTSRC
// register to make sure if the last reset is because of WDT.
//-----------------------------------------------------------------------------
void main (void)
{
   SFRPAGE = LEGACY_PAGE;              // Set SFR Page for PCA0MD and RSTSRC

   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable the watchdog timer

   OSCILLATOR_Init ();                 // Initialize system clock to 24.5/8 MHz
   PCA_Init ();                         // Intialize the PCA
   PORT_Init ();                        // Initialize crossbar and GPIO

   // First check the PORSF bit.
   if ((RSTSRC & RSTSRC_PORSF__BMASK) == RSTSRC_PORSF__NOT_SET)
   {  // if PORSF is set, all other RSTSRC flags are invalid
      // Check if the last reset was due to the Watch Dog Timer
      if (RSTSRC == RSTSRC_WDTRSF__SET)
      {
         TIMER2_Init (SYSCLK / 12 / 50);    // Make LED blink at 50Hz
         IE_EA = 1;                         // Enable global interrupts

         while (1);                          // Wait forever
      }
      else
      {
         // Init Timer2 to generate interrupts at a 10Hz rate.
         TIMER2_Init (SYSCLK / 12 / 10);
      }
   }

   // Calculate Watchdog Timer Timeout
   // Offset calculated in PCA clocks
   // Offset = ( 256 x PCA0CPL5 ) + 256 - PCA0L
   //        = ( 256 x 255(0xFF)) + 256 - 0
   // Time   = Offset * (12/SYSCLK)
   //        = 255 ms ( PCA uses SYSCLK/12 as its clock source)

   PCA0MD  &= ~0x40;                   // Disable watchdog timer
   PCA0L    = 0x00;                    // Set lower byte of PCA counter to 0
   PCA0H    = 0x00;                    // Set higher byte of PCA counter to 0
   PCA0CPL5 = 0xFF;                    // Write offset for the WDT
   PCA0MD  |= 0x40;                    // Enable the WDT

   IE_EA = 1;                          // Enable global interrupts

   //--------------------------------------------------------------------------
   // Main Application Loop/Task Scheduler
   //--------------------------------------------------------------------------

   while (1)
   {
       //----------------------------------------------------------------------
       // Task #1 - Check Port I/O
       //----------------------------------------------------------------------
       while (!SW1);                 // Force the MCU to stay in this task as
                                    // long as SW1 is pressed. This task must
                                    // finish before the watchdog timer
                                    // timeout expires.

       //----------------------------------------------------------------------
       // Task #2 - Reset Watchdog Timer
       //----------------------------------------------------------------------
       PCA0CPH5 = 0x00;             // Write a 'dummy' value to the PCA0CPH5
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
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24 MHz / 8
// oscillator as its clock source.  Also enables missing clock detector reset.
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   // Configure internal oscillator for 24 MHz / 8
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_8;

   SFRPAGE = LEGACY_PAGE;

   RSTSRC = 0x04;                      // Enable missing clock detector

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PCA_Init
//-----------------------------------------------------------------------------
// Return Value : None
// Parameters   : None
//
// This routine initializes the PCA to use the SYSCLK / 12
// as its clock source.  It also sets the offset value by writing to PCA0CPL2.
//-----------------------------------------------------------------------------
void PCA_Init()
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   PCA0CN     =  0x40;                 // PCA counter enable
   PCA0MD    &= ~0x40 ;                // Watchdog timer disabled
   PCA0MD    &=  0xF1;                 // Timebase selected - System clock / 12
   PCA0CPL5   =  0xFF;                 // Offset value

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
// Return Value : None
// Parameters   : None
//
// This function configures the Crossbar and GPIO ports.
//
// P1.3   digital   push-pull     LED
// P1.4   digital   open-drain    SW1
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P1MDOUT |= 0x08;                    // Enable LED as a push-pull output
   // Enable crossbar and weak pull-ups
   XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED |
          XBR2_XBARE__ENABLED;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// TIMER2_Init
//-----------------------------------------------------------------------------
// Return Value : None
// Parameters   :
//   1)  uint16_t counts - calculated Timer overflow rate
//                    range is positive range of integer: 0 to 65535
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/48 as its time base.
//-----------------------------------------------------------------------------
void TIMER2_Init (uint16_t counts)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON  &= ~0x60;                    // Timer2 clocked based on T2XCLK;

   TMR2RL  = -counts;                  // Init reload values
   TMR2    = 0xFFFF;                   // Set to reload immediately
   IE_ET2     = 1;                     // Enable Timer2 interrupts
   TMR2CN_TR2 = 1;                     // Start Timer2

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED whenever Timer2 overflows.
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                    // Clear Timer2 interrupt flag
   LED  = !LED;                        // Change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------