//=============================================================================
// F96x_PC0_Dual_Form_A_main.c
//=============================================================================
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example code shows the usage of the F96x's low-power Pulse Counter
// module to count pulses in dual counter mode.
//
// When SW1 is connected to PC0 (P0.0 to P1.0 on the LCD board, or P3.0 to
// P0.0 on the EMIF board) and SW2 is connected to PC1 (P0.1 to P1.1 on the
// LCD board, or P3.1 to P0.1 on the EMIF board), each button press and release
// will increment its respective pulse counter. When a pulse counter reaches
// its respected comparator value (either <PC0_COMPARATOR_PULSE_COUNTS> or
// <PC0_COMPARATOR_PULSE_COUNTS>), a comparator flag will be set, causing
// either LED3 or LED4 to be turned on.
//
// In place of SW1/SW2, a Form A type reed switch can be used (see section 25.2
// of the datasheet). This form of switch only connects to GND when closed, and
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
// 3) If using the LCD board, connect P0.3 <-> LED4/SW4 on J16.
// 4) Make the following connection to connect SW1/SW2 to PC0/PC1:
//       P0.0 <-> P1.0 (LCD board)
//       P0.1 <-> P1.1 (LCD board)
//       P3.0 <-> P1.0 (EMIF board)
//       P3.1 <-> P1.1 (EMIF board)
//    Alternatively, pulse sources can be connected directly to P1.0 and P1.1.
// 5) Move the VBAT SEL switch (SW5) to the middle VREG position.
// 6) Move the VIO switch (SW7) and VIORF switch (SW12) to the VBAT position.
// 7) Connect a 9V power adapter to P1 of the development board.
// 8) Modify the following constant to change how many pulses will be
//    required to cause a Pulse Counter comparator event: <PULSE_COUNTS>
// 9) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 10) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//11) If using the switches:
//       Press SW1 <PC0_COMPARATOR_PULSE_COUNTS> times, LED3 should turn on.
//       Press SW2 <PC1_COMPARATOR_PULSE_COUNTS> times, LED4 should turn on.
//    Else, if using pulse sources on PC0 and PC1, LED3 should turn on after
//    <PC0_COMPARATOR_PULSE_COUNTS> pulses have been generated on PC0. LED4
//    should turn on after <PC1_COMPARATOR_PULSE_COUNTS> pulses have been
//    generated on PC1.
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Updated Description / How to Test
//    - Added toggling of LED3/4 to indicate success.
//    - 27 JAN 2014
//
//=============================================================================
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
SI_SBIT(LED3, SFR_P0, 2);
SI_SBIT(LED4, SFR_P0, 3);
#endif

#ifdef UDP_F960_MCU_EMIF
SI_SBIT(LED3, SFR_P3, 2);
SI_SBIT(LED4, SFR_P3, 3);
#endif

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
//Number of pulses required to cause a comparator event on PC0 comparator 0
#define PC0_COMPARATOR_PULSE_COUNTS   4

//Number of pulses required to cause a comparator event on PC0 comparator 1
#define PC1_COMPARATOR_PULSE_COUNTS   4

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
bit PC0_Comparator1Flag;
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
   uint32_t current;
   uint32_t count0;
   uint32_t count1;

   SFRPAGE = LEGACY_PAGE;

   RTC_Init();
   PortInit();

   LED3 = LED_OFF;
   LED4 = LED_OFF;

   PC0_Init (PC0_DUAL_MODE, PC0_SAMPLING_RATE_2MS, 0);

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

   PC0_WriteComparator0 (PC0_COMPARATOR_PULSE_COUNTS);
   PC0_WriteComparator1 (PC1_COMPARATOR_PULSE_COUNTS);

   // Enable comparator and overflow interrupts
   SFRPAGE = DPPE_PAGE;
   PC0INT0 = PC0_CMP0EN| PC0_CMP1EN|PC0_OVRF;

   // Clear software flags
   PC0_Comparator0Flag = 0;
   PC0_Comparator1Flag = 0;

   EIE2 |= 0x10;                       // Enable PC0 interrupt
   IE_EA = 1;                             // Enable global interrupts

   while ((PC0_Comparator0Flag==0)&&(PC0_Comparator1Flag==0))
   {
      count0 = PC0_ReadCounter0 ();
      count1 = PC0_ReadCounter1 ();
   }

   while(1);
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
      LED3 = LED_ON;
   }
   else if((source&PC0_CMP1F)==PC0_CMP1F)
   {
      PC0INT0 = source & ~PC0_CMP1F;   // clear flag
      // Set software flag.
      PC0_Comparator1Flag = 1;
      // Do something on comparator event.
      LED4 = LED_ON;
   }
   else if((source&PC0_OVRF)==PC0_OVRF)
   {
      PC0INT0 = source & ~PC0_OVRF;   // clear flag
      // Do something on overflow event.
   }
}
//-----------------------------------------------------------------------------
// PortInit
//-----------------------------------------------------------------------------
void PortInit (void)
{
   SFRPAGE = LEGACY_PAGE;
   P1MDIN = ~0x03;                     // enable PC0 & PC1 as analog input

   // The crossbar does not have to be enabled to use the Pulse Counter
   XBR2 = 0x40;

}

//=============================================================================
