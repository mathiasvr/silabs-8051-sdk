//-----------------------------------------------------------------------------
// F36x_PCA0_16Bit_PWM_Output.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// Use the PCA to generate a 16-bit PWM waveform
//
// This program sends a PWM waveform out of an I/O pin, using the PCA's
// 16-bit PWM Output Mode. The duty cycle of the waveform is modified
// periodically within the main loop.
//
// In this example, PCA Module 0 is used to generate the waveform, and the
// crossbar is configured to send the CEX0 pin out on P0.0.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J2: P0.0 - J12: P3.2_LED
//    J1: Power selection, P2 9V, P4 USB or J9 Debugger
// 2) Connect the USB Debug Adapter to J9. If 9V power is selected on J1,
//    connect the 9V power adapter to P2. If VBUS is selected on J1, connect
//    USB cable to P4.
// 3) Compile and download code to C8051F360 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) Observe the triangle wave on LED D4, or use an oscilloscope to monitor
//    P0.0 and verify that the duty cycle of the waveform varies smoothly
//    between minimum and maximum values.
//
//
// Target:         C8051F36x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1
//    - Updated to use with Simplicity Studio (JL)
//    - 29 JUL 2014
//
// Release 1.0
//    -Initial Revision (BD / TP)
//    -16 OCT 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>


//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // Internal oscillator frequency in Hz

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Port_Init (void);
void PCA0_Init (void);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint16_t CEX0_Compare_Value;               // Holds current PCA compare value

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void) 
{
   uint16_t Delay_Count;                    // Used to implement a delay
   bit Duty_Direction = 0;             // 0 = Decrease; 1 = Increase


   Port_Init ();                       // Initialize crossbar and GPIO
   Oscillator_Init ();                 // Initialize oscillator
   PCA0_Init ();                       // Initialize PCA0

   SFRPAGE = LEGACY_PAGE;

   IE_EA = 1;                          // Globally enable interrupts

   while (1)
   {
      // Wait a little while
      for (Delay_Count = 120; Delay_Count > 0; Delay_Count--);

      if (Duty_Direction == 1)         // Direction = Increase
      {
         // First, check the ECOM0 bit
         if ((PCA0CPM0 & PCA0CPM0_ECOM__BMASK) == PCA0CPM0_ECOM__DISABLED)
         {
            PCA0CPM0 |= PCA0CPM0_ECOM__ENABLED; // Set ECOM0 if it is '0'
         }
         else                          // Increase duty cycle otherwise
         {
            CEX0_Compare_Value--;      // Increase duty cycle

            if (CEX0_Compare_Value == 0x0000)
            {
               Duty_Direction = 0;     // Change direction for next time
            }
         }
      }
      else                             // Direction = Decrease
      {
         if (CEX0_Compare_Value == 0xFFFF)
         {
            PCA0CPM0 &= ~PCA0CPM0_ECOM__BMASK; // Clear ECOM0
            Duty_Direction = 1;        // Change direction for next time
         }
         else
         {
            CEX0_Compare_Value++;      // Decrease duty cycle
         }
      }
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at 24.5 MHz.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // Set internal oscillator to run at its maximum frequency
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;

   // Select the internal osc. as the SYSCLK source
   CLKSEL = CLKSEL_CLKSL__DIVIDED_HFOSC;

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
// P0.0   digital   push-pull     PCA0 CEX0
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // Route CEX0 to P0.0, enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED |
          XBR1_XBARE__ENABLED |
          XBR1_PCA0ME__CEX0;

   P0MDOUT |= 0x01;                    // Set CEX0 (P0.0) to push-pull

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA time base, and sets up 16-bit PWM output
// mode for Module 0 (CEX0 pin).
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
//    should be cleared to 0.  This prevents the PCA0CPn match from occuring,
//    which results in the pin never being set high.
//
//    When adjusting the PWM duty cycle, the low byte of the PCA0CPn register
//    (PCA0CPLn) should be written first, followed by the high byte (PCA0CPHn).
//    Writing the low byte clears the ECOMn bit, and writing the high byte will
//    restore it.  This ensures that a match does not occur until the full
//    16-bit value has been written to the compare register.  Writing the high
//    byte first will result in the ECOMn bit being set to '0' after the 16-bit
//    write, and the duty cycle will also go to 0%.
//
//    It is also advisable to wait until just after a match occurs before
//    updating the PWM duty cycle.  This will help ensure that the ECOMn
//    bit is not cleared (by writing PCA0CPLn) at the time when a match was
//    supposed to happen.  This code implements the compare register update in
//    the PCA ISR upon a match interrupt.
//    -------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // Configure PCA time base; overflow interrupt disabled
   PCA0CN = 0x00;                      // Stop counter; clear all flags
   // Use SYSCLK as time base
   PCA0MD = PCA0MD_CPS__SYSCLK;

   PCA0CPM0 = 0xCB;                    // Module 0 = 16-bit PWM mode and
                                       // enable Module 0 Match and Interrupt
                                       // Flags

   // Configure initial PWM duty cycle = 50%
   CEX0_Compare_Value = 65536 - (65536 * 0.5);

   PCA0CPL0 = (CEX0_Compare_Value & 0x00FF);
   PCA0CPH0 = (CEX0_Compare_Value & 0xFF00)>>8;

   // Enable PCA interrupts
   EIE1 |= EIE1_EPCA0__ENABLED;

   // Start PCA counter
   PCA0CN_CR = 1;

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This is the ISR for the PCA.  It handles the case when a match occurs on
// channel 0, and updates the PCA0CPn compare register with the value held in
// the global variable "CEX0_Compare_Value".
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
   // Clear module 0 interrupt flag.
   PCA0CN_CCF0 = 0;

   PCA0CPL0 = (CEX0_Compare_Value & 0x00FF);
   PCA0CPH0 = (CEX0_Compare_Value & 0xFF00)>>8;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------