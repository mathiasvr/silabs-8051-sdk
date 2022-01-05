/*! @file radio.h
 * @brief This file is contains the public radio interface functions.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef RADIO_H_
#define RADIO_H_

/*****************************************************************************
 *  Global Macros & Definitions
 *****************************************************************************/
/*! Maximal packet length definition (FIFO size) */
#define RADIO_MAX_PACKET_LENGTH     64u

/*****************************************************************************
 *  Global Typedefs & Enums
 *****************************************************************************/
typedef struct
{
	unsigned char   * Radio_ConfigurationArray;

    uint8_t   Radio_ChannelNumber;
    uint8_t   Radio_PacketLength;
    uint8_t   Radio_State_After_Power_Up;

    uint16_t  Radio_Delay_Cnt_After_Reset;
} tRadioConfiguration;

/*****************************************************************************
 *  Global Variable Declarations
 *****************************************************************************/
extern const SI_SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration, tRadioConfiguration, SI_SEG_CODE, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(fixRadioPacket[RADIO_MAX_PACKET_LENGTH], uint8_t,
			SI_SEG_XDATA);

/*! Si4455 configuration array */
extern const SI_SEGMENT_VARIABLE(Radio_Configuration_Data_Array[], uint8_t,
			      SI_SEG_CODE);

/*****************************************************************************
 *  Global Function Declarations
 *****************************************************************************/
void  vRadio_Init(void);
BIT   gRadio_CheckReceived(void);
BIT   gRadio_CheckTransmitted(void);
void  vRadio_StartRX(uint8_t);
void  vRadio_StartTx(uint8_t, uint8_t *);
uint8_t    bRadio_Check_Ezconfig(uint16_t);

#endif /* RADIO_H_ */
