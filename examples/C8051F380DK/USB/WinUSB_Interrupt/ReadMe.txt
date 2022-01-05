		 --------------------------------------------------
                 USB Interrupt Example Readme File
           Copyright 2014 Silicon Laboratories, Inc.
		 --------------------------------------------------

1.0 COMPONENTS
-----------------------

This release contains the following components:

        * USB Interrupt Driver (WinUSB)
        * USB Interrupt Firmware Example
        * Host Application (SLAB_USB_Interrupt.exe)
        * Host Application Source


Last updated: 05 NOV 2012

2.0 Running the Example on the C8051F380 target board
-----------------------------------------------------

	1) Connect the hardware:

		a) Connect the target board to a PC through a Debug Adapter.

		b) Install shorting blocks at the following positions: 
		   J12[1-2] (P2.0 Switch), J12[3-4] (P2.1 Switch), 
		   J12[5-6] (P2.2 LED), J12[7-8] (P2.3 LED), J10 (P0.6), J11 (P0.7) 
		   and J16/J17 (P2.5 Potentiometer).

		c) Connect a USB cable between the target device and the host PC.

	2) Build and download the Interrupt Example firmware:

		a) Open the Silicon Laboratories IDE (v1.72 or later).

		b) Open the Connection Options window by selecting Options->
		   Connection Options... in the IDE menus. In the Debug Interface 
		   section, select C2.

		c) Connect to the target C8051F38x device (Alt+C, or IDE menu 
		   Debug->Connect).

		d) From the Project menu, select "Open Project" and browse to the 
		   Interrupt Example Firware project "USB0_Interrupt.wsp", located by
           default in the "Silabs\MCU\Examples\C8051F38x\USB_Interrupt\Firmware"
           directory.

		e) Build the project (F7, or through IDE menu Project->Build/Make Project).
		
		f) Download the firmware (Alt+D).

	3) Run the Interrupt Example firmware:

		a) In the IDE, Run the firmware (F5, or IDE menu Debug->Go).

		b) When Windows detects the device and the driver installation wizard opens:
		   a. Choose "Search for a suitable driver for my device", and click Next.
		   b. Check the box "Specify a location", and click Next.
		   c. Browse to directory ..\Driver and select the file SLAB_USB_Interrupt.inf,
              and click OK.
		   d. Follow the dialogue to finish driver installation.

		c) Once the firmware has been downloaded and is running, the IDE is no 
		   longer needed. Disconnect from the target device with the IDE and 
		   press the Reset button on the Target Board.

	4) Run the Interrupt Example host application:

		a) Run the SLAB_USB_Interrupt.exe application located by default in
		   C:\SiliconLabs\SimplicityStudio\v4\developer\sdks\8051\v#.#.#\examples\C8051F320DK\USB\WinUSB_Interrupt\host
		   where #.#.# is the SDK version number.

		b) The USB Interrupt Example application will display the current USB
		   devices connected to the PC. Select the device to be used for the 
		   example and open the device.

		c) Observe changes to SW1, SW2, Port pins, Potentiometer, and Temperature.
        
        d) Configure LED1, LED2, and Port pins by clickin on the appropriate
           controls.


3.0 Building the PC Application
-------------------------------

The PC application requires libraries for setupapi.lib and winusb.lib from the
Windows DDK. Please download the latest Windows DDK (or WDK) from Microsoft.

By default, the project has been configured to build from the Windows DDK
installed to C:\WINDDK\7600.16385.1.

To build using a different Windows DDK path, update the following directories:
1. Under Project->Properties->C/C++->General, update the "Additional Include
   directories" text box for the new path.
2. In DeviceInformation.cpp, update the #pragma comment (lib, ...) line using
   the new path to setupapi.lib.
3. In WinUsbDevice.cpp, update the #pragma comment (lib, ...) line using
   the new path to winusb.lib.


4.0 REVISION HISTORY
---------------------

Version 2.0
	Rewrite.
