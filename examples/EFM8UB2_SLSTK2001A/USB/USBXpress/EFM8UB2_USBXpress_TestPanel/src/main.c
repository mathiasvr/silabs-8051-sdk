/**************************************************************************//**
 * @file    main.c
 * @brief   Main routine for USBXpress Test Panel example
 *
 * This example communicates with the TestPanel Windows application installed
 * by the USBXpress SDK (typically installed at:
 * C:\SiliconLabs\MCU\USBXpress_SDK\Examples\TestPanel\Host\Windows).
 *
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

// -----------------------------------------------------------------------------
// Includes

#include <SI_EFM8UB2_Register_Enums.h>                // SFR declarations
#include "bsp.h"
#include "InitDevice.h"
#include "efm8_usbxpress.h"
#include "descriptor.h"
#include <stdint.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// Function Prototypes

void my_usbxp_callback(void);

// -----------------------------------------------------------------------------
// Variables

/// Last packet received from host
SI_SEGMENT_VARIABLE(outPacket[USB_PACKET_SIZE], uint8_t, SI_SEG_XDATA);

/// Next packet to sent to host
SI_SEGMENT_VARIABLE(inPacket[USB_PACKET_SIZE], uint8_t, SI_SEG_XDATA);

/// State of PB0
bool pb0State = false;

/// State of PB1
bool pb1State = false;

/// Current value of joystick
volatile uint8_t joystickValue;

/// Current measured temperature
volatile uint8_t temperature;

// -----------------------------------------------------------------------------
// Functions

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
 
/**************************************************************************//**
 * @brief Main loop
 *
 * The main loop sets up the device and then waits forever. All active tasks
 * are ISR driven.
 *
 *****************************************************************************/
void main(void)
{
  uint8_t portSave;

  enter_DefaultMode_from_RESET();

  USBX_init(&initStruct);
  USBX_apiCallbackEnable(my_usbxp_callback);

  BSP_LED_B = BSP_LED_OFF;
  BSP_LED_G = BSP_LED_OFF;
  BSP_LED_R = BSP_LED_OFF;

  // Enable global interrupts
  IE_EA = 1;

  // Spin forever
  while (1)
  {
    // It is possible that the contents of the following packets can change
    // while being updated.  This doesn't cause a problem in the sample
    // application because the bytes are all independent.  If data is NOT
    // independent, packet update routines should be moved to an interrupt
    // service routine, or interrupts should be disabled during data updates.

    // Update status of Green LED
    BSP_LED_G = (outPacket[0] == 1) ? BSP_LED_ON : BSP_LED_OFF;

    // Update status of Red LED
    BSP_LED_R = (outPacket[1] == 1) ? BSP_LED_ON : BSP_LED_OFF;

    // Set Port 1 pins
    portSave = (P1& 0xF0);
    P1 = portSave | (outPacket[2] & 0x0F);

    inPacket[0] = pb0State;       // Send status of switch 0
    inPacket[1] = pb1State;       // and switch 1 to host
    inPacket[2] = (P0 & 0x0F);    // Port 0 [0-3]
    inPacket[3] = joystickValue;  // Joystick Value
    inPacket[4] = temperature;    // Temperature sensor value
  }
}


// -----------------------------------------------------------------------------
// Interrupt Service Routines

/**************************************************************************//**
 * @brief USBXpress call-back
 *
 * This function is called by USBXpress. In this example any received data
 * sent back up to the host.
 *
 *****************************************************************************/
void my_usbxp_callback(void)
{
   uint16_t readLen;
   uint32_t intval = USBX_getCallbackSource();

   // Device Opened
   if (intval & USBX_DEV_OPEN)
   {
     // Enable the temperature sensor
     REF0CN |= REF0CN_TEMPE__ENABLED;

     // Start Timer 0 to enable joystick and temperatures sensor readings.
     TCON_TR0 = 1;

     USBX_blockRead(outPacket, USB_PACKET_SIZE, &readLen);
   }

   // Device Closed or Suspended
   if (intval & (USBX_DEV_CLOSE | USBX_DEV_SUSPEND))
   {
     // Stop Timer 0 to disable joystick and temperatures sensor readings.
     TCON_TR0 = 0;

     // Disable the temperature sensor
     REF0CN &= ~REF0CN_TEMPE__ENABLED;

     // Set the P1 pins high to save power
     P1 = 0xFF;

     // If entering suspend, call USBX_suspend() to enter low-power mode
     if (intval & USBX_DEV_SUSPEND)
     {
       USBX_suspend();
     }
   }

   // USB Read complete
   if (intval & USBX_RX_COMPLETE)
   {
     USBX_blockRead(outPacket, USB_PACKET_SIZE, &readLen);
   }
}
