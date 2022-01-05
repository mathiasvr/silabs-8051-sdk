//-----------------------------------------------------------------------------
// F93x_CapTouchSense_Switch.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program lights up the yellow LED on the C8051F93x target board when
// the capacitive touch sense (contactless) switch (P2.0) is touched. It
// also lights up the red LED when the (contactless) switch (P2.1) is touched.
//
// A relaxation oscillator is implemented using an on-chip analog comparator
// and external resistors R15 through R19. The high-to-low transitions of the
// relaxation oscillator are counted by Timer0. The relaxation oscillator
// frequency depends on the capacitance of the touch sense trace capacitor.
// The count of Timer0 is periodically checked on every Timer2 overflow.
// Depending on the count, it can be determined whether SW3 is being touched
// or not.
//
//
// How To Test:
//
// Setup:
// 1) Ensure that jumpers are placed on the following:
//       J8:   P1.5/LED1.5
//             P1.6/LED1.6
//             P0.2/SW0.2
//             P0.3/SW0.3
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J9.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//
// One Time Calibration (stored in non-volatile flash):
// 1) Both LEDs will be lit. Place a finger on either Touch Sense Pad (P2.0
//    or P2.1), and  hold it there. Do not remove this finger till step 3.
// 2) Press and hold switch SW2 (P0.2) for if your finger is on Touch Sense
//    Pad (P2.0). Otherwise press and hold SW3 (P0.3) if your finger is on
//    Touch Sense Pad (P2.1).  Release the switch after 1 second.
// 3) The yellow LED will continue to be lit if your finger was touching the
//    P2.0 pad. The red LED will continue to be lit if your finger was touching
//    the P2.1 pad. Remove finger from the Touch Sense Pad. The LEDs should
//    switch off.
//
// Usage:
// 1) Touch SW20. The Yellow LED (P1.6) should light up in response.
// 2) Touch SW21. The Red LED (P1.5) should light up in response.
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - Initialized capsense pins as analog to avoid a comparator failure
//    - 15 JAN 2014
//
// Release 1.2
//    - Updated to be compatible with Raisonance Toolchain (FB)
//    - 14 APR 2010
//
// Release 1.1
//    - Compiled and tested for C8051F930-TB (JH)
//    - 06 JULY 2009
//
// Release 1.0
//    - Initial Revision (FB)
//    - 05 OCT 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F930_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(RED_LED,    SFR_P1, 5);          // '0' means ON, '1' means OFF
SI_SBIT(YELLOW_LED, SFR_P1, 6);          // '0' means ON, '1' means OFF
SI_SBIT(SW2,        SFR_P0, 2);          // SW2 == 0 means switch pressed
SI_SBIT(SW3,        SFR_P0, 3);          // SW3 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK      24500000           // SYSCLK frequency in Hz
#define BAUDRATE      230400           // Baud rate of UART in bps

#define LED_ON              0          // Macros to turn LED on and off
#define LED_OFF             1

#define SW21_SENSITIVITY   250         // Sensitivity value used for both
                                       // Touch Sense switches. Larger values
                                       // make sthe switches more sensitive

#define CAL_ADDRESS         0          // Address in the scratchpad to store
                                       // the calibrated switch value

#define SCRATCHPAD          1          // Passed to the Flash access routines
                                       // to indicate that the calibration
                                       // constant is located in the scratchpad


//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
// Timer2 count of SW20 relaxation oscillator transitions
SI_UU16_t SW20_Timer_Count;
SI_UU16_t SW21_Timer_Count;

// Touch Switch: SW20==0 means finger sensed; SW20==1 means finger not sensed.
uint8_t SW20_Status;
uint8_t SW21_Status;

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void TouchSense_Init (void);

void Wait_MS(uint16_t ms);
void TouchSense_Update(void);
void Calibrate (void);
uint16_t Get_Calibration(void);

// FLASH read/write/erase routines
void FLASH_ByteWrite (uint16_t addr, uint8_t byte, uint8_t SFLE);
uint8_t   FLASH_ByteRead  (uint16_t addr, uint8_t SFLE);
void FLASH_PageErase (uint16_t addr, uint8_t SFLE);

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
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
	//Enter default mode
	enter_DefaultMode_from_RESET();
	SW20_Status = 1;                    // Set switch status to finger not
	SW21_Status = 1;                    // detected.

	if( Get_Calibration() == 0xFFFF)
	{
	  Calibrate ();
	}


	while(1)
	{
	  Wait_MS(25);                     // Polling Interval

	  if(!SW2 || !SW3)                 // If the P0.2 Switch is Pressed
	  {
		 Calibrate();                  // Calibrate the Touch Pads
	  }

	  TouchSense_Update();             // Update switch variables

	  YELLOW_LED = SW20_Status;        // Set LED states based on the
	  RED_LED = SW21_Status;           // switch variables
	}

	// NOTREACHED
	return 0;
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// TouchSense_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports.
//
//
//-----------------------------------------------------------------------------
void TouchSense_Update (void)
{

   SI_UU16_t timer_count_A, timer_count_B;

   // Prepare Timer2 for the first TouchSense reading
   TMR2CN &= ~0x80;                    // Clear overflow flag
   while(!(TMR2CN & 0x80));            // Wait for overflow
   timer_count_A.u16 = TMR2RL;         // Record value

   // Prepare Timer2 for the second TouchSense reading
   TMR2CN &= ~0x80;                    // Clear overflow flag
   while(!(TMR2CN & 0x80));            // Wait for overflow
   timer_count_B.u16 = TMR2RL;         // Record value

   // Calculate the oscillation period
   SW20_Timer_Count.u16 = timer_count_B.u16 - timer_count_A.u16;

   // Change the CP0 Multiplexer to select switch on P2.1
   CPT0MX = 0x8C;                      // Positive Mux: TouchSense Compare
                                       // Negative Mux: P2.1 - TouchSense Switch

   // Prepare Timer2 for the first TouchSense reading
   TMR2CN &= ~0x80;                    // Clear overflow flag
   while(!(TMR2CN & 0x80));            // Wait for overflow
   timer_count_A.u16 = TMR2RL;         // Record value

   // Prepare Timer2 for the second TouchSense reading
   TMR2CN &= ~0x80;                    // Clear overflow flag
   while(!(TMR2CN & 0x80));            // Wait for overflow
   timer_count_B.u16 = TMR2RL;         // Record value

   // Calculate the oscillation period
   SW21_Timer_Count.u16 = timer_count_B.u16 - timer_count_A.u16;

   // Change the CP0 Multiplexer to select switch on P2.0
   CPT0MX = 0xC8;                      // Positive Mux: P2.0 - TouchSense Switch
                                       // Negative Mux: TouchSense Compare

   // Update the status variable for SW20
   if(SW20_Timer_Count.u16 > (Get_Calibration() - SW21_SENSITIVITY))
   {
      SW20_Status = 0;

   } else
   {
      SW20_Status = 1;
   }
   // Update the status variable for SW21
   if(SW21_Timer_Count.u16 > (Get_Calibration() - SW21_SENSITIVITY))
   {
      SW21_Status = 0;

   } else
   {
      SW21_Status = 1;
   }
}


//-----------------------------------------------------------------------------
// Wait_MS
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters:
//   1) uint16_t ms - number of milliseconds of delay
//                        range is full range of integer: 0 to 65335
//
// This routine inserts a delay of <ms> milliseconds.
//-----------------------------------------------------------------------------
void Wait_MS(uint16_t ms)
{
   char i;

   TCON_TR0 = 0;                            // Stop Timer0

   TMOD &= ~0x0F;                      // Timer0 in 8-bit mode
   TMOD |= 0x02;

   CKCON &= ~0x03;                     // Timer0 uses a 1:48 prescaler
   CKCON |= 0x02;


   TH0 = -SYSCLK/48/10000;             // Set Timer0 Reload Value to
                                       // overflow at a rate of 10kHz

   TL0 = TH0;                          // Init Timer0 low byte to the
                                       // reload value

   TCON_TF0 = 0;                            // Clear Timer0 Interrupt Flag
   IE_ET0 = 0;                            // Timer0 interrupt disabled
   TCON_TR0 = 1;                            // Timer0 on

   while(ms--)
   {
      for(i = 0; i < 10; i++)
      {
         TCON_TF0 = 0;
         while(!TCON_TF0);
      }
   }

   TCON_TF0 = 0;

}


//-----------------------------------------------------------------------------
// Calibrate
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine will store the touch value in non-volatile Flash memory as
// the calibration threshold for the switch SW21.
//
// 1. Place a finger on the touch switch SW21, and hold it there.
// 2. Press switch SW2 (P0.7) for >1 sec and release using another finger.
// 3. Remove finger from SW21.
//-----------------------------------------------------------------------------
void Calibrate (void)
{
   uint8_t  EA_Save;
   uint8_t  switch_number;

   YELLOW_LED = LED_ON;
   RED_LED = LED_ON;

   while (SW2 && SW3);                 // Wait till any switch is pressed

   if(!SW2)                            // Decode which switch was pressed
   {
      switch_number = 0;
   } else
   {
      switch_number = 1;
   }

   while (!SW2 || !SW3);               // Wait till switches released

   EA_Save = IE;                       // Preserve IE_EA
   IE_EA = 0;                             // Disable interrupts

   TouchSense_Update();                // Update switch variables

   // Erase the scratchpad
   FLASH_PageErase (CAL_ADDRESS, SCRATCHPAD);


   if(switch_number == 0)
   {
      // Write the expected switch value to the scratchpad
      FLASH_ByteWrite ((uint16_t)(CAL_ADDRESS+MSB), SW20_Timer_Count.u8[MSB],
                       SCRATCHPAD);
      FLASH_ByteWrite ((uint16_t)(CAL_ADDRESS+LSB), SW20_Timer_Count.u8[LSB],
                       SCRATCHPAD);

   } else
   {

      // Write the expected switch value to the scratchpad
      FLASH_ByteWrite ((uint16_t)(CAL_ADDRESS+MSB), SW21_Timer_Count.u8[MSB],
                       SCRATCHPAD);
      FLASH_ByteWrite ((uint16_t)(CAL_ADDRESS+LSB), SW21_Timer_Count.u8[LSB],
                       SCRATCHPAD);
   }

   if ((EA_Save & 0x80) != 0)          // Restore IE_EA
   {
      IE_EA = 1;
   }
}

//-----------------------------------------------------------------------------
// Get_Calibration
//-----------------------------------------------------------------------------
//
// Return Value :
//   1) uint16_t   -  calibration value
//                        range is full range of integer: 0 to 65335
// Parameters: None
//
// This routine reads the calibration value from Flash
//-----------------------------------------------------------------------------
uint16_t Get_Calibration(void)
{
   SI_UU16_t cal_value;

   cal_value.u8[MSB] = FLASH_ByteRead ((uint16_t)(CAL_ADDRESS+MSB),
                                         SCRATCHPAD);
   cal_value.u8[LSB] = FLASH_ByteRead ((uint16_t)(CAL_ADDRESS+LSB),
                                         SCRATCHPAD);

   return cal_value.u16;

}


//-----------------------------------------------------------------------------
// Flash Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FLASH_ByteWrite
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - address of the byte to write to
//                    valid range is from 0x0000 to 0xFBFF for 64K devices
//                    valid range is from 0x0000 to 0x7FFF for 32K devices
//   2) char byte - byte to write to Flash.
//
// This routine writes <byte> to the linear FLASH address <addr>.
//-----------------------------------------------------------------------------
void FLASH_ByteWrite (uint16_t addr, uint8_t byte, uint8_t SFLE)
{
   uint8_t EA_Save = IE;                    // Preserve IE_EA
   SI_SEGMENT_VARIABLE_SEGMENT_POINTER(pwrite, uint8_t, xdata, data);
   //unsigned char xdata * data pwrite;// FLASH write pointer

   IE_EA = 0;                             // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor


   RSTSRC = 0x06;                      // Enable VDD monitor as a reset source
                                       // Leave missing clock detector enabled

   pwrite = (char xdata *) addr;

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x01;                      // PSWE = 1

   if(SFLE)
   {
      PSCTL |= 0x04;                   // Access Scratchpad
   }

   VDM0CN = 0x80;                      // Enable VDD monitor


   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   *pwrite = byte;                     // Write the byte

   PSCTL &= ~0x05;                     // SFLE = 0; PSWE = 0

   if ((EA_Save & 0x80) != 0)          // Restore IE_EA
   {
      IE_EA = 1;
   }

}

//-----------------------------------------------------------------------------
// FLASH_ByteRead
//-----------------------------------------------------------------------------
//
// Return Value :
//      uint8_t - byte read from Flash
// Parameters   :
//   1) FLADDR addr - address of the byte to read to
//                    valid range is from 0x0000 to 0xFBFF for 64K devices
//                    valid range is from 0x0000 to 0x7FFF for 32K devices
//
// This routine reads a <byte> from the linear FLASH address <addr>.
//-----------------------------------------------------------------------------
uint8_t FLASH_ByteRead (uint16_t addr, uint8_t SFLE)
{
   uint8_t EA_Save = IE;                    // Preserve IE_EA

   SI_SEGMENT_VARIABLE_SEGMENT_POINTER(pread, uint8_t, code, data);
   //char code * data pread;             // FLASH read pointer

   uint8_t byte;

   IE_EA = 0;                             // Disable interrupts

   pread = (char code *) addr;

   if(SFLE)
   {
      PSCTL |= 0x04;                   // Access Scratchpad
   }

   byte = *pread;                      // Read the byte

   PSCTL &= ~0x04;                     // SFLE = 0

   if ((EA_Save & 0x80) != 0)          // Restore IE_EA
   {
      IE_EA = 1;
   }

   return byte;

}

//-----------------------------------------------------------------------------
// FLASH_PageErase
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - address of any byte in the page to erase
//                    valid range is from 0x0000 to 0xFBFF for 64K devices
//                    valid range is from 0x0000 to 0x7FFF for 32K devices
//
// This routine erases the FLASH page containing the linear FLASH address
// <addr>.  Note that the page of Flash containing the Lock Byte cannot be
// erased if the Lock Byte is set.
//
//-----------------------------------------------------------------------------
void FLASH_PageErase (uint16_t addr, uint8_t SFLE)
{
   uint8_t EA_Save = IE;                    // Preserve IE_EA

   SI_SEGMENT_VARIABLE_SEGMENT_POINTER(pwrite, uint8_t, xdata, data);
   //unsigned char xdata * data pwrite;// FLASH write pointer

   IE_EA = 0;                             // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x06;                      // Enable VDD monitor as a reset source
                                       // Leave missing clock detector enabled

   pwrite = (char xdata *) addr;

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x03;                      // PSWE = 1; PSEE = 1

   if(SFLE)
   {
      PSCTL |= 0x04;                   // Access Scratchpad
   }

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
   *pwrite = 0;                        // Initiate page erase

   PSCTL &= ~0x07;                     // SFLE = 0; PSWE = 0; PSEE = 0

   if ((EA_Save & 0x80) != 0)          // Restore IE_EA
   {
      IE_EA = 1;
   }
}
