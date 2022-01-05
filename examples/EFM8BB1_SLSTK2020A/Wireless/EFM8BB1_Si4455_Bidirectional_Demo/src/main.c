//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This demo provides a way to evaluate the Si4463 Sub-GHz radio along with the EFM8 Busy Bee 8-bit microcontroller
// It involves 2 complete kits that can communicate with each other
// The demo runs a 915MHZ, 2GFSK, 10kbps data rate and a packet size of 7
// The demo has configurable number of packets that are send
// Each kit will have a self ID and a destination ID
// The self ID is the ID of the board itself and the destination ID is the target board to communicate with
// The demo will display an receive sensitivity in receive mode when it gets a packet
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 STK + Si4455
//-----------------------------------------------------------------------------
// 1) Place the SW104 switch in "AEM" mode.
// 2) Connect the Expansion Board to the BB1 Starter kit.
// 3) Plug in the radio Pico board onto the Expansion board.
// 4) Connect the EFM8BB1 STK board to a PC using a mini USB cable.
// 5) Compile and download code to the EFM8BB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 6) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 7) Do this for another board utilizing the same type of radio Pico card.
// 8) The Bidirectional demo should be running on BOTH kits.
// 9) Both Kits will have the Green LED on which represents a State of 0
// 10) Pushing the buttons will cycle the LED and transmit the state of the board
// 11) On a receive the board will match the state of the sender and alter its LED accordingly


//
// Target:         EFM8BB1
// Tool chain:     Generic
//
// Release 0.1 (CM)
//    - Initial Revision
//    - 12 DEC 2015
//
//-----------------------------------------------------------------------------
// Resources
//-----------------------------------------------------------------------------
// SYSCLK - 24.5 MHz HFOSC0 / 1
// SPI0   - 1 MHz
// Timer2 - 2 MHz (SPI CS delay)
// Timer3 - 1 kHz (1 ms tick)
// P0.2 - push button
// P0.3 - push button
// P0.6 - SCK
// P1.0 - MOSI
// P1.4 - LED Green
// P1.5 - LED Blue
// P1.6 - LED Red
//

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "InitDevice.h"
#include "SI_EFM8BB1_Defs.h"
#include "master_bsp.h"
#include "main.h"

/////////////////////////////////////////////////////////////////////////////
// Globals
/////////////////////////////////////////////////////////////////////////////
SI_SEGMENT_VARIABLE(State, unsigned int, SI_SEG_DATA);


/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////
void vSample_Code_Wait(unsigned long wWaitTime);


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
 
//---------------------------------------------------------------------------
// main() Routine
// --------------------------------------------------------------------------
void main (void)
{

	// Enter default mode
	enter_DefaultMode_from_RESET();

	IE_EA = 1;

	vRadio_Init();//LED Green will always be on from here on out.

	fixRadioPacket[0]='S';
	fixRadioPacket[1]='T';
	fixRadioPacket[2]='A';
	fixRadioPacket[3]='T';
	fixRadioPacket[4]='E';
	fixRadioPacket[5]='=';
	State = 0x00;
	vRadio_StartRX(pRadioConfiguration->Radio_ChannelNumber);

	while (1)
	{

		switch(State)
		{

		case 0x00:
			BSP_LED_B = BSP_LED_OFF; //Green
			BSP_LED_R = BSP_LED_OFF;
			break;

		case 0x01:
			BSP_LED_B = BSP_LED_ON; //Blue
			BSP_LED_R = BSP_LED_OFF;
			break;
		case 0x02:
			BSP_LED_B = BSP_LED_OFF; //Yellow
			BSP_LED_R = BSP_LED_ON;
			break;
		case 0x03:
			BSP_LED_B = BSP_LED_ON; //ALL (white)
			BSP_LED_R = BSP_LED_ON;
			break;


		default:
			break;
		}


		if(BSP_PB1 == BSP_PB_PRESSED)
		{
			if(State == 0x00) State = 0x03;
			else State--;


			fixRadioPacket[6] = State;

			vRadio_StartTx(0, (unsigned char *) fixRadioPacket);

			while(BSP_PB1 == BSP_PB_PRESSED); //wait for release of button



		}

		else if(BSP_PB0 == BSP_PB_PRESSED)
		{
			if(State == 0x03) State = 0x00;
			else State++;


			fixRadioPacket[6] = State;

			vRadio_StartTx(0, (unsigned char *) fixRadioPacket);
			while(BSP_PB0 == BSP_PB_PRESSED); //wait for release of button


		}


		if (gRadio_CheckReceived())
		{
			//match the state of the sender
			State = fixRadioPacket[6];

		}



	}
}

void vSample_Code_Wait(unsigned long wWaitTime)
{
	SI_SEGMENT_VARIABLE(wDelay, unsigned long , SI_SEG_DATA) = wWaitTime;

	for (; wDelay--; )
	{
		NOP();
	}
}






