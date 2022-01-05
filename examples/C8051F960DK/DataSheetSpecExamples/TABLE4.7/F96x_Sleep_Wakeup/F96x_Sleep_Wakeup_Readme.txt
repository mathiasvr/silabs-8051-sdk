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
This example code demonstrates the sleep wakeup time. A switch with port match
will wake the processor, change LED1 state, then put the processor back to sleep
.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.1 (SW2 - MCU Mode Cycle Switch)
P0.0 (LED1 - MCU Mode indicator)
Product Family:
--------------------------------------------------------------------------------
C8051F96x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.7 Idle Wakeup Time
   - No special condition
      + Min: 2  SYSCLKs
      + Typ: --
      + Max: 3  SYSCLKs
      
Table 4.5 Suspend Wakeup Time
   - Low power or precision oscillator
      + Min: --
      + Typ: 400 ns
      + Max: --	
      
Table 4.7 Sleep Wakeup Time  
   - No special conditions
      + Min: --
      + Typ: 2 us
      + Max: --	
  
How to Use:
--------------------------------------------------------------------------------
Refer to F96x_Sleep_Wakeup.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 04 NOV 2013

---eof--------------------------------------------------------------------------

