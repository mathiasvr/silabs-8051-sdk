//-----------------------------------------------------------------------------
// F70x_SPI0_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program configures a C8051F700 as a SPI Slave. The 'F700 MCU
// is configured in 4-wire Slave Mode.
//
// This example is intended to be used with the SPI0_Master example.
//
// Pinout:
//
// P0.0 - SPI SCK    (digital input, open-drain)
// P0.1 - SPI MISO   (digital output, push-pull)
// P0.2 - SPI MOSI   (digital input, open-drain)
// P0.3 - SPI NSS    (digital input, open-drain)
//
// P1.0 - LED        (digital output, push-pull)
//
// all other port pins unused.
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//	  - J15: VDD - power source
//    - J8: P1.0 - DS3
// 2) Verify that the J13 and J14 jumpers are not populated.
// 3) Connect the device to another F70x device running SPI - Master code.
// 4) Connect the USB Debug Adapter to P3.
// 5) Connect the USB cable to P4.
// 6) Compile and download code to the C8051F70x development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8:
//    The LED will blink fast during the write/read/verify
//    operations.
// 8) If the communication passes, the LEDs on both the Master and Slave
//    boards will blink slowly. If it fails, the LEDs will be OFF.
//
// Target:         C8051F70x
// Tool chain:     Simplicity Studio / Keil C51 9.53
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (PD)
//    -09 JUL 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "si_toolchain.h"
#include <SI_C8051F700_defs.h>         // SFR declarations

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

SI_SBIT(LED, SFR_P1, 0);                 // LED==LED_ON means ON

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint8_t SPI_Data = 0xA5;

uint8_t SPI_Data_Array[MAX_BUFFER_SIZE] = {0};

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void PCA0_Init (void);
void Oscillator_Init (void);
void PORT_Init (void);
void SPI0_Init (void);
void Init_Device (void);

SI_INTERRUPT_PROTO(SPI0_ISR, INTERRUPT_SPI0);

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
   WDTCN = 0xDE;                       // Disable watchdog timer
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   Init_Device ();                     // Initializes hardware peripherals

   IE_EA = 1;                          // Enable global interrupts

   LED = LED_OFF;

   while (1);                          // Loop forever
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function disables the watchdog timer.
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   PCA0MD = 0x00;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at 24.5 MHz.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   OSCICN |= 0x80;                     // Enable the precision internal osc.
      
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc. 
                                       // divided by 1 as the system clock
   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P0.0  -  SCK  (SPI0), Open-Drain, Digital
// P0.1  -  MISO (SPI0), Push-Pull,  Digital
// P0.2  -  MOSI (SPI0), Open-Drain, Digital
// P0.3  -  NSS  (SPI0), Open-Drain, Digital
//
// P1.6  -  Skipped,     Push-Pull,  Digital (LED on Target Board)
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   P0MDOUT = 0x02;                     // Make MISO push-pull
   P1MDOUT = 0x40;                     // Make the LED push-pull

   P1SKIP = 0x40;                      // Skip the LED (P1.6)

   XBR0 = 0x02;                        // Enable the SPI on the XBAR
   XBR1 = 0x40;                        // Enable the XBAR and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// SPI0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configures SPI0 to use 4-wire Slave mode. The SPI timing is
// configured for Mode 0,0 (data centered on first edge of clock phase and
// SCK line low in idle state).
//
//-----------------------------------------------------------------------------
void SPI0_Init()
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   SPI0CFG = 0x00;                     // Operate in Slave mode
                                       // CKPHA = '0', CKPOL = '0'
   SPI0CN = 0x05;                      // 4-wire Slave mode, SPI enabled

   IE_ESPI0 = 1;                       // Enable SPI interrupts

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Init_Device
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Calls all device initialization functions.
//
//-----------------------------------------------------------------------------
void Init_Device (void)
{

   PCA0_Init ();                   
   Oscillator_Init ();
   PORT_Init ();
   SPI0_Init ();
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
