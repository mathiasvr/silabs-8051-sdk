//-----------------------------------------------------------------------------
// F70x_CS0.c
//-----------------------------------------------------------------------------
// Copyright (C) 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program uses the CS0 peripheral with autoscan to determine when
// one of the four touch sense buttons on the C8051F700 target board
// have been pressed.  When a button is pressed, the CS0 greater than
// comparator interrupt gets serviced.  Inside this interrupt, firmware
// checks the CS0MUX register to determine which of the four buttons 
// has been pressed.  Firmware then updates a PCA SFR that adjusts the
// duty cycle of the PWM that is outputting on the LED's pin.  This
// adjustment brightens or dims the LED.
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J8: P1.0 - DS3
//	  - J15: VDD - power source
// 2) Connect the USB Debug Adapter to P3
// 3) Connect the USB cable to P4.
// 4) Compile and download code to the 'F70x target board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Press the switches to change the brightness of the LED.
//
// Target:         C8051F70x
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (PD)
//    -14 JUL 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"             // compiler declarations
#include <SI_C8051F700_Defs.h>         // SFR declarations


//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(LED,    SFR_P1, 0);              // '0' means ON, '1' means OFF

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK           24500000L


#define LED_ON           0
#define LED_OFF          1

#define SWITCH_THRESHOLD 0x7000


//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void);
void PORT_Init (void);
void CS0_Init (void);
void PCA0_Init (void);
SI_INTERRUPT_PROTO(CS0_ComparatorInterruptServiceRoutine, INTERRUPT_CS0_GRT);

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
   WDTCN = 0xDE;                       // Disable watchdog timer
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{


   PORT_Init ();                       // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator
   CS0_Init();                         // Initialize capacitive touch sense
                                       // to be used as a wake-up source
   PCA0_Init();                        // Initialize PCA to control LED
   
   IE_EA = 1;                          // Enable global interrupts

   
   while (1);                          // Spin forever
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
// This function initializes the system clock to use the internal high power
// oscillator.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{   
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;

   OSCICN |= 0x80;                     // Enable the precision internal osc.
   CLKSEL = 0x00;                      // Select internal internal osc.
                                       // divided by 1 as the system clock

   SFRPAGE = SFRPAGE_save;

}

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes one PCA channel to 8-bit PWM mode.
//
//-----------------------------------------------------------------------------
void PCA0_Init(void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;
   PCA0CN    = 0x40;
   PCA0MD    = 0x00;
   PCA0CPM0  = 0x42;
   SFRPAGE = SFRPAGE_save;
}
//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P1.0   digital   push-pull     LED
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;

   P0SKIP = 0xFF;                      // Skip all P0, route PCA to P1.0
   P1MDOUT |= 0x01;                    // P1.0 is push-pull
   P2MDIN &= ~0x0F;                    // P2.0-P2.3 set to analog input for 
                                       // CS0
   XBR1    = 0x41;                     // Enable crossbar and enable
                                       // weak pull-ups, 1 PCA channel
   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// CS0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function enables capacitive touch sense peripheral CS0 and the
// CS0 comparator.  The function enables the auto-scan feature and configures
// the start of conversion source to be CS0BUSY.
//-----------------------------------------------------------------------------
void CS0_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE


   SFRPAGE = LEGACY_PAGE;

   CS0CF = 0x70;                       // Conversion enabled on CS0BUSY 
   									         // overflow
   CS0CN = 0x88;                       // Enable CS0, comparator 
   CS0CN |= 0x10;                      // Set CS0BUSY to begin conversions
   EIE2 |= 0x02;                       // Enable CS0 Greater than comp. interrupts
   CS0TH = SWITCH_THRESHOLD;           // Set comparator threshold value

   SFRPAGE = CONFIG_PAGE;
   CS0SS = 0x00;                       // Set channel 0 as autoscan 
                                       // starting channel
   CS0SE = 0x03;                       // Set channel 1 as autoscan end channel

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------
SI_INTERRUPT(CS0_ComparatorInterruptServiceRoutine, CS0DCMP_IRQn)
{
   static int16_t counter;
   CS0CN &= ~0x01;                     // Acknowledge interrupt

   switch (CS0MX)                      // Determine which button was pressed
   {
      case (0):
         PCA0CPH0 = 0xF0;              // Set PCA duty cycle to change
         break;                        // LED's brightness
      case (1):
         PCA0CPH0 = 0xB0;
         break;
      case (2):
         PCA0CPH0 = 0x80;
         break;
      case (3):
         PCA0CPH0 = 0x40;
         break;
   };
   CS0CN |= 0x10;                      // Set CS0Busy to start another scan
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
