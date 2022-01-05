//-----------------------------------------------------------------------------
// PCA_Lib_16Bit_PWM_Output.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program sends a PWM waveform out of an I/O pin, using the PCA's
// 16-bit PWM Output Mode.  The duty cycle of the waveform is modified
// periodically within the main loop.
//
// In this example, PCA Module 0 is used to generate the waveform, and the
// crossbar is configured to send the CEX0 pin out on P2.0.
//
// The frequency of the PWM signal generated at the CEX0 pin is equal to the
// PCA main timebase frequency divided by 65536.
//
// The PCA time base in this example is configured to use SYSCLK, and SYSCLK
// is set up to use the internal oscillator running at 24.5 MHz.  Therefore,
// the frequency of the PWM signal will be 24.5 MHz / 65536 = 373.8 Hz.
// Using different PCA clock sources or a different processor clock will
// result in a different frequency for the PWM signal.
//
//    -------------------------------------------------------------------------
//    How "16-Bit PWM Mode" Works:
//
//       The PCA's 16-bit PWM Mode works by setting an output pin low every
//    time the main 16-bit PCA counter (PCA0H) overflows, and then setting
//    the pin high whenever a specific match condition is met.
//
//    Upon a PCA0 overflow (PCA0 incrementing from 0xFFFF to 0x0000), the
//    CEXn pin will be set low.
//
//    When the PCA0 register increments and matches the PCA0CPn register for
//    the selected module, the CEXn pin will be set high, except when the
//    ECOMn bit in PCA0CPMn is cleared to '0'.  By varying the value of the
//    PCA0CPn register, the duty cycle of the waveform can also be varied.
//
//    When ECOMn = '1', the duty cycle of the PWM waveform is:
//
//       16-bit PWM Duty Cycle = (65536 - PCA0CPn) / 65536
//
//    To set the duty cycle to 100%, a value of 0x0000 should be loaded into
//    the PCA0CPn register for the module being used (with ECOMn set to '1').
//    When the value of PCA0CPn is equal to 0x0000, the pin will never be
//    set low.
//
//    To set the duty cycle to 0%, the ECOMn bit in the PCA0CPMn register
//    should be cleared to 0.  This prevents the PCA0CPn match from occurring,
//    which results in the pin never being set high.
//
// When adjusting the PWM duty cycle, the low byte of the PCA0CPn register
// (PCA0CPLn) should be written first, followed by the high byte (PCA0CPHn).
// Writing the low byte clears the ECOMn bit, and writing the high byte will
// restore it.  This ensures that a match does not occur until the full
// 16-bit value has been written to the compare register.  Writing the high
// byte first will result in the ECOMn bit being set to '0' after the 16-bit
// write, and the duty cycle will also go to 0%.
//
// It is also advisable to wait until just after a match occurs before
// updating the PWM duty cycle.  This will help ensure that the ECOMn
// bit is not cleared (by writing PCA0CPLn) at the time when a match was
// supposed to happen.  This code implements the compare register update in
// the PCA ISR upon a match interrupt.
//    -------------------------------------------------------------------------
//
// How To Test:
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8SB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8SB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Connect an oscilloscope to P2.0.
// 6) Verify that the duty cycle of the waveform varies smoothly between
//    minimum and maximum values.
//
//
// Target:         EFM8SB2
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 13 JAN 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "bsp.h"
#include "InitDevice.h"
#include "pca_0.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint16_t CEX0_Compare_Value;            // Holds current PCA compare value

//-----------------------------------------------------------------------------
// Callbacks
//-----------------------------------------------------------------------------

void PCA0_overflowCb()
{

}

void PCA0_intermediateOverflowCb()
{
}

void PCA0_channel0EventCb()
{
  PCA0_writeChannel(PCA0_CHAN0, CEX0_Compare_Value);
}

void PCA0_channel1EventCb()
{

}

void PCA0_channel2EventCb()
{

}

void PCA0_channel3EventCb()
{

}

void PCA0_channel4EventCb()
{

}

void PCA0_channel5EventCb()
{

}

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
  uint8_t delay_count;                        // Used to implement a delay
  bool duty_direction = 0;                     // 0 = Decrease; 1 = Increase

  enter_DefaultMode_from_RESET();

  BSP_DISP_EN = 0;                            // Display not driven by EFM8

  while (1)
  {
    // Wait a little while
    for (delay_count = 60; delay_count > 0; delay_count--);

    if (duty_direction == 1)                  // Direction = Increase
    {
      // First, check the ECOM0 bit
      if ((PCA0CPM0 & PCA0CPM0_ECOM__BMASK) == PCA0CPM0_ECOM__DISABLED)
      {
        PCA0CPM0 |= PCA0CPM0_ECOM__ENABLED;   // Set ECOM0 if it is '0'
      }
      else                                    // Increase duty cycle otherwise
      {
        CEX0_Compare_Value--;                 // Increase duty cycle

        if (CEX0_Compare_Value == 0x0000)
        {
          duty_direction = 0;                 // Change direction for next time
        }
      }
    }
    else                                      // Direction = Decrease
    {
      if (CEX0_Compare_Value == 0xFFFF)
      {
        PCA0CPM0 &= ~PCA0CPM0_ECOM__BMASK;    // Clear ECOM0
        duty_direction = 1;                   // Change direction for next time
      }
      else
      {
        CEX0_Compare_Value++;                 // Decrease duty cycle
      }
    }
  }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
