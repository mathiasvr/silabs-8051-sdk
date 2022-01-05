
#include "SliderLEDControl.h"
#include <si_toolchain.h>             // compiler declarations
#include "SI_C8051F970_Defs.h"
#include "SliderLibrary.h"
#define MARGIN 2

xdata LEDStateStruct LEDState[NUM_LED_STATES];

void Init_SliderLEDs(void);

void InitSliderLEDControl(void)
{
   Init_SliderLEDs();
   Init_LEDStruct();
}
void SliderLEDUpdate(uint8_t centroid, uint8_t active)
{
   static uint8_t last_val=0xFF;
   xdata uint8_t index, new_val;
   TurnOffLEDs();
   if(!active)
   {
      last_val = 0xFF;
      return;
   }
   if(active)
   {
      for(index = 0; index < NUM_LED_STATES; index++)
      {
         if((LEDState[index].MinState <= centroid) &&
            (LEDState[index].MaxState >= centroid))
         {
            new_val = index;
            break;
         }
      }
   } 
   if(last_val == 0xFF)
      last_val = new_val;

   if((new_val != (NUM_LED_STATES-1)) && ((new_val + 1) == last_val) )
   {
      if(centroid < (LEDState[last_val].MinState - MARGIN))
         last_val = new_val;
   }
   else if((new_val != 0) && ((new_val - 1) == last_val) )
   {
      if(centroid > (LEDState[last_val].MaxState + MARGIN))
         last_val = new_val;
   }
   else
   {
      last_val = new_val;
   }

   TurnOnLED(last_val);
}

void Init_SliderLEDs(void)
{
   xdata uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = 0x0F;
   P1MDOUT |= 0x08;
   P1MDOUT |= 0x04;
   P1MDOUT |= 0x10;
   P1MDOUT |= 0x40;

   SFRPAGE = SFRPAGE_save;
}

// This config is valid for the rev 2 F970 board, with 4 LEDs and 4 slider chevrons, yielding 7 slider states
void Init_LEDStruct(void)
{
   LEDState[0].MinState = SLIDER_MIN;
   LEDState[0].MaxState = SLIDER_MIN + 1*(SLIDER_MAX-SLIDER_MIN)/7;

   LEDState[1].MinState = SLIDER_MIN + 1*(SLIDER_MAX-SLIDER_MIN)/7 + 1;
   LEDState[1].MaxState = SLIDER_MIN + 2*(SLIDER_MAX-SLIDER_MIN)/7;

   LEDState[2].MinState = SLIDER_MIN + 2*(SLIDER_MAX-SLIDER_MIN)/7 + 1;
   LEDState[2].MaxState = SLIDER_MIN + 3*(SLIDER_MAX-SLIDER_MIN)/7;

   LEDState[3].MinState = SLIDER_MIN + 3*(SLIDER_MAX-SLIDER_MIN)/7 + 1;
   LEDState[3].MaxState = SLIDER_MIN + 4*(SLIDER_MAX-SLIDER_MIN)/7;

   LEDState[4].MinState = SLIDER_MIN + 4*(SLIDER_MAX-SLIDER_MIN)/7 + 1;
   LEDState[4].MaxState = SLIDER_MIN + 5*(SLIDER_MAX-SLIDER_MIN)/7;

   LEDState[5].MinState = SLIDER_MIN + 5*(SLIDER_MAX-SLIDER_MIN)/7 + 1;
   LEDState[5].MaxState = SLIDER_MIN + 6*(SLIDER_MAX-SLIDER_MIN)/7;

   LEDState[6].MinState = SLIDER_MIN + 6*(SLIDER_MAX-SLIDER_MIN)/7 + 1;
   LEDState[6].MaxState = SLIDER_MIN + 7*(SLIDER_MAX-SLIDER_MIN)/7;

   LEDState[7].MinState = SLIDER_MIN + 7*(SLIDER_MAX-SLIDER_MIN)/7 + 1;
   LEDState[7].MaxState = SLIDER_MIN + SLIDER_MAX;
}


void TurnOffLEDs(void)
{
  TurnOffLED(0);
  TurnOffLED(1);
  TurnOffLED(2);
  TurnOffLED(3);
  TurnOffLED(4);
  TurnOffLED(5);
  TurnOffLED(6);
}

void TurnOnLEDs(void)
{
  TurnOnLED(0);
  TurnOnLED(1);
  TurnOnLED(2);
  TurnOnLED(3);
  TurnOnLED(4);
  TurnOnLED(5);
  TurnOnLED(6);
}

void TurnOnLED(uint8_t index)
{
   switch(index)
   {
      case(0):
         P1 &= ~0x08;
         break;
      case(1):
         P1 &= ~0x04;
         P1 &= ~0x08;
         break;
      case(2):
         P1 &= ~0x04;
         break;
      case(3):
         P1 &= ~0x10;
         P1 &= ~0x04;

         break;
      case(4):
         P1 &= ~0x10;
         break;
      case(5):
         P1 &= ~0x40;
         P1 &= ~0x10;
         break;
      case(6):
         P1 &= ~0x40;
         break;
   }
}

void TurnOffLED(uint8_t index)
{
   switch(index)
   {
      case(0):
         P1 |= 0x08;
         break;
      case(1):
         P1 |= 0x04;
         P1 |= 0x08;
         break;
      case(2):
         P1 |= 0x04;
         break;
      case(3):
         P1 |= 0x10;
         P1 |= 0x04;

         break;
      case(4):
         P1 |= 0x10;
         break;
      case(5):
         P1 |= 0x40;
         P1 |= 0x10;
         break;
      case(6):
         P1 |= 0x40;
         break;
   }
}
