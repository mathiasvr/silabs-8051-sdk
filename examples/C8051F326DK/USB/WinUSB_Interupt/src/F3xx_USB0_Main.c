//-----------------------------------------------------------------------------
// F3xx_USB_Main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// USB interrupt transfer type example.
//
//
// How To Test:    See Readme.txt
// ------------
// 1) Import the project file to Simplicity Studio.
// 2) Ensure shorting blocks on C8051F326-TB are place on the following:
//    J2: VDD  - +3VD
//    J3: P2.2 - P2.2_LED
//    J3: P2.3 - P2.3_LED
//    J7: VDD  - VIO
//    J8: Power selection, P1 9V or J4 Debugger
// 3) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1.
// 4) Compile and download code to C8051F326 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 6) Run the GUI application SLAB_USB_Interrupt that comes with the firmware
//    and test the features.
//
//
// Target:         C8051F326/7
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
// Project Name:   USB0_Interrupt
//
//
// Release 2.1 (JL)
//    - Updated description.
//    - 20 MAY 2014
//
// Release 2.0
//    -Rewrite (CM)
//    -02 NOV 2012
//
// Release 1.1
//    -Changed "Usb_Init" to "USB_Init" (TP)
//    -07 OCT 2010
//
// Release 1.0
//    -Initial Revision (PD)
//    -04 JUN 2008
//
//-----------------------------------------------------------------------------
// Header Files
//-----------------------------------------------------------------------------

#include "c8051f3xx.h"
#include "F3xx_USB0_Register.h"
#include "F3xx_USB0_InterruptServiceRoutine.h"
#include "F3xx_USB0_Descriptor.h"
#include "F3xx_USB0_Main.h"

//-----------------------------------------------------------------------------
// Interrupt Prototypes
//-----------------------------------------------------------------------------

SI_INTERRUPT_PROTO(Timer2_ISR, TIMER2_IRQn);
SI_INTERRUPT_PROTO(ADC0_ConvComplete_ISR, ADC0EOC_IRQn);
SI_INTERRUPT_PROTO(Usb_ISR, INTERRUPT_USB0);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// IN_PACKET and OUT_PACKET buffer bytes immediately before and
// after they are transferred across USB inside the report handlers

SI_SEGMENT_VARIABLE(In_Packet[IN_EP1_PACKET_SIZE], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(Out_Packet[OUT_EP1_PACKET_SIZE], uint8_t, SI_SEG_XDATA);

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
  // Disable the watchdog here
}
 
//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main(void)
{
   uint16_t i;

   System_Init ();                     // Initialize Sysclk, Port IO, Timer, ADC0
   USB0_Init ();                       // Initialize USB0

   // Initialize packets to 0
   for (i = 0; i < sizeof (In_Packet); i++)
      In_Packet[i] = 0;
   for (i = 0; i < sizeof (Out_Packet); i++)
      Out_Packet[i] = 0;

   IE_EA = 1;                          // Enable global interrupts

   while (1)
   {
      // It is possible that the contents of the following packets can change
      // while being updated.  This doesn't cause a problem in the sample
      // application because the bytes are all independent.  If data is NOT
      // independent, packet update routines should be moved to an interrupt
      // service routine, or interrupts should be disabled during data updates.

      // Update LED states
      if (Out_Packet[0] == 1) SetLed(1, 1);
      else                    SetLed(1, 0);
      if (Out_Packet[1] == 1) SetLed(2, 1);
      else                    SetLed(2, 0);

      // Set Port 0 [4-7] pins
      P0 &= ~0xF0;
      P0 |= (Out_Packet[2] & 0xF0);

      In_Packet[0] = Switch1State;        // Send status of switch 1
      In_Packet[1] = Switch2State;        // and switch 2 to host
      In_Packet[2] = (P0 & 0x0F);         // Port 0 [0-3]
      In_Packet[3] = Potentiometer;       // Potentiometer value
      In_Packet[4] = Temperature;         // Temperature sensor value
   }
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------