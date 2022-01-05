/////////////////////////////////////////////////////////////////////////////
// project_config.h
/////////////////////////////////////////////////////////////////////////////

#ifndef PROJECT_CONFIG_H_
#define PROJECT_CONFIG_H_

#include "SI_EFM8SB2_Register_Enums.h"

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
 
/////////////////////////////////////////////////////////////////////////////
// Configuration
/////////////////////////////////////////////////////////////////////////////

#define CPT_DEMO_MODE          1   // 0 - STK works with CPT007B board with GPIO mode capsense buttons
								   // 1 - STK works with CPT112S board with I2C mode capsense buttons

#if CPT_DEMO_MODE
#define TOTAL_CAPSENSE_PIN        12
#else
#define TOTAL_CAPSENSE_PIN        7
#endif

/////////////////////////////////////////////////////////////////////////////
// Pin Definitions
/////////////////////////////////////////////////////////////////////////////

SI_SBIT(SDA, SFR_P0, 6);
SI_SBIT(SCL, SFR_P0, 7);
SI_SBIT(CAPSENSE_INT, SFR_P1, 1);

#endif /* PROJECT_CONFIG_H_ */
