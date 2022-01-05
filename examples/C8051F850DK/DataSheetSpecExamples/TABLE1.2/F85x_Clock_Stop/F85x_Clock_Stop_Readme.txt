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
This example code demonstrates the power consumption and use of different Low 
Dropout Regulator (LDO) settings in stop mode. A switch is implemented which 
prevents the program from immediately from entering stop mode, primarily as a 
safety precaution to protect the MCU from being forcibly stuck in stop mode.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P1.7 (S1 - Stop Mode Activate Switch)

Product Family:
--------------------------------------------------------------------------------
C8051F85x/86x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5 6/13

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 1.2 Digital Core (SYSCLK) - Stop Mode
   - LDO Status = Enabled
      + Min: --
      + Typ: 105 uA
      + Max: --
   - LDO Status = Disabled
      + Min: --
      + Typ: 0.2 uA
      + Max: --

Notes On Example and Modes:
--------------------------------------------------------------------------------
Available Low Dropout Regulator Settings:
   - LDO Enabled (Standard Stop Mode) (Default Mode)
      + All clocks stopped
	   + Core LDO and comparators still running
	   + Pins retain state
   - LDO Disabled (Shutdown Mode)
      + All clocks stopped
	   + Core LDO and all analog circuits shut down
	   + Pins retain state
   
Important User Warning:
   If an 'F85x MCU is programmed to enter stop mode immediately post-reset,
   the debugger will no longer be able to reprogram or interact with the
   board, rendering it inoperable. This example code features a switch
   which acts as a safeguard to prevent the MCU from entering stop mode
   until after the switch is pressed. If the user chooses to remove this 
   safeguards, they do so at their own risk.
  
How to Use:
--------------------------------------------------------------------------------
Refer to F85x_Clock_Stop.c for detailed directions on using this example.

  
Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (PMD)
	- 2 AUG 2013

---eof--------------------------------------------------------------------------

