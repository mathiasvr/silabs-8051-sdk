-------------------------------------------------------------------------------
 Readme.txt
-------------------------------------------------------------------------------

Copyright 2014 Silicon Laboratories, Inc.
http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt

Program Description:
-------------------

This program demonstrates the implementation of a simple USB application
using the interrupt transfer type. This includes support for device enumeration,
control and interrupt transactions, and definitions of descriptor data. The
purpose of this software is to give a simple working example of an interrupt
transfer application; it does not include support for multiple configurations,
or other transfer types.

Packet Descriptions:
-------------------

IN Endpoint 1 (Device to Host, 8-bytes):
Packet[0] - SW1
Packet[1] - SW2
Packet[2] - P0.0 - P0.3 (Read latch value)
             -------------------------------
            | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
            | x | x | x | x |P03|P02|P01|P00|
             -------------------------------

Packet[3] - Potentiometer
Packet[4] - Temperature
Packet[5-7] - Unused

OUT Endpoint 1 (Host to Device, 8-bytes):
Packet[0] - LED1
Packet[1] - LED2
Packet[2] - P0.4 - P0.7 (Write latch value)
             -------------------------------
            | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
            |P07|P06|P05|P04| x | x | x | x |
             -------------------------------

Packet[3-7] - Unused


This project can be configured for full-speed or low-speed USB:

1) Full-speed (default): Configure full-speed by leaving _USB_LOW_SPEED_,
   in F3xx_USB0_InterruptServiceRoutine.h, undefined.

   System Clock:        24 MHz (Clock Multiplier / 2)
   USB Clock:           48 MHz (Clock Multiplier using Internal Oscillator * 2)
   USB Bit Rate:        12 Mbps
   Timer Overflow Rate: 50 Hz

2) Low-speed: Configure low-speed by defining _USB_LOW_SPEED_, in
   F3xx_USB0_InterruptServiceRoutine.h.

   System Clock:        12  MHz (Internal Oscillator)
   USB Clock:           6   MHz (Internal Oscillator / 2)
   USB Bit Rate:        1.5 Mbps
   Timer Overflow Rate: 25  Hz

   Note: Low-speed limits max endpoint packet sizes to 8 bytes

How To Test: C8051F326-TB
-----------

1) Import the project file to Simplicity Studio.
2) Ensure shorting blocks on C8051F326-TB are place on the following:
   J2: VDD  - +3VD
   J3: P2.2 - P2.2_LED
   J3: P2.3 - P2.3_LED
   J7: VDD  - VIO
   J8: Power selection, P1 9V or J4 Debugger
3) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
   connect the 9V power adapter to P1.
4) Compile and download code to C8051F326 target board with Simplicity
   IDE by selecting Run -> Debug from the menus,
   or clicking the Debug button in the quick menu,
   or pressing F11.
5) Run the code by selecting Run -> Resume from the menus,
   or clicking the Resume button in the quick menu,
   or pressing F8.
6) Run the GUI application SLAB_USB_Interrupt that comes with the firmware
   and test the features:
   C:\SiliconLabs\SimplicityStudio\v4\developer\sdks\8051\v#.#.#\examples\C8051F320DK\USB\WinUSB_Interrupt\host
   where #.#.# is the SDK version number.


Known Issues and Limitations:
----------------------------

1)  Firmware works with the Simplicity Studio and the
    Keil C51 tool chain. Project and code modifications will be
    necessary for use with different tool chains.

2)  Place jumpers on target board to connect the LEDs, switches, and
    the Pot to the microcontroller, if necessary.

3)  Windows application and driver supports Win XP and Win7 only.


Target and Tool Chain Information:
---------------------------------

Target:         C8051F326/7
Tool chain:     Simplicity Studio / Keil C51 9.51
Project Name:   USB0_Interrupt


Command Line Options:
--------------------

Assembler : Default
Compiler  : Default
Linker    : Default


File List:
---------

c8051f3xx.h
F3xx_USB0_Main.h
F3xx_USB0_Descriptor.h
F3xx_USB0_InterruptServiceRoutine.h
F3xx_USB0_Register.h
F326_USB0_Interrupt.c
F3xx_USB0_Descriptor.c
F3xx_USB0_InterruptServiceRoutine.c
F3xx_USB0_Main.c
F3xx_USB0_Standard_Requests.c
ReadMe.txt (this file)

Release Information:
-------------------
Version 2.1
        Updated How To Test using with Simplicity Studio. (JL)
        20 MAY 2014

Version 2.0
        Rewrite (CM).
        02 NOV 2012

-------------------------------------------------------------------------------
 End Of File
-------------------------------------------------------------------------------