#ifndef _HARDWARE_CONFIG_H
#define _HARDWARE_CONFIG_H

#include "SI_C8051F990_Register_Enums.h"

typedef struct TechSpecificStruct {
   uint8_t mux;
   uint8_t accumulation;
   uint8_t gain;
} TechSpecificStruct;


xdata TechSpecificStruct tech_spec[];

extern SI_SEGMENT_VARIABLE(gain_values[], uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(mux_values[], uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(accumulation_values[], uint8_t, SI_SEG_CODE);

#endif
