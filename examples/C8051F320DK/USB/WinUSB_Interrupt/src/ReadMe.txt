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

Project Note:
------------

This project is organized so that it can be compiled for six different
microcontroller families: The F320/1, F326/7, F34x, F38x, T6220/1/6/7 
T320/3, and T622/3 T326/7 families.  

There are two changes that need to be made to customize the project for
each family:

1) In the file c8051f3xx.h, change the include file to family being used
   by modifying the TARGET_MCU definition as appropriate.
2) Include the appropriate "xxxx_USB0_Interrupt.c" file in the build.
   This is done by right-clicking on the file in the project window and
   adding to the build.  

If this example was found on the Silicon Laboratories Development kit CD,
these changes have already been made for the respective device.

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
   
How To Test:
-----------

1) Open the project file in the Silicon Labs IDE.
2) Open C8051f3xx.h and define TARGET_MCU as MCU_F320, MCU_F326, MCU_340,
   MCU_380, MCU_T620, or MCU_T622.
3) Download the code to any C8051F32x, C8051F34x, C8051F38x, or C8051T62x
   target board using the Silicon Labs IDE.
4) Run the GUI application that comes with the firmware and test the features.


Known Issues and Limitations:
----------------------------
	
1)  Firmware works with the Silicon Labs IDE v1.71 or later and the 
	Keil C51 tool chain. Project and code modifications will be 
	necessary for use with different tool chains.

2)  Place jumpers on target board to connect the LEDs, switches, and
    the Pot to the microcontroller, if necessary.

3)  Windows application and driver supports Win XP and 7 only.


Target and Tool Chain Information:
---------------------------------

Target:         C8051F320/1,
                C8051F326/7,
                C8051F34x,
                C8051F38x,
                C8051T620/1/6/7, C8051T320/3,
                C8051T622/3, C8051T326/7
Tool chain:     Keil / Raisonance
                Silicon Laboratories IDE version 2.6
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
F320_USB0_Interrupt.c
F326_USB0_Interrupt.c
F340_USB0_Interrupt.c
F380_USB0_Interrupt.c
T620_USB0_Interrupt.c
T622_USB0_Interrupt.c
F3xx_USB0_Descriptor.c
F3xx_USB0_InterruptServiceRoutine.c
F3xx_USB0_Main.c
F3xx_USB0_Standard_Requests.c
ReadMe.txt (this file)


Release Information:
-------------------
	        
Version 2.0
	Rewrite (CM).
        02 NOV 2012

-------------------------------------------------------------------------------
 End Of File
-------------------------------------------------------------------------------
