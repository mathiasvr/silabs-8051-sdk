//-----------------------------------------------------------------------------
// F327DC_FeaturesDemo.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the features available on the ToolStick
// F327 Daughter Card.
//
// The firmware will cycle through four different states of the LED based
// on button presses: fast blink, slow blink, on, and off.
//
// The program also outputs the status of the LED to the UART at a baud rate
// defined by <BAUDRATE>.
//
// How To Test:
// 1) Connect the ToolStick327DC to a ToolStick Base Adapter.
// 2) Compile and download code to the ToolStick Daughter Card by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 3) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 4) To toggle between the LED states, press S1 (P0.0).
//
//
// Target:         ToolStick327DC
// Tool chain:     Simplicity Studio / Keil C51 9.51
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 20 MAY 2014
//
// Release 1.0
//    -Initial Revision (PD)
//    -3/24/08
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F326_Register_Enums.h"
#include "stdio.h"
#include "stdlib.h"

//-----------------------------------------------------------------------------
// SFR and Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P2, 0);                  // LED='1' means ON
SI_SBIT(SW, SFR_P0, 0);                   // SW = '0' means button is pressed
//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSTEMCLOCK      12000000      // SYSTEMCLOCK frequency in Hz

#define BAUDRATE        115200         // Baud rate of UART in bps
#define PRINT_RATE      20             // Times per second to update on UART
#define OVERFLOWS_NEEDED  (SYSTEMCLOCK / 65536 / PRINT_RATE)

#define MAX_BLINK_RATE  40             // Max blink rate in Hz for Timer mode
#define MIN_BLINK_RATE  1              // Min blink rate in Hz for Timer mode
#define BLINK_DIFF  (MAX_BLINK_RATE - MIN_BLINK_RATE) // For easy calculations

// Firmware states
#define TIMER_MODE      0              // Uses the Timer to blink the LED
#define SWITCH_MODE     1              // Use switch state to turn LED on/off 

#define FAST_BLINK_FREQUENCY 20
#define SLOW_BLINK_FREQUENCY 5

#define FAST_BLINKING ((SYSTEMCLOCK / 65536) / FAST_BLINK_FREQUENCY) / 2
#define SLOW_BLINKING ((SYSTEMCLOCK / 65536) / SLOW_BLINK_FREQUENCY)  / 2

typedef enum {
   BLINK_SLOW,
   BLINK_FAST,
   OFF,
   ON,
}ledState;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(TIMER0_ISR, 1);

void OSCILLATOR_Init (void);
void PORT_Init (void);
void TIMER0_Init (void);
void UART0_Init (void);
void Print_String (char pstring[]);
void WaitForButtonPress(void);

// ISRs defined: INT0_ISR, TIMER0_ISR.

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

bit Blink_State      = TIMER_MODE;     // Starts with blinking the LED

int16_t PWM_Duty_Cycle   = 0;              // Percentage from 0 to 100
int16_t Blink_Rate       = 0;              // From MIN_BLINK_RATE to MAX_BLINK_RATE
bit update           = 0;              // Forces a UART update

int32_t Num_LED_Flashes = 0;              // Simply counts the number of times
                                       // the LED blinks
uint8_t BlinkingFrequency;

ledState LEDState = ON;

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
   OSCILLATOR_Init ();                 // Initialize system clock to 24.5 Mhz

   PORT_Init ();                       // Initialize crossbar and GPIO

   TIMER0_Init ();                     // Initialize Timer0 for UART update

   UART0_Init ();                      // Enable UART using global settings

   IE_EA = 1;                             // Enable global interrupts

   while (1)
   {
      switch (LEDState) {
      case BLINK_SLOW:
         BlinkingFrequency = FAST_BLINKING;
         LEDState = BLINK_FAST;
         break;
      case BLINK_FAST:
         LEDState = OFF;
         LED = 0;
         break;
      case OFF:
         LEDState = ON;
         LED = 1;
         break;
      case ON:
         LEDState = BLINK_SLOW;
         BlinkingFrequency = SLOW_BLINKING;
         break;
      }

      update = 1;

      WaitForButtonPress();
   }
}

//-----------------------------------------------------------------------------
// WaitForButtonPress
//-----------------------------------------------------------------------------
//
// Waits for the button to be pressed and then released, with a built in
// debounce delay. If the switch toggles during the debounce delay, the
// delay is reset.
//
//-----------------------------------------------------------------------------
void WaitForButtonPress(void)
{
   uint8_t debouceDelay;
   while(SW);
   while(!SW) {
      for (debouceDelay = 0; debouceDelay < 0xFF; debouceDelay++) {
         if (SW == 0) {
            debouceDelay = 0;
         }
      }
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
   static char overflow_counter = 0;
   uint16_t local_blinking_frequency;

   // Timer 0 is used to control the LED on the daughter card.  When in
   // TIMER_MODE, a timer overflow counter gets incremented until
   // the terminal value (set in the foreground) is reached
   // When in BLINK_MODE, the interrupt checks the status of the switch
   // every overflow and turns the switch on or off depending on the
   // logic level of the switch.
   if (LEDState == BLINK_FAST || LEDState == BLINK_SLOW)
   {
      overflow_counter++;

      if (overflow_counter >= BlinkingFrequency)
      {
         Num_LED_Flashes++;            // Increment global counter
         LED = !LED;                   // Toggle LED state  
         overflow_counter = 0;         // Reset overflow counter
      }
   }

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
      local_blinking_frequency = SYSTEMCLOCK / (BlinkingFrequency * 65336 * 2);
      // Timer mode
      if ((LEDState == BLINK_FAST || LEDState == BLINK_SLOW) & update)
      {
         putchar ('\f');               // Clear screen

         Print_String ("LED Blink Rate: ");
         // Add a hundreds digit if <MAX_BLINK_RATE> requires one
         // Tens digit
         putchar ((local_blinking_frequency / 10) + 0x30);             
         // Ones digit
         putchar ((local_blinking_frequency % 10) + 0x30);             
         Print_String (" Hz\n");

         update = 0;
      }
      // PWM mode
      else if ((LEDState == OFF || LEDState == ON) & update)
      {
         putchar ('\f');               // Clear screen

         Print_String ("LED in push-button mode.  ");

         if (LEDState == OFF)
         {
            Print_String ("LED is OFF.");
         }
         else
         {
            Print_String ("LED is ON.");
         }

         update = 0;
      }

      overflows = 0;                   // Reset the count
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
// This function initializes the system clock to use the internal 12 MHz
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{

   OSCICN = 0x83;                      // Configure internal oscillator to
                                       // 12 MHz
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
// P0.4   digital   push-pull     UART TX
// P0.5   digital   open-drain    UART RX
// P0.6   digital   open-drain    /INT0
// P0.6   digital   open-drain    SW push button switch
// P1.3   digital   push-pull     LED (and CEX0 depending on Blink_State)
// P1.5   analog                  Potentiometer (ADC input)
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P0MDOUT |= 0x10;                    // Set P0.4 to push-pull output
   P2MDOUT |= 0x01;                    // Set P2.0 to push-pull output
   GPIOCN = 0x40;                      // Enable ports as input pins
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
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Baudrate generator, for <BAUDRATE1> and 8-N-1.
//-----------------------------------------------------------------------------

void UART0_Init (void)
{
   uint16_t baud;
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       // clear SCON0_RI and SCON0_TI bits

   // Baud Rate = [BRG Clock / (65536 - (SBRLH0:SBRLL0))] x 1/2 x 1/Prescaler

   if (SYSTEMCLOCK/BAUDRATE/2/0xFFFF < 1) {
      baud = -(SYSTEMCLOCK/BAUDRATE/2);
      SBCON0 |= 0x03;                  // Set prescaler to 1
   } else if (SYSTEMCLOCK/BAUDRATE/2/0xFFFF < 4) {
      baud = -(SYSTEMCLOCK/BAUDRATE/2/4);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x01;                  // Set prescaler to 4

   } else if (SYSTEMCLOCK/BAUDRATE/2/0xFFFF < 12) {
      baud = -(SYSTEMCLOCK/BAUDRATE/2/12);
      SBCON0 &= ~0x03;                 // Set prescaler to 12
   } else {
      baud = -(SYSTEMCLOCK/BAUDRATE/2/48);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x02;                  // Set prescaler to 4
   }

   //Set the baud rate generator reload values
   SBRLH0 = baud >> 8;
   SBRLL0 = baud & 0xFF;

   SBCON0 |= 0x40;                     // Enable baud rate generator

   SCON0_TI = 1;                            // Indicate TX0 ready
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

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
