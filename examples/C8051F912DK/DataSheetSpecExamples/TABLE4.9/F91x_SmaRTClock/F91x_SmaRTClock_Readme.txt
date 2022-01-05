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
This example demonstrates the consistency of the SmaRTClock Oscillator freqency
through the full voltage supply range (1.8 V to 3.6 V). This feature only
available on ‘F912 and ‘F902 devices. The clock frequency can be measured
through P0.0.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.0 (SYSCLK - LP Oscillator)

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.2

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.9 SmaRTClock Internal Low Frequency Oscillator (16.4 kHz)
   
Typical Expected Specs (typical is 16.4 kHz):

   At 1.8V - 17.2 kHz
   At 1.9V - 17.1 kHz
   At 2.0V - 17.1 kHz
   At 2.1V - 16.9 kHz
   At 2.2V - 16.9 kHz
   At 2.3V - 16.9 kHz
   At 2.4V - 16.8 kHz
   At 2.5V - 16.8 kHz
   At 2.6V - 16.8 kHz
   At 2.7V - 16.7 kHz
   At 2.8V - 16.7 kHz
   At 2.9V - 16.5 kHz
   At 3.0V - 16.5 kHz
   At 3.1V - 16.5 kHz
   At 3.2V - 16.5 kHz
   At 3.3V - 16.5 kHz
   At 3.4V - 16.5 kHz
   At 3.5V - 16.5 kHz
   At 3.6V - 16.7 kHz

How to Use:
--------------------------------------------------------------------------------
Refer to F91x_SmaRTClock.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 23 OCT 2013 

---eof--------------------------------------------------------------------------
