//-----------------------------------------------------------------------------
// F36x_SMBus_Slave_Multibyte.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Example software to demonstrate the C8051F36x SMBus interface in Slave mode
// - Interrupt-driven SMBus implementation
// - Only slave states defined
// - Multi-byte SMBus data holders used for both transmit and receive
// - Timer1 used as SMBus clock rate (used only for free timeout detection)
// - Timer3 used by SMBus for SCL low timeout detection
// - ARBLOST support included
// - supports multiple-byte writes and multiple-byte reads
// - Pinout:
//    P0.0 -> SDA (SMBus)
//    P0.1 -> SCL (SMBus)
//
//    P3.2 -> LED
//
//    all other port pins unused
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: P3.2 - P3.2_LED
//    J1: Power selection, P2 9V, P4 USB or J9 Debugger
// 2) Connect the USB Debug Adapter to J9. If 9V power is selected on J1,
//    connect the 9V power adapter to P2. If VBUS is selected on J1, connect
//    USB cable to P4.
// 3) Connect SPI bus to a SMBus master.
// 4) Compile and download code to C8051F360 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 6) The slave code will copy the write data to the read
//    data, so a successive write and read will effectively echo the data
//    written. To verify that the code is working properly, verify on the
//    master that the data received is the same as the data written.
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
//    -16 OCT 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define  SYSCLK         24500000       // System clock frequency in Hz

#define  SMB_FREQUENCY  10000          // Target SMBus frequency
                                       // This example supports between 10kHz
                                       // and 100kHz

#define  WRITE          0x00           // SMBus WRITE command
#define  READ           0x01           // SMBus READ command

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

SI_SBIT(LED, SFR_P3, 2);                 // LED='1' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SMBus_Init (void);
void Timer1_Init (void);
void Timer3_Init (void);
void Port_Init (void);

SI_INTERRUPT_PROTO(SMBUS0_ISR, SMBUS0_IRQn);
SI_INTERRUPT_PROTO(Timer3_ISR, TIMER3_IRQn);

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // WDTE = 0 (Disable watchdog timer)
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


   SFRPAGE = CONFIG_PAGE;

   OSCICN |= OSCICN_IFCN__HFOSC_DIV_1; // Set internal oscillator to highest
                                       // setting of 24500000

   Port_Init();                        // Initialize Crossbar and GPIO
   Timer1_Init();                      // Configure Timer1 for use
                                       // with SMBus baud rate

   Timer3_Init ();                     // Configure Timer3 for use with
                                       // SCL low timeout detect

   SMBus_Init ();                      // Configure and enable SMBus

   EIE1 |= EIE1_ESMB0__ENABLED;        // Enable the SMBus interrupt

   LED = 0;

   IE_EA = 1;                          // Global interrupt enable

   SFRPAGE = LEGACY_PAGE;

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

      LED = ~LED;
   }
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
// SMBus configured as follows:
// - SMBus enabled
// - Slave mode not inhibited
// - Timer1 used as clock source. The maximum SCL frequency will be
//   approximately 1/3 the Timer1 overflow rate
// - Setup and hold time extensions enabled
// - Bus Free and SCL Low timeout detection enabled
//
//-----------------------------------------------------------------------------
void SMBus_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   SMB0CF = 0x1D;                      // Use Timer1 overflows as SMBus clock
                                       // source;
                                       // Enable slave mode;
                                       // Enable setup & hold time
                                       // extensions;
                                       // Enable SMBus Free timeout detect;
                                       // Enable SCL low timeout detect;

   SMB0CF |= SMB0CF_ENSMB__ENABLED;    // Enable SMBus

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Timer1_Init()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Timer1 configured as the SMBus clock source as follows:
// - Timer1 in 8-bit auto-reload mode
// - SYSCLK or SYSCLK / 4 as Timer1 clock source
// - Timer1 overflow rate => 3 * SMB_FREQUENCY
// - The resulting SCL clock rate will be ~1/3 the Timer1 overflow rate
// - Timer1 enabled
//
//-----------------------------------------------------------------------------
void Timer1_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

// Make sure the Timer can produce the appropriate frequency in 8-bit mode
// Supported SMBus Frequencies range from 10kHz to 100kHz.  The CKCON register
// settings may need to change for frequencies outside this range.
#if ((SYSCLK/SMB_FREQUENCY/3) < 255)
   #define SCALE 1
      CKCON |= CKCON_T1M__SYSCLK;      // Timer1 clock source = SYSCLK
#elif ((SYSCLK/SMB_FREQUENCY/4/3) < 255)
   #define SCALE 4
      CKCON &= ~CKCON_T1M__BMASK;
      CKCON |= CKCON_SCA__SYSCLK_DIV_4; // Timer1 clock source = SYSCLK / 4
#elif ((SYSCLK/SMB_FREQUENCY/12/3) < 255)
   #define SCALE 8
      CKCON &= ~0x0B;                  // Timer1 clock source = SYSCLK / 12
#elif ((SYSCLK/SMB_FREQUENCY/48/3) < 255)
   #define SCALE 4
      CKCON |= 0x02;
      CKCON &= ~0x09;                  // Timer1 clock source = SYSCLK / 48
#else
   #error "Error: Timer1 cannot produce the desired SMBus frequency"
#endif

   TMOD = TMOD_T1M__MODE2;             // Timer1 in 8-bit auto-reload mode

   // Timer1 configured to overflow at 1/3 the rate defined by SMB_FREQUENCY
   TH1 = -(SYSCLK/SMB_FREQUENCY/SCALE/3);

   TL1 = TH1;                          // Init Timer1

   TCON_TR1 = 1;                       // Timer1 enabled

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Timer3_Init()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Timer3 configured for use by the SMBus low timeout detect feature as
// follows:
// - Timer3 in 16-bit auto-reload mode
// - SYSCLK/12 as Timer3 clock source
// - Timer3 reload registers loaded for a 25ms overflow period
// - Timer3 pre-loaded to overflow after 25ms
// - Timer3 enabled
//
//-----------------------------------------------------------------------------
void Timer3_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   TMR3CN = 0x00;                      // Timer3 configured for 16-bit auto-
                                       // reload, low-byte interrupt disabled

   CKCON &= ~0x40;                     // Timer3 uses SYSCLK/12

   TMR3RL = -(SYSCLK/12/40);           // Timer3 configured to overflow after
   TMR3 = TMR3RL;                      // ~25ms (for SMBus low timeout detect):
                                       // 1/.025 = 40

   EIE1 |= 0x80;                       // Timer3 interrupt enable
   TMR3CN |= 0x04;                     // Start Timer3

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
//
// P3.2   digital   push-pull     LED
//
// all other port pins unused
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   P0MDOUT = 0x00;                     // All P0 pins open-drain output

   P3MDOUT |= 0x04;                    // Make the LED (P3.2) a push-pull
                                       // output

   XBR0 = XBR0_SMB0E__ENABLED;         // Enable SMBus pins
   // Enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED;

   P0 = 0xFF;

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
               SMB0CN_ACK = 1;         // ACK the received slave address

               if((SMB0DAT&0x01) == READ) // If the transfer is a master READ,
               {
                  // Prepare outgoing byte
                  SMB0DAT = SMB_DATA_OUT[sent_byte_counter-1];
                  sent_byte_counter++;
               }
            }
            else                       // If received slave address does not
            {                          // match,
               SMB0CN_ACK = 0;         // NACK received address
            }
            break;

         // Slave Receiver: Data received
         case  SMB_SRDB:

            if (rec_byte_counter < NUM_BYTES_WR)
            {
               // Store incoming data
               SMB_DATA_IN[rec_byte_counter-1] = SMB0DAT;
               rec_byte_counter++;

               SMB0CN_ACK = 1;          // ACK received data
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
SI_INTERRUPT(Timer3_ISR, TIMER3_IRQn)
{
   SMB0CF &= ~0x80;                    // Disable SMBus
   SMB0CF |= 0x80;                     // Re-enable SMBus
   TMR3CN &= ~0x80;                    // Clear Timer3 interrupt-pending flag
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------