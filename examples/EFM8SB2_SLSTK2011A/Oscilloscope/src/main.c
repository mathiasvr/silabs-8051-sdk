//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This demo samples the ADC input at 500 ksps and displays the measured waveform.
// Panning, triggers, and other general oscilloscope options are supported.
//
// Note: The joystick is only enabled when the oscilloscope is in stop mode.
//
//-----------------------------------------------------------------------------
// How To Test: EFM8SB2 STK
//-----------------------------------------------------------------------------
// 1) Place the SW104 switch in "AEM" mode.
// 2) Connect the EFM8SB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8SB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) The Oscilloscope demo should start.
// 6) Read the instructions splash screen for user controls.
// 7) Press a push button to dismiss the splash screen.
// 8) Press PB1 to toggle oscilloscope mode between run (green) and stop (red)
// 9) Press PB0 to initiate a single sequence (blue) (capture the first waveform
//    that triggers a rising or falling edge depending on menu configuration)
//    and stop.
// 10) When stopped (red), pressing the joystick has the following effect:
//     Up - Increase trigger level voltage
//     Down - Decrease trigger level voltage
//     Left - Scroll horizontal position to the left
//     Right - Scroll horizontal position to the right
//     Center - Press to enter menu mode
// 11) In menu mode, pressing the joystick has the following effect:
//     Up - Move current setting selection up
//     Down - Move current setting selection down
//     Left - Decrease current setting value
//     Right - Increase current setting value
//     Center - Press to leave menu mode
// 12) Mode Settings:
//     Display - Draw waveforms using vectors (connected dots) or dots
//     Labels - Turn voltage/time labels off
//     Mode - Auto: periodically update waveform display even when
//            not triggered.
//            Normal: only update waveform display when triggered
//     Slope - Trigger level on rising or falling edge
//     Input - Analog input: joystick (P1.7) or external input (P1.3)
//     Rate - Sample rate: 31.25 kHz, 62.5 kHz, 125 kHz, 250 kHz, 500 kHz,
//            or 500x2 kHz (500 kHz sample rate with 2x linear interpolation)
//
// Target:         EFM8SB2
// Tool chain:     Generic
//
// Release 0.1 (CM)
//    - Initial Revision
//    - 10 OCT 2014
//
//-----------------------------------------------------------------------------
// Resources
//-----------------------------------------------------------------------------
// SYSCLK - 24.5 MHz HFOSC / 1
// ADC0   - 10-bit
// SPI1   - 1 MHz
// Timer0 - ADC start of conversion (31.25 kHz to 500 kHz)
// Timer2 - 2 MHz (SPI CS delay)
// Timer3 - 1 kHz (1 ms tick)
// P0.2 - push button
// P0.3 - push button
// P1.0 - LCD SCK
// P1.2 - LCD MOSI
// P1.3 - External ADC input
// P1.5 - LCD CS (Active High)
// P1.7 - Joystick (analog voltage divider)
// P2.0 - LED Green
// P2.1 - LED Blue
// P2.2 - LED Red
// P2.6 - Display enable
//

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "InitDevice.h"
#include "disp.h"
#include "oscilloscope.h"

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

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
 
//---------------------------------------------------------------------------
// main() Routine
// --------------------------------------------------------------------------
void main (void)
{
    // Enter default mode
    enter_DefaultMode_from_RESET();

    // Enable all interrupts
    IE_EA = 1;

    DISP_Init();
    Oscilloscope_Init();

    while (1)
    {
        Oscilloscope_main();
    }
}
