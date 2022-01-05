//-----------------------------------------------------------------------------
// F93x_Comparator_Hysteresis.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the hysteresis characteristics of the comparator,
// as specified by table 4.13 of the datasheet.
//
// Refer to F93x_Comparator_Hysteresis_Readme.txt for more information regarding
// the functionality of this code example.
//
// How To Test (Table 4.13 - Hysteresis):
// ---------------------------------------------------------
// 1) Ensure a shorting block is placed the J11 (VBAT & WALL_PWR) .
// 2) Place VDD Select (2 CELL) in the SW4 position and power the board
//    using the P2 power connector.
// 3) Remove all of the shorting blocks on the board except for the block(s)
//    across the following pins:
//    a) LED1.5 and P1.5 on J8
//    b) LED1.6 and P1.6 on J8
//    c) SW0.2 and P0.2H on J8
//    d) SW0.3 and P0.3H on J8
//    e) Imeasure on J17
// 4) Default is Positive Hysteresis Control. If measurement of the
//    Negative Hysteresis Control is desired, set #define NEGATIVE_DIRECITON  1
// 5) Download and run the code on a C8051F930-TB target board.
// 6) To cycle between Comparator mode, push SW0.2 (P0.2).DS3 and DS4
//    could indicate mode as below:
//
//      DS4  |  DS3  |  CP0 Mode
//    -------------------------------------
//      OFF  |  OFF  |   Mode 0
//      OFF  |  ON   |   Mode 1
//      ON   |  OFF  |   Mode 2
//      ON   |  ON   |   Mode 3
// 7) Attach oscilloscope probes to P0.0.
// 8) CP0 negative input is digital supply 1.8v. Use a power supply to sweep
//    the voltage at P0.6 as CP0 positive input voltage. Adjusting input voltage
//    refer to CP0 hysteresis character in Table 4.13. observe the P0.0 on the
//    scope.
// 9) To cycle between Hysteresis type, push SW0.3 (P0.3).

//
// Target:         C8051F93x/92x
// Tool Chain:     Generic
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (MT)
//    - 03 NOV 2013
//
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F930_Register_Enums.h>

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(DS3,   SFR_P1, 5);                // '0' means ON, '1' means OFF
SI_SBIT(DS4,   SFR_P1, 6);                // '0' means ON, '1' means OFF
SI_SBIT(SW0_2, SFR_P0, 2);                // SW0_2 == 0 means switch pressed
SI_SBIT(SW0_3, SFR_P0, 3);                // SW0_3 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK              20000000    // Clock speed in Hz
#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

#define LED_ON              0
#define LED_OFF             1

#define NEGATIVE_DIRECITON  0
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);                // Configure the system clock
void PORT_Init (void);                  // Configure the Crossbar and GPIO
void Wait_MS(unsigned int ms);          // Inserts programmable delay
void Mode_Update(void);                 // Cycle CP0 mode or turn on/off CP0
                                        // when push SW0_2 or SW0_3
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Debounce latches for SW0.2
uint8_t switch1Latch = 0;
// Debounce latches for SW0.3
uint8_t switch2Latch = 0;

// Mode type enumerations
enum COMPARATOR_MODE {MODE0 = 0,
                      MODE1 = 1,
                      MODE2 = 2,
                      MODE3 = 3};
						
enum COMPARATOR_MODE comparatorMode;

// Hysteresis type enumerations
enum HYSTERESIS_SEL {HYSTERESIS1 = 0,
                     HYSTERESIS2 = 1,
                     HYSTERESIS3 = 2,
                     HYSTERESIS4 = 3};

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
    PCA0MD &= ~0x40;                    // Disable Watchdog timer
}
 
enum HYSTERESIS_SEL hysteresisSel;
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{


    PORT_Init ();                       // Initialize crossbar and GPIO
    SYSCLK_Init();                      // Initialize the system clock

    // Default hysteresis1
    hysteresisSel = HYSTERESIS1;
    // Default Comparator0 enabled as Mode0
    comparatorMode = MODE0;
    CPT0CN = 0x80;
    CPT0MD = MODE0;

    // Comparator0 Negative Input Digital Supply 1.8v
    // Comparator0 Positive Input P0.6
    CPT0MX = 0xE3;

#if (NEGATIVE_DIRECITON == 1)
    CPT0CN |= hysteresisSel;
#else
    // Positive direction
    CPT0CN |= hysteresisSel << 2;
#endif

    while(1)
    {
        Mode_Update();
    }

}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// Pinout:
//
// P0.0   digital   push-pull           CP0 output
// P0.6   analog                        CP0 positive input

// P0.2   digital   open-drain          Switch 1
// P0.3   digital   open-drain          Switch 2

// P1.5   digital   push-pull           DS3
// P1.6   digital   push-pull           DS4
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
    // Switch
    P0MDIN  |=  0x0C;                   // P0.0, P0.2, P0.3 are digital
    P0MDIN  &= ~0x40;                   // P0.6 is analog
    P0MDOUT &= ~0x0D;                   // P0.0 P0.2, P0.3 are open-drain
    P0      |=  0x0D;                   // Set P0.2, P0.3 latches to '1'
    P0SKIP  |=  0x4C;                   // P0.2, P0.3 P0.6 skipped in Crossbar

    // LEDs
    P1MDIN  |= 0x60;                    // P1.5, P1.6 are digital
    P1MDOUT |= 0x60;                    // P1.5, P1.6 are push-pull
    P1      |= 0x60;                    // Set P1.5, P1.6 latches to '1'
    P1SKIP  |= 0x60;                    // P1.5, P1.6 skipped in Crossbar

    XBR0 = 0x10;                        // CP0 output available at P0.0
    XBR2    = 0x40;                     // Enable crossbar and enable
                                        // weak pull-ups
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal low power
// oscillator at its maximum frequency.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{

    // Select Low-Power Oscillator 20 MHz as system clock source
    CLKSEL = 0x04;
    OSCICN &= ~0x80;
    REG0CN &=~0x10;

    // At clock frequencies above 10 MHz, the system clock cycle becomes
    // short enough that the one-shot timer no longer provides a power benefit.
    // Disabling the one-shot timer at higher frequencies reduces power
    // consumption.
    FLSCL = 0x40;
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Mode_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function checks switch input and select CP0 mode or turn on/off CP0
// once each time the switch is pressed.
// The cycling order is as follows:
//
// Switch Sw0_2:
// Mode 0: CP0 Mode 0
// Mode 1: CP0 Mode 1
// Mode 2: CP0 Mode 2
// Mode 3: CP0 Mode 3
//
// Switch SW0_3:
// HYSTERESIS1
// HYSTERESIS2
// HYSTERESIS3
// HYSTERESIS4
//
// P0.2   digital   open-drain   input   SW0_2
// P0.3   digital   open-drain   input   SW0_3
//
//-----------------------------------------------------------------------------
void Mode_Update(void)
{
    // Cycle CP0 mode if P0.2 is pushed
    if( (SW0_2 == SW_PRESSED) && (!switch1Latch) )
    {
        switch (comparatorMode)
        {
            case MODE0:
                comparatorMode = MODE1;

                CPT0MD = MODE1;
                DS3 = LED_ON;           // Turn ON DS3
                DS4 = LED_OFF;          // Turn OFF DS4
                break;

            case MODE1:
                comparatorMode = MODE2;

                CPT0MD = MODE2;
                DS3 = LED_OFF;          // Turn OFF DS3
                DS4 = LED_ON;           // Turn ON DS4
                break;

            case MODE2:
                comparatorMode = MODE3;

                CPT0MD = MODE3;
                DS3 = LED_ON;           // Turn ON DS3
                DS4 = LED_ON;           // Turn ON DS4
                break;

            case MODE3:
                comparatorMode = MODE0;

                CPT0MD = MODE0;
                DS3 = LED_OFF;          // Turn OFF DS3
                DS4 = LED_OFF;          // Turn OFF DS4
                break;
        }
        CPT0CN = 0x80;
        // Wait 50 milliseconds
        Wait_MS(50);
        // Arm latches to prevent auto-cycling with switch held
        switch1Latch = 1;
    }

    // Reset latch once switch is released
    else if(SW0_2 == SW_NOT_PRESSED)
    {
        switch1Latch = 0;
    }

    // Toggle CP0 ON/OFF if P0.3 is pushed
    if( (SW0_3 == SW_PRESSED) && (!switch2Latch) )
    {
        switch(hysteresisSel)
        {
            case HYSTERESIS1:
                hysteresisSel = HYSTERESIS2;
                break;

            case HYSTERESIS2:
                hysteresisSel = HYSTERESIS3;
                break;

            case HYSTERESIS3:
                hysteresisSel = HYSTERESIS4;
                break;

            case HYSTERESIS4:
                hysteresisSel = HYSTERESIS1;
                break;
        }
#if (NEGATIVE_DIRECITON == 1)
        CPT0CN &= ~0x03;
        CPT0CN |= hysteresisSel;
#else
        // Positive direction
        CPT0CN &= ~0x0C;
        CPT0CN |= hysteresisSel << 2;
#endif
        // Wait 50 milliseconds
        Wait_MS(50);
        switch2Latch = 1;
    }

    // Reset latch once switch is released
    else if(SW0_3 == SW_NOT_PRESSED)
    {
        switch2Latch = 0;
    }
}
//-----------------------------------------------------------------------------
// Wait_MS
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters:
//   1) unsigned int ms - number of milliseconds of delay
//                        range is full range of integer: 0 to 256
//
// This routine inserts a delay of <ms> milliseconds.
//-----------------------------------------------------------------------------
void Wait_MS(unsigned int ms)
{
    uint8_t i;

    TCON_TR0 = 0;                            // Stop Timer0

    TMOD &= ~0x0F;                      // Timer0 in 8-bit mode
    TMOD |= 0x02;

    CKCON &= ~0x03;                     // Timer0 uses a 1:48 prescaler
    CKCON |= 0x02;


    TH0 = -SYSCLK/48/10000;             // Set Timer0 Reload Value to
                                        // overflow at a rate of 10kHz

    TL0 = TH0;                          // Init Timer0 low byte to the
                                        // reload value

    TCON_TF0 = 0;                            // Clear Timer0 Interrupt Flag
    IE_ET0 = 0;                            // Timer0 interrupt disabled
    TCON_TR0 = 1;                            // Timer0 on

    while(ms--)
    {
        for(i = 0; i < 10; i++)
        {
            TCON_TF0 = 0;
            while(!TCON_TF0);
        }
    }
    TCON_TF0 = 0;
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
