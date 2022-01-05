/*
 * SB1_WakeOnUART.h
 *
 *  Created on: Jun 2, 2014
 *      Author: jiguo1
 */

#ifndef EFM8SB2_98X_WAKEONUART_MAIN_H_
#define EFM8SB2_98X_WAKEONUART_MAIN_H_

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK          24500000L      // SYSCLK frequency in Hz

#define TIMEOUT_MS         1           // Wait time in ms

#define WAKE_LED_ACTIVE    1           // 0 = Red LED will be always off
                                       // 1 = Red LED will be on if MCU is awake

#endif /* EFM8SB2_98X_WAKEONUART_MAIN_H_ */
