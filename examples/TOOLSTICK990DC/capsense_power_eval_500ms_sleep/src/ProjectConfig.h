#ifndef _PROJECT_CONFIG_H
#define _PROJECT_CONFIG_H
#include "SI_C8051F990_Register_Enums.h"


// Defines the size of the sensor node array, as well as other non-volatile and
// volatile arrays that have a one-to-one correspondence to the number of
// sensors in the project.
#define DEF_NUM_SENSORS 1



// Determines the expected difference between untouched sensor output
// and touched sensor output.
#define DEF_AVERAGE_TOUCH_DELTA           10000

// Gives a default value for the default thresholds used for
// the debounce active determination.  These are in units of capsense
// output codes
#define DEF_ACTIVE_SENSOR_DELTA            750
#define DEF_INACTIVE_SENSOR_DELTA          500

#define DEF_SINGLE_ACTIVE_SENSOR_DELTA     300


// Sets the number of consecutive samples that must fall above
// the ACTIVE_PERCENTAGE threshold before a channel is characterized
// as active
#define DEF_BUTTON_DEBOUNCE 2

// These frequency settings assume proper configuration of the timer source.  This
// configuration should be included in HardwareRoutines.c/h
#define DEF_ACTIVE_MODE_PERIOD  20
#define DEF_SLEEP_MODE_PERIOD 500

// Timer overflow events, based on ACTIVE_MODE_FREQUENCY
// set this to 1 for free run active mode sensing, 0 for active mode with low power
#define DEF_COUNTS_BEFORE_SLEEP     100

// This setting determines if an active mode algorithm waits in a low power state
// to stall out remaining time before 1/ACTIVE_MODE_FREQUENCY is reached since
// the last scan.  If this is set to 0, the system will continuously scan and
// update variables instead of stalling.  The ACTIVE_MODE_FREQUENCY setting is
// still used to determine the time that must pass with all sensors inactive
// before sleep mode is entered.
#define DEF_FREE_RUN_SETTING          1

// Set this to 1 to enable sleep mode sensing, set this to 0 to always remain in active mode
#define DEF_SLEEP_MODE_ENABLE         1




#endif
