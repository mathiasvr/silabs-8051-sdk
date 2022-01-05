//-----------------------------------------------------------------------------
// F93x_CapTouchSense_Tune.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program can be used to determine the effect of supply voltage
// on the sensitivity of capacitive touch sense switches. It prints
// the oscillation period to a UART terminal.
//
// When using touch sense switches, the oscillation period of the RC
// relaxation oscillator can be used to determine when the switch
// has been pressed. The oscillation period will increase when the
// switch capacitance increases (indicating a user's finger is placed
// on the switch).
//
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J12:  P0.4/TX
//             P0.5/RX
//             VDD/VIO
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect a USB cable from the development board P3 to a PC
// 3) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the USB port (virtual com port) at <230400> and 8-N-1
// 4) Connect the USB Debug Adapter to J9.
// 5) Turn the POWER switch (SW5) to the "ON" position.
// 6) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) Touch the "Touch Sense Switches" connected to P1.0 and P1.1 and see
//    the corresponding oscillation period displayed on the UART Terminal.
//    The oscillation period is given in system clocks.
//
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 15 JAN 2014
//
// Release 1.2
//    - Updated to be compatible with Raisonance Toolchain (FB)
//    - 14 APR 2010
//
// Release 1.1
//    - Compiled and tested for C8051F930-TB (JH)
//    - 06 JULY 2009
//
// Release 1.0
//    - Initial Revision (FB)
//    - 06 SEP 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F930_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(RED_LED,    SFR_P1, 5);          // '0' means ON, '1' means OFF
SI_SBIT(YELLOW_LED, SFR_P1, 6);          // '0' means ON, '1' means OFF
SI_SBIT(SW2,        SFR_P0, 2);          // SW2 == 0 means switch pressed
SI_SBIT(SW3,        SFR_P0, 3);          // SW3 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK      24500000           // SYSCLK frequency in Hz

#define SW20_THRESHOLD  1000           // Threshold for SW20
#define SW21_THRESHOLD  1000           // Threshold for SW21

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
// Timer2 count of SW20 relaxation oscillator transitions
SI_UU16_t SW20_Timer_Count;
SI_UU16_t SW21_Timer_Count;

// Touch Switch: SW20==0 means finger sensed; SW20==1 means finger not sensed.
uint8_t SW20_Status;
uint8_t SW21_Status;

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void Wait_MS(uint16_t ms);
void TouchSense_Update(void);

//-----------------------------------------------------------------------------
// Generic UART definitions to support both Keil and SDCC
//-----------------------------------------------------------------------------

#ifdef SDCC
#include <sdcc_stdio.h>
#endif

char *GETS (char *, uint16_t);

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
  // Disable the watchdog here
}
 
//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
	//Enter default mode
	enter_DefaultMode_from_RESET();

	SW20_Status = 1;                    // Set switch status to finger not
	SW21_Status = 1;                    // detected.
	SCON0_TI = 1;                            // Indicate TX0 ready

	printf("\n\nOscillation Period:\n\n");

	while(1)
	{
	  Wait_MS(250);

	  TouchSense_Update();

	  printf("\rSW20: %5d, SW21: %5d\n",
		 SW20_Timer_Count.u16, SW21_Timer_Count.u16 );

	  YELLOW_LED = SW20_Status;
	  RED_LED = SW21_Status;
	}

	// NOTREACHED
	return 0;
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// TouchSense_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports.
//
//
//-----------------------------------------------------------------------------
void TouchSense_Update (void)
{

   SI_UU16_t timer_count_A, timer_count_B;

   // Prepare Timer2 for the first TouchSense reading
   TMR2CN &= ~0x80;                    // Clear overflow flag
   while(!(TMR2CN & 0x80));            // Wait for overflow
   timer_count_A.u16 = TMR2RL;         // Record value

   // Prepare Timer2 for the second TouchSense reading
   TMR2CN &= ~0x80;                    // Clear overflow flag
   while(!(TMR2CN & 0x80));            // Wait for overflow
   timer_count_B.u16 = TMR2RL;         // Record value

   // Calculate the oscillation period
   SW20_Timer_Count.u16 = timer_count_B.u16 - timer_count_A.u16;

   // Change the CP0 Multiplexer to select switch on P2.1
   CPT0MX = 0x8C;                      // Positive Mux: TouchSense Compare
                                       // Negative Mux: P2.1 - TouchSense Switch

   // Prepare Timer2 for the first TouchSense reading
   TMR2CN &= ~0x80;                    // Clear overflow flag
   while(!(TMR2CN & 0x80));            // Wait for overflow
   timer_count_A.u16 = TMR2RL;         // Record value

   // Prepare Timer2 for the second TouchSense reading
   TMR2CN &= ~0x80;                    // Clear overflow flag
   while(!(TMR2CN & 0x80));            // Wait for overflow
   timer_count_B.u16 = TMR2RL;         // Record value

   // Calculate the oscillation period
   SW21_Timer_Count.u16 = timer_count_B.u16 - timer_count_A.u16;

   // Change the CP0 Multiplexer to select switch on P2.0
   CPT0MX = 0xC8;                      // Positive Mux: P2.0 - TouchSense Switch
                                       // Negative Mux: TouchSense Compare

}


//-----------------------------------------------------------------------------
// Wait_MS
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters:
//   1) uint16_t ms - number of milliseconds of delay
//                        range is full range of integer: 0 to 65335
//
// This routine inserts a delay of <ms> milliseconds.
//-----------------------------------------------------------------------------
void Wait_MS(uint16_t ms)
{
   char i;

   TCON_TR0 = 0;                            // Stop Timer0

   TMOD &= ~0x0F;                      // Timer0 in 8-bit mode
   TMOD |= 0x02;

   CKCON &= ~0x03;                     // Timer0 uses a 1:48 prescaler
   CKCON |= 0x02;


   TH0 = -SYSCLK/48/10000;             // Set Timer0 Reload Value to
                                       // overflow at a rate of 10kHz

   TL0 = TH0;                          // Init Timer0 low byte to the
                                       // reload value

   TCON_TF0 = 0;                            // Clear Timer0 Interrupt Flag
   IE_ET0 = 0;                            // Timer0 interrupt disabled
   TCON_TR0 = 1;                            // Timer0 on

   while(ms--)
   {
      for(i = 0; i < 10; i++)
      {
         TCON_TF0 = 0;
         while(!TCON_TF0);
      }
   }

   TCON_TF0 = 0;

}
