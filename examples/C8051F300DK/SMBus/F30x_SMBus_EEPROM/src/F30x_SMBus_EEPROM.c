//-----------------------------------------------------------------------------
// F30x_SMBus_EEPROM.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates how the C8051F30x SMBus interface can communicate
// with a 256 byte I2C Serial EEPROM (Microchip 24LC02B).
// - Interrupt-driven SMBus implementation
// - Only master states defined (no slave or arbitration)
// - Timer1 used as SMBus clock source
// - Timer2 used by SMBus for SCL low timeout detection
// - SCL frequency defined by <SMB_FREQUENCY> constant
// - Pinout:
//    P0.0 -> SDA (SMBus)
//    P0.1 -> SCL (SMBus)
//    P0.2 -> LED
//
//    P0.7 -> C2D (debug interface)
//
//    all other port pins unused
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J3: P0.2_LED
// 2) Connect the USB Debug Adapter to J4.
// 3) Connect the 9V power adapter to P1.
// 4) Connect the device to a Microchip 24LC02B EEPROM. Using an 8 pin
//    DIP package, the connections are as follows: EEPROM <-> F330
//       - Pin 1 - 3: unconnected
//       - Pin 4: Vss -> GND
//       - Pin 5: SDA <-> P0.0
//       - Pin 6: SCL <-> P0.1
//       - Pin 7: WP -> GND
//       - Pin 8: Vcc -> +3VD
// 5) Compile and download code to a 'F300 device on a C8051F300-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8:
//         a) the test will indicate proper communication with the EEPROM by
//            turning on the LED at the end the end of the test
//         b) the test can also be verified by running to the if statements
//            in main and checking the sent and received values by adding
//            the variables to the Watch Window
//
// Target:         C8051F300
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 code with compiler agnostic code
//    - 09 JAN 2014
//
// Release 1.0
//    -Initial Revision (TP)
//    -01 MAR 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F300_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define  SYSCLK         24500000       // System clock frequency in Hz

#define  SMB_FREQUENCY  10000          // Target SCL clock rate
                                       // This example supports between 10kHz
                                       // and 100kHz

#define  WRITE          0x00           // SMBus WRITE command
#define  READ           0x01           // SMBus READ command
#define LED_ON           1             // Turns the LED on
#define LED_OFF          0             // Turns the LED off

// Device addresses (7 bits, lsb is a don't care)
#define  EEPROM_ADDR    0xA0           // Device address for slave target
                                       // Note: This address is specified
                                       // in the Microchip 24LC02B
                                       // datasheet.
// SMBus Buffer Size
#define  SMB_BUFF_SIZE  0x08           // Defines the maximum number of bytes
                                       // that can be sent or received in a
                                       // single transfer

// Status vector - top 4 bits only
#define  SMB_MTSTA      0xE0           // (MT) start transmitted
#define  SMB_MTDB       0xC0           // (MT) data byte transmitted
#define  SMB_MRDB       0x80           // (MR) data byte received
// End status vector definition

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
uint8_t * pSMB_DATA_IN;                      // Global pointer for SMBus data
                                       // All receive data is written here

uint8_t SMB_SINGLEBYTE_OUT;                 // Global holder for single byte writes.

uint8_t * pSMB_DATA_OUT;                     // Global pointer for SMBus data.
                                       // All transmit data is read from here

uint8_t SMB_DATA_LEN;                       // Global holder for number of bytes
                                       // to send or receive in the current
                                       // SMBus transfer.

uint8_t WORD_ADDR;                          // Global holder for the EEPROM word
                                       // address that will be accessed in
                                       // the next transfer

uint8_t TARGET;                             // Target SMBus slave address

volatile bit SMB_BUSY = 0;             // Software flag to indicate when the
                                       // EEPROM_ByteRead() or
                                       // EEPROM_ByteWrite()
                                       // functions have claimed the SMBus

volatile bit SMB_RW;                   // Software flag to indicate the
                                       // direction of the current transfer

volatile bit SMB_SENDWORDADDR;         // When set, this flag causes the ISR
                                       // to send the 8-bit <WORD_ADDR>
                                       // after sending the slave address.

volatile bit SMB_RANDOMREAD;           // When set, this flag causes the ISR
                                       // to send a START signal after sending
                                       // the word address.
                                       // For the 24LC02B EEPROM, a random read
                                       // (a read from a particular address in
                                       // memory) starts as a write then
                                       // changes to a read after the repeated
                                       // start is sent. The ISR handles this
                                       // switchover if the <SMB_RANDOMREAD>
                                       // bit is set.

volatile bit SMB_ACKPOLL;              // When set, this flag causes the ISR
                                       // to send a repeated START until the
                                       // slave has acknowledged its address

// 16-bit SFR declarations
SI_SBIT(LED, SFR_P0, 2);
SI_SBIT(SDA, SFR_P0, 0);                 // SMBus on P0.0
SI_SBIT(SCL, SFR_P0, 1);                 // and P0.1

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(TIMER2_ISR, TIMER2_IRQn);
SI_INTERRUPT_PROTO(SMBUS0_ISR, SMBUS0_IRQn);

void SMBus_Init(void);
void Timer1_Init(void);
void Timer2_Init(void);
void Port_Init(void);

void EEPROM_ByteWrite(uint8_t addr, uint8_t dat);
void EEPROM_WriteArray(uint8_t dest_addr, uint8_t * src_addr,
                       uint8_t len);
uint8_t EEPROM_ByteRead(uint8_t addr);
void EEPROM_ReadArray(uint8_t * dest_addr, uint8_t src_addr,
                      uint8_t len);

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
   PCA0MD &= ~0x40;                    // WDTE = 0 (disable watchdog timer)
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, then loops forever sending
// and receiving SMBus data to the slave EEPROM.

void main (void)
{
   char in_buff[8] = {0};              // Incoming data buffer
   char out_buff[8] = "ABCDEFG";       // Outgoing data buffer

   uint8_t temp_char;                                // Temporary variable
   bit error_flag = 0;                 // Flag for checking EEPROM contents
   uint8_t i;                                        // Temporary counter variable


   // Set internal oscillator to highest
   // setting of 24500000
   OSCICN |= 0x03;

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

   Port_Init ();                       // Initialize Crossbar and GPIO

   // Turn off the LED before the test starts
   LED = LED_OFF;                   // Turn off the LED before the test

   Timer1_Init ();                     // Configure Timer1 for use as SMBus
                                       // clock source

   Timer2_Init ();                     // Configure Timer2 for use with SMBus
                                       // low timeout detect

   SMBus_Init ();                      // Configure and enable SMBus

   EIE1 |= 0x01;                       // Enable the SMBus interrupt

   IE_EA = 1;                             // Global interrupt enable

   // Read and write some bytes to the EEPROM and check for proper
   // communication

   // Write the value 0xAA to location 0x25 in the EEPROM
   EEPROM_ByteWrite(0x25, 0xAA);

   // Read the value at location 0x25 in the EEPROM
   temp_char = EEPROM_ByteRead(0x25);

   // Check that the data was read properly
   if (temp_char != 0xAA)
   {
      error_flag = 1;
   }

   // Write the value 0xBB to location 0x25 in the EEPROM
   EEPROM_ByteWrite(0x25, 0xBB);

   // Write the value 0xCC to location 0x38 in the EEPROM
   EEPROM_ByteWrite(0x38, 0xCC);

   // Read the value at location 0x25 in the EEPROM
   temp_char = EEPROM_ByteRead(0x25);

   // Check that the data was read properly
   if (temp_char != 0xBB)
   {
      error_flag = 1;
   }

   // Read the value at location 0x38 in the EEPROM
   temp_char = EEPROM_ByteRead(0x38);

   // Check that the data was read properly
   if (temp_char != 0xCC)
   {
      error_flag = 1;
   }

   // Store the outgoing data buffer at EEPROM address 0x50
   EEPROM_WriteArray(0x50, out_buff, sizeof(out_buff));

   // Fill the incoming data buffer with data starting at EEPROM address 0x50
   EEPROM_ReadArray(in_buff, 0x50, sizeof(in_buff));

   // Check that the data that came from the EEPROM is the same as what was
   // sent
   for (i = 0; i < sizeof(in_buff); i++)
   {
      if (in_buff[i] != out_buff[i])
      {
         error_flag = 1;
      }
   }

   // Indicate communication is good
   if (error_flag == 0)
   {
      // LED = ON indicates that the test passed
      LED = LED_ON;
   }

   while(1);

}

//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SMBus_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// The SMBus peripheral is configured as follows:
// - SMBus enabled
// - Slave mode disabled
// - Timer1 used as clock source. The maximum SCL frequency will be
//   approximately 1/3 the Timer1 overflow rate
// - Setup and hold time extensions enabled
// - Free and SCL low timeout detection enabled
//
void SMBus_Init (void)
{
   SMB0CF = 0x5D;                      // Use Timer1 overflows as SMBus clock
                                       // source;
                                       // Disable slave mode;
                                       // Enable setup & hold time extensions;
                                       // Enable SMBus Free timeout detect;
                                       // Enable SCL low timeout detect;

   SMB0CF |= 0x80;                     // Enable SMBus;
}

//-----------------------------------------------------------------------------
// Timer1_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Timer1 is configured as the SMBus clock source as follows:
// - Timer1 in 8-bit auto-reload mode
// - SYSCLK / 12 as Timer1 clock source
// - Timer1 overflow rate => 3 * SMB_FREQUENCY
// - The maximum SCL clock rate will be ~1/3 the Timer1 overflow rate
// - Timer1 enabled
//
void Timer1_Init (void)
{
// Make sure the Timer can produce the appropriate frequency in 8-bit mode
// Supported SMBus Frequencies range from 10kHz to 100kHz.  The CKCON register
// settings may need to change for frequencies outside this range.
#if ((SYSCLK/SMB_FREQUENCY/3) < 255)
   #define SCALE 1
      CKCON |= 0x10;                   // Timer1 clock source = SYSCLK
#elif ((SYSCLK/SMB_FREQUENCY/4/3) < 255)
   #define SCALE 4
      CKCON |= 0x01;
      CKCON &= ~0x12;                  // Timer1 clock source = SYSCLK / 4
#endif

   TMOD = 0x20;                        // Timer1 in 8-bit auto-reload mode

   TH1 = -(SYSCLK/SMB_FREQUENCY/SCALE/3); // Timer1 configured to overflow at 1/3
                                       // the rate defined by SMB_FREQUENCY

   TL1 = TH1;                          // Init Timer1

   TCON_TR1 = 1;                            // Timer1 enabled
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Timer2 configured for use by the SMBus low timeout detect feature as
// follows:
// - Timer2 in 16-bit auto-reload mode
// - SYSCLK/12 as Timer2 clock source
// - Timer2 reload registers loaded for a 25ms overflow period
// - Timer2 pre-loaded to overflow after 25ms
// - Timer2 enabled
//
void Timer2_Init (void)
{
   TMR2CN = 0x00;                      // Timer2 configured for 16-bit auto-
                                       // reload, low-byte interrupt disabled

   CKCON &= ~0x10;                     // Timer2 uses SYSCLK/12

   TMR2RL = -(SYSCLK/12/40);           // Timer2 configured to overflow after
   TMR2 = TMR2RL;                      // ~25ms (for SMBus low timeout detect)

   IE |= 0x20;                         // Timer2 interrupt enable
   TMR2CN_TR2 = 1;                            // Start Timer2
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports.
//
// P0.0   digital   open-drain    SMBus SDA
// P0.1   digital   open-drain    SMBus SCL
// P0.2   digital   push-pull     LED
//
// all other port pins unused
//
// Note: If the SMBus is moved, the SCL and SDA sbit declarations must also
// be adjusted.
//
void PORT_Init (void)
{
   P0MDOUT = 0x00;                     // All P0 pins open-drain output

   P0MDOUT |= 0x04;                    // Make the LED (P0.2) a push-pull
                                       // output

   XBR1 = 0x04;                        // Enable SMBus
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups

   P0 = 0xFF;
}

//-----------------------------------------------------------------------------
// SMBus Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// SMBus ISR state machine
// - Master only implementation - no slave or arbitration states defined
// - All incoming data is written starting at the global pointer <pSMB_DATA_IN>
// - All outgoing data is read from the global pointer <pSMB_DATA_OUT>
//
SI_INTERRUPT(SMBUS0_ISR, SMBUS0_IRQn)
{
   static bit FAIL = 0;                // Used by the ISR to flag failed
                                       // transfers

   static uint8_t i;             // Used by the ISR to count the
                                       // number of data bytes sent or
                                       // received

   static bit SEND_START = 0;          // Send a start

   switch (SMB0CN & 0xF0)              // Status vector
   {
      // Master Transmitter/Receiver: START condition transmitted.
      case SMB_MTSTA:
         SMB0DAT = TARGET;             // Load address of the target slave
         SMB0DAT &= 0xFE;              // Clear the LSB of the address for the
                                       // R/W bit
         SMB0DAT |= (uint8_t) SMB_RW;       // Load R/W bit
         SMB0CN_STA = 0;                      // Manually clear START bit
         i = 0;                        // Reset data byte counter
         break;

      // Master Transmitter: Data byte (or Slave Address) transmitted
      case SMB_MTDB:
         if (SMB0CN_ACK)                      // Slave Address or Data Byte
         {                             // Acknowledged?
            if (SEND_START)
            {
               SMB0CN_STA = 1;
               SEND_START = 0;
               break;
            }
            if(SMB_SENDWORDADDR)       // Are we sending the word address?
            {
               SMB_SENDWORDADDR = 0;   // Clear flag
               SMB0DAT = WORD_ADDR;    // Send word address

               if (SMB_RANDOMREAD)
               {
                  SEND_START = 1;      // Send a START after the next SMB0CN_ACK cycle
                  SMB_RW = READ;
               }

               break;
            }

            if (SMB_RW==WRITE)         // Is this transfer a WRITE?
            {

               if (i < SMB_DATA_LEN)   // Is there data to send?
               {
                  // send data byte
                  SMB0DAT = *pSMB_DATA_OUT;

                  // increment data out pointer
                  pSMB_DATA_OUT++;

                  // increment number of bytes sent
                  i++;
               }
               else
               {
                 SMB0CN_STO = 1;              // Set SMB0CN_STO to terminte transfer
                 SMB_BUSY = 0;         // Clear software busy flag
               }
            }
            else {}                    // If this transfer is a READ,
                                       // then take no action. Slave
                                       // address was transmitted. A
                                       // separate 'case' is defined
                                       // for data byte recieved.
         }
         else                          // If slave NACK,
         {
            if(SMB_ACKPOLL)
            {
               SMB0CN_STA = 1;                // Restart transfer
            }
            else
            {
               FAIL = 1;               // Indicate failed transfer
            }                          // and handle at end of ISR
         }
         break;

      // Master Receiver: byte received
      case SMB_MRDB:
         if ( i < SMB_DATA_LEN )       // Is there any data remaining?
         {
            *pSMB_DATA_IN = SMB0DAT;   // Store received byte
            pSMB_DATA_IN++;            // Increment data in pointer
            i++;                       // Increment number of bytes received
            SMB0CN_ACK = 1;                   // Set SMB0CN_ACK bit (may be cleared later
                                       // in the code)

         }

         if (i == SMB_DATA_LEN)        // This is the last byte
         {
            SMB_BUSY = 0;              // Free SMBus interface
            SMB0CN_ACK = 0;                   // Send NACK to indicate last byte
                                       // of this transfer
            SMB0CN_STO = 1;                   // Send STOP to terminate transfer
         }

         break;

      default:
         FAIL = 1;                     // Indicate failed transfer
                                       // and handle at end of ISR
         break;
   }

   if (FAIL)                           // If the transfer failed,
   {
      SMB0CF &= ~0x80;                 // Reset communication
      SMB0CF |= 0x80;
      SMB0CN_STA = 0;
      SMB0CN_STO = 0;
      SMB0CN_ACK = 0;

      SMB_BUSY = 0;                    // Free SMBus

      FAIL = 0;
   }

   SMB0CN_SI = 0;                             // Clear interrupt flag
}

//-----------------------------------------------------------------------------
// Timer2 Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// A Timer2 interrupt indicates an SMBus SCL low timeout.
// The SMBus is disabled and re-enabled if a timeout occurs.
//
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
   SMB0CF &= ~0x80;                    // Disable SMBus
   SMB0CF |= 0x80;                     // Re-enable SMBus
   TMR2CN_TF2H = 0;                           // Clear Timer2 interrupt-pending flag
   SMB_BUSY = 0;                       // Free bus
}

//-----------------------------------------------------------------------------
// Support Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// EEPROM_ByteWrite
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint8_t addr - address to write in the EEPROM
//                        range is full range of character: 0 to 255
//
//   2) uint8_t dat - data to write to the address <addr> in the EEPROM
//                        range is full range of character: 0 to 255
//
// This function writes the value in <dat> to location <addr> in the EEPROM
// then polls the EEPROM until the write is complete.
//
void EEPROM_ByteWrite(uint8_t addr, uint8_t dat)
{
   while (SMB_BUSY);                   // Wait for SMBus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)

   // Set SMBus ISR parameters
   TARGET = EEPROM_ADDR;               // Set target slave address
   SMB_RW = WRITE;                     // Mark next transfer as a write
   SMB_SENDWORDADDR = 1;               // Send Word Address after Slave Address
   SMB_RANDOMREAD = 0;                 // Do not send a START signal after
                                       // the word address
   SMB_ACKPOLL = 1;                    // Enable Acknowledge Polling (The ISR
                                       // will automatically restart the
                                       // transfer if the slave does not
                                       // acknoledge its address.

   // Specify the Outgoing Data
   WORD_ADDR = addr;                   // Set the target address in the
                                       // EEPROM's internal memory space

   SMB_SINGLEBYTE_OUT = dat;           // Store <dat> (local variable) in a
                                       // global variable so the ISR can read
                                       // it after this function exits

   // The outgoing data pointer points to the <dat> variable
   pSMB_DATA_OUT = &SMB_SINGLEBYTE_OUT;

   SMB_DATA_LEN = 1;                   // Specify to ISR that the next transfer
                                       // will contain one data byte

   // Initiate SMBus Transfer
   SMB0CN_STA = 1;

}

//-----------------------------------------------------------------------------
// EEPROM_WriteArray
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint8_t dest_addr - beginning address to write to in the EEPROM
//                        range is full range of character: 0 to 255
//
//   2) uint8_t* src_addr - pointer to the array of data to be written
//                        range is full range of character: 0 to 255
//
//   3) uint8_t len - length of the array to be written to the EEPROM
//                        range is full range of character: 0 to 255
//
// Writes <len> data bytes to the EEPROM slave specified by the <EEPROM_ADDR>
// constant.
//
void EEPROM_WriteArray(uint8_t dest_addr, uint8_t * src_addr, uint8_t len)
{
   uint8_t i;
   uint8_t * pData = (uint8_t*) src_addr;

   for( i = 0; i < len; i++ ){
      EEPROM_ByteWrite(dest_addr++, *pData++);
   }

}

//-----------------------------------------------------------------------------
// EEPROM_ByteRead
//-----------------------------------------------------------------------------
//
// Return Value :
//   1) uint8_t data - data read from address <addr> in the EEPROM
//                        range is full range of character: 0 to 255
//
// Parameters   :
//   1) uint8_t addr - address to read data from the EEPROM
//                        range is full range of character: 0 to 255
//
// This function returns a single byte from location <addr> in the EEPROM then
// polls the <SMB_BUSY> flag until the read is complete.
//
uint8_t EEPROM_ByteRead(uint8_t addr)
{
   uint8_t retval;               // Holds the return value

   while (SMB_BUSY);                   // Wait for SMBus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)

   // Set SMBus ISR parameters
   TARGET = EEPROM_ADDR;               // Set target slave address
   SMB_RW = WRITE;                     // A random read starts as a write
                                       // then changes to a read after
                                       // the repeated start is sent. The
                                       // ISR handles this switchover if
                                       // the <SMB_RANDOMREAD> bit is set.
   SMB_SENDWORDADDR = 1;               // Send Word Address after Slave Address
   SMB_RANDOMREAD = 1;                 // Send a START after the word address
   SMB_ACKPOLL = 1;                    // Enable Acknowledge Polling

   // Specify the Incoming Data
   WORD_ADDR = addr;                   // Set the target address in the
                                       // EEPROM's internal memory space

   pSMB_DATA_IN = &retval;             // The incoming data pointer points to
                                       // the <retval> variable.

   SMB_DATA_LEN = 1;                   // Specify to ISR that the next transfer
                                       // will contain one data byte

   // Initiate SMBus Transfer
   SMB0CN_STA = 1;
   while(SMB_BUSY);                    // Wait until data is read

   return retval;

}

//-----------------------------------------------------------------------------
// EEPROM_ReadArray
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint8_t* dest_addr - pointer to the array that will be filled
//                                 with the data from the EEPROM
//                        range is full range of character: 0 to 255
//
//   2) uint8_t src_addr - beginning address to read data from the EEPROM
//                        range is full range of character: 0 to 255
//
//   3) uint8_t len - length of the array to be read from the EEPROM
//                        range is full range of character: 0 to 255
//
// Reads up to 256 data bytes from the EEPROM slave specified by the
// <EEPROM_ADDR> constant.
//
void EEPROM_ReadArray (uint8_t * dest_addr, uint8_t src_addr, uint8_t len)
{
   while (SMB_BUSY);                   // Wait for SMBus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)

   // Set SMBus ISR parameters
   TARGET = EEPROM_ADDR;               // Set target slave address
   SMB_RW = WRITE;                     // A random read starts as a write
                                       // then changes to a read after
                                       // the repeated start is sent. The
                                       // ISR handles this switchover if
                                       // the <SMB_RANDOMREAD> bit is set.
   SMB_SENDWORDADDR = 1;               // Send Word Address after Slave Address
   SMB_RANDOMREAD = 1;                 // Send a START after the word address
   SMB_ACKPOLL = 1;                    // Enable Acknowledge Polling

   // Specify the Incoming Data
   WORD_ADDR = src_addr;               // Set the target address in the
                                       // EEPROM's internal memory space

   // Set the the incoming data pointer
   pSMB_DATA_IN = (uint8_t*) dest_addr;

   SMB_DATA_LEN = len;                 // Specify to ISR that the next transfer
                                       // will contain <len> data bytes

   // Initiate SMBus Transfer
   SMB0CN_STA = 1;
   while(SMB_BUSY);                    // Wait until data is read

}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
