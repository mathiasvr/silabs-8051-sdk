/*!
 * File:
 *  si4455_api_lib.h
 *
 * Description:
 *  This file contains the Si4455 API library.
 *
 * Silicon Laboratories Confidential
 * Copyright 2011 Silicon Laboratories, Inc.
 */

#ifndef _SI4455_API_LIB_H_
#define _SI4455_API_LIB_H_
#define RADIO_DRIVER_EXTENDED_SUPPORT
#define RADIO_DRIVER_FULL_SUPPORT
extern SI_SEGMENT_VARIABLE( Si4455Cmd, union si4455_cmd_reply_union, SI_SEG_XDATA );
extern SI_SEGMENT_VARIABLE( radioCmd[16], uint8_t, SI_SEG_XDATA );


#define SI4455_FIFO_SIZE 64

enum
{
    SI4455_SUCCESS,
    SI4455_NO_PATCH,
    SI4455_CTS_TIMEOUT,
    SI4455_PATCH_FAIL,
    SI4455_COMMAND_ERROR
};

/* Minimal driver support functions */
void si4455_reset(void);
void si4455_power_up(uint8_t BOOT_OPTIONS, uint8_t XTAL_OPTIONS,
		     uint32_t XO_FREQ);

uint8_t si4455_configuration_init(const uint8_t * pSetPropCmd);

void si4455_write_ezconfig_array(uint8_t numBytes, uint8_t * pEzConfigArray);
void si4455_ezconfig_check(uint16_t CHECKSUM);

void si4455_start_tx(uint8_t CHANNEL, uint8_t CONDITION, uint16_t TX_LEN);
void si4455_start_rx(uint8_t CHANNEL, uint8_t CONDITION, uint16_t RX_LEN,
		     uint8_t NEXT_STATE1, uint8_t NEXT_STATE2,
		     uint8_t NEXT_STATE3);

void si4455_get_int_status(uint8_t PH_CLR_PEND, uint8_t MODEM_CLR_PEND,
			   uint8_t CHIP_CLR_PEND);

void si4455_gpio_pin_cfg(uint8_t GPIO0, uint8_t GPIO1, uint8_t GPIO2,
			 uint8_t GPIO3, uint8_t NIRQ, uint8_t SDO,
			 uint8_t GEN_CONFIG);

void si4455_set_property(uint8_t GROUP, uint8_t NUM_PROPS, uint8_t START_PROP,
			 ... );

void si4455_change_state(uint8_t NEXT_STATE1);

#ifdef RADIO_DRIVER_EXTENDED_SUPPORT
  /* Extended driver support functions */
  void si4455_nop(void);

  void si4455_fifo_info(uint8_t FIFO);
  void si4455_part_info(void);

  void si4455_write_tx_fifo(uint8_t numBytes, uint8_t * pData );
  void si4455_read_rx_fifo(uint8_t numBytes, uint8_t * pRxData );

  void si4455_get_property(uint8_t GROUP, uint8_t NUM_PROPS,
			   uint8_t START_PROP);

  #ifdef RADIO_DRIVER_FULL_SUPPORT
    /* Full driver support functions */
    void si4455_func_info(void);

    void si4455_frr_a_read(uint8_t respByteCount);
    void si4455_frr_b_read(uint8_t respByteCount);
    void si4455_frr_c_read(uint8_t respByteCount);
    void si4455_frr_d_read(uint8_t respByteCount);

    void si4455_read_cmd_buff(void);
    void si4455_request_device_state(void);

    void si4455_get_adc_reading(uint8_t ADC_EN, uint8_t ADC_CFG );

    void si4455_get_ph_status(uint8_t PH_CLR_PEND);
    void si4455_get_modem_status(uint8_t MODEM_CLR_PEND );
    void si4455_get_chip_status(uint8_t CHIP_CLR_PEND );

  #endif
#endif

#endif //_SI4455_API_LIB_H_
