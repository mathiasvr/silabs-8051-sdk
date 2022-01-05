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
This code is used to demonstrate the power consumption of the different modes
of the comparator peripheral. The system clock and comparator are first
initialized. Then, pushing S1 (P1.7) turns the Comparator On and Off in the mode
that the user defines in CPT0MD. The difference of the measured current along JP2
is the consumption of the Comparator at the defined mode.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P1.7 (S1 - Comparator Enable/Disable Switch)
P2.1 (S2 - Comparator Mode Switch)

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5 6/13

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 1.2 Analog Peripheral (CPnMD) - Comparator 0/1 (CMP0/1)

Typical Expected Specs:
   Mode 3 (CMP0) - 25 uA
   Mode 2 (CMP0) - 10 uA
   Mode 1 (CMP0) - 3 uA
   Mode 0 (CMP0) - 0.5 uA

Notes On Example and Modes:
--------------------------------------------------------------------------------
Available Comparator Modes (both CMP0 and CMP1):
   Mode 0 - Slowest Setting, Lowest Power
   Mode 1
   Mode 2
   Mode 3 - Fastest Setting, Highest Power
   
How to Use:
--------------------------------------------------------------------------------
Refer to F85x_Comparator_Performance.c for detailed directions on using this 
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (AJ)
	- 10 JULY 2013

---eof--------------------------------------------------------------------------