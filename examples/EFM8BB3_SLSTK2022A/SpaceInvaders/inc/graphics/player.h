#define player_width 16
#define player_height 8
static SI_SEGMENT_VARIABLE(player_bits[], const uint8_t, SI_SEG_CODE) = {
  0x01, 0x00, 0x03, 0x80, 0x03, 0x80, 0x3F, 0xF8, 0x7F, 0xFC, 0x7F, 0xFC, 
  0x7F, 0xFC, 0x7F, 0xFC};
