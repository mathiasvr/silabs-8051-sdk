//-----------------------------------------------------------------------------
// F3xx_USB_Main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// USB bulk file transfer example
//
//
// How To Test:    See Readme.txt
// ------------
// 1) Import the project file to Simplicity Studio.
// 2) Ensure shorting blocks on C8051F326-TB are place on the following:
//    J2: VDD  - +3VD
//    J3: P2.2 - P2.2_LED
//    J3: P2.3 - P2.3_LED
//    J7: VDD  - VIO
//    J8: Power selection, P1 9V or J4 Debugger
// 3) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1.
// 4) Compile and download code to C8051F326 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 6) Run the GUI application SLAB_USB_Bulk that comes with the firmware
//    and test the features.
//
//
// Target:         C8051F326/7
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
// Project Name:   USB0_Bulk
//
//
// Release 2.1 (JL)
//    - Updated description.
//    - 20 MAY 2014
//
// Release 2.0
//    -Rewrite (CM)
//    -02 NOV 2012
//
// Release 1.1
//    -Changed "Usb_Init" to "USB_Init" (TP)
//    -07 OCT 2010
//
// Release 1.0
//    -Initial Revision (PD)
//    -04 JUN 2008
//

//-----------------------------------------------------------------------------
// Header Files
//-----------------------------------------------------------------------------

#include "c8051f3xx.h"
#include "F3xx_USB0_Register.h"
#include "F3xx_USB0_Descriptor.h"
#include "F3xx_USB0_InterruptServiceRoutine.h"
#include "F3xx_USB0_Main.h"
#include "F3xx_USB0_Bulk.h"
#include "F3xx_Flash.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Buffer used to transmit a USB packet to the host
// In_Packet (Foreground) => USB IN FIFO (ISR) => Host
SI_SEGMENT_VARIABLE(In_Packet[IN_EP1_PACKET_SIZE], uint8_t, SI_SEG_XDATA);

// Buffer used to receive a USB packet from the host
// Host => USB OUT FIFO (ISR) => Out_Packet (Foreground)
SI_SEGMENT_VARIABLE(Out_Packet[OUT_EP1_PACKET_SIZE], uint8_t, SI_SEG_XDATA);

// State of the In_Packet buffer used to transmit a USB packet to the host:
//
// In_Packet_Ready = 0:
//    - In_Packet is empty
//    - Load In_Packet with a USB packet
//    - Set In_Packet_Ready to 1
// In_Packet_Ready = 1:
//    - In_Packet contains a USB packet to send to the host
//    - Wait until In_Packet_Ready = 0, indicating transmission of a packet
//      (to USB OUT FIFO)
uint8_t In_Packet_Ready = 0;

// State of the Out_Packet buffer used to receive a USB packet from the host:
//
// Out_Packet_Ready = 0:
//    - Out_Packet is empty
//    - Wait until Out_Packet_Ready = 1, indicating reception of a packet
// Out_Packet_Ready = 1:
//    - Out_Packet contains a USB packet received from the host
//    - Unload Out_Packet
//    - Set Out_Packet_Ready = 0
uint8_t Out_Packet_Ready = 0;

// ISR can set this flag to reset the state machine to idle
uint8_t AsyncResetState = 0;

//-----------------------------------------------------------------------------
// Static Global Variables - State Machine
//-----------------------------------------------------------------------------

// State machine state
static uint8_t State = ST_IDLE;

// State variables for Read Page
static uint8_t TxBlock;    // Current block to send to host
static uint8_t TxPage;     // Current flash page to send to host
static uint8_t TxValid;    // Current flash page to send to host is valid

// State variables for Write Page
static uint8_t RxBlock;    // Current block to receive from host
static uint8_t RxPage;     // Current flash page to receive from host
static uint8_t RxValid;    // Current flash page to receive from host is valid

//-----------------------------------------------------------------------------
// Static Function Prototypes - State Machine
//-----------------------------------------------------------------------------

static void StateIdle (void);
static void StateSetFlashKey (void);
static void StateTxPageInfo (void);
static void StateReadPage (void);
static void StateTxBlock (void);
static void StateWritePage (void);
static void StateRxBlock (void);
static void StateTxSuccess (void);
static void StateTxInvalid (void);
static void StateMachine (void);

//-----------------------------------------------------------------------------
// Static Functions - State Machine
//-----------------------------------------------------------------------------

// Ready to receive a command from the host
static void StateIdle (void)
{
   // Recieved an OUT packet
   if (Out_Packet_Ready)
   {
      // Decode the command
      switch (Out_Packet[0])
      {
         case CMD_SET_FLASH_KEY:
            State = ST_SET_FLASH_KEY;
            break;

         case CMD_GET_PAGE_INFO:
            // Done processing Out_Packet
            Out_Packet_Ready = 0;
            State = ST_TX_PAGE_INFO;
            break;

         case CMD_READ_PAGE:
            State = ST_READ_PAGE;
            break;

         case CMD_WRITE_PAGE:
            State = ST_WRITE_PAGE;
            break;

         default:
            // Done processing Out_Packet
            Out_Packet_Ready = 0;
            State = ST_TX_INVALID;
            break;
      }
   }
}

// Receive flash key from host
static void StateSetFlashKey (void)
{
   // Set the flash key
   SetFlashKey (&Out_Packet[1]);

   // Done processing Out_Packet
   Out_Packet_Ready = 0;

   State = ST_TX_SUCCESS;
}

// Send number of flash pages and flash page size to host
static void StateTxPageInfo (void)
{
   // If able to send an IN packet
   if (!In_Packet_Ready)
   {
      In_Packet[0] = RSP_SUCCESS;
      In_Packet[1] = FLASH_NUM_PAGES;
      In_Packet[2] = LOBYTE(FLASH_PAGE_SIZE);
      In_Packet[3] = HIBYTE(FLASH_PAGE_SIZE);

      // Ready to send IN packet
      In_Packet_Ready = 1;

      State = ST_IDLE;
   }
}

// Send flash page to host in blocks (command stage)
static void StateReadPage (void)
{
   // Turn on LED1 while reading page
   SetLed (1, 1);

   // Store the page number
   TxPage = Out_Packet[1];

   // Done processing Out_Packet
   Out_Packet_Ready = 0;

   // Reset the block counter
   //
   // There are 8, 64-byte blocks in a 512-byte
   // flash page
   TxBlock = 0;

   // Check if the requested page number is valid.
   //
   // If the page is invalid, we'll send dummy blocks
   // back and return an error in the response packet.
   if (TxPage < FLASH_NUM_PAGES)
      TxValid = 1;
   else
      TxValid = 0;

   State = ST_TX_BLOCK;
}

// Send flash page to host in blocks (data/response stage)
static void StateTxBlock (void)
{
   // Response stage:
   // Finished sending last block to host
   if (TxBlock == 8)
   {
      // Turn off LED1 after finished reading page
      SetLed (1, 0);

      // Send appropriate response
      if (TxValid)
         State = ST_TX_SUCCESS;
      else
         State = ST_TX_INVALID;
   }
   // Data stage:
   // If able to send an IN packet
   else if (!In_Packet_Ready)
   {
      // Calculate the flash address based on current page and block number
      uint16_t address = FLASH_START + (FLASH_PAGE_SIZE * TxPage) + (IN_EP1_PACKET_SIZE * TxBlock);

      // Only read from flash if the flash page is valid.
      // Otherwise, send whatever dummy data is left in In_Packet
      if (TxValid)
      {
         ReadFlashPage (address, In_Packet, IN_EP1_PACKET_SIZE);
      }

      // Ready to send IN packet
      In_Packet_Ready = 1;

      TxBlock++;
   }
}

// Receive flash page from host in blocks and write to flash (command stage)
static void StateWritePage (void)
{
   // Turn on LED2 while writing page
   SetLed (2, 1);

   // Store the page number
   RxPage = Out_Packet[1];

   // Done processing Out_Packet
   Out_Packet_Ready = 0;

   // Reset the block counter
   //
   // There are 8, 64-byte blocks in a 512-byte
   // flash page
   RxBlock = 0;

   // Check if the requested page number is valid.
   //
   // If the page is invalid, we'll send dummy blocks
   // back and return an error in the response packet.
   if (RxPage < FLASH_NUM_PAGES)
   {
      // Flash page is valid
      RxValid = 1;

      // Erase the flash page
      EraseFlashPage (FLASH_START + (RxPage * FLASH_PAGE_SIZE));
   }
   else
   {
      // Flash page is invalid
      RxValid = 0;
   }

   State = ST_RX_BLOCK;
}

// Receive flash page from host in blocks and write to flash (response/data stage)
static void StateRxBlock (void)
{
   // Response stage:
   // Finished receiving last block
   if (RxBlock == 8)
   {
      // Turn off LED2 after finished writing page
      SetLed (2, 0);

      if (RxValid)
         State = ST_TX_SUCCESS;
      else
         State = ST_TX_INVALID;
   }
   // Data stage:
   // If received an OUT packet
   else if (Out_Packet_Ready)
   {
      uint16_t address = FLASH_START + (FLASH_PAGE_SIZE * RxPage) + (OUT_EP1_PACKET_SIZE * RxBlock);

      if (RxValid)
      {
         WriteFlashPage (address, Out_Packet, OUT_EP1_PACKET_SIZE);
      }

      // Finished processing OUT packet
      Out_Packet_Ready = 0;

      RxBlock++;
   }
}

// Send a response (success)
static void StateTxSuccess (void)
{
   // If able to send an IN packet
   if (!In_Packet_Ready)
   {
      In_Packet[0] = RSP_SUCCESS;

      // Ready to send IN packet
      In_Packet_Ready = 1;

      State = ST_IDLE;
   }
}

// Send a response (invalid)
static void StateTxInvalid (void)
{
   // If able to send an IN packet
   if (!In_Packet_Ready)
   {
      In_Packet[0] = RSP_INVALID;

      // Ready to send IN packet
      In_Packet_Ready = 1;

      State = ST_IDLE;
   }
}

// Manage state transitions
static void StateMachine (void)
{
   uint8_t EA_Save;

   // Received a reset state control request
   // from the host. Transition to the idle
   // state after completing the current state
   // function
   if (AsyncResetState)
   {
      // Disable interrupts
      EA_Save = IE_EA;
      IE_EA = 0;

      // Set index to endpoint 1 registers
      POLL_WRITE_BYTE(INDEX, 1);

      // Flush IN/OUT FIFOs
      POLL_WRITE_BYTE(EINCSR1, rbInFLUSH);
      POLL_WRITE_BYTE(EOUTCSR1, rbOutFLUSH);

      // Flush software In_Packet/Out_Packet buffers
      In_Packet_Ready = 0;
      Out_Packet_Ready = 0;

      // Reset state to idle
      State = ST_IDLE;

      // Turn off both LEDs
      SetLed(1, 0);
      SetLed(2, 0);

      // Finished resetting state
      AsyncResetState = 0;

      // Restore interrupts
      IE_EA = EA_Save;
   }

   switch (State)
   {
      case ST_IDLE:
         StateIdle ();
         break;

      case ST_SET_FLASH_KEY:
         StateSetFlashKey ();
         break;

      case ST_TX_PAGE_INFO:
         StateTxPageInfo ();
         break;

      case ST_READ_PAGE:
         StateReadPage ();
         break;

      case ST_TX_BLOCK:
         StateTxBlock ();
         break;

      case ST_WRITE_PAGE:
         StateWritePage ();
         break;

      case ST_RX_BLOCK:
         StateRxBlock ();
         break;

      case ST_TX_SUCCESS:
         StateTxSuccess ();
         break;

      case ST_TX_INVALID:
         StateTxInvalid ();
         break;
   }
}

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
  // Disable the watchdog here
}
 
//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------

void main (void)
{
   uint8_t EA_Save;

   System_Init ();                     // Initialize Sysclk, Port IO
   USB0_Init ();                       // Initialize USB0

   IE_EA = 1;                          // Enable global interrupts

   // Turn off both LEDs
   SetLed(1, 0);
   SetLed(2, 0);

   while (1)
   {
      StateMachine ();

      EA_Save = IE_EA;
      IE_EA = 0;
      Send_Packet_Foreground ();
      IE_EA = EA_Save;
   }
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------