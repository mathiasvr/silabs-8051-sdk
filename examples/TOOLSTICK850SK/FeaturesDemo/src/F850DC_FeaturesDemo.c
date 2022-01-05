//-----------------------------------------------------------------------------
// F850DC_FeaturesDemo.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the features available on the ToolStick
// F850 Daughter Card.
//
// Based on the setting of a level-sensitive interrupt pin, the firmware
// will either blink the red LED (DS1) or PWM the LED.  The blinking speed
// or the PWM duty cycle is set based on the value of the potentiometer.
// Voltage across the potentiometer is measured using the on-chip ADC.
//
// The program also outputs the status of the LED to the UART at a baud rate
// defined by <BAUDRATE>.
//
// Pressing the S2 switch on the daughter card toggles the ADC enable bit.  If
// the ADC is disabled, turning the potentiometer will not have any effect
// on blink or PWM rate.
//
// How To Test:
// 1) Connect the ToolStick381DC to a ToolStick Base Adapter.
// 2) Compile and download code to the ToolStick Daughter Card by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 3) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 4) Turn the potentiometer and see that the blink rate increases or decreases
// 5) Press the switch S2 to turn off the ADC and check that turning the
//    potentiometer has no effect on the blink rate.
// 6) Press the switch S2 again to re-enable the ADC and potentiometer.
// 7) Press the switch S1 to toggle between PWM mode and blink rate mode.
// 8) Turn the potentiometer and see that the brightness or blink rate of the
//    LED changes.
//
//
// Target:         ToolStick850DC
// Tool chain:     Simplicity Studio / Keil C51 9.51
//
// Release 1.2 (BL)
//    - Updated Description / How to Test
//    - 21 MAR 2014
//
// Release 1.1 (TP)
//    -Updated ADC0CN_ADEN, ADC0CN_ADINT, and SCON0_TI to the correct names
//    -1 AUG 2013
//
// Release 1.0
//    -Initial Revision (TP)
//    -29 APR 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F850_Register_Enums.h"
#include "stdio.h"
#include "stdlib.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(DS1, SFR_P1, 0);                  // '0' means ON, '1' means OFF
SI_SBIT(DS2, SFR_P1, 1);                  // '0' means ON, '1' means OFF
SI_SBIT(GPIO0, SFR_P1, 5);                // GPIO0/RTS from ToolStick Terminal
SI_SBIT(S1, SFR_P1, 7);                   // S1 == 0 means switch pressed
SI_SBIT(S2, SFR_P2, 1);                   // S2 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK            24500000L/8L // SYSCLK frequency in Hz

#define BAUDRATE                115200 // Baud rate of UART in bps
#define PRINT_RATE                  20 // Times per second to update on UART

#define OVERFLOWS_NEEDED  (SYSCLK / 65536 / PRINT_RATE)

#define ADC_CLOCK               100000 // SAR clock
#define ADC_SAMPLE_RATE            100 // Timer 2 overflow rate in Hz
#define MAX_ADC_COUNTS           1024L // Measured potentiometer ADC result
                                       // that results in 100% PWM or
                                       // MAX_BLINK_RATE

#define MAX_BLINK_RATE              40 // Max blink rate in Hz for Timer mode
#define MIN_BLINK_RATE               4 // Min blink rate in Hz for Timer mode
#define BLINK_DIFF  (MAX_BLINK_RATE - MIN_BLINK_RATE) // For easy calculations

// Firmware states
#define TIMER_MODE                   0 // Uses the Timer to blink the LED
#define PWM_MODE                     1 // Use a PCA channel to PWM the LED

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(TIMER3_ISR, TIMER3_IRQn);
SI_INTERRUPT_PROTO(ADC0_ISR, ADC0EOC_IRQn);
SI_INTERRUPT_PROTO(TIMER0_ISR, TIMER0_IRQn);
SI_INTERRUPT_PROTO(PORT_MATCH_ISR, PMATCH_IRQn);

void Port_Init(void);
void Timer0_Init(void);
void Timer2_Init(uint16_t counts);
void Timer3_Init(void);
void ADC0_Init(void);
void PCA0_Init(void);
void UART0_Init(void);
void Port_Match_Init(void);
void Print_String(char pstring[]);
// ISRs defined: PORT_MATCH_ISR, TIMER0_ISR, TIMER1_ISR, TIMER3_ISR, ADC0_ISR.

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

bit Blink_State = TIMER_MODE;          // Starts with blinking the LED

int16_t PWM_Duty_Cycle = 0;                // Percentage from 0 to 100
int16_t Blink_Rate = 0;                    // From MIN_BLINK_RATE to MAX_BLINK_RATE
bit update = 0;                        // Forces a UART update

int32_t Num_LED_Flashes = 0;              // Simply counts the number of times
                                       // the LED blinks

bit S1_pressed = 0;

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
   WDTCN = 0xDE;
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   uint16_t i;

   // Disable the watchdog timer

   Port_Init();                        // Initialize crossbar and GPIO

   Timer0_Init();                      // Initialize Timer0 for UART update

   Timer2_Init(SYSCLK / 12 / ADC_SAMPLE_RATE); // Init Timer2 for ADC

   Timer3_Init();                      // Init timer used to blink LED

   PCA0_Init();                        // Init PCA Module 0 to blink LED

   ADC0_Init();                        // Enable ADC0

   Port_Match_Init();                  // Enable Port Match

   UART0_Init();                       // Enable UART using global settings

   IE_EA = 1;                             // Enable global interrupts

   while (1)
   {
      // Toggle the ADC enable bit when the S2 switch is pressed
      if(S2 == 0)
      {
         ADC0CN0_ADEN = !ADC0CN0_ADEN;                 // Toggle the bit
         DS2 = !DS2;

         while(S2 == 0);               // Wait until S2 is released

         for(i = 0; i < 10000; i++);

         while(S2 == 0);               // Wait until S2 is released
      }

      // Check if the S1 switch was pressed and triggered the Port Match ISR
      if(S1_pressed == 1)
      {
         while(S1 == 0);               // Wait until S1 is released

         for(i = 0; i < 10000; i++);

         while(S1 == 0);               // Wait until S1 is released

         EIE1 |= 0x02;                 // Enable the Port Match interrupt

         S1_pressed = 0;
      }
   }
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_MATCH_ISR
//-----------------------------------------------------------------------------
//
// PORT_MATCH_ISR is triggered upon level transitions on the P1.5 or P1.7 pins.
// If P1.5 is logic high, the firmware switches to a state that blinks the LED
// If P1.5 is logic low, the fimware applies PWM wave to the LED to control
// the brightness.
//
// If P1.7 transitions from high to low, the firmware toggles the state.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PORT_MATCH_ISR, PMATCH_IRQn)
{
   bit S1_state = S1;                  // Save the state of the switch, as this
                                       // determines whether P1.5 or P1.7
                                       // caused the interrupt

   if (Blink_State == PWM_MODE)
   {
      if (S1_state != 0)
      {
         P1MAT |= 0x20;                // Switch P1.5 to active-low
      }
      else
      {
         S1_pressed = 1;
         EIE1 &= ~0x02;                // Temporarily disable the Port Match
                                       // interrupt
      }

      Blink_State = TIMER_MODE;        // State is now Timer mode
      update = 1;

      // Switch to Timer Mode
      PCA0CN_CR = 0;                          // Stop PCA Counter
      TMR3CN |= 0x04;                  // Start Timer3

      XBR1 &= ~0x01;                   // Remove CEX0 from Crossbar
   }
   else
   {
      if (S1_state != 0)
      {
         P1MAT &= ~0x20;               // Switch P1.5 to active-high
      }
      else
      {
         S1_pressed = 1;
         EIE1 &= ~0x02;                // Temporarily disable the Port Match
                                       // interrupt
      }

      Blink_State = PWM_MODE;          // State is now PWM mode
      update = 1;

      // Switch To PWM Mode
      TMR3CN &= ~0x04;                 // Stop Timer3
      PCA0CN_CR = 1;                          // Start PCA Counter

      XBR1 |= 0x01;                    // Put CEX0 on Crossbar (P1.0)
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

   // Timer0 is used to schedule the number of times per second the status is
   // printed to the UART.  Since Timer0 and Timer1 share a SYSCLK prescaler,
   // it is easier to use the prescaler only for Timer1 (UART) and run
   // Timer0 from SYSCLK.  Running Timer0 from SYSCLK requires keeping track
   // of the number of Timer0 overflows to schedule a reasonable UART update
   // rate.  Updating on every Timer0 overflow would  print too fast.
   // The benefit is that there is more flexibility in scheduling the update
   // rate as it is no longer any dependent on the fixed UART prescaler.

   overflows++;

   if (overflows == OVERFLOWS_NEEDED)  // Time to print
   {
      if ((Blink_State == TIMER_MODE)  // Timer mode
          &&  update)
      {
         putchar('\f');                // Clear screen

         Print_String("LED Blink Rate: ");
         // Add a hundreds digit if <MAX_BLINK_RATE> requires one
         putchar((Blink_Rate / 10) + 0x30); // Tens digit
         putchar((Blink_Rate % 10) + 0x30); // Ones digit
         Print_String(" Hz\n");

         update = 0;
      }
      else if ((Blink_State == PWM_MODE) // PWM mode
               && update)
      {
         putchar('\f');                // Clear screen

         Print_String("LED PWM Duty Cycle: ");
         putchar((PWM_Duty_Cycle / 100) + 0x30);        // Hundreds digit
         putchar(((PWM_Duty_Cycle / 10) % 10)  + 0x30); // Tens digit
         putchar((PWM_Duty_Cycle % 10)  + 0x30);        // Ones digit
         Print_String(" %\n");

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
   static uint16_t Blink_Rate_Old = 0;

   ADC0CN0_ADINT = 0;                          // Clear conversion complete flag

   ADC_result = ADC0;

   if(Blink_State == TIMER_MODE)
   {
      if(ADC_result >= (MAX_ADC_COUNTS - 10))
      {
         reload_speed = MAX_BLINK_RATE;
      }
      else
      {
         reload_speed = (uint32_t) (ADC_result * BLINK_DIFF);
         reload_speed = reload_speed / MAX_ADC_COUNTS;
         reload_speed += MIN_BLINK_RATE;
      }

      Blink_Rate = reload_speed;       // Set global variable

      if(Blink_Rate != Blink_Rate_Old)
      {
         update = 1;
      }

      Blink_Rate_Old = Blink_Rate;

      TMR3RL = -((uint16_t) (SYSCLK / 12 / reload_speed));
   }
   else
   {
      if(ADC_result >= (MAX_ADC_COUNTS - 10))
      {
         percentage = 255;
         PWM_Duty_Cycle = 100;
      }
      else
      {
         percentage = (uint32_t) ADC_result * (uint32_t) 256;
         percentage = (uint32_t) percentage / (uint32_t) MAX_ADC_COUNTS;
         PWM_Duty_Cycle = (uint16_t) (((uint32_t)ADC_result * (uint32_t)100) / (uint32_t)MAX_ADC_COUNTS);
      }

      if (PWM_Duty_Cycle != PWM_Duty_Cycle_Old)
      {
         update = 1;
      }

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
   TMR3CN &= ~0x80;                    // Clear Timer3 Flags

   DS1 = !DS1;

   Num_LED_Flashes++;
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports.
//
//
// P0.4   digital   push-pull     UART TX
// P0.5   digital   open-drain    UART RX
//
// P1.0   digital   push-pull     LED (DS1)
// P1.1   digital   push-pull     LED (DS2)
//
// P1.2   analog                  Potentiometer (R10)
//
// P1.7   digital   open-drain    Switch S1
// P2.1   digital   open-drain    Switch S2
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // Configure Hardware Switches
   P1MDIN |= 0x80;                     // P1.7 is digital
   P1MDOUT &= ~0x80;                   // P1.7 is open-drain
   P1 |= 0x80;                         // Set P1.7 latch -> '1'
   P2MDOUT &= ~0x02;                   // P2.1 is open-drain
   P2 |= 0x02;                         // Set P2.1 latch -> '1'

   // Configure LEDs
   P1MDIN |= 0x03;                     // P1.0, P1.1 are digital
   P1MDOUT |= 0x03;                    // P1.0, P1.1 are push-pull

   // Configure UART
   P0MDOUT |= 0x10;                    // Enable TX as a push-pull output

   // Configure Potentiometer
   P1MDIN &= ~0x04;                    // P1.2 is analog
   P1 |= 0x04;                         // Set P1.2 latch -> '1'

   // Configure Crossbar
   P0SKIP |= 0xCF;
   P1SKIP |= 0x00;                 // Move PCA output to P1.0
   XBR0 = 0x01;                        // Enable UART on P0.4(TX) and P0.5(RX)
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Initializes the Timer in 16-bit reload mode using SYSCLK as the time base.
//
//-----------------------------------------------------------------------------
void Timer0_Init (void)
{
   // Initialize Timer0; set global variable for number overflows to reach
   // interrupt rate set by <PRINT_RATE>

   TMOD |= 0x01;                       // Mode 1 - 16-bit counter
   CKCON |= 0x04;                      // Use SYSCLK as timebase

   TL0 = 0;                            // Clear Timer0 low byte
   TH0 = 0;                            // Clear Timer0 high byte

   IE |= 0x02;                         // Enable Timer0 Interrupts

   TCON_TR0 = 1;                            // Enable Timer 0
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1)  int16_t counts - number of milliseconds of delay
//                    range is postive range of integer: 0 to 65535
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/12 as its time base.
//
//-----------------------------------------------------------------------------
void Timer2_Init (uint16_t counts)
{
   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON &= ~0x30;                     // Timer2 clocked based on TMR2CN_T2XCLK;

   TMR2RL = -counts;                   // Init reload values
   TMR2 = 0xffff;                      // Set to reload immediately
   IE_ET2 = 0;                            // Disable Timer2 interrupts
   TMR2CN_TR2 = 1;                            // Start Timer2
}

//-----------------------------------------------------------------------------
// Timer3_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure Timer3 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/12 as its time base.
//
// Initially sets Timer3 to overflow at the maximum blink rate.  The Timer3 ISR
// is used to toggle the DS2 LED.
//
//-----------------------------------------------------------------------------
void Timer3_Init (void)
{
   TMR3CN = 0x00;                      // Stop Timer3; Clear flags;
                                       // use SYSCLK/12 as timebase

   CKCON &= ~0xC0;                     // Timer3 clocked based on T3XCLK

   TMR3RL = (uint16_t) -(SYSCLK / 12 / MIN_BLINK_RATE); // Init reload values

   TMR3 = 0xffff;                      // Set to reload immediately
   EIE1 |= 0x80;                       // Enable Timer3 interrupts

   TMR3CN |= 0x04;                     // Start Timer3
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the ADC to measure potetiometer connected
// to P1.2.  Also enables the internal high-speed voltage reference.
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   ADC0CN0 = 0x02;                     // ADC0 disabled, conversions triggered
                                       // on Timer2 overflows

   REF0CN = 0x18;                      // Select internal high speed voltage
                                       // reference at 1.65 V

   ADC0MX = 0x0A;                      // ADC0 positive input = ADC0.10 or
                                       // P1.2

   ADC0CF = (((SYSCLK*2)/12250000)-1)<<3; // Set SAR clock to 12.25 MHz

   ADC0CF &= ~0x07;                    // 0.5x gain, normal tracking mode,
                                       // 10-bit mode

   ADC0AC = 0x00;                      // Right-justify results, no shifting
                                       // applied

   EIE1 |= 0x08;                       // Enable ADC0 conversion complete int.

   ADC0CN0_ADEN = 1;                           // Enable ADC0
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear SCON0_RI and SCON0_TI bits
   #if (SYSCLK/BAUDRATE/2/256 < 1)
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   #elif (SYSCLK/BAUDRATE/2/256 < 4)
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x01;
   #elif (SYSCLK/BAUDRATE/2/256 < 12)
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   #else
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   #endif

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                            // START Timer1
   SCON0_TI = 1;                             // Indicate TX0 ready
}

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
   // Configure PCA time base; overflow interrupt disabled
   PCA0CN = 0x00;                      // Stop counter; clear all flags
   PCA0MD = 0x08;                      // Use SYSCLK as time base

   PCA0CPM0 = 0x42;                    // Module 0 = 8-bit PWM mode

   // Configure initial PWM duty cycle = 50%
   PCA0CPH0 = 256 - (256 * 0.5);
}

//-----------------------------------------------------------------------------
// Port_Match_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure P1.7 to be a level-sensitive interrupt pin.
// Starts active-low, but the PORT MATCH ISR will switch this back and forth.
//
//-----------------------------------------------------------------------------
void Port_Match_Init (void)
{
   P1MASK = 0xA0;                      // Enable P1.5 and P1.7 to generate Port
                                       // Mis-Match events

   P1MAT = 0xA0;                       // Set expected states of P1.5 and P1.7.
                                       // P1.7 and P1.5 are now active-low
                                       // interrupt pins.

   EIE1 |= 0x02;                       // Enable Port Match interrupt
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
//         1) char pstring[] - null terminated character string
//
// Prints the strings stored in pstring to the UART.
//
//-----------------------------------------------------------------------------
void Print_String (char pstring[])
{
   uint8_t i = 0;
   while (pstring[i])
   {
      putchar(pstring[i++]);
   }
}

#if defined SDCC
void putchar (char c)
{
  if (c == '\n')
  {
    while (!SCON0_TI);
    SCON0_TI = 0;
    SBUF0 = 0x0d;                      /* output PCA0CN_CR  */
  }
  while (!SCON0_TI);
  SCON0_TI = 0;
  SBUF0 = c;
}
#endif

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
