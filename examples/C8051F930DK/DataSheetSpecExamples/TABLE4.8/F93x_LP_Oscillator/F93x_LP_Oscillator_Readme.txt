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
This example demonstrates the consistency of the Internal Low Power Oscillator
frequency through the full voltage supply range (1.8 V to 3.6 V). The system
clock can be measured through P0.0.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.0 (SYSCLK - LP Oscillator)

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.3

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.8 Internal Low Power Oscillator (20 MHz)
   
Typical Expected Specs (typical is 20 MHz):

   At 1.8V - 20.32 MHz
   At 1.9V - 20.40 MHz
   At 2.0V - 20.48 MHz
   At 2.1V - 20.40 MHz
   At 2.2V - 20.32 MHz
   At 2.3V - 20.32 MHz
   At 2.4V - 20.32 MHz
   At 2.5V - 20.24 MHz
   At 2.6V - 20.40 MHz
   At 2.7V - 20.24 MHz
   At 2.8V - 20.40 MHz
   At 2.9V - 20.16 MHz
   At 3.0V - 20.24 MHz
   At 3.1V - 20.24 MHz
   At 3.2V - 20.08 MHz
   At 3.3V - 20.24 MHz
   At 3.4V - 20.16 MHz
   At 3.5V - 20.24 MHz
   At 3.6V - 20.24 MHz

How to Use:
--------------------------------------------------------------------------------
Refer to F93x_LP_Oscillator.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 12 SEP 2013 

---eof--------------------------------------------------------------------------
