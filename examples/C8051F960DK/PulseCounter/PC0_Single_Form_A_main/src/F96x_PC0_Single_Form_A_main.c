//=============================================================================
// F96x_PC0_Single_Form_A_main.c
//=============================================================================
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example code shows the usage of the F96x's low-power Pulse Counter
// module to count pulses in single counter mode with a Form A type
// reed-switch.
//
// When SW1 is connected to PC0 (P0.0 to P1.0 on the LCD board, or P3.0 to
// P0.0 on the EMIF board), each button press and release will increment
// Pulse Counter 0. When the Pulse Counter 0 equals the value set in
// PC0 Comparator 0 (set by the constant <PULSE_COUNTS>) the PC0
// comparator flag will be set, causing LED2 to toggle (P0.1 on LCD,
// P3.1 on EMIF). After <PULSE_COUNTS> more pulses, LED2 will toggle again.
//
// In place of SW1, a Form A type reed switch can be used (see section 25.2 of
// the datasheet). This form of switch only connects to GND when closed, and
// requires a pullup (supplied internally by the MCU) to function.
//
//   Form A reed switch. With the switch closed, a connection to GND is made.
//   When the switch is open, no connection is made. A pull-up is required.
//          
//   ___/ ____x
//  |         
// GND
//
// How To Test:
//
// 1) In "Define Hardware", #define either "UDP_F960_MCU_MUX_LCD" or
//    "UDP_F960_MCU_EMIF" depending on which board you are using.
// 2) Connect the USB Debug Adapter to J13.
// 3) Make the following connection to connect SW1 to Pulse Counter 1:
//       P0.0 <-> P1.0 (LCD board)
//       P3.0 <-> P1.0 (EMIF board)
//    Alternatively, a pulse source, such as a Form A reed switch can be
//    connected directly to P1.0.
// 4) Move the VBAT SEL switch (SW5) to the middle VREG position.
// 5) Move the VIO switch (SW7) and VIORF switch (SW12) to the VBAT position.
// 6) Connect a 9V power adapter to P1 of the development board.
// 7) Modify the following constant to change how many pulses will be
//    required to cause a Pulse Counter comparator event: <PULSE_COUNTS>
// 7) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) If using SW1 to generate pulses, press SW1 a number of times as indicated
//    by the constant <PULSE_COUNTS>.
//    Alternatively, wait until the pulse generator connected to P1.0 generates
//    a number of pulses as indicated by the constant <PULSE_COUNTS>.
//    LED2 should toggle after every <PULSE_COUNTS> number of pulses.
//
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Updated Description / How to Test
//    - Added toggling LED2 after every PULSE_COUNTS on PC0 to indicate success
//    - 24 JAN 2014
//
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F960_Register_Enums.h>
#include "PulseCounter.h"
#include "SmaRTClockF960.h"

//-----------------------------------------------------------------------------
// Define Hardware
//-----------------------------------------------------------------------------
#define UDP_F960_MCU_MUX_LCD
//#define UDP_F960_MCU_EMIF
//-----------------------------------------------------------------------------
// Hardware Dependent definitions
//-----------------------------------------------------------------------------
#ifdef UDP_F960_MCU_MUX_LCD
SI_SBIT(LED2, SFR_P0, 1);
#endif

#ifdef UDP_F960_MCU_EMIF
SI_SBIT(LED2, SFR_P3, 1);
#endif

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
#define PULSE_COUNTS       4     //Number of button presses required to
                                 //trigger a Pulse Counter comparator event
                                 // This can be a 24 bit value.
#define LED_ON             0
#define LED_OFF            1

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
   PCA0MD  &= ~0x40;                   // disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
bit PC0_Comparator0Flag;
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void PortInit (void);
//-----------------------------------------------------------------------------
// Main Prototypes
//-----------------------------------------------------------------------------
void main (void)
{
   uint8_t value;
   uint8_t error;
   uint32_t count;
   uint32_t current;

   SFRPAGE = LEGACY_PAGE;

   RTC_Init();
   PortInit();

   LED2 = LED_OFF;

   PC0_Init (PC0_SINGLE_MODE, PC0_SAMPLING_RATE_2MS, 0);

   // set debounce time to 4 ms
   value = PC0_SetDebounceTimeHigh (4000);
   value = PC0_SetDebounceTimeLow (4000);

   // Use auto calibrate for Form A switch pull-up current
   error = PC0_AutoCalibrate (PC0_CAL_PC0);

   if(error)
   {
      // On error, set current to 1 uA
      current = PC0_SetPullUpCurrent (1000);
   }

   current = PC0_ReadPullUpCurrent ();

   PC0_WriteComparator0 (PULSE_COUNTS);

   // Enable comparator and overflow interrupts
   SFRPAGE = DPPE_PAGE;
   PC0INT0 = PC0_CMP0EN|PC0_OVRF;

   // Clear software flags
   PC0_Comparator0Flag = 0;

   EIE2 |= 0x10;                          // Enable PC0 interrupt
   IE_EA = 1;                             // Enable global interrupts

   while (1)
   {

      while (PC0_Comparator0Flag==0);     // Wait for a comparator interrupt
      PC0_Comparator0Flag = 0;            // Clear the software flag

      count = PC0_ReadCounter0 ();        // Read the current counter

      PC0_WriteComparator0 (count + PULSE_COUNTS);    //Set the comparator to
                                                      //fire after PULSE_COUNTS
                                                      //more pulses.

      LED2 = !LED2;                       //Toggle the LED
   }
}
//-----------------------------------------------------------------------------
// PC0 ISR
//-----------------------------------------------------------------------------
SI_INTERRUPT(PC0_ISR, PC0_IRQn)
{
   uint8_t source;

   //EIE2 &= ~0x10;                       // disable further interrupts

   SFRPAGE = DPPE_PAGE;

   source = PC0_ReadPC0INT0();         // read using accessor function

   if((source&PC0_CMP0F)==PC0_CMP0F)
   {
      PC0INT0 = source & ~PC0_CMP0F;   // clear flag
      // Set software flag.
      PC0_Comparator0Flag = 1;
      // Do something on comparator event.
   }
   else if((source&PC0_OVRF)==PC0_OVRF)
   {
      PC0INT0 = source & ~PC0_CMP1F;   // clear flag
      // Do something on overflow event.
   }
}
//-----------------------------------------------------------------------------
// PortInit
//-----------------------------------------------------------------------------
void PortInit (void)
{
   SFRPAGE = LEGACY_PAGE;
   P1MDIN = ~0x01;                     // enable PC0 as analog input

   // The crossbar does not have to be enabled to use the Pulse Counter
   // but is required for the LED to function.
   XBR2 = 0x40;

}

//=============================================================================
