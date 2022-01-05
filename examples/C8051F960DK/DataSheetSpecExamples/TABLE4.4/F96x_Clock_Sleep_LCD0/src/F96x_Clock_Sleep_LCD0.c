//-----------------------------------------------------------------------------
// F96x_Clock_Sleep_LCD0.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the current consumption while driving an LCD in
// low-power sleep mode.
//
// To configure the LCD to the desired low-current mode, modify the settings in
// F96x_Clock_Sleep_LCD0_Config.h
//
// Refer to F96x_Clock_Sleep_LCD0_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.4 - LCD Sleep Mode Current):
// -----------------------------------------------
//
// To Use with the UDP F960 MUX LCD board:
//
// To measure current consumption:
// 1) Cut the trace on the bottom of the PCB between the two pins of the VBAT
//    CURRENT header J15.
// 2) Cut the trace on the bottom of the PCB between the two pins of the header
//    J20.
// 3) Connect a current meter between the two pins on header J15 to measure
//    the current draw during sleep mode.
// 4) Set the VBAT Selector SW5 to the desired power source.
// 5) Set VIO Select (SW7) and VIORF(SW12) Select to VBAT.
// 6) Select the desired LCD configuration in F96x_Clock_Sleep_LCD0_Config.h
// 7) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 8) Disconnect the debugger in order to reduce current draw. Apply the
//    the desired power source to run the code.
// 9) Initially, the device will be running the LCD in low-power mode. To
//    toggle between sleep mode with the LCD and without the LCD, press
//    SW2 (P0.1).
//
// To measure LCD Drive Voltage:
// 1) If current measurements are not desired, follow the above steps
//    minus the steps that require trace-cutting.
// 2) If using one of the non-bypass modes, measure VLCD by attaching a
//    voltmeter between GND and one of the LCD drive pins, such as P4.0.
// 3) The square wave generated will have a maximum voltage of VLCD. In
//    constant contrast mode, this will be equal to VLCD_mV, regardless of the
//    value of Vbat. This may be Vbat in the other two regulated modes,
//    auto-bypass and minimum contrast.
//
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 6 FEB 2014
//
// Release 1.0
//    - Initial Revision (MT)
//    - 09 NOV 2013
//
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F960_Register_Enums.h"
#include "F96x_Clock_Sleep_LCD0.h"
#include "F96x_Clock_Sleep_LCD0_Config.h"
//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(SW2, SFR_P0, 1);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void LCD_Init (void);
void Port_Init (void);
void Enter_Sleep (void);
void RTC_Init (void);

void LCD0_ConfigClear(void);
void LCD0_ConfigMuxMode(uint8_t muxMode);
void LCD0_ConfigSegments(uint8_t segments);
void LCD0_SetContrast(uint16_t contrast);
void LCD0_ConfigContrastMode(uint8_t contrastMode);
void LCD0_ConfigRefresh(uint8_t refresh, uint8_t muxMode);
void LCD0_ConfigEnable(void);
//-----------------------------------------------------------------------------
// Global Variable
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
    PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
    
                                        // enable)
    // Initialize Port I/O (SW2)
    Port_Init();             
   
    // Initialize Oscillator (Default is 20.0 MHz LPOSC)
    Oscillator_Init();  
    
    //Enable the LCD and go to sleep.
    LCD_Init();

    Enter_Sleep();

    while (1)
    {
       //The device will wake when the switch is pressed, wait for it to be
       // released before continuing.
        while( SW2 == SW_PRESSED );

        // Turn off the LCD and go back to sleep.
        LCD0_ConfigClear();
         
        // Sleep mode, ends upon SW2 Port Match event
        Enter_Sleep();

        //The device will wake when the switch is pressed, wait for it to be
        // released before continuing.
        while( SW2 == SW_PRESSED );

        //Enable the LCD and go to sleep.
        LCD_Init();

        // Sleep mode, ends upon SW2 Port Match event
        Enter_Sleep();
   } 
}

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to Low Power frequency
// oscillator at the maximum frequency.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
 
#if (VBAT_MONITOR == ENABLED)
    // Enable VBAT Supply monitor
    VDM0CN = 0x08;
    // waiting for VBAT above the VBAT warn monitor threshold
    while (!(VDM0CN & 0x02));
    
    RSTSRC = 0x02;                      // Enable missing clock detector and
                                        // set VBAT Monitor as reset source
#endif

    RTC_Init();                         // Enable SmaRTClock

    OSCICN &= ~0x80;
    REG0CN &=~0x10;
    PMU0CF = 0x20;                      // Clean all Wakeup flag
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins. The switch used
// is configured to wake up from sleep mode and change the CPU Mode
//
// P0.1   digital   open-drain   input   SW2
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    SFRPAGE = LEGACY_PAGE;
    // SW2 (P0.1) Input - Oscillator Clock Cycle Switch
    P0MDIN |= 0x02;                     // P0.1 is digital
    P0MDOUT &= ~0x02;                   // P0.1 is open-drain
    P0 |= 0x02;                         // Set the P0.1 latch to '1'
   
    // Set Port Match settings for S1 (P0.2)
    P0MASK |= 0x02;                     // Enable Port Match check for P0.2
    P0MAT  |= 0x02;                     // Set Port Match value to 1 for P0.2
   
    P1MDIN &= ~0x0C;                    // P1.2 and P1.3 (RTC Pins) are
       
    // Configure 32 analog pin as LCD Pin.
    SFRPAGE  =  CONFIG_PAGE;
    P3MDIN = 0x00;
    P4MDIN = 0x00;
    P5MDIN = 0x00;
    P6MDIN = 0x00;
    SFRPAGE = LEGACY_PAGE;
  
    // Crossbar Initialization
    XBR2 = 0x40;                        // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Runtime Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Enter_Sleep
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Puts the processor into sleep mode, in order to allow the user
// to measure Sleep mode current draw. 
// System wake up when SW2 (P0.1) is pressed.
//
//-----------------------------------------------------------------------------
void Enter_Sleep (void)
{ 
   //clear any wake-up flags, if they are set.
    PMU0CF = 0x20;
    // Go into  mode
    PMU0CF = SLEEP_MODE | PORT_MATCH_WAKEUP;
    //--sleep mode--//
    //= = = = = = = = = = = = = = =
    //= = = = = = = = = = = = = = =
    //--wake on port match event--//
    PMU0CF = 0x20;
}

//-----------------------------------------------------------------------------
// Oscillator_Cycle
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function sets the CPU Mode to the next setting.
// The process occurs once each time the processor is woken up from sleep
// mode after a port match event happen.
//
// Mode 1: Mux-Mode 1
// Mode 2: Mux-Mode 2
// Mode 3: Mux-Mode 3
// Mode 4: Mux-Mode 4
//-----------------------------------------------------------------------------
void LCD_Init(void)
{
   LCD0_SetContrast(VLCD_mV);
   LCD0_ConfigClear();
   LCD0_ConfigContrastMode(CONTRAST_MODE);
   LCD0_ConfigMuxMode(MUX_MODE);
   LCD0_ConfigRefresh(REFRESH_RATE_HZ, MUX_MODE);
   LCD0_ConfigSegments(LCD_SEGMENTS);
   LCD0_ConfigEnable();
}
//-----------------------------------------------------------------------------
// LCD0_ConfigClear ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will configure the LCD for the different modes.
//
//-----------------------------------------------------------------------------
void LCD0_ConfigClear(void)
{
   uint8_t restore;
   restore = SFRPAGE;
   SFRPAGE = LCD0_PAGE;

   SFRPAGE = LCD0_PAGE;

   LCD0MSCN = 0x00;                    // Clear LCD0MSCN
   LCD0MSCN = 0x02;                    // Reset LCD Data registers
   LCD0MSCN = 0x00;                    // Clear reset bit
   LCD0CN   = 0x00;                    // Clear LCD0CN

   SFRPAGE = restore;
}
//-----------------------------------------------------------------------------
// LCD0_ConfigMuxMode ()
//-----------------------------------------------------------------------------
//
// Return Value : uint8_t muxMode
// Parameters   : None
//
// This function will initialize mux mode for the LCD. This involves setting
// the mux mode in the LCD0CD SFR, enabling the bias, and configuring the COMx
// pins for analog input mode.
//
// This function clears LCD0CN and LCD0MSCN.
//
//-----------------------------------------------------------------------------
void LCD0_ConfigMuxMode(uint8_t muxMode)
{
   uint8_t restore;
   restore = SFRPAGE;
   SFRPAGE = LCD0_PAGE;

   switch (muxMode)
   {
      case STATIC_MUX_MODE:
         LCD0CN   &= ~0x07;            // clear MUXMD & BIAS
         LCD0CN    =  0x00;            // Set 1 mux mode
         LCD0MSCN &= ~0x40 ;           // Disable bias
         LCD0CF    =  0x20;            // Disable comparator, (bias must be disabled)
         SFRPAGE   =  LEGACY_PAGE;
         P2MDIN   &= ~0x10;            // COM0 analog mode
         break;

      case TWO_MUX_MODE:
         SFRPAGE   =  LCD0_PAGE;
         LCD0CN   &= ~0x07;            // clear MUXMD & BIAS
         LCD0CN   |=  0x03;            // Set 2 mux mode, 1/2 bias
         LCD0MSCN |=  0x40 ;           // Enable bias
         LCD0CF    =  0x00;            // Enable comparator, (bias must be enabled)
         SFRPAGE   =  LEGACY_PAGE;
         P2MDIN   &= ~0x30;            // COM0/1 analog mode
         break;

      case THREE_MUX_MODE:
         SFRPAGE   =  LCD0_PAGE;
         LCD0CN   &= ~0x07;            // clear MUXMD & BIAS
         LCD0CN   |=  0x04;            // Set 3 mux mode
         LCD0MSCN |=  0x40 ;           // Enable bias
         LCD0CF    =  0x00;            // Enable comparator, (bias must be enabled)
         SFRPAGE   =  LEGACY_PAGE;
         P2MDIN   &= ~0x70;            // COM0/1/2 analog mode
         break;

      case FOUR_MUX_MODE:
         SFRPAGE   =  LCD0_PAGE;
         LCD0CN   &= ~0x07;            // clear MUXMD & BIAS
         LCD0CN   |=  0x06;            // Set 4 mux mode
         LCD0MSCN |=  0x40 ;           // Enable bias
         LCD0CF    =  0x00;            // Enable comparator, (bias must be enabled)
         SFRPAGE   =  LEGACY_PAGE;
         P2MDIN   &= ~0xF0;            // COM0/1/2/3 analog mode
         break;

      default:
         break;
   }

   SFRPAGE = restore;
}
//-----------------------------------------------------------------------------
// LCD0_ConfigSegments ()
//-----------------------------------------------------------------------------
//
// Return Value : uint8_t segments
// Parameters   : None
//
// This function will configure the MCU according to the number of LCD segments
// used. The 32-bit mode with be enabled if the number of segments is greater
// than 16. The analog pins are configured according to the number of
// segments. This assumes that the segments start at P3.0 and are allocated
// contiguously.
//
//-----------------------------------------------------------------------------
void LCD0_ConfigSegments(uint8_t segments)
{
   SI_UU32_t segmentBits;
   uint8_t restore;
   restore = SFRPAGE;
   SFRPAGE = LCD0_PAGE;

   if(segments>16)
   {
      LCD0CN |= 0x08;                  // Set 32 segment mode
   }

   //Initialize all LCD segments to be ON.
   LCD0D0 = 0xFF;
   LCD0D1 = 0xFF;
   LCD0D2 = 0xFF;
   LCD0D3 = 0xFF;
   LCD0D4 = 0xFF;
   LCD0D5 = 0xFF;
   LCD0D6 = 0xFF;
   LCD0D7 = 0xFF;
   LCD0D8 = 0xFF;
   LCD0D9 = 0xFF;
   LCD0DA = 0xFF;
   LCD0DB = 0xFF;
   LCD0DC = 0xFF;
   LCD0DD = 0xFF;
   LCD0DE = 0xFF;
   LCD0DF = 0xFF;

   // initialize bit mask to 32 ones.
   segmentBits.u32 = 0xFFFFFFFF;

   // shift right by 32 minus number of segments
   segmentBits.u32 >>= (32 - segments);

   // Resulting number of ones is equal to number of segments

   // Complement to get PxMDIN setting.
   segmentBits.u32 = ~ segmentBits.u32;

   SFRPAGE = CONFIG_PAGE;
   P3MDIN = segmentBits.u8[B0];
   P4MDIN = segmentBits.u8[B1];
   P5MDIN = segmentBits.u8[B2];
   P6MDIN = segmentBits.u8[B3];

   SFRPAGE = restore;
}
//-----------------------------------------------------------------------------
// LCD0_ConfigContrastMode ()
//-----------------------------------------------------------------------------
//
// Return Value : uint8_t contrastMode, uint16_t contrast, uint16_t minContrast
// Parameters   : None
//
// This function will configure LCD SFR bits according to the selected
// contrast mode. The contrast and VBAT monitor thresholds are set as needed.
//-----------------------------------------------------------------------------
void LCD0_ConfigContrastMode(uint8_t contrastMode)
{
   uint8_t restore;
   restore = SFRPAGE;
   SFRPAGE = LCD0_PAGE;

   switch (contrastMode)
   {
      case LCD_BYPASS_MODE:
         LCD0MSCN &= ~0x04;            // Disable low-power charge pump
         LCD0MSCF |= 0x01;             // Enable charge pump bypass
         LCD0PWR &= ~0x08;             // Clear MODE bit for mode 1 & 4
         LCD0VBMCN = 0x00;             // VBM not used
         break;

      case LCD_MIN_CONTRAST_MODE:
         LCD0MSCN |=  0x40 ;           // Enable bias
         LCD0CF    =  0x00;            // Enable comparator, (bias must be enabled)
         LCD0MSCN &= ~0x04;            // Disable low-power charge pump
         LCD0MSCF |= 0x01;             // Enable charge pump bypass
         LCD0PWR |= 0x08;              // Set mode bit for mode 2 & 3
         LCD0VBMCN = 0xC1;             // enable, offset mode, contrast +60 mV
        break;

      case LCD_CONSTANT_CONTRAST_MODE:
         LCD0MSCN |=  0x40 ;           // Enable bias
         LCD0CF    =  0x00;            // Enable comparator, (bias must be enabled)
         LCD0MSCN |= 0x04;             // Enable low-power change pump
         LCD0MSCF &= ~0x01;            // Disable charge pump bypass
         LCD0PWR |= 0x08;              // Set mode bit for mode 2 & 3
         LCD0VBMCN = 0xC1;             // enable, offset mode, contrast +60 mV
         break;

      case LCD_AUTO_BYPASS_MODE:
         LCD0MSCN |=  0x40 ;           // Enable bias
         LCD0CF    =  0x00;            // Enable comparator, (bias must be enabled)
         LCD0MSCN |= 0x04;             // Enable low-power change pump
         LCD0MSCF &= ~0x01;            // Disable charge pump bypass
         LCD0PWR &= ~0x08;             // Clear MODE bit for mode 1 & 4
         LCD0VBMCN = 0xC1;             // Enable, offset mode, contrast +60 mV
         break;

      default:
         break;
   }

   SFRPAGE = restore;
}
//-----------------------------------------------------------------------------
// LCD0_SetContrast ()
//-----------------------------------------------------------------------------
//
// Return Value : uint16_t contrast
// Parameters   : None
//
// This function will calculate the resister setting for the desired contrast
// voltage.
//
//-----------------------------------------------------------------------------
void LCD0_SetContrast (uint16_t contrast)
{
   uint32_t value;

   uint8_t restore;
   restore = SFRPAGE;
   SFRPAGE = LCD0_PAGE;

   if(contrast<1900)
      LCD0CNTRST = 0;

   value = contrast - 1900;

   value *= 31;
   value += 1820/2;                    // add half of divisor for rounding
   value /= 1820;

   if (value <32)
      LCD0CNTRST = (uint8_t) (value);
   else
      LCD0CNTRST = 31;

   SFRPAGE = restore;
}
//-----------------------------------------------------------------------------
// LCD0_ConfigRefresh ()
//-----------------------------------------------------------------------------
//
// Return Value : uint8_t refresh, uint8_t muxMode
// Parameters   : None
//
// This function will set the LCD clock divider according to the desired
// refresh rate. The lcdClockFreq is set according to the RTC mode as defined
// in the lcdInit.h header file.
//
//-----------------------------------------------------------------------------
void LCD0_ConfigRefresh(uint8_t refresh, uint8_t muxMode)
{
   SI_UU16_t divider;
   uint16_t  lcdClockFreq;
   uint8_t restore;

   restore = SFRPAGE;
   SFRPAGE = LCD0_PAGE;

#if ( RTC_LFO == ENABLED )
   lcdClockFreq = RTC_LFO_FREQ;
#else
   lcdClockFreq = RTC_XTAL_FREQ/2;
   LCD0CN |= 0x20;                      // lcdClock = rtcClock/2
#endif

   divider.u16 = lcdClockFreq;
   divider.u16 /= refresh;
   divider.u16 /= muxMode;
   divider.u16 >>=2;
   divider.u16--;

   if(divider.u16 > 0x3FF)
      divider.u16 = 0x3FF;

   LCD0CLKDIVH = divider.u8[MSB];
   LCD0CLKDIVL = divider.u8[LSB];

   SFRPAGE = restore;
}

//-----------------------------------------------------------------------------
// LCD0_ConfigEnable ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will configure the LCD for the different modes.
//
//-----------------------------------------------------------------------------
void LCD0_ConfigEnable(void)
{
   uint8_t restore;
   restore = SFRPAGE;
   SFRPAGE = LCD0_PAGE;

   SFRPAGE = LCD0_PAGE;
   LCD0MSCN |= 0x11;                    // Enable LCD, enable Clocks

   SFRPAGE = restore;
}

//-----------------------------------------------------------------------------
// RTC_Write ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : reg, value
//
// This function will Write one byte from the specified RTC register.
// Using a register number greater that 0x0F is not permitted,
//
// This function uses the new F960 fast write mode.
//
//-----------------------------------------------------------------------------
void RTC_Write (uint8_t reg, uint8_t value)
{
    uint8_t  restoreSFRPAGE;
	restoreSFRPAGE = SFRPAGE;

    SFRPAGE = LEGACY_PAGE;
 	reg &= 0x0F;                        // mask low nibble
    RTC0ADR  = reg;                     // pick register
    RTC0DAT = value;                    // write data

	SFRPAGE= restoreSFRPAGE;
}
//-----------------------------------------------------------------------------
// RTC_Read ()
//-----------------------------------------------------------------------------
//
// Return Value : RTC0DAT
// Parameters   : reg
//
// This function will read one byte from the specified RTC register.
// Using a register number greater that 0x0F is not permitted,
//
//
// This function uses the new F960 fast read mode.
//
//-----------------------------------------------------------------------------
uint8_t RTC_Read (uint8_t reg)
{
    uint8_t value;
	uint8_t  restoreSFRPAGE;

	restoreSFRPAGE = SFRPAGE;
    SFRPAGE = LEGACY_PAGE;
    
	reg &= 0x0F;                        // mask low nibble
    RTC0ADR  = (reg |0x80);             // write address setting READ bit
	value = RTC0DAT;
	SFRPAGE= restoreSFRPAGE;

    return value;                       // return value
}

//-----------------------------------------------------------------------------
// RTC_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will initialize the RTC. First it unlocks the RTC interface,
// enables the RTC, clears ALRMn and CAPTUREn bits. Then it sets up the RTC
// to operate using a 32.768khZ crystal or low frequency oscillator. Lastly
// it enables the alarm and interrupt. This function uses the RTC primitive 
// functions to access the internal RTC registers.
//
//-----------------------------------------------------------------------------
void RTC_Init (void)
{
    uint16_t i;

    uint8_t CLKSEL_SAVE = CLKSEL;

    RTC0KEY = 0xA5;                     // Unlock the smaRTClock interface
    RTC0KEY = 0xF1;

#if (RTC_LFO == ENABLED)
    RTC_Write (RTC0XCN, 0x08);          // Using the Low Frequency Oscillator
    RTC_Write (RTC0CN, 0x80);           // Enable smaRTClock oscillator
#else
    RTC_Write (RTC0XCN, 0x60);          // Configure the smaRTClock
                                        // oscillator for crystal mode
                                        // Bias Doubling Enabled
                                        // AGC Disabled

    RTC_Write (RTC0XCF, 0x8A);          // 8a Enable Automatic Load Capacitance
    // stepping and set the desired
    // load capacitance

    RTC_Write (RTC0CN, 0x80);           // Enable smaRTClock oscillator

    //----------------------------------
    // Wait for smaRTClock to start
    //----------------------------------

    CLKSEL    =  0x74;                  // Switch to 156 kHz low power
    // internal oscillator
    // Wait > 20 ms
    for (i=0x150;i!=0;i--);

    // Wait for smaRTClock valid
    while ((RTC_Read (RTC0XCN) & 0x10)== 0x00);

    // Wait for Load Capacitance Ready
    while ((RTC_Read (RTC0XCF) & 0x40)== 0x00);

    //----------------------------------
    // smaRTClock has been started
    //----------------------------------

    RTC_Write (RTC0XCN, 0x40);          // Disable bias doubling

    // Wait > 20 ms
    for (i=0x150;i!=0;i--);

    RTC_Write (RTC0XCN, 0x40);          // Enable Automatic Gain Control
    // and disable bias doubling

    RTC_Write (RTC0CN, 0xC0);           // Enable missing smaRTClock detector
#endif 
    
    // and leave smaRTClock oscillator
    // enabled.
    // Wait > 2 ms
    for (i=0x40;i!=0;i--);
    
    PMU0CF = 0x20;                      // Clear PMU wake-up source flags

    CLKSEL = CLKSEL_SAVE;               // Restore system clock
    while(!(CLKSEL & 0x80));            // Poll CLKRDY
    
    // Initializes the system clock to use the smaRTClock oscillator.
    RSTSRC = 0x06;                      // Enable missing clock detector and
    // leave VDD Monitor enabled.
    
    CLKSEL &= ~0x70;                    // Specify a clock divide value of 1
    
    while(!(CLKSEL & 0x80));            // Wait for CLKRDY to ensure the
    // divide by 1 has been applied
    
    //CLKSEL = 0x03;                    // Select smaRTClock oscillator
                                        // as the system clock
    //OSCICN &= ~0x80;                  // Disable internal oscillator
    
    RTC0KEY = 0x55;                     // Lock smaRTClock interface;
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
