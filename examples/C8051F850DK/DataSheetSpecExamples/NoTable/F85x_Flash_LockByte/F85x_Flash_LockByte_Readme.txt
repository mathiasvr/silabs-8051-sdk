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
This example code demonstrates the use of the debugger for code memory page
locking and tests code memory to ensure that the user has correctly locked
a given block of code memory.
 
Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)

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
- Location of Page Lock Byte (per MCU):
   + F850/3 and F860/3: 0x1FFF - Default Mode
   + F851/4 and F861/4: 0x0FFF
   + F852/5 and F862/5: 0x07FF
   
- Maximum Locked Pages (per MCU):
   + F850/3 and F860/3: 14 Pages (Lock Byte Values: 0xFE to 0xF1)
   + F851/4 and F861/4: 6 Pages  (Lock Byte Values: 0xFE to 0xF9)
   + F852/5 and F862/5: 2 Pages  (Lock Byte Values: 0xFE to 0xFD)
   
   Note: For more information, refer to the "Program Memory" section in the
         reference documentation for the F85x MCU.
		 
How to Use:
--------------------------------------------------------------------------------
Refer to F85x_Flash_LockByte.asm for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (PMD)
	- 8 JUL 2013

---eof--------------------------------------------------------------------------

