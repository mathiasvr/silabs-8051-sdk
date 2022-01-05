//-----------------------------------------------------------------------------
// F85x_SMBus_Master_Multibyte.c
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
// - Timer1 used as SMBus clock source
// - Timer3 used by SMBus for SCL low timeout detection
// - SCL frequency defined by <SMB_FREQUENCY> constant
// - SMB0CN_ARBLOST support included
// - supports multiple-byte writes and multiple-byte reads
// - Pinout:
//    P0.0 -> SDA (SMBus)
//    P0.1 -> SCL (SMBus)
//
//    P1.0 -> LED1
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
// How To Test: UPMU-F850
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - JP2
// 2) Ensure that J32 is not populated.
// 3) Connect the device to an SMBus slave running the corresponding
//    SMBus_Slave_Multibyte code.
// 4) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 5) Compile and download code to the 'F85x UDP MCU board by selecting 
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the 
//		Resume button in the quick menu, or pressing F8:
//         a) The test will indicate proper communication with the EEPROM by
//            toggling the LED on and off each time a value is sent and
//            received.
//         b) The best method to view the proper functionality is to run to
//            the indicated line of code in the TEST CODE section of main and
//            view the SMB_DATA_IN and SMB_DATA_OUT variable arrays in the
//            Watch Window.
//
//-----------------------------------------------------------------------------
// How To Test: ToolStick F85x
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 3) Connect the device to an SMBus slave running the corresponding
//    SMBus_Slave_Multibyte code.
// 4) Compile and download code to the ToolStick F85x board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8:
//         a) The test will indicate proper communication with the EEPROM by
//            toggling the LED on and off each time a value is sent and
//            received.
//         b) The best method to view the proper functionality is to run to
//            the indicated line of code in the TEST CODE section of main and
//            view the SMB_DATA_IN and SMB_DATA_OUT variable arrays in the
//            Watch Window.
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.2 (BL)
//		- Updated How To Test with Studio specific instructions
//		- 3 JAN 2014
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 27 FEB 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>
#include "InitDevice.h"
#include "F85x_SMBus_Master_Multibyte.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(LED1, SFR_P1, 0);                 // DS5 P1.0 LED

SI_SBIT(SDA, SFR_P0, 0);                  // SMBus on P0.0
SI_SBIT(SCL, SFR_P0, 1);                  // and P0.1

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

volatile bit SMB_BUSY;                 // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

volatile bit SMB_RW;                   // Software flag to indicate the
                                       // direction of the current transfer

uint16_t NUM_ERRORS;                        // Counter for the number of errors.

//-----------------------------------------------------------------------------
// Function PROTOTYPES
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
// MAIN Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, then loops forever sending
// and receiving SMBus data to the slave.
//
//-----------------------------------------------------------------------------
void main (void)
{
   volatile uint8_t dat;                    // Test counter
   volatile uint8_t data_count;             // SMB_DATA_IN and SMB_DATA_OUT counter
   uint8_t i;                               // Dummy variable counters

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
         LED1 = LED_OFF;
      }
      else {
         LED1 = !LED1;
      }

      // Run to here to view the SMB_DATA_IN and SMB_DATA_OUT variable arrays

      T0_Waitms (50);                  // Wait 50 ms until the next cycle
                                       // so that LED blinks slow enough to see
   }

// END TEST CODE---------------------------------------------------------------

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
void SMB_Read(void)
{
   while(SMB_BUSY);                    // Wait for bus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)
   SMB_RW = 1;                         // Mark this transfer as a READ

   SMB0CN_STA = 1;                            // Start transfer

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
