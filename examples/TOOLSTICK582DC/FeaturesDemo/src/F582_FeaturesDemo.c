//-----------------------------------------------------------------------------
// F582DC_FeaturesDemo.c
//-----------------------------------------------------------------------------
// Copyright 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates the features available on the ToolStick
// F582 Daughter Card.
//
// Based on the setting of a level-sensitive interrupt pin, the firmware
// will either blink the green LED at a visible rate, or PWM the LED so that
// it appears bright or dim. The blinking speed or the PWM duty cycle is set
// based on the value of the potentiometer. Voltage across the potentiometer
// is measured using the on-chip 12-bit ADC.
//
// The program also outputs the status of the LED to the UART at a baud rate
// defined by <BAUDRATE>.
//
// Pressing the switch on the daughter card toggles the ADC enable bit.  If
// the ADC is disabled, turning the potentiometer will not have any effect
// on blink or PWM rate.
//
// How To Test:
// 1) Connect the ToolStick F582 DC to the PC and download the code
// 2) Turn the potentiometer and see that the blink rate increases or decreases
// 3) Press the switch SW to turn off the ADC and check that turning the
//    potentiometer has no effect on the blink rate.
// 4) Press the switch SW again to re-enable the ADC and potentiometer.
// 5) Connect to the ToolStick F582 DC using ToolStickTerminal and see that
//    the UART output changes as the potentiometer is turned
// 6) In ToolStickTerminal, toggle GPIO0/RTS to force the firmware to
//    switch modes.
// 7) Turn the potentiometer and see that the brightness of the LED changes
//    and check that the output of the UART also changes.
//
//
// Target:         ToolStick F582 DC
// Tool chain:     Keil C51 8 / Keil EVAL C51
//                 Silicon Laboratories IDE version 3.4
// Release 1.1 (SG)
//    - Revised for Simplicity Studio
//    - 28 JUL 2015
//
// Release 1.0
//    -Initial Revision (GP)
//    -08 OCT 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F580_Register_Enums.h>         // SFR declarations
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P1, 2);                 // LED == 1 means ON
SI_SBIT(SW,  SFR_P1, 4);                 // SW == 0 means switch depressed

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK          3000000L       // SYSCLK frequency in Hz

#define BAUDRATE        115200         // Baud rate of UART in bps
#define PRINT_RATE      20             // Times per second to update on UART

#define OVERFLOWS_NEEDED  (SYSCLK / 65536 / PRINT_RATE)

#define ADC_CLOCK       100000         // SAR clock
#define ADC_SAMPLE_RATE 100            // Timer 2 overflow rate in Hz
#define ADC_COUNTS      4096           // Number of ADC counts

#define MAX_BLINK_RATE  40             // Max blink rate in Hz for Timer mode
#define MIN_BLINK_RATE  1              // Min blink rate in Hz for Timer mode
#define BLINK_DIFF  (MAX_BLINK_RATE - MIN_BLINK_RATE) // For easy calculations

// Firmware states
#define TIMER_MODE      0              // Uses the Timer to blink the LED
#define PWM_MODE        1              // Use a PCA channel to PWM the LED

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);

void TIMER0_Init (void);
void TIMER2_Init (uint16_t counts);
void TIMER3_Init (void);

void ADC0_Init (void);
void PCA0_Init (void);
void UART0_Init (void);
void INT0_Init (void);

void Print_String (uint8_t pstring[]);

// ISRs defined: INT0_ISR, TIMER0_ISR, TIMER3_ISR, ADC0_ISR.

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

bit Blink_State      = TIMER_MODE;     // Starts with blinking the LED

uint16_t PWM_Duty_Cycle   = 0;              // Percentage from 0 to 100
uint16_t Blink_Rate       = 0;              // From MIN_BLINK_RATE to MAX_BLINK_RATE
bit update           = 0;              // Forces a UART update

uint32_t Num_LED_Flashes  = 0;              // Simply counts the number of times
                                       // the LED blinks

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
   PCA0MD &= ~0x40;                    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   SFRPAGE = ACTIVE1_PAGE;             // Set SFR Page for PCA0MD


   OSCILLATOR_Init ();                 // Initialize system clock

   PORT_Init ();                       // Initialize crossbar and GPIO

   TIMER0_Init ();                     // Initialize Timer0 for UART update

   TIMER2_Init (SYSCLK / 12 / ADC_SAMPLE_RATE);   // Init Timer2 for ADC

   TIMER3_Init ();                     // Init timer used to blink LED

   PCA0_Init ();                       // Init PCA Module 0 to blink LED

   ADC0_Init ();                       // Enable ADC0

   INT0_Init ();                       // Enable /INT0

   UART0_Init ();                      // Enable UART using global settings

   IE_EA = 1;                          // Enable global interrupts (IE)

   while (1)
   {
      // Toggle the ADC enable bit when the switch is pressed
      while (SW != 0);                 // Wait until SW is pressed

      ADC0CN_ADEN = ~ADC0CN_ADEN;      // Toggle the bit (ADC0CN)

      while (SW == 0);                 // Wait until SW is released
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at its maximum frequency / 8 (3 Mhz)
//
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0x84;                      // Configure internal oscillator for
                                       // 24 MHz / 8

   SFRPAGE = ACTIVE1_PAGE;

   RSTSRC = 0x04;                      // Enable missing clock detector

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// Pinout:
//
// P0.0   digital   open-drain    Skipped
// P0.1   digital   open-drain    Skipped
// P0.2   digital   open-drain    Skipped
// P0.3   digital   open-drain    Skipped
// P0.4   digital   push-pull     UART TX
// P0.5   digital   open-drain    UART RX
// P0.6   digital   open-drain    Skipped
// P0.7   digital   open-drain    Skipped

// P1.0   digital   open-drain    /INT0 (Skipped)
// P1.1   digital   open-drain    Skipped
// P1.2   digital   push-pull     LED (and CEX0 depending on Blink_State)
// P1.3   analog                  Potentiometer (ADC input) (Skipped)
// P1.4   digital   open-drain    SW
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0SKIP  = 0xCF;                     // Skip P0.0-P0.3, P0.6-P0.7
   P1SKIP  = 0x3;                      // Skip P1.0-P1.1

   P0MDOUT = 0x10;                     // P0.4 is push-pull
   P1MDOUT = 0x04;                     // P1.2 is push-pull

   P1MDIN  = 0xF7;                     // P1.3 is analog input

   XBR0    = 0x01;                     // Enable UART on the crossbar

   XBR2    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// TIMER0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Initializes the Timer in 16-bit reload mode using SYSCLK as the time base.
//
//-----------------------------------------------------------------------------
void TIMER0_Init (void)
{
   // No need to set SFRPAGE as all registers accessed in this function
   // are available on all pages

   // Initialize Timer0; set global variable for number overflows to reach
   // interrupt rate set by <PRINT_RATE>

   TMOD   |= 0x01;                     // Mode 1 - 16-bit counter
   CKCON  |= 0x04;                     // Use SYSCLK as timebase

   TL0     = 0x00;                     // Clear Timer0 low byte
   TH0     = 0x00;                     // Clear Timer0 high byte

   IE_ET0     = 1;                     // Enable Timer0 Interrupts (IE)

   TCON_TR0     = 1;                   // Enable Timer 0 (TCON)
}

//-----------------------------------------------------------------------------
// TIMER2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1)  uint16_t counts - number of milliseconds of delay
//                    range is postive range of integer: 0 to 32767
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/12 as its time base.
//
//-----------------------------------------------------------------------------
void TIMER2_Init (uint16_t counts)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE;

   TMR2CN  = 0x00;                     // Stop Timer2; Clear TMR2CN_TF2;
                                       // use SYSCLK/12 as timebase
   CKCON  &= ~0x30;                    // Timer2 clocked based on TMR2CN_T2XCLK;

   TMR2RL  = -counts;                  // Init reload values
   TMR2    = 0xFFFF;                   // Set to reload immediately

   IE_ET2     = 0;                     // Disable Timer2 interrupts (IE)
   TMR2CN_TR2     = 1;                 // Start Timer2 (TMR2CN)

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// TIMER3_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure Timer3 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/12 as its time base.
//
// Initially sets Timer3 to overflow at the maximum blink rate.  The Timer3 ISR
// is used to toggle the LED.
//
//-----------------------------------------------------------------------------
void TIMER3_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE;

   TMR3CN  = 0x00;                     // Stop Timer3; Clear flags;
                                       // use SYSCLK/12 as timebase

   CKCON  &= ~0xC0;                    // Timer3 clocked based on T3XCLK;

   TMR3RL  = -(SYSCLK / 12 / MIN_BLINK_RATE);  // Init reload values
   TMR3    = 0xFFFF;                   // Set to reload immediately

   EIE1    |= 0x40;                    // Enable Timer3 interrupts
   TMR3CN  |= 0x04;                    // Start Timer3

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the ADC to measure potetiometer connected
// to P1.3 in single-ended mode.  Also enables the internal voltage reference.
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE;

   // Initialize the Gain to account for a 5V input and 2.25 VREF
   // Solve the equation provided in Section 9.3.1 of the Datasheet

   // The 5V input is scaled by a factor of 0.44 so that the maximum input
   // voltage seen by the pin is 2.2V

   // 0.44 = (GAIN/4096) + GAINADD * (1/64)

   // Set GAIN to 0x6CA and GAINADD to 1
   // GAIN = is the 12-bit word formed by ADC0GNH[7:0] ADC0GNL[7:4]
   // GAINADD is bit ADC0GNA.0

   ADC0CF |= 0x01;                     // Set GAINEN = 1
   ADC0H   = 0x04;                     // Load the ADC0GNH address
   ADC0L   = 0x6C;                     // Load the upper byte of 0x6CA to ADC0GNH
   ADC0H   = 0x07;                     // Load the ADC0GNL address
   ADC0L   = 0xA0;                     // Load the lower nibble of 0x6CA to ADC0GNL
   ADC0H   = 0x08;                     // Load the ADC0GNA address
   ADC0L   = 0x01;                     // Set the GAINADD bit
   ADC0CF &= ~0x01;                    // Set GAINEN = 0

   ADC0CN = 0x03;                      // ADC0 disabled, normal tracking,
                                       // conversion triggered on TMR2 overflow
                                       // Output is right-justified

   REF0CN = 0x23;                      // Enable on-chip VREF and buffer
                                       // Set voltage reference to 2.25V setting

   ADC0MX = 0x0B;                      // Set ADC input to P1.3

   ADC0CF = ((SYSCLK/3000000)-1)<<3;   // Set SAR clock to 3MHz

   EIE1 |= 0x04;                       // Enable ADC0 conversion complete int.

   ADC0CN_ADEN = 1;                    // Enable ADC0 (ADC0CN)

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       // clear SCON0_RI and SCON0_TI bits

   // Baud Rate = [BRG Clock / (65536 - (SBRLH0:SBRLL0))] x 1/2 x 1/Prescaler

#if   (SYSCLK/BAUDRATE/2/0xFFFF < 1)
      SBRL0 = -(SYSCLK/BAUDRATE/2);
      SBCON0 |= 0x03;                  // Set prescaler to 1
#elif (SYSCLK/BAUDRATE/2/0xFFFF < 4)
      SBRL0 = -(SYSCLK/BAUDRATE/2/4);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x01;                  // Set prescaler to 4
#elif (SYSCLK/BAUDRATE/2/0xFFFF < 12)
      SBRL0 = -(SYSCLK/BAUDRATE/2/12);
      SBCON0 &= ~0x03;                 // Set prescaler to 12
#else
      SBRL0 = -(SYSCLK/BAUDRATE/2/48);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x02;                  // Set prescaler to 48
#endif

   SBCON0 |= 0x40;                     // Enable baud rate generator

   SFRPAGE = ACTIVE1_PAGE;

   SCON0_TI = 1;                       // Indicate TX0 ready (SCON0)

   SFRPAGE = SFRPAGE_save;
}

#ifdef SDCC

// SDCC does not include a definition for putchar(), which is used in printf()
// and so it is defined here.  The prototype does not need to be explicitly
// defined because it is provided in stdio.h

//-----------------------------------------------------------------------------
// putchar
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : character to send to UART
//
// This function outputs a character to the UART.
//-----------------------------------------------------------------------------
void putchar (char output)
{
   if (output == '\n')
   {
      while (!SCON0_TI);
      SCON0_TI = 0;
      SBUF0 = 0x0d;
   }
   while (!SCON0_TI);
   SCON0_TI = 0;
   SBUF0 = output;
}

#endif

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure Module0 to 8-bit PWM.  The duty cycle is initially set to 50%. The
// duty cycle will actually be determined by the value of the potentiometer.
//
// The PCA counter isn't started until the user switches to PWM mode.
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE;

   // Configure PCA time base; overflow interrupt disabled
   PCA0CN = 0x00;                      // Stop counter; clear all flags
   PCA0MD = 0x08;                      // Use SYSCLK as time base

   PCA0CPM0 = 0x42;                    // Module 0 = 8-bit PWM mode

   // Configure initial PWM duty cycle = 50%
   PCA0CPH0 = 256 - (256 * 0.5);

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// INT0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure /INT0 (P1.0) to be a level-sensitive interrupt pin.
// Starts active-low, but the INT0 ISR will switch this back and forth.
//
//-----------------------------------------------------------------------------
void INT0_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;              // Configured for TCON_IT01CF

   TCON_IT0 = 0x00;                    // Configure /INT0 to P1.0
                                       // Active Low

   TCON_IT0    = 0;                    // /INT0 is level sensitive (TCON)

   IE_EX0    = 1;                      // Enable /INT0 interrupts (IE)

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INT0_ISR
//-----------------------------------------------------------------------------
//
// INT0_ISR is triggered upon a level transition on the /INT0 pin.
// If /INT0 is logic high, the firmware switches to a state that blinks the LED
// If /INT0 is logic low, the fimware applies PWM wave to the LED to control
// the brightness.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(INT0_ISR, INT0_IRQn)
{
   SFRPAGE = CONFIG_PAGE;

   if (TCON_IT0 & 0x08)                // Currently active-high
   {
      Blink_State = TIMER_MODE;        // State is now Timer mode
      update = 1;

      SFRPAGE = ACTIVE1_PAGE;

      // Switch to Timer Mode
      PCA0CN_CR      = 0;              // Stop PCA Counter (PCA0CN)
      TMR3CN |= 0x04;                  // Start Timer3

      SFRPAGE = CONFIG_PAGE;

      TCON_IT0 &= ~0x08;               // Switch to active-low

      XBR1   &= ~0x04;                 // Remove CEX0 from Crossbar

   }
   else
   {
      Blink_State = PWM_MODE;          // State is now PWM mode
      update = 1;

      SFRPAGE = ACTIVE1_PAGE;

      // Switch To PWM Mode
      TMR3CN &= ~0x04;                 // Stop Timer3
      PCA0CN_CR = 1;                   // Start PCA Counter (PCA0CN)

      SFRPAGE = CONFIG_PAGE;

      TCON_IT0 |= 0x08;                // Switch to active-high

      XBR1   |= 0x04;                  // Put CEX0 on Crossbar (P1.2)

   }
}

//-----------------------------------------------------------------------------
// TIMER0_ISR
//-----------------------------------------------------------------------------
//
// This ISR is triggered upon a Timer0 overflow.  It is used to update the
// UART with the status of the LED.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER0_ISR, TIMER0_IRQn)
{
   static uint16_t overflows = 0;
   uint16_t PWM_duty_cycle_inverse;

   // Timer0 is used to schedule the number of times per second the status is
   // printed to the UART.  Running Timer0 from SYSCLK requires keeping track
   // of the number of Timer0 overflows to schedule a reasonable UART update
   // rate.  Updating on every Timer0 overflow would print too fast.
   // The benefit is that there is more flexibility in scheduling the update
   // rate as it is no longer any dependent on the fixed UART prescaler.

   overflows++;

   SFRPAGE = ACTIVE1_PAGE;             // Configure for writes to SBUF0

   if (overflows == OVERFLOWS_NEEDED)  // Time to print
   {
      if ((Blink_State == TIMER_MODE) & update)          // Timer mode
      {
         putchar ('\f');               // Clear screen

         Print_String ("LED Blink Rate: ");
         // Add a hundreds digit if <MAX_BLINK_RATE> requires one
         putchar ((Blink_Rate / 10) + 0x30);             // Tens digit
         putchar ((Blink_Rate % 10) + 0x30);             // Ones digit
         Print_String (" Hz\n");

         update = 0;
      }
      else if ((Blink_State == PWM_MODE) & update)       // PWM mode
      {
         putchar ('\f');               // Clear screen

         PWM_duty_cycle_inverse = 100 - PWM_Duty_Cycle;

         Print_String ("LED PWM Duty Cycle: ");
         putchar ((PWM_duty_cycle_inverse / 100) + 0x30);        // Hundreds digit
         putchar (((PWM_duty_cycle_inverse / 10) % 10)  + 0x30); // Tens digit
         putchar ((PWM_duty_cycle_inverse % 10)  + 0x30);        // Ones digit
         Print_String (" %\n");

         update = 0;
      }

      overflows = 0;                   // Reset the count
   }
}

//-----------------------------------------------------------------------------
// ADC0_ISR
//-----------------------------------------------------------------------------
//
// ADC0_ISR is triggerered upon the completion of an ADC conversion.  The ISR
// calculates the Blink_Rate or PWM_Duty_Cycle based on the voltage
// across the potentiometer.
//
// Blink mode:
// A full-scale reading will configure the Timer to blink the LED at
// <MAX_BLINK_RATE>
// A zero reading will configure the Timer the LED to blink at <MIN_BLINK_RATE>
//
// PWM mode:
// A full scale value will configure the PCA to use a duty cycle of 100%
// A zero reading will configure the PCA to use a duty cycle of 0%
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC0_ISR, ADC0EOC_IRQn)
{
   uint16_t ADC_result;
   uint32_t reload_speed, percentage;
   static uint16_t PWM_Duty_Cycle_Old = 0;
   static uint16_t Blink_Rate_Old     = 0;

   // The ADC, PCA, and TIMER registers are available on all the ACTIVE1_PAGE

   ADC0CN_ADINT = 0;                   // Clear conversion complete flag

   ADC_result = ADC0;

   if (Blink_State == TIMER_MODE)
   {
      if (ADC_result == 0x0FFF)        // Assume max reading indicates VDD
      {                                // and not (VDD - 1 LSB)
         reload_speed = MAX_BLINK_RATE;
      }
      else
      {
         reload_speed = (uint32_t) ((uint32_t) ADC_result * (uint32_t) BLINK_DIFF);
         reload_speed =  reload_speed / ADC_COUNTS;
         reload_speed += MIN_BLINK_RATE;
      }

      Blink_Rate = reload_speed;       // Set global variable

      if (Blink_Rate != Blink_Rate_Old) {
         update = 1; }

      Blink_Rate_Old = Blink_Rate;

      TMR3RL  = - ((uint16_t) (SYSCLK / 12 / reload_speed));
   }
   else
   {

      if (ADC_result == 0x0FFF)        // Assume max reading indicates VDD
      {                                // and not (VDD - 1 LSB)
         percentage = 255;
         PWM_Duty_Cycle = 100;
      }
      else
      {
         percentage = (uint32_t) ADC_result * (uint32_t) 256;
         percentage = (uint32_t) percentage / (uint32_t) ADC_COUNTS;
         PWM_Duty_Cycle = (uint16_t) ( ((uint32_t)ADC_result * (uint32_t)100) / (uint32_t)ADC_COUNTS );
      }

      if (PWM_Duty_Cycle != PWM_Duty_Cycle_Old) {
         update = 1; }

      PWM_Duty_Cycle_Old = PWM_Duty_Cycle;

      // Set new PCA0 Module0 PWM duty cycle
      PCA0CPH0 = (uint8_t) percentage;
   }
}


//-----------------------------------------------------------------------------
// TIMER3_ISR
//-----------------------------------------------------------------------------
//
// This ISR is triggered upon a Timer3 overflow.  The ISR simply toggles
// the state of the LED and keeps track of the number of times the LED blinks.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER3_ISR, TIMER3_IRQn)
{
   // The TMR3CN and Port registers are available on the ACTIVE1_PAGE, so there
   // is no need to switch the SFRPAGE.

   TMR3CN &= ~0x80;                    // Clear Timer3 Flags

   LED = !LED;

   Num_LED_Flashes++;
}


//-----------------------------------------------------------------------------
// Support Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Print_String
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//         1) uint8_t pstring[] - null terminated character string
//
// Prints the strings stored in pstring to the UART.
//
//-----------------------------------------------------------------------------
void Print_String (uint8_t pstring[])
{
   uint8_t i = 0;

   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE;

   while (pstring[i])
   {
      putchar(pstring[i++]);
   }

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
