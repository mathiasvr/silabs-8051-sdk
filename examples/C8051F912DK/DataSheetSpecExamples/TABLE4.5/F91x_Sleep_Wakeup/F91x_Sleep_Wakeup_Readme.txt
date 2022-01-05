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
This example code demonstrates the Power Management wakeup time. A switch with
port match will wake the processor, change DS3 state, then put the processor
back to sleep.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.2 (SW0_2 - MCU Mode Cycle Switch)
P1.5 (DS3 - MCU Mode indicator)
Product Family:
--------------------------------------------------------------------------------
C8051F91x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.2

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.5 Idle Wakeup Time
   - No special condition
      + Min: 2  SYSCLKs
      + Typ: --
      + Max: 3  SYSCLKs

Table 4.5 Suspend Wakeup Time
   - Low power oscillator
      + Min: --
      + Typ: 400 ns
      + Max: --

   - Precision oscillator
      + Min: --
      + Typ: 400 ns
      + Max: --

Table 4.5 Sleep Wakeup Time
   - Two-cell mode
      + Min: --
      + Typ: 2 us
      + Max: --

   - One-cell mode
      + Min: --
      + Typ: 10 us
      + Max: --


How to Use:
--------------------------------------------------------------------------------
Refer to F91x_Sleep_Wakeup.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 23 OCT 2013

---eof--------------------------------------------------------------------------

