//-----------------------------------------------------------------------------
// F30x_SMBus_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Example software to demonstrate the C8051F30x SMBus interface in Slave mode
// - Interrupt-driven SMBus implementation
// - Only slave states defined
// - 1-byte SMBus data holder used for both transmit and receive
// - Timer1 used as SMBus clock rate (used only for free timeout detection)
// - Timer2 used by SMBus for SCL low timeout detection
// - SMB0CN_ARBLOST support included
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
// 4) Connect the device to another 'Fxxx device running SMBus - Master code.
// 5) Compile and download code to a 'F300 device on a C8051F300-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8:
//    The slave code will write data and read data from the
//    same data byte, so a successive write and read will effectively echo the
//    data written.  To verify that the code is working properly, verify on the
//    master that the data written is the same as the data received.
//
//
// Target:         C8051F300
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 code with compiler agnostic code
//    - 10 JAN 2014
//
// Release 1.0
//    -Initial Revision (TP)
//    -02 MAR 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F300_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define  SYSCLK         24500000       // System clock frequency in Hz

#define  SMB_FREQUENCY  10000          // Target SMBus frequency
                                       // This example supports between 10kHz
                                       // and 100kHz

#define  WRITE          0x00           // SMBus WRITE command
#define  READ           0x01           // SMBus READ command
#define LED_ON           0             // Turns the LED on
#define LED_OFF          1             // Turns the LED off

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

// 16-bit SFR declarations

SI_SBIT(LED, SFR_P0, 2);                 // LED on port P0.2

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(TIMER2_ISR, TIMER2_IRQn);
SI_INTERRUPT_PROTO(SMBUS0_ISR, SMBUS0_IRQn);

void SMBus_Init (void);
void Timer1_Init (void);
void Timer2_Init (void);
void Port_Init (void);

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
   PCA0MD &= ~0x40;                    // WDTE = 0 (Disable watchdog
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, then waits for SMBus
// communication.
//
void main (void)
{
                                       // timer)

   OSCICN |= 0x03;                     // Set internal oscillator to highest
                                       // setting of 24500000

   Port_Init();                        // Initialize Crossbar and GPIO
   Timer1_Init();                      // Configure Timer1 for use
                                       // with SMBus baud rate

   Timer2_Init();                      // Configure Timer for use with
                                       // SCL low timeout detect

   SMBus_Init ();                      // Configure and enable SMBus

   EIE1 |= 0x01;                       // Enable the SMBus interrupt

   LED = 0;

   IE_EA = 1;                             // Global interrupt enable

   SMB_DATA = 0xFD;                    // Initialize SMBus data holder

   while(1)
   {
      while(!DATA_READY);              // New SMBus data received?
      DATA_READY = 0;
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
void SMBus_Init (void)
{
   SMB0CF = 0x1D;                      // Use Timer1 overflows as SMBus clock
                                       // source;
                                       // Enable slave mode;
                                       // Enable setup & hold time
                                       // extensions;
                                       // Enable SMBus Free timeout detect;
                                       // Enable SCL low timeout detect;

   SMB0CF |= 0x80;                     // Enable SMBus;
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

   // Timer1 configured to overflow at 1/3 the rate defined by SMB_FREQUENCY
   TH1 = (uint8_t) -(SYSCLK/SMB_FREQUENCY/SCALE/3);

   TL1 = TH1;                          // Init Timer1

   TCON_TR1 = 1;                            // Timer1 enabled
}

//-----------------------------------------------------------------------------
// Timer2_Init()
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

   CKCON &= ~0x20;                     // Timer2 uses SYSCLK/12

   TMR2RL = (uint16_t) -(SYSCLK/12/40);           // Timer2 configured to overflow after
   TMR2 = TMR2RL;                      // ~25ms (for SMBus low timeout detect):
                                       // 1/.025 = 40

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
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SMBus Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// SMBus ISR state machine
// - Slave only implementation - no master states defined
// - All incoming data is written to global variable <SMB_DATA>
// - All outgoing data is read from global variable <SMB_DATA>
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(SMBUS0_ISR, SMBUS0_IRQn)
{
   bit RXTX = 0;                       // Software flag for slave
                                       // receiver-to-transmitter transition

   if (SMB0CN_ARBLOST == 0)
   {
      switch (SMB0CN & 0xF0)           // Decode the SMBus status vector
      {
         // Slave Receiver: Start+Address received
         case  SMB_SRADD:

            SMB0CN_STA = 0;                   // Clear SMB0CN_STA bit
            if((SMB0DAT&0xFE) == (SLAVE_ADDR&0xFE)) // Decode address
            {                          // If the received address matches,
               SMB0CN_ACK = 1;                // SMB0CN_ACK the received slave address
               if((SMB0DAT&0x01) == READ) // If the transfer is a master READ,
               {
                  SMB0DAT = SMB_DATA;  // Prepare outgoing byte

                  if(!(SMB0DAT&0x80))  // If data byte MSB = '0',
                  {
                     SMB0CN_STO = 1;          // Set SMB0CN_STO to force outgoing MSB => '0'
                     RXTX = 1;         // Set receiver-to-transmitter flag
                  }
               }
            }
            else                       // If received slave address does not
            {                          // match,
               SMB0CN_ACK = 0;                // NACK received address
            }
            break;

         // Slave Receiver: Data received
         case  SMB_SRDB:

            SMB_DATA = SMB0DAT;        // Store incoming data
            DATA_READY = 1;            // Indicate new data received
            SMB0CN_ACK = 1;                   // SMB0CN_ACK received data

            break;

         // Slave Receiver: Stop received while either a Slave Receiver or
         // Slave Transmitter
         case  SMB_SRSTO:

            SMB0CN_STO = 0;                   // SMB0CN_STO must be cleared by software when
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

            SMB0CN_STO = 0;                   // SMB0CN_STO must be cleared by software when
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

   SMB0CN_SI = 0;                             // Clear SMBus interrupt flag

   if (RXTX)                           // If RXTX flag is set, the last state
   {                                   // was a receiver-to-transmitter
                                       // transition;
      while(!SMB0CN_TXMODE);                  // Wait for slave to enter transmit
                                       // mode,
      SMB0CN_STO = 0;                         // Clear the SMB0CN_STO bit and the RXTX flag
      RXTX = 0;
   }
}

//-----------------------------------------------------------------------------
// Timer2 Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// A Timer2 interrupt indicates an SMBus SCL low timeout.
// The SMBus is disabled and re-enabled here
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
   SMB0CF &= ~0x80;                    // Disable SMBus
   SMB0CF |= 0x80;                     // Re-enable SMBus
   TMR2CN_TF2H = 0;                           // Clear Timer2 interrupt-pending flag
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------