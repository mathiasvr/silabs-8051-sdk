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

#define SUSPEND                 0x40        // Value to write to PMU0CF to place
                                            // the device in Suspend mode

#define SLEEP                   0x80        // Value to write to PMU0CF to place
                                            // the device in Sleep Mode

#define POWER_MODE              SLEEP       // Select between Suspend and Sleep
                                            // mode. When debugging, if the
                                            // MCU is stopped/halted while
                                            // in Sleep Mode, the connection
                                            // to the IDE will be lost. The
                                            // IDE connection will not be lost
                                            // if the MCU is stopped/halted
                                            // while in suspend mode.
#define MEASUREMENT_INTERVAL    SECONDS
#define SECONDS                 0
#define MINUTES                 1
#define HOURS                   2
#define TWO_SECONDS             3

#define POLL_SW_STATE           1
#define ENTER_SLEEP_MODE        0

#endif /* DEMO_CONFIG_H_ */
