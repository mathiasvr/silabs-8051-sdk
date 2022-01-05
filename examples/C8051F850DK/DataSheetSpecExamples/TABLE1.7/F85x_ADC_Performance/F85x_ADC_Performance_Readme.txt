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
This example code demonstrates the performance metrics of the Analog-to-Digital
Converter (ADC), such as Signal-to-Noise ratio. A UART connection from the MCU
is required to collect data, and an excel spreadsheet is provided with this
code to facilitate analysis of the data.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.4 (UTX - UART Transmit Pin)
P0.5 (URX - UART Receive Pin)
P1.2 (ADC0 - ADC Input Pin)

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5 6/13

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 1.7 Analog-to-Digital Converter (ADC) - Offset Error
   - Sampling Rate = 800 ksps (10-Bit Mode)
      + Min: -1 LSB
	  + Typ:  0 LSB
	  + Max:  1 LSB
   - Sampling Rate = 200 ksps (12-Bit Mode)
      + Min: -2 LSB
	  + Typ:  0 LSB
	  + Max:  2 LSB
	  
Table 1.7 Analog-to-Digital Converter (ADC) - Signal-to-Noise Ratio
   - Sampling Rate = 800 ksps (10-Bit Mode)
      + Min: TBD
	  + Typ: 60 dB
	  + Max: --
   - Sampling Rate = 200 ksps (12-Bit Mode)
      + Min: TBD
	  + Typ: 66 dB
	  + Max: --
	  
Table 1.7 Analog-to-Digital Converter (ADC) - SNR Plus Distortion
   - Sampling Rate = 800 ksps (10-Bit Mode)
      + Min: TBD
	  + Typ: 60 dB
	  + Max: --
   - Sampling Rate = 200 ksps (12-Bit Mode)
      + Min: TBD
	  + Typ: 66 dB
	  + Max: --
	  
Notes On Example and Modes:
-------------------------------------------------------------------------------- 
F85x ADC Settings:
   - Number of ADC Samples: 4096 (Default)
   - ADC Sampling Rate: 3.2 kHz in 10-Bit Mode (800 Hz in 12-Bit Mode)
   		 
How to Use:
--------------------------------------------------------------------------------
Refer to F85x_ADC_Performance.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (PMD)
	- 27 JUN 2013

---eof--------------------------------------------------------------------------

