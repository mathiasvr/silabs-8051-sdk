//-----------------------------------------------------------------------------
// F380_USB0Interrupt.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Stub file for Firmware Template.
//
//
// How To Test:    See Readme.txt
//
//
// Target:         C8051F38x
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.6
// Command Line:   See Readme.txt
// Project Name:   USB0_Interrupt
//
// Release 2.0
//    -Rewrite (CM)
//    -02 NOV 2012
//
// Release 1.1
//    -All changes by BW
//    -1 SEP 2010
//    -Updated USB0_Init() to write 0x89 to CLKREC instead of 0x80
// Release 1.0
//    -Initial Revision (PD)
//    -04 JUN 2008
//

//-----------------------------------------------------------------------------
// Header Files
//-----------------------------------------------------------------------------

#include "c8051f3xx.h"
#include "F3xx_USB0_InterruptServiceRoutine.h"
#include "F3xx_USB0_Register.h"
#include "F3xx_USB0_Main.h"

#if TARGET_MCU != MCU_F380
#error Invalid TARGET_MCU definition
#endif // TARGET_MCU != MCU_F380

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------

#define SYSCLK             24000000    // SYSCLK frequency in Hz

// USB clock selections (SFR CLKSEL)
#define USB_4X_CLOCK       0x00        // Select 4x clock multiplier, for USB
#define USB_INT_OSC_DIV_2  0x10        // Full Speed
#define USB_EXT_OSC        0x20
#define USB_EXT_OSC_DIV_2  0x30
#define USB_EXT_OSC_DIV_3  0x40
#define USB_EXT_OSC_DIV_4  0x50

// System clock selections (SFR CLKSEL)
#define SYS_INT_OSC        0x00        // Select to use internal oscillator
#define SYS_EXT_OSC        0x01        // Select to use an external oscillator
#define SYS_4X_DIV_2       0x02

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

// Initialization Routines
void Sysclk_Init (void);               // Initialize the system clock
void Port_Init (void);                 // Configure ports
void Timer2_Init (void);               // Configure Timer2
void ADC0_Init (void);                 // Configure ADC0
void USB0_Init (void);                 // Configure USB core
void USB0_Suspend (void);              // Suspend System
void Delay (void);                     // Approximately 80 us/1 ms on
                                       // Full/Low Speed

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

SI_SBIT(Led1, SFR_P2, 2);                // LED='1' means ON
SI_SBIT(Led2, SFR_P2, 3);

SI_SBIT(Sw1, SFR_P2, 0);                 // SW='0' means pressed
SI_SBIT(Sw2, SFR_P2, 1);

// This constant is added to Temperature
SI_SEGMENT_VARIABLE(TEMP_ADD, const int16_t, SI_SEG_CODE) = -231;

//-----------------------------------------------------------------------------
// External Global Variables
//-----------------------------------------------------------------------------

uint8_t Switch1State = 0;                   // Indicate status of switch
uint8_t Switch2State = 0;                   // starting at 0 == unpressed

uint8_t Potentiometer = 0x00;               // Last read potentiometer value
uint8_t Temperature = 0x00;                 // Last read temperature sensor value

//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// System_Init
//-----------------------------------------------------------------------------
// This function is declared in the header file F3xx_HIDtoUART.h and is
// called in the main(void) function.  It calls initialization routines
// local to this file.
//
//-----------------------------------------------------------------------------
void System_Init (void)
{
   PCA0MD &= ~0x40;                    // Disable watchdog timer

   Sysclk_Init ();                     // Initialize oscillator
   Port_Init ();                       // Initialize crossbar and GPIO
   Timer2_Init ();                     // Initialize Timer2
   ADC0_Init ();                       // Initialize ADC0
}

//-----------------------------------------------------------------------------
// USB0_Init
//-----------------------------------------------------------------------------
// USB Initialization performs the following:
// - Initialize USB0
// - Enable USB0 interrupts
// - Enable USB0 transceiver
// - Enable USB0 with suspend detection
//
//-----------------------------------------------------------------------------
void USB0_Init (void)
{
   POLL_WRITE_BYTE (POWER,  0x08);     // Force Asynchronous USB Reset
   POLL_WRITE_BYTE (IN1IE,  0x03);     // Enable Endpoint 0-1 in interrupts
   POLL_WRITE_BYTE (OUT1IE, 0x02);     // Enable Endpoint 1 out interrupts
   POLL_WRITE_BYTE (CMIE,   0x07);     // Enable Reset, Resume, and Suspend
                                       // interrupts

#ifdef _USB_LOW_SPEED_
   USB0XCN = 0xC0;                     // Enable transceiver; select low speed
   POLL_WRITE_BYTE (CLKREC, 0xA9);     // Enable clock recovery, single-step
                                       // mode disabled
#else
   USB0XCN = 0xE0;                     // Enable transceiver; select full speed
   POLL_WRITE_BYTE (CLKREC, 0x89);     // Enable clock recovery, single-step
                                       // mode disabled
#endif // _USB_LOW_SPEED_

   EIE1 |= 0x02;                       // Enable USB0 Interrupts

                                       // Enable USB0 by clearing the USB
   POLL_WRITE_BYTE (POWER, 0x01);      // Inhibit Bit and enable suspend
                                       // detection
}

//-----------------------------------------------------------------------------
// USB0_Suspend
//-----------------------------------------------------------------------------
// Enter suspend mode after suspend signalling is present on the bus
//
void USB0_Suspend(void)
{
   uint8_t ADC0CN_Val = ADC0CN;    // Store the current value of ADC0CN
   uint8_t REF0CN_Val = REF0CN;    // Store the current value of REF0CN

   // Power Down

   ADC0CN = 0x00;             // Disable ADC0
   IE_EA = 0;                    // Disable global interrupts
   REF0CN = 0x00;             // Disable voltage references
   XBR1 &= ~0x40;             // Disengage the crossbar

   USB0XCN &= ~0x40;          // Suspend USB Transceiver
   CLKSEL = 0x10;             // USB Clock now uses int osc / 2
   CLKMUL = 0x00;             // Disable Clock Multiplier
   REG01CN |= 0x10;           // Place voltage regulator in low power mode
   OSCICN |= 0x20;            // Force internal oscillator suspend

   NOP ();

   // Power Up
   // Code execution begins here when resume signaling is received.

   REG01CN &= ~0x10;          // Place voltage regulator in full power mode
   Sysclk_Init ();
   USB0XCN |= 0x40;           // Enable USB Transceiver

   XBR1 |= 0x40;              // Engage the crossbar
   ADC0CN = ADC0CN_Val;       // Place ADC0 in its previous state
   REF0CN = REF0CN_Val;       // Place REF0CN in its previous state
   IE_EA = 1;
}

//-----------------------------------------------------------------------------
// Sysclk_Init
//-----------------------------------------------------------------------------
// This function initializes the system clock and the USB clock.
// Low-speed System Clock:  12 MHz  USB Clock: 12 MHz
// Full-speed System Clock: 24 MHz  USB Clock: 48 MHz
//
//-----------------------------------------------------------------------------
void Sysclk_Init (void)
{
#ifdef _USB_LOW_SPEED_

   OSCICN |= 0x03;                     // Configure internal oscillator for
                                       // its maximum frequency and enable
                                       // missing clock detector

   CLKSEL  = SYS_INT_OSC;              // Select System clock
   CLKSEL |= USB_INT_OSC_DIV_2;        // Select USB clock
#else
   OSCICN |= 0x03;                     // Configure internal oscillator for
                                       // its maximum frequency and enable
                                       // missing clock detector

   CLKMUL  = 0x00;                     // Select internal oscillator as
                                       // input to clock multiplier

   CLKMUL |= 0x80;                     // Enable clock multiplier
   Delay();                            // Delay for clock multiplier to begin
   CLKMUL |= 0xC0;                     // Initialize the clock multiplier
   Delay();                            // Delay for clock multiplier to begin

   while(!(CLKMUL & 0x20));            // Wait for multiplier to lock
   CLKSEL  = 0x02;                     // Set sys clock to clkmul / 2
#endif  // _USB_LOW_SPEED_
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
// P0.0   digital   open-drain   GPIO Input
// P0.1   digital   open-drain   GPIO Input
// P0.2   digital   open-drain   GPIO Input
// P0.3   digital   open-drain   GPIO Input
//
// P0.4   digital   push-pull    GPIO Output
// P0.5   digital   push-pull    GPIO Output
// P0.6   digital   push-pull    GPIO Output
// P0.7   digital   push-pull    GPIO Output
//
// P2.0   digital   open-drain   SW1
// P2.1   digital   open-drain   SW2
//
// P2.2   digital   push-pull    LED1
// P2.3   digital   push-pull    LED2
//
// P2.5   analog                 Potentiometer
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P2MDIN    = 0xDF; // P2.5 is analog
   P0MDOUT   = 0xF0; // P0.4 - P0.7 is push-pull
   P2MDOUT   = 0x0C; // P2.2 - P2.3 is push-pull
   P0SKIP    = 0xFF; // P0 skipped
   P2SKIP    = 0x2F; // P2.0 - P2.3, P2.5 skipped
   XBR1      = 0x40; // Enable the crossbar
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
// 
// Timer 2 reload, used to check if switch pressed on overflow and
// used for ADC continuous conversion.
//
// Low-speed reload rate:  25 Hz
// Full-speed reload rate: 50 Hz
//
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;

   CKCON  &= ~0xF0;                    // Timer2 clocked based on TMR2CN_T2XCLK (SYSCLK/12)
   TMR2RL  = 0x10000 - 40000;          // Initialize reload value
   TMR2    = 0xffff;                   // Set to reload immediately

   IE_ET2     = 1;                        // Enable Timer2 interrupts
   TMR2CN_TR2     = 1;                        // Start Timer2
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
// 
// Configures ADC for single ended continuous conversion or Timer2
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   REF0CN  = 0x0E;                     // Enable voltage reference VREF
   AMX0P = 0x1E;                       // Positive input starts as temp sensor
   AMX0N = 0x1F;                       // Single ended mode(neginput = gnd)

   ADC0CF  = 0xF8;                     // SAR Period 0x1F, Right adjusted

   ADC0CN  = 0xC2;                     // Continuous converion on timer 2 
                                       // overflow; low power tracking mode on

   EIE1   |= 0x08;                     // Enable conversion complete interrupt
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// Called when timer 2 overflows, check to see if switch is pressed
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   // Sw1 pressed
   if (!Sw1)
   {
      Switch1State = 1;
   }
   // Unpressed
   else
   {
      Switch1State = 0;
   }

   // Sw2 pressed
   if (!Sw2)
   {
      Switch2State = 1;
   }
   // Unpressed
   else
   {
      Switch2State = 0;
   }

   // Clear Timer2 interrupt flag
   TMR2CN_TF2H = 0;
}

//-----------------------------------------------------------------------------
// ADC0_ConvComplete_ISR
//-----------------------------------------------------------------------------
//
// Called after a conversion of the ADC has finished
// Updates the appropriate variable for potentiometer or temperature sensor
// Switches the ADC multiplexor value to switch between the potentiometer 
// and temp sensor
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC0_ConvComplete_ISR, ADC0EOC_IRQn)
{
   int16_t temp;

   // This switches the AMUX between the temperature sensor and the
   // potentiometer pin after conversion
   if (AMX0P == 0x1E)
   {
      // Add offset to Temperature
      temp = (ADC0H << 8) + ADC0L;
      temp += TEMP_ADD;
      Temperature = (uint8_t)temp;

      // Switch to potentiometer
      AMX0P = 0x04;

      // Place ADC0 in left-adjusted mode
      ADC0CF = 0xFC;
   }
   else
   {
      Potentiometer = ADC0H;

      // Switch to temperature sensor
      AMX0P = 0x1E;

      // Place ADC0 in right-adjusted mode
      ADC0CF = 0xF8;
   }

   ADC0CN_ADINT = 0;
}

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Delay
//-----------------------------------------------------------------------------
// Used for a small pause, approximately 80 us in Full Speed,
// and 1 ms when clock is configured for Low Speed
//
//-----------------------------------------------------------------------------
void Delay (void)
{
   int x;
   for (x = 0; x < 500; x)
      x++;
}

//-----------------------------------------------------------------------------
// SetLed
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//                1) uint8_t led: specified which LED to set (1 or 2)
//                2) uint8_t on: 0 means off, 1 means on
//
// Set the state of the specified LED
//
//-----------------------------------------------------------------------------
void SetLed (uint8_t led, uint8_t on)
{
   if (led == 1)
   {
      if (on)
         Led1 = 1;
      else
         Led1 = 0;
   }
   else if (led == 2)
   {
      if (on)
         Led2 = 1;
      else
         Led2 = 0;
   }
}
