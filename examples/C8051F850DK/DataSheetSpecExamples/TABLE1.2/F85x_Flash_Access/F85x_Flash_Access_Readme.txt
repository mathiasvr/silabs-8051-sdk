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
This code is used to demonstrate the power consumption of writing to and erasing
parts of flash memory. The system clock is first initialized. Then the user has
the option to run either the Flash_Write or the Flash_Erase function, where the
write function runs for 25000 cycles in order to give the user enough room for
measurement (erase function runs for 5 cycles).

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P1.0 (LED toggle for Flash Write/Erase and Pin Used for Measurement)
P1.7 (Flash Write Initialization Switch)
P2.1 (Flash Erase Initialization Switch)

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5 6/13

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 1.2 Flash Current on VDD
   
Typical Expected Specs:
   Flash Write - Current: TBD mA
   Flash Erase - Current: TBD mA
   
Table 1.4 Flash Write/Erase Time
   Flash Write - Time: 20 usec
   Flash Erase - Time: 5 msec
   
How to Use:
--------------------------------------------------------------------------------
Refer to F85x_Flash_Access.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (AJ)
	- 10 JULY 2013

---eof--------------------------------------------------------------------------