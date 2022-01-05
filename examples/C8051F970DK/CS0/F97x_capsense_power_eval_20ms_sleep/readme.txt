Capacitive Sensing library example tuned to sleep with a 20 ms wake period

This example uses the capacitive sensing firmware library to detect
touches on the 24 sensors on the F970 target board.  The library
configuration files are set to use touch detection thresholds appropriate
for sensors without an overlay.

When firmware detects a touch on any of the sensors in the touch pad space
by lighting up LED05.  If firmware detects a second touch in the touch pad
area, LED15 lights up.  If the user touches the slider, the LEDs beside the 
slider will show the output from the centroid algorithm.

When no touches are detected, the firmware puts the MCU into a sleep mode
with 20 ms scanning in sleep mode.

For more information on the capacitive sense library and 
Capacitive Sense Profiler, see AN0828: “Capacitive Sensing Library Overview”
and AN0829: “Capacitive Sensing Library Configuration Guide” by clicking the
Application Notes tile from the main Simplicity Studio launcher.