#ifndef _LIBRARY_PUBLICS_H
#define _LIBRARY_PUBLICS_H

#include "SI_C8051F990_Register_Enums.h"
// Defines the depth of the buffers for raw and processed data 
// used in the sensor node struct.
#define DEF_SENSOR_BUFFER_SIZE 6


// Algorithm state init/update functions
void InitAlgorithmControlledHardware(void);
void InitAlgorithm(void);
void MasterAlgorithmTick(void);

// Sensor access API
typedef struct{
   uint16_t raw_buffer[DEF_SENSOR_BUFFER_SIZE];
   uint16_t process_buffer[DEF_SENSOR_BUFFER_SIZE];
   uint16_t current_baseline;
   uint16_t touch_delta;
   uint8_t single_active;
   uint8_t debounce_active;
   uint8_t active_debounce_count;
   uint8_t inactive_debounce_count;
//   uint8_t raw;
   uint16_t noise;
   int16_t baseline_accumulator;
//   uint8_t accumulation;
   SI_UU32_t exp_value;
} SensorNode;


uint8_t AnySensorActive(void);
void Init_SensorNodeRoutines(void);
void ResetSensorNode(uint8_t sensor_index, uint16_t fill_value);
uint8_t AnySensorSingleActive(void);
uint8_t IsSensorSingleActive(sensor_index);
uint8_t AnySensorDebounceActive(void);
uint8_t IsSensorDebounceActive(sensor_index);

uint8_t SensorNode_Push_raw(uint8_t sensor_index, uint16_t new_value);
uint8_t SensorNode_Push_process(uint8_t sensor_index, uint16_t new_value);

uint16_t SensorNode_ReadFromNewest_raw(uint8_t sensor_index, uint8_t index);
uint16_t SensorNode_ReadFromNewest_process(uint8_t sensor_index, uint8_t index);

void SensorNode_Replace_process(uint8_t, uint8_t, uint16_t);

uint8_t * GetInfoArrayPointer(void);

extern xdata SensorNode Node[];

extern SI_SEGMENT_VARIABLE(ActiveSensorDelta, uint16_t, SI_SEG_DATA);
extern SI_SEGMENT_VARIABLE(InactiveSensorDelta, uint16_t, SI_SEG_DATA);
extern SI_SEGMENT_VARIABLE(NumSensors, uint8_t, SI_SEG_DATA);
extern SI_SEGMENT_VARIABLE(SensorBufferSize, uint8_t, SI_SEG_DATA);
extern SI_SEGMENT_VARIABLE(AverageTouchDeltaDefault, uint16_t, SI_SEG_DATA);
extern SI_SEGMENT_VARIABLE(ActiveSensorDeltaDefault, uint16_t, SI_SEG_DATA);
extern SI_SEGMENT_VARIABLE(InactiveSensorDeltaDefault, uint16_t, SI_SEG_DATA);
extern SI_SEGMENT_VARIABLE(SingleActiveSensorDelta, uint16_t, SI_SEG_DATA);
extern SI_SEGMENT_VARIABLE(ButtonDebounce, uint8_t, SI_SEG_DATA);
extern SI_SEGMENT_VARIABLE(ActiveModeFrequency, uint16_t, SI_SEG_DATA);
extern SI_SEGMENT_VARIABLE(SleepModeFrequency, uint16_t, SI_SEG_DATA);
extern SI_SEGMENT_VARIABLE(CountsBeforeSleep, uint16_t, SI_SEG_DATA);
extern SI_SEGMENT_VARIABLE(FreeRunSetting, uint8_t, SI_SEG_DATA);
extern SI_SEGMENT_VARIABLE(SleepModeEnable, uint8_t, SI_SEG_DATA);
extern SI_SEGMENT_VARIABLE(AverageTouchDelta, uint16_t, SI_SEG_DATA);
// Low power routines
void LowPowerTick(void);

// Noise information
extern xdata uint16_t system_noise_average;
uint8_t CheckNoiseOnChannel(uint8_t index);

#endif
