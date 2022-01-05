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
This example code demonstrates the use of the Comparator as a killswitch for
the PCA timer. If the comparator switches state, then the PCA will deactivate.
 
Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
PCA0 (Programmable Counter Array - Enabled)
P0.0 (Programmable Counter Array - CEX0 Output)
CMP0 (Comparator 0 - Enabled)
P0.1 (Comparator 0 - Negative Input Pin)
LDO  (Comparator 0 - Positive Input Pin)

Product Family:
--------------------------------------------------------------------------------
C8051F85x/86x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5 6/13

Specs Demonstrated:
--------------------------------------------------------------------------------
This code example implements the functionality of various MCU features. As
such, it does not explicitly demonstrate any numerical specifications.

Notes On Example and Modes:
--------------------------------------------------------------------------------
Available PCA Modes:
   - Comparator Clear Polarity 0
      + HFOSC (Divided by 1)
      + PCA0 (Inhibited on Logic Low)
      + CMP0
   - Comparator Clear Polarity 1
      + HFOSC (Divided by 1)
      + PCA0 (Inhibited on Logic High)
      + CMP0
         
How to Use:
--------------------------------------------------------------------------------
Refer to F85x_PCA0_Comparator_Kill.c for detailed directions on this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (PMD)
	- 8 JUL 2013

---eof--------------------------------------------------------------------------

