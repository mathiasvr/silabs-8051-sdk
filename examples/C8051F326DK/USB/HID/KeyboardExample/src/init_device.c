/////////////////////////////////////
//  Generated Initialization File  //
/////////////////////////////////////

#include "si_toolchain.h"
//#include "C8051F326_defs.h"
#include "C8051F326.h"

// Peripheral specific initialization functions,
// Called from the Init_Device() function
void Port_IO_Init()
{
    P2MDOUT   = 0x0C;
}

void Oscillator_Init()
{
    int i = 0;
    CLKMUL    = 0x80;
    for (i = 0; i < 20; i++);    // Wait 5us for initialization
    CLKMUL    |= 0xC0;
    while ((CLKMUL & 0x20) == 0);
    CLKSEL    = 0x02;
}

// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
    Port_IO_Init();
    Oscillator_Init();
}
