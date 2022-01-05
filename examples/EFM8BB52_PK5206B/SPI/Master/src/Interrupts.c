//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

         
// USER INCLUDES			
#include <SI_EFM8BB52_Register_Enums.h>
#include "EFM8BB52_SPI_Master.h"

//-----------------------------------------------------------------------------
// SPI0_ISR
//-----------------------------------------------------------------------------
//
// SPI0 ISR Content goes here. Remember to clear flag bits:
// SPI0CN0::MODF (Mode Fault Flag)
// SPI0CN0::RXOVRN (Receive Overrun Flag)
// SPI0CN0::SPIF (SPI# Interrupt Flag)
// SPI0CN0::WCOL (Write Collision Flag)
//
//
// Handles all error checks and single-byte writes.
//
// Note: SPI_WRITE_ARRAY is not handled by this ISR in order to take
// advantage of double-buffering (checking the SPI0CN0_TXBMT flag) using polling.
//
//
// Typical Write:
//
//              | 1st sent | 2nd sent | 3rd sent |   ...    | last sent |
//              ---------------------------------------------------------
//  Master NSSv | Command  |   Data1  |   Data2  |   ...    |   DataN   |  NSS^
//  Slave       |   N/A    |    N/A   |    N/A   |   ...    |    N/A    |
//
// Typical Read:
//
//              | 1st sent | 2nd sent | 3rd sent |   ...    | last sent |
//              ---------------------------------------------------------
//  Master NSSv | Command  |   dummy  |   dummy  |   ...    |   dummy   |  NSS^
//  Slave       |   N/A    |   Data1  |   Data2  |   ...    |   DataN   |
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(SPI0_ISR, SPI0_IRQn)
{
   static uint8_t array_index = 0;
   static char state = 0;
   uint8_t rx_byte;

   if(SPI0CN0_WCOL == 1)
   {
      // Write collision occurred
      SPI0CN0_WCOL = 0;                        // Clear the write collision flag

      Error_Flag = 1;
   }
   else
   {
      // Cache the value from the RX FIFO (since reading
      // SPI0DAT more than once would dequeue or underrun the FIFO)
      rx_byte = SPI0DAT;

      if(rx_byte == ERROR_OCCURRED)
      {
         // This example recognizes when an error occurs, but does not include
         // any error handling.  The transfer can be aborted or rescheduled,
         // if desired.
         Error_Flag = 1;
      }

      // When the Master enters the ISR, the SPI0CN0_SPIF flag should be set from
      // sending the Command byte.  This ISR handles the remaining steps of the
      // SPI transfer process.
      // <state> == 0: writing or reading 1 byte of data
      // <state> == 1: for READ commands (first time, only a dummy byte is
      //               sent but the second time, the data must be read from
      //               SPI0DAT)
      // <state> == 2: NSS = 1 to end the transfer, final byte read
      //
      // Note: SPI_WRITE_BUFFER is not handled here because it's done in
      // polled mode
      if(state == 0)
      {
         switch(Command)
         {
            case SLAVE_LED_ON:
            case SLAVE_LED_OFF:
               SPI0CN0_NSSMD0 = 1;      // Release the slave (not expecting
                                       // data back)

               break;

            case SPI_WRITE:
               SPI0DAT = SPI_Data;

               state = 2;              // Advance to the final state (only
                                       // writing one byte)

               break;

            case SPI_READ:
               SPI0DAT = 0xFF;         // Send a dummy byte so the Slave can
                                       // send the data

               state = 2;              // Advance to the final state (only
                                       // reading one byte)

               break;

            case SPI_READ_BUFFER:
               array_index = 0;        // Clear the data counter

               SPI0DAT = 0xA1;         // Send a dummy byte so the Slave can
                                       // start sending the data

               state = 1;              // Advance to the next state where the
                                       // data can be received
                                       // The data from the slave is not
                                       // available until after the second
                                       // transfer is completed.
                                       // The dummy byte allows the slave to
                                       // send data, since the Master controls
                                       // SCK.

               break;

            default:
               state = 2;              // Any errors in the Command parsing
                                       // should go to state 2 where SPI0CN0_NSSMD0
                                       // is de-asserted
         }
      }
      else if(state == 1)              // This state is for READ_ARRAY
      {                                // commands where the data must be read
                                       // after the first dummy byte is sent
         switch(Command)
         {
            case SPI_READ_BUFFER:
               SPI_Data_Array[array_index] = rx_byte;
               SPI0DAT = 0xA2;

               array_index++;

               if(array_index == (MAX_BUFFER_SIZE-1))
               {
                  state = 2;
               }

               break;
            default:
               state = 2;              // Any errors in the Command parsing
                                       // should go to state 2 where NSS0MD0
                                       // is de-asserted
         }
      }
      else if(state == 2)
      {
         switch(Command)
         {
            case SPI_READ:
               SPI_Data = rx_byte;     // Read the data from the slave

               break;

            case SPI_READ_BUFFER:
               SPI_Data_Array[array_index] = rx_byte; // Read the last data
               state = 0;                             // without sending a
                                                      // dummy byte

               break;
         }

         SPI0CN0_NSSMD0 = 1;            // De-select the Slave

         state = 0;                    // Reset the state
      }

      SPI0CN0_SPIF = 0;                 // Clear the SPIF0 flag
   }
}
