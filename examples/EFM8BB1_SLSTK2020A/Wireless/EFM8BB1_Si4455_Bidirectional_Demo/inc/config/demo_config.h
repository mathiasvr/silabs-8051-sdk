/////////////////////////////////////////////////////////////////////////////
// demo_config.h
/////////////////////////////////////////////////////////////////////////////

#ifndef DEMO_CONFIG_H_
#define DEMO_CONFIG_H_

/////////////////////////////////////////////////////////////////////////////
// Build Options
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Configuration
/////////////////////////////////////////////////////////////////////////////

SI_SBIT(SDA, SFR_P1, 2);
SI_SBIT(SCL, SFR_P1, 3);


#define POLL_SW_STATE           1
#define ENTER_SLEEP_MODE        0

#endif /* DEMO_CONFIG_H_ */
