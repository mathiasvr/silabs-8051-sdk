#define dpad_nw_width 16
#define dpad_nw_height 16
static SI_SEGMENT_VARIABLE(dpad_nw_bits[], const uint8_t, SI_SEG_CODE) = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0xE0, 0x00, 0xF0, 0x00, 0xB8, 
  0x00, 0x1C, 0x00, 0x0E, 0x00, 0x07, 0x00, 0x02};
