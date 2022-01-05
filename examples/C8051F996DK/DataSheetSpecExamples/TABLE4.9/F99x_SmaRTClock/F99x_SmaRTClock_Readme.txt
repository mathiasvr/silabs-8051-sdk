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
This code is used to show the consistency of the SmaRTClock Internal Low 
Frequency Oscillator through the full voltage supply range (1.8 V to 3.6 V).
The program initializes the crossbar to map the clock to P0.0. Then the System 
Clock is set to the Low Frequency Oscillator. 

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.0 (SYSCLK - LP Oscillator)

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.1

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.9 SmaRTClock Internal Low Frequency Oscillator (16.4 kHz)
   
Typical Expected Specs (typical is 16.4 kHz):

   At 1.8V - 16.5 kHz
   At 1.9V - 16.5 kHz
   At 2.0V - 16.4 kHz
   At 2.1V - 16.3 kHz
   At 2.2V - 16.3 kHz
   At 2.3V - 16.1 kHz
   At 2.4V - 16.1 kHz
   At 2.5V - 16.1 kHz
   At 2.6V - 16.1 kHz
   At 2.7V - 16.0 kHz
   At 2.8V - 16.0 kHz
   At 2.9V - 15.9 kHz
   At 3.0V - 15.9 kHz
   At 3.1V - 15.9 kHz
   At 3.2V - 15.7 kHz
   At 3.3V - 16.0 kHz
   At 3.4V - 15.9 kHz
   At 3.5V - 16.0 kHz
   At 3.6V - 16.0 kHz

How to Use:
--------------------------------------------------------------------------------
Refer to F99x_SmaRTClock.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 29 OCT 2013 

---eof--------------------------------------------------------------------------
