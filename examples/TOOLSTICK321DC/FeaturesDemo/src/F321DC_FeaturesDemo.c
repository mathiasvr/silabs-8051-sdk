//-----------------------------------------------------------------------------
// F321DC_FeaturesDemo.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the features available on the ToolStick
// F321 Daughter Card.
//
// This example will either blink the green LED (D2) or adjust its brightness
// by using PWM. The blinking speed or the PWM duty cycle is set based on the
// value of the potentiometer. Voltage across the potentiometer is measured
// using the on-chip 10-bit ADC. Pressing the button S1 (P2.0) will switch
// between blink and brightness modes.
//
// The program also outputs the status of the LED to the UART at a baud rate
// defined by <BAUDRATE>.
//
// How To Test:
// 1) Connect the ToolStick321DC to a ToolStick Base Adapter.
// 2) Compile and download code to the ToolStick Daughter Card by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 3) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 4) Turn the potentiometer and see that the blink rate increases or decreases.
// 5) To toggle between blink and brightness modes, press switch S1 (P2.0).
// 6) Turn the potentiometer and see that the brightness of the LED changes.
//
//
// Target:         ToolStick321DC
// Tool chain:     Simplicity Studio / Keil C51 9.51
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 21 MAR 2014
//
// Release 1.0
//    -Initial Revision (PD)
//    -3/24/08
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F320_Register_Enums.h"
#include "stdio.h"
#include "stdlib.h"

//-----------------------------------------------------------------------------
// SFR and Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P2, 2);                  // LED='1' means ON
SI_SBIT(S1, SFR_P2, 0);                   // SW = '0' means button is pressed
//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSTEMCLOCK   12000000L/4      // SYSTEMCLOCK frequency in Hz

#define BAUDRATE        115200         // Baud rate of UART in bps
#define PRINT_RATE      20             // Times per second to update on UART
#define OVERFLOWS_NEEDED  (SYSTEMCLOCK / 65536 / PRINT_RATE)

#define ADC_CLOCK       100000         // SAR clock
#define ADC_SAMPLE_RATE 100            // Timer 2 overflow rate in Hz
#define ADC_COUNTS      1024           // Number of ADC counts

#define MAX_BLINK_RATE  40             // Max blink rate in Hz for Timer mode
#define MIN_BLINK_RATE  1              // Min blink rate in Hz for Timer mode
#define BLINK_DIFF  (MAX_BLINK_RATE - MIN_BLINK_RATE) // For easy calculations

// Firmware states
#define TIMER_MODE      0              // Uses the Timer to blink the LED
#define PWM_MODE        1              // Use a PCA channel to PWM the LED

#define SW_PRESSED      0
#define SW_NOT_PRESSED  1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(ADC0_ISR, 10);
SI_INTERRUPT_PROTO(TIMER0_ISR, 1);

void OSCILLATOR_Init (void);
void PORT_Init (void);
void TIMER0_Init (void);
void TIMER2_Init (int16_t counts);
void ADC0_Init (void);
void PCA0_Init (void);
void UART0_Init (void);
void INT0_Init (void);

void Print_String (char pstring[]);
void LED_SwitchModes(void);

// ISRs defined: INT0_ISR, TIMER0_ISR, ADC0_ISR.

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

bit Blink_State      = TIMER_MODE;     // Starts with blinking the LED

int16_t PWM_Duty_Cycle   = 0;              // Percentage from 0 to 100
int16_t Blink_Rate       = 0;              // From MIN_BLINK_RATE to MAX_BLINK_RATE
bit update           = 0;              // Forces a UART update

int32_t Num_LED_Flashes = 0;              // Simply counts the number of times
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
   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
                                       // enable)

   OSCILLATOR_Init ();                 // Initialize system clock to 24.5 Mhz

   PORT_Init ();                       // Initialize crossbar and GPIO

   TIMER0_Init ();                     // Initialize Timer0 for UART update

   // Init Timer2 for ADC
   TIMER2_Init (SYSTEMCLOCK / 12 / ADC_SAMPLE_RATE);   

   PCA0_Init ();                       // Init PCA Module 0 to blink LED

   ADC0_Init ();                       // Enable ADC0

   UART0_Init ();                      // Enable UART using global settings

   IE_EA = 1;                             // Enable global interrupts

   while (1)
   {
      //Wait for S1 to be pressed and released:
      while (S1 != SW_PRESSED);
      while (S1 != SW_NOT_PRESSED);

      //Switch LED modes between blinking and brightness
      LED_SwitchModes();
   }
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// TIMER0_ISR
//-----------------------------------------------------------------------------
//
// This ISR is triggered upon a Timer0 overflow.  It is used to update the
// UART with the status of the LED.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER0_ISR, 1)
{
   static int16_t overflows = 0;

   // Timer0 is used to schedule the number of times per second the status is
   // printed to the UART.  Since Timer0 and Timer1 share a SYCLK prescaler, it
   // is easier to use the prescaler only for Timer1 (UART baud rate) and run
   // Timer0 from SYSTEMCLOCK.  Running Timer0 from SYSTEMCLOCK requires keeping 
   // track of the number of Timer0 overflows to schedule a reasonable UART 
   // update rate.  Updating on every Timer0 overflow would  print too fast.
   // The benefit is that there is more flexibility in scheduling the update
   // rate as it is no longer any dependent on the fixed UART prescaler.

   overflows++;

   if (overflows == OVERFLOWS_NEEDED)  // Time to print
   {
      // Timer mode
      if ((Blink_State == TIMER_MODE) & update)          
      {
         putchar ('\f');               // Clear screen

         Print_String ("LED Blink Rate: ");
         // Add a hundreds digit if <MAX_BLINK_RATE> requires one
         // Tens digit
         putchar ((Blink_Rate / 10) + 0x30);             
         // Ones digit
         putchar ((Blink_Rate % 10) + 0x30);             
         Print_String (" Hz\n");

         update = 0;
      }
      // PWM mode
      else if ((Blink_State == PWM_MODE) & update)       
      {
         putchar ('\f');               // Clear screen

         Print_String ("LED PWM Duty Cycle: ");
         // Hundreds digit
         putchar ((PWM_Duty_Cycle / 100) + 0x30);        
         // Tens digit
         putchar (((PWM_Duty_Cycle / 10) % 10)  + 0x30); 
         // Ones digit
         putchar ((PWM_Duty_Cycle % 10)  + 0x30);        
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
SI_INTERRUPT(ADC0_ISR, 10)
{
   uint16_t ADC_result;
   uint32_t reload_speed, percentage;
   static int16_t PWM_Duty_Cycle_Old = 0;
   static int16_t Blink_Rate_Old     = 0;
   static uint8_t ADC_Blink_Counter = 0;

   ADC0CN_ADINT = 0;                         // Clear conversion complete flag

   ADC_result = ADC0;

   if (Blink_State == TIMER_MODE)
   {
      if (ADC_result == 0x03FF)        // Assume max reading indicates VDD
      {                                // and not (VDD - 1 LSB)
         reload_speed = MAX_BLINK_RATE;
      }
      else
      {
         reload_speed = (int32_t) (ADC_result * BLINK_DIFF);
         reload_speed =  reload_speed / ADC_COUNTS;
         reload_speed += MIN_BLINK_RATE;
      }

      Blink_Rate = reload_speed;       // Set global variable

      if (Blink_Rate != Blink_Rate_Old) {
         update = 1; }

      Blink_Rate_Old = Blink_Rate;

      TMR3RL  = - ((uint16_t) (SYSTEMCLOCK / 12l / reload_speed));

      // Toggle the state of the LED every <ADC_SAMPLE_RATE/Blink_Rate>
      // ADC interrupt
      if (ADC_Blink_Counter++ >= (ADC_SAMPLE_RATE/Blink_Rate))
      {
         LED = !LED;
         Num_LED_Flashes++;
         ADC_Blink_Counter = 0;
      }
      
   }
   else
   {

      if (ADC_result == 0x03FF)        // Assume max reading indicates VDD
      {                                // and not (VDD - 1 LSB)
         percentage = 255;
         PWM_Duty_Cycle = 100;
      }
      else
      {
         percentage = (int32_t) ADC_result * (int32_t) 256;
         percentage = (int32_t) percentage / (int32_t) ADC_COUNTS;
         PWM_Duty_Cycle = (int16_t) ( ((int32_t)ADC_result * (int32_t)100) / (int32_t)ADC_COUNTS );
      }

      if (PWM_Duty_Cycle != PWM_Duty_Cycle_Old) {
         update = 1; }

      PWM_Duty_Cycle_Old = PWM_Duty_Cycle;

      // Set new PCA0 Module0 PWM duty cycle
      PCA0CPH0 = (char) percentage;
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
// This function initializes the system clock to use the internal 12MHz/4
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{

   OSCICN = 0x81;                      // Configure internal oscillator for
                                       // 12MHz
   RSTSRC = 0x04;                      // Enable missing clock detector
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
//
// P0.0   digital   open-drain    /INT0
// P0.4   digital   push-pull     UART TX
// P0.5   digital   open-drain    UART RX
// P1.7   analog                  Potentiometer (ADC input)
// P2.0   digital   open-drain    SW push button switch
// P2.2   digital   push-pull     LED (and CEX0 depending on Blink_State)
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P1MDIN   &=~0x80;                   // P1.7 is analog, rest of P1 digital
   P0MDOUT   = 0x10;                   // P0.4 is push-pull
   P2MDOUT   = 0x04;                   // P2.2 is push-pull

   P0SKIP    = 0xCF;                   // Skip all of P0 except for UART0 pins
   P1SKIP    = 0xFF;                   // Skip port pins on P1
   P2SKIP    = 0x03;                   // Skip P2.0, P2.1 to route PCA to P2.2

   XBR0      = 0x01;                   // Enable UART
   XBR1      = 0x40;                   // Enable Crossbar; pull-ups enabled
}

//-----------------------------------------------------------------------------
// TIMER0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Initializes the Timer in 16-bit reload mode using SYSTEMCLOCK as the time base.
//
//-----------------------------------------------------------------------------
void TIMER0_Init (void)
{

   // Initialize Timer0; set global variable for number overflows to reach
   // interrupt rate set by <PRINT_RATE>

   TMOD   |= 0x01;                     // Mode 1 - 16-bit counter
   CKCON  |= 0x04;                     // Use SYSTEMCLOCK as timebase

   TL0     = 0;                        // Clear Timer0 low byte
   TH0     = 0;                        // Clear Timer0 high byte

   IE     |= 0x02;                     // Enable Timer0 Interrupts

   TCON_TR0    = 1;                         // Enable Timer 0
}

//-----------------------------------------------------------------------------
// TIMER2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1)  int16_t counts - number of milliseconds of delay
//                    range is postive range of integer: 0 to 32767
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSTEMCLOCK/12 as its time base.
//
//-----------------------------------------------------------------------------
void TIMER2_Init (int16_t counts)
{
   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSTEMCLOCK/12 as timebase
   CKCON  &= ~0x30;                    // Timer2 clocked based on TMR2CN_T2XCLK;

   TMR2RL  = -counts;                  // Init reload values
   TMR2    = 0xffff;                   // Set to reload immediately
   IE_ET2     = 0;                        // Disable Timer2 interrupts
   TMR2CN_TR2     = 1;                        // Start Timer2
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configures ADC0 to make single-ended analog measurements on pin P0.1
//  
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   ADC0CN = 0x02;                      // ADC0 disabled, normal tracking, 
                                       // conversion triggered on TMR2 overflow

   REF0CN = 0x08;                      // Enable on-chip VREF and buffer

   AMX0P = 0x07;                       // ADC0 positive input = P1.7
   AMX0N = 0x1F;                       // ADC0 negative input = GND
                                       // i.e., single ended mode

   // Set SAR clock to 3MHz
   ADC0CF = ((SYSTEMCLOCK/3000000)-1)<<3;   

   ADC0CF &= ~0x04;                    // Right-justify results 

   EIE1 |= 0x08;                       // Enable ADC0 conversion complete int.

   ADC0CN_ADEN = 1;                          // Enable ADC0
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART1 using Timer1, for <baudrate> and 8-N-1.
//
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear SCON0_RI and SCON0_TI bits
   if (SYSTEMCLOCK/BAUDRATE/2/256 < 1) {
      TH1 = -(SYSTEMCLOCK/BAUDRATE/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   } else if (SYSTEMCLOCK/BAUDRATE/2/256 < 4) {
      TH1 = -(SYSTEMCLOCK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x09;
   } else if (SYSTEMCLOCK/BAUDRATE/2/256 < 12) {
      TH1 = -(SYSTEMCLOCK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   } else {
      TH1 = -(SYSTEMCLOCK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   }

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                            // START Timer1
   SCON0_TI = 1;                            // Indicate TX0 ready
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
   PCA0MD = 0x08;                      // Use SYSTEMCLOCK as time base

   PCA0CPM0 = 0x42;                    // Module 0 = 8-bit PWM mode

   // Configure initial PWM duty cycle = 50%
   PCA0CPH0 = 256 - (256 * 0.5);

}

//-----------------------------------------------------------------------------
// Support Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// LED_SwitchModes
//-----------------------------------------------------------------------------
//
// LED_SwitchModes is called upon S1 (P2.0) being pressed and released.
// Calling this function switches between the LED blinking and a PWM being
// applied to the LED to control its brightness.
//
//-----------------------------------------------------------------------------
void LED_SwitchModes(void)
{
   if (Blink_State == PWM_MODE)                  // Currently active-high
   {
      Blink_State = TIMER_MODE;        // State is now Timer mode
      update = 1;

      // Switch to Timer Mode
      PCA0CN_CR      = 0;                     // Stop PCA Counter
      XBR1   &= ~0x01;                 // Remove CEX0 from Crossbar
      TMR3CN |= 0x04;                  // Start Timer3
   }
   else
   {
      Blink_State = PWM_MODE;          // State is now PWM mode
      update = 1;

      // Switch To PWM Mode
      TMR3CN &= ~0x04;                 // Stop Timer3
      XBR1   |= 0x01;                  // Put CEX0 on Crossbar (P1.3)
      PCA0CN_CR      = 1;                     // Start PCA Counter
   }
}

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

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
