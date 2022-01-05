#ifndef __SILICON_LABS_EFM8_CONFIG_H
#define __SILICON_LABS_EFM8_CONFIG_H

// Improve performance/save space by not automatically setting/clearing the
// SFR page every time we touch UART1.  For now, we will let the peripheral
// driver handle switching between pages.
#define EFM8PDL_UART1_AUTO_PAGE 1

#define EFM8PDL_UART0_TX_BUFTYPE SI_SEG_IDATA
#define EFM8PDL_UART0_RX_BUFTYPE SI_SEG_IDATA

#endif // __SILICON_LABS_EFM8_CONFIG_H
