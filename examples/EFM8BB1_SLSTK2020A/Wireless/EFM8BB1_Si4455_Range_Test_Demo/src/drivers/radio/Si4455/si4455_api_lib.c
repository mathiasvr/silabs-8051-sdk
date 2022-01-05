/*!
 * File:
 *  si4455_api_lib.c
 *
 * Description:
 *  This file contains the Si4455 API library.
 *
 * Silicon Laboratories Confidential
 * Copyright 2011 Silicon Laboratories, Inc.
 */

#include "..\..\..\master_bsp.h"
#include <stdarg.h>

#define SI4455_USER_CONFIG_XTAL_FREQ 30000000L
SI_SEGMENT_VARIABLE( Si4455Cmd, union si4455_cmd_reply_union, SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( radioCmd[16u], uint8_t, SI_SEG_XDATA );
#define RADIO_DRIVER_EXTENDED_SUPPORT

/*!
 * This functions is used to reset the si4455 radio by applying shutdown and
 * releasing it.  After this function @ref si4455_boot should be called.  You
 * can check if POR has completed by waiting 4 ms or by polling GPIO 0, 2, or 3.
 * When these GPIOs are high, it is safe to call @ref si4455_boot.
 */
void si4455_reset(void)
{
    uint8_t loopCount;

    /* Put radio in shutdown, wait then release */
    radio_hal_AssertShutdown();
    //! @todo this needs to be a better delay function.
    for (loopCount = 255; loopCount != 0; loopCount--);
    radio_hal_DeassertShutdown();
    for (loopCount = 255; loopCount != 0; loopCount--);
    radio_comm_ClearCTS();
}

void si4455_power_up(uint8_t BOOT_OPTIONS, uint8_t XTAL_OPTIONS, uint32_t XO_FREQ)
{
    radioCmd[0] = SI4455_CMD_ID_POWER_UP;
    radioCmd[1] = BOOT_OPTIONS;
    radioCmd[2] = XTAL_OPTIONS;
    radioCmd[3] = (uint8_t)(XO_FREQ >> 24);
    radioCmd[4] = (uint8_t)(XO_FREQ >> 16);
    radioCmd[5] = (uint8_t)(XO_FREQ >> 8);
    radioCmd[6] = (uint8_t)(XO_FREQ);

    radio_comm_SendCmd( SI4455_CMD_ARG_COUNT_POWER_UP, radioCmd );
}

/*!
 * This function is used to load all properties and commands with a list of NULL terminated commands.
 * Before this function @si446x_reset should be called.
 */
uint8_t si4455_configuration_init(const uint8_t* pSetPropCmd)
{
  SI_SEGMENT_VARIABLE(col, uint8_t, SI_SEG_DATA);
  SI_SEGMENT_VARIABLE(response, uint8_t, SI_SEG_DATA);
  SI_SEGMENT_VARIABLE(numOfBytes, uint8_t, SI_SEG_DATA);

  /* While cycle as far as the pointer points to a command */
  while (*pSetPropCmd != 0x00)
  {
    /* Commands structure in the array:
     * --------------------------------
     * LEN | <LEN length of data>
     */

    numOfBytes = *pSetPropCmd++;

    if (numOfBytes > 16u)
    {
        /* Initial configuration of Si4x55 */
        if ( SI4455_CMD_ID_WRITE_TX_FIFO == *pSetPropCmd ) 
        {
            if (numOfBytes > 128u)
            {
                /* Number of command bytes exceeds maximal allowable length */
                return SI4455_COMMAND_ERROR;
            }
            
            /* Load array to the device */
            pSetPropCmd++;
            si4455_write_ezconfig_array( numOfBytes - 1, pSetPropCmd);
            
            /* Point to the next command */
            pSetPropCmd += numOfBytes - 1;
            
            /* Continue command interpreter */
            continue;
            
        }
        else
        {
            /* Number of command bytes exceeds maximal allowable length */
            return SI4455_COMMAND_ERROR;
        }
    }

    for (col = 0u; col < numOfBytes; col++)
    {
      radioCmd[col] = *pSetPropCmd;
      pSetPropCmd++;
    }

    if (radio_comm_SendCmdGetResp(numOfBytes, radioCmd, 1, &response) != 0xFF)
    {
      /* Timeout occured */
      return SI4455_CTS_TIMEOUT;
    }

    /* Check response byte of EZCONFIG_CHECK command */
    if ( SI4455_CMD_ID_EZCONFIG_CHECK == radioCmd[0] ) 
    {
        if (response)
        {
            /* Number of command bytes exceeds maximal allowable length */
            return SI4455_COMMAND_ERROR;
        }    
    }
    
    if (radio_hal_NirqLevel() == 0)
    {
      /* Get and clear all interrupts.  An error has occured... */
      si4455_get_int_status(0, 0, 0);
      if (Si4455Cmd.GET_INT_STATUS.CHIP_PEND & SI4455_CMD_GET_CHIP_STATUS_REP_CMD_ERROR_PEND_MASK)
      {
        return SI4455_COMMAND_ERROR;
      }
    }
  }

  return SI4455_SUCCESS;
}

void si4455_write_ezconfig_array(uint8_t numBytes, uint8_t* pEzConfigArray)
{
  radio_comm_WriteData(SI4455_CMD_ID_WRITE_TX_FIFO, 1, numBytes, pEzConfigArray);
}

void si4455_ezconfig_check(uint16_t CHECKSUM)
{
  /* Do not check CTS */

  radio_hal_ClearNsel();

  /* Command byte */
  radio_hal_SpiWriteByte(SI4455_CMD_ID_EZCONFIG_CHECK);

  /* CRC */
  radio_hal_SpiWriteByte((uint16_t) CHECKSUM >> 8u);
  radio_hal_SpiWriteByte((uint16_t) CHECKSUM & 0x00FF);

  radio_hal_SetNsel();

  /* Get the respoonse from the radio chip */
  radio_comm_GetResp(1u, radioCmd);
}

void si4455_start_tx(uint8_t CHANNEL, uint8_t CONDITION, uint16_t TX_LEN)
{
    radioCmd[0] = SI4455_CMD_ID_START_TX;
    radioCmd[1] = CHANNEL;
    radioCmd[2] = CONDITION;
    radioCmd[3] = (uint8_t)(TX_LEN >> 8);
    radioCmd[4] = (uint8_t)(TX_LEN);

    radio_comm_SendCmd( SI4455_CMD_ARG_COUNT_START_TX, radioCmd );
}

void si4455_start_rx(uint8_t CHANNEL, uint8_t CONDITION, uint16_t RX_LEN, uint8_t NEXT_STATE1, uint8_t NEXT_STATE2, uint8_t NEXT_STATE3)
{
    radioCmd[0] = SI4455_CMD_ID_START_RX;
    radioCmd[1] = CHANNEL;
    radioCmd[2] = CONDITION;
    radioCmd[3] = (uint8_t)(RX_LEN >> 8);
    radioCmd[4] = (uint8_t)(RX_LEN);
    radioCmd[5] = NEXT_STATE1;
    radioCmd[6] = NEXT_STATE2;
    radioCmd[7] = NEXT_STATE3;

    radio_comm_SendCmd( SI4455_CMD_ARG_COUNT_START_RX, radioCmd );
}

void si4455_get_int_status(uint8_t PH_CLR_PEND, uint8_t MODEM_CLR_PEND, uint8_t CHIP_CLR_PEND)
{
    radioCmd[0] = SI4455_CMD_ID_GET_INT_STATUS;
    radioCmd[1] = PH_CLR_PEND;
    radioCmd[2] = MODEM_CLR_PEND;
    radioCmd[3] = CHIP_CLR_PEND;

    radio_comm_SendCmdGetResp( SI4455_CMD_ARG_COUNT_GET_INT_STATUS,
                              radioCmd,
                              SI4455_CMD_REPLY_COUNT_GET_INT_STATUS,
                              radioCmd );

    Si4455Cmd.GET_INT_STATUS.INT_PEND       = radioCmd[0];
    Si4455Cmd.GET_INT_STATUS.INT_STATUS     = radioCmd[1];
    Si4455Cmd.GET_INT_STATUS.PH_PEND        = radioCmd[2];
    Si4455Cmd.GET_INT_STATUS.PH_STATUS      = radioCmd[3];
    Si4455Cmd.GET_INT_STATUS.MODEM_PEND     = radioCmd[4];
    Si4455Cmd.GET_INT_STATUS.MODEM_STATUS   = radioCmd[5];
    Si4455Cmd.GET_INT_STATUS.CHIP_PEND      = radioCmd[6];
    Si4455Cmd.GET_INT_STATUS.CHIP_STATUS    = radioCmd[7];
}

void si4455_gpio_pin_cfg(uint8_t GPIO0, uint8_t GPIO1, uint8_t GPIO2, uint8_t GPIO3, uint8_t NIRQ, uint8_t SDO, uint8_t GEN_CONFIG)
{
    radioCmd[0] = SI4455_CMD_ID_GPIO_PIN_CFG;
    radioCmd[1] = GPIO0;
    radioCmd[2] = GPIO1;
    radioCmd[3] = GPIO2;
    radioCmd[4] = GPIO3;
    radioCmd[5] = NIRQ;
    radioCmd[6] = SDO;
    radioCmd[7] = GEN_CONFIG;

    radio_comm_SendCmdGetResp( SI4455_CMD_ARG_COUNT_GPIO_PIN_CFG,
                              radioCmd,
                              SI4455_CMD_REPLY_COUNT_GPIO_PIN_CFG,
                              radioCmd );

    Si4455Cmd.GPIO_PIN_CFG.GPIO0        = radioCmd[0];
    Si4455Cmd.GPIO_PIN_CFG.GPIO1        = radioCmd[1];
    Si4455Cmd.GPIO_PIN_CFG.GPIO2        = radioCmd[2];
    Si4455Cmd.GPIO_PIN_CFG.GPIO3        = radioCmd[3];
    Si4455Cmd.GPIO_PIN_CFG.NIRQ         = radioCmd[4];
    Si4455Cmd.GPIO_PIN_CFG.SDO          = radioCmd[5];
    Si4455Cmd.GPIO_PIN_CFG.GEN_CONFIG   = radioCmd[6];
}

#ifdef __C51__
#pragma maxargs (13)  /* allow 13 bytes for parameters */
#endif
void si4455_set_property( uint8_t GROUP, uint8_t NUM_PROPS, uint8_t START_PROP, ... )
{
    va_list argList;
    uint8_t cmdIndex;

    radioCmd[0] = SI4455_CMD_ID_SET_PROPERTY;
    radioCmd[1] = GROUP;
    radioCmd[2] = NUM_PROPS;
    radioCmd[3] = START_PROP;

    va_start (argList, START_PROP);
    cmdIndex = 4;
    while(NUM_PROPS--)
    {
        radioCmd[cmdIndex] = va_arg (argList, uint8_t);
        cmdIndex++;
    }
    va_end(argList);

    radio_comm_SendCmd( cmdIndex, radioCmd );
}

void si4455_change_state(uint8_t NEXT_STATE1)
{
    radioCmd[0] = SI4455_CMD_ID_CHANGE_STATE;
    radioCmd[1] = NEXT_STATE1;

    radio_comm_SendCmd( SI4455_CMD_ARG_COUNT_CHANGE_STATE, radioCmd );
}

#ifdef RADIO_DRIVER_EXTENDED_SUPPORT
void si4455_nop(void)
{
    radioCmd[0] = SI4455_CMD_ID_NOP;

    radio_comm_SendCmd( SI4455_CMD_ARG_COUNT_NOP, radioCmd );
}

void si4455_fifo_info(uint8_t FIFO)
{
    radioCmd[0] = SI4455_CMD_ID_FIFO_INFO;
    radioCmd[1] = FIFO;

    radio_comm_SendCmdGetResp( SI4455_CMD_ARG_COUNT_FIFO_INFO,
                              radioCmd,
                              SI4455_CMD_REPLY_COUNT_FIFO_INFO,
                              radioCmd );

    Si4455Cmd.FIFO_INFO.RX_FIFO_COUNT   = radioCmd[0];
    Si4455Cmd.FIFO_INFO.TX_FIFO_SPACE   = radioCmd[1];
}

void si4455_part_info(void)
{
    radioCmd[0] = SI4455_CMD_ID_PART_INFO;

    radio_comm_SendCmdGetResp( SI4455_CMD_ARG_COUNT_PART_INFO,
                              radioCmd,
                              SI4455_CMD_REPLY_COUNT_PART_INFO,
                              radioCmd );

    Si4455Cmd.PART_INFO.CHIPREV         = radioCmd[0];
    Si4455Cmd.PART_INFO.PART.u8[MSB]    = radioCmd[1];
    Si4455Cmd.PART_INFO.PART.u8[LSB]    = radioCmd[2];
    Si4455Cmd.PART_INFO.PBUILD          = radioCmd[3];
    Si4455Cmd.PART_INFO.ID.u8[MSB]      = radioCmd[4];
    Si4455Cmd.PART_INFO.ID.u8[LSB]      = radioCmd[5];
    Si4455Cmd.PART_INFO.CUSTOMER        = radioCmd[6];
    Si4455Cmd.PART_INFO.ROMID           = radioCmd[7];
    Si4455Cmd.PART_INFO.BOND            = radioCmd[8];
}

void si4455_write_tx_fifo(uint8_t numBytes, uint8_t* pTxData)
{
    radio_comm_WriteData( SI4455_CMD_ID_WRITE_TX_FIFO, 0, numBytes, pTxData );
}

void si4455_read_rx_fifo(uint8_t numBytes, uint8_t* pRxData)
{
    radio_comm_ReadData( SI4455_CMD_ID_READ_RX_FIFO, 0, numBytes, pRxData );
}

void si4455_get_property(uint8_t GROUP, uint8_t NUM_PROPS, uint8_t START_PROP)
{
    radioCmd[0] = SI4455_CMD_ID_GET_PROPERTY;
    radioCmd[1] = GROUP;
    radioCmd[2] = NUM_PROPS;
    radioCmd[3] = START_PROP;

    radio_comm_SendCmdGetResp( SI4455_CMD_ARG_COUNT_GET_PROPERTY,
                              radioCmd,
                              radioCmd[2],
                              radioCmd );

    Si4455Cmd.GET_PROPERTY.DATA0    = radioCmd[0];
    Si4455Cmd.GET_PROPERTY.DATA1    = radioCmd[1];
    Si4455Cmd.GET_PROPERTY.DATA2    = radioCmd[2];
    Si4455Cmd.GET_PROPERTY.DATA3    = radioCmd[3];
    Si4455Cmd.GET_PROPERTY.DATA4    = radioCmd[4];
    Si4455Cmd.GET_PROPERTY.DATA5    = radioCmd[5];
    Si4455Cmd.GET_PROPERTY.DATA6    = radioCmd[6];
    Si4455Cmd.GET_PROPERTY.DATA7    = radioCmd[7];
    Si4455Cmd.GET_PROPERTY.DATA8    = radioCmd[8];
    Si4455Cmd.GET_PROPERTY.DATA9    = radioCmd[9];
    Si4455Cmd.GET_PROPERTY.DATA10   = radioCmd[10];
    Si4455Cmd.GET_PROPERTY.DATA11   = radioCmd[11];
    Si4455Cmd.GET_PROPERTY.DATA12   = radioCmd[12];
    Si4455Cmd.GET_PROPERTY.DATA13   = radioCmd[13];
    Si4455Cmd.GET_PROPERTY.DATA14   = radioCmd[14];
    Si4455Cmd.GET_PROPERTY.DATA15   = radioCmd[15];
}

#ifdef RADIO_DRIVER_FULL_SUPPORT
void si4455_func_info(void)
{
    radioCmd[0] = SI4455_CMD_ID_FUNC_INFO;

    radio_comm_SendCmdGetResp( SI4455_CMD_ARG_COUNT_FUNC_INFO,
                              radioCmd,
                              SI4455_CMD_REPLY_COUNT_FUNC_INFO,
                              radioCmd );

    Si4455Cmd.FUNC_INFO.REVEXT          = radioCmd[0];
    Si4455Cmd.FUNC_INFO.REVBRANCH       = radioCmd[1];
    Si4455Cmd.FUNC_INFO.REVINT          = radioCmd[2];
    Si4455Cmd.FUNC_INFO.PATCH.u8[MSB]   = radioCmd[3];
    Si4455Cmd.FUNC_INFO.PATCH.u8[LSB]   = radioCmd[4];
    Si4455Cmd.FUNC_INFO.FUNC            = radioCmd[5];
    Si4455Cmd.FUNC_INFO.SVNFLAGS        = radioCmd[6];
    Si4455Cmd.FUNC_INFO.SVNREV.u8[B3]   = radioCmd[7];
    Si4455Cmd.FUNC_INFO.SVNREV.u8[B2]   = radioCmd[8];
    Si4455Cmd.FUNC_INFO.SVNREV.u8[B1]   = radioCmd[9];
    Si4455Cmd.FUNC_INFO.SVNREV.u8[B0]   = radioCmd[10];
}

void si4455_frr_a_read(uint8_t respByteCount)
{
    radio_comm_ReadData(SI4455_CMD_ID_FRR_A_READ,
                            0,
                        respByteCount,
                        radioCmd);

    Si4455Cmd.FRR_A_READ.FRR_A_VALUE = radioCmd[0];
    Si4455Cmd.FRR_A_READ.FRR_B_VALUE = radioCmd[1];
    Si4455Cmd.FRR_A_READ.FRR_C_VALUE = radioCmd[2];
    Si4455Cmd.FRR_A_READ.FRR_D_VALUE = radioCmd[3];
}


void si4455_frr_b_read(uint8_t respByteCount)
{
    radio_comm_ReadData(SI4455_CMD_ID_FRR_B_READ,
                            0,
                        respByteCount,
                        radioCmd);

    Si4455Cmd.FRR_B_READ.FRR_B_VALUE = radioCmd[0];
    Si4455Cmd.FRR_B_READ.FRR_C_VALUE = radioCmd[1];
    Si4455Cmd.FRR_B_READ.FRR_D_VALUE = radioCmd[2];
    Si4455Cmd.FRR_B_READ.FRR_A_VALUE = radioCmd[3];
}


void si4455_frr_c_read(uint8_t respByteCount)
{
    radio_comm_ReadData(SI4455_CMD_ID_FRR_C_READ,
                            0,
                        respByteCount,
                        radioCmd);

    Si4455Cmd.FRR_C_READ.FRR_C_VALUE = radioCmd[0];
    Si4455Cmd.FRR_C_READ.FRR_D_VALUE = radioCmd[1];
    Si4455Cmd.FRR_C_READ.FRR_A_VALUE = radioCmd[2];
    Si4455Cmd.FRR_C_READ.FRR_B_VALUE = radioCmd[3];
}


void si4455_frr_d_read(uint8_t respByteCount)
{
    radio_comm_ReadData(SI4455_CMD_ID_FRR_D_READ,
                            0,
                        respByteCount,
                        radioCmd);

    Si4455Cmd.FRR_D_READ.FRR_D_VALUE = radioCmd[0];
    Si4455Cmd.FRR_D_READ.FRR_A_VALUE = radioCmd[1];
    Si4455Cmd.FRR_D_READ.FRR_B_VALUE = radioCmd[2];
    Si4455Cmd.FRR_D_READ.FRR_C_VALUE = radioCmd[3];
}

void si4455_request_device_state(void)
{
    radioCmd[0] = SI4455_CMD_ID_REQUEST_DEVICE_STATE;

    radio_comm_SendCmdGetResp( SI4455_CMD_ARG_COUNT_REQUEST_DEVICE_STATE,
                              radioCmd,
                              SI4455_CMD_REPLY_COUNT_REQUEST_DEVICE_STATE,
                              radioCmd );

    Si4455Cmd.REQUEST_DEVICE_STATE.CURR_STATE       = radioCmd[0];
    Si4455Cmd.REQUEST_DEVICE_STATE.CURRENT_CHANNEL  = radioCmd[1];
}

void si4455_read_cmd_buff(void)
{
    radioCmd[0] = SI4455_CMD_ID_READ_CMD_BUFF;

    radio_comm_SendCmdGetResp( SI4455_CMD_ARG_COUNT_READ_CMD_BUFF,
                               radioCmd,
                               SI4455_CMD_REPLY_COUNT_READ_CMD_BUFF,
                               radioCmd );

    Si4455Cmd.READ_CMD_BUFF.CMD_BUFF0   = radioCmd[0];
    Si4455Cmd.READ_CMD_BUFF.CMD_BUFF1   = radioCmd[1];
    Si4455Cmd.READ_CMD_BUFF.CMD_BUFF2   = radioCmd[2];
    Si4455Cmd.READ_CMD_BUFF.CMD_BUFF3   = radioCmd[3];
    Si4455Cmd.READ_CMD_BUFF.CMD_BUFF4   = radioCmd[4];
    Si4455Cmd.READ_CMD_BUFF.CMD_BUFF5   = radioCmd[5];
    Si4455Cmd.READ_CMD_BUFF.CMD_BUFF6   = radioCmd[6];
    Si4455Cmd.READ_CMD_BUFF.CMD_BUFF7   = radioCmd[7];
    Si4455Cmd.READ_CMD_BUFF.CMD_BUFF8   = radioCmd[8];
    Si4455Cmd.READ_CMD_BUFF.CMD_BUFF9   = radioCmd[9];
    Si4455Cmd.READ_CMD_BUFF.CMD_BUFF10  = radioCmd[10];
    Si4455Cmd.READ_CMD_BUFF.CMD_BUFF11  = radioCmd[11];
    Si4455Cmd.READ_CMD_BUFF.CMD_BUFF12  = radioCmd[12];
    Si4455Cmd.READ_CMD_BUFF.CMD_BUFF13  = radioCmd[13];
    Si4455Cmd.READ_CMD_BUFF.CMD_BUFF14  = radioCmd[14];
    Si4455Cmd.READ_CMD_BUFF.CMD_BUFF15  = radioCmd[15];
}


void si4455_get_adc_reading(uint8_t ADC_EN, uint8_t ADC_CFG)
{
    radioCmd[0] = SI4455_CMD_ID_GET_ADC_READING;
    radioCmd[1] = ADC_EN;
    radioCmd[2] = ADC_CFG;

    radio_comm_SendCmdGetResp( SI4455_CMD_ARG_COUNT_GET_ADC_READING,
                              radioCmd,
                              SI4455_CMD_REPLY_COUNT_GET_ADC_READING,
                              radioCmd );

    Si4455Cmd.GET_ADC_READING.GPIO_ADC.u8[MSB]      = radioCmd[0];
    Si4455Cmd.GET_ADC_READING.GPIO_ADC.u8[LSB]      = radioCmd[1];
    Si4455Cmd.GET_ADC_READING.BATTERY_ADC.u8[MSB]   = radioCmd[2];
    Si4455Cmd.GET_ADC_READING.BATTERY_ADC.u8[LSB]   = radioCmd[3];
    Si4455Cmd.GET_ADC_READING.TEMP_ADC.u8[MSB]      = radioCmd[4];
    Si4455Cmd.GET_ADC_READING.TEMP_ADC.u8[LSB]      = radioCmd[5];
    Si4455Cmd.GET_ADC_READING.TEMP_SLOPE            = radioCmd[6];
    Si4455Cmd.GET_ADC_READING.TEMP_INTERCEPT        = radioCmd[7];
}


void si4455_get_ph_status(uint8_t PH_CLR_PEND)
{
    radioCmd[0] = SI4455_CMD_ID_GET_PH_STATUS;
    radioCmd[1] = PH_CLR_PEND;

    radio_comm_SendCmdGetResp( SI4455_CMD_ARG_COUNT_GET_PH_STATUS,
                              radioCmd,
                              SI4455_CMD_REPLY_COUNT_GET_PH_STATUS,
                              radioCmd );

    Si4455Cmd.GET_PH_STATUS.PH_PEND        = radioCmd[0];
    Si4455Cmd.GET_PH_STATUS.PH_STATUS      = radioCmd[1];
}


void si4455_get_modem_status( uint8_t MODEM_CLR_PEND )
{
    radioCmd[0] = SI4455_CMD_ID_GET_MODEM_STATUS;
    radioCmd[1] = MODEM_CLR_PEND;

    radio_comm_SendCmdGetResp( SI4455_CMD_ARG_COUNT_GET_MODEM_STATUS,
                              radioCmd,
                              SI4455_CMD_REPLY_COUNT_GET_MODEM_STATUS,
                              radioCmd );

    Si4455Cmd.GET_MODEM_STATUS.MODEM_PEND   = radioCmd[0];
    Si4455Cmd.GET_MODEM_STATUS.MODEM_STATUS = radioCmd[1];
    Si4455Cmd.GET_MODEM_STATUS.CURR_RSSI    = radioCmd[2];
    Si4455Cmd.GET_MODEM_STATUS.LATCH_RSSI   = radioCmd[3];
    Si4455Cmd.GET_MODEM_STATUS.ANT1_RSSI    = radioCmd[4];
    Si4455Cmd.GET_MODEM_STATUS.ANT2_RSSI    = radioCmd[5];
    Si4455Cmd.GET_MODEM_STATUS.AFC_FREQ_OFFSET.u8[MSB]  = radioCmd[6];
    Si4455Cmd.GET_MODEM_STATUS.AFC_FREQ_OFFSET.u8[LSB]  = radioCmd[7];
}


void si4455_get_chip_status( uint8_t CHIP_CLR_PEND )
{
    radioCmd[0] = SI4455_CMD_ID_GET_CHIP_STATUS;
    radioCmd[1] = CHIP_CLR_PEND;

    radio_comm_SendCmdGetResp( SI4455_CMD_ARG_COUNT_GET_CHIP_STATUS,
                              radioCmd,
                              SI4455_CMD_REPLY_COUNT_GET_CHIP_STATUS,
                              radioCmd );

    Si4455Cmd.GET_CHIP_STATUS.CHIP_PEND         = radioCmd[0];
    Si4455Cmd.GET_CHIP_STATUS.CHIP_STATUS       = radioCmd[1];
    Si4455Cmd.GET_CHIP_STATUS.CMD_ERR_STATUS    = radioCmd[2];
}


#endif

#endif
