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
This example code demonstrates the performance metrics of the Analog-to-digital
converter (ADC), such as Signal-to-Noise ratio. A UART connection from the MCU
is required to collect data, and an excel spreadsheet is provided with this
code to facilitate analysis of the data.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.4 (UTX - UART Transmit Pin)
P0.5 (URX - UART Receive Pin)
P0.0 (ADC0 - ADC Input Pin)

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.2

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.10 Analog-to-Digital Converter (ADC0)
   - Offset Error
      + Min: --
	  + Typ: < +/- 1 LSB
	  + Max: +/-2 LSB

   - Signal-to-Noise Plus Distortion 10 bit
      + Min: 54 dB
	  + Typ: 58 dB
	  + Max: --

   - Signal-to-Noise Plus Distortion 12 bit
      + Min: 62 dB
	  + Typ: 65 dB
	  + Max: --

   - Signal-to-Distortion 10 bit
      + Min: --
	  + Typ: 73 dB
	  + Max: --

   - Signal-to-Distortion 12 bit
      + Min: --
	  + Typ: 76 dB
	  + Max: --

   - Spurious-Free Dynamic Range 10 bit
      + Min: --
	  + Typ: 75 dB
	  + Max: --

   - Spurious-Free Dynamic Range 12 bit
      + Min: --
	  + Typ: 82 dB
	  + Max: --

Notes On Example and Modes:
--------------------------------------------------------------------------------
Waveform Generator Settings:
   - Input Signal: Sinusoidal Waveform
   - Frequency: 5kHz in 10-Bit Mode (1.5 kHz in 12-Bit Mode)
   - Amplitude: 1.6 Vpp
   - Offset: 800 mV

F91x ADC Settings:
   - Number of ADC Samples: 64 (Default)
   - ADC Sampling Rate: 300 kHz in 8-Bit Mode (50 kHz in 12-Bit Mode)

How to Use:
--------------------------------------------------------------------------------
Refer to F91x_ADC_Performance.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 25 OCT 2013

---eof--------------------------------------------------------------------------

