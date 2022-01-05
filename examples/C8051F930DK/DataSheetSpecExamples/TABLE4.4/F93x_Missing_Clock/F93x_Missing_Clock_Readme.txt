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
This example demonstrates the Missing clock reset detecting and Missing
clock reset detector timeout. Pressing a switch to change system clock
from 32kHz to less than 10kHz that will cause the processor to reset
triggered by the Missing Clock Detector.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.2 (SW0.2 - System clock Switch)
P1.5 (DS3 - missing clock timeout point out)
P1.6 (DS4 - missing clock last reset point out)

Product Family:
--------------------------------------------------------------------------------
C8051F93x/92x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.3 

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.4 Reset - Missing Clock Detector
   - System clock frequency which triggers a missing clock detector timeout
      + Min: --
      + Typ: 7  kHz
      + Max: 10	kHz
   
   - Missing Clock Detector timeout
      + Min: 100  us
      + Typ: 650  us
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
Refer to F93x_Missing_Clock.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 10 SEP 2013

---eof--------------------------------------------------------------------------

