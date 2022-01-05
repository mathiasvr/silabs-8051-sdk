//-----------------------------------------------------------------------------
// F70x_CS0.c
//-----------------------------------------------------------------------------
// Copyright (C) 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program configures the CS0 to autoscan on channels 0 thru 3 before
// switching the device to suspend mode.  When a button is pressed, the
// device wakes up and outputs the button being pressed through UART.
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//	  - J15: VDD - power source
//    - J12: P0.4 - RXD
//    - J12: P0.5 - TXD
// 2) Connect the USB Debug Adapter to P3.
// 3) Connect the USB cable to P4.
// 4) Compile and download code to the 'F70x target board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the virtual COM port at <BAUDRATE> and 8-N-1.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Press any of the CS0 switches and the MCU will wake up and output
//    which button is being pressed to the terminal program.
//
// Target:         C8051F70x
// Tool chain:     Simplicity Studio / Keil C51 9.53
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
#include <SI_C8051F700_defs.h>         // SFR declarations
#include <stdio.h>


//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(LED,    SFR_P1, 0);              // '0' means ON, '1' means OFF

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK           24500000L
#define BAUDRATE         230400

#define LED_ON           0
#define LED_OFF          1

#define SWITCH_THRESHOLD 0xE000//0x7000

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
volatile uint16_t delay;

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void);
void PORT_Init (void);
void CS0_Init (void);
void PCA0_Init (void);
void UART0_Init (void);
void SYSCLK_Init (void);
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

   uint16_t mux;

   SYSCLK_Init ();
   PORT_Init ();                       // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator
   CS0_Init ();                        // Initialize capacitive touch sense
                                       // to be used as a wake-up source
   PCA0_Init();                        // Initialize PCA to control LED
   UART0_Init ();
   
   IE_EA = 1;                          // Enable global interrupts

   
   //----------------------------------
   // Main Application Loop
   //----------------------------------
   while (1)
   {

      SFRPAGE = CONFIG_PAGE;

      OSCICN |= 0x20;                  // Place MCU in suspend mode
      for (delay = 0; delay < 0xFFF0; delay++);
      SFRPAGE = 0;
      mux = (uint16_t)CS0MX;                // Read CS0 mux value to output
      printf("Button Press on channel %x \n", (uint16_t)mux);
      for (delay = 0; delay < 0xFFF0; delay++);

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
// This routine initializes the system clock to use the internal 24.5MHz / 8
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0x83;                      // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock

   SFRPAGE = SFRPAGE_save;
}
//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at 24.5 MHz.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   OSCICN |= 0x80;                     // Enable the precision internal osc.

   //RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
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
// This function configures the crossbar and GPIO ports.
//
// P0.4  -  TX0  (UART0), Push-Pull, Digital
// P0.5  -  RX0  (UART0), Open-Drain, Digital
// P1.0  -  Skipped,     Push-Pull,  Digital (LED on Target Board)
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x10;                    // Make TX0 push-pull

   P1MDOUT = 0x01;                     // Make the LED push-pull
   P1SKIP = 0x01;                      // Skip the LED (P1.0)

   P2MDIN &= ~0x0F;                    // P2.0-P2.3 set to analog input for 
                                       // CS0

   XBR0 |= 0x01;                       // Enable UART on P0.4(TX) and P0.5(RX)
   XBR1 = 0x40;                        // Enable the XBAR and weak pull-ups

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
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configures the UART0 using Timer1, for <BAUDRATE> and 8-N-1. Once this is
// set up, the standard printf function can be used to output data.
//
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

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
   TCON_TR1 = 1;                       // START Timer1
   SCON0_TI = 1;                       // Indicate TX0 ready

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------
SI_INTERRUPT(CS0_ComparatorInterruptServiceRoutine, CS0DCMP_IRQn)
{
   static int16_t counter;
   CS0CN &= ~0x01;                     // Acknowledge interrupt

   CS0CN |= 0x10;                      // Set CS0Busy to start another scan
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
