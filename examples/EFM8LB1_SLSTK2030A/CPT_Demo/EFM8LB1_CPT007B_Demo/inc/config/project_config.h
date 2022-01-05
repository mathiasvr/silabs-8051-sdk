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

#define CPT_DEMO_MODE				0		// 0 - STK works with CPT007B board with GPIO mode capsense buttons
											// 1 - STK works with CPT112S board with I2C mode capsense buttons

#if CPT_DEMO_MODE
#define TOTAL_CAPSENSE_PIN				12
#else
#define TOTAL_CAPSENSE_PIN				7
#endif

/////////////////////////////////////////////////////////////////////////////
// Port sbits
/////////////////////////////////////////////////////////////////////////////

SI_SBIT(CS00, SFR_P0, 7);
SI_SBIT(CS01, SFR_P1, 1);
SI_SBIT(CS02, SFR_P1, 2);
SI_SBIT(CS03, SFR_P1, 3);
SI_SBIT(CS04, SFR_P3, 0);
SI_SBIT(CS05, SFR_P3, 1);
SI_SBIT(CS06, SFR_P3, 2);

#endif /* PROJECT_CONFIG_H_ */
