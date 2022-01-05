//-----------------------------------------------------------------------------
// F36x_SPI0_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program configures a C8051F36x as a SPI Slave. The 'F36x MCU
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
// P3.2 - LED        (digital output, push-pull)
//
// all other port pins unused.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: P3.2 - P3.2_LED
//    J1: Power selection, P2 9V, P4 USB or J9 Debugger
// 2) Connect the USB Debug Adapter to J9. If 9V power is selected on J1,
//    connect the 9V power adapter to P2. If VBUS is selected on J1, connect
//    USB cable to P4.
// 3) Verify J15 is NOT populated on the Target Board.
// 4) Connect SPI bus to another device running the SPI0_Master code.
// 5) Compile and download code to C8051F360 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 7) If the communication passes, the LEDs on both the Master and Slave
//    boards will blink slowly. If it fails, the LEDs will be OFF.
//
//
// Target:         C8051F36x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1
//    - Updated to use with Simplicity Studio (JL)
//    - 29 JUL 2014
//
// Release 1.0
//    -Initial Revision (TP)
//    -14 DEC 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>

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

SI_SBIT(LED, SFR_P3, 2);                  // LED='1' means ON

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
void Port_Init (void);
void SPI0_Init (void);
void Init_Device (void);

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable the Watchdog Timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{

   Oscillator_Init ();
   Port_Init ();
   SPI0_Init ();

   IE_EA = 1;                          // Enable global interrupts

   LED = 0;

   while (1);                          // Loop forever
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

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
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // Set the internal oscillator to 24.5 MHz
   OSCICN = OSCICN_IOSCEN__ENABLED | OSCICN_IFCN__HFOSC_DIV_1;

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Port_Init
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
// P3.2  -  Skipped,     Push-Pull,  Digital (LED D4 on Target Board)
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   P0MDOUT = 0x02;                     // Make MISO push-pull
   P3MDOUT = 0x04;                     // Make the LED push-pull

   P3SKIP = 0x04;                      // Skip the LED (P3.2)

   // Enable the SPI on the XBAR
   XBR0 = XBR0_SPI0E__ENABLED;
   // Enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED;

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   SPI0CFG = 0x00;                     // Operate in Slave mode
                                       // CKPHA = '0', CKPOL = '0'
   // 4-wire Slave mode, SPI enabled
   SPI0CN = SPI0CN_NSSMD__4_WIRE_SLAVE |
            SPI0CN_SPIEN__ENABLED;

   IE_ESPI0 = 1;                       // Enable SPI interrupts

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
               LED = 1;
               state = 0;              // End of transfer (no bytes to send)

               break;

            case SLAVE_LED_OFF:
               LED = 0;
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
               SPI0DAT = SPI_Data_Array[array_index]; // Immediately load
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