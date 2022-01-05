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
This code is used to show the consistency of the Internal Precision Oscillator
through the full voltage supply range (1.8 V to 3.6 V). The program initializes
the crossbar to map the clock to P0.0. Then the System Clock is set to the
Internal Precision Oscillator.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.0 (SYSCLK - PR Oscillator)

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.1

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.7 Internal Precision Oscillator (24.5 MHz)
   
Typical Expected Specs (typical is 24.5 MHz):

   At 1.8V - 24.75 MHz
   At 1.9V - 24.75 MHz
   At 2.0V - 24.51 MHz
   At 2.1V - 24.51 MHz
   At 2.2V - 24.63 MHz
   At 2.3V - 24.75 MHz
   At 2.4V - 24.51 MHz
   At 2.5V - 24.63 MHz
   At 2.6V - 24.87 MHz
   At 2.7V - 24.87 MHz
   At 2.8V - 24.75 MHz
   At 2.9V - 24.63 MHz
   At 3.0V - 24.75 MHz
   At 3.1V - 24.63 MHz
   At 3.2V - 24.75 MHz
   At 3.3V - 24.63 MHz
   At 3.4V - 24.75 MHz
   At 3.5V - 24.63 MHz
   At 3.6V - 24.75 MHz

How to Use:
--------------------------------------------------------------------------------
Refer to F99x_PR_Oscillator_F.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 29 OCT 2013 

---eof--------------------------------------------------------------------------
