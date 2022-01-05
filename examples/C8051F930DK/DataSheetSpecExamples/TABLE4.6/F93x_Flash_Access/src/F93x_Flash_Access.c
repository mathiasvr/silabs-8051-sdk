//-----------------------------------------------------------------------------
// F93x_Flash_Access.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the current consumption and cycle times of Flash
// erases and writes, as shown on table 4.6 of the datasheet.
//
// Refer to F93x_Flash_Access_Readme.txt for more information regarding this
// code example's functionality.
//
//
// How to Use (Measure Current):
//
// 1) Remove the J17 (Imeasure) shorting block and attach an ammeter between
//    the J17 (Imeasure) headers.
// 2) Place VDD Select SW4 in the 2 Cell (3.3v) position and power the board
//    using the P2 power connector.
// 3) Remove all other shorting blocks on the board, except for the block
//    across the following pins:
//     a) VBAT and WALL_PWR on J11
//     b) P0.2H and SW0.2 on J8
//     c) P0.3H and SW0.3 on J8
// 4) Compile and download the code to a C8051F930-TB target board.
// 5) Run the code.
// 6) Check the current initially
// 7) Hit Sw0.2 to write to flash or SW0.3 to erase from flash.
// 8) Check the new current value.
// 9) The difference is the flash current.
//
// How to Use (Measure Time):
//
// 1) Place VDD Select SW4 in the 2 Cell (3.3v) position and power the board
//    using the P2 power connector.
// 2) Ensure a shorting block attach the J17 (Imeasure) headers.
// 3) Remove all other shorting blocks on the board, except for the block
//    across the following pins:
//     a) VBAT and WALL_PWR on J11
//     b) P0.2H and SW0.2 on J8
//     c) P0.3H and SW0.3 on J8
// 4) Compile and download the code to a C8051F930-TB target board.
// 5) Connect an oscilloscope probe to GND and P1.5
// 6) Run the code.
// 7) Use SW0.2(P0.2) for Flash Write or SW0.3(P0.3) for Flash Erase.
//     a) When measuring for Flash Write, look at the time of the pin staying
//        low .
//     b) When measuring for Flash Erase, divide the time of the pin staying
//        low by 5 (the time accounts for all 5 pages erased).
//
// Target:         C8051F93x/92x
// Tool chain:     Generic
// Command Line:   None
//
//
// Revision History:
//
// Release 1.0
//    - Initial Revision (MT)
//    - 11 SEP 2013
//
//
// Refer to F93x_Flash_Access_Readme.txt for more information.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F930_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SW_PRESSED           0
#define SW_NOT_PRESSED       1

SI_SBIT(SW0_2, SFR_P0, 2);
SI_SBIT(SW0_3, SFR_P0, 3);
SI_SBIT(DS3,   SFR_P1, 5);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);               // Configure the system clock
void Flash_Write(void);                // Flash Write Instruction
void Flash_Erase(void);                // Flash Erase Instruction
void Port_Init(void);                  // Initializing the Switches

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
    PCA0MD &= ~0x40;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
    uint8_t inputlatch = 0;
    uint8_t inputlatch2 = 0;
    // Disable the watchdog timer

    Port_Init();
    SYSCLK_Init();
    while(1)
    {
        if (SW0_2 == SW_PRESSED)        // Flash Write Initialization Switch
        {
            inputlatch = 1;
        }

        if ((SW0_2 == SW_NOT_PRESSED) && (inputlatch == 1))
        {
            Flash_Write();
            inputlatch = 0;
        }

        if (SW0_3 == SW_PRESSED)        // Flash Erase Initialization Switch
        {
            inputlatch2 = 1;
        }

        if ((SW0_3 == SW_NOT_PRESSED) && (inputlatch2 == 1))
        {
            Flash_Erase();
            inputlatch2 = 0;
        }

    }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator divided by 1 as its clock source.
//
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{

    VDM0CN |= 0x80;                     // Enable VDD Monitor

    // Select Precision Internal Oscillator 24.5MHz as system
    // clock source
    REG0CN |= 0x10;
    OSCICN |= 0x80;	
    CLKSEL = 0x00;

    // Waiting for above the VDD monitor warning threshold
    while (!(VDM0CN & 0x20));
    RSTSRC = 0x02;                      // Enable missing clock detector and
                                        // set VDD Monitor as reset source

    //At clock frequencies above 10 MHz, the system clock cycle becomes
    //short enough that the one-shot timer no longer provides a power benefit.
    //Disabling the one-shot timer at higher frequencies
    //reduces power consumption.
    FLSCL = 0x40;	
}

//-----------------------------------------------------------------------------
// Port_Init
//
// Return Value : None
// Parameters   : None
//
//  This function configures the crossbar and GPIO ports.
//
// P0.2  digital  open-drain input      Flash Write Initialization Switch
// P0.3  digital  open-drain input      Flash Erase Initialization Switch
// P1.5  digital  push-pull  output     measurement port
//-----------------------------------------------------------------------------

void Port_Init (void)
{
    P0MDOUT &= ~0x0C;
    P0 |= 0x0C;
    P1MDOUT |= 0x20;
    P1 |= 0x20;
    XBR2 = 0x40;                        // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Runtime Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Flash_Erase
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function is used to erase data from flash with enough cycles for a user
// to properly measure the current/time.
//
//-----------------------------------------------------------------------------

void Flash_Erase (void)
{
    int counter = 0;
    bit EA_SAVE = IE_EA;                   // Preserve IE_EA
    char xdata * data pwrite;           // Flash write pointer
    DS3 = 0;                            // Initialize LED as off
    IE_EA = 0;                             // Disable interrupts
    while(counter < 5)
    {
        counter++;

        VDM0CN = 0x80;                  // Enable VDD monitor

        RSTSRC = 0x02;                  // Enable VDD monitor as a reset source

        pwrite = (char xdata *) 0x0F00;

        FLKEY  = 0xA5;                  // Key Sequence 1
        FLKEY  = 0xF1;                  // Key Sequence 2
        PSCTL |= 0x03;                  // PSWE = 1; PSEE = 1

        VDM0CN = 0x80;                  // Enable VDD monitor

        RSTSRC = 0x02;                  // Enable VDD monitor as a reset source
        *pwrite = 0;                    // Initiate page erase

        PSCTL &= ~0x03;                 // PSWE = 0; PSEE = 0
    }
    IE_EA = EA_SAVE;                       // Restore interrupts
    DS3 = 1;                            // Turn LED Off when done
}

//-----------------------------------------------------------------------------
// Flash_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function is used to write data to flash with enough cycles for a user
// to properly measure the current/time.
//
//-----------------------------------------------------------------------------

void Flash_Write (void)
{
    int counter = 0;
    bit EA_SAVE = IE_EA;                   // Preserve IE_EA
    char xdata * data pwrite;           // Flash write pointer

    DS3 = 0;                            // Turn LED on while write a byte
                                        // to flash.
    IE_EA = 0;                             // Disable interrupts
    while(counter < 10240)
    {
        counter++;

        VDM0CN = 0x80;                  // Enable VDD monitor

        RSTSRC = 0x02;                  // Enable VDD monitor as a
                                        // reset source

        pwrite = (char xdata *) 0x0800;

        FLKEY  = 0xA5;                  // Key Sequence 1
        FLKEY  = 0xF1;                  // Key Sequence 2
        PSCTL |= 0x01;                  // PSWE = 1 which enables writes

        VDM0CN = 0x80;                  // Enable VDD monitor

        RSTSRC = 0x02;                  // Enable VDD monitor as a reset
                                        // source

        *pwrite = 0xA5;                 // Write the byte

        PSCTL &= ~0x01;                 // PSWE = 0 which disable writes

    }
    IE_EA = EA_SAVE;                       // Restore interrupts
    DS3 = 1;                            // Turn LED Off when done
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
