//-----------------------------------------------------------------------------
// SMBus_Master.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Example software to demonstrate the EFM8UB2 SMBus interface in
// Master mode.
// - Interrupt-driven SMBus implementation
// - Only master states defined (no slave or arbitration)
// - 1-byte SMBus data holders used for each transmit and receive
// - Timer1 used as SMBus clock source
// - Timer3 used by SMBus for SCL low timeout detection
// - SCL frequency defined by <SMB_FREQUENCY> constant
// - SMB0CN0_ARBLOST support included
// - Pinout:
//    P1.2 -> SDA (SMBus)
//    P1.3 -> SCL (SMBus)
//
//    P1.6 -> LED
//
//    all other port pins unused
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect the device to another EFM8 device running SMBus - Slave code.
// 3) Connect the EFM8UB2 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8UB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) a. The test will indicate proper communication with the slave by
//       toggling LED0 on and off each time a value is sent and received.
//    b. The best method to view the proper functionality is to run to
//       the indicated line of code in the TEST CODE section of main and
//       view the SMB_DATA_IN and SMB_DATA_OUT variables in the Watch
//       Window.
//
//
// Target:         EFM8UB2
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 13 JAN 2015
//

//-----------------------------------------------------------------------------
// Includes and Device-Specific Parameters
//-----------------------------------------------------------------------------

#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define  SYSCLK         12000000       // System clock frequency in Hz

#define  SMB_FREQUENCY  10000          // Target SCL clock rate
                                       // This example supports between 10kHz
                                       // and 100kHz

#define  WRITE          0x00           // SMBus WRITE command
#define  READ           0x01           // SMBus READ command

#define LED_ON           0             // Turns the LED on
#define LED_OFF          1             // Turns the LED off
// Device addresses (7 bits, LSB is a don't care)
#define  SLAVE_ADDR     0xF0           // Device address for slave target

// Status vector - top 4 bits only
#define  SMB_MTSTA      0xE0           // (MT) start transmitted
#define  SMB_MTDB       0xC0           // (MT) data byte transmitted
#define  SMB_MRDB       0x80           // (MR) data byte received
// End status vector definition

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
uint8_t SMB_DATA_IN;                        // Global holder for SMBus data
                                       // All receive data is written here

uint8_t SMB_DATA_OUT;                       // Global holder for SMBus data.
                                       // All transmit data is read from here

uint8_t TARGET;                             // Target SMBus slave address

volatile bool SMB_BUSY;                          // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

volatile bool SMB_RW;                            // Software flag to indicate the
                                       // direction of the current transfer

uint16_t NUM_ERRORS;                        // Counter for the number of errors.

// 16-bit SI_SFR declarations

SI_SBIT (LED, SFR_P1, 6);                 // LED on port P1.3

SI_SBIT (SDA, SFR_P1, 2);                 // SMBus on P1.2
SI_SBIT (SCL, SFR_P1, 3);                 // and P1.3

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void SMB_Write (void);
void SMB_Read (void);
void T0_Wait_ms (uint8_t ms);

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
   PCA0MD &= ~0x40;                    // WDTE = 0 (watchdog timer enable bit)
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, then loops forever sending
// and receiving SMBus data to the slave <SLAVE_ADDR>.
//
void main (void)
{
   volatile uint8_t dat;                    // Test counter
   uint8_t i;                               // Dummy variable counters


   HFO0CN |= 0x03;                     // Set internal oscillator to highest
                                       // setting of 12000000

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

   enter_DefaultMode_from_RESET();

// TEST CODE-------------------------------------------------------------------

   dat = 0;                            // Output data counter
   NUM_ERRORS = 0;                     // Error counter
   while (1)
   {
      // SMBus Write Sequence
      SMB_DATA_OUT = dat;              // Define next outgoing byte
      TARGET = SLAVE_ADDR;             // Target the EFM8UB2/Si8250 Slave for next
                                       // SMBus transfer
      SMB_Write();                     // Initiate SMBus write

      // SMBus Read Sequence
      TARGET = SLAVE_ADDR;             // Target the EFM8UB2/Si8250 Slave for next
                                       // SMBus transfer
      SMB_Read();

      // Check transfer data
      if(SMB_DATA_IN != SMB_DATA_OUT)  // Received data match transmit data?
      {
         NUM_ERRORS++;                 // Increment error counter if no match
      }

      // Indicate that an error has occurred (LED no longer lit)
      if (NUM_ERRORS > 0)
      {
         LED = 0;
      }
      else
      {
         LED = ~LED;
      }

      // Run to here to view the SMB_DATA_IN and SMB_DATA_OUT variables

      dat++;

      T0_Wait_ms (50);                  // Wait 50 ms until the next cycle
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
// 2) Write outgoing data to the <SMB_DATA_OUT> variable
// 3) Call SMB_Write()
//
void SMB_Write (void)
{
   while (SMB_BUSY);                   // Wait for SMBus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)
   SMB_RW = 0;                         // Mark this transfer as a WRITE
   SMB0CN0_STA = 1;                            // Start transfer
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
// 3) Read input data from <SMB_DATA_IN> variable
//
void SMB_Read (void)
{
   while (SMB_BUSY);                   // Wait for bus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)
   SMB_RW = 1;                         // Mark this transfer as a READ

   SMB0CN0_STA = 1;                            // Start transfer

   while (SMB_BUSY);                   // Wait for transfer to complete
}

//-----------------------------------------------------------------------------
// T0_Wait_ms
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
void T0_Wait_ms (uint8_t ms)
{
   TCON &= ~0x30;                      // Stop Timer0; Clear TCON_TF0
   TMOD &= ~0x0f;                      // 16-bit free run mode
   TMOD |=  0x01;

   CKCON0 |= 0x04;                      // Timer0 counts SYSCLKs

   while (ms) {
      TCON_TR0 = 0;                         // Stop Timer0
      TH0 = ((-(SYSCLK/1000)) >> 8);   // Overflow in 1ms
      TL0 = ((-(SYSCLK/1000)) & 0xFF);
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
