/*
 * pca_config.h
 */

#ifndef PCA_CONFIG_H_
#define PCA_CONFIG_H_

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define PCA_CLK      10000  // PCA clock frequency in Hz derived from LFOSC/8
#define INT_PER      2      // Interrupt period in seconds to sample ADC and output values
 // PCA_CLK cycles per interrupt
#define PCA_TIMEOUT  (PCA_CLK * INT_PER)


#endif /* PCA_CONFIG_H_ */
