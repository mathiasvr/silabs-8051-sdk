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
This program initializes the comparator, whose current output status can be
monitored using the crossbar-enabled pins P0.0 and P0.1. Both synchronous
and asynchronous comparator outputs are enabled. A switch connected to P1.7
allows the user to enable stop mode. While in stop mode, the comparator's
asynchronous output will continue to function, but the synchronous function
will disable.
 
Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
CMP0 (Comparator 0 - Enabled)
P0.0 (Comparator 0 - CP0 Synchronous Output)
P0.1 (Comparator 0 - CP0A Asynchronous Output)
P0.2 (Comparator 0 - Negative Input Pin)
LDO  (Comparator 0 - Positive Input Pin)

NOTE: P0.2 is the listed pin which corresponds to CP0N.0. With different board
      versions, it is possible that this negative input will be routed to a
	  different port pin.

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
Available Comparator Modes:
   - Normal Mode
      + HFOSC (Divided by 1)
      + CMP0
   - Stop Mode
      + CMP0 (Active)

Important User Warning:
   If an 'F85x MCU is programmed to enter stop mode immediately post-reset,
   the debugger will no longer be able to reprogram or interact with the
   board, rendering it inoperable. This example code features a switch
   which acts as a safeguard to prevent the MCU from entering stop mode
   until after the switch is pressed. If the user chooses to remove this 
   safeguards, they do so at their own risk.

How to Use:
--------------------------------------------------------------------------------
Refer to F85x_Comparator.c for detailed directions on this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (PMD)
	- 8 JUL 2013

---eof--------------------------------------------------------------------------

