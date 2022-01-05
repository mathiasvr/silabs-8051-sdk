#ifndef _LOW_POWER_CONFIG_H
#define _LOW_POWER_CONFIG_H


// Functions which must be defined with implementation-specific
// responsibilities.  These are called by LowPowerRoutines.c
void ConfigureSensorForSleepMode(void);
void ConfigureTimerForSleepMode(void);
void ConfigureTimerForActiveMode(void);
void EnterLowPowerState(void);
void CheckTimer(void);
extern xdata uint8_t TimerTick;


#endif