Capacitive Sensing library example tuned for no overlay

This example uses the capacitive sensing firmware library to detect
touches on the 2 sensors found on the F996 target board.  The library
configuration files are set to use touch detection thresholds appropriate
for sensors with no overlay.

When firmware detects a touch on C8 or C9, DS4 lights up.  When firmware 
detects a second touch, DS3 lights up.

For more information on the capacitive sense library and 
Capacitive Sense Profiler, see AN0828: “Capacitive Sensing Library Overview”
and AN0829: “Capacitive Sensing Library Configuration Guide” by clicking the
Application Notes tile from the main Simplicity Studio launcher.