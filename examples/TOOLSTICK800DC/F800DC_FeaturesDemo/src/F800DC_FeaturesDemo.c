//-----------------------------------------------------------------------------
// F800DC_FeaturesDemo.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates the features available on the ToolStick
// F800 Daughter Card.
//
// Based on the setting of a level-sensitive pin, the firmware
// will either blink the green LED or PWM the LED.  The blinking speed
// or the PWM duty cycle is set based on the value of the potentiometer.
// Voltage across the potentiometer is measured using the on-chip 10-bit ADC.
//
// The program also outputs the status of the LED to the UART at a baud rate
// defined by <BAUDRATE>.
//
// ToolStick Terminal can be downloaded from the following url:
// http://www.silabs.com/products/mcu/Pages/8-bit-microcontroller-software.aspx#toolstick
//
// How To Test:
// 1) Connect the ToolStick800DC to a ToolStick Base Adapter.
// 2) Compile and download code to the ToolStick Daughter Card by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 3) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 4) Connect to the ToolStick using ToolStick Terminal using the settings
//    defined by <BAUD_RATE>
// 5) Turn the potentiometer and see that the blink rate increases or decreases and
//    the UART output changes.
// 6) Press and hold switch P1.4 and turn the potentiometer and see that the LED
//    brightness changes and the UART output changes.
//
//
// Target:         ToolStickF800DC
// Tool chain:     Simplicity Studio / Keil C51 9.51
//
//
// Release 1.0
//    -Initial Revision (PKC)
//    -12 OCT 2009
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F800_Register_Enums.h>
#include <stdio.h>
#include <stdlib.h>

SI_SBIT(LED, SFR_P1, 0);  
SI_SBIT(SW, SFR_P1, 4); 
SI_SBIT(P1_2_RTS, SFR_P1, 2); 
 
//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK          24500000/8L    // SYSCLK frequency in Hz

#define BAUDRATE        115200         // Baud rate of UART in bps
#define PRINT_RATE      20             // Times per second to update on UART
#define OVERFLOWS_NEEDED  (SYSCLK / 65536 / PRINT_RATE)

#define ADC_CLOCK       100000         // SAR clock
#define ADC_SAMPLE_RATE 100            // in Hz
#define ADC_COUNTS      1024           // Number of ADC counts

#define TMR2_RATE       100            // Fixed Timer2 overflow rate in Hz
                                       // Used for ADC conversions and LED blinks

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
void TIMER2_Init (void);
void ADC0_Init (void);
void PCA0_Init (void);
void UART0_Init (void);
void PORT_MATCH_Init (void);

void Print_String (char pstring[]);

// ISRs defined: 
SI_INTERRUPT_PROTO(TIMER0_ISR, TIMER0_IRQn);
SI_INTERRUPT_PROTO(PORT_MATCH_ISR, PMATCH_IRQn);
SI_INTERRUPT_PROTO(ADC0_ISR, ADC0EOC_IRQn);
SI_INTERRUPT_PROTO(TIMER2_ISR, TIMER2_IRQn);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

bit Blink_State      = TIMER_MODE;     // Starts with blinking the LED

uint8_t PWM_Duty_Cycle    = 0;              // Percentage from 0 to 100
uint8_t Blink_Rate        = 0;              // From MIN_BLINK_RATE to MAX_BLINK_RATE
bit update           = 0;              // Forces a UART update

long Num_LED_Flashes = 0;              // Simply counts the number of times
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
   PCA0MD &= ~0x40;                    // WDTE = 0 (disable watchdog timer)
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   
   OSCILLATOR_Init ();                 // Initialize system clock to 24.5 Mhz

   PORT_Init ();                       // Initialize crossbar and GPIO

   TIMER0_Init ();                     // Initialize Timer0 for UART update

   TIMER2_Init ();                     // Init timer used to blink LED

   PCA0_Init ();                       // Init PCA Module 0 to blink LED

   ADC0_Init ();                       // Enable ADC0

   PORT_MATCH_Init ();                 // Enable port match

   UART0_Init ();                      // Enable UART using global settings

   IE_EA = 1;                             // Enable global interrupts

   while (1)
   {
      // Toggle the ADC enable bit when the switch is pressed
   }
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// PORT_MATCH_ISR
//-----------------------------------------------------------------------------
//
// PORT_MATCH_ISR is triggered upon a level transition on the P1.4 pin.
// If P1.4 is logic high, the firmware switches to a state that blinks the LED
// If P1.4 is logic low, the fimware applies PWM wave to the LED to control
// the brightness.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PORT_MATCH_ISR, 8)
{
   if (P1_2_RTS == 1)                  // If P1.2 (RTS) is logic high, allow
                                       // control using the P1.4 push-button switch
   {   
      P1MAT |= 0x04;                   // Switch P1.2 to active-low
      P1MASK |= 0x10;                  // Enable P1.4 switch port match interrupts

      /*if (Blink_State != TIMER_MODE)
      {
         Blink_State = TIMER_MODE;        // State is now Timer mode
         update = 1;
         
         // Switch to Timer Mode
         PCA0CN_CR      = 0;                     // Stop PCA Counter
   
         XBR1   &= ~0x01;                 // Remove CEX0 from Crossbar
      }*/
      
      if (SW == 1)                     // P1.4 currently active-high
      {
         P1MAT |= 0x10;                   // Switch P1.4 to active-low
   
         Blink_State = TIMER_MODE;        // State is now Timer mode
         update = 1;
    
         // Switch to Timer Mode
         PCA0CN_CR      = 0;                     // Stop PCA Counter
   
         XBR1   &= ~0x01;                 // Remove CEX0 from Crossbar
      }
      else
      {
         P1MAT &= ~0x10;                  // Switch P1.4 to active-high
   
         Blink_State = PWM_MODE;          // State is now PWM mode
         update = 1;
   
         // Switch To PWM Mode
         PCA0CN_CR      = 1;                     // Start PCA Counter
   
         XBR1   |= 0x01;                  // Put CEX0 on Crossbar (P1.0)
      }
   }
   else
   {
      P1MAT &= ~0x04;                  // Switch P1.2 to active-high
      P1MASK &= ~0x10;                 // Ignore P1.4 switch

      if (Blink_State  != PWM_MODE)
      {
         Blink_State = PWM_MODE;          // State is now PWM mode
         update = 1;
   
         // Switch To PWM Mode
         PCA0CN_CR      = 1;                     // Start PCA Counter
   
         XBR1   |= 0x01;                  // Put CEX0 on Crossbar (P1.0)
      }
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
   static int overflows = 0;

   // Timer0 is used to schedule the number of times per second the status is
   // printed to the UART.  Since Timer0 and Timer1 share a SYCLK prescaler, it
   // is easier to use the prescaler only for Timer1 (UART baud rate) and run
   // Timer0 from SYSCLK.  Running Timer0 from SYSCLK requires keeping track
   // of the number of Timer0 overflows to schedule a reasonable UART update
   // rate.  Updating on every Timer0 overflow would  print too fast.
   // The benefit is that there is more flexibility in scheduling the update
   // rate as it is no longer any dependent on the fixed UART prescaler.

   overflows++;

   if (overflows == OVERFLOWS_NEEDED)                    // Time to print
   {
      if ((Blink_State == TIMER_MODE) & update)          // Timer mode
      {
         putchar ('\f');                                 // Clear screen

         Print_String ("LED Blink Rate: ");
         // Add a hundreds digit if <MAX_BLINK_RATE> requires one
         putchar (((MAX_BLINK_RATE-Blink_Rate+1) / 10) + 0x30);             // Tens digit
         putchar (((MAX_BLINK_RATE-Blink_Rate+1) % 10) + 0x30);             // Ones digit
         Print_String (" Hz\n");

       update = 0;
      }
      else if ((Blink_State == PWM_MODE) & update)       // PWM mode
      {
         putchar ('\f');                                 // Clear screen

         Print_String ("LED PWM Duty Cycle: ");
         putchar (((100-PWM_Duty_Cycle) / 100) + 0x30);        // Hundreds digit
         putchar ((((100-PWM_Duty_Cycle) / 10) % 10)  + 0x30); // Tens digit
         putchar (((100-PWM_Duty_Cycle) % 10)  + 0x30);        // Ones digit
         Print_String (" %\n");

       update = 0;
      }

      overflows = 0;                                     // Reset the count
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
   unsigned int ADC_result;
   unsigned long reload_speed, percentage;
   static int PWM_Duty_Cycle_Old = 0;
   static int Blink_Rate_Old     = 0;

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
         reload_speed = (long) (ADC_result * BLINK_DIFF);
         reload_speed =  reload_speed / ADC_COUNTS;
         reload_speed += MIN_BLINK_RATE;
      }

      Blink_Rate = reload_speed;       // Set global variable

      if (Blink_Rate != Blink_Rate_Old) {
         update = 1; }

      Blink_Rate_Old = Blink_Rate;
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
         percentage = (long) ADC_result * (long) 256;
         percentage = (long) percentage / (long) ADC_COUNTS;
         PWM_Duty_Cycle = (int) ( ((long)ADC_result * (long)100) / (long)ADC_COUNTS );
      }

      if (PWM_Duty_Cycle != PWM_Duty_Cycle_Old) {
         update = 1; }

      PWM_Duty_Cycle_Old = PWM_Duty_Cycle;

      // Set new PCA0 Module0 PWM duty cycle
      PCA0CPH0 = (char) percentage;
   }
}


//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// This ISR is triggered upon a TIMER2 overflow. If LED control is in 
// "TIMER_MODE", the ISR toggles the state of the LED and keeps track of the 
// number of times the LED blinks.
// The timer is also used for periodically starting ADC conversions.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
   static uint32_t TMR2_Count = 0;
   
   TMR2CN &= ~0x80;                    // Clear TIMER2 Flags

   if ((Blink_State == TIMER_MODE) && (TMR2_Count % Blink_Rate == 0))
   {
      LED = !LED;
      Num_LED_Flashes++;
   }
   
   ADC0CN_ADBUSY = 1;                     // Start ADC0 Conversion
   
   TMR2_Count++;
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
// This function initializes the system clock to use the internal 24.5/8 MHz
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   OSCICN |= 0x80;                     // Enable the precision internal osc.
   OSCICN &= ~0x03;                    // IFCN=00b (for div by 8)

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock
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
// P0.4   digital   push-pull     UART TX
// P0.5   digital   open-drain    UART RX
// P0.7   analog                  Potentiometer (ADC input)
// P1.0   digital   push-pull     LED (and CEX0 depending on Blink_State)
// P1.2   digital   open-drain    Digital input (RTS)
// P1.4   digital   open-drain    SW push button switch

//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P0MDIN   &= ~0x80;                  // P0.7 is analog, rest of P0 digital
   P0MDOUT   = 0x10;                   // P0.4 is push-pull
   P1MDOUT   = 0x01;                   // P1.0 is push-pull

   P0SKIP    = 0xCF;                   // Skip all of P0 except for UART0 pins

   P0       |= 0x20;                   // Set port latch for pin RX
                                       // to configure as input

   P1       |= 0x14;                   // Set port latch for push-button and RTS
                                       // to configure as input

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
// Initializes the Timer in 16-bit reload mode using SYSCLK as the time base.
//
//-----------------------------------------------------------------------------
void TIMER0_Init (void)
{
   // Initialize Timer0; set global variable for number overflows to reach
   // interrupt rate set by <PRINT_RATE>

   TMOD   |= 0x01;                     // Mode 1 - 16-bit counter
   CKCON  |= 0x04;                     // Use SYSCLK as timebase

   TL0     = 0;                        // Clear Timer0 low byte
   TH0     = 0;                        // Clear Timer0 high byte

   IE_ET0     = 1;                        // Enable Timer0 Interrupts

   TCON_TR0     = 1;                        // Enable Timer 0
}

//-----------------------------------------------------------------------------
// TIMER2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure TIMER2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/12 as its time base.
//
// Initially sets TIMER2 to overflow at the maximum blink rate.  The TIMER2 ISR
// is used to toggle the LED.
//
//-----------------------------------------------------------------------------
void TIMER2_Init (void)
{
   TMR2CN  = 0x00;                     // Stop TIMER2; Clear flags;
                                       // use SYSCLK/12 as timebase

   CKCON  &= ~0xC0;                    // TIMER2 clocked based on T3XCLK;

   TMR2RL  = -(SYSCLK / 12 / TMR2_RATE);  // Init reload values

   TMR2    = 0xffff;                   // Set to reload immediately
   IE_ET2     = 1;                        // Enable TIMER2 interrupts

   TMR2CN |= 0x04;                     // Use SYSCLK/12 as timebase
                                       // Start TIMER2
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the ADC to measure potetiometer connected
// to P0.7 in single-ended mode.  Also enables the internal voltage reference.
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   ADC0MX = 0x07;                      // Select P0.7 as the ADC input pin

   ADC0CN = 0x00;                      // ADC0 disabled, normal tracking,
                                       // conversion triggered on write to ADC0CN_ADBUSY

   REF0CN = 0x08;                      // Select VDD as voltage reference

   ADC0CF = ((SYSCLK/8300000)-1)<<3;   // Set SAR clock to 8.3MHz

   ADC0CF |= 0x01;                     // Set ADC gain to 1

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
   if (SYSCLK/BAUDRATE/2/256 < 1) {
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   } else if (SYSCLK/BAUDRATE/2/256 < 4) {
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x09;
   } else if (SYSCLK/BAUDRATE/2/256 < 12) {
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   } else {
      TH1 = -(SYSCLK/BAUDRATE/2/48);
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
   PCA0MD = 0x08;                      // Use SYSCLK as time base

   PCA0CPM0 = 0x42;                    // Module 0 = 8-bit PWM mode

   // Configure initial PWM duty cycle = 50%
   PCA0CPH0 = 256 - (256 * 0.5);

   // Start PCA counter
   PCA0CN_CR = 1;
}

//-----------------------------------------------------------------------------
// PORT_MATCH_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure P1.4 to be a level-sensitive interrupt pin.
// Starts active-low, but the PORT MATCH ISR will switch this back and forth.
//
// Configure P1.2 to be a level-sensitive interrupt pin.
// Starts active-low, but the PORT MATCH ISR will switch this back and forth.
//
//-----------------------------------------------------------------------------
void PORT_MATCH_Init (void)
{
   P1MASK = 0x14;                      // Set P1.4 and P1.2 as the pins to monitor
   P1MAT  = 0x14;                      // Interrupt when P1.4 or P1.2 is low

   EIE1 |= 0x02;                       // Enable Port Match Interrupts
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
   unsigned char i = 0;
   while (pstring[i])
   {
      putchar(pstring[i++]);
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
