/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "HardwareConfig.h"
#include "ProjectConfig.h"

xdata TechSpecificStruct tech_spec[DEF_NUM_SENSORS];

#define PORT(port, pin) (port<<3) | (pin)


// Default values in a one-to-one mapping with the sensor node.  These values
// are copied into the sensor node entry for that sensor at startup.
SI_SEGMENT_VARIABLE(mux_values[DEF_NUM_SENSORS], uint8_t, SI_SEG_CODE) =
{

// TOUCH SENSE SWITCH
PORT(0,3),

};
// Default values in a one-to-one mapping with the sensor node.  These values
// are copied into the sensor node entry for that sensor at startup.
SI_SEGMENT_VARIABLE(gain_values[DEF_NUM_SENSORS], uint8_t, SI_SEG_CODE) =
{
0x07,

};

// Default values in a one-to-one mapping with the sensor node.  These values
// are copied into the sensor node entry for that sensor at startup.
SI_SEGMENT_VARIABLE(accumulation_values[DEF_NUM_SENSORS], uint8_t, SI_SEG_CODE) =
{
0x07,

};
