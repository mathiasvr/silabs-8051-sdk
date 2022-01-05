-------------------------------------------------------------------------------
 Readme.txt
-------------------------------------------------------------------------------

Copyright 2014 Silicon Laboratories, Inc.
http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt

Program Description:
-------------------

This program demonstrates how the example communicates with a PC application to
control LED blinking.


How To Test: C8051F326-TB
-------------------------

1) Ensure shorting blocks on C8051F326-TB are place on the following:
   J2: VDD  - +3VD
   J3: P2.2 - P2.2_LED
   J3: P2.3 - P2.3_LED
   J7: VDD  - VIO
   J8: Power selection, P1 9V or J4 Debugger
2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
   connect the 9V power adapter to P1.
3) Include the appropriate "F3xx_Blink_Control_F326.c" file to build and
   exclude other device source files from build by right-clicking on these
   files in the project window and selecting
   Resource Configurations -> Exclude from Build... to remove objects from
   building.
4) Compile and download code to C8051F326 target board with Simplicity
   IDE by selecting Run -> Debug from the menus,
   or clicking the Debug button in the quick menu,
   or pressing F11.
5) Run the code by selecting Run -> Resume from the menus,
   or clicking the Resume button in the quick menu,
   or pressing F8.
6) Connect USB cable to J9 and PC.
7) Run the GUI application HID_Blinky that comes with the firmware
   and test the features:
   C:\SiliconLabs\SimplicityStudio\v4\developer\sdks\8051\v#.#.#\examples\C8051F320DK\USB\HID\BlinkyExample\host
   where #.#.# is the SDK version number.


Known Issues and Limitations:
----------------------------

1)  Firmware works with the Silicon Labs Simplicity Studio and the
    Keil C51 tool chain. Project and code modifications will be
    necessary for use with different tool chains.

2)  Place jumpers on target board to connect the LEDs.

3)  Windows application supports Win2K, XP, Win7 and 98SE only.


Target and Tool Chain Information:
---------------------------------

Target:         C8051F326/7
Tool chain:     Simplicity Studio / Keil C51 9.51
Project Name:   F3xx_BlinkyExample


Command Line Options:
--------------------

Assembler : Default
Compiler  : Default
Linker    : Default


File List:
---------

c8051f3xx.h
F3xx_Blink_Control.h
F3xx_USB0_Descriptor.h
F3xx_USB0_InterruptServiceRoutine.h
F3xx_USB0_Register.h
F3xx_USB0_ReportHandler.h
F3xx_Blink_Control_F326.c
F3xx_USB0_Descriptor.c
F3xx_USB0_InterruptServiceRoutine.c
F3xx_USB0_USB0_Main.c
F3xx_USB0_ReportHandler.c
F3xx_USB0_Standard_Requests.c
ReadMe.txt (this file)


Release Information:
-------------------

Version 1.2 (JL)
        Updated How To Test using with Simplicity Studio.
        13 MAY 2014

Version 1.1
        Added support for F38x, T620, and T622 familes. (ES)
        20 JAN 2011

Version 1.0
        Initial release (PD).
        13 DEC 2005
-------------------------------------------------------------------------------
 End Of File
-------------------------------------------------------------------------------