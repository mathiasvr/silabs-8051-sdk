//-----------------------------------------------------------------------------
// F93x_SMBus_Master_Multibyte.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Example software to demonstrate the C8051F93x SMBus interface in
// Master mode.
// - Interrupt-driven SMBus implementation
// - Only master states defined (no slave or arbitration)
// - multiple-byte SMBus data holders used for each transmit and receive
// - Timer1 used as SMBus clock source
// - Timer3 used by SMBus for SCL low timeout detection
// - SCL frequency defined within Configurator
// - SMB0CN_ARBLOST support included
// - supports multiple-byte writes and multiple-byte reads
// - Pinout:
//    P0.0 -> SDA (SMBus)
//    P0.1 -> SCL (SMBus)
//
//    P1.6 -> YELLOW_LED
//
//    P2.0 -> C2D (debug interface)
//
//    all other port pins unused
//
// How To Test:
//
// 1) Verify that J13 and J14 are not populated.
// 2) Connect the device to another 'F91x device running SMBus slave multibyte
//    or SMBus slave multibyte HWACK code.
// 3) Ensure that jumpers are placed on the following:
//       J8:   P1.6/LED1.6
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 4) Ensure the Battery Cell select switch (SW4) is set to "2 CELL".
// 5) Connect the USB Debug Adapter to J9.
// 6) Turn the POWER switch (SW5) to the "ON" position.
// 7) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//    The slave code will write data and read data from the
//    same data byte, so a successive write and read will effectively echo the
//    data written.  To verify that the code is working properly, verify on the
//    master that the data written is the same as the data received.
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 15 JAN 2014
//
// Release 1.2
//    - Updated to be compatible with Raisonance Toolchain (FB)
//    - 14 APR 2010
//
// Release 1.1
//    - Compiled and tested for C8051F930-TB (JH)
//    - 07 JULY 2009
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 NOV 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F930_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include "F93x_SMBus_Master_Multibyte.h"

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

// Global holder for SMBus data
// All receive data is written here
uint8_t SMB_DATA_IN[NUM_BYTES_RD];

// Global holder for SMBus data.
// All transmit data is read from here
uint8_t SMB_DATA_OUT[NUM_BYTES_WR];


uint8_t TARGET;                             // Target SMBus slave address

bit SMB_BUSY;                          // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

bit SMB_RW;                            // Software flag to indicate the
                                       // direction of the current transfer

uint16_t NUM_ERRORS;                        // Counter for the number of errors.

SI_SBIT(SDA, SFR_P0, 0);                 // SMBus on P0.0
SI_SBIT(SCL, SFR_P0, 1);                 // and P0.1

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SMB_Write (void);
void SMB_Read (void);
void T0_Waitms (uint8_t ms);

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
	volatile uint8_t dat;         // Test counter
	volatile uint8_t data_count;  // SMB_DATA_IN and SMB_DATA_OUT counter
	uint8_t i;                    // Dummy variable counters

	//Enter default mode
	enter_DefaultMode_from_RESET();

	// If slave is holding SDA low because of an improper SMBus reset or error
	while(!SDA)
	{
	  // Provide clock pulses to allow the slave to advance out
	  // of its current state. This will allow it to release SDA.
	  XBR1 = 0x40;                     // Enable Crossbar
	  SCL = 0;                         // Drive the clock low
	  for(i = 0; i < 255; i++);        // Hold the clock low
	  SCL = 1;                         // Release the clock
	  while(!SCL);                     // Wait for open-drain
									   // clock output to rise
	  for(i = 0; i < 10; i++);         // Hold the clock high
	  XBR1 = 0x00;                     // Disable Crossbar
	}

	//Enter default mode
	enter_DefaultMode_2_from_DefaultMode();

	YELLOW_LED = LED_OFF;

	// TEST CODE-------------------------------------------------------------------

	dat = 0;                            // Output data counter
	NUM_ERRORS = 0;                     // Error counter
	while (1)
	{
	  // SMBus Write Sequence
	  for (data_count = 0; data_count < NUM_BYTES_WR; data_count++)
	  {
		 SMB_DATA_OUT[data_count] = dat; // Define next outgoing byte
		 dat++;
	  }
	  TARGET = SLAVE_ADDR;             // Target the Slave for next SMBus
									   // transfer
	  SMB_Write();                     // Initiate SMBus write

	  // SMBus Read Sequence
	  TARGET = SLAVE_ADDR;             // Target the Slave for next SMBus
									   // transfer
	  SMB_Read();

	  // Check transfer data
	  for (data_count = 0; data_count < NUM_BYTES_RD; data_count++)
	  {
		 // Received data match transmit data?
		 if(SMB_DATA_IN[data_count] != SMB_DATA_OUT[data_count])
		 {
			NUM_ERRORS++;              // Increment error counter if no
									   // match
		 }
	  }

	  // Indicate that an error has occurred (LED no longer lit)
	  if (NUM_ERRORS > 0) {
		 YELLOW_LED = LED_OFF;
	  }
	  else {
		 YELLOW_LED = !YELLOW_LED;
	  }

	  // Run to here to view the SMB_DATA_IN and SMB_DATA_OUT variable arrays

	  T0_Waitms (50);                  // Wait 50 ms until the next cycle
									   // so that LED blinks slow enough to see
	}

	// END TEST CODE---------------------------------------------------------------

	while (1) {}                             // Spin forever

	// NOTREACHED
	return 0;
}

//-----------------------------------------------------------------------------
// Support Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SMB_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Writes a single byte to the slave with address specified by the <TARGET>
// variable.
// Calling sequence:
// 1) Write target slave address to the <TARGET> variable
// 2) Write outgoing data to the <SMB_DATA_OUT> variable array
// 3) Call SMB_Write()
//
//-----------------------------------------------------------------------------
void SMB_Write (void)
{
   while (SMB_BUSY);                   // Wait for SMBus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)
   SMB_RW = 0;                         // Mark this transfer as a WRITE
   SMB0CN_STA = 1;                            // Start transfer
}

//-----------------------------------------------------------------------------
// SMB_Read
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Reads a single byte from the slave with address specified by the <TARGET>
// variable.
// Calling sequence:
// 1) Write target slave address to the <TARGET> variable
// 2) Call SMB_Write()
// 3) Read input data from <SMB_DATA_IN> variable array
//
//-----------------------------------------------------------------------------
void SMB_Read (void)
{
   while (SMB_BUSY);                   // Wait for bus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)
   SMB_RW = 1;                         // Mark this transfer as a READ

   SMB0CN_STA = 1;                            // Start transfer

   while (SMB_BUSY);                   // Wait for transfer to complete
}

//-----------------------------------------------------------------------------
// T0_Waitms
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint8_t ms - number of milliseconds to wait
//                        range is full range of character: 0 to 255
//
// Configure Timer0 to wait for <ms> milliseconds using SYSCLK as its time
// base.
//
//-----------------------------------------------------------------------------
void T0_Waitms (uint8_t ms)
{
   TCON &= ~0x30;                      // Stop Timer0; Clear TCON_TF0
   TMOD &= ~0x0f;                      // 16-bit free run mode
   TMOD |=  0x01;

   CKCON |= 0x04;                      // Timer0 counts SYSCLKs

   while (ms) {
      TCON_TR0 = 0;                         // Stop Timer0
      TH0 = ((-SYSCLK/1000) >> 8);     // Overflow in 1ms
      TL0 = ((-SYSCLK/1000) & 0xFF);
      TCON_TF0 = 0;                         // Clear overflow indicator
      TCON_TR0 = 1;                         // Start Timer0
      while (!TCON_TF0);                    // Wait for overflow
      ms--;                            // Update ms counter
   }

   TCON_TR0 = 0;                            // Stop Timer0
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
