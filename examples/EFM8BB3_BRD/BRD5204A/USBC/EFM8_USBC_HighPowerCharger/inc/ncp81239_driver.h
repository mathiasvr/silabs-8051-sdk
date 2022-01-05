/**************************************************************************//**
 * @file
 * @brief   Header file for NCP81239 driver
 *
 * @author  Silicon Laboratories
 * @version 1.0.0
 *
 * This header file contains the macro definition and function declarations
 * that support the NCP81239 driver.
 *******************************************************************************
 * @section License
 * (C) Copyright 2016 Silicon Labs Inc,
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *******************************************************************************
 * @addtogroup ncp81239_driver   NCP81239 Driver
 * @{
 *
 *****************************************************************************/

#ifndef __SILICON_LABS_NCP81239_DRIVER_H__
#define __SILICON_LABS_NCP81239_DRIVER_H__

// -----------------------------------------------------------------------------
// Includes

#include "bsp.h"

// -----------------------------------------------------------------------------
// Definitions

#define NCP81239_I2C_SLAVE_ADDR   (0x74 << 1) ///< I2C Slave Address of NCP81239

// -----------------------------------------------------------------------------
// Enums

/// NCP81239 Function Return Codes
typedef enum {
  NCP81239_STATUS_OK            = 0x0000,  ///< Success
  NCP81239_STATUS_ARBLOST_ERROR = 0x0100,  ///< I2C Arbitration Lost Error
  NCP81239_STATUS_NACK_ERROR    = 0x0101,  ///< I2C NAK Error
  NCP81239_STATUS_UNKNOWN_ERROR = 0x0102,  ///< Unknown Error
  NCP81239_STATUS_TXUNDER_ERROR = 0x0103,  ///< I2C Underrun Error
  NCP81239_STATUS_BUSY_ERROR    = 0x0104,  ///< I2C Busy Error
  NCP81239_STATUS_INVALID_SETTING = 0x0105,  ///< Invalid Setting
} ncp81239_error_t;

/// NCP81239 Registers
typedef enum {
  // Read-Write Registers
  NCP81239_REG_ENABLE = 0x00,         ///< Enable Bits
  NCP81239_REG_DAC_TARGET = 0x01,     ///< Output Target Voltage
  NCP81239_REG_SLEW_RATE = 0x02,      ///< Output Slew Rate
  NCP81239_REG_PWM_FREQUENCY = 0x03,  ///< PWM Frequency
  NCP81239_REG_FET = 0x04,            ///< FET Enables
  NCP81239_REG_OCP_CLIM = 0x05,       ///< Internal Current Limit Settings
  NCP81239_REG_CSx_CLIND = 0x06,      ///< Sense Current Limit Settings
  NCP81239_REG_GM_AMP_SETTING = 0x07, ///< Amplifier GM Settings
  NCP81239_REG_AMUX = 0x08,           ///< Analog Mux Settings
  NCP81239_REG_INT = 0x09,            ///< Interrupt Masks
  NCP81239_REG_INT_SHUTDOWN = 0x0A,   ///< Interrupt Shutdown Mask

  // Read-Only Registers
  NCP81239_REG_VFN = 0x10,            ///< VFN Value after ADC Read
  NCP81239_REG_VIN = 0x11,            ///< VIN Value after ADC Read
  NCP81239_REG_CS2 = 0x12,            ///< CS2 Value after ADC Read
  NCP81239_REG_CS1 = 0x13,            ///< CS1 Value after ADC Read
  NCP81239_REG_MISC = 0x14,           ///< Miscellaneous Values
  NCP81239_REG_SHUTDOWN = 0x15,       ///< Shutdown Value when int_shutdown set
} ncp81239_reg_t;

typedef enum {
  NCP81239_REG_ENABLE_DISABLE = 0x04, ///< Disable the regulator output
  NCP81239_REG_ENABLE_ENABLE = 0x0C,  ///< Enable the regulator output
} ncp81239_reg_enable_t;

typedef enum {
  NCP81239_SLEW_RATE_0P61_MVUS = 0x00,  ///< 0.61 mV/us
  NCP81239_SLEW_RATE_1P2_MVUS = 0x01,   ///< 1.2 mV/us
  NCP81239_SLEW_RATE_2P4_MVUS = 0x02,   ///< 2.4 mV/us
  NCP81239_SLEW_RATE_4P9_MVUS = 0x03,   ///< 4.9 mV/us
} ncp81239_slew_rate_t;

/***************************************************************************//**
 * @addtogroup ncp81239_driver_func    Functions in NCP81239 Driver
 *
 * The NCP81239 driver implements the following functions.
 *
 * @{
 ******************************************************************************/

/***************************************************************************//**
 *  @brief
 *    Initializes the NCP81239.
 *
 *  @note
 *    The NCP81239 output will be disabled after this function is called.
 *
 *  @return
 *    Result of the function call as type @ref ncp81239_error_t.
 *
 *****************************************************************************/
ncp81239_error_t ncp81239_init(void);

/***************************************************************************//**
 *  @brief
 *    Enables the NCP81239.
 *
 *  @return
 *    Result of the function call as type @ref ncp81239_error_t.
 *
 *****************************************************************************/
ncp81239_error_t ncp81239_enable(void);

/***************************************************************************//**
 *  @brief
 *    Disables the NCP81239.
 *
 *  @return
 *    Result of the function call as type @ref ncp81239_error_t.
 *
 *****************************************************************************/
ncp81239_error_t ncp81239_disable(void);

/***************************************************************************//**
 *  @brief
 *    Reads from an I2C Register of the NCP81239.
 *
 *  @param addr
 *    Address of I2C register
 *
 *  @param dat
 *    Pointer to write the data read from the I2C register
 *
 *  @return
 *    Result of the function call as type @ref ncp81239_error_t.
 *
 *****************************************************************************/
ncp81239_error_t ncp81239_readReg(uint8_t addr, uint8_t * dat);

/***************************************************************************//**
 *  @brief
 *    Writes to an I2C Register of the NCP81239.
 *
 *  @param addr
 *    Address of I2C register
 *
 *  @param dat
 *    Data to write to the I2C register
 *
 *  @return
 *    Result of the function call as type @ref ncp81239_error_t.
 *
 *****************************************************************************/
ncp81239_error_t ncp81239_writeReg(uint8_t addr, uint8_t dat);

/***************************************************************************//**
 *  @brief
 *    Sets the output voltage of the NCP81239.
 *
 *  @param outputMv
 *    Output voltage in millivolts.
 *
 *  @return
 *    Result of the function call as type @ref ncp81239_error_t.
 *
 *****************************************************************************/
ncp81239_error_t ncp81239_setOutputVoltage(uint16_t outputMv);

/***************************************************************************//**
 *  @brief
 *    Sets the slew rate of the NCP81239.
 *
 *  @param slewRate
 *    Target slew rate of type @ref ncp81239_slew_rate_t
 *
 *  @return
 *    Result of the function call as type @ref ncp81239_error_t.
 *
 *****************************************************************************/
ncp81239_error_t ncp81239_setSlewRate(ncp81239_slew_rate_t slewRate);

/** @}  (end addtogroup ncp81239_driver_func)  */
/** @}  (end addtogroup ncp81239_driver)  */

#endif /* __SILICON_LABS_NCP81239_DRIVER_H__ */
