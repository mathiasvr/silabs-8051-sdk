#define heart_full_width 24
#define heart_full_height 24
static SI_SEGMENT_VARIABLE(heart_full_bits[], const uint8_t, SI_SEG_CODE) = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x01, 0xC0, 0x1F, 0xC7, 0xF0, 
  0x3F, 0xEF, 0xF0, 0x3A, 0xFF, 0xB8, 0x77, 0xFF, 0xDC, 0x7F, 0xBF, 0xFC, 
  0x7F, 0xF7, 0xFC, 0x7F, 0xFF, 0xFC, 0x37, 0xFF, 0xF8, 0x77, 0xFF, 0xD8, 
  0x3F, 0xFF, 0xB8, 0x3B, 0xFF, 0xF0, 0x1D, 0xFF, 0x70, 0x1F, 0xFF, 0xE0, 
  0x0F, 0xFF, 0xC0, 0x07, 0xBF, 0xC0, 0x03, 0xC7, 0x80, 0x00, 0xFE, 0x00, 
  0x00, 0xFC, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
