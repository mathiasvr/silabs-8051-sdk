//-----------------------------------------------------------------------------
// F96x_Flash_Access.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the cycle time and current consumption of flash
// reads and writes as specified on table 4.8 of the datasheet.
//
// Refer to F96x_Flash_Access_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Use (Measure Current):
//
// 1) Cut the trace on the bottom of the PCB between the two pins of the VBAT
//    CURRENT header (J15).
// 2) Cut the trace on the bottom of the PCB between the two pins of the header
//    J20.
// 3) Connect a current meter between the two pins on header J15 to measure
//    the current draw of the device.
// 4) Place VDD Select SW5 in the VREG(3.3v) position and power the board
//    using the P1 power connector.
// 5) Remove all other shorting blocks. 
// 6) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) Record the current consumption.
// 9) Hit Sw2 to write to flash or SW3 to erase from flash.
// 10) Check the new current value.
// 11) The difference is the flash current.
//
// How to Use (Measure Time):
//
// 1) Place VDD Select SW5 in the VREG (3.3v) position and power the board
//    using the P2 power connector.
// 2) Ensure the pins of VBAT CURRENT (J15) are shorted together.
// 3) Remove all other shorting blocks on the board.
// 4) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Connect an oscilloscope probe to GND and P0.0
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Use SW2(P0.1) for Flash Write or SW3(P0.1) for Flash Erase.
//     a) When measuring for Flash Write, look at the time of the pin staying
//        low .
//     b) When measuring for Flash Erase, divide the time of the pin staying 
//        low by 5 (the time accounts for all 5 pages erased).
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.2 (BL)
//    - Updated Description / How to Test
//    - 6 FEB 2014
//
//
// Release 1.0
//    - Initial Revision (MT)
//    - 18 OCT 2013
//
//
// Refer to F96x_Flash_Access_Readme.txt for more information.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include "SI_C8051F960_Register_Enums.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SW_PRESSED           0
#define SW_NOT_PRESSED       1

SI_SBIT(SW2, SFR_P0, 1);
SI_SBIT(SW3, SFR_P0, 2);
SI_SBIT(LED1, SFR_P0, 0);

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
        if (SW2 == SW_PRESSED)        // Flash Write Initialization Switch
        {
            inputlatch = 1;
        }

        if ((SW2 == SW_NOT_PRESSED) && (inputlatch == 1))
        {
            Flash_Write();
            inputlatch = 0;
        }

        if (SW3 == SW_PRESSED)        // Flash Erase Initialization Switch
        {
            inputlatch2 = 1;
        }

        if ((SW3 == SW_NOT_PRESSED) && (inputlatch2 == 1))
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
 	//At clock frequencies above 10 MHz, the system clock cycle becomes 
    //short enough that the one-shot timer no longer provides a power benefit.
    //Disabling the one-shot timer at higher frequencies 
    //reduces power consumption. 
    SFRPAGE = CONFIG_PAGE;
    FLSCL     = 0x40;        
    SFRPAGE = LEGACY_PAGE;
   
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
    
}

//-----------------------------------------------------------------------------
// Port_Init
//
// Return Value : None
// Parameters   : None
//
//  This function configures the crossbar and GPIO ports.
//
// P0.0  digital  push-pull  output     measurement port
// P0.1  digital  open-drain input      Flash Write Initialization Switch
// P0.2  digital  open-drain input      Flash Erase Initialization Switch
//
//-----------------------------------------------------------------------------

void Port_Init (void)
{
    SFRPAGE   = LEGACY_PAGE ;
    P0MDOUT   = 0x01;
    P0 |= 0x07;
 
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
    uint8_t counter = 0;
    bit EA_SAVE = IE_EA;                   // Preserve IE_EA
    char xdata * data pwrite;           // Flash write pointer
    LED1 = 0;                           // Initialize LED as off
    IE_EA = 0;                             // Disable interrupts
    VDM0CN = 0x80;                      // Enable VDD monitor
    
    RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
    while(counter < 5) 
    {
        counter++;
        PSCTL |= 0x03;                  // PSWE = 1; PSEE = 1
        pwrite = (char xdata *) 0x0F00;

        FLKEY  = 0xA5;                  // Key Sequence 1
        FLKEY  = 0xF1;                  // Key Sequence 2
       
        *pwrite = 0;                    // Initiate page erase

        PSCTL &= ~0x03;                 // PSWE = 0; PSEE = 0        
    }
    IE_EA = EA_SAVE;                       // Restore interrupts
    LED1 = 1;                           // Turn LED Off when done
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
    int16_t counter = 0;
    bit EA_SAVE = IE_EA;                   // Preserve IE_EA
    char xdata * data pwrite;           // Flash write pointer
    
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
        
        LED1 = 0;                           // Turn LED on while write a byte
                                            // to flash.
        *pwrite = 0xA5;                 // Write the byte
        LED1 = 1;                           // Turn LED Off when done

        PSCTL &= ~0x01;                 // PSWE = 0 which disable writes

    }
    IE_EA = EA_SAVE;                       // Restore interrupts

}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
