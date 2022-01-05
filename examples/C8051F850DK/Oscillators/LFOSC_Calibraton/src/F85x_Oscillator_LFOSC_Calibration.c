//-----------------------------------------------------------------------------
// F85x_Oscillator_LFOSC_Calibration.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// The program uses Timer 3 to capture the Low Frequency Timer period and
// compares it against the HFOSC at 24.5 MHz.  This enables the LFOSC to be
// tuned for 70 kHz, rather than 80 kHz.
//
// Note: For more precise results, measure the actual system clock frequency
// before pressing S1 and redefine SYSCLK with the measured value.  Also,
// reduce the RANGE define, as this determines how close the frequency has
// to get before the cycle quits.
//
//-----------------------------------------------------------------------------
// How To Test: UPMU-F850
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - J27: DS6 P1.1 - P1.1/ADC0.9
//    - J27: S1 - P1.7
//    - JP2
// 2) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to the 'F85x UDP MCU board by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
// 5) Press S1 to start the LFOSC calibration.
// 6) Observe the /SYSCLK frequency output on P0.0 at 70 kHz.
//    If the calibration passes, LED1 (DS5 P1.0) will turn on.
//    If the calibration fails, LED2 (DS6 P1.1) will turn on.
//
//-----------------------------------------------------------------------------
// How To Test: ToolStick F85x
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 3) Compile and download code to the ToolStick F85x board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) Press S1 to start the LFOSC calibration.
// 6) Observe the /SYSCLK frequency output on P0.0 at 70 kHz.
//    If the calibration passes, LED1 (DS1 P1.0) will turn on.
//    If the calibration fails, LED2 (DS2 P1.1) will turn on.
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
//    - 19 FEB 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK              24500000   // SYSCLK in Hz

#define LFOSC                  70000   // New target LFOSC frequency

#define CAPTURE_COUNT   SYSCLK/LFOSC   // The ideal Timer 3 capture count value
                                       // to perfectly calibrate at the target
                                       // LFOSC frequency
#define RANGE                     10   // The acceptable count range away from
                                       // the ideal value that's still
                                       // considered "good"
#define NUM_RETRIES                5   // Defines the number of retries at
                                       // saturation before the calibration
                                       // code gives up and marks a failure to
                                       // calibrate

// LED states
#define LED_ON                     0
#define LED_OFF                    1

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

SI_SBIT(LED1, SFR_P1, 0);                 // DS5 P1.0 LED
SI_SBIT(LED2, SFR_P1, 1);                 // DS6 P1.1 LED
SI_SBIT(S1, SFR_P1, 7);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void LFOSC_Calibration(void);

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

   // Wait until S1 is pressed before calibrating the LFOSC
   while (S1 == 1);

   LFOSC_Calibration();

   IE_EA = 1;                             // Set global interrupt enable

   while (1);
}

//-----------------------------------------------------------------------------
// LFOSC_Calibration ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine calibrates the LFOSC to the desired value instead of 80 kHz
// using the capture feature of Timer 3.
//
//-----------------------------------------------------------------------------
void LFOSC_Calibration (void)
{
   uint8_t LFOSC_cal = 0;
   volatile uint16_t count = 0;
   volatile count2 = 0;
   bit cal_failed = 0;
   bit cal_finished = 0;
   uint8_t retry = 0;

   // Set up Timer 3 to capture on the LFOSC
   CKCON |= 0xC0;                      // Timer 3 uses the SYSCLK
   TMR3CN = 0x10;                      // Enable Timer 3 capture mode

   LFOSC_cal = (OSCLCN & 0x3C) >> 2;   // Factory value is set to 80 kHz

   // Wait until the LFOSC is within the range or the calibration fails due
   // to saturation of the OSCLF field
   while ((!cal_finished)
          && (!cal_failed))
   {
      OSCLCN |= 0x03;                  // Set the LFOSC divider to 1
      OSCLCN |= 0x80;                  // Enable the LFOSC
      while ((OSCLCN & 0x40) != 0x40); // Poll until the LFOSC is ready

      TMR3 = 0x0000;                   // Reset Timer 3
      TMR3CN &= ~0x80;                 // Clear Timer High Overflow Flag
      TMR3CN |= 0x04;                  // Start Timer 3

      while ((TMR3CN & 0x80) != 0x80); // Wait until the first capture
                                       // occurs (first LFOSC falling edge)

      TMR3CN &= ~0x80;                 // Clear Timer High Overflow Flag
      count = TMR3RL;

      while ((TMR3CN & 0x80) != 0x80); // Wait until the second capture
                                       // occurs (second LFOSC falling edge)

      count2 = TMR3RL;

      TMR3CN &= ~0x04;                 // Stop Timer 3
      TMR3CN &= ~0x80;                 // Clear Timer High Overflow Flag

      // Calculate the number of system clocks for one LFOSC period
      count = count2 - count;

      OSCLCN &= ~0x80;                 // Disable the LFOSC, since changes
                                       // to OSCLF can only be made with
                                       // the LFOSC disabled

      // If count is too low, this means the oscillator is running too fast
      // Increase the OSCLF field to slow the LFOSC down.
      if (count < (CAPTURE_COUNT - RANGE))
      {
         if (LFOSC_cal < 0x0F)         // Range checking
         {
            LFOSC_cal++;
            OSCLCN = LFOSC_cal << 2;
         }
      }
      // If the count is too high, this means the oscillator is running too
      // slowly. Decrease the OSCLF field to speed the LFOSC up.
      else if (count > (CAPTURE_COUNT + RANGE))
      {
         if (LFOSC_cal > 0x00)         // Range checking
         {
            LFOSC_cal--;
            OSCLCN = LFOSC_cal << 2;
         }
      }
      // Otherwise, count is within the appropriate range and calibration is
      // complete
      else
      {
         cal_finished = 1;
      }

      // If the OSCLF field saturates at 0's or 1's and the cal didn't finish,
      // indicate the calibration failed after the specified number of retries.
      if (((LFOSC_cal == 0x00)
          || (LFOSC_cal == 0x0F))
          && (!cal_finished))
      {
         if (retry == NUM_RETRIES)
         {
            cal_failed = 1;
         }

         retry++;
      }
   }

   if (cal_finished)
   {
      // Re-enable the LFOSC at the final calibration value
      OSCLCN |= 0x03;                  // Set the LFOSC divider to 1
      OSCLCN |= 0x80;                  // Enable the LFOSC
      while ((OSCLCN & 0x40) != 0x40); // Poll until the LFOSC is ready

      // Indicate calibration success and switch to the LFOSC
      LED1 = LED_ON;
      LED2 = LED_OFF;
      CLKSEL = 0x02;
   }
   else
   {
      // Indicate calibration failure
      LED1 = LED_OFF;
      LED2 = LED_ON;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
