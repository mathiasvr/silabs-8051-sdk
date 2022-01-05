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
write function runs for 10240 cycles in order to give the user enough room for
measurement (erase function runs for 5 cycles).

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P1.5 (LED toggle for Flash Write/Erase and Pin Used for Measurement)
P0.2 (Flash Write Initialization Switch)
P0.3 (Flash Erase Initialization Switch)

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.3

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.6 Flash
   - Flash Current on VDD
      + Flash Write - Current: TBD mA
      + Flash Erase - Current: TBD mA

   - Flash Write Time
      + Min: 57   us
      + Typ: 64   us
      + Max: 71	  us

   - Flash Erase Time
      + Min: 28   ms
      + Typ: 32   ms
      + Max: 36   ms

How to Use:
--------------------------------------------------------------------------------
Refer to F93x_Flash_Access.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 11 SEP 2013

---eof--------------------------------------------------------------------------
