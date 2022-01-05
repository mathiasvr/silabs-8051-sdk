SB1 Capacitive Sense for power eval 20 ms sleep period

This example uses the capacitive sensing firmware library to detect
touches on the 3 sensors found on the SB1 evaluation board.  The library
configuration files are set go into a sleep mode with a 20 ms sample rate
when no touches are detected.

When firmware detects a touch on C1, C2, or C3, the LED lights up. 

For more information on the capacitive sense library and 
Capacitive Sense Profiler, see AN0828: “Capacitive Sensing Library Overview”
and AN0829: “Capacitive Sensing Library Configuration Guide” by clicking the
Application Notes tile from the main Simplicity Studio launcher.