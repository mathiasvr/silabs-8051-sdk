//-----------------------------------------------------------------------------
// F800DC_CapacitiveSense.c
//-----------------------------------------------------------------------------
// Copyright (C) 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program uses the CS0 peripheral with autoscan to determine when
// one of the two capacitive touch sense buttons on the C8051F800 daughter 
// card have been pressed.  When a button is pressed, the CS0 greater than
// comparator interrupt gets serviced.  Inside this interrupt, firmware
// checks the CS0MUX register to determine which of the two buttons 
// has been pressed.  Firmware then toggles the corresponding LED to 
// indicate the touch.
//
// How To Test:
// 1) Connect the ToolStick800DC to a ToolStick Base Adapter.
// 2) Compile and download code to the ToolStick Daughter Card by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 3) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 4) Touch the switches to change LED states. Note that a brief touch will
//    toggle the LED corresponding to the switch. A constant touch will appear 
//    to dim the LED (because it is being toggled quickly).
//
// Target:         C8051F80x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (PKC)
//    -12 OCT 2009
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"             // compiler declarations
#include <SI_C8051F800_Defs.h>
#include <SI_C8051F800_Register_Enums.h>



//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(LED1_0,   SFR_P1, 0);           // '0' means ON, '1' means OFF
SI_SBIT(LED1_1,    SFR_P1, 1);           // '0' means ON, '1' means OFF

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK             24500000L


#define LED_ON           0
#define LED_OFF          1

#define SWITCH_THRESHOLD 0x7000


//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void);
void PORT_Init (void);
void CS0_Init (void);
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
   PCA0MD &= ~0x40;                    // WDTE = 0 (disable watchdog timer)
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
   
   IE_EA = 1;                             // Enable global interrupts

   
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
   OSCICN |= 0x80;                     // Enable the precision internal osc.
   OSCICN |= 0x03;                     // IFCN=11b ( = div by 1)
   CLKSEL = 0x00;                      // Select internal internal osc.
                                       // divided by 1 as the system clock
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
// P1.1   digital   push-pull     LED
// P1.5   analog                  Capacitive Switch
// P1.6   analog                  Capacitive Switch
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P1MDOUT |= 0x03;                    // Set P1.0, P1.1 to push-pull
   P1MDIN &= ~0x60;                    // P1.5-P1.6 set to analog input for 
                                       // CS0
   XBR1    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
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
   CS0CF = 0x70;                       // Conversion enabled on CS0BUSY 
   									   // overflow
   CS0CN = 0x88;                       // Enable CS0, comparator 
   CS0CN |= 0x10;                      // Set CS0BUSY to begin conversions
   EIE2 |= 0x02;                       // Enable CS0 Greater than comp. interrupts
   CS0TH = SWITCH_THRESHOLD;           // Set comparator threshold value

   CS0SS = 0x0D;                       // Set channel P1.5 as autoscan 
                                       // starting channel
   CS0SE = 0x0E;                       // Set channel P1.6 as autoscan end channel
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------
SI_INTERRUPT(CS0_ComparatorInterruptServiceRoutine, CS0DCMP_IRQn)
{
   CS0CN &= ~0x01;                     // Acknowledge interrupt

   switch (CS0MX)                      // Determine which button was pressed
   {
      case (0x0D):
         LED1_0 = !LED1_0;             // Toggle LED at P1.0
         break;
      case (0x0E):
         LED1_1 = !LED1_1;             // Toggle LED at P1.1
         break;
      default:
         break;
   }
   CS0CN |= 0x10;                      // Set CS0Busy to start another scan
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
