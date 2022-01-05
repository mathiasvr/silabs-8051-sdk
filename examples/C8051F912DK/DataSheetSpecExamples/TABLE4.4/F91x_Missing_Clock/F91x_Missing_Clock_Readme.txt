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
This example demonstrates the Missing clock reset detector and Missing
clock reset detector timeout. Press a switch could change system clock 
form 32kHz to less than 10kHz that will cause the processor be reset by
a missing clock detected.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.2 (SW0.2 - System clock Switch)
P1.5 (DS3 - missing clock timeout point out)
P1.6 (DS4 - missing clock last reset point out)

Product Family:
--------------------------------------------------------------------------------
C8051F91x/90x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.2 

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.4 Reset - Missing Clock Detector
   - System clock frequency which triggers a missing clock detector timeout
      + Min: --
      + Typ: 7  kHz
      + Max: 10	kHz
   
   - Missing Clock Detector Timeout
      + Min: 100  us
      + Typ: 525  us
      + Max: 1000 us

Notes On Example and Modes:
--------------------------------------------------------------------------------
Available Oscillator Settings:
   - 32 kHz (SmaRTClcok external crystal) (Default Mode)
      + SmaRTClcok (Divided by 1)
   - 4 kHz (low system frequency)
      + SmaRTClcok (Divided by 8)
 
How to Use:
--------------------------------------------------------------------------------
Refer to F91x_Missing_Clock.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 22 OCT 2013

---eof--------------------------------------------------------------------------

