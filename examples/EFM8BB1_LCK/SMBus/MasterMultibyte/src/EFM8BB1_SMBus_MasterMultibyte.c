//-----------------------------------------------------------------------------
// EFM8BB1_SMBus_MasterMultibyte.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Example software to demonstrate the SMBus interface in Master mode.
// - Interrupt-driven SMBus implementation
// - Only master states defined (no slave or arbitration)
// - multiple-byte SMBus data holders used for each transmit and receive
// - SCL frequency defined by <SMB_FREQUENCY> constant
// - SMB0CN0_ARBLOST support included
// - supports multiple-byte writes and multiple-byte reads
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC / 1
//   SMBus  - SMBus master, SCL = 10 kHz
//   Timer1 - SMBus clock source
//   Timer3 - SMBus SCL low timeout detection
//   P1.2   - SMBus SDA
//   P1.3   - SMBus SCL
//   P1.4   - LED
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 LCK (SMBus Master) + EFM8BB1 LCK (SMBus Slave)
//-----------------------------------------------------------------------------
// 1) Connect the device first EFM8BB1 LCK to second EFM8BB1 LCK running the
//    corresponding SMBus_Slave code.
// 2) Connect the EFM8BB1 LCK board to a PC using a micro USB cable.
// 3) Compile and download code to the first EFM8BB1 LCK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) a. The test will indicate proper communication with the slave by
//       toggling LED1 on and off each time a value is sent and received.
//    b. The best method to view the proper functionality is to run to
//       the indicated line of code in the TEST CODE section of main and
//       view the SMB_DATA_IN and SMB_DATA_OUT variables in the Watch
//       Window.
//
// Target:         EFM8BB1
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 10 OCT 2014
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB1_Register_Enums.h>
#include "InitDevice.h"
#include "EFM8BB1_SMBus_MasterMultibyte.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
// Global holder for SMBus data
// All receive data is written here
uint8_t SMB_DATA_IN[NUM_BYTES_RD];

// Global holder for SMBus data.
// All transmit data is read from here
uint8_t SMB_DATA_OUT[NUM_BYTES_WR];

uint8_t TARGET;                        // Target SMBus slave address

volatile bool SMB_BUSY;                 // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

volatile bool SMB_RW;                   // Software flag to indicate the
                                       // direction of the current transfer

uint16_t NUM_ERRORS;                   // Counter for the number of errors.

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void SMB_Write(void);
void SMB_Read(void);
void T0_Waitms(uint8_t ms);

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
// Main Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, then loops forever sending
// and receiving SMBus data to the slave.
//
//-----------------------------------------------------------------------------
void main (void)
{
   volatile uint8_t dat;               // Test counter
   volatile uint8_t data_count;        // SMB_DATA_IN and SMB_DATA_OUT counter
   uint8_t i;                          // Dummy variable counters

   enter_BusFreeMode_from_RESET();


   // If slave is holding SDA low because of an improper SMBus reset or error
   while(!SDA)
   {
      // Provide clock pulses to allow the slave to advance out
      // of its current state. This will allow it to release SDA.
      XBR2 = 0x40;                     // Enable Crossbar
      SCL = 0;                         // Drive the clock low
      for(i = 0; i < 255; i++);        // Hold the clock low
      SCL = 1;                         // Release the clock
      while(!SCL);                     // Wait for open-drain
                                       // clock output to rise
      for(i = 0; i < 10; i++);         // Hold the clock high
      XBR2 = 0x00;                     // Disable Crossbar
   }

   enter_DefaultMode_from_BusFreeMode();

   LED1 = LED_OFF;

   // TEST CODE----------------------------------------------------------------

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
         if (SMB_DATA_IN[data_count] != SMB_DATA_OUT[data_count])
         {
            NUM_ERRORS++;              // Increment error counter if no
                                       // match
         }
      }

      // Indicate that an error has occurred (LED no longer lit)
      if (NUM_ERRORS > 0)
      {
         LED1 = LED_OFF;
      }
      else
      {
         LED1 = !LED1;
      }

      // Run to here to view the SMB_DATA_IN and SMB_DATA_OUT variable arrays

      T0_Waitms (50);                  // Wait 50 ms until the next cycle
                                       // so that LED blinks slow enough to see
   }

   // END TEST CODE------------------------------------------------------------

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
void SMB_Write(void)
{
   while(SMB_BUSY);                    // Wait for SMBus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)
   SMB_RW = 0;                         // Mark this transfer as a WRITE
   SMB0CN0_STA = 1;                    // Start transfer
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
void SMB_Read(void)
{
   while(SMB_BUSY);                    // Wait for bus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)
   SMB_RW = 1;                         // Mark this transfer as a READ

   SMB0CN0_STA = 1;                    // Start transfer

   while(SMB_BUSY);                    // Wait for transfer to complete
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
void T0_Waitms(uint8_t ms)
{
   TCON &= ~0x30;                      // Stop Timer0; Clear TCON_TF0
   TMOD &= ~0x0f;                      // 16-bit free run mode
   TMOD |=  0x01;

   CKCON0 |= 0x04;                     // Timer0 counts SYSCLKs

   while (ms) {
      TCON_TR0 = 0;                    // Stop Timer0
      TH0 = ((-SYSCLK/1000) >> 8);     // Overflow in 1ms
      TL0 = ((-SYSCLK/1000) & 0xFF);
      TCON_TF0 = 0;                    // Clear overflow indicator
      TCON_TR0 = 1;                    // Start Timer0
      while (!TCON_TF0);               // Wait for overflow
      ms--;                            // Update ms counter
   }

   TCON_TR0 = 0;                       // Stop Timer0
}

