-------------------------------------------------------------------------------
 Readme.txt
-------------------------------------------------------------------------------

Copyright 2014 Silicon Laboratories, Inc.
http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt

Program Description:
-------------------

This program demonstrates the implementation of a simple USB application 
using the bulk transfer type. This includes support for device enumeration, 
control and bulk transactions, and definitions of descriptor data. The
purpose of this software is to give a simple working example of a bulk 
transfer application; it does not include support for multiple configurations, 
or other transfer types.

The file transfer firmware example uses a state machine to handle commands
initiated by the host application. These commands allow the host to query
the device for flash storage information, set flash key values, and read/write
flash pages allocated for file storage. 512 byte flash page reads/writes are
broken into 64-byte packets when sent over the bulk endpoints.

The file transfer command protocol allows for variable page sizes and a
variable number of flash pages used for file storage. Some simplifications
have been made to support the 512-byte flash page size found on all supported
target devices. A 512-byte flash page can be transferred using 8, 64-byte
packets with no protocol overhead. More complex protocols might include
packet lengths and control fields and are beyond the scope of this example.

Packet Descriptions:
-------------------

Bulk IN Endpoint 1 (Device to Host, 64-bytes):
Response[64] - Contains a response or read page packet for a host initiated
command

Bulk OUT Endpoint 1 (Host to Device, 64-bytes):
Command[64] - Contains a command or write page packet from the host

File Transfer Command Protocol:
------------------------------

The file transfer command protocol has the following commands:

Command Name             ID
--------------------    ----
Set Flash Key           0x00
Get Page Information    0x01
Write Page              0x02
Read Page               0x03

The command protocol is an over-simplified protocol that assumes
a fixed packet size of 64 byte. Variable packet sizes could be used
if necessary.

0.0 - Set Flash Key Command (ID 0x00):
-------------------------------------

Sets the flash key values used for flash write and erases. Flash keys
are not stored in ROM to reduce the likelihood of flash corruption.

Enable flash writes/erases by sending 0xA5, 0xF1
Disable flash writes/erases by sending 0x00, 0x00

Attempting to write or erase flash using a write page command with an
incorrect flash key will lock the flash write/erase interface until
the device is reset (see Reset State control request below).

Command[0] - 0x00 (Set Flash Key Command ID)
Command[1] - FLKEY[0]
Command[2] - FLKEY[1]
Command[3-63] - unused

0.1 - Set Flash Key Response:
----------------------------

Response[0] - 0x00 (Success)
Response[1-63] - unused

1.0 - Get Page Information Command (ID 0x01):
--------------------------------------------

Returns the number of flash pages being used for file storage and
the size of flash pages in bytes.

Command[0] - 0x01 (Get Page Information Command ID)
Command[1-63] - unused

1.1 - Get Page Information Response:
-----------------------------------

Response[0] - 0x00 (Success)
Response[1] - Number of writeable flash pages
Response[2-3] - Size of flash page in bytes (little-endian)
Response[4-63] - unused

2.0 - Write Page Command (ID 0x02):
----------------------------------

Writes an entire flash page to the flash region allocated for file
storage. The page number is indexed from 0 to (num pages - 1).
Page 0 is the first page in the flash storage region and is not
the flash page at address 0x0000.

Command[0] - 0x02 (Write Page Command ID)
Command[1] - Page number
Command[2-63] - unused

2.1 - Write Page Block 0 - 7 (8, 64-byte blocks for a 512 byte page):
--------------------------------------------------------------------

The host sends 8 raw 64-byte packets to the device containing the
data to write to the flash page. Even if the flash page is invalid,
the device must still receive 8 data packets.

Command[0-63] - Page N block write data

2.2 - Write Page Response:
-------------------------

The device sends a response indicating the status of the page write.
Invalid indicates that the page number specified in the command packet
is out of range.

Response[0] - 0x00 (Success), 0x01 (Invalid)
Response[1-63] - unused

3.0 - Read Page Command (ID 0x03):
---------------------------------

Reads an entire flash page from the flash region allocated for file
storage. The page number is indexed from 0 to (num pages - 1).
Page 0 is the first page in the flash storage region and is not
the flash page at address 0x0000.

Command[0] - 0x03 (Read Page Command ID)
Command[1] - Page number
Command[2-63] - unused

3.1 - Read Page Block 0 - 7 (8, 64-byte blocks for a 512 byte page):
-------------------------------------------------------------------

The device sends 8 raw 64-byte packets to the host containing the
specified flash page data. Even if the requested flash page is invalid,
the device must still send 8 data packets of dummy data.

Response[0-63] - Page N block read data

3.2 - Read Page Response:
------------------------

The device sends a response indicating the status of the page read.
Invalid indicates that the page number specified in the command packet
is out of range.

Response[0] - 0x00 (Success), 0x01 (Invalid)
Response[1-63] - unused

Vendor Specific Control Requests:
--------------------------------

The USB Bulk file transfer firmware example supports only a single control
request, Reset State. The reset state request is received in the USB ISR
and signals the state machine to return to the IDLE state, resetting the
USB IN/OUT FIFOs and IN/OUT firmware buffers. The reset state request has
an optional feature to unlock the flash write/erase interface. Attempting
to write or erase a flash page with an incorrect flash key will lock the
interface until a device reset occurs. The reset state request can check
if the flash interface is locked and perform a software reset to unlock
the flash interface.

Setup.bmRequestType - 0x40 (OUT, Vendor Specific, Device request)
Setup.bRequest      - 0x01 (Reset State Request)
Setup.wValue        - 0x0000 (default) or 0x0001 (unlock flash interface)
Setup.wIndex        - 0x0000
Setup.wLength       - 0x0000 (No data stage)

LED Status Indicators:
---------------------

Two LEDs, LED1 and LED2, are used to indicate that the device
is processing a Read Page or Write Page command. The corresponding
LED is on for the entire duration of the read or write page command.

LED1 - Read activity (on during page read command)
LED2 - Write activity (on during page write command)

Project Note:
------------

This project is organized so that it can be compiled for four different
microcontroller families: The F320/1, F326/7, F34x, and F38x families.  

There are two changes that need to be made to customize the project for
each family:

1) In the file c8051f3xx.h, change the include file to family being used
   by modifying the TARGET_MCU definition as appropriate.
2) Include the appropriate "F3xx_USB0_Interrupt.c" file in the build.
   This is done by right-clicking on the file in the project window and
   adding to the build.  

If this example was found on the Silicon Laboratories Development kit installer,
these changes have already been made for the respective device.

How To Test:
-----------

1) Open the project file in the Silicon Labs IDE.
2) Open C8051f3xx.h and define TARGET_MCU as MCU_F320, MCU_F326, MCU_340,
   or MCU_380.
3) Download the code to any C8051F32x, C8051F34x, C8051F38x target board using
   the Silicon Labs IDE.
4) Run the GUI application that comes with the firmware and test the features.


Known Issues and Limitations:
----------------------------
	
1)  Firmware works with the Silicon Labs IDE v1.71 or later and the 
	Keil C51 tool chain. Project and code modifications will be 
	necessary for use with different tool chains.

2)  Place jumpers on target board to connect the LEDs, if necessary.

3)  Windows application and driver supports Win XP and 7 only.


Target and Tool Chain Information:
---------------------------------

Target:         C8051F320/1,
                C8051F326/7,
                C8051F34x,
                C8051F38x
Tool chain:     Keil / Raisonance / SDCC
                Silicon Laboratories IDE version 2.6
Project Name:   USB0_Bulk


Command Line Options:
--------------------

Assembler : Default
Compiler  : Default
Linker    : Default 


File List:
---------

c8051f3xx.h
F3xx_Flash.h
F3xx_USB0_Bulk.h
F3xx_USB0_Descriptor.h
F3xx_USB0_InterruptServiceRoutine.h
F3xx_USB0_Main.h
F3xx_USB0_Register.h
F3xx_USB0_Standard_Requests.h
F3xx_USB0_Vendor_Requests.h
F320_USB0_Bulk.c
F326_USB0_Bulk.c
F340_USB0_Bulk.c
F380_USB0_Bulk.c
F3xx_Flash.c
F3xx_USB0_Descriptor.c
F3xx_USB0_InterruptServiceRoutine.c
F3xx_USB0_Main.c
F3xx_USB0_Standard_Requests.c
F3xx_USB0_Vendor_Requests.c
ReadMe.txt (this file)

Release Information:
-------------------

Version 2.0
	Rewrite (CM).
        02 NOV 2012

-------------------------------------------------------------------------------
 End Of File
-------------------------------------------------------------------------------
