//-----------------------------------------------------------------------------
// EFM8BB52_ProgrammableVREF.c
//-----------------------------------------------------------------------------
// Copyright 2020 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example code demonstrates the on-chip precision voltage reference and
// how to route to the VREF pin. The VREF pin is configured for analog mode,
// and this pin has two external capacitors (10 uF and 0.1 uF) tied through
// a series zero ohm resistor on the development kit. At least a 0.1 uF bypass
// capacitor is required when utilizing the VREF output.

// BTN0 and BTN1 are used to switch between the 4 programmable VREF output
// voltages: 1.2 V, 1.65V, 1.8V, and 2.4 V. The current VREF voltage is also
// printed to a terminal window via the UART.
//
// The system is clocked by the internal 24.5 MHz oscillator divided by 1.
// External interrupt service routines (ISR) handle the push button input and
// cycle through the VREF programmable settings.
//
// Terminal output is done via printf, which directs the characters to
// UART0. A UART initialization routine is therefore necessary.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   VREF   - on-chip programmable precision reference
//   UART0  - 115200 baud, 8-N-1
//   Timer1 - UART0 clock source
//   P0.0   - VREF output
//   P0.2   - Push button input (BTN0)
//   P0.3   - Push button input (BTN1)
//   P0.4   - UART0 TX
//   P0.5   - UART0 RX
//   P2.2   - Board Controller enable
//   P3.4   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB52 Pro Kit (PK)
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB52 PK board to a PC using a USB cable.
// 3) Compile and download code to the EFM8BB52 PK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the JLink CDC UART Port at 115200 baud rate and 8-N-1.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) Press push buttons and observe the output voltage on P0.0 using an
//    oscilloscope or digital multimeter. The terminal program will also print
//    the voltage setting for VREF output.
//
//
// Target:         EFM8BB52
// Tool chain:     Generic
//
// Release 0.1 (AG)
//    - Initial Revision
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB52_Register_Enums.h>
#include <InitDevice.h>
#include "retargetserial.h"

extern uint8_t vrefUpdate;           // Flag to send updated VREF info via UART
extern uint16_t vref;                // Initial value set in InitDevice.c

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P3, 4);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (BC_EN, SFR_P2, 2);            // Board Controller Enable
#define BC_DISCONNECTED 0              // 0 = Board Controller disconnected
                                       //     to EFM8 UART pins
#define BC_CONNECTED    1              // 1 = Board Controller connected
                                       //     to EFM8 UART pins

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
    enter_DefaultMode_from_RESET();

    DISP_EN = DISP_BC_DRIVEN;           // Display not driven by EFM8

    BC_EN = BC_CONNECTED;               // Board controller connected to EFM8
                                        // UART pins

    SCON0_TI = 1;                       // This STDIO library requires TI to
                                        // be set for prints to occur

    IE_EA = 1;                          // Enable global interrupts

    RETARGET_PRINTF ("Programmable VREF Example\n\n");

    RETARGET_PRINTF (" P0.0 VREF voltage: %d mV\n", vref); // Initial settings

    while (1)                           // Spin forever
    {
        if (vrefUpdate != 0x0)
        {
            RETARGET_PRINTF (" P0.0 VREF voltage: %d mV\n", vref);
            vrefUpdate = 0;
        }
    }
}
