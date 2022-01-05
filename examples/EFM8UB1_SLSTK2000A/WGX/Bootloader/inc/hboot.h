/******************************************************************************
 * Copyright (c) 2018 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
#ifndef __HBOOT_H__
#define __HBOOT_H__

#define HBOOT_CMD_START "hboot "

/**************************************************************************//**
 * Wait for host connection
 *****************************************************************************/
void hboot_pollForHost(void);

/**************************************************************************//**
 * Request bootload file from host
 *****************************************************************************/
void hboot_requestBootload(void);

#endif
