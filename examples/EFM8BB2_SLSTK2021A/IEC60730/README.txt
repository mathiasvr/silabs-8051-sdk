IEC60730 Demo
========================

 Program Description:
 This demo for IEC60730 demonstrates how the various modules work to
 provide functional safety for an OEM. It requires a jumper across UART0_RX
 and UART0_TX in order for the External Communications example to work
 correctly. The demo code continually runs iec60730_Bist(), with periodic
 interrupts occurring from two timers clocked from different oscillators. 
 Every 100ms a timer initiates a UART packet. Every 1s a timer calls
 iec60730_programmeCounterTest() in the IEC60730 library to verify
 that all IEC60730 safety tests have completed. If the jumper is removed, 
 the Communications example will no longer set IEC60730_COMMS_COMPLETE.
 This will cause the iec60730_programmeCounterTest() to fail
 and the STK will enter iec60730_SafeState(). Halting the processor will show 
 the device has entered iec60730_SafeState().