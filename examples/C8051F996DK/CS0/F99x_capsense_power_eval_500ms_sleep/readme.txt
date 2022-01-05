Capacitive Sensing library example tuned for 500 ms low power scanning

This example uses the capacitive sensing firmware library to detect
touches on the 2 sensors found on the F990 target board.  The library
puts the device in a low power mode if no touches appear on the sensors
after 1 second of scanning.  When in a low power mode, the device
periodically wakes at a rate of 500 ms to check for touches.
Note that when in low power mode, the device will not output data
to the Profiler tool.

For more information on the capacitive sense library and 
Capacitive Sense Profiler, see AN0828: “Capacitive Sensing Library Overview”
and AN0829: “Capacitive Sensing Library Configuration Guide” by clicking the
Application Notes tile from the main Simplicity Studio launcher.