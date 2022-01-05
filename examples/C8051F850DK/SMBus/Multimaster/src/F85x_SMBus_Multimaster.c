//-----------------------------------------------------------------------------
// F85x_SMBus_Multimaster.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Example software to demonstrate the SMBus interface in a Multi-Master
// environment.
// - Interrupt-driven SMBus implementation
// - Master and slave states defined
// - 1-byte SMBus data holders used for each transmit and receive
// - Timer1 used as SMBus clock source
// - Timer3 used by SMBus for SCL low timeout detection
// - SCL frequency defined by <SMB_FREQUENCY> constant
// - SMB0CN_ARBLOST support included
// - When another master attempts to transmit at the same time:
//      - the 'F85x SMBus master should detect any conflict using SMB0CN_ARBLOST
//      - the 'F85x SMBus master should give up the bus, if necessary, and
//        switch to slave receiver mode
//      - the 'F85x SMBus master should transmit after the other master is done
// - Pinout:
//    P0.0 -> SDA (SMBus)
//    P0.1 -> SCL (SMBus)
//
//    P1.0 -> LED1
//
//    P1.7 -> S1
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
// How To Test: UPMU-F850
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - JP2
// 2) Ensure that J32 is not populated.
// 3) Connect the device to another C8051Fxxx SMBus multimaster.
// 4) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 5) Verify that MY_ADDR and MULTI_ADDR are defined to the proper values.
//    By default, MY_ADDR is set to 0xF0, and MULTI_ADDR (the address of the
//    other multimaster) is set to 0x0F. The other multimaster should, if using
//    these defaults, have MY_ADDR set to 0x0F and MULTI_ADDR set to 0xF0.
// 6) Compile and download code to the 'F85x UDP MCU board by selecting 
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the 
//		Resume button in the quick menu, or pressing F8.
//    The test will indicate proper communication with the
//    other C8051Fxxx device by blinking the LED on the other device on and
//    off while switch S1 is pressed, even if the buttons on both boards are
//    pressed at the same time.
//
//      'F85x Multimaster                     Other C8051Fxxx Multimaster
//         LED1 (blinks) <------------------------------ Switch
//              S1 --------------------------------> LED (blinks)
//
//-----------------------------------------------------------------------------
// How To Test: ToolStick F85x
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Connect the device to another C8051Fxxx SMBus multimaster.
// 3) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 4) Verify that MY_ADDR and MULTI_ADDR are defined to the proper values.
//    By default, MY_ADDR is set to 0xF0, and MULTI_ADDR (the address of the
//    other multimaster) is set to 0x0F. The other multimaster should, if using
//    these defaults, have MY_ADDR set to 0x0F and MULTI_ADDR set to 0xF0.
// 5) Compile and download code to the ToolStick F85x board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//    The test will indicate proper communication with the
//    other C8051Fxxx device by blinking the LED on the other device on and
//    off while switch S1 is pressed, even if the buttons on both boards are
//    pressed at the same time.
//
//      'F85x Multimaster                     Other C8051Fxxx Multimaster
//         LED1 (blinks) <------------------------------ Switch
//              S1 --------------------------------> LED (blinks)
//
// Target:         C8051F85x/85x
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.2 (BL)
//		- Updated How To Test with Studio specific instructions
//		- 3 JAN 2014
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    -Initial Revision (TP)
//    -27 FEB 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>
#include "InitDevice.h"
#include "F85x_SMBus_Multimaster.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Global holder for SMBus master data. All receive data is written here
// while in master mode
// Slave->Master
uint8_t SMB_DATA_IN_MASTER = 0x00;

// Global holder for SMBus master data. All transmit data is read from here
// while in master mode
// Master->Slave
uint8_t SMB_DATA_OUT_MASTER = 0x00;

// Global holder for SMBus slave data. All receive data is written here
// while slave mode
// Master->Slave
uint8_t SMB_DATA_IN_SLAVE = 0x00;

// Global holder for SMBus slave data. All transmit data is read from here
// while in slave mode
// Slave->Master
uint8_t SMB_DATA_OUT_SLAVE = 0x00;

volatile bit DATA_READY = 0;           // Set to '1' by the SMBus ISR
                                       // when a new data byte has been
                                       // received in slave mode.

uint8_t TARGET;                             // Target SMBus slave address

volatile bit SMB_BUSY;                 // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

volatile bit SMB_RW;                   // Software flag to indicate the
                                       // direction of the current transfer

uint8_t NUM_ERRORS;                         // Counter for the number of errors.

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(LED1, SFR_P1, 0);                 // DS5 P1.0 LED

SI_SBIT(S1, SFR_P1, 7);

SI_SBIT(SDA, SFR_P0, 0);                  // SMBus on P0.0
SI_SBIT(SCL, SFR_P0, 1);                  // and P0.1

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SMB_Write(void);
void Blink_LED(void);
void Stop_LED(void);

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
// MAIN Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, then loops forever checking
// for and sending the LED commands.
//
//-----------------------------------------------------------------------------
void main (void)
{
   bit switch_pressed_flag = 0;        // Used to detect when the switch is
                                       // released
   uint8_t i;                               // Dummy variable counters

   enter_BusFreeMode_from_RESET();

   // If slave is holding SDA low because of an improper SMBus reset or error
   while(!SDA)
   {
      // Provide clock pulses to allow the slave to advance out
      // of its current state. This will allow it to release SDA.
      XBR2 = 0x40;                     // Enable Crossbar
      SCL = 0;                         // Drive the clock low
      for(i = 0; i < 255; i++);        // Hold the clock low
      SCL = 1;                         // Release the clock
      while(!SCL);                     // Wait for open-drain
                                       // clock output to rise
      for(i = 0; i < 10; i++);         // Hold the clock high
      XBR2 = 0x00;                     // Disable Crossbar
   }

   enter_DefaultMode_from_BusFreeMode();

   LED1 = LED_OFF;

   while(1)
   {
      // Check if the switch was just pressed
      if((switch_pressed_flag == 0) && (S1 == 0))
      {
          // SMBus Write Sequence

          // Tell the other device the switch was pressed
          SMB_DATA_OUT_MASTER = SW_PRESSED;

          TARGET = MULTI_ADDR;         // Target the other multimaster device
          SMB_Write();                 // Initiate SMBus write

          switch_pressed_flag = 1;
      }

      // Determine if the switch was just released
      if((switch_pressed_flag == 1) && (S1 != 0))
      {
          // SMBus Write Sequence

          // Tell the other device the switch was released
          SMB_DATA_OUT_MASTER = SW_RELEASED;

          TARGET = MULTI_ADDR;         // Target the other multimaster device
          SMB_Write();                 // Initiate SMBus write

          switch_pressed_flag = 0;
      }

      // Check if data was sent to this device from the other multimaster
      // device
      if(DATA_READY == 1)
      {
         if(SMB_DATA_IN_SLAVE == SW_PRESSED)
         {
            Blink_LED();
         }
         else
         {
            if(SMB_DATA_IN_SLAVE == SW_RELEASED)
            {
               Stop_LED();
            }
         }

         DATA_READY = 0;
      }
   }  // Loop forever
}

//-----------------------------------------------------------------------------
// Support Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SMB_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Writes a single byte to the slave with address specified by the <TARGET>
// variable.
// Calling sequence:
// 1) Write target slave address to the <TARGET> variable
// 2) Write outgoing data to the <SMB_data_out> variable
// 3) Call SMB_Write()
//
//-----------------------------------------------------------------------------
void SMB_Write(void)
{
   while(SMB_BUSY);                    // Wait for SMBus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)
   SMB_RW = 0;                         // Mark this transfer as a WRITE
   SMB0CN_STA = 1;                            // Start transfer
}

//-----------------------------------------------------------------------------
// Blink_LED
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Starts the LED blinking.
//
//-----------------------------------------------------------------------------
void Blink_LED(void)
{
   TCON_TR0 = 1;
}

//-----------------------------------------------------------------------------
// Stop_LED
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Stops the LED from blinking.
//
//-----------------------------------------------------------------------------
void Stop_LED(void)
{
   TCON_TR0 = 0;

   LED1 = LED_OFF;                     // Turn the LED off when it's not
                                       // blinking
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
