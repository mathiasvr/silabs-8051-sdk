

extern void initDisplay();
extern void printTx(uint8_t val);
extern void printRx(uint8_t val);
extern void refreshLcd(void);
extern SI_SEGMENT_VARIABLE(txCharReady, bool, SI_SEG_IDATA);
extern SI_SEGMENT_VARIABLE(txChar, uint8_t, SI_SEG_XDATA);

