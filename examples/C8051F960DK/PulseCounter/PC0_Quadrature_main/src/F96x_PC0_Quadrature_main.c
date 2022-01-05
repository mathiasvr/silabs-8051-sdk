//=============================================================================
// F96x_PC0_Quadature_main.c
//=============================================================================
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example code shows the usage of the Pulse Counter module in Quadrature
// mode. This mode accepts pulses that are 90 degrees out of phase to determine
// either clockwise or counter-clockwise rotation.
//
// If PC0 (P1.0) is Low, and PC1 transitions from Low to High, or, if PC1
// (P1.1) is High, and PC0 transitions from Low to High, the module will
// increment Pulse Counter 0. This indicates that the signals represent
// clockwise rotation. If three clockwise pulses are counted, LED3 will
// turn on.
//
//    Clockwise
// ___     ___     ___     _
//|   |___|   |___|   |___|      PC1 (P1.1)
//   ___     ___     ___
//__|   |___|   |___|   |___     PC0 (P1.0)
//
//
// If PC0 (P1.0) is High, and PC1 transitions from Low to High, or, if PC1
// (P1.1) is Low, and PC0 transitions from Low to High, the module will
// increment Pulse Counter 1. This indicates that the signals represent
// counter-clockwise rotation. If three counter-clockwise pulses are counted,
// LED4 will turn on.
//
//    Counter-Clockwise
//   ___     ___     ___
//__|   |___|   |___|   |___     PC1 (P1.1)
// ___     ___     ___     _
//|   |___|   |___|   |___|      PC0 (P1.0)
//
//
// In this example, SW1 and SW2 or external pulse sources can be connected to 
// PC1 and PC2 and used to detect clockwise/counter-clockwise rotation.
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
// 11) If using the switches:
//       a) Press and hold SW1.
//       b) Press and hold SW2.
//       c) Release SW1.
//       d) Release SW2.
//       e) LED4 should turn on, indicating counter-clockwise rotation.
//       f) Pressing and releasing the switches in the opposite order (starting
//          with SW2) should cause LED4 to turn on, indicating clockwise
//          rotation.
//    Else, if using pulse-sources, LED3 and LED4 should light when the signals
//    indicate clockwise/counter-clockwise rotation, respectively.
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Updated Description / How to Test
//    - Added toggling of LED3/4 to indicate success.
//    - 27 JAN 2014
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
bit PC0_ClockwiseFlag;
bit PC0_CounterClockwiseFlag;
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

   PC0_Init (PC0_QUADRATURE_MODE, PC0_SAMPLING_RATE_2MS, 0);

   // set debounce time to 4 ms
   value = PC0_SetDebounceTimeHigh (4000);
   value = PC0_SetDebounceTimeLow (4000);

   // Use auto calibrate for Quadrature switch pull-up current
   error = PC0_AutoCalibrate (PC0_CAL_PC0);

   if(error)
   {
      // On error, set current to 1 uA
      current = PC0_SetPullUpCurrent (1000);
   }

   PC0_WriteComparator0 (0x0003);
   PC0_WriteComparator1 (0x0003);

   // Enable comparator and overflow interrupts
   SFRPAGE = DPPE_PAGE;
   PC0INT0 = PC0_CMP0EN| PC0_CMP1EN|PC0_OVRF;
   // Clear Quadrature Error
   PC0INT1 = PC0_ERROREN;

   // Clear software flags
   PC0_ClockwiseFlag = 0;
   PC0_CounterClockwiseFlag = 0;

   EIE2 |= 0x10;                       // enable PC0 interrupt
   IE_EA = 1;

   while(1)
   {
      count0 = PC0_ReadCounter0 ();
      count1 = PC0_ReadCounter1 ();

      if (PC0_ClockwiseFlag == 1)
      {
         LED3 = LED_ON;
         LED4 = LED_OFF;
      }

      if (PC0_CounterClockwiseFlag == 1)
      {
         LED4 = LED_ON;
         LED3 = LED_OFF;
      }
   }

}
//-----------------------------------------------------------------------------
// PC0 ISR
//-----------------------------------------------------------------------------
SI_INTERRUPT(PC0_ISR, PC0_IRQn)
{
   uint8_t source;
   static bit quadready = 0;

   SFRPAGE = DPPE_PAGE;

   source = PC0_ReadPC0INT0();         // read using accessor function

   if((source&PC0_CMP0F)==PC0_CMP0F)
   {
      PC0INT0 = source & ~PC0_CMP0F;   // clear flag
      // Set software flag.
      PC0_ClockwiseFlag = 1;
      PC0_CounterClockwiseFlag = 0;
      // Do something on comparator event.
      PC0_WriteComparator0(PC0_ReadCounter0() + 3);
      PC0_WriteComparator1(PC0_ReadCounter1() + 3);
   }
   else if((source&PC0_CMP1F)==PC0_CMP1F)
   {
      PC0INT0 = source & ~PC0_CMP1F;   // clear flag
      // Set software flag.
      PC0_CounterClockwiseFlag = 1;
      PC0_ClockwiseFlag = 0;
      // Do something on comparator event.
      PC0_WriteComparator1(PC0_ReadCounter1() + 3);
      PC0_WriteComparator0(PC0_ReadCounter0() + 3);
   }
   else if((source&PC0_OVRF)==PC0_OVRF)
   {
      PC0INT0 = source & ~PC0_OVRF;   // clear flag
      // Do something on overflow event.
   }

   source = PC0_ReadPC0INT1();         // read using accessor function

   if((source&PC0_ERRORF)==PC0_ERRORF)
   {
      PC0INT1 = source & ~PC0_ERRORF;  // clear flag
      if(quadready == 0)
      {
         // Ignore initial error on POR.
         quadready = 1;
      }
      else
      {
         // Do something on quadrature error event.
      }
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
