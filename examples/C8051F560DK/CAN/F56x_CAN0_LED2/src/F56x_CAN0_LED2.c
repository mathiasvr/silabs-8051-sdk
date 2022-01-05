//-----------------------------------------------------------------------------
// F56x_CAN0_LED2.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program monitors the CAN bus for messages indicating that the LED
// should turn on.  It also periodically monitors the P1.4 switch and
// transmits a message to turn OFF or ON the LED on the other MCU.
//
// Messages are sent using message object TRANSMIT_MO and use the 11-bit
// arbitration ID defined by LED1_ID.
//
// Message are received into message object RECEIVE_MO if the received
// messages matches the arbitration ID defined by LED2_ID.
//
// This file is identical to F56x_CAN0_LED1.c, except for the definition
// of #define DEVICE_NO
//
//
// How To Test:
//
// 1) Verify the LED and switch pin jumpers are populated
//    (J19 for device A and J15 for device B).
//    Verify regulator switch are also populated.
//    (J14 for device A and J8 for device B)
//
// 2) Make sure the CAN jumpers in J17 (for A side) and J12 (for B side)
//    are connected.
//
// 3) Download the code to a F56x-TB (either device A or device B) that is
//    connected as above to another device running the F56x_CAN0_LED1.c
//    code.
//
// 4) Press the P1.4 switch on Side A to turn on the LED on Side B.  Press the
//    1.4 switch on Side B to turn on the LED on Side A.
//
// Target:         C8051F550 (Side A of a C8051F560-TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.2 / 8  MAY 2015 (DL)
//    -Updated to use with Simplicity Studio
//
// Release 1.1 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0 / 18 MAR 2009 (GP)
//    - Initial Revision
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F550_Register_Enums.h>

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void CAN0_Init (void);
void TIMER0_Init(void);

void CAN0_SendMessage (uint8_t message);

SI_INTERRUPT_PROTO(CAN0_ISR, INTERRUPT_CAN0);
SI_INTERRUPT_PROTO(TIMER0_ISR, INTERRUPT_TIMER0);

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define DEVICE_NO            2

#define SYSCLK        24000000         // System clock speed in Hz

#define SW_POLL             50         // How often to poll the switch per sec.
                                       // Minimum value is 1

// Reload value for Timer0 assuming a 1:48 scaling factor for Timer0 clock
#define TIMER0_RL_HIGH  (0xFFFF -((SYSCLK/48/SW_POLL >> 8) & 0x00FF))
#define TIMER0_RL_LOW   (0xFFFF -(SYSCLK/48/SW_POLL & 0x00FF))


// Message Objects to use (range 0-31).
#define TRANSMIT_MO        0x05
#define RECEIVE_MO         0x06

#if (DEVICE_NO == 0x01)

   // 11-bit Arbitration Message IDs
   #define LEDTX_ID        0x001       // Transmitted by device 1
   #define LEDRX_ID        0x002       // Received from device 2

#else

   // 11-bit Arbitration Message IDs
   #define LEDTX_ID        0x002       // Transmitted by device 1
   #define LEDRX_ID        0x001       // Received from device 2

#endif


// Size of CAN Messages
#define MESSAGE_SIZE   1               // Range is 1-8 bytes

// Contents of message
#define LED_ON              0x01
#define LED_OFF             0x00

// Status of the switch
#define SW_ON               0x00
#define SW_OFF              0x01

//-----------------------------------------------------------------------------
// Bit Definition Masks
//-----------------------------------------------------------------------------

// CAN0STAT
#define BOff  0x80                     // Busoff Status
#define EWarn 0x40                     // Warning Status
#define EPass 0x20                     // Error Passive
#define RxOk  0x10                     // Receive Message Successfully
#define TxOk  0x08                     // Transmitted Message Successfully
#define LEC   0x07                     // Last Error Code

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P1, 3);                 // LED = 1 turns on the LED
SI_SBIT(SW,  SFR_P1, 4);                 // SW = 0 = pressed

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint8_t  SW_status;                         // Keeps track of switch

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
   PCA0MD &= ~0x40;                    // Disable Watchdog Timer
}
 
bit CAN_ERROR = 0;                     // 0 = No Errors during transmission
                                       // 1 = Some error(s) occurred
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SFRPAGE = LEGACY_PAGE;              // Set for PCA0MD


   OSCILLATOR_Init ();                 // Initialize oscillator
   PORT_Init ();                       // Initialize crossbar and GPIO
   TIMER0_Init ();                     // Initialize Timer 0
   CAN0_Init ();                       // Start CAN peripheral

   SW_status = SW_OFF;                 // Assume it is off to start
   LED = LED_ON;                      // Initial state is off

   IE_EA = 1;                          // Enable global interrupts

   // The CAN0 ISR will handle any received messages and turn on/off the LED

   // The Timer0 ISR will periodically poll the switch and transmit a message
   // on the CAN Bus is the switch has changed state

   while (1) {}
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Initialize the internal oscillator to 24 MHz
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0x87;                      // Set internal oscillator divider to 1

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P0.6   digital  push-pull        CAN TX
// P0.7   digital  open-drain       CAN RX
//
// P1.3   digital  push-pull        LED
// P1.4   digital  open-drain       SW
//
//-----------------------------------------------------------------------------

void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE  = CONFIG_PAGE;             // Port SFR's on Configuration page

   P0MDOUT  |= 0x40;                   // P0.6 (CAN0 TX) is push-pull
   P1MDOUT  |= 0x08;                   // P1.3 (LED) is push-pull

   XBR0     = 0x02;                    // Enable CAN0 on Crossbar
   XBR2     = 0x40;                    // Enable Crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// TIMER0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure Timer0 to 16-bit Timer mode and generate an interrupt
// every TIMER0_RL Timer0 cycles using SYSCLK/48 as the Timer0 time base.
//
//-----------------------------------------------------------------------------
void TIMER0_Init(void)
{
   // No need to set SFRPAGE as all registers accessed in this function
   // are available on all pages

   TH0 = TIMER0_RL_HIGH;               // Init Timer0 High register
   TL0 = TIMER0_RL_LOW;                // Init Timer0 Low register

   TMOD  = 0x01;                       // Timer0 in 16-bit mode
   CKCON = 0x02;                       // Timer0 uses a 1:48 prescaler
   IE_ET0 = 1;                       // Timer0 interrupt enabled
   TCON  = 0x10;                       // Timer0 ON
}

//-----------------------------------------------------------------------------
// CAN0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the CAN peripheral and two message objects
//
// CAN Bit Clock : 1 Mbps
// Auto Retransmit : Automatic Retransmission is enabled
// MsgVal        : Set to Valid based on the #define MESSAGE_OBJECTS
// Filtering     : Enabled for all valid message objects
// Message Identifier : 11-bit standard; Each message object is only used by
//                      one message ID
// Direction     : One buffer each is configured for transmit and receive
// End of Buffer : All message objects are treated as separate buffers
//
// The following interrupts are enabled and are handled by CAN0_ISR
//
// Error Interrupts
// Status Change Interrupt
// Receive Interrupt
//
//-----------------------------------------------------------------------------

void CAN0_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE  = CAN0_PAGE;               // All CAN register are on page 0x0C

   CAN0CN |= 0x01;                     // Start Intialization mode

   //---------Initialize general CAN peripheral settings

   CAN0CN |= 0x4E;                     // Enable Error and Module
                                       // Enable access to bit timing register

   // See the CAN Bit Timing Spreadsheet for how to calculate this value
   CAN0BT = 0x1402;                    // Based on 24 Mhz CAN clock, set the
                                       // CAN bit rate to 1 Mbps

   //---------Initialize settings for Transmit Message Object

   // Command Mask Register
   CAN0IF1CM = 0x00F0;                 // Write Operation
                                       // Transfer ID Mask, MDir, MXtd
                                       // Transfer ID, Dir, Xtd, MsgVal
                                       // Transfer Control Bits
                                       // Don't set TxRqst or transfer data

   // Mask Registers
   CAN0IF1M1 = 0x0000;                 // Mask Bits 15-0 not used for filtering
   CAN0IF1M2 = 0x5FFC;                 // Ignore Extended Identifier for
                                       // filtering
                                       // Used Direction bit for filtering
                                       // Use ID bits 28-18 for filtering

   // Message Control Registers
   CAN0IF1MC = 0x0080 | MESSAGE_SIZE;  // Disable Transmit Interrupt
                                       // Message Object is a Single Message
                                       // Message Size set by #define
   // Arbitration Registers
   CAN0IF1A1 = 0x0000;                 // 11-bit ID, so lower 16-bits not used

   // Arbitration Registers
   CAN0IF1A2 = 0xA000 | (LEDTX_ID << 2);  // Set MsgVal to valid
                                          // Set Direction to write
                                          // Set 11-bit Identifier to iter

   CAN0IF1CR = TRANSMIT_MO;            // Start command request

   while (CAN0IF1CRH & 0x80) {}        // Poll on Busy bit


   //---------Initialize settings for Receive Message Object

   // Can use the same settings for Receive object, so no need reinitalize the
   // first four CAN register again

   // Command Mask Register
   //   CAN0IF1CM = 0x00F0;

   // Mask Registers
   //  CAN0IF1M1 = 0x0000;
   //  CAN0IF1M2 = 0x5FFC;

   // Arbitration Registers
   //  CAN0IF1A1 = 0x0000;


   // Message Control Registers
   CAN0IF1MC = 0x1480 | MESSAGE_SIZE;  // Enable Receive Interrupt
                                       // Message Object is a Single Message
                                       // Message Size set by #define
   // Arbitration Registers
   CAN0IF1A2 = 0x8000 | (LEDRX_ID << 2);  // Set MsgVal to valid
                                          // Set Object Direction to read
                                          // Set 11-bit Identifier to iter

   CAN0IF1CR = RECEIVE_MO;             // Start command request

   while (CAN0IF1CRH & 0x80) {}        // Poll on Busy bit

   //--------- CAN Initalization is complete

   CAN0CN &= ~0x41;                    // Return to Normal Mode and disable
                                       // access to bit timing register

   EIE2 |= 0x02;                       // Enable CAN interupts

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Supporting Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CAN0_SendMessage
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Send the data
//
//-----------------------------------------------------------------------------

void CAN0_SendMessage (uint8_t message)
{
   // This function assumes that the message object is fully initialized
   // in CAN0_Init and so all it has to do is fill the data registers and
   // initiate transmission

   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE  = CAN0_PAGE;               // All CAN register are on page 0x0C

   CAN0IF1DA1L = message;              // Transfer message to data register

   CAN0IF1CM = 0x0087;                 // Set Direction to Write
                                       // Write TxRqst, all 8 data bytes

   CAN0IF1CR = TRANSMIT_MO;            // Start command request

   while (CAN0IF1CRH & 0x80) {}        // Poll on Busy bit

   SFRPAGE = SFRPAGE_save;             // Restore SFRPAGE
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// TIMER0_ISR
//-----------------------------------------------------------------------------
//
// The ISR is triggered upon any CAN errors or upon a message reception
//
// If an error occurs, a global counter is updated
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(TIMER0_ISR, TIMER0_IRQn)
{
   // Timer0 ISR pending flag is automatically cleared by vectoring to ISR

   if (SW != SW_status)                // Switch has changed since last check
   {
      SW_status = SW;                  // Update the status
      CAN0_SendMessage (SW_status);    // Send the new status over CAN
   }
}


//-----------------------------------------------------------------------------
// CAN0_ISR
//-----------------------------------------------------------------------------
//
// The ISR is triggered upon any CAN errors or upon a complete transmission
//
// If an error occurs, a global counter is updated
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(CAN0_ISR, CAN0_IRQn)
{
   // SFRPAGE is set to CAN0_Page automatically when ISR starts

   uint8_t rx_data;                         // Buffer for data from bus

   uint8_t status = CAN0STAT;               // Read status, which clears the Status
                                       // Interrupt bit pending in CAN0IID

   uint8_t Interrupt_ID = CAN0IID;          // Read which message object caused
                                       // the interrupt

   CAN0IF1CM = 0x007F;                 // Read all of message object to IF1
                                       // Clear IntPnd and newData


   CAN0IF1CR = Interrupt_ID;           // Start command request to actually
                                       // clear the interrupt

   while (CAN0IF1CRH & 0x80) {}        // Poll on Busy bit

   // If receive completed successfully
   if ((status & RxOk) && (Interrupt_ID == RECEIVE_MO))
   {
       rx_data = CAN0IF1DA1L;          // Read the data byte

       if (rx_data == SW_ON) {         // Configure LED based on received data
          LED = LED_ON; }
       else {
          LED = LED_OFF; }
   }


   // If an error occured, simply update the global variable and continue
   if (status & LEC)
   {
       // The LEC bits identify the type of error, but those are grouped here
      if ((status & LEC) != 0x07)
      {
          CAN_ERROR = 1;
      }
   }

   if (status & BOff)
   {
      CAN_ERROR = 1;
   }

   if (status & EWarn)
   {
      CAN_ERROR = 1;
   }

   // Old SFRPAGE is popped off the SFR stack when ISR exits
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------

