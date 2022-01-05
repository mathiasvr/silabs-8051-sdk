//-----------------------------------------------------------------------------
// SMBus_Multimaster.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Example software to demonstrate the EFM8UB2 SMBus interface in
// a Multi-Master environment.
// - Interrupt-driven SMBus implementation
// - Master and slave states defined
// - 1-byte SMBus data holders used for each transmit and receive
// - Timer1 used as SMBus clock source
// - Timer3 used by SMBus for SCL low timeout detection
// - SCL frequency defined by <SMB_FREQUENCY> constant
// - SMB0CN0_ARBLOST support included
// - When another master attempts to transmit at the same time:
//      - the EFM8UB2 SMBus master should detect any conflict using SMB0CN0_ARBLOST
//      - the EFM8UB2 SMBus master should give up the bus, if necessary, and
//        switch to slave receiver mode
//      - the EFM8UB2 SMBus master should transmit after the other master is done
// - Pinout:
//    P1.2 -> SDA (SMBus)
//    P1.3 -> SCL (SMBus)
//
//    P0.2 -> SW
//
//    P1.6 -> LED
//
//    all other port pins unused
//
// How To Test:
// 1) Place the switch in "AEM" mode.
// 2) Connect the device to another EFM8 device running SMBus - Slave code.
// 3) Connect the EFM8UB2 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8UB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) The test will indicate proper communication with the
//    other EFM8 device by blinking the LED on the other device on and
//    off each time SW is pressed, even if the buttons on both boards are
//    pressed at the same time.
//
//      EFM8UB2 Multimaster                     Other EFM8 Multimaster
//         LED (blinks) <------------------------------ Switch
//              SW --------------------------------> LED (blinks)
//
// NOTE: Before running the test, verify that MY_ADDR and MULTI_ADDR are
// defined to the proper values.
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

//
//

//-----------------------------------------------------------------------------
// Includes
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

// Device addresses (7 bits, lsb is a don't care)
uint8_t  MY_ADDR      =  0x0F;         // Device address for this device target
uint8_t  MULTI_ADDR   =  0xF0;         // Device address for other multimaster
                                       // target

// Status vector - top 4 bits only
#define  SMB_MTSTA      0xE0           // (MT) start transmitted
#define  SMB_MTDB       0xC0           // (MT) data byte transmitted
#define  SMB_MRDB       0x80           // (MR) data byte received

#define  SMB_SRADD      0x20           // (SR) slave address received
                                       //    (also could be a lost
                                       //    arbitration)
#define  SMB_SRSTO      0x10           // (SR) STOP detected while SR or ST,
                                       //    or lost arbitration
#define  SMB_SRDB       0x00           // (SR) data byte received, or
                                       //    lost arbitration
#define  SMB_STDB       0x40           // (ST) data byte transmitted
#define  SMB_STSTO      0x50           // (ST) STOP detected during a
                                       //    transaction; bus error
// End status vector definition

// Data to indicate the switch was pressed or released
#define  SW_PRESSED     0x0A
#define  SW_RELEASED    0x50

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

// Global holder for SMBus master data. All receive data is written here
// while in master mode
// Slave->Master
uint8_t SMB_DATA_IN_MASTER = 0x00;

// Global holder for SMBus master data. All transmit data is read from here
// while in master mode
// Master->Slave
uint8_t SMB_DATA_OUT_MASTER = 0x00;

// Global holder for SMBus slave data. All receive data is written here
// while slave mode
// Master->Slave
uint8_t SMB_DATA_IN_SLAVE = 0x00;

// Global holder for SMBus slave data. All transmit data is read from here
// while in slave mode
// Slave->Master
uint8_t SMB_DATA_OUT_SLAVE = 0x00;

volatile bool DATA_READY = 0;           // Set to '1' by the SMBus ISR
                                       // when a new data byte has been
                                       // received in slave mode.

uint8_t TARGET;                             // Target SMBus slave address

volatile bool SMB_BUSY;                 // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

volatile bool SMB_RW;                   // Software flag to indicate the
                                       // direction of the current transfer

uint8_t NUM_ERRORS;                         // Counter for the number of errors.

// 16-bit SI_SFR declarations

SI_SBIT(SW, SFR_P0, 2);                        // Switch on P0.2

SI_SBIT(LED, SFR_P1, 6);                      // LED on port P1.6

SI_SBIT(SDA, SFR_P1, 2);                  // SMBus on P1.2
SI_SBIT(SCL, SFR_P1, 3);                  // and P1.3

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(TIMER3_ISR, TIMER3_IRQn);
SI_INTERRUPT_PROTO(SMBUS0_ISR, SMBUS0_IRQn);
SI_INTERRUPT_PROTO(TIMER0_ISR, TIMER0_IRQn);

void SMB_Write (void);
void Blink_LED (void);
void Stop_LED (void);

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
// Main routine performs all configuration tasks, then loops forever checking
// for and sending the LED commands.
//
void main (void)
{
   bool switch_pressed_flag = 0;        // Used to detect when the switch is
                                       // released
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

   while (1)
   {
      // Check if the switch was just pressed
      if ((switch_pressed_flag == 0) && (SW == 0))
      {
          // SMBus Write Sequence

          // Tell the other device the switch was pressed
          SMB_DATA_OUT_MASTER = SW_PRESSED;

          TARGET = MULTI_ADDR;         // Target the other multimaster device
          SMB_Write();                 // Initiate SMBus write

          switch_pressed_flag = 1;
      }

      // Determine if the switch was just released
      if ((switch_pressed_flag == 1) && (SW != 0))
      {
          // SMBus Write Sequence

          // Tell the other device the switch was released
          SMB_DATA_OUT_MASTER = SW_RELEASED;

          TARGET = MULTI_ADDR;         // Target the other multimaster device
          SMB_Write();                 // Initiate SMBus write

          switch_pressed_flag = 0;
      }

      // Check if data was sent to this device from the other multimaster
      // device
      if (DATA_READY == 1)
      {
         if (SMB_DATA_IN_SLAVE == SW_PRESSED)
         {
            Blink_LED();
         }
         else
         {
            if (SMB_DATA_IN_SLAVE == SW_RELEASED)
            {
               Stop_LED();
            }
         }

         DATA_READY = 0;
      }
   }  // Loop forever
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
// 2) Write outgoing data to the <SMB_data_out> variable
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
// Blink_LED
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Starts the LED blinking.
//
void Blink_LED (void)
{
   TCON_TR0 = 1;
}

//-----------------------------------------------------------------------------
// Stop_LED
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Stops the LED from blinking.
//
void Stop_LED (void)
{
   TCON_TR0 = 0;

   LED = 1;                            // Turn the LED off when it's not
                                       // blinking
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
