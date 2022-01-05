/////////////////////////////////////////////////////////////////////////////
// project_config.h
/////////////////////////////////////////////////////////////////////////////

#ifndef PROJECT_CONFIG_H_
#define PROJECT_CONFIG_H_

#include "SI_EFM8BB2_Register_Enums.h"
 
/////////////////////////////////////////////////////////////////////////////
// Configuration
/////////////////////////////////////////////////////////////////////////////

#define SYSCLK_FREQ_MHz					          49
#define CLOCK_CYCLE_NS		  (1000/SYSCLK_FREQ_MHz)
#define FOR_LOOP_TIME_NS	     (CLOCK_CYCLE_NS*16) 		// tested value for uint16_t
#define MICRO_TO_NS								1000

#define CPT_DEMO_MODE				0		// 0 - STK works with CPT212B board
											// 1 - STK works with CPT213B board

#if CPT_DEMO_MODE == 1
#define CPT_DEVICE 213
#elif CPT_DEMO_MODE == 0
#define CPT_DEVICE 212
#endif

#define CS00_MASK   0x0001
#define CS01_MASK   0x0002
#define CS02_MASK   0x0004
#define CS03_MASK   0x0008
#define CS04_MASK   0x0010
#define CS05_MASK   0x0020
#define CS06_MASK   0x0040
#define CS07_MASK   0x0080
#define CS08_MASK   0x0100
#define CS09_MASK   0x0200
#define CS10_MASK   0x0400
#define CS11_MASK   0x0800
#if CPT_DEMO_MODE
#define CS12_MASK   0x1000
#endif
#if CPT_DEMO_MODE
#define TOTAL_CAPSENSE_PIN				13
#else
#define TOTAL_CAPSENSE_PIN				12
#endif

/////////////////////////////////////////////////////////////////////////////
// Pin Definitions
/////////////////////////////////////////////////////////////////////////////

SI_SBIT(CAPSENSE_INT, SFR_P0, 7);
SI_SBIT(CPT_RST, SFR_P1, 1);

extern uint8_t pkt_count;

#endif /* PROJECT_CONFIG_H_ */
