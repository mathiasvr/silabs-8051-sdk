//------------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//------------------------------------------------------------------------------
//Supported Devices:
//  EFM8BB51F16G_QFN20
//  EFM8BB51F16G_TSSOP20
//  EFM8BB51F16I_QFN20
//  EFM8BB51F16I_TSSOP20
//  EFM8BB51F8G_QFN20
//  EFM8BB51F8G_TSSOP20
//  EFM8BB51F8I_QFN20
//  EFM8BB51F8I_TSSOP20

#ifndef SI_EFM8BB51_DEVICES_H
#define SI_EFM8BB51_DEVICES_H

#define EFM8BB51F16G_QFN20 0x13
#define EFM8BB51F16G_TSSOP20 0x14
#define EFM8BB51F16I_QFN20 0x17
#define EFM8BB51F16I_TSSOP20 0x18
#define EFM8BB51F8G_QFN20 0x12
#define EFM8BB51F8G_TSSOP20 0x11
#define EFM8BB51F8I_QFN20 0x16
#define EFM8BB51F8I_TSSOP20 0x15

#if (EFM8BB51_DEVICE == EFM8BB51F16G_QFN20)
#define DEVICE_DERIVID EFM8BB51F16G_QFN20
#define DEVICE_FLASH_SIZE 0x3800
#define DEVICE_XRAM_SIZE 0x0400
#define DEVICE_PKG_QFN20 1

#elif (EFM8BB51_DEVICE == EFM8BB51F16G_TSSOP20)
#define DEVICE_DERIVID EFM8BB51F16G_TSSOP20
#define DEVICE_FLASH_SIZE 0x3800
#define DEVICE_XRAM_SIZE 0x0400
#define DEVICE_PKG_TSSOP20 1

#elif (EFM8BB51_DEVICE == EFM8BB51F16I_QFN20)
#define DEVICE_DERIVID EFM8BB51F16I_QFN20
#define DEVICE_FLASH_SIZE 0x3800
#define DEVICE_XRAM_SIZE 0x0400
#define DEVICE_PKG_QFN20 1

#elif (EFM8BB51_DEVICE == EFM8BB51F16I_TSSOP20)
#define DEVICE_DERIVID EFM8BB51F16I_TSSOP20
#define DEVICE_FLASH_SIZE 0x3800
#define DEVICE_XRAM_SIZE 0x0400
#define DEVICE_PKG_TSSOP20 1

#elif (EFM8BB51_DEVICE == EFM8BB51F8G_QFN20)
#define DEVICE_DERIVID EFM8BB51F8G_QFN20
#define DEVICE_FLASH_SIZE 0x1800
#define DEVICE_XRAM_SIZE 0x0400
#define DEVICE_PKG_QFN20 1

#elif (EFM8BB51_DEVICE == EFM8BB51F8G_TSSOP20)
#define DEVICE_DERIVID EFM8BB51F8G_TSSOP20
#define DEVICE_FLASH_SIZE 0x1800
#define DEVICE_XRAM_SIZE 0x0400
#define DEVICE_PKG_TSSOP20 1

#elif (EFM8BB51_DEVICE == EFM8BB51F8I_QFN20)
#define DEVICE_DERIVID EFM8BB51F8I_QFN20
#define DEVICE_FLASH_SIZE 0x1800
#define DEVICE_XRAM_SIZE 0x0400
#define DEVICE_PKG_QFN20 1

#elif (EFM8BB51_DEVICE == EFM8BB51F8I_TSSOP20)
#define DEVICE_DERIVID EFM8BB51F8I_TSSOP20
#define DEVICE_FLASH_SIZE 0x1800
#define DEVICE_XRAM_SIZE 0x0400
#define DEVICE_PKG_TSSOP20 1

#endif

#endif // SI_EFM8BB51_DEVICES_H

//-eof--------------------------------------------------------------------------

