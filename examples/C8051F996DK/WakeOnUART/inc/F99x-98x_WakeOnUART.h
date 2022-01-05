/*
 * F99x-98x_WakeOnUART.h
 *
 *  Created on: Jun 2, 2014
 *      Author: jiguo1
 */

#ifndef F99X_98X_WAKEONUART_MAIN_H_
#define F99X_98X_WAKEONUART_MAIN_H_

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK          24500000L      // SYSCLK frequency in Hz

#define TIMEOUT_MS         1           // Wait time in ms

#define WAKE_LED_ACTIVE    1           // 0 = Red LED will be always off
                                       // 1 = Red LED will be on if MCU is awake

#endif /* F99X_98X_WAKEONUART_MAIN_H_ */
