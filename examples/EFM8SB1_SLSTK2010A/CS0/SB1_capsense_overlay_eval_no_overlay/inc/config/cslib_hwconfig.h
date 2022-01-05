
#ifndef __SILICON_LABS_CSLIB_HWCONFIG_H
#define __SILICON_LABS_CSLIB_HWCONFIG_H
// Defines the size of the sensor node array, as well as other non-volatile and
// volatile arrays that have a one-to-one correspondence to the number of
// sensors in the project.
#define DEF_NUM_SENSORS 3

// Removing the last sensor, P1.4 is grounded by default on the SLSTK2010A
// $[mux values]
#define MUX_VALUE_ARRAY 2, 3, 13 //, 12
// [mux values]$

// $[gain values]
// 0 = gain of 1, 7 = gain of 8
// No overlay gain values
#define GAIN_VALUE_ARRAY 1, 1, 1 //, 1
// 1/16" overlay gain values
//#define GAIN_VALUE_ARRAY 7, 7, 7 //, 7
// 1/8" overlay gain values
//#define GAIN_VALUE_ARRAY 7, 7, 7 //, 7


// [gain values]$

// $[accumulation values]
#define ACCUMULATION_VALUE_ARRAY 3, 3, 3 //, 3
// [accumulation values]$

#define ACTIVE_THRESHOLD_ARRAY 70, 70, 70  //, 70
#define INACTIVE_THRESHOLD_ARRAY 30, 30, 30  //, 30

// No overlay average touch delta
#define AVERAGE_TOUCH_DELTA_ARRAY 4080>>4, 4080>>4, 4080>>4  //, 4080>>4
// 1/16" overlay average touch delta
//#define AVERAGE_TOUCH_DELTA_ARRAY 400>>4, 400>>4, 400>>4  //, 200>>4
// 1/8" overlay average touch delta
//#define AVERAGE_TOUCH_DELTA_ARRAY 200>>4, 200>>4, 200>>4  //, 200>>4

// Pin mask used for port register config in sleep mode
// $[Active mode mask]
#define ACTIVE_MODE_MASK_P0 	0x0C
//#define ACTIVE_MODE_MASK_P1     0x30 // P1.4 is grounded, uncomment this line if using the center button.
#define ACTIVE_MODE_MASK_P1     0x20
// [Active mode mask]$

// $[Sleep mode mask]
#define SLEEP_MODE_MASK_P0     0x00
#define SLEEP_MODE_MASK_P1     0x00
// [Sleep mode mask]$
#endif // __SILICON_LABS_CSLIB_HWCONFIG_H
