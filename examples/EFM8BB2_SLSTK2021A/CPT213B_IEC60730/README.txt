CPT213B Demo
-------------

This demo demonstrates the capacitive button function of CPT213B fixed
function board with a IEC compliant host device. This demo runs on the
IEC60730 software compliant EFM8BB2 STK, and communicates with
CPT213B through the SMBUS pins on the EXP header. Virtual buttons on
STK's LCD screen indicate whether the capacitive sensing buttons on CPT213B
are pressed or released.

This demo also shows the run time config load feature of the CPT213B device.
When either of the two push buttons are pressed, different configurations are
loaded onto the CPT device. Pushing PB0 on the STK activates all the 13
capactive touch buttons on the CPT device while PB1 activates only the middle
row.

The following points are critical to note before running the example code.

1) The code assumed that you have a valid configuration profile loaded
   prior to executing the code. If that is not the case, first navigate
   to Xpress configurator and load a valid configuration there.
2) This code is written to function with a slave address of 0xF0. So, make
   sure the I2C slave address is set to 0xF0 in the Xpress configurator.
3) Since the code is time sensitive, make sure to release the push buttons
   as soon as they are pressed. If you press and hold the push buttons for a
   long time, the host device can go into a safe state.

----------------------------------------------------------------------------
How To Test: EFM8BB2 STK
----------------------------------------------------------------------------
1) Place the switch in "AEM" mode.
2) Attach CPT213B board to the STK through EXP headers
3) Connect the EFM8BB2 STK board to a PC using a mini USB cable.
4) Compile and download code to the EFM8BB2 STK board.
   In Simplicity Studio IDE, select Run -> Debug from the menu bar,
   click the Debug button in the quick menu, or press F11.
5) Run the code.
   In Simplicity Studio IDE, select Run -> Resume from the menu bar,
   click the Resume button in the quick menu, or press F8.
6) The demo program should start with a set of circles on display and by
   pressing/releasing capacitive buttons on CPT213B, the circles turns
   ON/OFF.
7) Press Push Button 1 and release it quickly. A different configuration is
   loaded onto the device and now only the middle row is active on the CPT
   board. Touching the top or bottom row will not activate the buttons on the
   LCD screen.
8) Press Push Button 0 and release it quickly. Now, all the 13 buttons are active
   and the same is reflected on the LCD screen

Target:         EFM8BB2
Tool chain:     Generic