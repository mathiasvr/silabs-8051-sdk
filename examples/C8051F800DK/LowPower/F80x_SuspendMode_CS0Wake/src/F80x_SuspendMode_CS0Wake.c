//-----------------------------------------------------------------------------
// F80x_CS0.c
//-----------------------------------------------------------------------------
// Copyright (C) 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program configures the CS0 to autoscan on channels 5 thru 6 before
// switching the device to suspend mode.  When a button is pressed, the
// device wakes up and outputs the button being pressed through UART.
//
//-----------------------------------------------------------------------------
// How To Test:
//-----------------------------------------------------------------------------
// 1) Ensure that jumpers are placed on the following:
//    - J2
//    - J3: TX_MCU - P0.4
//    - J3: RX_MCU - P0.5
// 2) Connect the USB Debug Adapter to J4.
// 4) Compile and download code to a 'F80x device by selecting Run -> Debug
//    from the menus, clicking the Debug button in the quick menu, or pressing F11.
// 6) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the USB port (virtual COM port) at <BAUDRATE> and 8-N-1.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Press the capacitive sense buttons, P1.5 and P1.6, and observe the output.
//
// Target:         C8051F80x
// Tool chain:     Simplicity Studio / Keil C51 9.51
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
#include <SI_C8051F800_Defs.h>            // SFR declarations
#include <stdio.h>


//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(LED,    SFR_P1, 0);              // '0' means ON, '1' means OFF

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK             24500000L
#define BAUDRATE           230400

#define LED_ON           0
#define LED_OFF          1

#define SWITCH_THRESHOLD 0x7000

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
volatile uint16_t delay;

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void SYSCLK_Init (void);
void PORT_Init (void);
void CS0_Init (void);
void PCA0_Init (void);
void UART0_Init (void);
SI_INTERRUPT_PROTO(CS0_ComparatorInterruptServiceRoutine, INTERRUPT_CS0_GRT);
void OSCILLATOR_Init (void);

//-----------------------------------------------------------------------------
// Generic UART definitions to support both Keil and SDCC
//-----------------------------------------------------------------------------

#ifdef SDCC
#include <sdcc_stdio.h>
#endif

char *GETS (char *, unsigned int);

#ifdef __C51__
#define GETS gets
#endif

#ifdef SDCC
#include <sdcc_stdio.c>
#endif

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
   uint16_t mux;
   PORT_Init ();                       // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator
   CS0_Init();                         // Initialize capacitive touch sense
   	   	   	   	   	   	   	   	   	   // to be used as a wake-up source
   SYSCLK_Init();					   // Initialize system clock to
   	   	   	   	   	   	   	   	   	   // 24.5MHz

   PCA0_Init();                        // Initialize PCA to control LED
   UART0_Init();                       // Initialize UART
   
   IE_EA = 1;                             // Enable global interrupts

   while (1)
   {
      OSCICN |= 0x20;                  // Place MCU in suspend mode
      for (delay = 0; delay < 0xFFF0; delay++);
      mux = (uint16_t)CS0MX;                     // Read CS0 mux value to output
      printf("Button Press on channel %x \n", (uint16_t)mux);
      for (delay = 0; delay < 0xFFF0; delay++);
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   OSCICN = 0x83;                      // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock
}

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

   CLKSEL = 0x00;                      // Select internal internal osc.
                                       // divided by 1 as the system clock
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
   PCA0CN    = 0x40;
   PCA0MD    = 0x00;
   PCA0CPM0  = 0x42;
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
// P1.1   digital   open-drain    Switch
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P0SKIP = 0xCF;                      // Skip all P0, route PCA to P1.0
   P0MDOUT |= 0x10;                    // Set P0.4 to push-pull output
   P1MDOUT |= 0x01;                    // P1.0 is push-pull
   P1MDIN &= ~0x60;                    // P1.5-P1.6 set to analog input for 
                                       // CS0
   XBR0 = 0x01;                        // Route UART
   XBR1 = 0x41;                        // Enable crossbar and enable
                                       // weak pull-ups, 1 PCA channel
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

   CS0SS = 0x0D;                       // Set channel 5 as autoscan 
                                       // starting channel
   CS0SE = 0x0E;                       // Set channel 6 as autoscan end channel
}


//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits
   #if (SYSCLK/BAUDRATE/2/256 < 1) 
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   #elif (SYSCLK/BAUDRATE/2/256 < 4) 
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x01;
   #elif (SYSCLK/BAUDRATE/2/256 < 12) 
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   #else 
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   #endif

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                            // START Timer1
   SCON0_TI = 1;                            // Indicate TX0 ready
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
         PCA0CPH0 = 0xF0;              // Set PCA duty cycle to change
         break;                        // LED's brightness
      case (0x0E):
         PCA0CPH0 = 0x40;
         break;
   }

   CS0CN |= 0x10;                      // Set CS0Busy to start another scan
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
