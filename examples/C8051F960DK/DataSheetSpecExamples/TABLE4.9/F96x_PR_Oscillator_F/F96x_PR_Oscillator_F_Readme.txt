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
through the full voltage supply range (1.8 V to 3.8 V). The program initializes
the crossbar to map the clock to P0.3. Then the System Clock is set to the
Internal Precision Oscillator.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.3 (SYSCLK - PR Oscillator)

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.9 Internal Precision Oscillator (24.5 MHz)
   
Typical Expected Specs (typical is 24.5 MHz):

   At 1.8V - 24.69 MHz
   At 1.9V - 25.31 MHz
   At 2.0V - 24.39 MHz
   At 2.1V - 24.39 MHz
   At 2.2V - 24.39 MHz
   At 2.3V - 25.00 MHz
   At 2.4V - 24.69 MHz
   At 2.5V - 24.39 MHz
   At 2.6V - 24.69 MHz
   At 2.7V - 24.69 MHz
   At 2.8V - 24.69 MHz
   At 2.9V - 25.00 MHz
   At 3.0V - 24.09 MHz
   At 3.1V - 25.00 MHz
   At 3.2V - 25.00 MHz
   At 3.3V - 24.69 MHz
   At 3.4V - 24.39 MHz
   At 3.5V - 25.00 MHz
   At 3.6V - 24.39 MHz
   At 3.7V - 24.69 MHz
   At 3.8V - 24.69 MHz
   
How to Use:
--------------------------------------------------------------------------------
Refer to F96x_PR_Oscillator_F.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 18 OCT 2013 

---eof--------------------------------------------------------------------------
