/////////////////////////////////////////////////////////////////////////////
// demo_config.h
/////////////////////////////////////////////////////////////////////////////

#ifndef DEMO_CONFIG_H_
#define DEMO_CONFIG_H_

/////////////////////////////////////////////////////////////////////////////
// Configuration
/////////////////////////////////////////////////////////////////////////////

#define PB_TIME_SET               BSP_JOYSTICK
#define PB_ANALOG_DIGITAL         BSP_PB1
#define PB_LCD_OFF                BSP_PB0

#define SHOW_ANALOG_CLOCK_FACE      1

#define SUSPEND            		    0x40        // Value to write to PMU0CF to place
                                                // the device in Suspend mode

#define SLEEP              		    0x80        // Value to write to PMU0CF to place
                                                // the device in Sleep Mode

#define POWER_MODE     			    SLEEP       // Select between Suspend and Sleep
                                                // mode. When debugging, if the
                                                // MCU is stopped/halted while
                                                // in Sleep Mode, the connection
                                                // to the IDE will be lost. The
                                                // IDE connection will not be lost
                                                // if the MCU is stopped/halted
                                                // while in suspend mode.

// Initial settings for time and date
#define HOURS_INIT           	    01          // Valid range of HOURS_INIT is 0-23
#define MINUTE_INIT          	    23          // Valid range of MINUTE_INIT is 0-59
#define SECOND_INIT          	    45          // Valid range of SECOND_INIT is 0-59

#endif /* DISP_CONFIG_H_ */
