//-----------------------------------------------------------------------------
// F37x_EEPROM.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// 
// This program writes 0xFF to all 512 bytes of EEPROM and reads all 512 bytes
// for confirmation. Secondly, the program writes 0xAA to all 512 bytes and 
// reads all 512 bytes for confirmation. Lastly, the program writes 0x55 to all  
// 512 bytes and reads all 512 bytes for confirmation.
//
// How To Test:
//
// 1) Download code to the C8051F370 UDP MCU board
// 2) Run the program
//
//
// Target:         C8051F37x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.3
//    - Corrected SCL and SDA sbit defintions (ST)
//      SCL is P2.2 and SDA is P2.3 when Swap = 1 
//    - 19 DEC 2012
//
// Release 1.2
//    - Corrected references to SMB1CF in SMBus_Init (ST)
//    - 27 JUL 2012
//
// Release 1.1
//    - Added comments and formatting (ST)
//    - 15 JUN 2012
//
// Release 1.0
//    - Initial Revision (BD)
//

//-----------------------------------------------------------------------------
// Includes and Device-Specific Parameters
//-----------------------------------------------------------------------------

#include <SI_C8051F390_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------


#define  SYSCLK         24500000       // System clock frequency in Hz

#define  SMB_FREQUENCY  100000         // Target SCL clock rate

#define  WRITE          0x00           // SMBus WRITE command
#define  READ           0x01           // SMBus READ command

#define LED_ON           0             // Turns the LED on
#define LED_OFF          1             // Turns the LED off

// Device addresses (7 bits, lsb is a don't care)
#define  EEPROM_ADDR    0xA0           // Device address for slave target
                                       
#define  SMB_BUFF_SIZE  0x08           // Defines the maximum number of bytes
                                       // that can be sent or received in a
                                       // single transfer

// Status vector - top 4 bits only
#define  SMB_MTSTA      0xE0           // (MT) start transmitted
#define  SMB_MTDB       0xC0           // (MT) data byte transmitted
#define  SMB_MRDB       0x80           // (MR) data byte received
// End status vector definition

#define  UPPER_MEM_BANK 0x02           // Mask to access upper 256 bytes of
                                       // EEPROM.

#define SMBUS 1
#define Swap 1

#if (SMBUS == 0)
	#define INTERRUPT_MY_SMBUS 7
	#define INTERRUPT_MY_TIMER 14
#elif (SMBUS == 1)
	#define INTERRUPT_MY_SMBUS 15
	#define INTERRUPT_MY_TIMER 16
#endif 


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

uint8_t SI_SEG_XDATA SAMPLE_ARRAY[512];        // Test array for EEPROM

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


SI_SBIT(SCL, SFR_P2, 2);                 // SMBus on P2.2
SI_SBIT(SDA, SFR_P2, 3);                 // and P2.3



//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SMBus_Init(void);
void Timer0_Init(void);
void Port_Init(void);

SI_INTERRUPT_PROTO(TIMER3_ISR, INTERRUPT_MY_TIMER);
SI_INTERRUPT_PROTO(SMBUS0_ISR, INTERRUPT_MY_SMBUS);

void EEPROM_ByteWrite(uint16_t addr, uint8_t dat);
void EEPROM_WriteArray(uint16_t dest_addr, uint8_t * src_addr,
                       uint8_t len);
uint8_t EEPROM_ByteRead(uint16_t addr);
void EEPROM_ReadArray(uint8_t * dest_addr, uint16_t src_addr,
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
//
//-----------------------------------------------------------------------------

void main (void)
{
   char in_buff[8] = {0};              // Incoming data buffer
   char out_buff[8] = "ABCDEFG";       // Outgoing data buffer

   bit error_flag = 0;                 // Flag for checking EEPROM contents
   uint8_t i;                               // Temporary counter variable
   uint16_t j;                              // Temporary counter variable


   
	

   // Set internal oscillator to divide by 2 (24500000)
   OSCICN |= 0x03;
   CLKSEL = 0x00;
   
   #if (Swap == 0)
   #elif (Swap == 1 && SMBUS == 0)
   		SMBTC |= 0x40;
   #elif (Swap == 1 && SMBUS == 1)
   		SMBTC |= 0x80;
   #endif

   #if (SMBUS == 1)
   		SFRPAGE = 0xF;	
   #endif
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

   Timer0_Init ();                     // Configure Timer0 for use as SMBus
                                       // clock source

   SMBus_Init ();                      // Configure and enable SMBus

   #if (SMBUS == 0)
		   EIE1 |= 0x01;
   #elif (SMBUS == 1)
   		EIE2 |= 0x01;                 // Enable the SMBus interrupt
   #endif

   IE_EA = 1;                             // Global interrupt enable

   // TEST BEGIN***************************************************************

   // Write 0xFF to every byte of EEPROM
   for (j=0; j < 512; j++)
   {
      EEPROM_ByteWrite(j, (j & 0x0FF));
   }

   for (j=0; j < 512; j++)
   {
      SAMPLE_ARRAY[j] = EEPROM_ByteRead(j);
      if ( SAMPLE_ARRAY[j] != (j & 0x0FF))
	  {
         error_flag = 1;       
	  }
   }


   // Write 0xAA to every byte of EEPROM
   for (j=0; j < 512; j++)
   {
      EEPROM_ByteWrite(j, 0xAA);
   }

   for (j=0; j < 512; j++)
   {
      SAMPLE_ARRAY[j] = EEPROM_ByteRead(j);
      if ( SAMPLE_ARRAY[j] != 0xAA)
	  {
         error_flag = 1;       
	  }
   }   

   // Write 0x55 to every byte of EEPROM
   for (j=0; j < 512; j++)
   {
      EEPROM_ByteWrite(j, 0x55);
   }
    
   for (j=0; j < 512; j++)
   {
      SAMPLE_ARRAY[j] = EEPROM_ByteRead(j);
      if ( SAMPLE_ARRAY[j] != 0x55)
	  {
         error_flag = 1;       
	  }
   }   

   if (error_flag)
   {
      while(1);         // Test failed 
   }

   while(1);            // Test passed

   //TEST END******************************************************************

}


//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SMBus_Init()
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
//-----------------------------------------------------------------------------
void SMBus_Init (void)
{
   unsigned char SFRPAGEsave = SFRPAGE;
   #if (SMBUS == 0)
   SFRPAGE = 0x00;
   SMB0CF = 0x54;                      // Use Timer0 overflows as SMBus clock
                                       // source;
                                       // Disable slave mode;
                                       // Enable setup & hold time extensions;
                                       // Enable SMBus Free timeout detect;
                                       // Disable SCL low timeout detect;

   SMB0CF |= 0x80;                     // Enable SMBus;
   #elif (SMBUS == 1)
   SFRPAGE = 0x0F;
   SMB1CF = 0x54;                      // Use Timer0 overflows as SMBus clock
                                       // source;
                                       // Disable slave mode;
                                       // Enable setup & hold time extensions;
                                       // Enable SMBus Free timeout detect;
                                       // Disable SCL low timeout detect;

   SMB1CF |= 0x80;                     // Enable SMBus;
   #endif
   SFRPAGE = SFRPAGEsave;
}

//-----------------------------------------------------------------------------
// Timer0_Init()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Timer0 is configured as the SMBus clock source as follows:
// - Timer0 in 8-bit auto-reload mode
// - SYSCLK / 12 as Timer0 clock source
// - Timer0 overflow rate => 3 * SMB_FREQUENCY
// - The maximum SCL clock rate will be ~1/3 the Timer0 overflow rate
// - Timer0 enabled
//
//-----------------------------------------------------------------------------
void Timer0_Init (void)
{
// Make sure the Timer can produce the appropriate frequency in 8-bit mode
// Supported SMBus Frequencies range from 10kHz to 100kHz.  The CKCON register
// settings may need to change for frequencies outside this range.
	
	unsigned char SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE = 0x0;


		#if ((SYSCLK/SMB_FREQUENCY/3) < 255)
		   #define SCALE 1
		      CKCON |= 0x04;                   // Timer0 clock source = SYSCLK
		#elif ((SYSCLK/SMB_FREQUENCY/4/3) < 255)
		   #define SCALE 4
		      CKCON |= 0x01;
		      CKCON &= ~0x06;                  // Timer0 clock source = SYSCLK / 4
		#endif

		TMOD = 0x02;                           // Timer0 in 8-bit auto-reload mode

		TH0 = -(SYSCLK/SMB_FREQUENCY/SCALE/3); // Timer0 configured to overflow at 1/3
		                                       // the rate defined by SMB_FREQUENCY

		TL0 = TH0;                             // Init Timer0

		TCON_TR0 = 1;                               // Timer0 enabled
      SFRPAGE = SFRPAGE_SAVE;

}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports.
//
// P2.2   digital   open-drain    SMBus SCL
// P2.3   digital   open-drain    SMBus SDA
//
// all other port pins unused
//
// Note: If the SMBus is moved, the SCL and SDA sbit declarations must also
// be adjusted.
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{


   P0SKIP    = 0xFF;
   P1SKIP    = 0xFF;
   P2SKIP 	 = 0x03;


   #if (SMBUS == 0)
   XBR0 = 0x84;
   #elif (SMBUS == 1)
   XBR0 = 0xC0;                        // Enable SMBus pins and EE Pullup
   #endif

   XBR1 = 0x40;                        // Enable crossbar and weak pull-ups
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
//-----------------------------------------------------------------------------
SI_INTERRUPT(SMBUS0_ISR, INTERRUPT_MY_SMBUS)
{
   static bit FAIL = 0;                // Used by the ISR to flag failed
                                       // transfers

   static unsigned char i;             // Used by the ISR to count the
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
         SMB0DAT |= SMB_RW;            // Load R/W bit
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
                                       // and handle at end of ISR;
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
// Support Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// EEPROM_ByteWrite ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint16_t addr - address to write in the EEPROM
//                        range is: 0 to 512
//
//   2) uint8_t dat - data to write to the address <addr> in the EEPROM
//                        range is full range of character: 0 to 255
//
// This function writes the value in <dat> to location <addr> in the EEPROM
// then polls the EEPROM until the write is complete.
//
//-----------------------------------------------------------------------------
void EEPROM_ByteWrite(uint16_t addr, uint8_t dat)
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

   if ((addr & 0x100) == 0x100)        // Check to see if accessing upper 256
                                       // bytes of EEPROM
   {                                   
      WORD_ADDR = (addr & 0x0FF);      // Set the target address of EEPROM
      TARGET |= UPPER_MEM_BANK;        // Set bit to access upper 256 bytes of
   }                                   // EEPROM
   else
   {
      WORD_ADDR = addr;                // Set the target address in the
   }                                   // EEPROM's internal memory space

   // Specify the Outgoing Data
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
// EEPROM_WriteArray ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint16_t dest_addr - beginning address to write to in the EEPROM
//                        range is: 0 to 512
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
//-----------------------------------------------------------------------------
void EEPROM_WriteArray(uint16_t dest_addr, uint8_t * src_addr, uint8_t len)
{
   uint8_t i;
   uint8_t * pData = (uint8_t*) src_addr;

   for( i = 0; i < len; i++ ){
      EEPROM_ByteWrite(dest_addr++, *pData++);
   }

}
 
//-----------------------------------------------------------------------------
// EEPROM_ByteRead ()
//-----------------------------------------------------------------------------
//
// Return Value :
//   1) uint8_t data - data read from address <addr> in the EEPROM
//                        range is full range of character: 0 to 255
//
// Parameters   :
//   1) uint16_t addr - address to read data from the EEPROM
//                        range is: 0 to 512
//
// This function returns a single byte from location <addr> in the EEPROM then
// polls the <SMB_BUSY> flag until the read is complete.
//
//-----------------------------------------------------------------------------
uint8_t EEPROM_ByteRead(uint16_t addr)
{
   uint8_t retval;                          // Holds the return value

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
   if ((addr & 0x100) == 0x100)        // Check to see if accessing upper 256
                                       // bytes of EEPROM
   {                                   
      WORD_ADDR = (addr & 0x0FF);      // Set the target address of EEPROM
      TARGET |= UPPER_MEM_BANK;        // Set bit to access upper 256 bytes of
   }                                   // EEPROM
   else
   {
      WORD_ADDR = addr;                // Set the target address in the
   }                                   // EEPROM's internal memory space   
   
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
// EEPROM_ReadArray ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint8_t* dest_addr - pointer to the array that will be filled
//                                 with the data from the EEPROM
//                        range is full range of character: 0 to 255
//
//   2) uint16_t src_addr - beginning address to read data from the EEPROM
//                        range is: 0 to 512
//
//   3) uint8_t len - length of the array to be read from the EEPROM
//                        range is: 0 to 255
//
// Reads up to 256 data bytes from the EEPROM slave specified by the
// <EEPROM_ADDR> constant.
//
//-----------------------------------------------------------------------------
void EEPROM_ReadArray (uint8_t * dest_addr, uint16_t src_addr, uint8_t len)
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
   if ((src_addr & 0x100) == 0x100)    // Check to see if accessing upper 256
                                       // bytes of EEPROM
   {                                   
      WORD_ADDR = (src_addr & 0x0FF);  // Set the target address of EEPROM
      TARGET |= UPPER_MEM_BANK;        // Set bit to access upper 256 bytes of
   }                                   // EEPROM
   else
   {
      WORD_ADDR = src_addr;             // Set the target address in the
   } 

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
