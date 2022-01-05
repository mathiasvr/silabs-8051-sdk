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
This code is used to show the consistency of the Internal Low Power Oscillator
through the full voltage supply range (1.8 V to 3.6 V). The program initializes
the crossbar to map the clock to P0.0. Then the System Clock is set to the
Internal Low Power Oscillator.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.0 (SYSCLK - LP Oscillator)

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.1

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.8 Internal Low Power Oscillator (20 MHz)
   
Typical Expected Specs (typical is 20 MHz):

   At 1.8V - 19.80 MHz
   At 1.9V - 20.00 MHz
   At 2.0V - 20.00 MHz
   At 2.1V - 19.60 MHz
   At 2.2V - 20.00 MHz
   At 2.3V - 20.00 MHz
   At 2.4V - 20.00 MHz
   At 2.5V - 19.80 MHz
   At 2.6V - 16.60 MHz
   At 2.7V - 20.00 MHz
   At 2.8V - 20.40 MHz
   At 2.9V - 20.00 MHz
   At 3.0V - 19.80 MHz
   At 3.1V - 19.80 MHz
   At 3.2V - 19.60 MHz
   At 3.3V - 19.80 MHz
   At 3.4V - 19.80 MHz
   At 3.5V - 20.00 MHz
   At 3.6V - 19.80 MHz

How to Use:
--------------------------------------------------------------------------------
Refer to F99x_LP_Oscillator.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 29 OCT 2013 

---eof--------------------------------------------------------------------------
