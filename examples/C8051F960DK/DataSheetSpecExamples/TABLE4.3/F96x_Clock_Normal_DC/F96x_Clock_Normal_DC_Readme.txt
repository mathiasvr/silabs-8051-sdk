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
This example code demonstrates the power consumption of using internal precision 
oscillator in normal mode. This code example allows the user to both measure
the current consumption as well as monitor the oscillator signal to confirm the
oscillator frequency. 

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.3 (SYSCLK Output)
P0.2 (SW3 - Measurement Mode Cycle Switch)
PROSC(Precision Oscillator)

Product Family:
--------------------------------------------------------------------------------
C8051F96x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.3 Digital Supply Current  - Normal Mode
   - VBAT 3.0 V
      + Min: --
      + Typ: 4.1 mA
      + Max: --
   
   - VBAT 3.3 V
      + Min: --
      + Typ: 4.0 mA
      + Max: --
      
    - VBAT 3.6 V
      + Min: --
      + Typ: 3.8 mA
      + Max: --
      
Notes On Example and Modes:
--------------------------------------------------------------------------------
Available Oscillator Settings:
   - 24.5 MHz (Precision internal Oscillator) (Default Mode)
      + PROSC (Divided by 1)
  
	 
Available Measurement Settings:
   - Oscilloscope Mode (P0.3 Enabled)
   - Current Mode (P0.3 Disabled)

Note: Accurate current readings cannot be made while using oscilloscope mode,
      because a significant current draw is made by the P0.3 push-pull output.
  
How to Use:
--------------------------------------------------------------------------------
Refer to F96x_Clock_Normal_DC.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 15 OGT 2013

---eof--------------------------------------------------------------------------

