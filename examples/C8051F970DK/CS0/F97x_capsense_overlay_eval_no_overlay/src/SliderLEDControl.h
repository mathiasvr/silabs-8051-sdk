#ifndef _SLIDERLEDCONTROL_H
#define _SLIDERLEDCONTROL_H
#include <si_toolchain.h>

typedef struct {
	uint8_t MinState;
	uint8_t MaxState;
}LEDStateStruct;

void SliderLEDUpdate(uint8_t centroid, uint8_t active);
void InitSliderLEDControl(void);

#define NUM_LED_STATES 7

#define SLIDER_MIN 10
#define SLIDER_MAX 40

void Init_LEDStruct(void);
void Init_TouchCharStruct(void);
void TurnOffLEDs(void);
void TurnOnLED(uint8_t index);
void TurnOffLED(uint8_t index);
void MultiFingerDetectOutput();
void TurnOnLEDs(void);
void IndicateTouches(uint8_t);
#endif
