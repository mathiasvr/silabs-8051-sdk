#ifndef _HARDWARE_CONFIG_H
#define _HARDWARE_CONFIG_H

// Defines the size of the sensor node array, as well as other non-volatile and
// volatile arrays that have a one-to-one correspondence to the number of
// sensors in the project.
// $[Number of Sensors]
#define DEF_NUM_SENSORS                           2
// [Number of Sensors]$

// $[mux values]
#define MUX_VALUE_ARRAY 8, 9
// [mux values]$

// $[gain values]
#define GAIN_VALUE_ARRAY 3, 3
// [gain values]$

// $[accumulation values]
#define ACCUMULATION_VALUE_ARRAY 5, 5
// [accumulation values]$

#define ACTIVE_THRESHOLD_ARRAY   70, 70

#define INACTIVE_THRESHOLD_ARRAY 30, 30

// No overlay average touch delta
#define AVERAGE_TOUCH_DELTA_ARRAY 1000>>4, 1000>>4

// Pin mask used for port register config in sleep mode
// $[Active mode mask]
#define ACTIVE_MODE_MASK_P0 	0x00
#define ACTIVE_MODE_MASK_P1     0x03
// [Active mode mask]$

// $[Sleep mode mask]
#define SLEEP_MODE_MASK_P0     0x00
#define SLEEP_MODE_MASK_P1     0x03
// [Sleep mode mask]$
#endif




