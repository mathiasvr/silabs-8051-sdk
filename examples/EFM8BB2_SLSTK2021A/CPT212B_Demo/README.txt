CPT212B Demo
-------------

This demo demonstrates the capacitive button function of CPT212B fixed
function board. This demo runs on the EFM8BB2 STK, and communicates with
CPT212B through the SMBUS pins on the EXP header. Virtual buttons on
STK's LCD screen indicate whether the capacitive sensing buttons on CPT212B
are pressed or released.

This demo also shows the run time config load feature of the CPT212B device.
When either of the two push buttons are pressed, different configurations are
loaded onto the CPT device. Pushing PB0 on the STK uses a long duration of
the buzzer and uses a low frequency buzzer output while PB1 uses a long duration 
of the buzzer and uses a high frequency buzzer output.


The following points are critical to note before running the example code.

1) The code assumed that you have a valid configuration profile loaded
   prior to executing the code. If that is not the case, first navigate
   to Xpress configurator and load a valid configuration there.
2) This code is written to function with a slave address of 0xF0. So, make
   sure the I2C slave address is set to 0xF0 in the Xpress configurator.

----------------------------------------------------------------------------
How To Test: EFM8BB2 STK
----------------------------------------------------------------------------
1) Place the switch in "AEM" mode.
2) Attach CPT212B board to the STK through EXP headers
3) Connect the EFM8BB2 STK board to a PC using a mini USB cable.
4) Compile and download code to the EFM8BB2 STK board.
   In Simplicity Studio IDE, select Run -> Debug from the menu bar,
   click the Debug button in the quick menu, or press F11.
5) Run the code.
   In Simplicity Studio IDE, select Run -> Resume from the menu bar,
   click the Resume button in the quick menu, or press F8.
6) The demo program should start with a set of circles on display and by
   pressing/releasing capacitive buttons on CPT212B, the circles turns
   ON/OFF.
7) Press Push Button 1. A different configuration is
   loaded onto the device, and you should be able to hear the buzzer outputting
   a long duration buzzer at a low pitch frequency.
8) Press Push Button 0. Now, the buzzer duration for
   a touch will be long but with a high pitch frequency.

Target:         EFM8BB2
Tool chain:     Generic