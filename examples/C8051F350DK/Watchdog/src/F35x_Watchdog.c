//-----------------------------------------------------------------------------
// F35x_Watchdog.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// Use the Watchdog instead of disabling it!
//
// This program helps the user to learn about operating the Watch Dog Timer.
// The WDT is used to generate resets if the times between writes to the WDT
// update register (PCA0CPH2) exceed a specified limit. The WDT can be disabled
// and enabled in the software as needed. When enabled the PCA Module 2 acts as
// the WDT. This program resets the MCU when P1.0 switch is pressed. Also upon
// reset the LED blinks approximately five times faster when compared to before.
// The reset is caused due to a WDT overflow and can be confirmed by checking
// the value of the RSTSRC register where bit 3 is set to indicate a reset
// caused by WDT.
//
//
// How to Test:
// ------------
// 1) Ensure shorting blocks are installed on the following:
//    - J8 : Power selection, P1 9V or J4 Debugger
// 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1. After power the board, the red LED D3
//    is on.
// 3) Compile and download code to the C8051F350-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) The LED will blink at a rate of 10Hz. Once the switch is pressed and held
//    for a long enough time, the WDT will cause a reset, and the LED will
//    blink faster than before.
//
//
// Target:         C8051F35x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1 (JL)
//    - Updated descriptions
//    - 24 JUN 2014
//
// Release 1.0
//    -Initial Revision SM
//    -15 JULY 2006

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F350_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000 / 8      // SYSCLK frequency in Hz

SI_SBIT(LED, SFR_P0, 7);                 // LED='1' means ON
SI_SBIT(SW2, SFR_P1, 0);                 // SW2='0' means switch pressed

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void PCA_Init (void);
void Timer2_Init (int counts);
void Timer2_ISR (void);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
//
// The MAIN routine performs all the intialization, and then loops until the
// switch is pressed. When SW2 (P1.0) is pressed the code checks the RSTSRC
// register to make sure if the last reset is because of WDT.
//-----------------------------------------------------------------------------
void main (void)
{
   // Disable Watchdog timer
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;

   OSCILLATOR_Init ();                 // Initialize system clock to 24.5/8 MHz
   PCA_Init();                         // Intialize the PCA
   PORT_Init();                        // Initialize crossbar and GPIO

   // First check the PORSF bit.
   if ((RSTSRC & RSTSRC_PORSF__BMASK) == RSTSRC_PINRSF__NOT_SET)
   {
      // if PORSF is set, all other RSTSRC flags are invalid

      // Check if the last reset was due to the Watch Dog Timer
      if (RSTSRC == RSTSRC_WDTRSF__SET)
      {
         Timer2_Init (SYSCLK / 12 / 50); // Make LED blink at 50Hz

         IE_EA = 1;                      // Enable global interrupts

         while(1);                       // wait forever
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

   PCA0MD  &= ~PCA0MD_WDTE__ENABLED;

   PCA0L    = 0x00;                // Set lower byte of PCA counter to 0
   PCA0H    = 0x00;                // Set higher byte of PCA counter to 0
   
   PCA0CPL2 = 0xFF;                // Write offset for the WDT
   
   PCA0MD  |= PCA0MD_WDTE__ENABLED;// enable the WDT

   IE_EA = 1;                      // enable global interrupts


   //--------------------------------------------------------------------------
   // Main Application Loop/Task Scheduler
   //--------------------------------------------------------------------------

   while (1)
   {
      //----------------------------------------------------------------------
      // Task #1 - Check Port I/O
      //----------------------------------------------------------------------
      while(!SW2);                 // Force the MCU to stay in this task as
                                   // long as SW2 is pressed. This task must
                                   // finish before the watchdog timer
                                   // timeout expires.

      //----------------------------------------------------------------------
      // Task #2 - Reset Watchdog Timer
      //----------------------------------------------------------------------
      PCA0CPH2 = 0x00;             // Write a 'dummy' value to the PCA0CPH2
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
// This routine initializes the system clock to use the internal 24.5MHz / 8
// oscillator as its clock source.  Also enables missing clock detector reset.
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{

   // configure internal oscillator for its lowest frequency
   OSCICN = OSCICN_IOSCEN__ENABLED | OSCICN_IFCN__HFOSC_DIV_8;
   RSTSRC = 0x04;                  // enable missing clock detector
}

//-----------------------------------------------------------------------------
// PCA_Init
//-----------------------------------------------------------------------------
// Return Value : None
// Parameters   : None
// This routine initializes the PCA to use the SYSCLK / 12
// as its clock source.  It also sets the offset value by writing to PCA0CPL2.
//-----------------------------------------------------------------------------
void PCA_Init()
{
    PCA0CN   =  PCA0CN_CR__RUN;           // PCA counter enable
    PCA0MD  &= ~PCA0MD_WDTE__ENABLED;     // Watchdog timer disabled
    PCA0MD  &= ~PCA0MD_CPS__FMASK;        // Select Timebase
    PCA0MD  |= PCA0MD_CPS__SYSCLK_DIV_12; // System clock / 12
    PCA0CPL2 =  0xFF;                     // Offset value
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
// Return Value : None
// Parameters   : None
//
// This function configures the Crossbar and GPIO ports.
// P0.7   digital   push-pull     LED
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED;
   P0MDOUT |= 0x80;                // enable LED as a push-pull output
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
// Return Value : None
// Parameters   :
//   1)  int counts - calculated Timer overflow rate
//                    range is positive range of integer: 0 to 32767
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/48 as its time base.
//-----------------------------------------------------------------------------
void Timer2_Init (int counts)
{
   TMR2CN  = TMR2CN_TR2__STOP;         // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON = CKCON_T2MH__EXTERNAL_CLOCK |
           CKCON_T2ML__EXTERNAL_CLOCK; // Timer2 clocked based on T2XCLK;

   TMR2RL  = -counts;                  // Init reload values
   TMR2    = 0xffff;                   // set to reload immediately
   IE_ET2     = 1;                     // enable Timer2 interrupts
   TMR2CN_TR2 = 1;                     // start Timer2
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
// This routine changes the state of the LED whenever Timer2 overflows.
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                    // clear Timer2 interrupt flag
   LED = ~LED;                         // change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------