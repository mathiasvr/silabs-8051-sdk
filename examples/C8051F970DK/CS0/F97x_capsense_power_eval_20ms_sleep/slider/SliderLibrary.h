#ifndef _SLIDER_PUBLICS_H
#define _SLIDER_PUBLICS_H
#include <si_toolchain.h>
// Defines the depth of the buffers for the slider struct.
#define SLIDER_BUFFER_SIZE 5

typedef struct{
	uint16_t centroid;
	uint16_t buffer[SLIDER_BUFFER_SIZE];
   uint8_t length;
   uint8_t* channels;
   uint8_t* bins;
   uint8_t active;
}SliderCSLIB_node;

extern xdata SliderCSLIB_node Slider[];


void AssignSliderChannelArray(uint8_t, uint8_t*);
void AssignBinArray(uint8_t, uint8_t*);

uint8_t Slider_Push(uint8_t slider_index, uint16_t new_value);
void Slider_Replace(uint8_t slider_index, uint8_t buffer_index, uint16_t new_value);
uint16_t Slider_ReadFromNewest(uint8_t slider_index, int8_t index);
uint8_t IsSliderActive(uint8_t slider_index);
void AssignSliderLength(uint8_t, uint8_t);
void ResetSliderStruct(void);
void AssignSliderVariables(void);
void UpdateSliders(void);
void InitSliders(void);
// Note that Slider_AssignVariables is defined in the ProjectConfig.c file
void Slider_AssignVariables(void);
uint8_t* GetSliderInfoArrayPointer(void);

extern SI_SEGMENT_VARIABLE (NumSliders, uint8_t, SI_SEG_XDATA);

#endif
