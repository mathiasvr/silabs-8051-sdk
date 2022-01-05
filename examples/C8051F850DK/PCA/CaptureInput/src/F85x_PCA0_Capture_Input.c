//-----------------------------------------------------------------------------
// F85x_PCA0_Capture_Input.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program uses the PCA's capture mode to time events on an input pin.
// The difference between subsequent captures is calculated and stored in
// memory.
//
// In this example, PCA Module 0 is used to capture rising edges of an
// incoming digital waveform, and the crossbar is configured to receive the
// CEX0 pin on on P0.0.  The PCA is configured to clock once every 10 us.
//
// Timer2 overflows are used to toggle pin P0.1 at an overflow rate specified
// by the definition T2_RELOAD_CLOCKS. This signal at P0.1 is provided simply
// as a test method for the code.
//
//-----------------------------------------------------------------------------
// Configurator Settings:
//-----------------------------------------------------------------------------
//    Peripherals:
//       Clock Control:
//          SYSCLK/1 is selected as the clock source divider, with 
//          High-Frequency Oscillator selected as the source.
//
//       Interrupts:
//          Enabled All Interrupts is set, with PCA0 interrupts enabled.      
//
//       PCA:  
//          Channel 0 is set to Capture on positive edge of CEX0.
//          Capture/Compare Flag Interrupt is enabled.
//          PCA Counter/Timer Run Control is set to Start.
//
//       Reset Sources:
//          Missing clock detector is enabled as a reset source.
//
//       TIMER0/1:
//          
//          
//       TIMER2:
//          SYSCLK is selected as Clock Source
//          Target Overflow Frequency is set to 4000 (4kHz)
//          Timer Init value is set equal to the reload value (0xE813)
//          Run control is set to Start.
//
//       TIMER Setup:
//       
//
//       WatchDog Timer:
//          Watchdog timer is disabled.
//
//    Port I/O:
//       Crossbar is enabled, with Pull-ups enabled.
//       P0.0 is configured as a Digital Push-Pull Output.
//        
//    Crossbar0:
//        PCA0 - CEX0 is routed to the first available pin (P0.0).
//
//-----------------------------------------------------------------------------
// How To Test: UPMU-F850
//-----------------------------------------------------------------------------
//
// 1) Ensure shorting blocks are place on the following:
//    - JP2
// 2) Short P0.0 and P0.1 together with an external wire.
// 3) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 4) Compile and download code to the 'F85x UDP MCU board by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 5) Set a breakpoint in the PCA0_ISR function.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
//		Let it halt at the breakpoint.
// 7) Verify that the variable "capture_period" is equal to 50 (=500us).
//
// Alternately:
//
// 1) Ensure shorting blocks are place on the following:
//    - JP2
// 2) Run a separate signal generator into the P0.0 pin.
// 3) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 4) Compile and download code to the 'F85x UDP MCU board by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 5) Set a breakpoint in the PCA0_ISR function.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
//		Let it halt at the breakpoint.
// 7) Verify that the variable "capture_period" matches the expected time
//    between rising edges (in units of 10us).
//
//
//-----------------------------------------------------------------------------
// How To Test: ToolStick F85x
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Short P0.0 and P0.1 together with an external wire.
// 3) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 4) Compile and download code to the ToolStick F85x board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Set a breakpoint in the PCA0_ISR function.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//    Let it halt at the breakpoint.
// 7) Verify that the variable "capture_period" is equal to 50 (=500us).
//
// Alternately:
//
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Run a separate signal generator into the P0.0 pin.
// 3) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 4) Compile and download code to the ToolStick F85x board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Set a breakpoint in the PCA0_ISR function.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//    Let it halt at the breakpoint.
// 7) Verify that the variable "capture_period" matches the expected time
//    between rising edges (in units of 10us).
//
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.2 (BL)
//		- Updated How To Test
//		- 2 JAN 2014
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 15 JAN 2013
//
// Theory Of Operation
// ===========================================================================
//
// The PCA will capture rising edges on the CEX0 pin, and interrupt the
// processor.  Software subtracts the previous capture value from the most
// recent capture value to set the "captured_clocks" variable.
//
// The PCA time base in this example is configured to use Timer 0, overflowing
// every 10 us. The PCA clock rate will determine the maximum time between
// edges that can be reliably detected (this is equal to 65535 * the PCA clock).
//
// The time range that can be measured using this example is 10us (MIN) to
// 655.350 ms (MAX) when the clocks are set up as they are in this example.
// The precision is limited to the PCA clock rate (in this example, 10 us).
// Using different PCA clock sources or a different processor clock will allow
// for different time capture ranges or levels of precision.
//
//    -------------------------------------------------------------------------
//    How "Capture Mode" Works:
//
//    The PCA's Capture Mode works by making a copy of the current PCA timer
//    value (PCA0) into the capture/compare register for the module (PCA0CPn).
//    The module can be configured to capture rising edges, falling edges, or
//    both rising and falling edges.  When a capture occurs, the CCFn flag is
//    set, and an interrupt is (optionally) generated by the PCA module.
//
//    To detect an edge (and not reject it as noise) the new logic level must
//    be present at the pin for at least two SYSCLK cycles.
//
//    With a constantly-running PCA timer, the time between two edges on a
//    signal can be accurately measured to within 1 PCA clock cycle.
//    Multiple PCA channels can be used in capture mode to measure timing
//    between different signals.
//    -------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>
#include "InitDevice.h"
//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define T0_CLOCKS         245          // Use 245 clocks per T0 Overflow
                                       //    (245 = 10 us)
#define T2_RELOAD_CLOCKS 6125          // SYSCLKs per 1/2 cycle square wave
                                       //    (245 = 10 us)

SI_SBIT(TEST_OUT, SFR_P0, 1);            // Output for testing purposes
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

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
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   enter_DefaultMode_from_RESET();
   IE_EA = 1;

   while (1)
   {
      // check to see if Timer2 overflowed
      if (TMR2CN_TF2H)
      {
         TEST_OUT = ~TEST_OUT;         // Toggle test pin
         TMR2CN_TF2H = 0;                     // Clear T2 overflow
      }
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
