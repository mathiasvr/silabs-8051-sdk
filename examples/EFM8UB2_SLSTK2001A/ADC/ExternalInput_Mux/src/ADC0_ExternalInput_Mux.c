//-----------------------------------------------------------------------------
// ADC0_ExternalInput_Mux.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// This code example illustrates using the internal analog multiplexer to
// measure analog voltages on up to 8 different analog inputs.  Results are
// printed to a PC terminal program via the UART.
//
// The inputs are sequentially scanned, beginning with input 0 (P2.0), up
// to input number <ANALOG_INPUTS>-1 based on the values in <PIN_TABLE>.
//
//
// ADC Settling Time Requirements, Sampling Rate:
// ----------------------------------------------
//
// The total sample time per input is comprised of an input setting time
// (Tsettle), followed by a conversion time (Tconvert):
//
// Tsample  = Tsettle + Tconvert
//
// |--------Settling-------|==Conversion==|----Settling--- . . .
// Timer 2 overflow        ^
// ADC0 ISR                               ^
//
// The ADC input voltage must be allowed adequate time to settle before the
// conversion is made.  This settling depends on the external source
// impedance, internal mux impedance, and internal capacitance.
// Settling time is given by:
//
//                   | 2^n |
//    Tsettle =   ln | --- | * Rtotal * Csample
//                   | SA  |
//
// For example, assume Rtotal = 100 kohm. The expression evaluates to:
//
//                   | 2^10 |
//    Tsettle =   ln | ---- | * 105e3 * 5e-12 = 4.4 us
//                   | 0.25 |
//
// In addition, one must allow at least 1.5uS after changing analog mux
// inputs or PGA settings.  The settling time in this example, then, is
// dictated by the large external source resistance.
//
// The conversion is given in the datasheet (10-bit mode, SAR clock 12.25 MHz)
// as 1 us.
//
// Tsample, minimum  = Tsettle + Tconvert
//                   = 4.4 us + 1 us
//                   = 5.4 us
//
// Timer2 is set to start a conversion every 20 us, which is far longer
// than the minimum required.
//
// General:
// --------
//
// The system is clocked using the internal 12MHz oscillator. Results are
// printed to the UART from a loop with the rate set by a delay based on
// Timer0. This loop periodically reads the ADC value from a global array,
// <RESULT>.
//
// The ADC makes repeated measurements at 20 us intervals based on Timer2.
// The end of each ADC conversion initiates an interrupt which calls an
// averaging function. <INT_DEC> samples are averaged, then the Result
// values updated.
//
// For each power of 4 of <INT_DEC>, you gain 1 bit of effective resolution.
// For example, <INT_DEC> = 256 gain you 4 bits of resolution: 4^4 = 256.
//
// The ADC input multiplexer is set for a single-ended input.  The example
// sequentially scans through the inputs, starting at P2.0.  <ANALOG_INPUTS>
// inputs are read.  The amplifier is set for unity gain so a voltage range of
// 0 to Vref (2.43V) may be measured.  Although voltages up to Vdd may be
// applied without damaging the device, only the range 0 to Vref may be
// measured by the ADC.
//
// A 100 kohm potentiometer may be connected as a voltage divider between
// VREF and AGND as shown below:
//
// ---------
//          |
//         o| AGND ----|
//         o| VREF ----|<-|
//         o| P2.x     |  |
//         o|    |        |
//         o|     --------
//         o|
//         o|
//         o|
//          |
// ---------
//
// How To Test:
//
//-----------------------------------------------------------------------------
// How To Test: EFM8UB2 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8UB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8UB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the JLink CDC UART Port at 115200 baud rate and 8-N-1.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) Connect a variable voltage source (between 0 and Vref) to the Port2 pins,
//    or a potentiometer voltage divider as shown above.
// 7) HyperTerminal will print the voltages measured by the device if
//    everything is working properly.  Note that some of the analog inputs are
//    floating and will return nonzero values.
//
//
// Target:         EFM8UB2
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

#include <SI_EFM8UB2_Register_Enums.h>
#include "retargetserial.h"
#include "InitDevice.h"
//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define ANALOG_INPUTS 6                // Number of AIN pins to measure,
                                       // skipping the UART0 pins
#define INT_DEC       256              // Integrate and decimate ratio

#define TIMER0_RELOAD_HIGH  0          // Timer0 High register
#define TIMER0_RELOAD_LOW 255          // Timer0 Low register

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void Timer0_wait(int16_t ms);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

int32_t RESULT[ANALOG_INPUTS];            // ADC0 decimated value, one for each
                                       // analog input

// The <PIN_MUX_TABLE> values are the values to be written to the AMX0P
// register to select the P2.<PIN_TABLE> port pins.
// For the EFM8UB2, the AMX0P settings correspond to the following port pins:
//
//    AMX0P      Port Pin
//    0x00         P2.0
//    0x01         P2.1
//    0x02         P2.2
//    0x03         P2.3
//    0x04         P2.5
//    0x05         P2.6
//
SI_SEGMENT_VARIABLE(PIN_TABLE[ANALOG_INPUTS], uint8_t, SI_SEG_IDATA) = {0,1,2,3,5,6};
SI_SEGMENT_VARIABLE(PIN_MUX_TABLE[ANALOG_INPUTS], uint8_t, SI_SEG_IDATA) = {0,1,2,3,4,5};

uint8_t AMUX_INPUT = 0;          // Index of analog MUX inputs

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
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   uint8_t i;
   uint32_t measurement;

   enter_DefaultMode_from_RESET();

   SCON0_TI = 1;

   IE_EA = 1;                             // Enable global interrupts
   while (1)
   {
      IE_EA = 0;                          // Disable interrupts
      RETARGET_PRINTF("\f");
      for(i = 0; i < ANALOG_INPUTS; i++)
      {
         // The 10-bit ADC value is averaged across INT_DEC measurements.
         // The result is then stored in RESULT, and is right-justified
         // The measured voltage applied to the port pins is then:
         //
         //                           Vref (mV)
         //   measurement (mV) =   --------------- * Result (bits)
         //                        (2^10)-1 (bits)

         measurement =  RESULT[i] * 2430 / 1023;
         RETARGET_PRINTF("P2.%d voltage: %ld mV\n", (int16_t)PIN_TABLE[i], measurement);
      }
      IE_EA = 1;                          // Re-enable interrupts

      Timer0_wait(25);                  // Wait before displaying new values
   }
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_wait
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) int16_t ms - number of milliseconds to wait
//                        range is positive range of an int: 0 to 32767
//
// This function configures the Timer0 as a 16-bit timer, interrupt enabled.
// Using the internal osc. at 12MHz with a prescaler of 1:8 and reloading the
// T0 registers with TIMER0_RELOAD_HIGH/LOW, it will wait for <ms>
// milliseconds.
// Note: The Timer0 uses a 1:12 prescaler
//-----------------------------------------------------------------------------
void Timer0_wait(int16_t ms)
{
   TH0 = TIMER0_RELOAD_HIGH;           // Init Timer0 High register
   TL0 = TIMER0_RELOAD_LOW ;           // Init Timer0 Low register
   TMOD |= 0x01;                       // Timer0 in 16-bit mode
   CKCON0 &= 0xFC;                      // Timer0 uses a 1:12 prescaler
   TCON_TR0  = 1;                           // Timer0 ON

   while(ms)
   {
      TCON_TF0 = 0;                         // Clear flag to initialize
      while(!TCON_TF0);                     // Wait until timer overflows
      ms--;                            // Decrement ms
   }

   TCON_TR0 = 0;                            // Timer0 OFF
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
