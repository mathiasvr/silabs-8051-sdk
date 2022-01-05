/////////////////////////////////////////////////////////////////////////////
// project_config.h
/////////////////////////////////////////////////////////////////////////////

#ifndef PROJECT_CONFIG_H_
#define PROJECT_CONFIG_H_

#include "SI_EFM8LB1_Register_Enums.h"

#define CS00_MASK   0x01
#define CS01_MASK   0x02
#define CS02_MASK   0x04
#define CS03_MASK   0x08
#define CS04_MASK   0x10
#define CS05_MASK   0x20
#define CS06_MASK   0x40

/////////////////////////////////////////////////////////////////////////////
// Configuration
/////////////////////////////////////////////////////////////////////////////

#define CPT_DEMO_MODE				1		// 0 - STK works with CPT007B board with GPIO mode capsense buttons
											// 1 - STK works with CPT112S board with I2C mode capsense buttons

#if CPT_DEMO_MODE
#define TOTAL_CAPSENSE_PIN				12
#else
#define TOTAL_CAPSENSE_PIN				7
#endif

/////////////////////////////////////////////////////////////////////////////
// Pin Definitions
/////////////////////////////////////////////////////////////////////////////

SI_SBIT(SDA, SFR_P1, 2);
SI_SBIT(SCL, SFR_P1, 3);
SI_SBIT(CAPSENSE_INT, SFR_P0, 7);

#endif /* PROJECT_CONFIG_H_ */
