--------------------------------------------------------------------------------
 Copyright 2014 Silicon Laboratories, Inc.
 All rights reserved. This program and the accompanying materials
 are made available under the terms of the Silicon Laboratories End User 
 License Agreement which accompanies this distribution, and is available at
 http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 Original content and implementation provided by Silicon Laboratories.
--------------------------------------------------------------------------------

Program Description:
--------------------------------------------------------------------------------
This example demonstrates the following datasheet specifications for the Analog
to Digital Converter (ADC) from Table 4.9 (Power Consumption and Throughput Rate)
(ADC):

 - Power Consumption: Sampling at 300 ksps, 8 Bit Conversion mode settings
 - Power Consumption: Sampling at 300 ksps, 10 Bit Conversion mode settings
 - Power Consumption: Sampling at 300 ksps, 8 Bit Tracking mode settings
 - Power Consumption: Sampling at 300 ksps, 10 Bit Tracking mode settings
 - Throughput Rate: 8 Bit Conversion mode
 - Throughput Rate: 10 Bit Conversion mode
 - Throughput Rate: 8 Bit Tracking mode
 - Throughput Rate: 10 Bit Tracking mode

When testing the power consumption specifications, the ADC is initialized to 
8 Bit mode with the specified power conditions. Using switch SW0_2 on P0.2 
to toggle the ADC enable/disable, the power consumption can be verified 
by taking the difference of the current when ADC enabled and the current when
the ADC is disabled. When testing the throughput rate specifications, 
SW0_2 could be ignored and P1.5 is toggled every time a conversion is 
completed. Monitoring P1.5 with an oscilloscope will display a square wave with
a frequency that is half the sampling rate. To switch between the testing modes,
use the line #define ADC0_MODE_SELECT under the section labeled 
"Global Constants". The comments the #define statement describe the available
modes to choose from. The default mode is 0 which initializes the ADC to the 
8 Bit conversion power mode settings and a sampling rate of 300 ksps.

Resources Used:
--------------------------------------------------------------------------------
ADC0
PROSC
PORT1
TMR2
WDT0 (Disabled)

Part Family:
--------------------------------------------------------------------------------
C8051F93x/92x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.3 

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.9 Power Supply Current - ADC0
   - Sampling at 300 ksps, Conversion mode settings
      + Min: --
      + Typ: 800 uA
      + Max: --  uA
   - Sampling at 0 ksps, Tracking mode settings
      + Min: --
      + Typ: 680 uA
      + Max: --

Table 4.9 ADC - Throughput Rate  
    - 8/10 Bit Mode
      + Min: --
      + Typ: --
      + Max: 300 ksps
	
Notes On Example and Modes:
--------------------------------------------------------------------------------
- Mode 0: Power consumption, 300 ksps, 8 bit Conversion mode settings (Default)
   + ADC0
   + PFOSC
   + Port I/O
   + Timer 2

- Mode 1: Power consumption, 300 ksps, 10 bit Conversion mode settings
   + ADC0
   + PFOSC
   + Port I/O
   + Timer 2
      
- Mode 2: Power consumption, 300 ksps, 8 bit Tracking mode settings 
   + ADC0
   + PFOSC
   + Port I/O
   + Timer 2
      
- Mode 3: Power consumption, 300 ksps, 10 bit Tracking mode settings 
   + ADC0
   + PFOSC
   + Port I/O
   + Timer 2
      
Throughput Rate: Set #define THROUGHPUT_EN 1
   + ADC0
   + PFOSC
   + Port I/O
   + Timer 2

How to Use:
--------------------------------------------------------------------------------
Refer to F93x_ADC_Continuous_Sample.c for detailed directions on using this 
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (MT)
   -24 SEP 2013

---eof--------------------------------------------------------------------------
