#define out_disable_width 32
#define out_disable_height 32
static SI_SEGMENT_VARIABLE(out_disable_bits[], const uint8_t, SI_SEG_CODE) = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 
  0x00, 0x3F, 0xFC, 0x00, 0x00, 0xFF, 0xFE, 0x00, 0x01, 0xFF, 0xFF, 0x00, 
  0x03, 0xFF, 0xFF, 0xC0, 0x07, 0xF8, 0x3F, 0xC0, 0x0F, 0xE0, 0x0F, 0xE0, 
  0x0F, 0xF0, 0x1F, 0xF0, 0x1F, 0xF8, 0x3F, 0xF0, 0x1F, 0x7C, 0x7D, 0xF0, 
  0x3F, 0x3E, 0xF9, 0xF8, 0x3E, 0x1F, 0xF0, 0xF8, 0x3E, 0x0F, 0xE0, 0xF8, 
  0x3E, 0x07, 0xC0, 0xF8, 0x3E, 0x0F, 0xE0, 0xF8, 0x3E, 0x1F, 0xF0, 0xF8, 
  0x3F, 0x3E, 0xF9, 0xF8, 0x1F, 0x7C, 0x7D, 0xF0, 0x1F, 0xF8, 0x3F, 0xF0, 
  0x1F, 0xF0, 0x1F, 0xF0, 0x0F, 0xE0, 0x0F, 0xE0, 0x07, 0xF8, 0x3F, 0xC0, 
  0x03, 0xFF, 0xFF, 0xC0, 0x03, 0xFF, 0xFF, 0x80, 0x00, 0xFF, 0xFE, 0x00, 
  0x00, 0x7F, 0xFC, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
