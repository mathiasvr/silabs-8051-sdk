#define dpad_bold_n_width 16
#define dpad_bold_n_height 16
static SI_SEGMENT_VARIABLE(dpad_bold_n_bits[], const uint8_t, SI_SEG_CODE) = {
  0x01, 0x80, 0x03, 0xC0, 0x07, 0xE0, 0x0F, 0xF0, 0x1F, 0xF8, 0x3F, 0xFC, 
  0x7F, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0xE0, 0x07, 0xE0, 0x07, 0xE0, 
  0x07, 0xE0, 0x07, 0xE0, 0x07, 0xE0, 0x07, 0xE0};