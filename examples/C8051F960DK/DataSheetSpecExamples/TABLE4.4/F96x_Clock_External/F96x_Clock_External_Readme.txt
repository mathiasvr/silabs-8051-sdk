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
This example code demonstrates the power consumption and use of different 
CPU modes with an external 1 MHz oscillator. 

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.0 (SW2)
P0.2 (XTAL1 - Analog I/O Oscillator Pin)
P0.3 (XTAL2 - Analog I/O Oscillator Pin)

Product Family:
--------------------------------------------------------------------------------
C8051F96x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.4 Digital Core (SYSCLK) - Normal Mode
   - Frequency = 1 MHz (External oscillator)
      + Min: --
      + Typ: 190 uA
      + Max: --   

Table 4.4 Digital Core (SYSCLK) - Idle Mode   
   - Frequency = 1 MHz (External oscillator)
      + Min: --
      + Typ: 360 uA
      + Max: --

Table 4.4 Digital Core (SYSCLK) - Low Power Idle Mode   
   - Frequency = 1 MHz (External oscillator)
      + Min: --
      + Typ: 280 uA
      + Max: --
  
How to Use:
--------------------------------------------------------------------------------
Refer to F96x_Clock_External.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 18 OCT 2013

---eof--------------------------------------------------------------------------

