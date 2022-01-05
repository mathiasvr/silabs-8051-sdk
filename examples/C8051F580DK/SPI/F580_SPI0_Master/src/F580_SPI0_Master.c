//-----------------------------------------------------------------------------
// F580_SPI0_Master.c
//-----------------------------------------------------------------------------
// Copyright 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program configures a C8051F580 as a 4-wire SPI Single Master.
//
// The SPI clock in this example is limited to 500 kHz when used with the
// SPI0_Slave code example.  During a SPI_Read, the slave needs some time to
// interpret the command and write the appropriate data to the SPI0DAT
// register, and the slave no longer has enough time to complete the
// SPI_READ_BUFFER command with a clock greater than 500 kHz.  For faster SPI
// clocks, a dummy byte between the command and the first byte of Read data
// will be required.
//
// This example is intended to be used with the SPI0_Slave example.
//
// Pinout:
//
// P0.0 - SPI SCK    (digital output, push-pull)
// P0.1 - SPI MISO   (digital input, open-drain)
// P0.2 - SPI MOSI   (digital output, push-pull)
// P0.3 - SPI NSS    (digital output, push-pull)
//
// P1.3 - LED        (digital output, push-pull)
//
//
// How To Test:
//
// 1) Build and Download F580_SPI0_Master.c to Side A of a 'F580-TB
// 2) Build and Download F580_SPI0_Slave.c to Side B of a 'F580-TB
// 3) Verify that J22 and J32 are not populated with shorting blocks
// 4) Verify that J19 and J11 are fully populated with shorting blocks
// 5) Connect Side A pin P0.0 to Side B P0.0 (SCK to SCK)
// 6) Connect Side A pin P0.1 to Side B P0.1 (MISO to MISO)
// 7) Connect Side A pin P0.2 to Side B P0.2 (MOSI to MOSI)
// 8) Connect Side A pin P0.3 to Side B P0.3 (NSS to NSS)
// 9) Run the code.
//         a) the test will indicate proper communication with the EEPROM by
//            turning on the LED at the end the end of the test
//         b) Another method to verify correct functionality is:
//            - Place the cursor on an if-statement in the main method.
//            - Click the menu Run > Toggle Breakpoint.
//            - Click Resume in the toolbar.
//            - Click the Variables view. Inspect the variables
//              "temp_char", "in_buff", "out_buff", and "error_flag"
//
//
// Target:         C8051F580 (Side A of a C8051F580-TB)
// Tool chain:     Keil C51 8.0 / Keil EVAL C51
// Command Line:   None
// Command Line:   None
//
// Release 1.2 / 10 JUL 2015 (SG)
//    - Slowed down SPI clock to eliminate errors
//
// Release 1.1 / 10 JUL 2015 (SG)
//    - Revised for Simplicity Studio
//
// Release 1.0 / 21 JUL 2008 (ADT)
//    - Initial Revision
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F580_Register_Enums.h>         // SFR declarations

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24000000    // Internal oscillator frequency in Hz

#define SPI_CLOCK          400000      // Maximum SPI clock
                                       // The SPI clock is a maximum of 400 kHz
                                       // when this example is used with
                                       // the SPI0_Slave code example.

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

SI_SBIT(LED, SFR_P1, 3);                 // LED==1 means ON

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint8_t SPI_Data = 0xA5;

uint8_t SPI_Data_Array[MAX_BUFFER_SIZE] = {0};

uint8_t Error_Flag = 0;
// Error_Flag & 0x01 == SPI_Byte_Read or SPI_Byte_Write failed
// Error_Flag & 0x02 == SPI_Array_Read or SPI_Array_Write failed
// Error_Flag & 0x04 == write collision occurred
// Error_Flag & 0x08 == slave-side error
// Error_Flag & 0x10 == unknown command

uint8_t Command = 0x00;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void PCA0_Init (void);
void OSCILLATOR_Init (void);
void PORT_Init (void);
void SPI0_Init (void);
void Init_Device (void);

void SPI_LED_On (void);
void SPI_LED_Off (void);
void SPI_Byte_Write (void);
void SPI_Byte_Read (void);
void SPI_Array_Write (void);
void SPI_Array_Read (void);

void Delay(void);
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
   PCA0MD &= ~0x40;                    // Disable the Watchdog Timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   uint8_t test_value = 0x55;
   uint8_t test_array[MAX_BUFFER_SIZE] = {1,2,3,4,5,6,7,8};
   uint8_t i;

   Init_Device ();                     // Initializes hardware peripherals

   IE_EA = 1;                          // Enable global interrupts

   LED = 0;

   // TEST BEGIN --------------------------------------------------------------
   SFRPAGE = ACTIVE1_PAGE;             // Set for SPI


   SPI_Data = test_value;

   // Write a value
   SPI_Byte_Write ();

   while (!SPI0CN_NSSMD0);             // Wait until the Write transfer has
                                       // finished

   // Read the same value back
   SPI_Data = 0x00;
   SPI_Byte_Read ();

   while (!SPI0CN_NSSMD0);             // Wait until the Read transfer has
                                       // finished

   // Check if the sent value and returned value match
   if (SPI_Data != test_value)
   {
      Error_Flag |= 0x01;
   }

   // Copy test_array into SPI_Data_Array
   for (i = 0; i < MAX_BUFFER_SIZE; i++)
   {
      SPI_Data_Array[i] = test_array[i];
   }

   // Send the array to the slave
   SPI_Array_Write ();

   while (!SPI0CN_NSSMD0);             // Wait until the Write transfer has
                                       // finished

   // Clear SPI_Data_Array for the SPI_Buffer_Read function
   for (i = 0; i < MAX_BUFFER_SIZE; i++)
   {
      SPI_Data_Array[i] = 0;
   }

   // Read the array back from the slave
   SPI_Array_Read ();

   while (!SPI0CN_NSSMD0);             // Wait until the Read transfer has
                                       // finished

   // Check if the received array matches the sent array
   for (i = 0; i < MAX_BUFFER_SIZE; i++)
   {
      if (SPI_Data_Array[i] != test_array[i])
      {
         Error_Flag |= 0x02;
      }
   }

   // END OF TEST -------------------------------------------------------------

   while (1)
   {
      // If no error has occurred, blink the LEDs on the Master and Slave
      // boards
      if (Error_Flag == 0)
      {
         LED = 1;

         SPI_LED_On ();

         while (!SPI0CN_NSSMD0);

         Delay ();

         SPI_LED_Off ();

         LED = 0;

         while (!SPI0CN_NSSMD0);

         Delay ();
      }
   };
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
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE; 


   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at 24 MHz.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE; 

   OSCICN = 0x87;                      // Set to Int Oscillator / 1

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
// P0.0  -  SCK  (SPI0), Push-Pull,  Digital
// P0.1  -  MISO (SPI0), Open-Drain, Digital
// P0.2  -  MOSI (SPI0), Push-Pull,  Digital
// P0.3  -  NSS  (SPI0), Push-Pull,  Digital
//
// P1.3  -  Skipped,     Push-Pull,  Digital (LED on Target Board)
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT = 0x0D;                     // Make SCK, MOSI, and NSS push-pull
   P1MDOUT = 0x08;                     // Make the LED push-pull

   P1SKIP = 0x08;                      // Skip the LED (P1.3)

   XBR0 = 0x04;                        // Enable the SPI on the XBAR
   XBR2 = 0x40;                        // Enable the XBAR and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// SPI0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configures SPI0 to use 4-wire Single Master mode. The SPI timing is
// configured for Mode 0,0 (data centered on first edge of clock phase and
// SCK line low in idle state).
//
//-----------------------------------------------------------------------------
void SPI0_Init()
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE;

   SPI0CFG = 0x40;                     // Enable the SPI as a Master
                                       // CKPHA = '0', CKPOL = '0'
   SPI0CN = 0x0D;                      // 4-wire Single Master, SPI enabled

   // SPI clock frequency equation from the datasheet
   SPI0CKR = (SYSCLK / (2 * SPI_CLOCK)) - 1;

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
   PCA0_Init ();                       // Disable the Watchdog Timer first
   OSCILLATOR_Init ();
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
// Handles all error checks and single-byte writes.
//
// Note: SPI_WRITE_ARRAY is not handled by this ISR in order to take
// advantage of double-buffering (checking the SPI0CN_TXBMT flag) using polling.
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
   static uint8_t state = 0;

   if (SPI0CN_WCOL == 1)
   {
      // Write collision occurred
      SPI0CN_WCOL = 0;                 // Clear the write collision flag

      Error_Flag |= 0x04;
   }
   else
   {
      if (SPI0DAT == ERROR_OCCURRED)
      {
         // This example recognizes when an error occurs, but does not include
         // any error handling.  The transfer can be aborted or rescheduled,
         // if desired.
         Error_Flag |= 0x08;
      }

      // When the Master enters the ISR, the SPI0CN_SPIF flag should be set from
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
      if (state == 0)
      {
         switch (Command)
         {
            case SLAVE_LED_ON:
            case SLAVE_LED_OFF:
               SPI0CN_NSSMD0 = 1;      // Release the slave (not expecting
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

               SPI0DAT = 0xFF;         // Send a dummy byte so the Slave can
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
               Error_Flag |= 0x10;
               state = 2;              // Any errors in the Command parsing
                                       // should go to state 2 where SPI0CN_NSSMD0
                                       // is de-asserted
         }
      }
      else if (state == 1)             // This state is for READ_ARRAY
      {                                // commands where the data must be read
                                       // after the first dummy byte is sent
         switch (Command)
         {
            case SPI_READ_BUFFER:
               SPI_Data_Array[array_index] = SPI0DAT;
               SPI0DAT = 0xFF;

               array_index++;

               if (array_index == (MAX_BUFFER_SIZE - 1))
               {
                  state = 2;
               }

               break;
            default:
               Error_Flag |= 0x10;
               state = 2;              // Any errors in the Command parsing
                                       // should go to state 2 where SPI0CN_NSSMD0
                                       // is de-asserted
         }
      }
      else if (state == 2)
      {
         switch (Command)
         {
            case SPI_READ:
               SPI_Data = SPI0DAT;     // Read the data from the slave

               break;

            case SPI_READ_BUFFER:
               SPI_Data_Array[array_index] = SPI0DAT; // Read the last data
                                                      // without sending a
                                                      // dummy byte

               break;
         }

         SPI0CN_NSSMD0 = 1;            // De-select the Slave

         state = 0;                    // Reset the state
      }

      SPI0CN_SPIF = 0;                 // Clear the SPI0CN_SPIF flag
   }
}

//-----------------------------------------------------------------------------
// Support Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SPI_LED_On
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Turns the LED on the SPI Slave on.  The slave does not respond to this
// command, so the command consists of:
//
// Command = SLAVE_LED_ON
// Length = 1 byte (the command itself)
//
//-----------------------------------------------------------------------------
void SPI_LED_On (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE;

   while (!SPI0CN_NSSMD0);             // Wait until the SPI is free, in case
                                       // it's already busy

   SPI0CN_NSSMD0 = 0;

   Command = SLAVE_LED_ON;

   SPI0DAT = Command;

   // The rest of this command will be handled by the SPI ISR, which will
   // trigger when SPI0CN_SPIF is set from sending the Command

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// SPI_LED_Off
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Turns the LED on the SPI Slave off.  The slave does not respond to this
// command, so the command consists of:
//
// Command = SLAVE_LED_OFF
// Length = 1 byte (the command itself)
//
//-----------------------------------------------------------------------------
void SPI_LED_Off (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE;

   while (!SPI0CN_NSSMD0);             // Wait until the SPI is free, in case
                                       // it's already busy

   SPI0CN_NSSMD0 = 0;

   Command = SLAVE_LED_OFF;

   SPI0DAT = Command;

   // The rest of this command will be handled by the SPI ISR, which will
   // trigger when SPI0CN_SPIF is set from sending the Command

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// SPI_Byte_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Note: SPI_Data must contain the data to be sent before calling this
// function.
//
// Writes a single byte to the SPI Slave.  The slave does not respond to this
// command, so the command consists of:
//
// Command = SPI_WRITE
// Length = 1 byte of command, 1 byte of data
//
//-----------------------------------------------------------------------------
void SPI_Byte_Write (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE;

   while (!SPI0CN_NSSMD0);             // Wait until the SPI is free, in case
                                       // it's already busy

   SPI0CN_NSSMD0 = 0;

   Command = SPI_WRITE;

   SPI0DAT = Command;

   // The rest of this command will be handled by the SPI ISR, which will
   // trigger when SPI0CN_SPIF is set from sending the Command

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// SPI_Byte_Read
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Note: SPI_Data will contain the data received after calling this function.
//
// Reads a single byte from the SPI Slave.  The command consists of:
//
// Command = SPI_READ
// Length = 1 byte of command, 1 byte of data
//
//-----------------------------------------------------------------------------
void SPI_Byte_Read (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE;

   while (!SPI0CN_NSSMD0);             // Wait until the SPI is free, in case
                                       // it's already busy

   SPI0CN_NSSMD0 = 0;

   Command = SPI_READ;

   SPI0DAT = Command;

   // The rest of this command will be handled by the SPI ISR, which will
   // trigger when SPI0CN_SPIF is set from sending the Command

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// SPI_Array_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Note: SPI_Data_Array must contain the data to be sent before calling this
// function.
//
// Writes an array of values of size MAX_BUFFER_SIZE to the SPI Slave.  The
// command consists of:
//
// Command = SPI_WRITE_BUFFER
// Length = 1 byte of command, MAX_BUFFER_SIZE bytes of data
//
// Note: Polled mode is used for this function in order to buffer the data
// being sent using the SPI0CN_TXBMT flag.
//
//-----------------------------------------------------------------------------
void SPI_Array_Write (void)
{
   uint8_t array_index;

   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE;

   while (!SPI0CN_NSSMD0);             // Wait until the SPI is free, in case
                                       // it's already busy

   IE_ESPI0 = 0;                       // Disable SPI interrupts

   SPI0CN_NSSMD0 = 0;

   SPI0DAT = SPI_WRITE_BUFFER;         // Load the XMIT register
   while (SPI0CN_TXBMT != 1)           // Wait until the command is moved into
   {                                   // the XMIT buffer
   }

   for (array_index = 0; array_index < MAX_BUFFER_SIZE; array_index++)
   {
      SPI0DAT = SPI_Data_Array[array_index]; // Load the data into the buffer
      while (SPI0CN_TXBMT != 1)        // Wait until the data is moved into
      {                                // the XMIT buffer
      }
   }
   SPI0CN_SPIF = 0;
   while (SPI0CN_SPIF != 1)            // Wait until the last byte of the
   {                                   // data reaches the Slave
   }
   SPI0CN_SPIF = 0;

   SPI0CN_NSSMD0 = 1;                  // Diable the Slave

   IE_ESPI0 = 1;                       // Re-enable SPI interrupts

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// SPI_Array_Read
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Note: SPI_Data_Array will contain the data received after calling this
// function.
//
// Reads a single byte from the SPI Slave.  The command consists of:
//
// Command = SPI_READ_BUFFER
// Length = 1 byte of command, MAX_BUFFER_SIZE bytes of data
//
//-----------------------------------------------------------------------------
void SPI_Array_Read (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE;

   while (!SPI0CN_NSSMD0);             // Wait until the SPI is free, in case
                                       // it's already busy

   SPI0CN_NSSMD0 = 0;

   Command = SPI_READ_BUFFER;

   SPI0DAT = Command;

   // The rest of this command will be handled by the SPI ISR, which will
   // trigger when SPI0CN_SPIF is set from sending the Command

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Delay
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Delay for little while (used for blinking the LEDs)
//
//-----------------------------------------------------------------------------
void Delay (void)
{
   uint32_t count;

   for (count = 200000; count > 0; count--);
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
