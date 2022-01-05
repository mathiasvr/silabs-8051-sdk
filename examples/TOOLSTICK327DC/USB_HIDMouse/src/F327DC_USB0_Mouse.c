//-----------------------------------------------------------------------------
// F326_USB0_Mouse.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Source file for routines that mouse data.
//
//
// How To Test:    See Readme.txt
//
//
// Target:         ToolStick327DC
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Project Name:   TS_F326-7_USB_HIDMouse
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 20 MAY 2014
//
// Release 1.0
//    -Initial Revision (PD)
//    -27 MAR 2008
//

#include "F327DC_USB0_Mouse.h"
#include "SI_C8051F326_Register_Enums.h"
#include "F327DC_USB0_InterruptServiceRoutine.h"
#include "F327DC_USB0_Register.h"

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------

#define SYSCLK             12000000    // SYSCLK frequency in Hz

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
// 16-bit SFR Definitions for 'F326
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Local Definitions
//-----------------------------------------------------------------------------

#define Sw1 0x01                       // These are the port2 bits for Sw1

//-----------------------------------------------------------------------------
// Global Variable Declarations
//-----------------------------------------------------------------------------
signed char MOUSE_VECTOR;
uint8_t MOUSE_AXIS;
uint8_t MOUSE_BUTTON;
uint8_t IN_PACKET[4];

//-----------------------------------------------------------------------------
// Local Variable Declarations
//-----------------------------------------------------------------------------

uint8_t SWITCH_1_STATE = 0;      // Indicate status of switch

uint8_t TOGGLE1 = 0;             // Variable to make sure each button
uint8_t ONESHOT = 0;             // Ensures one direction change per
                                       // button press

//-----------------------------------------------------------------------------
// Local Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(Timer1_ISR, 3);

void Sysclk_Init(void);
void Port_Init(void);
void USB0_Init(void);
void Timer_Init(void);
void Delay(void);

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer1_ISR
//-----------------------------------------------------------------------------
// Called when timer 1 overflows, check to see if switch is pressed,
// then watch for release.
// This routine sets the Mouse_... variables for the report handler.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer1_ISR, 3)
{
   static signed char Mouse_Direction = 1;
   static uint8_t Mouse_StateMachine = 0;

   if (!(P0 & Sw1))                    // Check for switch #1 pressed
   {
         // The code below sets a flag once per button press
         // This flag will cause the mouse movement to
         // rotate by 90 degrees
         if(TOGGLE1 == 0)
         {
            ONESHOT = 1;
            TOGGLE1 = 1;
         }
   }
   else
   {
      TOGGLE1 = 0;                     // Reset toggle variable
   }

   if (ONESHOT == 1)
   {
      ONESHOT = 0;                     // Clear flag

      // Move to next mouse vector movement state
      if(++Mouse_StateMachine == 4) Mouse_StateMachine = 0;

      // each state sets the mouse to turn 90 degrees clockwise
      switch (Mouse_StateMachine)
      {
      case(0):
         // positive X direction
         Mouse_Direction = 1;
         MOUSE_AXIS = X_Axis;
      break;
      case(1):
         // negative Y direction
         Mouse_Direction = -1;
         MOUSE_AXIS = Y_Axis;
      break;
      case(2):
         // negative X direction
         Mouse_Direction = -1;
         MOUSE_AXIS = X_Axis;
      break;
      case(3):
         // positive Y direction
         Mouse_Direction = 1;
         MOUSE_AXIS = Y_Axis;
      break;
      }
   }

   // Sets Mouse Vector using state machine setting above
   MOUSE_VECTOR = (1 * ((100 >> 5)+1)) * Mouse_Direction;

   TCON_TF1 = 0;                              // Clear Timer2 interrupt flag
}

//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// USB0_Init
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// - Initialize USB0
// - Enable USB0 interrupts
// - Enable USB0 transceiver
// - Enable USB0 with suspend detection
//
// ----------------------------------------------------------------------------
void USB0_Init(void)
{

   POLL_WRITE_BYTE(POWER,  0x08);      // Force Asynchronous USB Reset
   POLL_WRITE_BYTE(IN1IE,  0x03);      // Enable Endpoint 0-1 in interrupts
   POLL_WRITE_BYTE(OUT1IE, 0x02);      // Enable Endpoint 0-1 out interrupts
   POLL_WRITE_BYTE(CMIE,   0x07);      // Enable Reset, Resume, and Suspend
                                       // interrupts
   USB0XCN = 0xE0;                     // Enable transceiver; select full speed
   POLL_WRITE_BYTE(CLKREC, 0x80);      // Enable clock recovery, single-step
                                       // mode disabled

   EIE1 |= 0x02;                       // Enable USB0 Interrupts
   IE_EA = 1;                             // Global Interrupt enable
                                       // Enable USB0 by clearing the USB
   POLL_WRITE_BYTE(POWER,  0x01);      // Inhibit Bit and enable suspend
                                       // detection

}

//-----------------------------------------------------------------------------
// System_Init(void)
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// This top-level initialization routine calls all support routine.
//
// ----------------------------------------------------------------------------
void System_Init(void)
{
   Sysclk_Init();
   Port_Init();
   Timer_Init();
}

//-----------------------------------------------------------------------------
// Sysclk_Init
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Initialize system clock to maximum frequency.
//
// ----------------------------------------------------------------------------
void Sysclk_Init(void)
{

	OSCICN |= 0x82;
	CLKMUL  = 0x00;
   CLKMUL |= 0x80;                     // Enable clock multiplier

	Delay();

   CLKMUL |= 0xC0;                     // Initialize the clock multiplier

	while(!(CLKMUL & 0x20));            // Wait for multiplier to lock

   CLKSEL = 0x02;                      // Use Clock Multiplier/2 as system clock

}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// - Configure the Crossbar and GPIO ports.
//
// ----------------------------------------------------------------------------
void Port_Init(void)
{

   P2MDOUT |= 0x0C;                    // enable LEDs as a push-pull outputs

}

//-----------------------------------------------------------------------------
// Timer_Init
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// - Timer 2 reload, used to check if switch pressed on overflow and
// used for ADC continuous conversion
//
// ----------------------------------------------------------------------------
void Timer_Init(void)
{

   TCON      = 0x40;
   TMOD      = 0x20;
   CKCON     = 0x02;

   TH1       = 0x00;                   // set reload value
   IE        = 0x08;                   // enable timer interrupt

}

//-----------------------------------------------------------------------------
// Delay
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Used for a small pause, approximately 80 us in Full Speed,
// and 1 ms when clock is configured for Low Speed
//
// ----------------------------------------------------------------------------
void Delay(void)
{
   int16_t x;
   for(x = 0;x < 500;x)
      x++;
}
