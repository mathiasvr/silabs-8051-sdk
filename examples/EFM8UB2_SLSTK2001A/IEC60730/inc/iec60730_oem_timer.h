/**************************************************************************//**
 * Copyright (c) 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef INC_IEC60730_OEM_TIMER_H_
#define INC_IEC60730_OEM_TIMER_H_

/// Test clock frequency
#define TESTCLK  80000L
/// System clock frequency
#define SYSCLK   48000000L

/// Test timer overflow frequency
#define IEC_TEST_TIMER_OVERFLOW_FREQUENCY 10
/// Used to determine how many ticks +/- #iec60730_systemClockCounter can be from
/// nominal #iec60730_systemClockToTestClockFrequency before entering safe state.
#define IEC60730_CLOCK_TEST_TOLERANCE 5

#endif /* INC_IEC60730_OEM_TIMER_H_ */
