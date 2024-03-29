/////////////////////////////////////////////////////////////////////////////
// adc.h
/////////////////////////////////////////////////////////////////////////////

#ifndef ADC_H_
#define ADC_H_

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include <board.h>

/////////////////////////////////////////////////////////////////////////////
// Configuration
/////////////////////////////////////////////////////////////////////////////

#define ADC_MEASUREMENTS_TO_AVG  40

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

uint32_t ADC_GetVoltage(void);

#endif /* ADC_H_ */
