//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================


// USER INCLUDES
#include <SI_EFM8UB2_Register_Enums.h>

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
extern uint8_t SMB_DATA_IN;                        // Global holder for SMBus data
                                       // All receive data is written here

extern uint8_t SMB_DATA_OUT;                       // Global holder for SMBus data.
                                       // All transmit data is read from here

extern uint8_t TARGET;                             // Target SMBus slave address

extern volatile bool SMB_BUSY;                          // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

extern volatile bool SMB_RW;                            // Software flag to indicate the
                                       // direction of the current transfer

extern uint16_t NUM_ERRORS;                        // Counter for the number of errors.

// 16-bit SI_SFR declarations

SI_SBIT (LED, SFR_P1, 6);                 // LED on port P1.3

SI_SBIT (SDA, SFR_P1, 2);                 // SMBus on P1.2
SI_SBIT (SCL, SFR_P1, 3);                 // and P1.3


//-----------------------------------------------------------------------------
// SMBus Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// SMBus ISR state machine
// - Master only implementation - no slave or arbitration states defined
// - All incoming data is written to global variable <SMB_DATA_IN>
// - All outgoing data is read from global variable <SMB_DATA_OUT>
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(SMBUS0_ISR, SMBUS0_IRQn)
{
   bool FAIL = 0;                       // Used by the ISR to flag failed
                                       // transfers
   static bool ADDR_SEND = 0;           // Used by the ISR to flag byte
                                       // transmissions as slave addresses

   if (SMB0CN0_ARBLOST == 0)                   // Check for errors
   {
      // Normal operation
      switch (SMB0CN0 & 0xF0)           // Status vector
      {
         // Master Transmitter/Receiver: START condition transmitted.
         case SMB_MTSTA:
            SMB0DAT = TARGET;          // Load address of the target slave
            SMB0DAT &= 0xFE;           // Clear the LSB of the address for the
                                       // R/W bit
            SMB0DAT |= (uint8_t) SMB_RW;    // Load R/W bit
            SMB0CN0_STA = 0;                   // Manually clear START bit
            ADDR_SEND = 1;
            break;

         // Master Transmitter: Data byte transmitted
         case SMB_MTDB:
            if (SMB0CN0_ACK)                   // Slave SMB0CN0_ACK?
            {
               if (ADDR_SEND)          // If the previous byte was a slave
               {                       // address,
                  ADDR_SEND = 0;       // Next byte is not a slave address
                  if (SMB_RW == WRITE) // If this transfer is a WRITE,
                  {
                     // send data byte
                     SMB0DAT = SMB_DATA_OUT;
                  }
                  else {}              // If this transfer is a READ,
                                       // proceed with transfer without
                                       // writing to SMB0DAT (switch
                                       // to receive mode)
               }
               else                    // If previous byte was not a slave
               {                       // address,
                  SMB0CN0_STO = 1;             // Set SMB0CN0_STO to terminate transfer
                  SMB_BUSY = 0;        // And free SMBus interface
               }
            }
            else                       // If slave NACK,
            {
               SMB0CN0_STO = 1;                // Send STOP condition, followed
               SMB0CN0_STA = 1;                // By a START
               NUM_ERRORS++;           // Indicate error
            }
            break;

         // Master Receiver: byte received
         case SMB_MRDB:
            SMB_DATA_IN = SMB0DAT;     // Store received byte
            SMB_BUSY = 0;              // Free SMBus interface
            SMB0CN0_ACK = 0;                   // Send NACK to indicate last byte
                                       // of this transfer

            SMB0CN0_STO = 1;                   // Send STOP to terminate transfer
            break;

         default:
            FAIL = 1;                  // Indicate failed transfer
                                       // and handle at end of ISR
            break;

      } // end switch
   }
   else
   {
      // SMB0CN0_ARBLOST = 1, error occurred... abort transmission
      FAIL = 1;
   } // end SMB0CN0_ARBLOST if

   if (FAIL)                           // If the transfer failed,
   {
      SMB0CF &= ~0x80;                 // Reset communication
      SMB0CF |= 0x80;
      SMB0CN0_STA = 0;
      SMB0CN0_STO = 0;
      SMB0CN0_ACK = 0;

      SMB_BUSY = 0;                    // Free SMBus

      FAIL = 0;
      LED = 0;

      NUM_ERRORS++;                    // Indicate an error occurred
   }

   SMB0CN0_SI = 0;                             // Clear interrupt flag
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
   SMB0CF &= ~0x80;                    // Disable SMBus
   SMB0CF |= 0x80;                     // Re-enable SMBus
   TMR3CN0 &= ~0x80;                    // Clear Timer3 interrupt-pending flag
   SMB0CN0_STA = 0;
   SMB_BUSY = 0;                       // Free SMBus
}

