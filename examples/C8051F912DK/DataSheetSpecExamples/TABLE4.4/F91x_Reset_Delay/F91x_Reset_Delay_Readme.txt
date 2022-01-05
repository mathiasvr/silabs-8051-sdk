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
This example code demonstrates the different reset delays when turning on
the device. The two reset delays described in this example are the Power-On
reset, and the Non Power-On reset. The first is caused when the board
is supplied with power to VDD after having been completely powered off.
The second reset source occurs when any standard reset source causes a
system reboot. In this example, the user pushes the RST switch to supply
this reset source.

Resources Used:
--------------------------------------------------------------------------------
WDT0   (Watchdog Timer - Disabled)
P1.5   (LED1 - Non-POR Delay Monitor)
P1.6   (LED2 - POR Delay Status Monitor)

Part Family:
--------------------------------------------------------------------------------
C8051F91x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.2

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.4 VBAT Ramp Time for Power-On Reset (POR)
   - Delay from VDD On to RST Logic High:
      + Min: --
      + Typ: --
      + Max: 3 ms

Table 4.4 Reset Time Delay (RST) - Non-POR Delay
   - Delay from VDD Threshold (1.8 V) to Code Execution:
      + Min: --
      + Typ: 10 us
      + Max: --

Notes On Example and Modes:
--------------------------------------------------------------------------------
Available Reset Modes:
   - Power-On Reset
      + Activate board by supplying VDD
      + Does not use firmware

   - Non Power-On Reset
      + Activate board by pushing RST
      + Uses firmware to facilitate measurement

NOTE: These modes are not determined by firmware, but by how the user operates
      the device being tested. Follow the instructions given in the "How to
      Measure" section to test the correct mode.


How to Use:
--------------------------------------------------------------------------------
Refer to F91x_Reset_Delay.asm for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 22 OCT 2013

---eof--------------------------------------------------------------------------

