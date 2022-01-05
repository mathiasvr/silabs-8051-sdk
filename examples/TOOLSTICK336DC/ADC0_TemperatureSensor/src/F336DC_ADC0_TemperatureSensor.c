//-----------------------------------------------------------------------------
// F336DC_ADC0_TemperatureSensor.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program is used to measure the temperature sensor on an 'F33x
// device.  It uses 1-point calibration and stores the offset value
// in Flash memory. The program measures the temperature with accuracy to
// 100ths of a degree Celsius.
//
// How To Test:
//
// 1) Change the #define AMBIENT_TEMPERATURE to accurately reflect the
//    the temperature of the testing environment
// 2) Connect the ToolStick336DC to a ToolStick Base Adapter.
// 3) Compile and download code to the ToolStick Daughter Card by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Set a break-point within the while(1) loop within the main function
//    after a temperature measurement is taken (line 135).
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8. The program should halt
//    at the set break-point.
// 6) Check the variable "temperature" for the measured temperature. The
//    temperature is held in this variable in hundredths of a degree:
//    for example, a value of 2500 would be equal to 25.00 degrees C.
// 7) Resume the code again to take another measurement.
//
// Target:         ToolStick336DC
// Tool chain:     Simplicity Studio / Keil C51 9.51
//
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 21 MAR 2014
//
// Release 1.0
//    -Initial Revision (HA)
//    -31 AUG 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F336_Register_Enums.h"
#include "stdio.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK      24500000           // SYSCLK frequency in Hz
#define BAUDRATE      115200           // Baud rate of UART in bps
#define TIMER2_RATE     1000           // Timer 2 overflow rate in Hz

//-----------------------------------------------------------------------------
// Temperature Sensor Calibration Parameters
//-----------------------------------------------------------------------------
#define AMBIENT_TEMPERATURE    25      // Ambient Calibration Temperature
                                       // (degC)

#define TEMP_SENSOR_GAIN     2860      // Temp Sensor Gain in (uV / degC)
                                       // Value obtained from datasheet

#define VREF                 2430      // ADC Voltage Reference (mV)

#define SOAK_TIME               5      // Soak Time in Seconds

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// TEMP_OFFSET allocates two bytes of code memory space in FLASH
// that will be used to store the calibrated temperature value
uint16_t code TEMP_OFFSET = 0xFFFF;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void ADC0_Init (void);
void UART0_Init (void);
void PORT_Init (void);
void TIMER2_Init (int16_t);

void Print_String (char pstring[]);
void Flash_ByteWrite (uint32_t addr, char byte);

void Wait_Soak_Time (uint8_t soak_time);
void Wait_One_Second (void);
int16_t  Get_Temperature (void);
void Calibrate_TempSensor (void);
uint16_t Measure_Temperature (void);

// ISRs defined: None.

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
// main Routine
//-----------------------------------------------------------------------------

void main (void)
{
   uint16_t temperature;           // Stores last measurement

   // Disable Watchdog timer
                                       // enable)
   PORT_Init ();                       // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator

   ADC0_Init ();                       // Init ADC0
   TIMER2_Init (SYSCLK/TIMER2_RATE);   // Init Timer 2
   UART0_Init ();

   ADC0CN_ADEN = 1;                          // Enable ADC0

   if (TEMP_OFFSET == 0xFFFF)          // True if first-time to execute
   {
      Print_String ("Calibrating...\n");
      Calibrate_TempSensor ();         // Execute calibration sequence
   }
   else
   {
      Print_String ("Calibration previously completed\n");
   }

   while (1)
   {
      temperature = Get_Temperature ();
      Print_String ("Temperature = ");

      // This method of printing the temperature is an alternative to using
      // printf() and its capabilities

      IE_EA = 0;
      putchar ((temperature / 1000) + 0x30);        // Tens digit
      putchar (((temperature / 100) % 10)  + 0x30); // Ones digit
      putchar ('.');
      putchar (((temperature / 10) % 10)  + 0x30);  // Tenths digit
      putchar ((temperature % 10)  + 0x30);         // Hundredths digit
      IE_EA = 1;

      Print_String (" degrees C\n");
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// P0.4    digital    push-pull     UART TX
// P0.5    digital    open-drain    UART RX
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P0SKIP  |= 0x01;                    // Skip P0.0 for external VREF
   P0MDIN  |= 0x01;                    // Configure P0.0 as analog input.
   P0MDOUT |= 0x10;                    // enable UTX as push-pull output
   XBR0    = 0x01;                     // Enable UART on P0.4(TX) and P0.5(RX)
   XBR1    = 0x40;                     // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24.5MHz
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   OSCICN |= 0x03;                     // Configure internal oscillator for
                                       // its maximum frequency
   RSTSRC  = 0x04;                     // Enable missing clock detector
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure ADC0 to use ADBUSY as conversion source, and to sense the output
// of the temp sensor.  Disables ADC end of conversion interrupt. Leaves ADC
// disabled.
//
//-----------------------------------------------------------------------------

void ADC0_Init (void)
{
   ADC0CN = 0x40;                      // ADC0 disabled; LP tracking
                                       // mode; ADC0 conversions are initiated
                                       // on a write to ADBusy
   AMX0P  = 0x10;                      // Temp sensor selected at + input
   AMX0N  = 0x11;                      // Single-ended mode

   ADC0CF = (SYSCLK/3000000) << 3;     // ADC conversion clock <= 3MHz

   ADC0CF &= ~0x04;                    // Make ADC0 right-justified
   REF0CN = 0x0e;                      // enable temp sensor, VREF = VDD, bias
                                       // generator is on.

   EIE1 &= ~0x08;                      // Disable ADC0 EOC interrupt
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear SCON0_RI and SCON0_TI bits
   if (SYSCLK/BAUDRATE/2/256 < 1) {
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   } else if (SYSCLK/BAUDRATE/2/256 < 4) {
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x09;
   } else if (SYSCLK/BAUDRATE/2/256 < 12) {
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   } else {
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   }

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                            // START Timer1
   SCON0_TI = 1;                            // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// TIMER2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1)  int16_t counts - number of milliseconds of delay
//                    range is postive range of integer: 0 to 32767
//
// Configure Timer2 to auto-reload at interval specified by <counts> (no
// interrupt generated) using SYSCLK as its time base.
//
//-----------------------------------------------------------------------------
void TIMER2_Init (int16_t counts)
{
   TMR2CN = 0x00;                      // STOP Timer2; Clear TMR2CN_TF2H and TMR2CN_TF2L;
                                       // disable low-byte interrupt; disable
                                       // split mode; select internal timebase
   CKCON |= 0x10;                      // Timer2 uses SYSCLK as its timebase

   TMR2RL  = -counts;                  // Init reload values
   TMR2    = TMR2RL;                   // Init Timer2 with reload value
   IE_ET2 = 0;                            // Disable Timer2 interrupts
   TMR2CN_TR2 = 1;                            // Start Timer2
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Wait_Soak_Time
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1)  uint8_t soak_time - number of seconds of delay
//                    range is postive range of integer: 0 to 255
//
// This routine waits for the number of seconds indicated in the constant
// <soak_time>.
//
//-----------------------------------------------------------------------------
void Wait_Soak_Time (uint8_t soak_time)
{
   uint8_t i;

   for( i = soak_time; i != 0; i--)
   {
      Wait_One_Second();
      Print_String ("Soaking\n");
   }
}

//-----------------------------------------------------------------------------
// Wait_One_Second
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine uses Timer2 to insert a delay of approximately one second.
// Timer 2 overflows <TIMER2_RATE> times per second.
//
//-----------------------------------------------------------------------------
void Wait_One_Second (void)
{
   uint16_t count;
   TMR2CN_TF2H = 0;                           // Clear Timer2 overflow flag
   TMR2CN_TR2 = 1;                            // Start Timer2

   for (count = TIMER2_RATE; count != 0; count--)
   {
      while (!TMR2CN_TF2H);                   // Wait for overflow
      TMR2CN_TF2H = 0;                        // Clear overflow indicator
   }

   TMR2CN_TR2 = 0;                            // Stop Timer2
}

//-----------------------------------------------------------------------------
// Calibrate_TempSensor
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine waits for the device to heat to operating temperature, takes an
// ADC measurement, and calculates a temperature.  It compares this measured
// value to AMBIENT_TEMPERATURE and calculates an offset, which is stored in a
// Flash memory location for later measurements.
//
//-----------------------------------------------------------------------------
void Calibrate_TempSensor (void)
{

   bit EA_state = IE_EA;                  // Preserves IE_EA state
   uint32_t offset_address;       // Used to write calibration
                                       // Value into FLASH memory

   int32_t temp_offset;                   // Stores returned value from ADC

   Wait_Soak_Time (SOAK_TIME);         // Let temperature of device stabilize

   temp_offset= (int32_t) Measure_Temperature ();     // Read oversampled ADC code

   // Now calculate the 0 DEG C offset value using temp_offset, the
   // temp sensor gain, and the ambient temperature.

   temp_offset = temp_offset - ((int32_t) AMBIENT_TEMPERATURE *
                                TEMP_SENSOR_GAIN / VREF * 65536 / 1000);

   offset_address = &TEMP_OFFSET;      // Point to TEMP_OFFSET

   Flash_ByteWrite (offset_address, temp_offset >> 8);

   offset_address++;                   // Move to low byte of TEMP_OFFSET in
                                       // Flash to store low byte of offset

   Flash_ByteWrite (offset_address, temp_offset);

   IE_EA = EA_state;                      // Restore interrupt state

}

//-----------------------------------------------------------------------------
// Measure_Temperature
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine averages 16383 ADC samples and returns a 16-bit unsigned
// result.
//
//-----------------------------------------------------------------------------
uint16_t Measure_Temperature (void)
{
   unsigned i;                         // Sample counter
   uint32_t accumulator = 0L;     // Where the ADC samples are integrated

   uint16_t currval;

   ADC0CN_ADINT = 0;                         // Clear end-of-conversion indicator
   ADC0CN_ADBUSY = 1;                        // Initiate conversion

   // read the ADC value and add to running total
   i = 0;
   do
   {
      while (!ADC0CN_ADINT);                 // Wait for conversion to complete
      ADC0CN_ADINT = 0;                      // Clear end-of-conversion indicator

      currval = ADC0;                  // Store latest ADC conversion
      ADC0CN_ADBUSY = 1;                     // Initiate conversion
      accumulator += currval;          // Accumulate
      i++;                             // Update counter
   } while (i != 16383);

   return (uint16_t) (accumulator >> 8);  // Obtain a 16-bit result
                                              // (14 + 10 = 24 - 8 = 16) bits
}

//-----------------------------------------------------------------------------
// Get_Temperature
//-----------------------------------------------------------------------------
//
// Return Value : int16_t - returns the temperature in hundredths of degrees
// Parameters   : None
//
// This routine averages 16383 ADC samples and returns a 16-bit unsigned
// result.
//
//-----------------------------------------------------------------------------
int16_t Get_Temperature (void)
{
   uint16_t ADC_code;
   int32_t result;

   ADC_code = Measure_Temperature ();

   result = ADC_code - TEMP_OFFSET;

   // result = result * (VREF / 65536) * (1000 / TEMP_SENSOR_GAIN) * ( 100 )
   // the equation below is re-arranged for fixed-point math.

   result = result * (int32_t) VREF / 256 * 1000 / TEMP_SENSOR_GAIN * 100 / 256;

   return (int16_t) result;
}

//-----------------------------------------------------------------------------
// Support Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Print_String
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//         1) char pstring[] - null terminated character string
//
// Prints the strings stored in pstring to the UART.
// This function is used instead of printf() because printf() consumes
// a considerable amount of code space.
//
//-----------------------------------------------------------------------------

void Print_String (char pstring[])
{
   uint8_t i = 0;
   while (pstring[i])
   {
      putchar(pstring[i++]);
   }
}

//-----------------------------------------------------------------------------
// Flash_ByteWrite
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//         1) uint16_t addr - location in Flash to write the byte
//         2) char byte - data to store to Flash
//
// Stores <byte> in address pointed to by <addr>.
// This function was obtained from Application Note 201.
//
//-----------------------------------------------------------------------------

void Flash_ByteWrite (uint32_t addr, char byte)
{
   bit EA_SAVE = IE_EA;                   // Preserve IE_EA
   char xdata * data pwrite;           // FLASH write pointer

   IE_EA = 0;                             // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   pwrite = (char xdata *) addr;       // Point to Flash location

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x01;                      // PSWE = 1

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   *pwrite = byte;                     // Write the byte

   PSCTL &= ~0x01;                     // PSWE = 0; disable writes

   IE_EA = EA_SAVE;                       // Restore interrupts
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
