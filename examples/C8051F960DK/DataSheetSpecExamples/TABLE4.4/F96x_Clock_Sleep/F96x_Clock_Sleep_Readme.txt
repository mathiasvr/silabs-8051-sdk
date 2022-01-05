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
CPU mode. A switch with port match will wake the processor, switch mode, 
then put the processor back to sleep when pushed.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.1 (SW2 - MCU Mode Cycle Switch)

Product Family:
--------------------------------------------------------------------------------
C8051F96x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.4 Digital Core (SYSCLK) - Suspend and Sleep Mode, LCD Disable
   - Suspend Mode
      + Min: --
      + Typ: 84 uA
      + Max: --	
   
   - Sleep Mode, SmaRTClock running (crystal)
      + Min: --
      + Typ: 0.35 uA
      + Max: --

   - Sleep Mode
      + Min: --
      + Typ: 0.08 uA
      + Max: --

How to Use:
--------------------------------------------------------------------------------
Refer to F96x_Clock_Sleep.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 16 OCT 2013

---eof--------------------------------------------------------------------------

