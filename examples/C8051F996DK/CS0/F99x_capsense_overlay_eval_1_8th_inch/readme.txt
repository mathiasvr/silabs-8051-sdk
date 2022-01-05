Capacitive Sensing library example tuned for a 1/8 inch acrylic overlay

This example uses the capacitive sensing firmware library to detect
touches on the 2 sensors on the F990 target board.  The library
configuration files are set to use touch detection thresholds appropriate
for sensors with a 1/8 inch overlay.

When firmware detects a touch on C8 or C9, the DS4 LED lights up.  If a second
touch is detected, DS3 also lights up.

For more information on the capacitive sense library and 
Capacitive Sense Profiler, see AN0828: “Capacitive Sensing Library Overview”
and AN0829: “Capacitive Sensing Library Configuration Guide” by clicking the
Application Notes tile from the main Simplicity Studio launcher.