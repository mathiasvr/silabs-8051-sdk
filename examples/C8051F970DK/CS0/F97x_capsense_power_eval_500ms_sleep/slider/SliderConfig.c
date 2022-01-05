
#include "SliderConfig.h"
#include "SliderLibrary.h"
#include <si_toolchain.h>

//SI_SEGMENT_VARIABLE (Slider[DEF_NUM_SLIDERS], SliderCSLIB_node, SI_SEG_XDATA);
xdata SliderCSLIB_node Slider[DEF_NUM_SLIDERS];

// Default values in a one-to-one mapping with a slider.  These values
// are copied into the sensor node entry for that sensor at startup using
// the slider initialization routines.

// This array stores the mux values for sensors of which the slider is
// composed.  The order in which these sensors are defined determines
// the centroid algorithm's processing.  They should match the contiguous 
// layout of the slider.
SI_SEGMENT_VARIABLE (slider0mux[SLIDER0_NUMCHANNELS], uint8_t, SI_SEG_XDATA) =
{
4,
6,
7,
5
};

SI_SEGMENT_VARIABLE (slider0binvalues[SLIDER0_NUMCHANNELS], uint8_t, SI_SEG_XDATA) =
{
10,
20,
30,
40
};

SI_SEGMENT_VARIABLE (NumSliders, uint8_t, SI_SEG_XDATA) = DEF_NUM_SLIDERS;

//-----------------------------------------------------------------------------
// Slider_AssignVariables
//-----------------------------------------------------------------------------
//
// Top-level call to initialize slider variables.
//
void InitSliders(void)
{
   ResetSliderStruct();
   AssignSliderChannelArray(0, slider0mux);   
   AssignBinArray(0, slider0binvalues);
   AssignSliderLength(0, SLIDER0_NUMCHANNELS);

}
