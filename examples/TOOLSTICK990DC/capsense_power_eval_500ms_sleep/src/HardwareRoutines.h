#ifndef _HARDWARE_ROUTINES_H
#define _HARDWARE_ROUTINES_H
#include "SI_C8051F990_Register_Enums.h"


// Note: the functions below are hardware-specific callbacks used by the library to 
// perform capacitive sense scanning.  All must be defined
// in the project in order for the library to function correctly.
uint16_t ExecuteConversion(void);
uint16_t ScanSensor(uint8_t);
void ConfigureSensorForActiveMode(void);
void TechSpecificNodeInit(uint8_t sensor_index);


#define __HAS_DEVICE_LAYER

#endif
