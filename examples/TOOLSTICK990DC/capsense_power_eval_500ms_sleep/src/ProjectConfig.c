/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "ProjectConfig.h"
#include "CSLibrary.h"
#include "SI_C8051F990_Register_Enums.h"



// Struct defined in SensorNodeRoutines.h.  Contains all state data for a
// sensor.
SI_SEGMENT_VARIABLE(Node[DEF_NUM_SENSORS], SensorNode, SI_SEG_XDATA);

SI_SEGMENT_VARIABLE(NumSensors, uint8_t, SI_SEG_DATA)                        = DEF_NUM_SENSORS;
SI_SEGMENT_VARIABLE(SensorBufferSize, uint8_t, SI_SEG_DATA)                  = DEF_SENSOR_BUFFER_SIZE;
SI_SEGMENT_VARIABLE(SingleActiveSensorDelta, uint16_t, SI_SEG_DATA)          = DEF_SINGLE_ACTIVE_SENSOR_DELTA;
SI_SEGMENT_VARIABLE(ButtonDebounce, uint8_t, SI_SEG_DATA)                    = DEF_BUTTON_DEBOUNCE;
SI_SEGMENT_VARIABLE(ActiveModeFrequency, uint16_t, SI_SEG_DATA)              = DEF_ACTIVE_MODE_PERIOD;
SI_SEGMENT_VARIABLE(SleepModeFrequency, uint16_t, SI_SEG_DATA)               = DEF_SLEEP_MODE_PERIOD;
SI_SEGMENT_VARIABLE(CountsBeforeSleep, uint16_t, SI_SEG_DATA)                = DEF_COUNTS_BEFORE_SLEEP;
SI_SEGMENT_VARIABLE(FreeRunSetting, uint8_t, SI_SEG_DATA)                    = DEF_FREE_RUN_SETTING;
SI_SEGMENT_VARIABLE(SleepModeEnable, uint8_t, SI_SEG_DATA)                   = DEF_SLEEP_MODE_ENABLE;
SI_SEGMENT_VARIABLE(ActiveSensorDelta, uint16_t, SI_SEG_DATA)                = DEF_ACTIVE_SENSOR_DELTA;
SI_SEGMENT_VARIABLE(InactiveSensorDelta, uint16_t, SI_SEG_DATA)              = DEF_INACTIVE_SENSOR_DELTA;
SI_SEGMENT_VARIABLE(AverageTouchDelta, uint16_t, SI_SEG_DATA)                = DEF_AVERAGE_TOUCH_DELTA;
