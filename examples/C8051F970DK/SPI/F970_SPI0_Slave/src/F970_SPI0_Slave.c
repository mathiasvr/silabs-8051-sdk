//-----------------------------------------------------------------------------
// F970_SPI0_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program configures a C8051F970 as a SPI Slave. The 'F970 MCU
// is configured in 4-wire Slave Mode.
//
// This example is intended to be used with the SPI0_Master example.
//
// Pinout:
//
// P2.0 - SPI SCK    (digital input, open-drain)
// P2.1 - SPI MISO   (digital output, push-pull)
// P2.2 - SPI MOSI   (digital input, open-drain)
// P2.3 - SPI NSS    (digital input, open-drain)
//
// P0.5 - LED        (digital output, push-pull)
//
// all other port pins unused.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Connect SPI bus to another device running the SPI0_Master code.
// 4) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 5) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 6) If the communication passes, the LEDs on both the Master and Slave
//    boards will blink slowly. If it fails, the LEDs will be OFF.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2/JL)
//    - 15 MAY 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000    // Internal oscillator frequency in Hz

#define MAX_BUFFER_SIZE    8           // Maximum buffer Master will send

// Instruction Set
#define  SLAVE_LED_ON      0x01        // Turn the Slave LED on
#define  SLAVE_LED_OFF     0x02        // Turn the Slave LED off
#define  SPI_WRITE         0x04        // Send a byte from the Master to the
                                       // Slave
#define  SPI_READ          0x08        // Send a byte from the Slave to the
                                       // Master
#define  SPI_WRITE_BUFFER  0x10        // Send a series of bytes from the
                                       // Master to the Slave
#define  SPI_READ_BUFFER   0x20        // Send a series of bytes from the Slave
                                       // to the Master
#define  ERROR_OCCURRED    0x40        // Indicator for the Slave to tell the
                                       // Master an error occurred

#define  LED_ON             0
#define  LED_OFF            1

SI_SBIT(LED, P0, 5);                     // LED==LED_ON means ON

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint8_t SPI_Data = 0xA5;

uint8_t SPI_Data_Array[MAX_BUFFER_SIZE] = {0};

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(SPI0_ISR, SPI0_IRQn);

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
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   enter_DefaultMode_from_RESET();

   while (1);                           // Loop forever
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SPI_ISR
//-----------------------------------------------------------------------------
//
// Handles all incoming data and interprets the commands sent from the Master.
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
//-----------------------------------------------------------------------------
SI_INTERRUPT(SPI0_ISR, SPI0_IRQn)
{
   volatile uint8_t fbchar;

   static uint8_t command;
   static uint8_t array_index = 0;
   static uint8_t state = 0;
   uint8_t dummy_byte;

   SFRPAGE = SPI0_PAGE;

   if (SPI0CN_WCOL == 1)
   {
      // Write collision occurred

      SPI0DAT = ERROR_OCCURRED;        // Indicate an error occurred
      SPI0CN_WCOL = 0;                 // Clear the Write collision flag
   }
   else if (SPI0CN_RXOVRN == 1)
   {
      // Receive overrun occurred

      SPI0DAT = ERROR_OCCURRED;        // Indicate an error occurred
      SPI0CN_RXOVRN = 0;               // Clear the Receive Overrun flag
   }
   else
   {
      // SPIF caused the interrupt

      // Some commands are single-byte commands (SLAVE_LED_ON/SLAVE_LED_OFF)
      // For multiple-byte commands, use the state to determine action
      // <state> == 0: new transfer; a command is being received
      // <state> == 1: writing/reading data
      if (state == 0)
      {
         command = SPI0DAT;            // Read the command

         array_index = 0;              // Reset the array index

         switch (command)
         {
            case SLAVE_LED_ON:
               LED = LED_ON;
               state = 0;              // End of transfer (no bytes to send)

               break;

            case SLAVE_LED_OFF:
               LED = LED_OFF;
               state = 0;              // End of transfer (no bytes to send)

               break;

            case SPI_WRITE:
               state = 1;              // Do nothing
                                       // Move to State 1 to read data from
                                       // Master
               break;

            case SPI_READ:
               SPI0DAT = SPI_Data;     // Immediately load SPI0DAT with the
                                       // data requested by the Master, so the
                                       // Master can receive it at the  end of
                                       // the second transfer.
                                       // Because the slave sends the data
                                       // immediately, the Master's SCK is
                                       // limited to a clock slow enough that
                                       // the Slave has time to respond to a
                                       // read.
               state = 0;              // End of transfer (only one byte)

               break;

            case SPI_WRITE_BUFFER:
               state = 1;              // Do nothing
                                       // Move to State 1 to read data from
                                       // Master
               break;

            case SPI_READ_BUFFER:
               fbchar = SPI_Data_Array[array_index];
               SPI0DAT = fbchar;//SPI_Data_Array[array_index]; // Immediately load
                                       // SPI0DAT with the data requested by
                                       // the Master, so the Master can receive
                                       // it at the end of the second transfer.
                                       // Because the slave sends the data
                                       // immediately, the Master's SCK is
                                       // limited to a clock slow enough that
                                       // the Slave has time to respond to a
                                       // read.
               array_index++;

               state = 1;              // Move to State 1 to continue to send
                                       // data to the Master (multiple bytes).
               break;

            default:
               state = 0;
         }
      }
      else if (state == 1)
      {
         switch (command)
         {
            case SPI_WRITE:
               SPI_Data = SPI0DAT;

               state = 0;              // End of transfer (one byte received)

               break;

            case SPI_WRITE_BUFFER:
               SPI_Data_Array[array_index] = SPI0DAT; // Receive the next byte

               array_index++;

               if (array_index == MAX_BUFFER_SIZE) // Check for last data
               {
                  state = 0;           // Reset the state (end of transfer)
               }
               else
               {
                  state = 1;           // Continue to read in data (more
                                       // bytes to receive)
               }
               break;

            case SPI_READ_BUFFER:
               SPI0DAT = SPI_Data_Array[array_index]; // Send the next byte
               dummy_byte = SPI0DAT;   // Read the dummy data the Master is
                                       // sending from SPI0DAT to prevent a
                                       // RXOVRN (Receive overrun) error
               array_index++;

               if (array_index == MAX_BUFFER_SIZE) // Check for last data
               {
                  state = 0;           // Reset the state (end of transfer)
               }
               else
               {
                  state = 1;           // Continue to send out data (more
                                       // bytes to send)
               }
               break;

            default:
               state = 0;
         }
      }

      SPI0CN_SPIF = 0;                 // Clear the SPIF flag
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
