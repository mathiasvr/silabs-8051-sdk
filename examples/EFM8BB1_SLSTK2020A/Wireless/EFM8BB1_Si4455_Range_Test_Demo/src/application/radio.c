/*! @file radio.c
 * @brief This file contains functions to interface with the radio chip.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#include "..\master_bsp.h"

/*****************************************************************************
 *  Local Macros & Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Global Variables
 *****************************************************************************/
const SI_SEGMENT_VARIABLE(Radio_Configuration_Data_Array[], unsigned char, SI_SEG_CODE) = \
RADIO_CONFIGURATION_DATA_ARRAY;

const SI_SEGMENT_VARIABLE(RadioConfiguration, tRadioConfiguration, SI_SEG_CODE) = \
		RADIO_CONFIGURATION_DATA;

const SI_SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration, tRadioConfiguration, SI_SEG_CODE, SI_SEG_CODE) = \
		&RadioConfiguration;

SI_SEGMENT_VARIABLE(RadioPacket[7], uint8_t, SI_SEG_XDATA); // array to hold data

/*****************************************************************************
 *  Local Function Declarations
 *****************************************************************************/
void vRadio_PowerUp(void);

/*!
 *  Power up the Radio.
 *
 *  @note
 *
 */
void vRadio_PowerUp(void)
{
	SI_SEGMENT_VARIABLE(wDelay, uint16_t, SI_SEG_XDATA) = 0u;

	/* Hardware reset the chip */
	si4455_reset();

	/* Wait until reset timeout or Reset IT signal */
	for (; wDelay < pRadioConfiguration->Radio_Delay_Cnt_After_Reset; wDelay++);
}

/*!
 *  Radio Initialization.
 *
 *  @author Sz. Papp
 *
 *  @note
 *
 */
void vRadio_Init(void)
{
	uint16_t wDelay;

	/* Power Up the radio chip */
	vRadio_PowerUp();

	/* Load radio configuration */
	while (SI4455_SUCCESS != si4455_configuration_init(pRadioConfiguration->Radio_ConfigurationArray))
	{
		/* Error hook */
#if !(defined SILABS_PLATFORM_WMB912)

#else
		vCio_ToggleLed(eHmi_Led4_c);
#endif
		for (wDelay = 0x7FFF; wDelay--; ) ;

		/* Power Up the radio chip */
		vRadio_PowerUp();
	}

	// Read ITs, clear pending ones
	si4455_get_int_status(0u, 0u, 0u);
}

/*!
 *  Check if Packet received IT flag is pending.
 *
 *  @return   TRUE - Packet successfully received / FALSE - No packet pending.
 *
 *  @note
 *
 */
BIT gRadio_CheckReceived(void)
{

	/* Read ITs, clear pending ones */
	si4455_get_int_status(0u, 0u, 0u);

	/* check the reason for the IT */
	if (Si4455Cmd.GET_INT_STATUS.PH_PEND & SI4455_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT)
	{
		/* Packet RX */
		sRange.RSSI = sRadioGetRSSI();
		si4455_read_rx_fifo(RadioConfiguration.Radio_PacketLength,
				    (uint8_t *) &RadioPacket[0u]);
		// check if packet is for this node
		if(sRange.DestID == RadioPacket[0] && sRange.SrcID == RadioPacket[1])
		{
			if(RadioPacket[2] == 1)  // first packet in the sequence
			{
				sRange.PacketValidCount = 0;
			}
			sRange.PacketValidCount++;


			sRange.PktNum = RadioPacket[2];
			sRange.PktCnt = RadioPacket[3];
		}

		else
		{
			//if the packet is not for this node, clear FIFO
			si4455_fifo_info(0x02);
		}

		//put the chip back into receive mode
		si4455_start_rx(0u, 0u, RadioConfiguration.Radio_PacketLength,
				SI4455_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE,
				SI4455_CMD_START_RX_ARG_RXVALID_STATE_ENUM_READY,
				SI4455_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_READY );


		return TRUE;
	}


	return FALSE;
}

/*!
 *  Check if Packet sent IT flag is pending.
 *
 *  @return   TRUE / FALSE
 *
 *  @note
 *
 */
BIT gRadio_CheckTransmitted(void)
{
	// if (RF_NIRQ == FALSE)
	// {
	/* Read ITs, clear pending ones */
	si4455_get_int_status(0u, 0u, 0u);

	/* check the reason for the IT */
	if (Si4455Cmd.GET_INT_STATUS.PH_PEND & SI4455_CMD_GET_INT_STATUS_REP_PACKET_SENT_PEND_BIT)
	{
		return TRUE;
	}
	//}

	return FALSE;
}

/*!
 *  Set Radio to RX mode, fixed packet length.
 *
 *  @param channel Freq. Channel
 *
 *  @note
 *
 */
void vRadio_StartRX(uint8_t channel)
{
	// Read ITs, clear pending ones
	si4455_get_int_status(0u, 0u, 0u);

	/* Start Receiving packet, channel 0, START immediately, Packet n bytes long */
	si4455_start_rx(channel, 0u, RadioConfiguration.Radio_PacketLength,
			SI4455_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE,
			SI4455_CMD_START_RX_ARG_RXVALID_STATE_ENUM_READY,
			SI4455_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_READY );
}

/*!
 *  Set Radio to TX mode, fixed packet length.
 *
 *  @param channel Freq. Channel, Packet to be sent
 *
 *  @note
 *
 */
void  vRadio_StartTx(uint8_t channel, uint8_t *pioFixRadioPacket)
{
	// Read ITs, clear pending ones
	si4455_get_int_status(0u, 0u, 0u);

	/* Fill the TX fifo with datas */
	si4455_write_tx_fifo(RadioConfiguration.Radio_PacketLength, pioFixRadioPacket);

	/* Start sending packet, channel 0, START immediately, Packet n bytes long, go RX when done */
	si4455_start_tx(channel, 0x80,  RadioConfiguration.Radio_PacketLength);
}

unsigned char sRadioGetRSSI(void)
{
	/* FRR D holds the latched RSSI (if any). */
	si4455_frr_d_read(SI4455_CMD_REPLY_COUNT_FRR_D_READ);
	return Si4455Cmd.FRR_D_READ.FRR_D_VALUE;
}

