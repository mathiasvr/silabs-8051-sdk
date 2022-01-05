//-----------------------------------------------------------------------------
// F970_SMBus_Slave_Multibyte.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Example software to demonstrate the C8051F970 SMBus interface in Slave mode
// - Interrupt-driven SMBus implementation
// - Only slave states defined
// - Multi-byte SMBus data holders used for both transmit and receive
// - Timer1 used as SMBus clock rate (used only for free timeout detection)
// - Timer3 used by SMBus for SCL low timeout detection
// - ARBLOST support included
// - supports multiple-byte writes and multiple-byte reads
// - Pinout:
//    P0.1 -> SDA (SMBus)
//    P0.2 -> SCL (SMBus)
//
//    P0.5 -> LED
//
//    all other port pins unused
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Connect SMBus to a SMBus master.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
//      The slave code will copy the write data to the read
//      data, so a successive write and read will effectively echo the data
//      written. To verify that the code is working properly, verify on the
//      master that the data received is the same as the data written.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 7 MAY 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define  SYSCLK         24500000       // System clock frequency in Hz

#define  SMB_FREQUENCY  10000          // Target SMBus frequency
                                       // This example supports between 10kHz
                                       // and 100kHz

#define  WRITE          0x00           // SMBus WRITE command
#define  READ           0x01           // SMBus READ command

#define LED_ON           0             // Turns the LED on
#define LED_OFF          1             // Turns the LED off

#define  SLAVE_ADDR     0xF0           // Device addresses (7 bits,
                                       // lsb is a don't care)

// Status vector - top 4 bits only
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

#define  NUM_BYTES_WR   3              // Number of bytes to write
                                       // Slave <- Master
#define  NUM_BYTES_RD   3              // Number of bytes to read
                                       // Slave -> Master

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

// Global holder for SMBus data.
// All receive data is written here
// NUM_BYTES_WR used because an SMBus write is Master->Slave
uint8_t SMB_DATA_IN[NUM_BYTES_WR];

// Global holder for SMBus data.
// All transmit data is read from here
// NUM_BYTES_RD used because an SMBus read is Slave->Master
uint8_t SMB_DATA_OUT[NUM_BYTES_RD];

bit DATA_READY = 0;                    // Set to '1' by the SMBus ISR
                                       // when a new data byte has been
                                       // received.

SI_SBIT(LED05, SFR_P0, 5);               // LED05==LED_ON means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(TIMER3_ISR, TIMER3_IRQn);
SI_INTERRUPT_PROTO(SMBUS0_ISR, SMBUS0_IRQn);

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
  SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
  PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, then waits for SMBus
// communication.
//
//-----------------------------------------------------------------------------
void main (void)
{
   uint8_t i;

   enter_DefaultMode_from_RESET();

   // Initialize the outgoing data array in case a read is done before a
   // write
   for (i = 0; i < NUM_BYTES_RD; i++)
   {
      SMB_DATA_OUT[i] = 0xFD;
   }

   while(1)
   {
      while(!DATA_READY);              // New SMBus data received?
      DATA_READY = 0;

      // Copy the data from the input array to the output array
      for (i = 0; i < NUM_BYTES_RD; i++)
      {
         SMB_DATA_OUT[i] = SMB_DATA_IN[i];
      }

      LED05 = !LED05;
   }
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SMBus Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// SMBus ISR state machine
// - Slave only implementation - no master states defined
// - All incoming data is written to global variable <SMB_data_IN>
// - All outgoing data is read from global variable <SMB_data_OUT>
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(SMBUS0_ISR, SMBUS0_IRQn)
{
   static uint8_t sent_byte_counter;
   static uint8_t rec_byte_counter;

   SFRPAGE = SMBUS0_PAGE;

   if (SMB0CN_ARBLOST == 0)
   {
      switch (SMB0CN & 0xF0)           // Decode the SMBus status vector
      {
         // Slave Receiver: Start+Address received
         case  SMB_SRADD:

            SMB0CN_STA = 0;            // Clear STA bit

            sent_byte_counter = 1;     // Reinitialize the data counters
            rec_byte_counter = 1;

            if((SMB0DAT&0xFE) == (SLAVE_ADDR&0xFE)) // Decode address
            {                          // If the received address matches,
                SMB0CN_ACK = 1;        // ACK the received slave address

               if((SMB0DAT&0x01) == READ) // If the transfer is a master READ,
               {
                  // Prepare outgoing byte
                  SMB0DAT = SMB_DATA_OUT[sent_byte_counter-1];
                  sent_byte_counter++;
               }
            }
            else                       // If received slave address does not
            {                          // match,
                SMB0CN_ACK = 0;        // NACK received address
            }
            break;

         // Slave Receiver: Data received
         case  SMB_SRDB:

            if (rec_byte_counter < NUM_BYTES_WR)
            {
               // Store incoming data
               SMB_DATA_IN[rec_byte_counter-1] = SMB0DAT;
               rec_byte_counter++;

               SMB0CN_ACK = 1;         // ACK received data
            }
            else
            {
               // Store incoming data
               SMB_DATA_IN[rec_byte_counter-1] = SMB0DAT;

               DATA_READY = 1;         // Indicate new data fully received
               SMB0CN_ACK = 1;         // ACK received data
            }

            break;

         // Slave Receiver: Stop received while either a Slave Receiver or
         // Slave Transmitter
         case  SMB_SRSTO:

            SMB0CN_STO = 0;            // STO must be cleared by software when
                                       // a STOP is detected as a slave
            break;

         // Slave Transmitter: Data byte transmitted
         case  SMB_STDB:

            if (SMB0CN_ACK == 1)       // If Master ACK's, send the next byte
            {
               if (sent_byte_counter <= NUM_BYTES_RD)
               {
                  // Prepare next outgoing byte
                  SMB0DAT = SMB_DATA_OUT[sent_byte_counter-1];
                  sent_byte_counter++;
               }
            }                          // Otherwise, do nothing
            break;

         // Slave Transmitter: Arbitration lost, Stop detected
         //
         // This state will only be entered on a bus error condition.
         // In normal operation, the slave is no longer sending data or has
         // data pending when a STOP is received from the master, so the TXMODE
         // bit is cleared and the slave goes to the SRSTO state.
         case  SMB_STSTO:

            SMB0CN_STO = 0;            // STO must be cleared by software when
                                       // a STOP is detected as a slave
            break;

         // Default: all other cases undefined
         default:

            SMB0CF &= ~0x80;           // Reset communication
            SMB0CF |= 0x80;
            SMB0CN_STA = 0;
            SMB0CN_STO = 0;
            SMB0CN_ACK = 0;
            break;
      }
   }
   // ARBLOST = 1, Abort failed transfer
   else
   {
      SMB0CN_STA = 0;
      SMB0CN_STO = 0;
      SMB0CN_ACK = 0;
   }

   SMB0CN_SI = 0;                      // Clear SMBus interrupt flag
}

//-----------------------------------------------------------------------------
// Timer3 Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// A Timer3 interrupt indicates an SMBus SCL low timeout.
// The SMBus is disabled and re-enabled here
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER3_ISR, TIMER3_IRQn)
{
   SFRPAGE = SMBUS0_PAGE;
   SMB0CF &= ~0x80;                    // Disable SMBus
   SMB0CF |= 0x80;                     // Re-enable SMBus

   SFRPAGE = TIMER3_PAGE;
   TMR3CN &= ~0x80;                    // Clear Timer3 interrupt-pending flag
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
