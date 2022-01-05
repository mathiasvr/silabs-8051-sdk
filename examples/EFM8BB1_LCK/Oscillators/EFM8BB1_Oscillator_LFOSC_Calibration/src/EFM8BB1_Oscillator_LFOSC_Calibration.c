//-----------------------------------------------------------------------------
// EFM8BB1_Oscillator_LFOSC_Calibration.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// The program uses Timer 3 to capture the Low Frequency Timer period and
// compares it against the HFOSC at 24.5 MHz.  This enables the LFOSC to be
// tuned for 70 kHz, rather than 80 kHz.
//
// Note: For more precise results, measure the actual system clock frequency
// before pressing PB0 and redefine SYSCLK with the measured value.  Also,
// reduce the RANGE define, as this determines how close the frequency has
// to get before the cycle quits.
//
//-----------------------------------------------------------------------------
// How To Test:
//-----------------------------------------------------------------------------
// 1) power the board using the DBG USB connector.
// 2) Compile and download code to the EFM8BB1 board by selecting
//	  Run -> Debug from the menus, clicking the Debug button in the quick menu,
//	  or pressing F11.
// 3) Run the code by selecting Run -> Resume from the menus, clicking the
//	  Resume button in the quick menu, or pressing F8.
// 4) Press PB0 to start the LFOSC calibration.
// 5) Observe the /SYSCLK frequency output on P0.7 at 70 kHz.
//    If the calibration passes, the LED will turn on.
//
// Target:         EFM8BB1
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

#include <SI_EFM8BB1_Register_Enums.h>
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

SI_SBIT(LED, SFR_P1, 4);		           // LED
SI_SBIT(S1, SFR_P0, 2);				       // PB0 button

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

void main(void)
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
void LFOSC_Calibration(void)
{
  uint8_t LFOSC_cal = 0;
  volatile uint16_t count = 0;
  volatile uint16_t count2 = 0;
  bool cal_failed = 0;
  bool cal_finished = 0;
  uint8_t retry = 0;

  // Set up Timer 3 to capture on the LFOSC
  CKCON0 |= 0xC0;                      // Timer 3 uses the SYSCLK
  TMR3CN0 = 0x10;                      // Enable Timer 3 capture mode

  LFOSC_cal = (LFO0CN & 0x3C) >> 2;   // Factory value is set to 80 kHz

  // Wait until the LFOSC is within the range or the calibration fails due
  // to saturation of the OSCLF field
  while ((!cal_finished)
         && (!cal_failed))
  {
    LFO0CN |= 0x03;                  // Set the LFOSC divider to 1
    LFO0CN |= 0x80;                  // Enable the LFOSC
    while ((LFO0CN & 0x40) != 0x40); // Poll until the LFOSC is ready

    TMR3 = 0x0000;                   // Reset Timer 3
    TMR3CN0 &= ~0x80;                // Clear Timer High Overflow Flag
    TMR3CN0 |= 0x04;                 // Start Timer 3

    while ((TMR3CN0 & 0x80) != 0x80); // Wait until the first capture
        // occurs (first LFOSC falling edge)

    TMR3CN0 &= ~0x80;                 // Clear Timer High Overflow Flag
    count = TMR3RL;

    while ((TMR3CN0 & 0x80) != 0x80); // Wait until the second capture
        // occurs (second LFOSC falling edge)

    count2 = TMR3RL;

    TMR3CN0 &= ~0x04;                 // Stop Timer 3
    TMR3CN0 &= ~0x80;                 // Clear Timer High Overflow Flag

    // Calculate the number of system clocks for one LFOSC period
    count = count2 - count;

    LFO0CN &= ~0x80;                 // Disable the LFOSC, since changes
                                     // to OSCLF can only be made with
                                     // the LFOSC disabled

    // If count is too low, this means the oscillator is running too fast
    // Increase the OSCLF field to slow the LFOSC down.
    if (count < (CAPTURE_COUNT - RANGE))
    {
      if (LFOSC_cal < 0x0F)         // Range checking
      {
        LFOSC_cal++;
        LFO0CN = LFOSC_cal << 2;
      }
    }
    // If the count is too high, this means the oscillator is running too
    // slowly. Decrease the OSCLF field to speed the LFOSC up.
    else if (count > (CAPTURE_COUNT + RANGE))
    {
      if (LFOSC_cal > 0x00)         // Range checking
      {
        LFOSC_cal--;
        LFO0CN = LFOSC_cal << 2;
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
    LFO0CN |= 0x03;                  // Set the LFOSC divider to 1
    LFO0CN |= 0x80;                  // Enable the LFOSC
    while ((LFO0CN & 0x40) != 0x40); // Poll until the LFOSC is ready

    // Indicate calibration success and switch to the LFOSC
    LED = LED_ON;
    CLKSEL = 0x02;
  }
  else
  {
    // Indicate calibration failure
    LED = LED_OFF;
  }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
