#include "SliderLibrary.h"
#include "SliderConfig.h"
#include <si_toolchain.h>
#include <stdio.h>
code char* SliderDescriptors[DEF_NUM_SLIDERS] =
{
"Slider1"
};

void OutputSliderDescriptors(void)
{
   uint8_t index, subindex;
   printf("*SLIDER_DESCRIPTORS ");
   for(index = 0; index < DEF_NUM_SLIDERS; index++)
   {
      printf("%s ",SliderDescriptors[index]);
      for(subindex = 0; subindex < Slider[index].length; subindex++)
      {
         printf("%bd ", Slider[index].channels[subindex]);
      }
      printf("|");
   }
   printf("\n");
}