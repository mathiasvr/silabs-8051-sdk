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
This code is used to show the consistency of the HF Oscillator through the full
voltage supply range (2.2 V to 3.6 V). The program initializes the crossbar to
map the clock to P0.0. Then the System Clock is set to the HF Oscillator.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.0 (SYSCLK - HF Oscillator)

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5 6/13

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 1.5 HF Oscillator (24.5 MHz)
   
Typical Expected Specs (typical is 24.5 MHz):
   At 2.2V - 24.61 MHz
   At 2.3V - 24.53 MHz
   At 2.4V - 24.49 MHz
   At 2.5V - 24.58 MHz
   At 2.6V - 24.56 MHz
   At 2.7V - 24.58 MHz
   At 2.8V - 24.39 MHz
   At 2.9V - 24.61 MHz
   At 3.0V - 24.56 MHz
   At 3.1V - 24.63 MHz
   At 3.2V - 24.56 MHz
   At 3.3V - 24.56 MHz
   At 3.4V - 24.58 MHz
   At 3.5V - 24.53 MHz
   At 3.6V - 24.53 MHz

How to Use:
--------------------------------------------------------------------------------
Refer to F85x_HF_Oscillator.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision
	- 11 JULY 2013

---eof--------------------------------------------------------------------------