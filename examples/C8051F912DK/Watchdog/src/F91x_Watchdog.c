//-----------------------------------------------------------------------------
// F91x_Watchdog.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program helps the user to learn about operating the Watch Dog Timer.
// The WDT is used to generate resets if the times between writes to the WDT
// update register (PCA0CPH5) exceed a specified limit. The WDT can be disabled
// and enabled in the software as needed. When enabled the PCA Module 2 acts as
// the WDT. This program resets the MCU when P0.2 switch is pressed. Also upon
// reset the LED blinks approximately five times faster when compared to before.
// The reset is caused due to a WDT overflow and can be confirmed by checking
// the value of the RSTSRC register where bit 3 is set to indicate a reset
// caused by WDT.
//
// How to Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J12:  P0.2/SW0.2
//             P1.6/LED1.6
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J4.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F91x device on a C8051F91x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8:
//        - The test will blink the LED at a rate of 10Hz until the switch SW2
//          (P0.2) is pressed.
//        - Once the the switch is pressed and held for a int32_t enough time,
//          it will prevent the WDT from being touched and the WDT will
//          cause a reset.
//        - Upon reset the code checks for a WDT reset and blinks the LED five
//          times faster than before to indicate the same.
//
// Target:         C8051F90x-C8051F91x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 14 JAN 2014
//
// Release 1.2 
//    - Compiled and tested for Raisonance Toolchain (FB)
//    - 17 MAY 10
//
// Release 1.1
//    - Port to C8051F912 from C8051F930 (JH) 
//    - Compile and test C8051F912_defs.h on 912 board (JH)
//    - 07 JULY 2009
//
// Release 1.0
//    -Initial Revision FB
//    -19 NOV 2007

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000 / 8      // SYSCLK frequency in Hz

SI_SBIT(LED, SFR_P1, 6);                 // LED==1 means ON
SI_SBIT(SW2, SFR_P0, 2);                 // SW2==0 means switch depressed

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void PCA_Init (void);
void Timer2_Init (int16_t counts);
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
   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
                                       // enable)

   OSCILLATOR_Init ();                 // Initialize system clock to 24.5/8 MHz
   PCA_Init();                         // Intialize the PCA
   PORT_Init();                        // Initialize crossbar and GPIO

   if ((RSTSRC & 0x02) == 0x00)        // First check the PORSF bit. if PORSF
   {                                   // is set, all other RSTSRC flags are
                                       // invalid
      // Check if the last reset was due to the Watch Dog Timer
      if (RSTSRC == 0x08)
      {
         Timer2_Init (SYSCLK / 12 / 50); // Make LED blink at 50Hz
         IE_EA = 1;                         // Enable global interrupts

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
   // Offset = ( 256 x PCA0CPL5 ) + 256 - PCA0L
   //        = ( 256 x 255(0xFF)) + 256 - 0
   // Time   = Offset * (12/SYSCLK)
   //        = 255 ms ( PCA uses SYSCLK/12 as its clock source)

   PCA0MD  &= ~0x40;                   // WDTE = 0 (clear watchdog timer
                                       // enable)
   PCA0L    = 0x00;                    // Set lower byte of PCA counter to 0
   PCA0H    = 0x00;                    // Set higher byte of PCA counter to 0
   PCA0CPL5 = 0xFF;                    // Write offset for the WDT
   PCA0MD  |= 0x40;                    // enable the WDT
   IE_EA = 1;                             // enable global interrupts

   //--------------------------------------------------------------------------
   // Main Application Loop/Task Scheduler
   //--------------------------------------------------------------------------

   while (1)
   {
       //----------------------------------------------------------------------
       // Task #1 - Check Port I/O
       //----------------------------------------------------------------------
       while(!SW2);                    // Force the MCU to stay in this task as
                                       // int32_t as SW2 is pressed. This task must
                                       // finish before the watchdog timer
                                       // timeout expires.

       //----------------------------------------------------------------------
       // Task #2 - Reset Watchdog Timer
       //----------------------------------------------------------------------
       PCA0CPH5 = 0x00;                // Write a 'dummy' value to the PCA0CPH5
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
// This routine initializes the system clock to use the internal 24.5MHz / 8
// oscillator as its clock source.  Also enables missing clock detector reset.
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{

   OSCICN |= 0x80;                     // Enable the precision internal osc.
      
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x30;                      // Select precision internal osc. 
                                       // divided by 8 as the system clock
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
    PCA0CN     =  0x40;                // PCA counter enable
    PCA0MD    &= ~0x40 ;               // Watchdog timer disabled-clearing bit 6
    PCA0MD    &=  0xF1;                // timebase selected - System clock / 12
    PCA0CPL5   =  0xFF;                // Offset value
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
// Return Value : None
// Parameters   : None
//
// This function configures the Crossbar and GPIO ports.
// P1.6   digital   push-pull     LED
//-----------------------------------------------------------------------------
void PORT_Init (void)
{

   XBR0     = 0x00;                    // no digital peripherals selected
   XBR2     = 0x40;                    // Enable crossbar and weak pull-ups
   P1MDOUT |= 0x40;                    // enable LED as a push-pull output
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
// Return Value : None
// Parameters   :
//   1)  int16_t counts - calculated Timer overflow rate
//                    range is positive range of integer: 0 to 32767
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/48 as its time base.
//-----------------------------------------------------------------------------
void Timer2_Init (int16_t counts)
{
   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON  &= ~0x30;                    // Timer2 clocked based on TMR2CN_T2XCLK;
   TMR2RL  = -counts;                  // Init reload values
   TMR2    = 0xffff;                   // set to reload immediately
   IE_ET2     = 1;                        // enable Timer2 interrupts
   TMR2CN_TR2     = 1;                        // start Timer2
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
   TMR2CN_TF2H = 0;                           // clear Timer2 interrupt flag
   LED = !LED;                         // change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
