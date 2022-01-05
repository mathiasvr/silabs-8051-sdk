//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

         
// USER INCLUDES			
#include <SI_EFM8LB1_Register_Enums.h>
#include "EFM8LB1_SMBus_Slave.h"

//-----------------------------------------------------------------------------
// SMBUS0_ISR
//-----------------------------------------------------------------------------
//
// SMBUS0 ISR Content goes here. Remember to clear flag bits:
// SMB0CN0::SI (SMBus Interrupt Flag)
//
//
// SMBus ISR state machine
// - Slave only implementation - no master states defined
// - All incoming data is written to global variable <SMB_DATA_IN>
// - All outgoing data is read from global variable <SMB_DATA_OUT>
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(SMBUS0_ISR, SMBUS0_IRQn)
{
   if (SMB0CN0_ARBLOST == 0)
   {
      switch (SMB0CN0 & 0xF0)          // Decode the SMBus status vector
      {
         // Slave Receiver: Start+Address received
         case  SMB_SRADD:

            SMB0CN0_STA = 0;           // Clear SMB0CN0_STA bit
            if ((SMB0DAT & 0xFE) == (SLAVE_ADDR & 0xFE)) // Decode address
            {                          // If the received address matches,
               SMB0CN0_ACK = 1;        // SMB0CN0_ACK the received slave address
               if ((SMB0DAT & 0x01) == READ) // If the transfer is a master READ,
               {
                  SMB0DAT = SMB_DATA;  // Prepare outgoing byte
               }
            }
            else                       // If received slave address does not
            {                          // match,
               SMB0CN0_ACK = 0;        // NACK received address
            }
            break;

         // Slave Receiver: Data received
         case  SMB_SRDB:

            SMB_DATA = SMB0DAT;        // Store incoming data
            DATA_READY = 1;            // Indicate new data received
            SMB0CN0_ACK = 1;           // SMB0CN0_ACK received data

            break;

         // Slave Receiver: Stop received while either a Slave Receiver or
         // Slave Transmitter
         case  SMB_SRSTO:

            SMB0CN0_STO = 0;           // SMB0CN0_STO must be cleared by software when
                                       // a STOP is detected as a slave
            break;

         // Slave Transmitter: Data byte transmitted
         case  SMB_STDB:
                                       // No action required;
                                       // one-byte transfers
                                       // only for this example
            break;

         // Slave Transmitter: Arbitration lost, Stop detected
         //
         // This state will only be entered on a bus error condition.
         // In normal operation, the slave is no longer sending data or has
         // data pending when a STOP is received from the master, so the SMB0CN0_TXMODE
         // bit is cleared and the slave goes to the SRSTO state.
         case  SMB_STSTO:

            SMB0CN0_STO = 0;           // SMB0CN0_STO must be cleared by software when
                                       // a STOP is detected as a slave
            break;

         // Default: all other cases undefined
         default:

            SMB0CF &= ~0x80;           // Reset communication
            SMB0CF |= 0x80;
            SMB0CN0_STA = 0;
            SMB0CN0_STO = 0;
            SMB0CN0_ACK = 0;
            break;
      }
   }
   // SMB0CN0_ARBLOST = 1, Abort failed transfer
   else
   {
      SMB0CN0_STA = 0;
      SMB0CN0_STO = 0;
      SMB0CN0_ACK = 0;
   }

   SMB0CN0_SI = 0;                     // Clear SMBus interrupt flag
}

//-----------------------------------------------------------------------------
// TIMER3_ISR
//-----------------------------------------------------------------------------
//
// TIMER3 ISR Content goes here. Remember to clear flag bits:
// TMR3CN::TF3H (Timer # High Byte Overflow Flag)
// TMR3CN::TF3L (Timer # Low Byte Overflow Flag)
//
// A Timer3 interrupt indicates an SMBus SCL low timeout.
// The SMBus is disabled and re-enabled here
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER3_ISR, TIMER3_IRQn)
{
   SMB0CF  &= ~0x80;                   // Disable SMBus
   SMB0CF  |=  0x80;                   // Re-enable SMBus
   TMR3CN0 &= ~0x80;                   // Clear Timer3 interrupt-pending flag
}



