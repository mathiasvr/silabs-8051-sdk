EFM8SB1 DataLogger Demo
========================

1. Introduction
This demo reads temperature and relative humidity samples from Si7021
periodically (1 sample per second by default), then write the measurement into
the last page of Flash, meanwhiel print them on LCD. Once the last page of Flash is full
(reaching to 85 log entries), this Flash page will be erased and filled with new samples.

Currently this demo supports logger and graph form display modes on LCD real
time. The logger mode also supports history mode controlled by the joystick,
it displays up to 16 log enteries on LCD for Logger mode.

--------------------------
LOG_START_ADDR  0x1E00
LOG_END_ADDR    0x1FFD
LOG_SIZE        510 Bytes
LOG_ENTRY_SIZE  6 Bytes
MAX_NUM_ENTRY   85 (LOG_SIZE / LOG_ENTRY_SIZE)
--------------------------

2. Hardware
EFM8SB10 STK Board

3. Usage
PB0 - Select Celsius, Fahrenheit or Relative Humidity display
PB1 - Select Logger or Graph mode
JoystickUp - Back button for Logger form
JoystickDown - Forward button of Logger form