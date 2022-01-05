//-----------------------------------------------------------------------------
// F380_USB0_Bulk.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Device specific initialization routines.
//
//
// How To Test:    See Readme.txt
//
//
// Target:         C8051F38x
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.6
// Command Line:   See Readme.txt
// Project Name:   USB0_Bulk
//
// Release 2.0
//    -Rewrite (CM)
//    -02 NOV 2012
//
// Release 1.1
//    -All changes by BW
//    -1 SEP 2010
//    -Updated USB0_Init() to write 0x89 to CLKREC instead of 0x80
//
// Release 1.0
//    -Initial Revision (PD)
//    -04 JUN 2008
//

//-----------------------------------------------------------------------------
// Header Files
//-----------------------------------------------------------------------------

#include "c8051f3xx.h"
#include "F3xx_USB0_Descriptor.h"
#include "F3xx_USB0_InterruptServiceRoutine.h"
#include "F3xx_USB0_Register.h"
#include "F3xx_USB0_Bulk.h"

#if TARGET_MCU != MCU_F380
#error Invalid TARGET_MCU definition
#endif // TARGET_MCU != MCU_F380

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------

#define SYSCLK             24000000    // SYSCLK frequency in Hz

// USB clock selections (SFR CLKSEL)
#define USB_4X_CLOCK       0x00        // Select 4x clock multiplier, for USB
#define USB_INT_OSC_DIV_2  0x10        // Full Speed
#define USB_EXT_OSC        0x20
#define USB_EXT_OSC_DIV_2  0x30
#define USB_EXT_OSC_DIV_3  0x40
#define USB_EXT_OSC_DIV_4  0x50

// System clock selections (SFR CLKSEL)
#define SYS_INT_OSC        0x00        // Select to use internal oscillator
#define SYS_EXT_OSC        0x01        // Select to use an external oscillator
#define SYS_4X_DIV_2       0x02

//-----------------------------------------------------------------------------
// Static Function Prototypes
//-----------------------------------------------------------------------------

static void Sysclk_Init (void);
static void Port_Init (void);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

SI_SBIT(Led1, SFR_P2, 2);         // LED='1' means ON
SI_SBIT(Led2, SFR_P2, 3);

//-----------------------------------------------------------------------------
// Global Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// System_Init
//-----------------------------------------------------------------------------
// This function is declared in the header file F3xx_HIDtoUART.h and is
// called in the main(void) function.  It calls initialization routines
// local to this file.
//
//-----------------------------------------------------------------------------
void System_Init (void)
{
   PCA0MD &= ~0x40;                    // Disable watchdog timer

   Sysclk_Init ();                     // Initialize oscillator
   Port_Init ();                       // Initialize crossbar and GPIO

   VDM0CN = 0x80;                      // Enable VDD Monitor
   Delay ();                           // Wait for VDD Monitor to stabilize
   RSTSRC = 0x02;                      // Enable VDD Monitor as a reset source
}

//-----------------------------------------------------------------------------
// USB0_Init
//-----------------------------------------------------------------------------
// USB Initialization performs the following:
// - Initialize USB0
// - Enable USB0 interrupts
// - Enable USB0 transceiver
// - Enable USB0 with suspend detection
//
//-----------------------------------------------------------------------------
void USB0_Init (void)
{
   POLL_WRITE_BYTE (POWER,  0x08);     // Force Asynchronous USB Reset
   POLL_WRITE_BYTE (IN1IE,  0x03);     // Enable Endpoint 0-1 in interrupts
   POLL_WRITE_BYTE (OUT1IE, 0x02);     // Enable Endpoint 1 out interrupts
   POLL_WRITE_BYTE (CMIE,   0x0F);     // Enable SOF, Reset, Resume, and
                                       // Suspend interrupts

   USB0XCN = 0xE0;                     // Enable transceiver; select full speed
   POLL_WRITE_BYTE (CLKREC, 0x89);     // Enable clock recovery, single-step
                                       // mode disabled

   EIE1 |= 0x02;                       // Enable USB0 Interrupts

                                       // Enable USB0 by clearing the USB
   POLL_WRITE_BYTE (POWER, 0x01);      // Inhibit Bit and enable suspend
                                       // detection
}

//-----------------------------------------------------------------------------
// USB0_Suspend
//-----------------------------------------------------------------------------
// Enter suspend mode after suspend signalling is present on the bus
//
void USB0_Suspend(void)
{
   // Power Down

   IE_EA = 0;                    // Disable global interrupts
   XBR1 &= ~0x40;             // Disengage the crossbar

   USB0XCN &= ~0x40;          // Suspend USB Transceiver
   CLKSEL = 0x10;             // USB Clock now uses int osc / 2
   CLKMUL = 0x00;             // Disable Clock Multiplier
   REG01CN |= 0x10;           // Place voltage regulator in low power mode
   OSCICN |= 0x20;            // Force internal oscillator suspend

   NOP ();

   // Power Up
   // Code execution begins here when resume signaling is received.

   REG01CN &= ~0x10;          // Place voltage regulator in full power mode
   Sysclk_Init ();
   USB0XCN |= 0x40;           // Enable USB Transceiver

   XBR1 |= 0x40;              // Engage the crossbar
   IE_EA = 1;
}

//-----------------------------------------------------------------------------
// Delay
//-----------------------------------------------------------------------------
// Used for a small pause, approximately 80 us in Full Speed,
// and 1 ms when clock is configured for Low Speed
//
//-----------------------------------------------------------------------------
void Delay (void)
{
   int16_t x;

   for (x = 0; x < 500; x)
   {
      x++;
   }
}

//-----------------------------------------------------------------------------
// SetLed
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//                1) uint8_t led: specified which LED to set (1 or 2)
//                2) uint8_t on: 0 means off, 1 means on
//
// Set the state of the specified LED
//
//-----------------------------------------------------------------------------
void SetLed (uint8_t led, uint8_t on)
{
   if (led == 1)
   {
      if (on)
         Led1 = 1;
      else
         Led1 = 0;
   }
   else if (led == 2)
   {
      if (on)
         Led2 = 1;
      else
         Led2 = 0;
   }
}

//-----------------------------------------------------------------------------
// Static Global Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Sysclk_Init
//-----------------------------------------------------------------------------
// This function initializes the system clock and the USB clock.
// Full-speed System Clock: 24 MHz  USB Clock: 48 MHz
//
//-----------------------------------------------------------------------------
static void Sysclk_Init (void)
{
   OSCICN |= 0x03;                     // Configure internal oscillator for
                                       // its maximum frequency and enable
                                       // missing clock detector

   CLKMUL  = 0x00;                     // Select internal oscillator as
                                       // input to clock multiplier

   CLKMUL |= 0x80;                     // Enable clock multiplier
   Delay();                            // Delay for clock multiplier to begin
   CLKMUL |= 0xC0;                     // Initialize the clock multiplier
   Delay();                            // Delay for clock multiplier to begin

   while(!(CLKMUL & 0x20));            // Wait for multiplier to lock
   CLKSEL  = 0x02;                     // Set sys clock to clkmul / 2
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P2.2   digital   push-pull    LED1
// P2.3   digital   push-pull    LED2
//
//-----------------------------------------------------------------------------
static void Port_Init (void)
{
   P2MDOUT   = 0x0C; // P2.2 - P2.3 is push-pull
   P2SKIP    = 0x0C; // P2.2 - P2.3 skipped
   XBR1      = 0x40; // Enable the crossbar
}
