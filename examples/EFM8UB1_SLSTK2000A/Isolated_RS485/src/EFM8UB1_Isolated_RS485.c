//-----------------------------------------------------------------------------
// EFM8UB1_RS485.c
//-----------------------------------------------------------------------------
// Copyright 2016 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the EFM8UB1 to write to and
// read from the UART1 interface. This program is intended to be used in conjunction
// with the Isolated-RS485 expansion board (EXP).  It will take in data from the RS485 transceiver on the EXP
// and change it's state to match.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   UART1  - 115200 baud, 8-N-1
//   Timer1 - UART1 clock source
//   P0.4   - UART1 TX
//   P0.5   - UART1 RX
//   P2.0   - Board Controller enable
//   P2.3   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8UB1 STK + Isolated RS485 EXP
//
// Note: this example requires either two EFM8UB1 STKs and two Isolated RS485 EXPs
//       or one EFM8UB1 and one Isolated RS485 EXP plus an RS485 host
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8UB1 STK board to a PC using a mini USB cable.
// 3) Connect the Iso-RS485-EXP board to the STK
// 4) Compile and download code to the EFM8UB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) On a another RS485 RX, TX or TXR, connect the 485+ and 485- pins
// 6) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 7) By pressing and releasing the buttons, characters will be sent
//	  through the isolation barrier over RS485.
//
//
// Target:         EFM8UB1
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 10 OCT 2016
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8UB1_Register_Enums.h>
#include <stdio.h>
#include "InitDevice.h"
#include "Flags.h"
#include "disp.h"
#include "render.h"
#include "thinfont.h"
#include "bsp.h"
#include "tick.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT(DISP_EN, SFR_P2, 3);           // Display Enable
SI_SEGMENT_VARIABLE(Line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);
#define FONT_SCALE_DEFAULT             1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void initLCD(void);
void initialize_demo(void);
void drawstatus(uint8_t * str, uint8_t displine, uint8_t variable);

#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT(BC_EN, SFR_P2, 0);             // Board Controller Enable
#define BC_DISCONNECTED 0              // 0 = Board Controller disconnected
//     to EFM8 UART pins
#define BC_CONNECTED    1              // 1 = Board Controller connected
//     to EFM8 UART pins

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
// Variables in Interrupts.c
extern uint8_t TX_Ready;
extern uint8_t Byte;
SI_SBIT(LED, SFR_P1, 4);
SI_SBIT(DE, SFR_P1, 2); //Write enable for RS485 transceiver
uint8_t temp_SFR_page;
uint8_t initialize = 1;
//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main(void)
{
	enter_DefaultMode_from_RESET();

	IE_EA = 1;
	P1MDOUT |= 0x10;
	DISP_Init();
	initialize_demo();
	while (1) {

			if ((TX_Ready == 1) && (wish_to_send == 1) && ((increase == 1) || (decrease == 1))) {
			if (increase == 1) {
				if (Byte >= 0x5A || Byte < 0x41) { // if Byte is a Z or out of bounds
					Byte = 0x41; // make it an A
				} else { // otherwise increment by 1
					Byte++;
				}
				increase = 0;
			} else if (decrease == 1) {
				if (Byte <= 0x41 || Byte > 0x5A) { // if Byte is an A or out of bounds
					Byte = 0x5A; // make it a Z
				} else { // otherwise decrement by 1
					Byte--;
				}
				decrease = 0;
			}
			TX_Ready = 0;
			//Byte += 32; Option to make the letter a lower case
			drawstatus("Output: %c", 13, Byte);
			temp_SFR_page = SFRPAGE;
			SFRPAGE = UART1_PAGE;

			DE = 1;  //write enable to transceiver
			Wait(50); //wait for enable pin to be stable
			SCON1_TI = 1; //write interrupt to send data
			Wait(50); //wait for enable pin to stable
			DE = 0; //release the write enable

			SFRPAGE = temp_SFR_page;
		}
		if (received_data == 1) {
			received_data = 0;
			drawstatus("Input: %c", 13, Byte);
		}
	}
}

void initLCD(void)
{
	DISP_Init();
	DISP_ClearAll();
}

void drawstatus(uint8_t * str, uint8_t displine, uint8_t variable)
{
	uint8_t i;
	SI_SEGMENT_VARIABLE(txtStr[25], uint8_t, RENDER_STR_SEG);

	sprintf(txtStr, str, variable);

	for (i = 0; i < FONT_HEIGHT; i++) {
		RENDER_ClrLine(Line);
		RENDER_StrLine(Line, 4, i, txtStr);
		DISP_WriteLine(displine + i, Line);
	}
}

void initialize_demo(void)
{
	Byte = 0x41; //initially an A
	drawstatus("RS485 Demo", 5, 0);
	drawstatus("Idle: %c", 13, Byte);
	DE = 0;
}
