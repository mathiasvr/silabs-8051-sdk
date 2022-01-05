//-----------------------------------------------------------------------------
// F36x_Comparator1_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This software shows the necessary configuration to use Comparator1 to detect
// when a switch is pressed and change the rate at which the LED is blinking.
// The code executes the initialization routines and then spins in an infinite
// while() loop, blinking the LED. If the button on P3.0 (on the target board)
// is pressed, then the comparators postive input (CP1+) input will drop
// below the comparator's negative input (CP1-). When this happens,
// Comparator1 will cause an interrupt to occur that will change the blinking
// rate of the LED. For this example, CP1+ is connected to the switch and
// CP1- is connected to VREF.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: P3.0 - SW3.0
//    J12: P3.2 - P3.2_LED
//    J4 : P2.0 - J5: P3.0
//    J4 : P2.1 - J2: P0.3
//    J15: P0.3 - VREF
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
// 5) Verify the LED is blinking. Press the P3.0 switch and then
//    verify that the LED is blinking at a different rate.
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
//    -Initial Revision (GV / GP / TP)
//    -23 OCT 2006


//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK               24500000  // Clock speed in Hz
#define COMPARATOR_SETTLE    10        // Settle time in us

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P3, 2);                  // Green LED

uint16_t LEDBlinkRate = 1000;               // Blink time in milliseconds

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);           // Configure the system clock
void Port_Init (void);                 // Configure the Crossbar and GPIO
void Comparator1_Init (void);          // Configure Comparator1
void VREF_Init (void);                 // Configure VREF

void Timer0_Delay_us (uint16_t us);
void Timer0_Delay_ms (uint16_t ms);

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
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{

   Oscillator_Init();                  // Initialize the system clock
   Port_Init ();                       // Initialize crossbar and GPIO
   Comparator1_Init();                 // Initialize Comparator1
   VREF_Init();                        // Initialize VREF

   LED = 0;

   IE_EA = 1;                          // Enable global interrupts

   SFRPAGE = LEGACY_PAGE;

   while(1)                            // Loop forever
   {
      LED = ~LED;

      Timer0_Delay_ms (LEDBlinkRate);
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
// This routine initializes the system clock to use the precision internal
// oscillator as its clock source.
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   OSCICN |= OSCICN_IFCN__HFOSC_DIV_1; // SYSCLK is internal osc.
                                       // running at 24.5 MHz

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
// Pinout:
//
// P0.3   analog    open-drain    VREF output
//
// P2.0   analog    push-pull     Comparator1 + Input
// P2.1   analog    open-drain    Comparator1 - Input
//
// P3.0   digital   open-drain    Switch
//
// P3.2   digital   push-pull     LED
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   P0SKIP = 0x08;                      // Skip P0.3 (VREF)
   P2SKIP = 0x03;                      // Skip P2.0 (CP1+) and P2.1 (CP1-)

   XBR1 = XBR1_XBARE__ENABLED;         // Enable crossbar and weak pull-ups

   P0MDIN &= ~0x08;                    // VREF in analog mode
   P2MDIN &= ~0x03;                    // Comparator1 inputs (P2.0 and P2.1)
                                       // are analog inputs.
   P3MDOUT = 0x04;                     // LED is push-pull output

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Comparator1_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the comparator to pins P2.0 and P2.1 and enables
// the Comparator1 interrupt.
//-----------------------------------------------------------------------------
void Comparator1_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   CPT1CN = 0x0F;                      // Positive and Negative Hysterisis =
                                       // 20 mV
   CPT1CN |= 0x80;                     // Comparator enabled
   CPT1MX = 0x00;                      // P2.1 = Negative Input
                                       // P2.0 = Positive Input

   Timer0_Delay_us (COMPARATOR_SETTLE); // Allow CP0 output to settle for 10 us

   CPT1MD = 0x10;                      // Enable Comparator1 falling-edge
                                       // interrupt
   EIE1 |= EIE1_ECP1__ENABLED;         // Enable the Comparator1 interrupt

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Comparator1_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function enables the VREF output on P0.3.
//-----------------------------------------------------------------------------
void VREF_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   REF0CN = REF0CN_BIASE__ENABLED |
            REF0CN_REFBE__ENABLED;     // Enable internal VREF generator

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Comparator1 Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// This ISR will trigger on a Comparator1 falling edge, or whenever the switch
// on P3.0 is pressed.
//
SI_INTERRUPT(Comparator1_ISR, CMP1_IRQn)
{
   EIE1 &= ~EIE1_ECP1__BMASK;          // Disable the Comparator1 interrupt
                                       // to effectively "debounce" the switch

   if ((CPT1CN & 0x40) == 0x00)        // If the output of CP1 is 0
   {                                   // (CP1+ < CP1-)
      // Vary the speed at which the LED blinks every time the switch is
      // pressed
      if (LEDBlinkRate == 1000)
      {
         LEDBlinkRate = 100;           // 100 ms = 10 Hz
      }
      else
      {
         LEDBlinkRate = 1000;          // 1000 ms = 1 Hz
      }
   }

   CPT1CN &= ~0x10;                    // Clear the falling-edge interrupt
                                       // flag

   EIE1 |= EIE1_ECP1__ENABLED;         // Re-enable the Comparator1 interrupt
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_Delay_us
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) unsigned int us - number of microseconds of delay
//                        range is full range of integer: 0 to 65335
//
// Configure Timer0 to delay <us> microseconds before returning.
//-----------------------------------------------------------------------------
void Timer0_Delay_us (uint16_t us)
{
   uint16_t i;                              // Microsecond counter

   TCON  &= ~0x30;                     // STOP Timer0 and clear overflow flag
   TMOD  &= ~0x0f;                     // Configure Timer0 to 16-bit mode
   TMOD  |=  TMOD_T0M__MODE1;
   CKCON |=  0x04;                     // Timer0 counts SYSCLKs

   for (i = 0; i < us; i++)            // Count microseconds
   {
      TCON &= ~TCON_TR0__BMASK;        // STOP Timer0
      TH0 = (-SYSCLK/1000000) >> 8;    // Set Timer0 to overflow in 1us
      TL0 = -SYSCLK/1000000;
      TCON |= TCON_TR0__RUN;           // START Timer0
      while (TCON_TF0 == 0);           // Wait for overflow
      TCON &= ~TCON_TF0__BMASK;        // Clear overflow indicator
   }
}

//-----------------------------------------------------------------------------
// Timer0_Delay_ms
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) unsigned int ms - number of milliseconds of delay
//                        range is full range of integer: 0 to 65335
//
// Configure Timer0 to delay <ms> milliseconds before returning.
//
// Note: This routine modifies the SCA0-1 bits in CKCON, which will affect
// the timing of Timer1.
//-----------------------------------------------------------------------------
void Timer0_Delay_ms (uint16_t ms)
{
   uint16_t i;                              // Millisecond counter

   TCON  &= ~0x30;                     // STOP Timer0 and clear overflow flag
   TMOD  &= ~0x0F;                     // Configure Timer0 to 16-bit mode
   TMOD  |=  TMOD_T0M__MODE1;

   CKCON &=  ~0x04;                    // Timer0 counts SYSCLK / 48
   CKCON |= 0x02;

   for (i = 0; i < ms; i++)            // Count milliseconds
   {
      TCON &= ~TCON_TR0__BMASK;        // STOP Timer0
      TH0 = (-SYSCLK/48/1000) >> 8;    // Set Timer0 to overflow in 1ms
      TL0 = -SYSCLK/48/1000;
      TCON |= TCON_TR0__RUN;           // START Timer0
      while (TCON_TF0 == 0);           // Wait for overflow
      TCON &= ~TCON_TF0__BMASK;        // Clear overflow indicator
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------