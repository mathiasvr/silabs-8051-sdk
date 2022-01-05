Watchdog Example

This example program shows how to use the Watchdog functionality and
detect whether the MCU has detected a reset caused by the Watchdog.
When the MCU is running, LED05 will blink at 5Hz, but if a Watchdog
reset is detected, LED05 will blink at 20Hz.

The user can induce a Watchdog reset by pressing and holding BTN03,
which will prevent the program from feeding the Watchdog.