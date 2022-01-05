/**************************************************************************//**
 * @file    descriptor.h
 * @brief   External descriptor variable declarations for descriptor.c
 * @author  Silicon Laboratories
 *
 *******************************************************************************
 * @section License
 * (C) Copyright 2016 Silicon Labs Inc,
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *
 ******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

// -----------------------------------------------------------------------------
// Includes

#include "efm8_usbxpress.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// Constants

#define USB_PACKET_SIZE   8

// -----------------------------------------------------------------------------
// Extern Variables

extern const USBX_Init_t code initStruct;

#ifdef __cplusplus
}
#endif

#endif  // __DESCRIPTOR_H__
