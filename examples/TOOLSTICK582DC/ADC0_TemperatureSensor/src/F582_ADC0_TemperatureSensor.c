//-----------------------------------------------------------------------------
// F582DC_ADC0_TemperatureSensor.c
//-----------------------------------------------------------------------------
// Copyright 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program measures the temperature sensor on an 'F582 device and outputs
// the temperature using the UART.
//
// The program uses the value defined for the #define AMBIENT to calculate
// the necessary offset for the temperature.  After waiting for the device
// to heat to operating temperature, it  calculates the offset by taking the
// difference between AMBIENT and the measured temperature.  This offset is
// subtracted from all further measurements.
//
// How To Test:
//
// 1) Compile and download the code to a ToolStick F582 Daughter Card
// 2) Connect to the ToolStick using ToolStick Terminal using the settings
//    defined by <BAUD_RATE>
// 3) Check that the printed temperature is close to the ambient temperature
//
// Target:         ToolStick C8051F582 DC
// Tool chain:     Keil C51 8.x / Keil EVAL C51 / SDCC
// Command Line:   None
//
// Release 1.1 (SG)
//    - Revised for Simplicity Studio
//    - 28 JUL 2015
//
// Release 1.0
//    -Initial Revision (GP)
//    -08 OCT 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F580_Register_Enums.h>         // SFR declarations
#include <stdio.h>

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

typedef struct {
   int16_t voltage;                        // Temp Sensor output in mV
   int16_t temperature;                    // Temp Sensor output in deg C
} temp_sense_out;

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK           24000000      // SYSCLK frequency in Hz
#define BAUDRATE           115200      // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Temperature Sensor Calibration Parameters
//-----------------------------------------------------------------------------

#define TEMP_SENSOR_GAIN     2230      // Temp Sensor Gain in (uV / degC)
                                       // Value obtained from datasheet

#define TEMP_SENSOR_OFFSET    924      // Temp Sensor Offset in mV
                                       // Value obtained from datasheet

#define VREF                 2200      // ADC Voltage Reference (mV)

#define AMBIENT                25      // Ambient temp in deg C

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void PORT_Init (void);
void OSCILLATOR_Init (void);
void UART0_Init (void);
void ADC0_Init (void);

void Print_String (uint8_t pstring[]);

temp_sense_out Get_Temperature_Voltage (void);

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
   PCA0MD &= ~0x40;                    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// main Routine
//-----------------------------------------------------------------------------

void main (void)
{
   temp_sense_out result;              // Last measurement from temp sensor
   int16_t offset;                         // Stores offset
   uint16_t i, j;

   SFRPAGE = ACTIVE1_PAGE;             // Set for PCA0MD and SBUF0


   PORT_Init ();                       // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator
   UART0_Init ();                      // Initialize UART
   ADC0_Init ();                       // Init ADC0

   Print_String ("Waiting for device to heat to operating temperature...\n");
   i = 1;
   while (i)                           // Waiting for device to heat
   {
      j = 1;
      while (j < 300) {j++;}
      i++;
   }

   Print_String ("Finished heating.  Calculating offset...\n");

   // The current temperature after self-heating is assumed to be Ambient
   result = Get_Temperature_Voltage();
   offset = result.temperature - AMBIENT;

   while (1)
   {
      // Get latest temperature and voltage
      result = Get_Temperature_Voltage();


      // Print voltage
      Print_String ("\fVoltage = ");

      // Alternative to using printf() which requires 1.5K bytes code space
      putchar ((result.voltage / 1000)  + 0x30);       // Ones digit
      putchar ('.');
      putchar (((result.voltage / 100) % 10)  + 0x30); // Tenths
      putchar (((result.voltage / 10) % 10)  + 0x30);  // Hundredths
      putchar ((result.voltage % 10)  + 0x30);         // Thousandths


      // Print temperature
      result.temperature -= offset;    // Subtract offset
      Print_String (" V; Temperature = ");

      if (result.temperature < 0)
      {
        putchar ('-');
        result.temperature *= -1;
      }
      putchar (((result.temperature / 10) % 10)  + 0x30);  // Tens digit
      putchar ((result.temperature % 10)  + 0x30);         // Ones digit

      Print_String (" deg. C\n");
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
// P0.0 - analog               VREF
// P0.4 - digital  push-pull   UART TX
// P0.5 - digital  open-drain  UART RX
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0SKIP |= 0x01;                     // Skip P0.0 (VREF)

   P0MDOUT |= 0x10;                    // Set TX pin to push-pull
   P0MDIN  &= ~0x01;                   // Set VREF to analog

   XBR0     = 0x01;                    // Enable UART0
   XBR2     = 0x40;                    // Enable crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24.0MHz
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0x87;                      // Configure internal oscillator for
                                       // maximum frequency

   SFRPAGE = ACTIVE1_PAGE;

   RSTSRC = 0x04;                      // Enable missing clock detector

   SFRPAGE = SFRPAGE_save;
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
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE;

   ADC0CN = 0x00;                      // ADC0 disabled; Burst mode disabled
                                       // Data is right-justified
                                       // ADC0 conversions are initiated
                                       // on a write to ADC0CN_ADBusy

   ADC0CF = (SYSCLK/3000000) << 3;     // ADC conversion clock <= 3MHz
                                       // Repeat Count = 1

   ADC0MX = 0x30;                      // Temperature Sensor is input

   REF0CN = 0x17;                      // Enable on-chip VREF and buffer
                                       // Set voltage reference to 2.25V setting
                                       // Enable temperature sensor

   EIE1 &= ~0x04;                      // Disable ADC0 conversion interrupt

   ADC0CN_ADEN = 1;                    // Enable ADC0 (ADC0CN)

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       // clear SCON0_RI and SCON0_TI bits

   // Baud Rate = [BRG Clock / (65536 - (SBRLH0:SBRLL0))] x 1/2 x 1/Prescaler

#if   (SYSCLK/BAUDRATE/2/0xFFFF < 1)
      SBRL0 = -(SYSCLK/BAUDRATE/2);
      SBCON0 |= 0x03;                  // Set prescaler to 1
#elif (SYSCLK/BAUDRATE/2/0xFFFF < 4)
      SBRL0 = -(SYSCLK/BAUDRATE/2/4);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x01;                  // Set prescaler to 4
#elif (SYSCLK/BAUDRATE/2/0xFFFF < 12)
      SBRL0 = -(SYSCLK/BAUDRATE/2/12);
      SBCON0 &= ~0x03;                 // Set prescaler to 12
#else
      SBRL0 = -(SYSCLK/BAUDRATE/2/48);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x02;                  // Set prescaler to 48
#endif

   SBCON0 |= 0x40;                     // Enable baud rate generator

   SFRPAGE = ACTIVE1_PAGE;

   SCON0_TI = 1;                       // Indicate TX0 ready

   SFRPAGE = SFRPAGE_save;
}

#ifdef SDCC

// SDCC does not include a definition for putchar(), which is used in printf()
// and so it is defined here.  The prototype does not need to be explicitly
// defined because it is provided in stdio.h

//-----------------------------------------------------------------------------
// putchar
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : character to send to UART
//
// This function outputs a character to the UART.
//-----------------------------------------------------------------------------
void putchar (char output)
{
   if (output == '\n')
   {
      while (!SCON0_TI);
      SCON0_TI = 0;
      SBUF0 = 0x0d;
   }
   while (!SCON0_TI);
   SCON0_TI = 0;
   SBUF0 = output;
}

#endif

//-----------------------------------------------------------------------------
// Support Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Get_Temperature_Voltage
//-----------------------------------------------------------------------------
//
// Return Value : temp_sense_out - ADC voltage and temperature in deg C
// Parameters   : None
//
// Obtain 4096 samples from the temperature sensor, average them and calculate
// voltage and temperature.
//
//-----------------------------------------------------------------------------
temp_sense_out Get_Temperature_Voltage (void)
{
   uint16_t i;                              // Sample counter
   uint32_t accumulator = 0L;               // Where the ADC samples are integrated
   uint16_t currval;                        // Current value of ADC0
   temp_sense_out result;              // Final voltage and temp from ADC

   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE1_PAGE;

   ADC0CN_ADINT = 0;                   // Clear end-of-conversion indicator
   ADC0CN_ADBUSY = 1;                  // Initiate conversion

   // Accumulate 4096 samples and average to get a 16-bit result
   // 4096 samples = 12 bits; 12 extra bits + 12 samples per bit = 24 bits
   // Shift by 8 bits to calculate a 16-bit result.

   i = 0;
   do
   {
      while (!ADC0CN_ADINT);           // Wait for conversion to complete
      ADC0CN_ADINT = 0;                // Clear end-of-conversion indicator

      currval = ADC0;                  // Store latest ADC conversion
      ADC0CN_ADBUSY = 1;               // Initiate conversion
      accumulator += currval;          // Accumulate
      i++;                             // Update counter
   } while (i != 4096);

   accumulator = accumulator >> 8;     // 8-bit shift to go from 24 to 16 bits

   result.voltage = (uint16_t) (((uint32_t) accumulator * (uint32_t) VREF) / (uint32_t) 65536);

   // From Temperature Sensor section of the datasheet:

   // V[temp] = (Slope * TempC) + Offset; solving for TempC
   // TempC   = (V[temp] - Offset) / Slope

   // V(temp) = Accumulator / 2^16 * VREF (2.25V)
   // Offset  = TEMP_SENSOR_OFFSET (924 mV)
   // Slope   = TEMP_SENSOR_GAIN   (2.23 mV / deg C)

   // Putting in the numbers and simplifying the equation leads to:
   // TempC   = Accumulator * 1008 / 65536 - 414

   result.temperature = (uint16_t) ((uint32_t)accumulator * (uint32_t)1008 / (uint32_t)65536 - (uint32_t)414);

   // Note that the numbers were rounded when equation was solved.
   // This rounding does introduce error to the calculation, but it is
   // negligible when looking for an estimated temperature.

   SFRPAGE = SFRPAGE_save;

   return result;
}

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

void Print_String (uint8_t pstring[])
{
   uint8_t i = 0;
   while (pstring[i])
   {
      putchar(pstring[i++]);
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
