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
// How To Test: EFM8BB1 STK + SI4455
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
// 8) The Range Test demo should be running on BOTH kits.
// 9) Read the instructions splash screen for user controls.
//	  North and South adjust the specified ID(Self or Destination)
//    Center switches between the IDs
//    Push buttons adjust the number of packets to be sent
//    West will enter TX mode where the demo can send packets
//    East will enter RX mode where the demo can receive packets
// 10) With all parameters set press East(RX) and West(TX) on each of the boards respectivly
//		Note: Be sure the IDs of the two boards match
//		Self ID of TX must equal Destination ID of RX
//		Destination ID of TX must equal the Self ID of RX
// 11) To change parameters push West on the Joystick which will return to the menu screen
// 12) On the TX board pressing Center on the joystick begins transmitting the specified number of packets
// 13) The RX board will indicate packets received, RSSI, and Percent sucessfully received
// 14) Center on the TX board will begin/stop transmission

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
//Busy Bee Kit Pins
// SYSCLK - 24.5 MHz HFOSC0 / 1
// ADC0   - 8-bit
// SPI0   - 1 MHz
// Timer0 - ADC start of conversion (31.25 kHz to 500 kHz)
// Timer2 - 2 MHz (SPI CS delay)
// Timer3 - 1 kHz (1 ms tick)
// P0.0 - Display enable
// P0.1 - CS (Active High)
// P0.2 - push button
// P0.3 - push button
// P0.6 - SCK
// P1.0 - MOSI
// P1.1 - External ADC input
// P1.4 - LED Green - > attached to Shutdown of Radio
// P1.5 - LED Blue
// P1.6 - LED Red
// P1.7 - Joystick (analog voltage divider)
//

// Radio Pins
// P0.0 - RF_NIRQ
// P0.6 - RF_SPI_CLK
// P0.7 - RF_SPI_MISO
// P1.0 - RF_SPI_MOSI
// P1.1 - RF_NSEL -> Active low
// P1.4 - RF_SDN
//

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "InitDevice.h"
#include "SI_EFM8BB1_Defs.h"
#include "master_bsp.h"
#include "main.h"
#include "render.h"
#include "disp.h"
#include <string.h>
#include "retargetserial.h"
#include "joystick.h"

/////////////////////////////////////////////////////////////////////////////
// Globals
/////////////////////////////////////////////////////////////////////////////
SI_SEGMENT_VARIABLE(sRange, sRange_t, SI_SEG_XDATA); // state and data
SI_SEGMENT_VARIABLE(str[29], char, SI_SEG_XDATA); //used for screen
SI_SEGMENT_VARIABLE(prev_joystick, uint8_t, SI_SEG_XDATA) = 0x08;
SI_SEGMENT_VARIABLE(transmitting, uint8_t, SI_SEG_DATA) = 0; //transmit flag

//this delay value adjusts how fast the packets are sent, if it's too low the numbers on the screen change too fast to read
#define PACKET_DELAY 25000

//This is used to for screen options so that if the button is held down the values don't increase too fast
#define DELAY_INTERVAL 2500


#define MAX_NODES 100
#define MAX_PACKETS 10000
#define MIN_NODES 0
#define MIN_PACKETS 0


/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////
void vSample_Code_Wait(unsigned long wWaitTime);
uint8_t GetJoystickDirection();
void sRangeInit();
void SetPacket();
void DemoScreenDraw();
void PrintString(SI_VARIABLE_SEGMENT_POINTER(str, char, SI_SEG_GENERIC), uint8_t posx, uint8_t posy);

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
	uint8_t dir;
	bool ID_Select = 0;

	// Enter default mode
	enter_DefaultMode_from_RESET();

	IE_EA = 1; //interrupt enable
	DISP_Init();
	DISP_ClearAll();
	vSample_Code_Wait(DELAY_INTERVAL);
	vRadio_Init();//LED Green will always be on from here on out.
	sRangeInit();

	while (1)
	{



		switch (sRange.DemoMode) {


		case SRANGE_DEMO_IDLE:

			vSample_Code_Wait(DELAY_INTERVAL);
			vRadio_StartRX(pRadioConfiguration->Radio_ChannelNumber);


			if(BSP_PB0 == BSP_PB_PRESSED)
			{

				sRange.PktCnt = (sRange.PktCnt >= MAX_PACKETS) ? MAX_PACKETS : ++sRange.PktCnt;
				vSample_Code_Wait(DELAY_INTERVAL);
			}
			else if(BSP_PB1 == BSP_PB_PRESSED)
			{

				sRange.PktCnt = (sRange.PktCnt <= MIN_PACKETS) ? MIN_PACKETS : --sRange.PktCnt;
				vSample_Code_Wait(DELAY_INTERVAL);
			}

			dir = GetJoystickDirection();
			if(dir != prev_joystick)
			{
				prev_joystick = dir;


				if(dir == JOYSTICK_C)
				{
					ID_Select = !ID_Select;

				}

				else if(dir == JOYSTICK_S)
				{

					if(ID_Select)
					{
						sRange.DestID = (sRange.DestID <= MIN_NODES) ? MAX_NODES : --sRange.DestID;
					}
					else sRange.SrcID = (sRange.SrcID <= MIN_NODES) ? MAX_NODES : --sRange.SrcID;

				}

				else if ( dir == JOYSTICK_N)
				{

					if(ID_Select)
					{
						sRange.DestID = (sRange.DestID >= MAX_NODES) ? MIN_NODES : ++sRange.DestID;
					}
					else sRange.SrcID = (sRange.SrcID >= MAX_NODES) ? MIN_NODES : ++sRange.SrcID;

				}
				else if(dir == JOYSTICK_E)
				{
					sRange.DemoMode = SRANGE_DEMO_RUN;
					sRange.RadioMode = SRADIO_MODE_RX;
					DISP_ClearAll();
					break;
				}
				else if(dir == JOYSTICK_W)
				{
					sRange.RadioMode = SRADIO_MODE_TX;
					sRange.DemoMode = SRANGE_DEMO_RUN;
					DISP_ClearAll();
					break;
				}
			}


			sprintf(str, "Joystick Options");
			PrintString(str, 0,0);
			sprintf(str, "West for TX Mode");
			PrintString(str, 0,10);
			sprintf(str, "East for RX Mode");
			PrintString(str, 0,20);
			sprintf(str, "Center to choose ID");
			PrintString(str, 0,30);
			sprintf(str, "North/South to set ID");
			PrintString(str, 0,40);
			sprintf(str, "PB0/PB1 sets Pkt Num");
			PrintString(str, 0,50);


			if(!ID_Select) sprintf(str, "SrcID:%d",(unsigned int) sRange.SrcID);
			else sprintf(str, "DestID:%d", (unsigned int)sRange.DestID);
			PrintString(str, 0,60);


			sprintf(str, "Num of Pkts:%d",(unsigned int) sRange.PktCnt);
			PrintString(str, 0,70);




			break;
		case SRANGE_DEMO_RUN:

			DemoScreenDraw();
			vSample_Code_Wait(DELAY_INTERVAL); // delay to allow the screen to let go of the SPI line
			dir = GetJoystickDirection();
			if(dir != prev_joystick)
			{
				prev_joystick = dir;
				if(dir==JOYSTICK_W) // West is go back, reset all
				{
					sRange.DemoMode = SRANGE_DEMO_IDLE;
					sRange.RSSI = 0;
					sRange.PERInt = 0;
					sRange.PacketSentCount = 0;
					sRange.PacketValidCount = 0;
					sRange.PktNum = 0;
					si4455_fifo_info(0x02); // clear FIFO
					DISP_ClearAll();

					break;
				}
			}

			if(sRange.RadioMode == SRADIO_MODE_TX) {

				if(GetJoystickDirection() == JOYSTICK_C) // begin/end transmit
				{
					sRange.PacketSentCount=0;
					sRange.PktNum=1;
					transmitting = 1;
					while(GetJoystickDirection() == JOYSTICK_C); //wait for unpressed
					while(GetJoystickDirection() != JOYSTICK_C && sRange.PktNum <= sRange.PktCnt)
					{

						sRange.PacketSentCount++;
						//Insert delay or packets will send too fast and the screen update will look bad
						vSample_Code_Wait(PACKET_DELAY);
						SetPacket();

						vRadio_StartTx(0, (unsigned char *) RadioPacket); // send the packet
						sRange.PktNum++;
						DemoScreenDraw();

						vSample_Code_Wait(PACKET_DELAY);
					}
					while(GetJoystickDirection() == JOYSTICK_C); //wait for unpressed


				}
				transmitting = 0;

				break;
			}
			else
			{

				if(gRadio_CheckReceived())
				{

					sRange.PERInt = (100*sRange.PacketValidCount) / (unsigned int) sRange.PktNum;

				}

				break;

			}
		default: break;
		}

	}
}

void DemoScreenDraw() {

	if(sRange.RadioMode == SRADIO_MODE_TX)
	{
		if(!transmitting)
		{
			sprintf(str, "Center to START");
			PrintString(str, 0, 100);
			sprintf(str, "WEST to go back");
			PrintString(str, 0, 110);
		}
		else
		{
			sprintf(str, "Center to STOP");
			PrintString(str, 0, 100);
			sprintf(str, "");
			PrintString(str, 0, 110);
		}
		sprintf(str, "In TX Mode");
		PrintString(str, 0, 0);
		sprintf(str, "CENTER begin/stop TX");
		PrintString(str, 0, 10);
		sprintf(str, "IDs:%d->%d", \
				(unsigned int) sRange.SrcID, (unsigned int) sRange.DestID);
		PrintString(str, 0, 20);
		sprintf(str, "Sent/Max");
		PrintString(str, 0, 30);
		sprintf(str, "%d/%d",(unsigned int) sRange.PacketSentCount,(unsigned int) sRange.PktCnt);
		PrintString(str, 0, 40);
	}
	else
	{
		sprintf(str, "In RX Mode");
		PrintString(str, 0, 0);
		sprintf(str, "IDs:%d->%d", \
				(unsigned int) sRange.SrcID, (unsigned int) sRange.DestID);
		PrintString(str, 0, 10);
		sprintf(str, "PER:%d",(unsigned int) sRange.PERInt);
		PrintString(str, 0, 20);
		sprintf(str, "RSSI:%d",(unsigned int) sRange.RSSI);
		PrintString(str, 0, 30);
		sprintf(str, "Rcvd/Sent");
		PrintString(str, 0, 40);
		sprintf(str, "%d/%d",(unsigned int) sRange.PacketValidCount,(unsigned int) sRange.PktNum);
		PrintString(str, 0, 50);
		sprintf(str, "WEST to go back");
		PrintString(str, 0, 110);
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

void PrintString(SI_VARIABLE_SEGMENT_POINTER(str, char, SI_SEG_GENERIC), uint8_t posx, uint8_t posy)
{
	uint8_t i;

	SI_SEGMENT_VARIABLE(line2[DISP_BUF_SIZE], char, SI_SEG_IDATA);

	for(i = 0; i < 8; i++)
	{
		RENDER_ClrLine(line2);
		RENDER_StrLine(line2, posx, i, str);
		DISP_WriteLine(posy, line2);
		posy++;
	}
}

uint8_t GetJoystickDirection()
{
	uint32_t mv;
	uint8_t dir;

	// Wait for conversion to complete
	ADC0CN0_ADINT = 0;
	while (!ADC0CN0_ADINT);
	ADC0CN0_ADINT = 0;

	mv = ((uint32_t)ADC0H) * 3300 / 256;
	dir = JOYSTICK_convert_mv_to_direction(mv);

	return dir;
}
void sRangeInit()
{

	uint8_t ii;

	sRange.RadioMode = SRADIO_MODE_RX;
	sRange.DestID = 0u;
	sRange.SrcID = 0u;
	sRange.PktPayload = 10u;
	sRange.PktNum = 0u;
	sRange.PktCnt = 20u;
	sRange.DemoMode = SRANGE_DEMO_IDLE;

	sRange.PERInt = 0u;
	sRange.PacketValidCount=0;
	sRange.PacketSentCount = 0;
	sRange.RSSI = 0;

	/* Init Payload */
	for (ii = 0u; ii < 7; ii++) {
		RadioPacket[ii] = 0x00;
	}
}

void SetPacket()
{
	RadioPacket[0] = sRange.SrcID;
	RadioPacket[1] = sRange.DestID;
	RadioPacket[2] = sRange.PktNum;
	RadioPacket[3] = sRange.PktCnt;
	RadioPacket[4] = 0xFF; //extra data space, can be used to send more information or to more nodes
	RadioPacket[5] = 0xFF;
	RadioPacket[6] = 0xFF;


}

