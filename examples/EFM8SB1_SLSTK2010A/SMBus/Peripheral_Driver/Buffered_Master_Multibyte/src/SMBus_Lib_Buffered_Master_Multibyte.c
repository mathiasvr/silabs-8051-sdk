//-----------------------------------------------------------------------------
// SMBus_Lib_Buffered_Master_Multibyte.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Example software to demonstrate the SMBus interface in
// Master mode using the EFM8 SMBus peripheral driver in
// buffered mode.
// - Interrupt-driven SMBus implementation
// - Only master states defined (no slave or arbitration)
// - multiple-byte SMBus data holders used for each transmit and receive
// - Timer1 used as SMBus clock source
// - Timer3 used by SMBus for SCL low timeout detection
// - SCL frequency defined by SMBUS module in Configurator
// - SMB0CN0_ARBLOST support included
// - supports multiple-byte writes and multiple-byte reads
// - Pinout:
//    P1.2 -> SDA (SMBus)
//    P1.3 -> SCL (SMBus)
//
//    P1.1 -> LED0
//
//    all other port pins unused
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect the device to another EFM8 device running SMBus - Slave Multibyte code.
// 3) Connect the EFM8SB1 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
//         a) The test will indicate proper communication with the Slave by
//            toggling the LED on and off each time a value is sent and
//            received.
//         b) The best method to view the proper functionality is to run to
//            the indicated line of code in the TEST CODE section of main and
//            view the SMB_DATA_IN and SMB_DATA_OUT variable arrays in the
//            Watch Window.
//
// Target:         EFM8SB1
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 9 JAN 2015
//
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "bsp.h"
#include "smb_0.h"
#include "InitDevice.h"
#include "SMBus_Lib_Buffered_Master_Multibyte.h"

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

// Global holder for SMBus data
// All receive data is written here
SI_SEGMENT_VARIABLE(SMB_DATA_IN[NUM_BYTES_RD],
                    uint8_t,
                    EFM8PDL_SMB0_RX_BUFTYPE);

// Global holder for SMBus data.
// All transmit data is read from here
SI_SEGMENT_VARIABLE(SMB_DATA_OUT[NUM_BYTES_WR],
                    uint8_t,
                    EFM8PDL_SMB0_TX_BUFTYPE);

// Counter for the number of errors.
uint16_t numErrors;

SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);

volatile bool transferInProgress = false;

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(SDA, SFR_P1, 2);
SI_SBIT(SCL, SFR_P1, 3);

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void T0_Waitms(uint8_t ms);

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
// main() Routine
// ----------------------------------------------------------------------------
int main(void)
{
  uint8_t i;
  uint8_t outData = 0;

  // Enter default mode
  enter_DefaultMode_from_RESET();

  BSP_DISP_EN = 0;
  BSP_LED0 = BSP_LED_OFF;

  // If slave is holding SDA low because of an improper SMBus reset or error
  while (!SDA)
  {
    // Provide clock pulses to allow the slave to advance out
    // of its current state. This will allow it to release SDA.
    XBR2 = XBR2_XBARE__ENABLED;       // Enable Crossbar
    SCL = 0;                          // Drive the clock low
    for (i = 0; i < 255; i++);        // Hold the clock low
    SCL = 1;                          // Release the clock
    while (!SCL);                     // Wait for open-drain
                                      // clock output to rise
    for (i = 0; i < 10; i++);         // Hold the clock high
    XBR2 = XBR2_XBARE__DISABLED;      // Disable Crossbar
  }

  // Enter default mode
  enter_Mode2_from_DefaultMode();

  // TEST CODE-------------------------------------------------------------------

  SMB0_reset();

  // Use Timer 1 for SCL clock rate
  // Enable SCL low timeout using Timer 3
  SMB0_init(SMB0_TIMER1, true);

  while (1)
  {
    // Error counter
    numErrors = 0;

    // SMBus Write Sequence
    for (i = 0; i < NUM_BYTES_WR; i++)
    {
      SMB_DATA_OUT[i] = outData;
      outData++;
    }

    transferInProgress = true;

    // Start write/read transfer
    // Send out data
    // Receive in data (loopback data from slave)
    SMB0_transfer(SLAVE_ADDR, SMB_DATA_OUT, SMB_DATA_IN, NUM_BYTES_WR, NUM_BYTES_RD);

    // Wait until transfer complete callback is called
    while(transferInProgress);

    // Check transfer data
    for (i = 0; i < NUM_BYTES_RD; i++)
    {
      // Received data match transmit data?
      if (SMB_DATA_IN[i] != SMB_DATA_OUT[i])
      {
        // Increment error counter if no match
        numErrors++;
      }
    }

    // Indicate that an error has occurred (LED no longer lit)
    if (numErrors > 0)
    {
      BSP_LED0 = BSP_LED_OFF;
    }
    else
    {
      BSP_LED0 = !BSP_LED0;
    }

    // Run to here to view the SMB_DATA_IN and SMB_DATA_OUT variable arrays

    T0_Waitms(50);  // Wait 50 ms until the next cycle
                    // so that LED blinks slow enough to see
  }

  // END TEST CODE---------------------------------------------------------------
}

//-----------------------------------------------------------------------------
// Callbacks
//-----------------------------------------------------------------------------

void SMB0_commandReceivedCb()
{
  // Not used
}

// Callback is called when an SMBus error occurs
void SMB0_errorCb (SMB0_TransferError_t error)
{
  switch (error)
  {
    case SMB0_ARBLOST_ERROR:
    case SMB0_NACK_ERROR:
    case SMB0_UNKNOWN_ERROR:
    case SMB0_TXUNDER_ERROR:
      numErrors++;
      break;
  }

  // Abort the transfer
  SMB0_abortTransfer();

  // Transfer is complete (error)
  transferInProgress = false;
}

void SMB0_transferCompleteCb()
{
  // Transfer is complete (success)
  transferInProgress = false;
}

//-----------------------------------------------------------------------------
// Support Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// T0_Waitms
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint8_t ms - number of milliseconds to wait
//                        range is full range of character: 0 to 255
//
// Configure Timer0 to wait for <ms> milliseconds using SYSCLK as its time
// base.
//
//-----------------------------------------------------------------------------
void T0_Waitms(uint8_t ms)
{
  TCON &= ~0x30;                      // Stop Timer0; Clear TCON_TF0
  TMOD &= ~0x0f;                      // 16-bit free run mode
  TMOD |= 0x01;

  CKCON0 |= 0x04;                     // Timer0 counts SYSCLKs

  while (ms)
  {
    TCON_TR0 = 0;                     // Stop Timer0
    TH0 = (uint8_t)((-SYSCLK / 1000) >> 8);    // Overflow in 1ms
    TL0 = ((-SYSCLK / 1000) & 0xFF);
    TCON_TF0 = 0;                     // Clear overflow indicator
    TCON_TR0 = 1;                     // Start Timer0
    while (!TCON_TF0);                // Wait for overflow
    ms--;                             // Update ms counter
  }

  TCON_TR0 = 0;                       // Stop Timer0
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
