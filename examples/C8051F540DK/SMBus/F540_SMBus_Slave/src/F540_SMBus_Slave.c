//-----------------------------------------------------------------------------
// F540_SMBus_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Example software to demonstrate the C8051F540 SMBus interface in Slave mode
// - Interrupt-driven SMBus implementation
// - Only slave states defined
// - 1-byte SMBus data holder used for both transmit and receive
// - Timer1 used as SMBus clock rate (used only for free timeout detection)
// - Timer3 used by SMBus for SCL low timeout detection
// - SMB0CN_ARBLOST support included
// - Pinout:
//    P0.0 -> SDA (SMBus)
//    P0.1 -> SCL (SMBus)
//    P1.3 -> LED
//
// How To Test:
//
// 1) Verify that J13 is not populated.
// 2) Download code to a 'F540 device that is connected to a SMBus master.
// 3) Run the code.  The slave code will write data and read data from the
//    same data byte, so a successive write and read will effectively echo the
//    data written.  To verify that the code is working properly, verify on the
//    master that the data written is the same as the data received.
//
// Target:         C8051F540 (Side A of a C8051F540-TB)
// Tool chain:     Simplicity Studio / Keil C51 9.53
// Command Line:   None
//
// Release 1.2 / 14 MAY 2015 (SY)
//    -Update SFR/bit name for Simplicity Studio
//
// Release 1.1 / 10 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0 / 06 NOV 2008 (GP)
//    -Initial Revision
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F540_Defs.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define  SYSCLK         24000000       // System clock frequency in Hz

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

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

uint8_t SMB_DATA;                           // Global holder for SMBus data.
                                       // All receive data is written
                                       // here;
                                       // all transmit data is read
                                       // from here

bit DATA_READY = 0;                    // Set to '1' by the SMBus ISR
                                       // when a new data byte has been
                                       // received.

SI_SBIT(LED, SFR_P1, 3);                 // LED==1 means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SMBUS0_Init (void);
void TIMER1_Init (void);
void TIMER3_Init (void);
void PORT_Init (void);

SI_INTERRUPT_PROTO(TIMER3_ISR, TIMER3_IRQn);
SI_INTERRUPT_PROTO(SMBUS0_ISR, SMBUS0_IRQn);

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
   PCA0MD = 0x00;                      // Disable watchdog timer
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
   SFRPAGE = LEGACY_PAGE;              // Set SFR Page for PCA0


   SFRPAGE = CONFIG_PAGE;              // Set SFR Page for XBR and port pins

   OSCICN |= 0x07;                     // Set internal oscillator to 24 MHz

   PORT_Init();                        // Initialize Crossbar and GPIO
   TIMER1_Init();                      // Configure Timer1 for use
                                       // with SMBus baud rate

   TIMER3_Init();                      // Configure Timer3 for use with
                                       // SCL low timeout detect

   SMBUS0_Init ();                     // Configure and enable SMBus

   EIE1 |= 0x01;                       // Enable the SMBus interrupt

   LED = 0;

   IE_EA = 1;                          // Global interrupt enable

   SMB_DATA = 0xFD;                    // Initialize SMBus data holder

   while(1)
   {
      while(!DATA_READY);              // New SMBus data received?
      DATA_READY = 0;
      LED = !LED;
   }
}

//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SMBUS0_Init()
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
void SMBUS0_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   SMB0CF = 0x1D;                      // Use Timer1 overflows as SMBus clock
                                       // source;
                                       // Enable slave mode;
                                       // Enable setup & hold time
                                       // extensions;
                                       // Enable SMBus Free timeout detect;
                                       // Enable SCL low timeout detect;

   SMB0CF |= 0x80;                     // Enable SMBus;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// TIMER1_Init
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
void TIMER1_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

// Make sure the Timer can produce the appropriate frequency in 8-bit mode
// Supported SMBus Frequencies range from 10kHz to 100kHz.  The CKCON register
// settings may need to change for frequencies outside this range.
#if ((SYSCLK/SMB_FREQUENCY/3) < 255)
   #define SCALE 1
      CKCON |= 0x08;                   // Timer1 clock source = SYSCLK
#elif ((SYSCLK/SMB_FREQUENCY/4/3) < 255)
   #define SCALE 4
      CKCON |= 0x01;
      CKCON &= ~0x0A;                  // Timer1 clock source = SYSCLK / 4
#endif

   TMOD = 0x20;                        // Timer1 in 8-bit auto-reload mode

   // Timer1 configured to overflow at 1/3 the rate defined by SMB_FREQUENCY
   TH1 = -(int8_t)(SYSCLK/SMB_FREQUENCY/SCALE/3);

   TL1 = TH1;                          // Init Timer1

   TCON_TR1 = 1;                       // Timer1 enabled

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// TIMER3_Init
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
void TIMER3_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   TMR3CN = 0x00;                      // Timer3 configured for 16-bit auto-
                                       // reload, low-byte interrupt disabled

   CKCON &= ~0x40;                     // Timer3 uses SYSCLK/12

   TMR3RL = -(int16_t)(SYSCLK/12/40);      // Timer3 configured to overflow after
   TMR3 = TMR3RL;                      // ~25ms (for SMBus low timeout detect):
                                       // 1/.025 = 40

   EIE1 |= 0x40;                       // Timer3 interrupt enable
   TMR3CN |= 0x04;                     // Start Timer3

   SFRPAGE = SFRPAGE_save;
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
// P1.3   digital   push-pull     LED
//
// all other port pins unused
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT = 0x00;                     // All P0 pins open-drain output

   P1MDOUT |= 0x08;                    // Make the LED (P1.3) a push-pull
                                       // output

   XBR0 = 0x08;                        // Enable SMBus pins
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups

   P0 = 0xFF;

   SFRPAGE = SFRPAGE_save;
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
// - All incoming data is written to global variable <SMB_DATA_IN>
// - All outgoing data is read from global variable <SMB_DATA_OUT>
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(SMBUS0_ISR, SMBUS0_IRQn)
{
   if (SMB0CN_ARBLOST == 0)
   {
      switch (SMB0CN & 0xF0)           // Decode the SMBus status vector
      {
         // Slave Receiver: Start+Address received
         case  SMB_SRADD:

            SMB0CN_STA = 0;            // Clear SMB0CN_STA bit
            if((SMB0DAT&0xFE) == (SLAVE_ADDR&0xFE)) // Decode address
            {                          // If the received address matches,
               SMB0CN_ACK = 1;         // SMB0CN_ACK the received slave address
               if((SMB0DAT&0x01) == READ) // If the transfer is a master READ,
               {
                  SMB0DAT = SMB_DATA;  // Prepare outgoing byte
               }
            }
            else                       // If received slave address does not
            {                          // match,
               SMB0CN_ACK = 0;         // NACK received address
            }
            break;

         // Slave Receiver: Data received
         case  SMB_SRDB:

            SMB_DATA = SMB0DAT;        // Store incoming data
            DATA_READY = 1;            // Indicate new data received
            SMB0CN_ACK = 1;            // SMB0CN_ACK received data

            break;

         // Slave Receiver: Stop received while either a Slave Receiver or
         // Slave Transmitter
         case  SMB_SRSTO:

            SMB0CN_STO = 0;            // SMB0CN_STO must be cleared by software when
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
         // data pending when a STOP is received from the master, so the SMB0CN_TXMODE
         // bit is cleared and the slave goes to the SRSTO state.
         case  SMB_STSTO:

            SMB0CN_STO = 0;            // SMB0CN_STO must be cleared by software when
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
   // SMB0CN_ARBLOST = 1, Abort failed transfer
   else
   {
      SMB0CN_STA = 0;
      SMB0CN_STO = 0;
      SMB0CN_ACK = 0;
   }

   SMB0CN_SI = 0;                     // Clear SMBus interrupt flag
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
   TMR3CN &= ~0x80;                    // Clear Timer3 interrupt-pending flag
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
