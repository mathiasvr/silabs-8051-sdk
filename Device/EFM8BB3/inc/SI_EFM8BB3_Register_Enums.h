//------------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//------------------------------------------------------------------------------
//Supported Devices:
//  EFM8BB31F16A_4QFN24
//  EFM8BB31F16A_5QFN32
//  EFM8BB31F16G_QFN24
//  EFM8BB31F16G_QFN32
//  EFM8BB31F16G_QFP32
//  EFM8BB31F16G_QSOP24
//  EFM8BB31F16I_4QFN24
//  EFM8BB31F16I_5QFN32
//  EFM8BB31F16I_QFN24
//  EFM8BB31F16I_QFN32
//  EFM8BB31F16I_QFP32
//  EFM8BB31F16I_QSOP24
//  EFM8BB31F32A_4QFN24
//  EFM8BB31F32A_5QFN32
//  EFM8BB31F32G_QFN24
//  EFM8BB31F32G_QFN32
//  EFM8BB31F32G_QFP32
//  EFM8BB31F32G_QSOP24
//  EFM8BB31F32I_4QFN24
//  EFM8BB31F32I_5QFN32
//  EFM8BB31F32I_QFN24
//  EFM8BB31F32I_QFN32
//  EFM8BB31F32I_QFP32
//  EFM8BB31F32I_QSOP24
//  EFM8BB31F64A_4QFN24
//  EFM8BB31F64A_5QFN32
//  EFM8BB31F64G_QFN24
//  EFM8BB31F64G_QFN32
//  EFM8BB31F64G_QFP32
//  EFM8BB31F64G_QSOP24
//  EFM8BB31F64I_4QFN24
//  EFM8BB31F64I_5QFN32
//  EFM8BB31F64I_QFN24
//  EFM8BB31F64I_QFN32
//  EFM8BB31F64I_QFP32
//  EFM8BB31F64I_QSOP24

#ifndef SI_EFM8BB3_REGISTER_ENUMS_H
#define SI_EFM8BB3_REGISTER_ENUMS_H

//Standard device includes
#include "SI_EFM8BB3_Defs.h"
//------------------------------------------------------------------------------
// ADC0ASAH Enums (ADC0 Autoscan Start Address High Byte @ 0xB6)
//------------------------------------------------------------------------------
#define ADC0ASAH_STADDRH__FMASK 0x0F ///< Start Address High
#define ADC0ASAH_STADDRH__SHIFT 0x00 ///< Start Address High
                                                            
//------------------------------------------------------------------------------
// ADC0ASAL Enums (ADC0 Autoscan Start Address Low Byte @ 0xB5)
//------------------------------------------------------------------------------
#define ADC0ASAL_ENDIAN__BMASK         0x01 ///< Endianness Control                             
#define ADC0ASAL_ENDIAN__SHIFT         0x00 ///< Endianness Control                             
#define ADC0ASAL_ENDIAN__BIG_ENDIAN    0x00 ///< ADC results in XRAM are stored in big-endian   
                                            ///< order. This will result in the most significant
                                            ///< byte stored in the even-numbered address.      
#define ADC0ASAL_ENDIAN__LITTLE_ENDIAN 0x01 ///< ADC results in XRAM are stored in little-endian
                                            ///< order. This will result in the most significant
                                            ///< byte stored in the odd-numbered address.       
                                                                                                
#define ADC0ASAL_STADDRL__FMASK        0xFE ///< Start Address Low                              
#define ADC0ASAL_STADDRL__SHIFT        0x01 ///< Start Address Low                              
                                                                                                
//------------------------------------------------------------------------------
// ADC0ASCF Enums (ADC0 Autoscan Configuration @ 0xA1)
//------------------------------------------------------------------------------
#define ADC0ASCF_NASCH__FMASK            0x03 ///< Number of Autoscan Channels                      
#define ADC0ASCF_NASCH__SHIFT            0x00 ///< Number of Autoscan Channels                      
#define ADC0ASCF_NASCH__ONE              0x00 ///< Autoscan will only use the ADC0MX setting        
                                              ///< directly.                                        
#define ADC0ASCF_NASCH__TWO              0x01 ///< Autoscan will alternate between ADC0MX and       
                                              ///< ADC0MX+1.                                        
#define ADC0ASCF_NASCH__THREE            0x02 ///< Autoscan will cycle through ADC0MX, ADC0MX+1 and 
                                              ///< ADC0MX+2.                                        
#define ADC0ASCF_NASCH__FOUR             0x03 ///< Autoscan will cycle through ADC0MX, ADC0MX+1,    
                                              ///< ADC0MX+2, and ADC0MX+3.                          
                                                                                                    
#define ADC0ASCF_ASACT__BMASK            0x20 ///< Autoscan Active                                  
#define ADC0ASCF_ASACT__SHIFT            0x05 ///< Autoscan Active                                  
#define ADC0ASCF_ASACT__NOT_ACTIVE       0x00 ///< No scan is in progress.                          
#define ADC0ASCF_ASACT__ACTIVE           0x20 ///< A scan is in progress.                           
                                                                                                    
#define ADC0ASCF_STEN__BMASK             0x40 ///< Autoscan Single Trigger Enable                   
#define ADC0ASCF_STEN__SHIFT             0x06 ///< Autoscan Single Trigger Enable                   
#define ADC0ASCF_STEN__MULTIPLE_TRIGGERS 0x00 ///< Each conversion in a scan requires a new         
                                              ///< conversion trigger from the selected conversion  
                                              ///< trigger source.                                  
#define ADC0ASCF_STEN__SINGLE_TRIGGER    0x40 ///< The selected conversion trigger source will begin
                                              ///< each scan cycle. All conversions within a scan   
                                              ///< cycle are performed automatically when the       
                                              ///< previous conversion is complete.                 
                                                                                                    
#define ADC0ASCF_ASEN__BMASK             0x80 ///< Autoscan Enable                                  
#define ADC0ASCF_ASEN__SHIFT             0x07 ///< Autoscan Enable                                  
#define ADC0ASCF_ASEN__HALT_SCAN         0x00 ///< Clearing to 0 will halt scan operations once any 
                                              ///< pending scan is complete.                        
#define ADC0ASCF_ASEN__START_SCAN        0x80 ///< Setting to 1 will initialize a scan operation. If
                                              ///< set to 1 at the end of a scan, a new scan will   
                                              ///< begin.                                           
                                                                                                    
//------------------------------------------------------------------------------
// ADC0ASCT Enums (ADC0 Autoscan Output Count @ 0xC7)
//------------------------------------------------------------------------------
#define ADC0ASCT_ASCNT__FMASK 0x3F ///< Autoscan Output Count
#define ADC0ASCT_ASCNT__SHIFT 0x00 ///< Autoscan Output Count
                                                             
//------------------------------------------------------------------------------
// ADC0CF0 Enums (ADC0 Configuration @ 0xBC)
//------------------------------------------------------------------------------
#define ADC0CF0_ADCLKSEL__BMASK  0x04 ///< ADC Clock Select 
#define ADC0CF0_ADCLKSEL__SHIFT  0x02 ///< ADC Clock Select 
#define ADC0CF0_ADCLKSEL__SYSCLK 0x00 ///< ADCCLK = SYSCLK. 
#define ADC0CF0_ADCLKSEL__HFOSC0 0x04 ///< ADCCLK = HFOSC0. 
                                                            
#define ADC0CF0_ADSC__FMASK      0xF8 ///< SAR Clock Divider
#define ADC0CF0_ADSC__SHIFT      0x03 ///< SAR Clock Divider
                                                            
//------------------------------------------------------------------------------
// ADC0CF1 Enums (ADC0 Configuration @ 0xB9)
//------------------------------------------------------------------------------
#define ADC0CF1_ADTK__FMASK 0x3F ///< Conversion Tracking Time
#define ADC0CF1_ADTK__SHIFT 0x00 ///< Conversion Tracking Time
                                                              
//------------------------------------------------------------------------------
// ADC0CF2 Enums (ADC0 Power Control @ 0xDF)
//------------------------------------------------------------------------------
#define ADC0CF2_ADPWR__FMASK         0x1F ///< Power Up Delay Time                               
#define ADC0CF2_ADPWR__SHIFT         0x00 ///< Power Up Delay Time                               
                                                                                                 
#define ADC0CF2_REFSL__FMASK         0x60 ///< Voltage Reference Select                          
#define ADC0CF2_REFSL__SHIFT         0x05 ///< Voltage Reference Select                          
#define ADC0CF2_REFSL__VREF_PIN      0x00 ///< The ADC0 voltage reference is the VREF pin        
                                          ///< (external or from the on-chip reference).         
#define ADC0CF2_REFSL__VDD_PIN       0x20 ///< The ADC0 voltage reference is the VDD pin.        
#define ADC0CF2_REFSL__INTERNAL_LDO  0x40 ///< The ADC0 voltage reference is the internal 1.8 V  
                                          ///< digital supply voltage.                           
#define ADC0CF2_REFSL__INTERNAL_VREF 0x60 ///< The ADC0 voltage reference is the internal voltage
                                          ///< reference.                                        
                                                                                                 
#define ADC0CF2_GNDSL__BMASK         0x80 ///< Analog Ground Reference                           
#define ADC0CF2_GNDSL__SHIFT         0x07 ///< Analog Ground Reference                           
#define ADC0CF2_GNDSL__GND_PIN       0x00 ///< The ADC0 ground reference is the GND pin.         
#define ADC0CF2_GNDSL__AGND_PIN      0x80 ///< The ADC0 ground reference is the AGND pin.        
                                                                                                 
//------------------------------------------------------------------------------
// ADC0CN0 Enums (ADC0 Control 0 @ 0xE8)
//------------------------------------------------------------------------------
#define ADC0CN0_TEMPE__BMASK         0x01 ///< Temperature Sensor Enable                         
#define ADC0CN0_TEMPE__SHIFT         0x00 ///< Temperature Sensor Enable                         
#define ADC0CN0_TEMPE__TEMP_DISABLED 0x00 ///< Disable the Temperature Sensor.                   
#define ADC0CN0_TEMPE__TEMP_ENABLED  0x01 ///< Enable the Temperature Sensor.                    
                                                                                                 
#define ADC0CN0_ADGN__FMASK          0x06 ///< Gain Control                                      
#define ADC0CN0_ADGN__SHIFT          0x01 ///< Gain Control                                      
#define ADC0CN0_ADGN__GAIN_1         0x00 ///< The on-chip PGA gain is 1.                        
#define ADC0CN0_ADGN__GAIN_0P75      0x02 ///< The on-chip PGA gain is 0.75.                     
#define ADC0CN0_ADGN__GAIN_0P5       0x04 ///< The on-chip PGA gain is 0.5.                      
#define ADC0CN0_ADGN__GAIN_0P25      0x06 ///< The on-chip PGA gain is 0.25.                     
                                                                                                 
#define ADC0CN0_ADWINT__BMASK        0x08 ///< Window Compare Interrupt Flag                     
#define ADC0CN0_ADWINT__SHIFT        0x03 ///< Window Compare Interrupt Flag                     
#define ADC0CN0_ADWINT__NOT_SET      0x00 ///< An ADC window compare event did not occur.        
#define ADC0CN0_ADWINT__SET          0x08 ///< An ADC window compare event occurred.             
                                                                                                 
#define ADC0CN0_ADBUSY__BMASK        0x10 ///< ADC Busy                                          
#define ADC0CN0_ADBUSY__SHIFT        0x04 ///< ADC Busy                                          
#define ADC0CN0_ADBUSY__NOT_SET      0x00 ///< An ADC0 conversion is not currently in progress.  
#define ADC0CN0_ADBUSY__SET          0x10 ///< ADC0 conversion is in progress or start an ADC0   
                                          ///< conversion.                                       
                                                                                                 
#define ADC0CN0_ADINT__BMASK         0x20 ///< Conversion Complete Interrupt Flag                
#define ADC0CN0_ADINT__SHIFT         0x05 ///< Conversion Complete Interrupt Flag                
#define ADC0CN0_ADINT__NOT_SET       0x00 ///< ADC0 has not completed a conversion since the last
                                          ///< time ADINT was cleared.                           
#define ADC0CN0_ADINT__SET           0x20 ///< ADC0 completed a data conversion.                 
                                                                                                 
#define ADC0CN0_IPOEN__BMASK         0x40 ///< Idle Powered-off Enable                           
#define ADC0CN0_IPOEN__SHIFT         0x06 ///< Idle Powered-off Enable                           
#define ADC0CN0_IPOEN__ALWAYS_ON     0x00 ///< Keep ADC powered on when ADEN is 1.               
#define ADC0CN0_IPOEN__POWER_DOWN    0x40 ///< Power down when ADC is idle (not converting).     
                                                                                                 
#define ADC0CN0_ADEN__BMASK          0x80 ///< ADC Enable                                        
#define ADC0CN0_ADEN__SHIFT          0x07 ///< ADC Enable                                        
#define ADC0CN0_ADEN__DISABLED       0x00 ///< Disable ADC0 (low-power shutdown).                
#define ADC0CN0_ADEN__ENABLED        0x80 ///< Enable ADC0 (active and ready for data            
                                          ///< conversions).                                     
                                                                                                 
//------------------------------------------------------------------------------
// ADC0CN1 Enums (ADC0 Control 1 @ 0xB2)
//------------------------------------------------------------------------------
#define ADC0CN1_ADRPT__FMASK           0x07 ///< Repeat Count                             
#define ADC0CN1_ADRPT__SHIFT           0x00 ///< Repeat Count                             
#define ADC0CN1_ADRPT__ACC_1           0x00 ///< Perform and Accumulate 1 conversion.     
#define ADC0CN1_ADRPT__ACC_4           0x01 ///< Perform and Accumulate 4 conversions.    
#define ADC0CN1_ADRPT__ACC_8           0x02 ///< Perform and Accumulate 8 conversions.    
#define ADC0CN1_ADRPT__ACC_16          0x03 ///< Perform and Accumulate 16 conversions.   
#define ADC0CN1_ADRPT__ACC_32          0x04 ///< Perform and Accumulate 32 conversions.   
                                                                                          
#define ADC0CN1_ADSJST__FMASK          0x38 ///< Accumulator Shift and Justify            
#define ADC0CN1_ADSJST__SHIFT          0x03 ///< Accumulator Shift and Justify            
#define ADC0CN1_ADSJST__RIGHT_NO_SHIFT 0x00 ///< Right justified. No shifting applied.    
#define ADC0CN1_ADSJST__RIGHT_SHIFT_1  0x08 ///< Right justified. Shifted right by 1 bit. 
#define ADC0CN1_ADSJST__RIGHT_SHIFT_2  0x10 ///< Right justified. Shifted right by 2 bits.
#define ADC0CN1_ADSJST__RIGHT_SHIFT_3  0x18 ///< Right justified. Shifted right by 3 bits.
                                                                                          
#define ADC0CN1_ADBITS__FMASK          0xC0 ///< Resolution Control                       
#define ADC0CN1_ADBITS__SHIFT          0x06 ///< Resolution Control                       
#define ADC0CN1_ADBITS__10_BIT         0x00 ///< ADC0 operates in 10-bit mode.            
#define ADC0CN1_ADBITS__12_BIT         0x40 ///< ADC0 operates in 12-bit mode.            
                                                                                          
//------------------------------------------------------------------------------
// ADC0CN2 Enums (ADC0 Control 2 @ 0xB3)
//------------------------------------------------------------------------------
#define ADC0CN2_ADCM__FMASK         0x0F ///< Start of Conversion Mode Select                   
#define ADC0CN2_ADCM__SHIFT         0x00 ///< Start of Conversion Mode Select                   
#define ADC0CN2_ADCM__ADBUSY        0x00 ///< ADC0 conversion initiated on write of 1 to ADBUSY.
#define ADC0CN2_ADCM__TIMER0        0x01 ///< ADC0 conversion initiated on overflow of Timer 0. 
#define ADC0CN2_ADCM__TIMER2        0x02 ///< ADC0 conversion initiated on overflow of Timer 2. 
#define ADC0CN2_ADCM__TIMER3        0x03 ///< ADC0 conversion initiated on overflow of Timer 3. 
#define ADC0CN2_ADCM__CNVSTR        0x04 ///< ADC0 conversion initiated on rising edge of       
                                         ///< CNVSTR.                                           
#define ADC0CN2_ADCM__CEX5          0x05 ///< ADC0 conversion initiated on rising edge of CEX5. 
#define ADC0CN2_ADCM__TIMER4        0x06 ///< ADC0 conversion initiated on overflow of Timer 4. 
#define ADC0CN2_ADCM__TIMER5        0x07 ///< ADC0 conversion initiated on overflow of Timer 5. 
#define ADC0CN2_ADCM__CLU0          0x08 ///< ADC0 conversion initiated on CLU0 Output.         
#define ADC0CN2_ADCM__CLU1          0x09 ///< ADC0 conversion initiated on CLU1 Output.         
#define ADC0CN2_ADCM__CLU2          0x0A ///< ADC0 conversion initiated on CLU2 Output.         
#define ADC0CN2_ADCM__CLU3          0x0B ///< ADC0 conversion initiated on CLU3 Output.         
                                                                                                
#define ADC0CN2_PACEN__BMASK        0x80 ///< Preserve Accumulator Enable                       
#define ADC0CN2_PACEN__SHIFT        0x07 ///< Preserve Accumulator Enable                       
#define ADC0CN2_PACEN__PAC_DISABLED 0x00 ///< The ADC accumulator is over-written with the      
                                         ///< results of any conversion (or set of conversions  
                                         ///< as specified by ADRPT).                           
#define ADC0CN2_PACEN__PAC_ENABLED  0x80 ///< The ADC accumulator always adds new results to the
                                         ///< existing output. The accumulator is never cleared 
                                         ///< in this mode.                                     
                                                                                                
//------------------------------------------------------------------------------
// ADC0GTH Enums (ADC0 Greater-Than High Byte @ 0xC4)
//------------------------------------------------------------------------------
#define ADC0GTH_ADC0GTH__FMASK 0xFF ///< Greater-Than High Byte
#define ADC0GTH_ADC0GTH__SHIFT 0x00 ///< Greater-Than High Byte
                                                               
//------------------------------------------------------------------------------
// ADC0GTL Enums (ADC0 Greater-Than Low Byte @ 0xC3)
//------------------------------------------------------------------------------
#define ADC0GTL_ADC0GTL__FMASK 0xFF ///< Greater-Than Low Byte
#define ADC0GTL_ADC0GTL__SHIFT 0x00 ///< Greater-Than Low Byte
                                                              
//------------------------------------------------------------------------------
// ADC0H Enums (ADC0 Data Word High Byte @ 0xBE)
//------------------------------------------------------------------------------
#define ADC0H_ADC0H__FMASK 0xFF ///< Data Word High Byte
#define ADC0H_ADC0H__SHIFT 0x00 ///< Data Word High Byte
                                                        
//------------------------------------------------------------------------------
// ADC0L Enums (ADC0 Data Word Low Byte @ 0xBD)
//------------------------------------------------------------------------------
#define ADC0L_ADC0L__FMASK 0xFF ///< Data Word Low Byte
#define ADC0L_ADC0L__SHIFT 0x00 ///< Data Word Low Byte
                                                       
//------------------------------------------------------------------------------
// ADC0LTH Enums (ADC0 Less-Than High Byte @ 0xC6)
//------------------------------------------------------------------------------
#define ADC0LTH_ADC0LTH__FMASK 0xFF ///< Less-Than High Byte
#define ADC0LTH_ADC0LTH__SHIFT 0x00 ///< Less-Than High Byte
                                                            
//------------------------------------------------------------------------------
// ADC0LTL Enums (ADC0 Less-Than Low Byte @ 0xC5)
//------------------------------------------------------------------------------
#define ADC0LTL_ADC0LTL__FMASK 0xFF ///< Less-Than Low Byte
#define ADC0LTL_ADC0LTL__SHIFT 0x00 ///< Less-Than Low Byte
                                                           
//------------------------------------------------------------------------------
// ADC0MX Enums (ADC0 Multiplexer Selection @ 0xBB)
//------------------------------------------------------------------------------
#define ADC0MX_ADC0MX__FMASK   0x1F ///< AMUX0 Positive Input Selection
#define ADC0MX_ADC0MX__SHIFT   0x00 ///< AMUX0 Positive Input Selection
#define ADC0MX_ADC0MX__ADC0P0  0x00 ///< Select ADC0.0.                
#define ADC0MX_ADC0MX__ADC0P1  0x01 ///< Select ADC0.1.                
#define ADC0MX_ADC0MX__ADC0P2  0x02 ///< Select ADC0.2.                
#define ADC0MX_ADC0MX__ADC0P3  0x03 ///< Select ADC0.3.                
#define ADC0MX_ADC0MX__ADC0P4  0x04 ///< Select ADC0.4.                
#define ADC0MX_ADC0MX__ADC0P5  0x05 ///< Select ADC0.5.                
#define ADC0MX_ADC0MX__ADC0P6  0x06 ///< Select ADC0.6.                
#define ADC0MX_ADC0MX__ADC0P7  0x07 ///< Select ADC0.7.                
#define ADC0MX_ADC0MX__ADC0P8  0x08 ///< Select ADC0.8.                
#define ADC0MX_ADC0MX__ADC0P9  0x09 ///< Select ADC0.9.                
#define ADC0MX_ADC0MX__ADC0P10 0x0A ///< Select ADC0.10.               
#define ADC0MX_ADC0MX__ADC0P11 0x0B ///< Select ADC0.11.               
#define ADC0MX_ADC0MX__ADC0P12 0x0C ///< Select ADC0.12.               
#define ADC0MX_ADC0MX__ADC0P13 0x0D ///< Select ADC0.13.               
#define ADC0MX_ADC0MX__ADC0P14 0x0E ///< Select ADC0.14.               
#define ADC0MX_ADC0MX__ADC0P15 0x0F ///< Select ADC0.15.               
#define ADC0MX_ADC0MX__ADC0P16 0x10 ///< Select ADC0.16.               
#define ADC0MX_ADC0MX__ADC0P17 0x11 ///< Select ADC0.17.               
#define ADC0MX_ADC0MX__ADC0P18 0x12 ///< Select ADC0.18.               
#define ADC0MX_ADC0MX__ADC0P19 0x13 ///< Select ADC0.19.               
#define ADC0MX_ADC0MX__TEMP    0x14 ///< Select ADC0.20                
#define ADC0MX_ADC0MX__LDO_OUT 0x15 ///< Select ADC0.21                
#define ADC0MX_ADC0MX__VDD     0x16 ///< Select ADC0.22                
#define ADC0MX_ADC0MX__GND     0x17 ///< Select ADC0.23                
#define ADC0MX_ADC0MX__NONE    0x1F ///< No input selected.            
                                                                       
//------------------------------------------------------------------------------
// ACC Enums (Accumulator @ 0xE0)
//------------------------------------------------------------------------------
#define ACC_ACC__FMASK 0xFF ///< Accumulator
#define ACC_ACC__SHIFT 0x00 ///< Accumulator
                                            
//------------------------------------------------------------------------------
// B Enums (B Register @ 0xF0)
//------------------------------------------------------------------------------
#define B_B__FMASK 0xFF ///< B Register
#define B_B__SHIFT 0x00 ///< B Register
                                       
//------------------------------------------------------------------------------
// DPH Enums (Data Pointer High @ 0x83)
//------------------------------------------------------------------------------
#define DPH_DPH__FMASK 0xFF ///< Data Pointer High
#define DPH_DPH__SHIFT 0x00 ///< Data Pointer High
                                                  
//------------------------------------------------------------------------------
// DPL Enums (Data Pointer Low @ 0x82)
//------------------------------------------------------------------------------
#define DPL_DPL__FMASK 0xFF ///< Data Pointer Low
#define DPL_DPL__SHIFT 0x00 ///< Data Pointer Low
                                                 
//------------------------------------------------------------------------------
// PFE0CN Enums (Prefetch Engine Control @ 0xC1)
//------------------------------------------------------------------------------
#define PFE0CN_FLRT__FMASK               0x30 ///< Flash Read Timing
#define PFE0CN_FLRT__SHIFT               0x04 ///< Flash Read Timing
#define PFE0CN_FLRT__SYSCLK_BELOW_25_MHZ 0x00 ///< SYSCLK < 25 MHz. 
#define PFE0CN_FLRT__SYSCLK_BELOW_50_MHZ 0x10 ///< SYSCLK < 50 MHz. 
                                                                    
//------------------------------------------------------------------------------
// PSW Enums (Program Status Word @ 0xD0)
//------------------------------------------------------------------------------
#define PSW_PARITY__BMASK   0x01 ///< Parity Flag                                       
#define PSW_PARITY__SHIFT   0x00 ///< Parity Flag                                       
#define PSW_PARITY__NOT_SET 0x00 ///< The sum of the 8 bits in the accumulator is even. 
#define PSW_PARITY__SET     0x01 ///< The sum of the 8 bits in the accumulator is odd.  
                                                                                        
#define PSW_F1__BMASK       0x02 ///< User Flag 1                                       
#define PSW_F1__SHIFT       0x01 ///< User Flag 1                                       
#define PSW_F1__NOT_SET     0x00 ///< Flag is not set.                                  
#define PSW_F1__SET         0x02 ///< Flag is set.                                      
                                                                                        
#define PSW_OV__BMASK       0x04 ///< Overflow Flag                                     
#define PSW_OV__SHIFT       0x02 ///< Overflow Flag                                     
#define PSW_OV__NOT_SET     0x00 ///< An overflow did not occur.                        
#define PSW_OV__SET         0x04 ///< An overflow occurred.                             
                                                                                        
#define PSW_RS__FMASK       0x18 ///< Register Bank Select                              
#define PSW_RS__SHIFT       0x03 ///< Register Bank Select                              
#define PSW_RS__BANK0       0x00 ///< Bank 0, Addresses 0x00-0x07                       
#define PSW_RS__BANK1       0x08 ///< Bank 1, Addresses 0x08-0x0F                       
#define PSW_RS__BANK2       0x10 ///< Bank 2, Addresses 0x10-0x17                       
#define PSW_RS__BANK3       0x18 ///< Bank 3, Addresses 0x18-0x1F                       
                                                                                        
#define PSW_F0__BMASK       0x20 ///< User Flag 0                                       
#define PSW_F0__SHIFT       0x05 ///< User Flag 0                                       
#define PSW_F0__NOT_SET     0x00 ///< Flag is not set.                                  
#define PSW_F0__SET         0x20 ///< Flag is set.                                      
                                                                                        
#define PSW_AC__BMASK       0x40 ///< Auxiliary Carry Flag                              
#define PSW_AC__SHIFT       0x06 ///< Auxiliary Carry Flag                              
#define PSW_AC__NOT_SET     0x00 ///< A carry into (addition) or borrow from            
                                 ///< (subtraction) the high order nibble did not occur.
#define PSW_AC__SET         0x40 ///< A carry into (addition) or borrow from            
                                 ///< (subtraction) the high order nibble occurred.     
                                                                                        
#define PSW_CY__BMASK       0x80 ///< Carry Flag                                        
#define PSW_CY__SHIFT       0x07 ///< Carry Flag                                        
#define PSW_CY__NOT_SET     0x00 ///< A carry (addition) or borrow (subtraction) did not
                                 ///< occur.                                            
#define PSW_CY__SET         0x80 ///< A carry (addition) or borrow (subtraction)        
                                 ///< occurred.                                         
                                                                                        
//------------------------------------------------------------------------------
// SP Enums (Stack Pointer @ 0x81)
//------------------------------------------------------------------------------
#define SP_SP__FMASK 0xFF ///< Stack Pointer
#define SP_SP__SHIFT 0x00 ///< Stack Pointer
                                            
//------------------------------------------------------------------------------
// CLKSEL Enums (Clock Select @ 0xA9)
//------------------------------------------------------------------------------
#define CLKSEL_CLKSL__FMASK           0x07 ///< Clock Source Select                             
#define CLKSEL_CLKSL__SHIFT           0x00 ///< Clock Source Select                             
#define CLKSEL_CLKSL__HFOSC0          0x00 ///< Clock derived from the Internal High Frequency  
                                           ///< Oscillator 0.                                   
#define CLKSEL_CLKSL__EXTOSC          0x01 ///< Clock derived from the External Oscillator      
                                           ///< circuit.                                        
#define CLKSEL_CLKSL__LFOSC           0x02 ///< Clock derived from the Internal Low-Frequency   
                                           ///< Oscillator.                                     
#define CLKSEL_CLKSL__HFOSC1          0x03 ///< Clock derived from the Internal High Frequency  
                                           ///< Oscillator 1.                                   
#define CLKSEL_CLKSL__HFOSC0_DIV_1P5  0x04 ///< Clock derived from the Internal High Frequency  
                                           ///< Oscillator 0, pre-scaled by 1.5.                
#define CLKSEL_CLKSL__HFOSC1_DIV_1P5  0x07 ///< Clock derived from the Internal High Frequency  
                                           ///< Oscillator 1, pre-scaled by 1.5.                
                                                                                                
#define CLKSEL_CLKDIV__FMASK          0x70 ///< Clock Source Divider                            
#define CLKSEL_CLKDIV__SHIFT          0x04 ///< Clock Source Divider                            
#define CLKSEL_CLKDIV__SYSCLK_DIV_1   0x00 ///< SYSCLK is equal to selected clock source divided
                                           ///< by 1.                                           
#define CLKSEL_CLKDIV__SYSCLK_DIV_2   0x10 ///< SYSCLK is equal to selected clock source divided
                                           ///< by 2.                                           
#define CLKSEL_CLKDIV__SYSCLK_DIV_4   0x20 ///< SYSCLK is equal to selected clock source divided
                                           ///< by 4.                                           
#define CLKSEL_CLKDIV__SYSCLK_DIV_8   0x30 ///< SYSCLK is equal to selected clock source divided
                                           ///< by 8.                                           
#define CLKSEL_CLKDIV__SYSCLK_DIV_16  0x40 ///< SYSCLK is equal to selected clock source divided
                                           ///< by 16.                                          
#define CLKSEL_CLKDIV__SYSCLK_DIV_32  0x50 ///< SYSCLK is equal to selected clock source divided
                                           ///< by 32.                                          
#define CLKSEL_CLKDIV__SYSCLK_DIV_64  0x60 ///< SYSCLK is equal to selected clock source divided
                                           ///< by 64.                                          
#define CLKSEL_CLKDIV__SYSCLK_DIV_128 0x70 ///< SYSCLK is equal to selected clock source divided
                                           ///< by 128.                                         
                                                                                                
#define CLKSEL_DIVRDY__BMASK          0x80 ///< Clock Divider Ready                             
#define CLKSEL_DIVRDY__SHIFT          0x07 ///< Clock Divider Ready                             
#define CLKSEL_DIVRDY__NOT_READY      0x00 ///< Clock has not propagated through divider yet.   
#define CLKSEL_DIVRDY__READY          0x80 ///< Clock has propagated through divider.           
                                                                                                
//------------------------------------------------------------------------------
// CLU0CF Enums (Configurable Logic Unit 0 Configuration @ 0xB1)
//------------------------------------------------------------------------------
#define CLU0CF_CLKSEL__FMASK     0x03 ///< CLU D flip-flop Clock Selection                  
#define CLU0CF_CLKSEL__SHIFT     0x00 ///< CLU D flip-flop Clock Selection                  
#define CLU0CF_CLKSEL__CARRY_IN  0x00 ///< The carry-in signal.                             
#define CLU0CF_CLKSEL__MXA_INPUT 0x01 ///< The MXA input.                                   
#define CLU0CF_CLKSEL__SYSCLK    0x02 ///< SYSCLK.                                          
                                                                                            
#define CLU0CF_CLKINV__BMASK     0x04 ///< CLU D flip-flop Clock Invert                     
#define CLU0CF_CLKINV__SHIFT     0x02 ///< CLU D flip-flop Clock Invert                     
#define CLU0CF_CLKINV__NORMAL    0x00 ///< Clock signal is not inverted.                    
#define CLU0CF_CLKINV__INVERT    0x04 ///< Clock signal will be inverted.                   
                                                                                            
#define CLU0CF_RST__BMASK        0x08 ///< CLU D flip-flop Reset                            
#define CLU0CF_RST__SHIFT        0x03 ///< CLU D flip-flop Reset                            
#define CLU0CF_RST__RESET        0x08 ///< Reset the flip flop.                             
                                                                                            
#define CLU0CF_OEN__BMASK        0x40 ///< CLU Port Output Enable                           
#define CLU0CF_OEN__SHIFT        0x06 ///< CLU Port Output Enable                           
#define CLU0CF_OEN__DISABLE      0x00 ///< Disables asynchronous output to the selected GPIO
                                      ///< pin                                              
#define CLU0CF_OEN__ENABLE       0x40 ///< Enables asynchronous output to the selected GPIO 
                                      ///< pin                                              
                                                                                            
#define CLU0CF_OUTSEL__BMASK     0x80 ///< CLU Output Select                                
#define CLU0CF_OUTSEL__SHIFT     0x07 ///< CLU Output Select                                
#define CLU0CF_OUTSEL__D_FF      0x00 ///< Select D flip-flop output of CLU                 
#define CLU0CF_OUTSEL__LUT       0x80 ///< Select LUT output.                               
                                                                                            
//------------------------------------------------------------------------------
// CLU0FN Enums (Configurable Logic Unit 0 Function Select @ 0xAF)
//------------------------------------------------------------------------------
#define CLU0FN_FNSEL__FMASK 0xFF ///< CLU Look-Up-Table function select
#define CLU0FN_FNSEL__SHIFT 0x00 ///< CLU Look-Up-Table function select
                                                                       
//------------------------------------------------------------------------------
// CLU0MX Enums (Configurable Logic Unit 0 Multiplexer @ 0x84)
//------------------------------------------------------------------------------
#define CLU0MX_MXB__FMASK   0x0F ///< CLU0 B Input Multiplexer Selection
#define CLU0MX_MXB__SHIFT   0x00 ///< CLU0 B Input Multiplexer Selection
#define CLU0MX_MXB__CLU0B0  0x00 ///< Select CLU0B.0                    
#define CLU0MX_MXB__CLU0B1  0x01 ///< Select CLU0B.1                    
#define CLU0MX_MXB__CLU0B2  0x02 ///< Select CLU0B.2                    
#define CLU0MX_MXB__CLU0B3  0x03 ///< Select CLU0B.3                    
#define CLU0MX_MXB__CLU0B4  0x04 ///< Select CLU0B.4                    
#define CLU0MX_MXB__CLU0B5  0x05 ///< Select CLU0B.5                    
#define CLU0MX_MXB__CLU0B6  0x06 ///< Select CLU0B.6                    
#define CLU0MX_MXB__CLU0B7  0x07 ///< Select CLU0B.7                    
#define CLU0MX_MXB__CLU0B8  0x08 ///< Select CLU0B.8                    
#define CLU0MX_MXB__CLU0B9  0x09 ///< Select CLU0B.9                    
#define CLU0MX_MXB__CLU0B10 0x0A ///< Select CLU0B.10                   
#define CLU0MX_MXB__CLU0B11 0x0B ///< Select CLU0B.11                   
#define CLU0MX_MXB__CLU0B12 0x0C ///< Select CLU0B.12                   
#define CLU0MX_MXB__CLU0B13 0x0D ///< Select CLU0B.13                   
#define CLU0MX_MXB__CLU0B14 0x0E ///< Select CLU0B.14                   
#define CLU0MX_MXB__CLU0B15 0x0F ///< Select CLU0B.15                   
                                                                        
#define CLU0MX_MXA__FMASK   0xF0 ///< CLU0 A Input Multiplexer Selection
#define CLU0MX_MXA__SHIFT   0x04 ///< CLU0 A Input Multiplexer Selection
#define CLU0MX_MXA__CLU0A0  0x00 ///< Select CLU0A.0                    
#define CLU0MX_MXA__CLU0A1  0x10 ///< Select CLU0A.1                    
#define CLU0MX_MXA__CLU0A2  0x20 ///< Select CLU0A.2                    
#define CLU0MX_MXA__CLU0A3  0x30 ///< Select CLU0A.3                    
#define CLU0MX_MXA__CLU0A4  0x40 ///< Select CLU0A.4                    
#define CLU0MX_MXA__CLU0A5  0x50 ///< Select CLU0A.5                    
#define CLU0MX_MXA__CLU0A6  0x60 ///< Select CLU0A.6                    
#define CLU0MX_MXA__CLU0A7  0x70 ///< Select CLU0A.7                    
#define CLU0MX_MXA__CLU0A8  0x80 ///< Select CLU0A.8                    
#define CLU0MX_MXA__CLU0A9  0x90 ///< Select CLU0A.9                    
#define CLU0MX_MXA__CLU0A10 0xA0 ///< Select CLU0A.10                   
#define CLU0MX_MXA__CLU0A11 0xB0 ///< Select CLU0A.11                   
#define CLU0MX_MXA__CLU0A12 0xC0 ///< Select CLU0A.12                   
#define CLU0MX_MXA__CLU0A13 0xD0 ///< Select CLU0A.13                   
#define CLU0MX_MXA__CLU0A14 0xE0 ///< Select CLU0A.14                   
#define CLU0MX_MXA__CLU0A15 0xF0 ///< Select CLU0A.15                   
                                                                        
//------------------------------------------------------------------------------
// CLU1CF Enums (Configurable Logic Unit 1 Configuration @ 0xB3)
//------------------------------------------------------------------------------
#define CLU1CF_CLKSEL__FMASK     0x03 ///< CLU D flip-flop Clock Selection                  
#define CLU1CF_CLKSEL__SHIFT     0x00 ///< CLU D flip-flop Clock Selection                  
#define CLU1CF_CLKSEL__CARRY_IN  0x00 ///< The carry-in signal.                             
#define CLU1CF_CLKSEL__MXA_INPUT 0x01 ///< The MXA input.                                   
#define CLU1CF_CLKSEL__SYSCLK    0x02 ///< SYSCLK.                                          
                                                                                            
#define CLU1CF_CLKINV__BMASK     0x04 ///< CLU D flip-flop Clock Invert                     
#define CLU1CF_CLKINV__SHIFT     0x02 ///< CLU D flip-flop Clock Invert                     
#define CLU1CF_CLKINV__NORMAL    0x00 ///< Clock signal is not inverted.                    
#define CLU1CF_CLKINV__INVERT    0x04 ///< Clock signal will be inverted.                   
                                                                                            
#define CLU1CF_RST__BMASK        0x08 ///< CLU D flip-flop Reset                            
#define CLU1CF_RST__SHIFT        0x03 ///< CLU D flip-flop Reset                            
#define CLU1CF_RST__RESET        0x08 ///< Reset the flip flop.                             
                                                                                            
#define CLU1CF_OEN__BMASK        0x40 ///< CLU Port Output Enable                           
#define CLU1CF_OEN__SHIFT        0x06 ///< CLU Port Output Enable                           
#define CLU1CF_OEN__DISABLE      0x00 ///< Disables asynchronous output to the selected GPIO
                                      ///< pin                                              
#define CLU1CF_OEN__ENABLE       0x40 ///< Enables asynchronous output to the selected GPIO 
                                      ///< pin                                              
                                                                                            
#define CLU1CF_OUTSEL__BMASK     0x80 ///< CLU Output Select                                
#define CLU1CF_OUTSEL__SHIFT     0x07 ///< CLU Output Select                                
#define CLU1CF_OUTSEL__D_FF      0x00 ///< Select D flip-flop output of CLU                 
#define CLU1CF_OUTSEL__LUT       0x80 ///< Select LUT output.                               
                                                                                            
//------------------------------------------------------------------------------
// CLU1FN Enums (Configurable Logic Unit 1 Function Select @ 0xB2)
//------------------------------------------------------------------------------
#define CLU1FN_FNSEL__FMASK 0xFF ///< CLU Look-Up-Table function select
#define CLU1FN_FNSEL__SHIFT 0x00 ///< CLU Look-Up-Table function select
                                                                       
//------------------------------------------------------------------------------
// CLU1MX Enums (Configurable Logic Unit 1 Multiplexer @ 0x85)
//------------------------------------------------------------------------------
#define CLU1MX_MXB__FMASK   0x0F ///< CLU1 B Input Multiplexer Selection
#define CLU1MX_MXB__SHIFT   0x00 ///< CLU1 B Input Multiplexer Selection
#define CLU1MX_MXB__CLU1B0  0x00 ///< Select CLU1B.0                    
#define CLU1MX_MXB__CLU1B1  0x01 ///< Select CLU1B.1                    
#define CLU1MX_MXB__CLU1B2  0x02 ///< Select CLU1B.2                    
#define CLU1MX_MXB__CLU1B3  0x03 ///< Select CLU1B.3                    
#define CLU1MX_MXB__CLU1B4  0x04 ///< Select CLU1B.4                    
#define CLU1MX_MXB__CLU1B5  0x05 ///< Select CLU1B.5                    
#define CLU1MX_MXB__CLU1B6  0x06 ///< Select CLU1B.6                    
#define CLU1MX_MXB__CLU1B7  0x07 ///< Select CLU1B.7                    
#define CLU1MX_MXB__CLU1B8  0x08 ///< Select CLU1B.8                    
#define CLU1MX_MXB__CLU1B9  0x09 ///< Select CLU1B.9                    
#define CLU1MX_MXB__CLU1B10 0x0A ///< Select CLU1B.10                   
#define CLU1MX_MXB__CLU1B11 0x0B ///< Select CLU1B.11                   
#define CLU1MX_MXB__CLU1B12 0x0C ///< Select CLU1B.12                   
#define CLU1MX_MXB__CLU1B13 0x0D ///< Select CLU1B.13                   
#define CLU1MX_MXB__CLU1B14 0x0E ///< Select CLU1B.14                   
#define CLU1MX_MXB__CLU1B15 0x0F ///< Select CLU1B.15                   
                                                                        
#define CLU1MX_MXA__FMASK   0xF0 ///< CLU1 A Input Multiplexer Selection
#define CLU1MX_MXA__SHIFT   0x04 ///< CLU1 A Input Multiplexer Selection
#define CLU1MX_MXA__CLU1A0  0x00 ///< Select CLU1A.0                    
#define CLU1MX_MXA__CLU1A1  0x10 ///< Select CLU1A.1                    
#define CLU1MX_MXA__CLU1A2  0x20 ///< Select CLU1A.2                    
#define CLU1MX_MXA__CLU1A3  0x30 ///< Select CLU1A.3                    
#define CLU1MX_MXA__CLU1A4  0x40 ///< Select CLU1A.4                    
#define CLU1MX_MXA__CLU1A5  0x50 ///< Select CLU1A.5                    
#define CLU1MX_MXA__CLU1A6  0x60 ///< Select CLU1A.6                    
#define CLU1MX_MXA__CLU1A7  0x70 ///< Select CLU1A.7                    
#define CLU1MX_MXA__CLU1A8  0x80 ///< Select CLU1A.8                    
#define CLU1MX_MXA__CLU1A9  0x90 ///< Select CLU1A.9                    
#define CLU1MX_MXA__CLU1A10 0xA0 ///< Select CLU1A.10                   
#define CLU1MX_MXA__CLU1A11 0xB0 ///< Select CLU1A.11                   
#define CLU1MX_MXA__CLU1A12 0xC0 ///< Select CLU1A.12                   
#define CLU1MX_MXA__CLU1A13 0xD0 ///< Select CLU1A.13                   
#define CLU1MX_MXA__CLU1A14 0xE0 ///< Select CLU1A.14                   
#define CLU1MX_MXA__CLU1A15 0xF0 ///< Select CLU1A.15                   
                                                                        
//------------------------------------------------------------------------------
// CLU2CF Enums (Configurable Logic Unit 2 Configuration @ 0xB6)
//------------------------------------------------------------------------------
#define CLU2CF_CLKSEL__FMASK     0x03 ///< CLU D flip-flop Clock Selection                  
#define CLU2CF_CLKSEL__SHIFT     0x00 ///< CLU D flip-flop Clock Selection                  
#define CLU2CF_CLKSEL__CARRY_IN  0x00 ///< The carry-in signal.                             
#define CLU2CF_CLKSEL__MXA_INPUT 0x01 ///< The MXA input.                                   
#define CLU2CF_CLKSEL__SYSCLK    0x02 ///< SYSCLK.                                          
                                                                                            
#define CLU2CF_CLKINV__BMASK     0x04 ///< CLU D flip-flop Clock Invert                     
#define CLU2CF_CLKINV__SHIFT     0x02 ///< CLU D flip-flop Clock Invert                     
#define CLU2CF_CLKINV__NORMAL    0x00 ///< Clock signal is not inverted.                    
#define CLU2CF_CLKINV__INVERT    0x04 ///< Clock signal will be inverted.                   
                                                                                            
#define CLU2CF_RST__BMASK        0x08 ///< CLU D flip-flop Reset                            
#define CLU2CF_RST__SHIFT        0x03 ///< CLU D flip-flop Reset                            
#define CLU2CF_RST__RESET        0x08 ///< Reset the flip flop.                             
                                                                                            
#define CLU2CF_OEN__BMASK        0x40 ///< CLU Port Output Enable                           
#define CLU2CF_OEN__SHIFT        0x06 ///< CLU Port Output Enable                           
#define CLU2CF_OEN__DISABLE      0x00 ///< Disables asynchronous output to the selected GPIO
                                      ///< pin                                              
#define CLU2CF_OEN__ENABLE       0x40 ///< Enables asynchronous output to the selected GPIO 
                                      ///< pin                                              
                                                                                            
#define CLU2CF_OUTSEL__BMASK     0x80 ///< CLU Output Select                                
#define CLU2CF_OUTSEL__SHIFT     0x07 ///< CLU Output Select                                
#define CLU2CF_OUTSEL__D_FF      0x00 ///< Select D flip-flop output of CLU                 
#define CLU2CF_OUTSEL__LUT       0x80 ///< Select LUT output.                               
                                                                                            
//------------------------------------------------------------------------------
// CLU2FN Enums (Configurable Logic Unit 2 Function Select @ 0xB5)
//------------------------------------------------------------------------------
#define CLU2FN_FNSEL__FMASK 0xFF ///< CLU Look-Up-Table function select
#define CLU2FN_FNSEL__SHIFT 0x00 ///< CLU Look-Up-Table function select
                                                                       
//------------------------------------------------------------------------------
// CLU2MX Enums (Configurable Logic Unit 2 Multiplexer @ 0x91)
//------------------------------------------------------------------------------
#define CLU2MX_MXB__FMASK   0x0F ///< CLU2 B Input Multiplexer Selection
#define CLU2MX_MXB__SHIFT   0x00 ///< CLU2 B Input Multiplexer Selection
#define CLU2MX_MXB__CLU2B0  0x00 ///< Select CLU2B.0                    
#define CLU2MX_MXB__CLU2B1  0x01 ///< Select CLU2B.1                    
#define CLU2MX_MXB__CLU2B2  0x02 ///< Select CLU2B.2                    
#define CLU2MX_MXB__CLU2B3  0x03 ///< Select CLU2B.3                    
#define CLU2MX_MXB__CLU2B4  0x04 ///< Select CLU2B.4                    
#define CLU2MX_MXB__CLU2B5  0x05 ///< Select CLU2B.5                    
#define CLU2MX_MXB__CLU2B6  0x06 ///< Select CLU2B.6                    
#define CLU2MX_MXB__CLU2B7  0x07 ///< Select CLU2B.7                    
#define CLU2MX_MXB__CLU2B8  0x08 ///< Select CLU2B.8                    
#define CLU2MX_MXB__CLU2B9  0x09 ///< Select CLU2B.9                    
#define CLU2MX_MXB__CLU2B10 0x0A ///< Select CLU2B.10                   
#define CLU2MX_MXB__CLU2B11 0x0B ///< Select CLU2B.11                   
#define CLU2MX_MXB__CLU2B12 0x0C ///< Select CLU2B.12                   
#define CLU2MX_MXB__CLU2B13 0x0D ///< Select CLU2B.13                   
#define CLU2MX_MXB__CLU2B14 0x0E ///< Select CLU2B.14                   
#define CLU2MX_MXB__CLU2B15 0x0F ///< Select CLU2B.15                   
                                                                        
#define CLU2MX_MXA__FMASK   0xF0 ///< CLU2 A Input Multiplexer Selection
#define CLU2MX_MXA__SHIFT   0x04 ///< CLU2 A Input Multiplexer Selection
#define CLU2MX_MXA__CLU2A0  0x00 ///< Select CLU2A.0                    
#define CLU2MX_MXA__CLU2A1  0x10 ///< Select CLU2A.1                    
#define CLU2MX_MXA__CLU2A2  0x20 ///< Select CLU2A.2                    
#define CLU2MX_MXA__CLU2A3  0x30 ///< Select CLU2A.3                    
#define CLU2MX_MXA__CLU2A4  0x40 ///< Select CLU2A.4                    
#define CLU2MX_MXA__CLU2A5  0x50 ///< Select CLU2A.5                    
#define CLU2MX_MXA__CLU2A6  0x60 ///< Select CLU2A.6                    
#define CLU2MX_MXA__CLU2A7  0x70 ///< Select CLU2A.7                    
#define CLU2MX_MXA__CLU2A8  0x80 ///< Select CLU2A.8                    
#define CLU2MX_MXA__CLU2A9  0x90 ///< Select CLU2A.9                    
#define CLU2MX_MXA__CLU2A10 0xA0 ///< Select CLU2A.10                   
#define CLU2MX_MXA__CLU2A11 0xB0 ///< Select CLU2A.11                   
#define CLU2MX_MXA__CLU2A12 0xC0 ///< Select CLU2A.12                   
#define CLU2MX_MXA__CLU2A13 0xD0 ///< Select CLU2A.13                   
#define CLU2MX_MXA__CLU2A14 0xE0 ///< Select CLU2A.14                   
#define CLU2MX_MXA__CLU2A15 0xF0 ///< Select CLU2A.15                   
                                                                        
//------------------------------------------------------------------------------
// CLU3CF Enums (Configurable Logic Unit 3 Configuration @ 0xBF)
//------------------------------------------------------------------------------
#define CLU3CF_CLKSEL__FMASK     0x03 ///< CLU D flip-flop Clock Selection                  
#define CLU3CF_CLKSEL__SHIFT     0x00 ///< CLU D flip-flop Clock Selection                  
#define CLU3CF_CLKSEL__CARRY_IN  0x00 ///< The carry-in signal.                             
#define CLU3CF_CLKSEL__MXA_INPUT 0x01 ///< The MXA input.                                   
#define CLU3CF_CLKSEL__SYSCLK    0x02 ///< SYSCLK.                                          
                                                                                            
#define CLU3CF_CLKINV__BMASK     0x04 ///< CLU D flip-flop Clock Invert                     
#define CLU3CF_CLKINV__SHIFT     0x02 ///< CLU D flip-flop Clock Invert                     
#define CLU3CF_CLKINV__NORMAL    0x00 ///< Clock signal is not inverted.                    
#define CLU3CF_CLKINV__INVERT    0x04 ///< Clock signal will be inverted.                   
                                                                                            
#define CLU3CF_RST__BMASK        0x08 ///< CLU D flip-flop Reset                            
#define CLU3CF_RST__SHIFT        0x03 ///< CLU D flip-flop Reset                            
#define CLU3CF_RST__RESET        0x08 ///< Reset the flip flop.                             
                                                                                            
#define CLU3CF_OEN__BMASK        0x40 ///< CLU Port Output Enable                           
#define CLU3CF_OEN__SHIFT        0x06 ///< CLU Port Output Enable                           
#define CLU3CF_OEN__DISABLE      0x00 ///< Disables asynchronous output to the selected GPIO
                                      ///< pin                                              
#define CLU3CF_OEN__ENABLE       0x40 ///< Enables asynchronous output to the selected GPIO 
                                      ///< pin                                              
                                                                                            
#define CLU3CF_OUTSEL__BMASK     0x80 ///< CLU Output Select                                
#define CLU3CF_OUTSEL__SHIFT     0x07 ///< CLU Output Select                                
#define CLU3CF_OUTSEL__D_FF      0x00 ///< Select D flip-flop output of CLU                 
#define CLU3CF_OUTSEL__LUT       0x80 ///< Select LUT output.                               
                                                                                            
//------------------------------------------------------------------------------
// CLU3FN Enums (Configurable Logic Unit 3 Function Select @ 0xBE)
//------------------------------------------------------------------------------
#define CLU3FN_FNSEL__FMASK 0xFF ///< CLU Look-Up-Table function select
#define CLU3FN_FNSEL__SHIFT 0x00 ///< CLU Look-Up-Table function select
                                                                       
//------------------------------------------------------------------------------
// CLU3MX Enums (Configurable Logic Unit 3 Multiplexer @ 0xAE)
//------------------------------------------------------------------------------
#define CLU3MX_MXB__FMASK   0x0F ///< CLU3 B Input Multiplexer Selection
#define CLU3MX_MXB__SHIFT   0x00 ///< CLU3 B Input Multiplexer Selection
#define CLU3MX_MXB__CLU3B0  0x00 ///< Select CLU3B.0                    
#define CLU3MX_MXB__CLU3B1  0x01 ///< Select CLU3B.1                    
#define CLU3MX_MXB__CLU3B2  0x02 ///< Select CLU3B.2                    
#define CLU3MX_MXB__CLU3B3  0x03 ///< Select CLU3B.3                    
#define CLU3MX_MXB__CLU3B4  0x04 ///< Select CLU3B.4                    
#define CLU3MX_MXB__CLU3B5  0x05 ///< Select CLU3B.5                    
#define CLU3MX_MXB__CLU3B6  0x06 ///< Select CLU3B.6                    
#define CLU3MX_MXB__CLU3B7  0x07 ///< Select CLU3B.7                    
#define CLU3MX_MXB__CLU3B8  0x08 ///< Select CLU3B.8                    
#define CLU3MX_MXB__CLU3B9  0x09 ///< Select CLU3B.9                    
#define CLU3MX_MXB__CLU3B10 0x0A ///< Select CLU3B.10                   
#define CLU3MX_MXB__CLU3B11 0x0B ///< Select CLU3B.11                   
#define CLU3MX_MXB__CLU3B12 0x0C ///< Select CLU3B.12                   
#define CLU3MX_MXB__CLU3B13 0x0D ///< Select CLU3B.13                   
#define CLU3MX_MXB__CLU3B14 0x0E ///< Select CLU3B.14                   
#define CLU3MX_MXB__CLU3B15 0x0F ///< Select CLU3B.15                   
                                                                        
#define CLU3MX_MXA__FMASK   0xF0 ///< CLU3 A Input Multiplexer Selection
#define CLU3MX_MXA__SHIFT   0x04 ///< CLU3 A Input Multiplexer Selection
#define CLU3MX_MXA__CLU3A0  0x00 ///< Select CLU3A.0                    
#define CLU3MX_MXA__CLU3A1  0x10 ///< Select CLU3A.1                    
#define CLU3MX_MXA__CLU3A2  0x20 ///< Select CLU3A.2                    
#define CLU3MX_MXA__CLU3A3  0x30 ///< Select CLU3A.3                    
#define CLU3MX_MXA__CLU3A4  0x40 ///< Select CLU3A.4                    
#define CLU3MX_MXA__CLU3A5  0x50 ///< Select CLU3A.5                    
#define CLU3MX_MXA__CLU3A6  0x60 ///< Select CLU3A.6                    
#define CLU3MX_MXA__CLU3A7  0x70 ///< Select CLU3A.7                    
#define CLU3MX_MXA__CLU3A8  0x80 ///< Select CLU3A.8                    
#define CLU3MX_MXA__CLU3A9  0x90 ///< Select CLU3A.9                    
#define CLU3MX_MXA__CLU3A10 0xA0 ///< Select CLU3A.10                   
#define CLU3MX_MXA__CLU3A11 0xB0 ///< Select CLU3A.11                   
#define CLU3MX_MXA__CLU3A12 0xC0 ///< Select CLU3A.12                   
#define CLU3MX_MXA__CLU3A13 0xD0 ///< Select CLU3A.13                   
#define CLU3MX_MXA__CLU3A14 0xE0 ///< Select CLU3A.14                   
#define CLU3MX_MXA__CLU3A15 0xF0 ///< Select CLU3A.15                   
                                                                        
//------------------------------------------------------------------------------
// CLEN0 Enums (Configurable Logic Enable 0 @ 0xC6)
//------------------------------------------------------------------------------
#define CLEN0_C0EN__BMASK   0x01 ///< CLU0 Enable                                      
#define CLEN0_C0EN__SHIFT   0x00 ///< CLU0 Enable                                      
#define CLEN0_C0EN__DISABLE 0x00 ///< CLU0 is disabled. The output of the block will be
                                 ///< logic low.                                       
#define CLEN0_C0EN__ENABLE  0x01 ///< CLU0 is enabled.                                 
                                                                                       
#define CLEN0_C1EN__BMASK   0x02 ///< CLU1 Enable                                      
#define CLEN0_C1EN__SHIFT   0x01 ///< CLU1 Enable                                      
#define CLEN0_C1EN__DISABLE 0x00 ///< CLU1 is disabled. The output of the block will be
                                 ///< logic low.                                       
#define CLEN0_C1EN__ENABLE  0x02 ///< CLU1 is enabled.                                 
                                                                                       
#define CLEN0_C2EN__BMASK   0x04 ///< CLU2 Enable                                      
#define CLEN0_C2EN__SHIFT   0x02 ///< CLU2 Enable                                      
#define CLEN0_C2EN__DISABLE 0x00 ///< CLU2 is disabled. The output of the block will be
                                 ///< logic low.                                       
#define CLEN0_C2EN__ENABLE  0x04 ///< CLU2 is enabled.                                 
                                                                                       
#define CLEN0_C3EN__BMASK   0x08 ///< CLU3 Enable                                      
#define CLEN0_C3EN__SHIFT   0x03 ///< CLU3 Enable                                      
#define CLEN0_C3EN__DISABLE 0x00 ///< CLU3 is disabled. The output of the block will be
                                 ///< logic low.                                       
#define CLEN0_C3EN__ENABLE  0x08 ///< CLU3 is enabled.                                 
                                                                                       
//------------------------------------------------------------------------------
// CLIE0 Enums (Configurable Logic Interrupt Enable 0 @ 0xC7)
//------------------------------------------------------------------------------
#define CLIE0_C0FIE__BMASK   0x01 ///< CLU0 Falling Edge Interrupt Enable                
#define CLIE0_C0FIE__SHIFT   0x00 ///< CLU0 Falling Edge Interrupt Enable                
#define CLIE0_C0FIE__DISABLE 0x00 ///< Interrupts will not be generated for CLU0 falling-
                                  ///< edge events.                                      
#define CLIE0_C0FIE__ENABLE  0x01 ///< Interrupts will be generated for CLU0 falling-edge
                                  ///< events.                                           
                                                                                         
#define CLIE0_C0RIE__BMASK   0x02 ///< CLU0 Rising Edge Interrupt Enable                 
#define CLIE0_C0RIE__SHIFT   0x01 ///< CLU0 Rising Edge Interrupt Enable                 
#define CLIE0_C0RIE__DISABLE 0x00 ///< Interrupts will not be generated for CLU0 rising- 
                                  ///< edge events.                                      
#define CLIE0_C0RIE__ENABLE  0x02 ///< Interrupts will be generated for CLU0 rising-edge 
                                  ///< events.                                           
                                                                                         
#define CLIE0_C1FIE__BMASK   0x04 ///< CLU1 Falling Edge Interrupt Enable                
#define CLIE0_C1FIE__SHIFT   0x02 ///< CLU1 Falling Edge Interrupt Enable                
#define CLIE0_C1FIE__DISABLE 0x00 ///< Interrupts will not be generated for CLU1 falling-
                                  ///< edge events.                                      
#define CLIE0_C1FIE__ENABLE  0x04 ///< Interrupts will be generated for CLU1 falling-edge
                                  ///< events.                                           
                                                                                         
#define CLIE0_C1RIE__BMASK   0x08 ///< CLU1 Rising Edge Interrupt Enable                 
#define CLIE0_C1RIE__SHIFT   0x03 ///< CLU1 Rising Edge Interrupt Enable                 
#define CLIE0_C1RIE__DISABLE 0x00 ///< Interrupts will not be generated for CLU1 rising- 
                                  ///< edge events.                                      
#define CLIE0_C1RIE__ENABLE  0x08 ///< Interrupts will be generated for CLU1 rising-edge 
                                  ///< events.                                           
                                                                                         
#define CLIE0_C2FIE__BMASK   0x10 ///< CLU2 Falling Edge Interrupt Enable                
#define CLIE0_C2FIE__SHIFT   0x04 ///< CLU2 Falling Edge Interrupt Enable                
#define CLIE0_C2FIE__DISABLE 0x00 ///< Interrupts will not be generated for CLU2 falling-
                                  ///< edge events.                                      
#define CLIE0_C2FIE__ENABLE  0x10 ///< Interrupts will be generated for CLU2 falling-edge
                                  ///< events.                                           
                                                                                         
#define CLIE0_C2RIE__BMASK   0x20 ///< CLU2 Rising Edge Interrupt Enable                 
#define CLIE0_C2RIE__SHIFT   0x05 ///< CLU2 Rising Edge Interrupt Enable                 
#define CLIE0_C2RIE__DISABLE 0x00 ///< Interrupts will not be generated for CLU2 rising- 
                                  ///< edge events.                                      
#define CLIE0_C2RIE__ENABLE  0x20 ///< Interrupts will be generated for CLU2 rising-edge 
                                  ///< events.                                           
                                                                                         
#define CLIE0_C3FIE__BMASK   0x40 ///< CLU3 Falling Edge Interrupt Enable                
#define CLIE0_C3FIE__SHIFT   0x06 ///< CLU3 Falling Edge Interrupt Enable                
#define CLIE0_C3FIE__DISABLE 0x00 ///< Interrupts will not be generated for CLU3 falling-
                                  ///< edge events.                                      
#define CLIE0_C3FIE__ENABLE  0x40 ///< Interrupts will be generated for CLU3 falling-edge
                                  ///< events.                                           
                                                                                         
#define CLIE0_C3RIE__BMASK   0x80 ///< CLU3 Rising Edge Interrupt Enable                 
#define CLIE0_C3RIE__SHIFT   0x07 ///< CLU3 Rising Edge Interrupt Enable                 
#define CLIE0_C3RIE__DISABLE 0x00 ///< Interrupts will not be generated for CLU3 rising- 
                                  ///< edge events.                                      
#define CLIE0_C3RIE__ENABLE  0x80 ///< Interrupts will be generated for CLU3 rising-edge 
                                  ///< events.                                           
                                                                                         
//------------------------------------------------------------------------------
// CLIF0 Enums (Configurable Logic Interrupt Flag 0 @ 0xE8)
//------------------------------------------------------------------------------
#define CLIF0_C0FIF__BMASK   0x01 ///< CLU0 Falling Edge Flag                            
#define CLIF0_C0FIF__SHIFT   0x00 ///< CLU0 Falling Edge Flag                            
#define CLIF0_C0FIF__NOT_SET 0x00 ///< A CLU0 falling edge has not been detected since   
                                  ///< this flag was last cleared.                       
#define CLIF0_C0FIF__SET     0x01 ///< A CLU0 falling edge (synchronized with SYSCLK) has
                                  ///< occurred. This bit must be cleared by firmware.   
                                                                                         
#define CLIF0_C0RIF__BMASK   0x02 ///< CLU0 Rising Edge Flag                             
#define CLIF0_C0RIF__SHIFT   0x01 ///< CLU0 Rising Edge Flag                             
#define CLIF0_C0RIF__NOT_SET 0x00 ///< A CLU0 rising edge has not been detected since    
                                  ///< this flag was last cleared.                       
#define CLIF0_C0RIF__SET     0x02 ///< A CLU0 rising edge (synchronized with SYSCLK) has 
                                  ///< occurred. This bit must be cleared by firmware.   
                                                                                         
#define CLIF0_C1FIF__BMASK   0x04 ///< CLU1 Falling Edge Flag                            
#define CLIF0_C1FIF__SHIFT   0x02 ///< CLU1 Falling Edge Flag                            
#define CLIF0_C1FIF__NOT_SET 0x00 ///< A CLU1 falling edge has not been detected since   
                                  ///< this flag was last cleared.                       
#define CLIF0_C1FIF__SET     0x04 ///< A CLU1 falling edge (synchronized with SYSCLK) has
                                  ///< occurred. This bit must be cleared by firmware.   
                                                                                         
#define CLIF0_C1RIF__BMASK   0x08 ///< CLU1 Rising Edge Flag                             
#define CLIF0_C1RIF__SHIFT   0x03 ///< CLU1 Rising Edge Flag                             
#define CLIF0_C1RIF__NOT_SET 0x00 ///< A CLU1 rising edge has not been detected since    
                                  ///< this flag was last cleared.                       
#define CLIF0_C1RIF__SET     0x08 ///< A CLU1 rising edge (synchronized with SYSCLK) has 
                                  ///< occurred. This bit must be cleared by firmware.   
                                                                                         
#define CLIF0_C2FIF__BMASK   0x10 ///< CLU2 Falling Edge Flag                            
#define CLIF0_C2FIF__SHIFT   0x04 ///< CLU2 Falling Edge Flag                            
#define CLIF0_C2FIF__NOT_SET 0x00 ///< A CLU2 falling edge has not been detected since   
                                  ///< this flag was last cleared.                       
#define CLIF0_C2FIF__SET     0x10 ///< A CLU2 falling edge (synchronized with SYSCLK) has
                                  ///< occurred. This bit must be cleared by firmware.   
                                                                                         
#define CLIF0_C2RIF__BMASK   0x20 ///< CLU2 Rising Edge Flag                             
#define CLIF0_C2RIF__SHIFT   0x05 ///< CLU2 Rising Edge Flag                             
#define CLIF0_C2RIF__NOT_SET 0x00 ///< A CLU2 rising edge has not been detected since    
                                  ///< this flag was last cleared.                       
#define CLIF0_C2RIF__SET     0x20 ///< A CLU2 rising edge (synchronized with SYSCLK) has 
                                  ///< occurred. This bit must be cleared by firmware.   
                                                                                         
#define CLIF0_C3FIF__BMASK   0x40 ///< CLU3 Falling Edge Flag                            
#define CLIF0_C3FIF__SHIFT   0x06 ///< CLU3 Falling Edge Flag                            
#define CLIF0_C3FIF__NOT_SET 0x00 ///< A CLU3 falling edge has not been detected since   
                                  ///< this flag was last cleared.                       
#define CLIF0_C3FIF__SET     0x40 ///< A CLU3 falling edge (synchronized with SYSCLK) has
                                  ///< occurred. This bit must be cleared by firmware.   
                                                                                         
#define CLIF0_C3RIF__BMASK   0x80 ///< CLU3 Rising Edge Flag                             
#define CLIF0_C3RIF__SHIFT   0x07 ///< CLU3 Rising Edge Flag                             
#define CLIF0_C3RIF__NOT_SET 0x00 ///< A CLU3 rising edge has not been detected since    
                                  ///< this flag was last cleared.                       
#define CLIF0_C3RIF__SET     0x80 ///< A CLU3 rising edge (synchronized with SYSCLK) has 
                                  ///< occurred. This bit must be cleared by firmware.   
                                                                                         
//------------------------------------------------------------------------------
// CLOUT0 Enums (Configurable Logic Output 0 @ 0xD1)
//------------------------------------------------------------------------------
#define CLOUT0_C0OUT__BMASK 0x01 ///< CLU0 Output State
#define CLOUT0_C0OUT__SHIFT 0x00 ///< CLU0 Output State
                                                       
#define CLOUT0_C1OUT__BMASK 0x02 ///< CLU1 Output State
#define CLOUT0_C1OUT__SHIFT 0x01 ///< CLU1 Output State
                                                       
#define CLOUT0_C2OUT__BMASK 0x04 ///< CLU2 Output State
#define CLOUT0_C2OUT__SHIFT 0x02 ///< CLU2 Output State
                                                       
#define CLOUT0_C3OUT__BMASK 0x08 ///< CLU3 Output State
#define CLOUT0_C3OUT__SHIFT 0x03 ///< CLU3 Output State
                                                       
//------------------------------------------------------------------------------
// CMP0CN0 Enums (Comparator 0 Control 0 @ 0x9B)
//------------------------------------------------------------------------------
#define CMP0CN0_CPHYN__FMASK                0x03 ///< Comparator Negative Hysteresis Control            
#define CMP0CN0_CPHYN__SHIFT                0x00 ///< Comparator Negative Hysteresis Control            
#define CMP0CN0_CPHYN__DISABLED             0x00 ///< Negative Hysteresis disabled.                     
#define CMP0CN0_CPHYN__ENABLED_MODE1        0x01 ///< Negative Hysteresis = Hysteresis 1.               
#define CMP0CN0_CPHYN__ENABLED_MODE2        0x02 ///< Negative Hysteresis = Hysteresis 2.               
#define CMP0CN0_CPHYN__ENABLED_MODE3        0x03 ///< Negative Hysteresis = Hysteresis 3 (Maximum).     
                                                                                                        
#define CMP0CN0_CPHYP__FMASK                0x0C ///< Comparator Positive Hysteresis Control            
#define CMP0CN0_CPHYP__SHIFT                0x02 ///< Comparator Positive Hysteresis Control            
#define CMP0CN0_CPHYP__DISABLED             0x00 ///< Positive Hysteresis disabled.                     
#define CMP0CN0_CPHYP__ENABLED_MODE1        0x04 ///< Positive Hysteresis = Hysteresis 1.               
#define CMP0CN0_CPHYP__ENABLED_MODE2        0x08 ///< Positive Hysteresis = Hysteresis 2.               
#define CMP0CN0_CPHYP__ENABLED_MODE3        0x0C ///< Positive Hysteresis = Hysteresis 3 (Maximum).     
                                                                                                        
#define CMP0CN0_CPFIF__BMASK                0x10 ///< Comparator Falling-Edge Flag                      
#define CMP0CN0_CPFIF__SHIFT                0x04 ///< Comparator Falling-Edge Flag                      
#define CMP0CN0_CPFIF__NOT_SET              0x00 ///< No comparator falling edge has occurred since this
                                                 ///< flag was last cleared.                            
#define CMP0CN0_CPFIF__FALLING_EDGE         0x10 ///< Comparator falling edge has occurred.             
                                                                                                        
#define CMP0CN0_CPRIF__BMASK                0x20 ///< Comparator Rising-Edge Flag                       
#define CMP0CN0_CPRIF__SHIFT                0x05 ///< Comparator Rising-Edge Flag                       
#define CMP0CN0_CPRIF__NOT_SET              0x00 ///< No comparator rising edge has occurred since this 
                                                 ///< flag was last cleared.                            
#define CMP0CN0_CPRIF__RISING_EDGE          0x20 ///< Comparator rising edge has occurred.              
                                                                                                        
#define CMP0CN0_CPOUT__BMASK                0x40 ///< Comparator Output State Flag                      
#define CMP0CN0_CPOUT__SHIFT                0x06 ///< Comparator Output State Flag                      
#define CMP0CN0_CPOUT__POS_LESS_THAN_NEG    0x00 ///< Voltage on CP0P < CP0N.                           
#define CMP0CN0_CPOUT__POS_GREATER_THAN_NEG 0x40 ///< Voltage on CP0P > CP0N.                           
                                                                                                        
#define CMP0CN0_CPEN__BMASK                 0x80 ///< Comparator Enable                                 
#define CMP0CN0_CPEN__SHIFT                 0x07 ///< Comparator Enable                                 
#define CMP0CN0_CPEN__DISABLED              0x00 ///< Comparator disabled.                              
#define CMP0CN0_CPEN__ENABLED               0x80 ///< Comparator enabled.                               
                                                                                                        
//------------------------------------------------------------------------------
// CMP0CN1 Enums (Comparator 0 Control 1 @ 0x99)
//------------------------------------------------------------------------------
#define CMP0CN1_DACLVL__FMASK   0x3F ///< Internal Comparator DAC Reference Level           
#define CMP0CN1_DACLVL__SHIFT   0x00 ///< Internal Comparator DAC Reference Level           
                                                                                            
#define CMP0CN1_CPINH__BMASK    0x80 ///< Output Inhibit                                    
#define CMP0CN1_CPINH__SHIFT    0x07 ///< Output Inhibit                                    
#define CMP0CN1_CPINH__DISABLED 0x00 ///< The comparator output will always reflect the     
                                     ///< input conditions.                                 
#define CMP0CN1_CPINH__ENABLED  0x80 ///< The comparator output will hold state any time the
                                     ///< PCA CEX2 channel is low.                          
                                                                                            
//------------------------------------------------------------------------------
// CMP0MD Enums (Comparator 0 Mode @ 0x9D)
//------------------------------------------------------------------------------
#define CMP0MD_CPMD__FMASK              0x03 ///< Comparator Mode Select                            
#define CMP0MD_CPMD__SHIFT              0x00 ///< Comparator Mode Select                            
#define CMP0MD_CPMD__MODE0              0x00 ///< Mode 0 (Fastest Response Time, Highest Power      
                                             ///< Consumption)                                      
#define CMP0MD_CPMD__MODE1              0x01 ///< Mode 1                                            
#define CMP0MD_CPMD__MODE2              0x02 ///< Mode 2                                            
#define CMP0MD_CPMD__MODE3              0x03 ///< Mode 3 (Slowest Response Time, Lowest Power       
                                             ///< Consumption)                                      
                                                                                                    
#define CMP0MD_INSL__FMASK              0x0C ///< Comparator Input Selection                        
#define CMP0MD_INSL__SHIFT              0x02 ///< Comparator Input Selection                        
#define CMP0MD_INSL__CMXP_CMXN          0x00 ///< Connect the comparator inputs directly to the     
                                             ///< signals selected in the CMP0MX register. CMP+ is  
                                             ///< selected by CMXP and CMP- is selected by CMXN. The
                                             ///< internal DAC is not active.                       
#define CMP0MD_INSL__CMXP_GND           0x04 ///< Connect the CMP+ input to the signal selected by  
                                             ///< CMXP, and CMP- is connected to GND. The internal  
                                             ///< DAC is not active.                                
#define CMP0MD_INSL__DAC_CMXN           0x08 ///< Connect the CMP+ input to the internal DAC output,
                                             ///< and CMP- is selected by CMXN. The internal DAC    
                                             ///< uses the signal specified by CMXP as its full-    
                                             ///< scale reference.                                  
#define CMP0MD_INSL__CMXP_DAC           0x0C ///< Connect the CMP- input to the internal DAC output,
                                             ///< and CMP+ is selected by CMXP. The internal DAC    
                                             ///< uses the signal specified by CMXN as its full-    
                                             ///< scale reference.                                  
                                                                                                    
#define CMP0MD_CPFIE__BMASK             0x10 ///< Comparator Falling-Edge Interrupt Enable          
#define CMP0MD_CPFIE__SHIFT             0x04 ///< Comparator Falling-Edge Interrupt Enable          
#define CMP0MD_CPFIE__FALL_INT_DISABLED 0x00 ///< Comparator falling-edge interrupt disabled.       
#define CMP0MD_CPFIE__FALL_INT_ENABLED  0x10 ///< Comparator falling-edge interrupt enabled.        
                                                                                                    
#define CMP0MD_CPRIE__BMASK             0x20 ///< Comparator Rising-Edge Interrupt Enable           
#define CMP0MD_CPRIE__SHIFT             0x05 ///< Comparator Rising-Edge Interrupt Enable           
#define CMP0MD_CPRIE__RISE_INT_DISABLED 0x00 ///< Comparator rising-edge interrupt disabled.        
#define CMP0MD_CPRIE__RISE_INT_ENABLED  0x20 ///< Comparator rising-edge interrupt enabled.         
                                                                                                    
#define CMP0MD_CPINV__BMASK             0x40 ///< Output Inversion                                  
#define CMP0MD_CPINV__SHIFT             0x06 ///< Output Inversion                                  
#define CMP0MD_CPINV__NORMAL            0x00 ///< Output is not inverted.                           
#define CMP0MD_CPINV__INVERT            0x40 ///< Output is inverted.                               
                                                                                                    
#define CMP0MD_CPLOUT__BMASK            0x80 ///< Comparator Latched Output Flag                    
#define CMP0MD_CPLOUT__SHIFT            0x07 ///< Comparator Latched Output Flag                    
#define CMP0MD_CPLOUT__LOW              0x00 ///< Comparator output was logic low at last PCA       
                                             ///< overflow.                                         
#define CMP0MD_CPLOUT__HIGH             0x80 ///< Comparator output was logic high at last PCA      
                                             ///< overflow.                                         
                                                                                                    
//------------------------------------------------------------------------------
// CMP0MX Enums (Comparator 0 Multiplexer Selection @ 0x9F)
//------------------------------------------------------------------------------
#define CMP0MX_CMXP__FMASK   0x0F ///< Comparator Positive Input MUX Selection
#define CMP0MX_CMXP__SHIFT   0x00 ///< Comparator Positive Input MUX Selection
#define CMP0MX_CMXP__CMP0P0  0x00 ///< External pin CMP0P.0.                  
#define CMP0MX_CMXP__CMP0P1  0x01 ///< External pin CMP0P.1.                  
#define CMP0MX_CMXP__CMP0P2  0x02 ///< External pin CMP0P.2.                  
#define CMP0MX_CMXP__CMP0P3  0x03 ///< External pin CMP0P.3.                  
#define CMP0MX_CMXP__CMP0P4  0x04 ///< External pin CMP0P.4.                  
#define CMP0MX_CMXP__CMP0P5  0x05 ///< External pin CMP0P.5.                  
#define CMP0MX_CMXP__CMP0P6  0x06 ///< External pin CMP0P.6.                  
#define CMP0MX_CMXP__CMP0P7  0x07 ///< External pin CMP0P.7.                  
#define CMP0MX_CMXP__CMP0P8  0x08 ///< External pin CMP0P.8.                  
#define CMP0MX_CMXP__CMP0P9  0x09 ///< External pin CMP0P.9.                  
#define CMP0MX_CMXP__CMP0P10 0x0A ///< External pin CMP0P.10.                 
#define CMP0MX_CMXP__CMP0P11 0x0B ///< External pin CMP0P.11.                 
#define CMP0MX_CMXP__CMP0P12 0x0C ///< External pin CMP0P.12.                 
#define CMP0MX_CMXP__CMP0P15 0x0F ///< External pin CMP0P.15.                 
                                                                              
#define CMP0MX_CMXN__FMASK   0xF0 ///< Comparator Negative Input MUX Selection
#define CMP0MX_CMXN__SHIFT   0x04 ///< Comparator Negative Input MUX Selection
#define CMP0MX_CMXN__CMP0N0  0x00 ///< External pin CMP0N.0.                  
#define CMP0MX_CMXN__CMP0N1  0x10 ///< External pin CMP0N.1.                  
#define CMP0MX_CMXN__CMP0N2  0x20 ///< External pin CMP0N.2.                  
#define CMP0MX_CMXN__CMP0N3  0x30 ///< External pin CMP0N.3.                  
#define CMP0MX_CMXN__CMP0N4  0x40 ///< External pin CMP0N.4.                  
#define CMP0MX_CMXN__CMP0N5  0x50 ///< External pin CMP0N.5.                  
#define CMP0MX_CMXN__CMP0N6  0x60 ///< External pin CMP0N.6.                  
#define CMP0MX_CMXN__CMP0N7  0x70 ///< External pin CMP0N.7.                  
#define CMP0MX_CMXN__CMP0N8  0x80 ///< External pin CMP0N.8.                  
#define CMP0MX_CMXN__CMP0N9  0x90 ///< External pin CMP0N.9.                  
#define CMP0MX_CMXN__CMP0N10 0xA0 ///< External pin CMP0N.10.                 
#define CMP0MX_CMXN__CMP0N11 0xB0 ///< External pin CMP0N.11.                 
#define CMP0MX_CMXN__CMP0N12 0xC0 ///< External pin CMP0N.12.                 
#define CMP0MX_CMXN__CMP0N15 0xF0 ///< External pin CMP0N.15.                 
                                                                              
//------------------------------------------------------------------------------
// CMP1CN0 Enums (Comparator 1 Control 0 @ 0xBF)
//------------------------------------------------------------------------------
#define CMP1CN0_CPHYN__FMASK                0x03 ///< Comparator Negative Hysteresis Control            
#define CMP1CN0_CPHYN__SHIFT                0x00 ///< Comparator Negative Hysteresis Control            
#define CMP1CN0_CPHYN__DISABLED             0x00 ///< Negative Hysteresis disabled.                     
#define CMP1CN0_CPHYN__ENABLED_MODE1        0x01 ///< Negative Hysteresis = Hysteresis 1.               
#define CMP1CN0_CPHYN__ENABLED_MODE2        0x02 ///< Negative Hysteresis = Hysteresis 2.               
#define CMP1CN0_CPHYN__ENABLED_MODE3        0x03 ///< Negative Hysteresis = Hysteresis 3 (Maximum).     
                                                                                                        
#define CMP1CN0_CPHYP__FMASK                0x0C ///< Comparator Positive Hysteresis Control            
#define CMP1CN0_CPHYP__SHIFT                0x02 ///< Comparator Positive Hysteresis Control            
#define CMP1CN0_CPHYP__DISABLED             0x00 ///< Positive Hysteresis disabled.                     
#define CMP1CN0_CPHYP__ENABLED_MODE1        0x04 ///< Positive Hysteresis = Hysteresis 1.               
#define CMP1CN0_CPHYP__ENABLED_MODE2        0x08 ///< Positive Hysteresis = Hysteresis 2.               
#define CMP1CN0_CPHYP__ENABLED_MODE3        0x0C ///< Positive Hysteresis = Hysteresis 3 (Maximum).     
                                                                                                        
#define CMP1CN0_CPFIF__BMASK                0x10 ///< Comparator Falling-Edge Flag                      
#define CMP1CN0_CPFIF__SHIFT                0x04 ///< Comparator Falling-Edge Flag                      
#define CMP1CN0_CPFIF__NOT_SET              0x00 ///< No comparator falling edge has occurred since this
                                                 ///< flag was last cleared.                            
#define CMP1CN0_CPFIF__FALLING_EDGE         0x10 ///< Comparator falling edge has occurred.             
                                                                                                        
#define CMP1CN0_CPRIF__BMASK                0x20 ///< Comparator Rising-Edge Flag                       
#define CMP1CN0_CPRIF__SHIFT                0x05 ///< Comparator Rising-Edge Flag                       
#define CMP1CN0_CPRIF__NOT_SET              0x00 ///< No comparator rising edge has occurred since this 
                                                 ///< flag was last cleared.                            
#define CMP1CN0_CPRIF__RISING_EDGE          0x20 ///< Comparator rising edge has occurred.              
                                                                                                        
#define CMP1CN0_CPOUT__BMASK                0x40 ///< Comparator Output State Flag                      
#define CMP1CN0_CPOUT__SHIFT                0x06 ///< Comparator Output State Flag                      
#define CMP1CN0_CPOUT__POS_LESS_THAN_NEG    0x00 ///< Voltage on CP1P < CP1N.                           
#define CMP1CN0_CPOUT__POS_GREATER_THAN_NEG 0x40 ///< Voltage on CP1P > CP1N.                           
                                                                                                        
#define CMP1CN0_CPEN__BMASK                 0x80 ///< Comparator Enable                                 
#define CMP1CN0_CPEN__SHIFT                 0x07 ///< Comparator Enable                                 
#define CMP1CN0_CPEN__DISABLED              0x00 ///< Comparator disabled.                              
#define CMP1CN0_CPEN__ENABLED               0x80 ///< Comparator enabled.                               
                                                                                                        
//------------------------------------------------------------------------------
// CMP1CN1 Enums (Comparator 1 Control 1 @ 0xAC)
//------------------------------------------------------------------------------
#define CMP1CN1_DACLVL__FMASK   0x3F ///< Internal Comparator DAC Reference Level           
#define CMP1CN1_DACLVL__SHIFT   0x00 ///< Internal Comparator DAC Reference Level           
                                                                                            
#define CMP1CN1_CPINH__BMASK    0x80 ///< Output Inhibit                                    
#define CMP1CN1_CPINH__SHIFT    0x07 ///< Output Inhibit                                    
#define CMP1CN1_CPINH__DISABLED 0x00 ///< The comparator output will always reflect the     
                                     ///< input conditions.                                 
#define CMP1CN1_CPINH__ENABLED  0x80 ///< The comparator output will hold state any time the
                                     ///< PCA CEX2 channel is low.                          
                                                                                            
//------------------------------------------------------------------------------
// CMP1MD Enums (Comparator 1 Mode @ 0xAB)
//------------------------------------------------------------------------------
#define CMP1MD_CPMD__FMASK              0x03 ///< Comparator Mode Select                            
#define CMP1MD_CPMD__SHIFT              0x00 ///< Comparator Mode Select                            
#define CMP1MD_CPMD__MODE0              0x00 ///< Mode 0 (Fastest Response Time, Highest Power      
                                             ///< Consumption)                                      
#define CMP1MD_CPMD__MODE1              0x01 ///< Mode 1                                            
#define CMP1MD_CPMD__MODE2              0x02 ///< Mode 2                                            
#define CMP1MD_CPMD__MODE3              0x03 ///< Mode 3 (Slowest Response Time, Lowest Power       
                                             ///< Consumption)                                      
                                                                                                    
#define CMP1MD_INSL__FMASK              0x0C ///< Comparator Input Selection                        
#define CMP1MD_INSL__SHIFT              0x02 ///< Comparator Input Selection                        
#define CMP1MD_INSL__CMXP_CMXN          0x00 ///< Connect the comparator inputs directly to the     
                                             ///< signals selected in the CMP1MX register. CMP+ is  
                                             ///< selected by CMXP and CMP- is selected by CMXN. The
                                             ///< internal DAC is not active.                       
#define CMP1MD_INSL__CMXP_GND           0x04 ///< Connect the CMP+ input to the signal selected by  
                                             ///< CMXP, and CMP- is connected to GND. The internal  
                                             ///< DAC is not active.                                
#define CMP1MD_INSL__DAC_CMXN           0x08 ///< Connect the CMP+ input to the internal DAC output,
                                             ///< and CMP- is selected by CMXN. The internal DAC    
                                             ///< uses the signal specified by CMXP as its full-    
                                             ///< scale reference.                                  
#define CMP1MD_INSL__CMXP_DAC           0x0C ///< Connect the CMP- input to the internal DAC output,
                                             ///< and CMP+ is selected by CMXP. The internal DAC    
                                             ///< uses the signal specified by CMXN as its full-    
                                             ///< scale reference.                                  
                                                                                                    
#define CMP1MD_CPFIE__BMASK             0x10 ///< Comparator Falling-Edge Interrupt Enable          
#define CMP1MD_CPFIE__SHIFT             0x04 ///< Comparator Falling-Edge Interrupt Enable          
#define CMP1MD_CPFIE__FALL_INT_DISABLED 0x00 ///< Comparator falling-edge interrupt disabled.       
#define CMP1MD_CPFIE__FALL_INT_ENABLED  0x10 ///< Comparator falling-edge interrupt enabled.        
                                                                                                    
#define CMP1MD_CPRIE__BMASK             0x20 ///< Comparator Rising-Edge Interrupt Enable           
#define CMP1MD_CPRIE__SHIFT             0x05 ///< Comparator Rising-Edge Interrupt Enable           
#define CMP1MD_CPRIE__RISE_INT_DISABLED 0x00 ///< Comparator rising-edge interrupt disabled.        
#define CMP1MD_CPRIE__RISE_INT_ENABLED  0x20 ///< Comparator rising-edge interrupt enabled.         
                                                                                                    
#define CMP1MD_CPINV__BMASK             0x40 ///< Output Inversion                                  
#define CMP1MD_CPINV__SHIFT             0x06 ///< Output Inversion                                  
#define CMP1MD_CPINV__NORMAL            0x00 ///< Output is not inverted.                           
#define CMP1MD_CPINV__INVERT            0x40 ///< Output is inverted.                               
                                                                                                    
#define CMP1MD_CPLOUT__BMASK            0x80 ///< Comparator Latched Output Flag                    
#define CMP1MD_CPLOUT__SHIFT            0x07 ///< Comparator Latched Output Flag                    
#define CMP1MD_CPLOUT__LOW              0x00 ///< Comparator output was logic low at last PCA       
                                             ///< overflow.                                         
#define CMP1MD_CPLOUT__HIGH             0x80 ///< Comparator output was logic high at last PCA      
                                             ///< overflow.                                         
                                                                                                    
//------------------------------------------------------------------------------
// CMP1MX Enums (Comparator 1 Multiplexer Selection @ 0xAA)
//------------------------------------------------------------------------------
#define CMP1MX_CMXP__FMASK   0x0F ///< Comparator Positive Input MUX Selection
#define CMP1MX_CMXP__SHIFT   0x00 ///< Comparator Positive Input MUX Selection
#define CMP1MX_CMXP__CMP1P0  0x00 ///< External pin CMP1P.0.                  
#define CMP1MX_CMXP__CMP1P1  0x01 ///< External pin CMP1P.1.                  
#define CMP1MX_CMXP__CMP1P2  0x02 ///< External pin CMP1P.2.                  
#define CMP1MX_CMXP__CMP1P3  0x03 ///< External pin CMP1P.3.                  
#define CMP1MX_CMXP__CMP1P4  0x04 ///< External pin CMP1P.4.                  
#define CMP1MX_CMXP__CMP1P5  0x05 ///< External pin CMP1P.5.                  
#define CMP1MX_CMXP__CMP1P6  0x06 ///< External pin CMP1P.6.                  
#define CMP1MX_CMXP__CMP1P7  0x07 ///< External pin CMP1P.7.                  
#define CMP1MX_CMXP__CMP1P8  0x08 ///< External pin CMP1P.8.                  
#define CMP1MX_CMXP__CMP1P9  0x09 ///< External pin CMP1P.9.                  
#define CMP1MX_CMXP__CMP1P10 0x0A ///< External pin CMP1P.10.                 
#define CMP1MX_CMXP__CMP1P11 0x0B ///< External pin CMP1P.11.                 
#define CMP1MX_CMXP__CMP1P12 0x0C ///< External pin CMP1P.12.                 
#define CMP1MX_CMXP__CMP1P15 0x0F ///< External pin CMP1P.15.                 
                                                                              
#define CMP1MX_CMXN__FMASK   0xF0 ///< Comparator Negative Input MUX Selection
#define CMP1MX_CMXN__SHIFT   0x04 ///< Comparator Negative Input MUX Selection
#define CMP1MX_CMXN__CMP1N0  0x00 ///< External pin CMP1N.0.                  
#define CMP1MX_CMXN__CMP1N1  0x10 ///< External pin CMP1N.1.                  
#define CMP1MX_CMXN__CMP1N2  0x20 ///< External pin CMP1N.2.                  
#define CMP1MX_CMXN__CMP1N3  0x30 ///< External pin CMP1N.3.                  
#define CMP1MX_CMXN__CMP1N4  0x40 ///< External pin CMP1N.4.                  
#define CMP1MX_CMXN__CMP1N5  0x50 ///< External pin CMP1N.5.                  
#define CMP1MX_CMXN__CMP1N6  0x60 ///< External pin CMP1N.6.                  
#define CMP1MX_CMXN__CMP1N7  0x70 ///< External pin CMP1N.7.                  
#define CMP1MX_CMXN__CMP1N8  0x80 ///< External pin CMP1N.8.                  
#define CMP1MX_CMXN__CMP1N9  0x90 ///< External pin CMP1N.9.                  
#define CMP1MX_CMXN__CMP1N10 0xA0 ///< External pin CMP1N.10.                 
#define CMP1MX_CMXN__CMP1N11 0xB0 ///< External pin CMP1N.11.                 
#define CMP1MX_CMXN__CMP1N12 0xC0 ///< External pin CMP1N.12.                 
#define CMP1MX_CMXN__CMP1N15 0xF0 ///< External pin CMP1N.15.                 
                                                                              
//------------------------------------------------------------------------------
// CRC0CN0 Enums (CRC0 Control 0 @ 0xCE)
//------------------------------------------------------------------------------
#define CRC0CN0_CRCPNT__BMASK        0x01 ///< CRC Result Pointer                               
#define CRC0CN0_CRCPNT__SHIFT        0x00 ///< CRC Result Pointer                               
#define CRC0CN0_CRCPNT__ACCESS_LOWER 0x00 ///< CRC0DAT accesses bits 7-0 of the 16-bit CRC      
                                          ///< result.                                          
#define CRC0CN0_CRCPNT__ACCESS_UPPER 0x01 ///< CRC0DAT accesses bits 15-8 of the 16-bit CRC     
                                          ///< result.                                          
                                                                                                
#define CRC0CN0_CRCVAL__BMASK        0x04 ///< CRC Initialization Value                         
#define CRC0CN0_CRCVAL__SHIFT        0x02 ///< CRC Initialization Value                         
#define CRC0CN0_CRCVAL__SET_ZEROES   0x00 ///< CRC result is set to 0x0000 on write of 1 to     
                                          ///< CRCINIT.                                         
#define CRC0CN0_CRCVAL__SET_ONES     0x04 ///< CRC result is set to 0xFFFF on write of 1 to     
                                          ///< CRCINIT.                                         
                                                                                                
#define CRC0CN0_CRCINIT__BMASK       0x08 ///< CRC Initialization Enable                        
#define CRC0CN0_CRCINIT__SHIFT       0x03 ///< CRC Initialization Enable                        
#define CRC0CN0_CRCINIT__DO_NOT_INIT 0x00 ///< Do not initialize the CRC result.                
#define CRC0CN0_CRCINIT__INIT        0x08 ///< Initialize the CRC result to ones or zeroes vased
                                          ///< on the value of CRCVAL.                          
                                                                                                
//------------------------------------------------------------------------------
// CRC0CN1 Enums (CRC0 Control 1 @ 0x86)
//------------------------------------------------------------------------------
#define CRC0CN1_CRCDN__BMASK     0x40 ///< Automatic CRC Calculation Complete        
#define CRC0CN1_CRCDN__SHIFT     0x06 ///< Automatic CRC Calculation Complete        
#define CRC0CN1_CRCDN__NOT_SET   0x00 ///< A CRC calculation is in progress.         
#define CRC0CN1_CRCDN__SET       0x40 ///< A CRC calculation is not in progress.     
                                                                                     
#define CRC0CN1_AUTOEN__BMASK    0x80 ///< Automatic CRC Calculation Enable          
#define CRC0CN1_AUTOEN__SHIFT    0x07 ///< Automatic CRC Calculation Enable          
#define CRC0CN1_AUTOEN__DISABLED 0x00 ///< Disable automatic CRC operations on flash.
#define CRC0CN1_AUTOEN__ENABLED  0x80 ///< Enable automatic CRC operations on flash. 
                                                                                     
//------------------------------------------------------------------------------
// CRC0CNT Enums (CRC0 Automatic Flash Sector Count @ 0xD3)
//------------------------------------------------------------------------------
#define CRC0CNT_CRCCNT__FMASK 0xFF ///< Automatic CRC Calculation Block Count
#define CRC0CNT_CRCCNT__SHIFT 0x00 ///< Automatic CRC Calculation Block Count
                                                                             
//------------------------------------------------------------------------------
// CRC0DAT Enums (CRC0 Data Output @ 0xCB)
//------------------------------------------------------------------------------
#define CRC0DAT_CRC0DAT__FMASK 0xFF ///< CRC Data Output
#define CRC0DAT_CRC0DAT__SHIFT 0x00 ///< CRC Data Output
                                                        
//------------------------------------------------------------------------------
// CRC0FLIP Enums (CRC0 Bit Flip @ 0xCF)
//------------------------------------------------------------------------------
#define CRC0FLIP_CRC0FLIP__FMASK 0xFF ///< CRC0 Bit Flip
#define CRC0FLIP_CRC0FLIP__SHIFT 0x00 ///< CRC0 Bit Flip
                                                        
//------------------------------------------------------------------------------
// CRC0IN Enums (CRC0 Data Input @ 0xCA)
//------------------------------------------------------------------------------
#define CRC0IN_CRC0IN__FMASK 0xFF ///< CRC Data Input
#define CRC0IN_CRC0IN__SHIFT 0x00 ///< CRC Data Input
                                                     
//------------------------------------------------------------------------------
// CRC0ST Enums (CRC0 Automatic Flash Sector Start @ 0xD2)
//------------------------------------------------------------------------------
#define CRC0ST_CRCST__FMASK 0xFF ///< Automatic CRC Calculation Starting Block
#define CRC0ST_CRCST__SHIFT 0x00 ///< Automatic CRC Calculation Starting Block
                                                                              
//------------------------------------------------------------------------------
// DACGCF0 Enums (DAC Global Configuration 0 @ 0x88)
//------------------------------------------------------------------------------
#define DACGCF0_D1SRC__FMASK       0x03 ///< DAC1 Data Source                                  
#define DACGCF0_D1SRC__SHIFT       0x00 ///< DAC1 Data Source                                  
#define DACGCF0_D1SRC__DAC1        0x00 ///< Input = DAC1H:DAC1L.                              
#define DACGCF0_D1SRC__DAC1_INVERT 0x01 ///< Input = Inverse of DAC1H:DAC1L (one's complement).
#define DACGCF0_D1SRC__DAC0        0x02 ///< Input = DAC0H:DAC0L.                              
#define DACGCF0_D1SRC__DAC0_INVERT 0x03 ///< Input = Inverse of DAC0H:DAC0L (one's complement).
                                                                                               
#define DACGCF0_D1AMEN__BMASK      0x04 ///< DAC1 Alternating Mode Enable                      
#define DACGCF0_D1AMEN__SHIFT      0x02 ///< DAC1 Alternating Mode Enable                      
#define DACGCF0_D1AMEN__NORMAL     0x00 ///< DAC1 always updates from the data source selected 
                                        ///< in D1SRC. This mode may be used with any trigger  
                                        ///< source.                                           
#define DACGCF0_D1AMEN__ALTERNATE  0x04 ///< DAC1 updates occur on the rising or falling edge  
                                        ///< of the trigger signal. On a falling edge, DAC1    
                                        ///< receives the DAC1H/L registers. On a rising edge, 
                                        ///< DAC1 receives the data source selected in D1SRC.  
                                        ///< *This mode may only be used with Configurable     
                                        ///< Logic trigger sources, and the selected trigger   
                                        ///< source must be high or low for two or more SYSCLK 
                                        ///< cycles.*                                          
                                                                                               
#define DACGCF0_D01REFSL__BMASK    0x08 ///< DAC0 and DAC1 Reference Voltage Select            
#define DACGCF0_D01REFSL__SHIFT    0x03 ///< DAC0 and DAC1 Reference Voltage Select            
#define DACGCF0_D01REFSL__VREF     0x00 ///< Select the VREF pin.                              
#define DACGCF0_D01REFSL__VDD      0x08 ///< Select the VDD supply.                            
                                                                                               
#define DACGCF0_D3SRC__FMASK       0x30 ///< DAC3 Data Source                                  
#define DACGCF0_D3SRC__SHIFT       0x04 ///< DAC3 Data Source                                  
#define DACGCF0_D3SRC__DAC3        0x00 ///< Input = DAC3H:DAC3L.                              
#define DACGCF0_D3SRC__DAC3_INVERT 0x10 ///< Input = Inverse of DAC3H:DAC3L (one's complement).
#define DACGCF0_D3SRC__DAC2        0x20 ///< Input = DAC2H:DAC2L.                              
#define DACGCF0_D3SRC__DAC2_INVERT 0x30 ///< Input = Inverse of DAC2H:DAC2L (one's complement).
                                                                                               
#define DACGCF0_D3AMEN__BMASK      0x40 ///< DAC3 Alternating Mode Enable                      
#define DACGCF0_D3AMEN__SHIFT      0x06 ///< DAC3 Alternating Mode Enable                      
#define DACGCF0_D3AMEN__NORMAL     0x00 ///< DAC3 always updates from the data source selected 
                                        ///< in D3SRC. This mode may be used with any trigger  
                                        ///< source.                                           
#define DACGCF0_D3AMEN__ALTERNATE  0x40 ///< DAC3 updates occur on the rising or falling edge  
                                        ///< of the trigger signal. On a falling edge, DAC3    
                                        ///< receives the DAC3H/L registers. On a rising edge, 
                                        ///< DAC3 receives the data source selected in D3SRC.  
                                        ///< *This mode may only be used with Configurable     
                                        ///< Logic trigger sources, and the selected trigger   
                                        ///< source must be high or low for two or more SYSCLK 
                                        ///< cycles.*                                          
                                                                                               
#define DACGCF0_D23REFSL__BMASK    0x80 ///< DAC2 and DAC3 Reference Voltage Select            
#define DACGCF0_D23REFSL__SHIFT    0x07 ///< DAC2 and DAC3 Reference Voltage Select            
#define DACGCF0_D23REFSL__VREF     0x00 ///< Select the VREF pin.                              
#define DACGCF0_D23REFSL__VDD      0x80 ///< Select the VDD supply.                            
                                                                                               
//------------------------------------------------------------------------------
// DACGCF1 Enums (DAC Global Configuration 1 @ 0x98)
//------------------------------------------------------------------------------
#define DACGCF1_D0UDIS__BMASK   0x01 ///< DAC0 Update Disable                  
#define DACGCF1_D0UDIS__SHIFT   0x00 ///< DAC0 Update Disable                  
#define DACGCF1_D0UDIS__ENABLE  0x00 ///< Allow triggers to update DAC0 output.
#define DACGCF1_D0UDIS__DISABLE 0x01 ///< Triggers will not update DAC0 output.
                                                                               
#define DACGCF1_D1UDIS__BMASK   0x02 ///< DAC1 Update Disable                  
#define DACGCF1_D1UDIS__SHIFT   0x01 ///< DAC1 Update Disable                  
#define DACGCF1_D1UDIS__ENABLE  0x00 ///< Allow triggers to update DAC1 output.
#define DACGCF1_D1UDIS__DISABLE 0x02 ///< Triggers will not update DAC1 output.
                                                                               
#define DACGCF1_D2UDIS__BMASK   0x04 ///< DAC2 Update Disable                  
#define DACGCF1_D2UDIS__SHIFT   0x02 ///< DAC2 Update Disable                  
#define DACGCF1_D2UDIS__ENABLE  0x00 ///< Allow triggers to update DAC2 output.
#define DACGCF1_D2UDIS__DISABLE 0x04 ///< Triggers will not update DAC2 output.
                                                                               
#define DACGCF1_D3UDIS__BMASK   0x08 ///< DAC3 Update Disable                  
#define DACGCF1_D3UDIS__SHIFT   0x03 ///< DAC3 Update Disable                  
#define DACGCF1_D3UDIS__ENABLE  0x00 ///< Allow triggers to update DAC3 output.
#define DACGCF1_D3UDIS__DISABLE 0x08 ///< Triggers will not update DAC3 output.
                                                                               
//------------------------------------------------------------------------------
// DACGCF2 Enums (DAC Global Configuration 2 @ 0xA2)
//------------------------------------------------------------------------------
#define DACGCF2_D01REFGN__FMASK     0x03 ///< DAC0/1 Reference Buffer Gain                     
#define DACGCF2_D01REFGN__SHIFT     0x00 ///< DAC0/1 Reference Buffer Gain                     
#define DACGCF2_D01REFGN__ATTEN_2P0 0x00 ///< Selected reference will be attenuated by a factor
                                         ///< of 2.                                            
#define DACGCF2_D01REFGN__ATTEN_2P4 0x01 ///< Selected reference will be attenuated by a factor
                                         ///< of 2.4 (Gain = 1/2.4).                           
#define DACGCF2_D01REFGN__ATTEN_3P0 0x02 ///< Selected reference will be attenuated by a factor
                                         ///< of 3 (Gain = 1/3).                               
                                                                                               
#define DACGCF2_D23REFGN__FMASK     0x30 ///< DAC2/3 Reference Buffer Gain                     
#define DACGCF2_D23REFGN__SHIFT     0x04 ///< DAC2/3 Reference Buffer Gain                     
#define DACGCF2_D23REFGN__ATTEN_2P0 0x00 ///< Selected reference will be attenuated by a factor
                                         ///< of 2.                                            
#define DACGCF2_D23REFGN__ATTEN_2P4 0x10 ///< Selected reference will be attenuated by a factor
                                         ///< of 2.4 (Gain = 1/2.4).                           
#define DACGCF2_D23REFGN__ATTEN_3P0 0x20 ///< Selected reference will be attenuated by a factor
                                         ///< of 3 (Gain = 1/3).                               
                                                                                               
//------------------------------------------------------------------------------
// DAC0CF0 Enums (DAC0 Configuration 0 @ 0x91)
//------------------------------------------------------------------------------
#define DAC0CF0_UPDATE__FMASK       0x07 ///< DAC0 Update Trigger Source                        
#define DAC0CF0_UPDATE__SHIFT       0x00 ///< DAC0 Update Trigger Source                        
#define DAC0CF0_UPDATE__SYSCLK      0x00 ///< DAC0 output updates occur on every clock cycle.   
#define DAC0CF0_UPDATE__TIMER3      0x01 ///< DAC0 output updates occur on Timer 3 high byte    
                                         ///< overflow.                                         
#define DAC0CF0_UPDATE__TIMER4      0x02 ///< DAC0 output updates occur on Timer 4 high byte    
                                         ///< overflow.                                         
#define DAC0CF0_UPDATE__TIMER5      0x03 ///< DAC0 output updates occur on Timer 5 high byte    
                                         ///< overflow.                                         
#define DAC0CF0_UPDATE__CLU0        0x04 ///< DAC0 output updates occur on Configurable Logic   
                                         ///< output 0 rising edge.                             
#define DAC0CF0_UPDATE__CLU1        0x05 ///< DAC0 output updates occur on Configurable Logic   
                                         ///< output 1 rising edge.                             
#define DAC0CF0_UPDATE__CLU2        0x06 ///< DAC0 output updates occur on Configurable Logic   
                                         ///< output 2 rising edge.                             
#define DAC0CF0_UPDATE__CLU3        0x07 ///< DAC0 output updates occur on Configurable Logic   
                                         ///< output 3 rising edge.                             
                                                                                                
#define DAC0CF0_LJST__BMASK         0x20 ///< DAC0 Left Justify Enable                          
#define DAC0CF0_LJST__SHIFT         0x05 ///< DAC0 Left Justify Enable                          
#define DAC0CF0_LJST__RIGHT_JUSTIFY 0x00 ///< DAC0 input is treated as right-justified.         
#define DAC0CF0_LJST__LEFT_JUSTIFY  0x20 ///< DAC0 input is treated as left-justified.          
                                                                                                
#define DAC0CF0_RSTMD__BMASK        0x40 ///< DAC0 Reset Mode                                   
#define DAC0CF0_RSTMD__SHIFT        0x06 ///< DAC0 Reset Mode                                   
#define DAC0CF0_RSTMD__NORMAL       0x00 ///< All resets will reset DAC0 and its associated     
                                         ///< registers.                                        
#define DAC0CF0_RSTMD__PERSIST      0x40 ///< DAC0 output will persist through all resets except
                                         ///< for power-on-resets.                              
                                                                                                
#define DAC0CF0_EN__BMASK           0x80 ///< DAC0 Enable                                       
#define DAC0CF0_EN__SHIFT           0x07 ///< DAC0 Enable                                       
#define DAC0CF0_EN__DISABLE         0x00 ///< DAC0 is disabled and not driven at the output pin.
#define DAC0CF0_EN__ENABLE          0x80 ///< DAC0 is enabled and will drive the output pin.    
                                                                                                
//------------------------------------------------------------------------------
// DAC0CF1 Enums (DAC0 Configuration 1 @ 0x92)
//------------------------------------------------------------------------------
#define DAC0CF1_DRVGAIN__FMASK    0x03 ///< DAC0 Output Buffer Gain
#define DAC0CF1_DRVGAIN__SHIFT    0x00 ///< DAC0 Output Buffer Gain
#define DAC0CF1_DRVGAIN__GAIN_2P0 0x00 ///< DAC output gain is 2.  
#define DAC0CF1_DRVGAIN__GAIN_2P4 0x01 ///< DAC output gain is 2.4.
#define DAC0CF1_DRVGAIN__GAIN_3P0 0x02 ///< DAC output gain is 3.  
                                                                   
//------------------------------------------------------------------------------
// DAC0H Enums (DAC0 Data Word High Byte @ 0x85)
//------------------------------------------------------------------------------
#define DAC0H_DAC0H__FMASK 0xFF ///< Data Word High Byte
#define DAC0H_DAC0H__SHIFT 0x00 ///< Data Word High Byte
                                                        
//------------------------------------------------------------------------------
// DAC0L Enums (DAC0 Data Word Low Byte @ 0x84)
//------------------------------------------------------------------------------
#define DAC0L_DAC0L__FMASK 0xFF ///< Data Word Low Byte
#define DAC0L_DAC0L__SHIFT 0x00 ///< Data Word Low Byte
                                                       
//------------------------------------------------------------------------------
// DAC1CF0 Enums (DAC1 Configuration 0 @ 0x93)
//------------------------------------------------------------------------------
#define DAC1CF0_UPDATE__FMASK       0x07 ///< DAC1 Update Trigger Source                        
#define DAC1CF0_UPDATE__SHIFT       0x00 ///< DAC1 Update Trigger Source                        
#define DAC1CF0_UPDATE__SYSCLK      0x00 ///< DAC1 output updates occur on every clock cycle.   
#define DAC1CF0_UPDATE__TIMER3      0x01 ///< DAC1 output updates occur on Timer 3 high byte    
                                         ///< overflow.                                         
#define DAC1CF0_UPDATE__TIMER4      0x02 ///< DAC1 output updates occur on Timer 4 high byte    
                                         ///< overflow.                                         
#define DAC1CF0_UPDATE__TIMER5      0x03 ///< DAC1 output updates occur on Timer 5 high byte    
                                         ///< overflow.                                         
#define DAC1CF0_UPDATE__CLU0        0x04 ///< DAC1 output updates occur on Configurable Logic   
                                         ///< output 0 rising edge.                             
#define DAC1CF0_UPDATE__CLU1        0x05 ///< DAC1 output updates occur on Configurable Logic   
                                         ///< output 1 rising edge.                             
#define DAC1CF0_UPDATE__CLU2        0x06 ///< DAC1 output updates occur on Configurable Logic   
                                         ///< output 2 rising edge.                             
#define DAC1CF0_UPDATE__CLU3        0x07 ///< DAC1 output updates occur on Configurable Logic   
                                         ///< output 3 rising edge.                             
                                                                                                
#define DAC1CF0_LJST__BMASK         0x20 ///< DAC1 Left Justify Enable                          
#define DAC1CF0_LJST__SHIFT         0x05 ///< DAC1 Left Justify Enable                          
#define DAC1CF0_LJST__RIGHT_JUSTIFY 0x00 ///< DAC1 input is treated as right-justified.         
#define DAC1CF0_LJST__LEFT_JUSTIFY  0x20 ///< DAC1 input is treated as left-justified.          
                                                                                                
#define DAC1CF0_RSTMD__BMASK        0x40 ///< DAC1 Reset Mode                                   
#define DAC1CF0_RSTMD__SHIFT        0x06 ///< DAC1 Reset Mode                                   
#define DAC1CF0_RSTMD__NORMAL       0x00 ///< All resets will reset DAC1 and its associated     
                                         ///< registers.                                        
#define DAC1CF0_RSTMD__PERSIST      0x40 ///< DAC1 output will persist through all resets except
                                         ///< for power-on-resets.                              
                                                                                                
#define DAC1CF0_EN__BMASK           0x80 ///< DAC1 Enable                                       
#define DAC1CF0_EN__SHIFT           0x07 ///< DAC1 Enable                                       
#define DAC1CF0_EN__DISABLE         0x00 ///< DAC1 is disabled and not driven at the output pin.
#define DAC1CF0_EN__ENABLE          0x80 ///< DAC1 is enabled and will drive the output pin.    
                                                                                                
//------------------------------------------------------------------------------
// DAC1CF1 Enums (DAC1 Configuration 1 @ 0x94)
//------------------------------------------------------------------------------
#define DAC1CF1_DRVGAIN__FMASK    0x03 ///< DAC1 Output Buffer Gain
#define DAC1CF1_DRVGAIN__SHIFT    0x00 ///< DAC1 Output Buffer Gain
#define DAC1CF1_DRVGAIN__GAIN_2P0 0x00 ///< DAC output gain is 2.  
#define DAC1CF1_DRVGAIN__GAIN_2P4 0x01 ///< DAC output gain is 2.4.
#define DAC1CF1_DRVGAIN__GAIN_3P0 0x02 ///< DAC output gain is 3.  
                                                                   
//------------------------------------------------------------------------------
// DAC1H Enums (DAC1 Data Word High Byte @ 0x8A)
//------------------------------------------------------------------------------
#define DAC1H_DAC1H__FMASK 0xFF ///< Data Word High Byte
#define DAC1H_DAC1H__SHIFT 0x00 ///< Data Word High Byte
                                                        
//------------------------------------------------------------------------------
// DAC1L Enums (DAC1 Data Word Low Byte @ 0x89)
//------------------------------------------------------------------------------
#define DAC1L_DAC1L__FMASK 0xFF ///< Data Word Low Byte
#define DAC1L_DAC1L__SHIFT 0x00 ///< Data Word Low Byte
                                                       
//------------------------------------------------------------------------------
// DAC2CF0 Enums (DAC2 Configuration 0 @ 0x95)
//------------------------------------------------------------------------------
#define DAC2CF0_UPDATE__FMASK       0x07 ///< DAC2 Update Trigger Source                        
#define DAC2CF0_UPDATE__SHIFT       0x00 ///< DAC2 Update Trigger Source                        
#define DAC2CF0_UPDATE__SYSCLK      0x00 ///< DAC2 output updates occur on every clock cycle.   
#define DAC2CF0_UPDATE__TIMER3      0x01 ///< DAC2 output updates occur on Timer 3 high byte    
                                         ///< overflow.                                         
#define DAC2CF0_UPDATE__TIMER4      0x02 ///< DAC2 output updates occur on Timer 4 high byte    
                                         ///< overflow.                                         
#define DAC2CF0_UPDATE__TIMER5      0x03 ///< DAC2 output updates occur on Timer 5 high byte    
                                         ///< overflow.                                         
#define DAC2CF0_UPDATE__CLU0        0x04 ///< DAC2 output updates occur on Configurable Logic   
                                         ///< output 0 rising edge.                             
#define DAC2CF0_UPDATE__CLU1        0x05 ///< DAC2 output updates occur on Configurable Logic   
                                         ///< output 1 rising edge.                             
#define DAC2CF0_UPDATE__CLU2        0x06 ///< DAC2 output updates occur on Configurable Logic   
                                         ///< output 2 rising edge.                             
#define DAC2CF0_UPDATE__CLU3        0x07 ///< DAC2 output updates occur on Configurable Logic   
                                         ///< output 3 rising edge.                             
                                                                                                
#define DAC2CF0_LJST__BMASK         0x20 ///< DAC2 Left Justify Enable                          
#define DAC2CF0_LJST__SHIFT         0x05 ///< DAC2 Left Justify Enable                          
#define DAC2CF0_LJST__RIGHT_JUSTIFY 0x00 ///< DAC2 input is treated as right-justified.         
#define DAC2CF0_LJST__LEFT_JUSTIFY  0x20 ///< DAC2 input is treated as left-justified.          
                                                                                                
#define DAC2CF0_RSTMD__BMASK        0x40 ///< DAC2 Reset Mode                                   
#define DAC2CF0_RSTMD__SHIFT        0x06 ///< DAC2 Reset Mode                                   
#define DAC2CF0_RSTMD__NORMAL       0x00 ///< All resets will reset DAC2 and its associated     
                                         ///< registers.                                        
#define DAC2CF0_RSTMD__PERSIST      0x40 ///< DAC2 output will persist through all resets except
                                         ///< for power-on-resets.                              
                                                                                                
#define DAC2CF0_EN__BMASK           0x80 ///< DAC2 Enable                                       
#define DAC2CF0_EN__SHIFT           0x07 ///< DAC2 Enable                                       
#define DAC2CF0_EN__DISABLE         0x00 ///< DAC2 is disabled and not driven at the output pin.
#define DAC2CF0_EN__ENABLE          0x80 ///< DAC2 is enabled and will drive the output pin.    
                                                                                                
//------------------------------------------------------------------------------
// DAC2CF1 Enums (DAC2 Configuration 1 @ 0x96)
//------------------------------------------------------------------------------
#define DAC2CF1_DRVGAIN__FMASK    0x03 ///< DAC2 Output Buffer Gain
#define DAC2CF1_DRVGAIN__SHIFT    0x00 ///< DAC2 Output Buffer Gain
#define DAC2CF1_DRVGAIN__GAIN_2P0 0x00 ///< DAC output gain is 2.  
#define DAC2CF1_DRVGAIN__GAIN_2P4 0x01 ///< DAC output gain is 2.4.
#define DAC2CF1_DRVGAIN__GAIN_3P0 0x02 ///< DAC output gain is 3.  
                                                                   
//------------------------------------------------------------------------------
// DAC2H Enums (DAC2 Data Word High Byte @ 0x8C)
//------------------------------------------------------------------------------
#define DAC2H_DAC2H__FMASK 0xFF ///< Data Word High Byte
#define DAC2H_DAC2H__SHIFT 0x00 ///< Data Word High Byte
                                                        
//------------------------------------------------------------------------------
// DAC2L Enums (DAC2 Data Word Low Byte @ 0x8B)
//------------------------------------------------------------------------------
#define DAC2L_DAC2L__FMASK 0xFF ///< Data Word Low Byte
#define DAC2L_DAC2L__SHIFT 0x00 ///< Data Word Low Byte
                                                       
//------------------------------------------------------------------------------
// DAC3CF0 Enums (DAC3 Configuration 0 @ 0x9A)
//------------------------------------------------------------------------------
#define DAC3CF0_UPDATE__FMASK       0x07 ///< DAC3 Update Trigger Source                        
#define DAC3CF0_UPDATE__SHIFT       0x00 ///< DAC3 Update Trigger Source                        
#define DAC3CF0_UPDATE__SYSCLK      0x00 ///< DAC3 output updates occur on every clock cycle.   
#define DAC3CF0_UPDATE__TIMER3      0x01 ///< DAC3 output updates occur on Timer 3 high byte    
                                         ///< overflow.                                         
#define DAC3CF0_UPDATE__TIMER4      0x02 ///< DAC3 output updates occur on Timer 4 high byte    
                                         ///< overflow.                                         
#define DAC3CF0_UPDATE__TIMER5      0x03 ///< DAC3 output updates occur on Timer 5 high byte    
                                         ///< overflow.                                         
#define DAC3CF0_UPDATE__CLU0        0x04 ///< DAC3 output updates occur on Configurable Logic   
                                         ///< output 0 rising edge.                             
#define DAC3CF0_UPDATE__CLU1        0x05 ///< DAC3 output updates occur on Configurable Logic   
                                         ///< output 1 rising edge.                             
#define DAC3CF0_UPDATE__CLU2        0x06 ///< DAC3 output updates occur on Configurable Logic   
                                         ///< output 2 rising edge.                             
#define DAC3CF0_UPDATE__CLU3        0x07 ///< DAC3 output updates occur on Configurable Logic   
                                         ///< output 3 rising edge.                             
                                                                                                
#define DAC3CF0_LJST__BMASK         0x20 ///< DAC3 Left Justify Enable                          
#define DAC3CF0_LJST__SHIFT         0x05 ///< DAC3 Left Justify Enable                          
#define DAC3CF0_LJST__RIGHT_JUSTIFY 0x00 ///< DAC3 input is treated as right-justified.         
#define DAC3CF0_LJST__LEFT_JUSTIFY  0x20 ///< DAC3 input is treated as left-justified.          
                                                                                                
#define DAC3CF0_RSTMD__BMASK        0x40 ///< DAC3 Reset Mode                                   
#define DAC3CF0_RSTMD__SHIFT        0x06 ///< DAC3 Reset Mode                                   
#define DAC3CF0_RSTMD__NORMAL       0x00 ///< All resets will reset DAC3 and its associated     
                                         ///< registers.                                        
#define DAC3CF0_RSTMD__PERSIST      0x40 ///< DAC3 output will persist through all resets except
                                         ///< for power-on-resets.                              
                                                                                                
#define DAC3CF0_EN__BMASK           0x80 ///< DAC3 Enable                                       
#define DAC3CF0_EN__SHIFT           0x07 ///< DAC3 Enable                                       
#define DAC3CF0_EN__DISABLE         0x00 ///< DAC3 is disabled and not driven at the output pin.
#define DAC3CF0_EN__ENABLE          0x80 ///< DAC3 is enabled and will drive the output pin.    
                                                                                                
//------------------------------------------------------------------------------
// DAC3CF1 Enums (DAC3 Configuration 1 @ 0x9C)
//------------------------------------------------------------------------------
#define DAC3CF1_DRVGAIN__FMASK    0x03 ///< DAC3 Output Buffer Gain
#define DAC3CF1_DRVGAIN__SHIFT    0x00 ///< DAC3 Output Buffer Gain
#define DAC3CF1_DRVGAIN__GAIN_2P0 0x00 ///< DAC output gain is 2.  
#define DAC3CF1_DRVGAIN__GAIN_2P4 0x01 ///< DAC output gain is 2.4.
#define DAC3CF1_DRVGAIN__GAIN_3P0 0x02 ///< DAC output gain is 3.  
                                                                   
//------------------------------------------------------------------------------
// DAC3H Enums (DAC3 Data Word High Byte @ 0x8E)
//------------------------------------------------------------------------------
#define DAC3H_DAC3H__FMASK 0xFF ///< Data Word High Byte
#define DAC3H_DAC3H__SHIFT 0x00 ///< Data Word High Byte
                                                        
//------------------------------------------------------------------------------
// DAC3L Enums (DAC3 Data Word Low Byte @ 0x8D)
//------------------------------------------------------------------------------
#define DAC3L_DAC3L__FMASK 0xFF ///< Data Word Low Byte
#define DAC3L_DAC3L__SHIFT 0x00 ///< Data Word Low Byte
                                                       
//------------------------------------------------------------------------------
// DERIVID Enums (Derivative Identification @ 0xAD)
//------------------------------------------------------------------------------
#define DERIVID_DERIVID__FMASK               0xFF ///< Derivative ID          
#define DERIVID_DERIVID__SHIFT               0x00 ///< Derivative ID          
#define DERIVID_DERIVID__EFM8BB31F64G_QFN32  0x01 ///< EFM8BB31F64G-{R}-QFN32 
#define DERIVID_DERIVID__EFM8BB31F64G_QFP32  0x02 ///< EFM8BB31F64G-{R}-QFP32 
#define DERIVID_DERIVID__EFM8BB31F64G_QSOP24 0x03 ///< EFM8BB31F64G-{R}-QSOP24
#define DERIVID_DERIVID__EFM8BB31F64G_QFN24  0x04 ///< EFM8BB31F64G-{R}-QFN24 
#define DERIVID_DERIVID__EFM8BB31F32G_QFN32  0x05 ///< EFM8BB31F32G-{R}-QFN32 
#define DERIVID_DERIVID__EFM8BB31F32G_QFP32  0x06 ///< EFM8BB31F32G-{R}-QFP32 
#define DERIVID_DERIVID__EFM8BB31F32G_QSOP24 0x07 ///< EFM8BB31F32G-{R}-QSOP24
#define DERIVID_DERIVID__EFM8BB31F32G_QFN24  0x08 ///< EFM8BB31F32G-{R}-QFN24 
#define DERIVID_DERIVID__EFM8BB31F16G_QFN32  0x09 ///< EFM8BB31F16G-{R}-QFN32 
#define DERIVID_DERIVID__EFM8BB31F16G_QFP32  0x0A ///< EFM8BB31F16G-{R}-QFP32 
#define DERIVID_DERIVID__EFM8BB31F16G_QSOP24 0x0B ///< EFM8BB31F16G-{R}-QSOP24
#define DERIVID_DERIVID__EFM8BB31F16G_QFN24  0x0C ///< EFM8BB31F16G-{R}-QFN24 
#define DERIVID_DERIVID__EFM8BB31F64I_QFN32  0x11 ///< EFM8BB31F64I-{R}-QFN32 
#define DERIVID_DERIVID__EFM8BB31F64I_QFP32  0x12 ///< EFM8BB31F64I-{R}-QFP32 
#define DERIVID_DERIVID__EFM8BB31F64I_QSOP24 0x13 ///< EFM8BB31F64I-{R}-QSOP24
#define DERIVID_DERIVID__EFM8BB31F64I_QFN24  0x14 ///< EFM8BB31F64I-{R}-QFN24 
#define DERIVID_DERIVID__EFM8BB31F32I_QFN32  0x15 ///< EFM8BB31F32I-{R}-QFN32 
#define DERIVID_DERIVID__EFM8BB31F32I_QFP32  0x16 ///< EFM8BB31F32I-{R}-QFP32 
#define DERIVID_DERIVID__EFM8BB31F32I_QSOP24 0x17 ///< EFM8BB31F32I-{R}-QSOP24
#define DERIVID_DERIVID__EFM8BB31F32I_QFN24  0x18 ///< EFM8BB31F32I-{R}-QFN24 
#define DERIVID_DERIVID__EFM8BB31F16I_QFN32  0x19 ///< EFM8BB31F16I-{R}-QFN32 
#define DERIVID_DERIVID__EFM8BB31F16I_QFP32  0x1A ///< EFM8BB31F16I-{R}-QFP32 
#define DERIVID_DERIVID__EFM8BB31F16I_QSOP24 0x1B ///< EFM8BB31F16I-{R}-QSOP24
#define DERIVID_DERIVID__EFM8BB31F16I_QFN24  0x1C ///< EFM8BB31F16I-{R}-QFN24 
#define DERIVID_DERIVID__EFM8BB31F64A_QFN32  0x21 ///< EFM8BB31F64A-{R}-QFN32 
#define DERIVID_DERIVID__EFM8BB31F64A_QFP32  0x22 ///< EFM8BB31F64A-{R}-QFP32 
#define DERIVID_DERIVID__EFM8BB31F64A_QSOP24 0x23 ///< EFM8BB31F64A-{R}-QSOP24
#define DERIVID_DERIVID__EFM8BB31F64A_QFN24  0x24 ///< EFM8BB31F64A-{R}-QFN24 
#define DERIVID_DERIVID__EFM8BB31F32A_QFN32  0x25 ///< EFM8BB31F32A-{R}-QFN32 
#define DERIVID_DERIVID__EFM8BB31F32A_QFP32  0x26 ///< EFM8BB31F32A-{R}-QFP32 
#define DERIVID_DERIVID__EFM8BB31F32A_QSOP24 0x27 ///< EFM8BB31F32A-{R}-QSOP24
#define DERIVID_DERIVID__EFM8BB31F32A_QFN24  0x28 ///< EFM8BB31F32A-{R}-QFN24 
#define DERIVID_DERIVID__EFM8BB31F16A_QFN32  0x29 ///< EFM8BB31F16A-{R}-QFN32 
#define DERIVID_DERIVID__EFM8BB31F16A_QFP32  0x2A ///< EFM8BB31F16A-{R}-QFP32 
#define DERIVID_DERIVID__EFM8BB31F16A_QSOP24 0x2B ///< EFM8BB31F16A-{R}-QSOP24
#define DERIVID_DERIVID__EFM8BB31F16A_QFN24  0x2C ///< EFM8BB31F16A-{R}-QFN24 
                                                                              
//------------------------------------------------------------------------------
// DEVICEID Enums (Device Identification @ 0xB5)
//------------------------------------------------------------------------------
#define DEVICEID_DEVICEID__FMASK 0xFF ///< Device ID
#define DEVICEID_DEVICEID__SHIFT 0x00 ///< Device ID
                                                    
//------------------------------------------------------------------------------
// REVID Enums (Revision Identifcation @ 0xB6)
//------------------------------------------------------------------------------
#define REVID_REVID__FMASK 0xFF ///< Revision ID
#define REVID_REVID__SHIFT 0x00 ///< Revision ID
#define REVID_REVID__REV_A 0x00 ///< Revision A.
                                                
//------------------------------------------------------------------------------
// IT01CF Enums (INT0/INT1 Configuration @ 0xE4)
//------------------------------------------------------------------------------
#define IT01CF_IN0SL__FMASK       0x07 ///< INT0 Port Pin Selection   
#define IT01CF_IN0SL__SHIFT       0x00 ///< INT0 Port Pin Selection   
#define IT01CF_IN0SL__P0_0        0x00 ///< Select P0.0.              
#define IT01CF_IN0SL__P0_1        0x01 ///< Select P0.1.              
#define IT01CF_IN0SL__P0_2        0x02 ///< Select P0.2.              
#define IT01CF_IN0SL__P0_3        0x03 ///< Select P0.3.              
#define IT01CF_IN0SL__P0_4        0x04 ///< Select P0.4.              
#define IT01CF_IN0SL__P0_5        0x05 ///< Select P0.5.              
#define IT01CF_IN0SL__P0_6        0x06 ///< Select P0.6.              
#define IT01CF_IN0SL__P0_7        0x07 ///< Select P0.7.              
                                                                      
#define IT01CF_IN0PL__BMASK       0x08 ///< INT0 Polarity             
#define IT01CF_IN0PL__SHIFT       0x03 ///< INT0 Polarity             
#define IT01CF_IN0PL__ACTIVE_LOW  0x00 ///< INT0 input is active low. 
#define IT01CF_IN0PL__ACTIVE_HIGH 0x08 ///< INT0 input is active high.
                                                                      
#define IT01CF_IN1SL__FMASK       0x70 ///< INT1 Port Pin Selection   
#define IT01CF_IN1SL__SHIFT       0x04 ///< INT1 Port Pin Selection   
#define IT01CF_IN1SL__P0_0        0x00 ///< Select P0.0.              
#define IT01CF_IN1SL__P0_1        0x10 ///< Select P0.1.              
#define IT01CF_IN1SL__P0_2        0x20 ///< Select P0.2.              
#define IT01CF_IN1SL__P0_3        0x30 ///< Select P0.3.              
#define IT01CF_IN1SL__P0_4        0x40 ///< Select P0.4.              
#define IT01CF_IN1SL__P0_5        0x50 ///< Select P0.5.              
#define IT01CF_IN1SL__P0_6        0x60 ///< Select P0.6.              
#define IT01CF_IN1SL__P0_7        0x70 ///< Select P0.7.              
                                                                      
#define IT01CF_IN1PL__BMASK       0x80 ///< INT1 Polarity             
#define IT01CF_IN1PL__SHIFT       0x07 ///< INT1 Polarity             
#define IT01CF_IN1PL__ACTIVE_LOW  0x00 ///< INT1 input is active low. 
#define IT01CF_IN1PL__ACTIVE_HIGH 0x80 ///< INT1 input is active high.
                                                                      
//------------------------------------------------------------------------------
// XOSC0CN Enums (External Oscillator Control @ 0x86)
//------------------------------------------------------------------------------
#define XOSC0CN_XFCN__FMASK           0x07 ///< External Oscillator Frequency Control           
#define XOSC0CN_XFCN__SHIFT           0x00 ///< External Oscillator Frequency Control           
#define XOSC0CN_XFCN__MODE0           0x00 ///< Select external oscillator mode 0: Crystal      
                                           ///< frequency <= 20 kHz, RC/C frequency <= 25 kHz, C
                                           ///< mode K factor = 0.87.                           
#define XOSC0CN_XFCN__MODE1           0x01 ///< Select external oscillator mode 1: 20 kHz <     
                                           ///< Crystal frequency <= 58 kHz, 25 kHz < RC/C      
                                           ///< frequency <= 50 kHz, C mode K factor = 2.6.     
#define XOSC0CN_XFCN__MODE2           0x02 ///< Select external oscillator mode 2: 58 kHz <     
                                           ///< Crystal frequency <= 155 kHz, 50 kHz < RC/C     
                                           ///< frequency <= 100 kHz, C mode K factor = 7.7.    
#define XOSC0CN_XFCN__MODE3           0x03 ///< Select external oscillator mode 3: 155 kHz <    
                                           ///< Crystal frequency <= 415 kHz, 100 kHz < RC/C    
                                           ///< frequency <= 200 kHz, C mode K factor = 22.     
#define XOSC0CN_XFCN__MODE4           0x04 ///< Select external oscillator mode 4: 415 kHz <    
                                           ///< Crystal frequency <= 1.1 MHz, 200 kHz < RC/C    
                                           ///< frequency <= 400 kHz, C mode K factor = 65.     
#define XOSC0CN_XFCN__MODE5           0x05 ///< Select external oscillator mode 5: 1.1 MHz <    
                                           ///< Crystal frequency <= 3.1 MHz, 400 kHz < RC/C    
                                           ///< frequency <= 800 kHz, C mode K factor = 180.    
#define XOSC0CN_XFCN__MODE6           0x06 ///< Select external oscillator mode 6: 3.1 MHz <    
                                           ///< Crystal frequency <= 8.2 kHz, 800 kHz < RC/C    
                                           ///< frequency <= 1.6 MHz, C mode K factor = 664.    
#define XOSC0CN_XFCN__MODE7           0x07 ///< Select external oscillator mode 7: 8.2 MHz <    
                                           ///< Crystal frequency <= 25 MHz, 1.6 MHz < RC/C     
                                           ///< frequency <= 3.2 MHz, C mode K factor = 1590.   
                                                                                                
#define XOSC0CN_XOSCMD__FMASK         0x70 ///< External Oscillator Mode                        
#define XOSC0CN_XOSCMD__SHIFT         0x04 ///< External Oscillator Mode                        
#define XOSC0CN_XOSCMD__DISABLED      0x00 ///< External Oscillator circuit disabled.           
#define XOSC0CN_XOSCMD__CMOS          0x20 ///< External CMOS Clock Mode.                       
#define XOSC0CN_XOSCMD__CMOS_DIV_2    0x30 ///< External CMOS Clock Mode with divide by 2 stage.
#define XOSC0CN_XOSCMD__RC            0x40 ///< RC Oscillator Mode.                             
#define XOSC0CN_XOSCMD__CRYSTAL       0x60 ///< Crystal Oscillator Mode                         
#define XOSC0CN_XOSCMD__CRYSTAL_DIV_2 0x70 ///< Crystal Oscillator Mode with divide by 2 stage  
                                                                                                
#define XOSC0CN_XCLKVLD__BMASK        0x80 ///< External Oscillator Valid Flag                  
#define XOSC0CN_XCLKVLD__SHIFT        0x07 ///< External Oscillator Valid Flag                  
#define XOSC0CN_XCLKVLD__NOT_SET      0x00 ///< External Oscillator is unused or not yet stable.
#define XOSC0CN_XCLKVLD__SET          0x80 ///< External Oscillator is running and stable.      
                                                                                                
//------------------------------------------------------------------------------
// FLKEY Enums (Flash Lock and Key @ 0xB7)
//------------------------------------------------------------------------------
#define FLKEY_FLKEY__FMASK    0xFF ///< Flash Lock and Key                             
#define FLKEY_FLKEY__SHIFT    0x00 ///< Flash Lock and Key                             
#define FLKEY_FLKEY__LOCKED   0x00 ///< Flash is write/erase locked.                   
#define FLKEY_FLKEY__FIRST    0x01 ///< The first key code has been written (0xA5).    
#define FLKEY_FLKEY__UNLOCKED 0x02 ///< Flash is unlocked (writes/erases allowed).     
#define FLKEY_FLKEY__DISABLED 0x03 ///< Flash writes/erases are disabled until the next
                                   ///< reset.                                         
#define FLKEY_FLKEY__KEY1     0xA5 ///< Flash writes and erases are enabled by writing 
                                   ///< 0xA5 followed by 0xF1 to the FLKEY register.   
#define FLKEY_FLKEY__KEY2     0xF1 ///< Flash writes and erases are enabled by writing 
                                   ///< 0xA5 followed by 0xF1 to the FLKEY register.   
                                                                                       
//------------------------------------------------------------------------------
// PSCTL Enums (Program Store Control @ 0x8F)
//------------------------------------------------------------------------------
#define PSCTL_PSWE__BMASK          0x01 ///< Program Store Write Enable                      
#define PSCTL_PSWE__SHIFT          0x00 ///< Program Store Write Enable                      
#define PSCTL_PSWE__WRITE_DISABLED 0x00 ///< Writes to flash program memory disabled.        
#define PSCTL_PSWE__WRITE_ENABLED  0x01 ///< Writes to flash program memory enabled; the MOVX
                                        ///< write instruction targets flash memory.         
                                                                                             
#define PSCTL_PSEE__BMASK          0x02 ///< Program Store Erase Enable                      
#define PSCTL_PSEE__SHIFT          0x01 ///< Program Store Erase Enable                      
#define PSCTL_PSEE__ERASE_DISABLED 0x00 ///< Flash program memory erasure disabled.          
#define PSCTL_PSEE__ERASE_ENABLED  0x02 ///< Flash program memory erasure enabled.           
                                                                                             
//------------------------------------------------------------------------------
// HFO0CAL Enums (High Frequency Oscillator 0 Calibration @ 0xC7)
//------------------------------------------------------------------------------
#define HFO0CAL_HFO0CAL__FMASK 0xFF ///< Oscillator Calibration
#define HFO0CAL_HFO0CAL__SHIFT 0x00 ///< Oscillator Calibration
                                                               
//------------------------------------------------------------------------------
// HFO1CAL Enums (High Frequency Oscillator 1 Calibration @ 0xD6)
//------------------------------------------------------------------------------
#define HFO1CAL_HFO1CAL__FMASK 0x7F ///< Oscillator Calibration
#define HFO1CAL_HFO1CAL__SHIFT 0x00 ///< Oscillator Calibration
                                                               
//------------------------------------------------------------------------------
// HFOCN Enums (High Frequency Oscillator Control @ 0xEF)
//------------------------------------------------------------------------------
#define HFOCN_HFO0EN__BMASK    0x08 ///< HFOSC0 Oscillator Enable                        
#define HFOCN_HFO0EN__SHIFT    0x03 ///< HFOSC0 Oscillator Enable                        
#define HFOCN_HFO0EN__DISABLED 0x00 ///< Disable High Frequency Oscillator 0 (HFOSC0 will
                                    ///< still turn on if requested by any block in the  
                                    ///< device or selected as the SYSCLK source).       
#define HFOCN_HFO0EN__ENABLED  0x08 ///< Force High Frequency Oscillator 0 to run.       
                                                                                         
#define HFOCN_HFO1EN__BMASK    0x80 ///< HFOSC1 Oscillator Enable                        
#define HFOCN_HFO1EN__SHIFT    0x07 ///< HFOSC1 Oscillator Enable                        
#define HFOCN_HFO1EN__DISABLED 0x00 ///< Disable High Frequency Oscillator 1 (HFOSC1 will
                                    ///< still turn on if requested by any block in the  
                                    ///< device or selected as the SYSCLK source).       
#define HFOCN_HFO1EN__ENABLED  0x80 ///< Force High Frequency Oscillator 1 to run.       
                                                                                         
//------------------------------------------------------------------------------
// I2C0ADM Enums (I2C0 Slave Address Mask @ 0xFF)
//------------------------------------------------------------------------------
#define I2C0ADM_SLVM__FMASK              0x7F ///< I2C Hardware Slave Address                       
#define I2C0ADM_SLVM__SHIFT              0x00 ///< I2C Hardware Slave Address                       
                                                                                                    
#define I2C0ADM_FACS__BMASK              0x80 ///< Force Address Clock Stretching                   
#define I2C0ADM_FACS__SHIFT              0x07 ///< Force Address Clock Stretching                   
#define I2C0ADM_FACS__DONT_FORCE_STRETCH 0x00 ///< The I2C0INT bit is not set by acking the slave   
                                              ///< address alone. Additional conditions are required
                                              ///< to set I2C0INT.                                  
#define I2C0ADM_FACS__FORCE_STRETCH      0x80 ///< The I2C0INT bit is always set when matching slave
                                              ///< address is acknowledged. This will force clock   
                                              ///< stretching until firmware clears the I2C0INT bit.
                                                                                                    
//------------------------------------------------------------------------------
// I2C0CN0 Enums (I2C0 Control @ 0xBA)
//------------------------------------------------------------------------------
#define I2C0CN0_BUSY__BMASK        0x01 ///< Busy                                              
#define I2C0CN0_BUSY__SHIFT        0x00 ///< Busy                                              
#define I2C0CN0_BUSY__NOT_SET      0x00 ///< Device will acknowledge an I2C master.            
#define I2C0CN0_BUSY__SET          0x01 ///< Device will not respond to an I2C master. All I2C 
                                        ///< data sent to the device will be NACKed.           
                                                                                               
#define I2C0CN0_I2C0EN__BMASK      0x02 ///< I2C Enable                                        
#define I2C0CN0_I2C0EN__SHIFT      0x01 ///< I2C Enable                                        
#define I2C0CN0_I2C0EN__DISABLED   0x00 ///< Disable the I2C0 Slave module.                    
#define I2C0CN0_I2C0EN__ENABLED    0x02 ///< Enable the I2C0 Slave module.                     
                                                                                               
#define I2C0CN0_PRELOAD__BMASK     0x04 ///< Preload Disable                                   
#define I2C0CN0_PRELOAD__SHIFT     0x02 ///< Preload Disable                                   
#define I2C0CN0_PRELOAD__ENABLED   0x00 ///< Data bytes must be written into the TX FIFO via   
                                        ///< the I2C0DOUT register before the 8th SCL clock of 
                                        ///< the matching slave address byte transfer arrives  
                                        ///< for an I2C read operation.                        
#define I2C0CN0_PRELOAD__DISABLED  0x04 ///< Data bytes need not be preloaded for I2C read     
                                        ///< operations. The data byte can be written to       
                                        ///< I2C0DOUT during interrupt servicing.              
                                                                                               
#define I2C0CN0_TIMEOUT__BMASK     0x08 ///< SCL Low Timeout Enable                            
#define I2C0CN0_TIMEOUT__SHIFT     0x03 ///< SCL Low Timeout Enable                            
#define I2C0CN0_TIMEOUT__DISABLED  0x00 ///< Disable I2C SCL low timeout detection using Timer 
                                        ///< 4.                                                
#define I2C0CN0_TIMEOUT__ENABLED   0x08 ///< Enable I2C SCL low timeout detection using Timer 4
                                        ///< if Timer 4 RLFSEL is set appropriately.           
                                                                                               
#define I2C0CN0_PINMD__BMASK       0x10 ///< Pin Mode Enable                                   
#define I2C0CN0_PINMD__SHIFT       0x04 ///< Pin Mode Enable                                   
#define I2C0CN0_PINMD__GPIO_MODE   0x00 ///< Set the I2C0 Slave pins in GPIO mode.             
#define I2C0CN0_PINMD__I2C_MODE    0x10 ///< Set the I2C0 Slave pins in I2C mode.              
                                                                                               
#define I2C0CN0_PINDRV__BMASK      0x20 ///< Pin Drive Strength                                
#define I2C0CN0_PINDRV__SHIFT      0x05 ///< Pin Drive Strength                                
#define I2C0CN0_PINDRV__LOW_DRIVE  0x00 ///< SDA and SCL will use low drive strength.          
#define I2C0CN0_PINDRV__HIGH_DRIVE 0x20 ///< SDA and SCL will use high drive strength.         
                                                                                               
#define I2C0CN0_ADDRCHK__BMASK     0x40 ///< Address Check Enable                              
#define I2C0CN0_ADDRCHK__SHIFT     0x06 ///< Address Check Enable                              
#define I2C0CN0_ADDRCHK__DISABLED  0x00 ///< The matching slave address is not copied into the 
                                        ///< receive FIFO.                                     
#define I2C0CN0_ADDRCHK__ENABLED   0x40 ///< The matching slave address is copied into the     
                                        ///< receive FIFO.                                     
                                                                                               
//------------------------------------------------------------------------------
// I2C0DIN Enums (I2C0 Received Data @ 0xBC)
//------------------------------------------------------------------------------
#define I2C0DIN_I2C0DIN__FMASK 0xFF ///< I2C0 Received Data
#define I2C0DIN_I2C0DIN__SHIFT 0x00 ///< I2C0 Received Data
                                                           
//------------------------------------------------------------------------------
// I2C0DOUT Enums (I2C0 Transmit Data @ 0xBB)
//------------------------------------------------------------------------------
#define I2C0DOUT_I2C0DOUT__FMASK 0xFF ///< I2C0 Transmit Data
#define I2C0DOUT_I2C0DOUT__SHIFT 0x00 ///< I2C0 Transmit Data
                                                             
//------------------------------------------------------------------------------
// I2C0FCN0 Enums (I2C0 FIFO Control 0 @ 0xAD)
//------------------------------------------------------------------------------
#define I2C0FCN0_RXTH__FMASK     0x03 ///< RX FIFO Threshold                                 
#define I2C0FCN0_RXTH__SHIFT     0x00 ///< RX FIFO Threshold                                 
#define I2C0FCN0_RXTH__ZERO      0x00 ///< RFRQ will be set anytime new data arrives in the  
                                      ///< RX FIFO (when the RX FIFO is not empty).          
#define I2C0FCN0_RXTH__ONE       0x01 ///< RFRQ will be set if the RX FIFO contains more than
                                      ///< one byte.                                         
                                                                                             
#define I2C0FCN0_RFLSH__BMASK    0x04 ///< RX FIFO Flush                                     
#define I2C0FCN0_RFLSH__SHIFT    0x02 ///< RX FIFO Flush                                     
#define I2C0FCN0_RFLSH__FLUSH    0x04 ///< Initiate an RX FIFO flush.                        
                                                                                             
#define I2C0FCN0_RFRQE__BMASK    0x08 ///< Read Request Interrupt Enable                     
#define I2C0FCN0_RFRQE__SHIFT    0x03 ///< Read Request Interrupt Enable                     
#define I2C0FCN0_RFRQE__DISABLED 0x00 ///< I2C0 interrupts will not be generated when RFRQ is
                                      ///< set.                                              
#define I2C0FCN0_RFRQE__ENABLED  0x08 ///< I2C0 interrupts will be generated if RFRQ is set. 
                                                                                             
#define I2C0FCN0_TXTH__FMASK     0x30 ///< TX FIFO Threshold                                 
#define I2C0FCN0_TXTH__SHIFT     0x04 ///< TX FIFO Threshold                                 
#define I2C0FCN0_TXTH__ZERO      0x00 ///< TFRQ will be set when the TX FIFO is empty.       
#define I2C0FCN0_TXTH__ONE       0x10 ///< TFRQ will be set when the TX FIFO contains one or 
                                      ///< fewer bytes.                                      
                                                                                             
#define I2C0FCN0_TFLSH__BMASK    0x40 ///< TX FIFO Flush                                     
#define I2C0FCN0_TFLSH__SHIFT    0x06 ///< TX FIFO Flush                                     
#define I2C0FCN0_TFLSH__FLUSH    0x40 ///< Initiate a TX FIFO flush.                         
                                                                                             
#define I2C0FCN0_TFRQE__BMASK    0x80 ///< Write Request Interrupt Enable                    
#define I2C0FCN0_TFRQE__SHIFT    0x07 ///< Write Request Interrupt Enable                    
#define I2C0FCN0_TFRQE__DISABLED 0x00 ///< I2C0 interrupts will not be generated when TFRQ is
                                      ///< set.                                              
#define I2C0FCN0_TFRQE__ENABLED  0x80 ///< I2C0 interrupts will be generated if TFRQ is set. 
                                                                                             
//------------------------------------------------------------------------------
// I2C0FCN1 Enums (I2C0 FIFO Control 1 @ 0xAB)
//------------------------------------------------------------------------------
#define I2C0FCN1_RXE__BMASK     0x04 ///< RX FIFO Empty                                     
#define I2C0FCN1_RXE__SHIFT     0x02 ///< RX FIFO Empty                                     
#define I2C0FCN1_RXE__NOT_EMPTY 0x00 ///< The RX FIFO contains data.                        
#define I2C0FCN1_RXE__EMPTY     0x04 ///< The RX FIFO is empty.                             
                                                                                            
#define I2C0FCN1_RFRQ__BMASK    0x08 ///< Receive FIFO Request                              
#define I2C0FCN1_RFRQ__SHIFT    0x03 ///< Receive FIFO Request                              
#define I2C0FCN1_RFRQ__NOT_SET  0x00 ///< The number of bytes in the RX FIFO is less than or
                                     ///< equal to RXTH.                                    
#define I2C0FCN1_RFRQ__SET      0x08 ///< The number of bytes in the RX FIFO is greater than
                                     ///< RXTH.                                             
                                                                                            
#define I2C0FCN1_TXNF__BMASK    0x40 ///< TX FIFO Not Full                                  
#define I2C0FCN1_TXNF__SHIFT    0x06 ///< TX FIFO Not Full                                  
#define I2C0FCN1_TXNF__FULL     0x00 ///< The TX FIFO is full.                              
#define I2C0FCN1_TXNF__NOT_FULL 0x40 ///< The TX FIFO has room for more data.               
                                                                                            
#define I2C0FCN1_TFRQ__BMASK    0x80 ///< Transmit FIFO Request                             
#define I2C0FCN1_TFRQ__SHIFT    0x07 ///< Transmit FIFO Request                             
#define I2C0FCN1_TFRQ__NOT_SET  0x00 ///< The number of bytes in the TX FIFO is greater than
                                     ///< TXTH.                                             
#define I2C0FCN1_TFRQ__SET      0x80 ///< The number of bytes in the TX FIFO is less than or
                                     ///< equal to TXTH.                                    
                                                                                            
//------------------------------------------------------------------------------
// I2C0FCT Enums (I2C0 FIFO Count @ 0xF5)
//------------------------------------------------------------------------------
#define I2C0FCT_RXCNT__FMASK 0x07 ///< RX FIFO Count
#define I2C0FCT_RXCNT__SHIFT 0x00 ///< RX FIFO Count
                                                    
#define I2C0FCT_TXCNT__FMASK 0x70 ///< TX FIFO Count
#define I2C0FCT_TXCNT__SHIFT 0x04 ///< TX FIFO Count
                                                    
//------------------------------------------------------------------------------
// I2C0SLAD Enums (I2C0 Slave Address @ 0xBD)
//------------------------------------------------------------------------------
#define I2C0SLAD_I2C0SLAD__FMASK 0x7F ///< I2C Hardware Slave Address
#define I2C0SLAD_I2C0SLAD__SHIFT 0x00 ///< I2C Hardware Slave Address
                                                                     
//------------------------------------------------------------------------------
// I2C0STAT Enums (I2C0 Status @ 0xB9)
//------------------------------------------------------------------------------
#define I2C0STAT_RD__BMASK        0x01 ///< I2C Read                                        
#define I2C0STAT_RD__SHIFT        0x00 ///< I2C Read                                        
#define I2C0STAT_RD__NOT_SET      0x00 ///< An I2C Read operation is not in progress.       
#define I2C0STAT_RD__SET          0x01 ///< An I2C Read operation is in progress.           
                                                                                            
#define I2C0STAT_WR__BMASK        0x02 ///< I2C Write                                       
#define I2C0STAT_WR__SHIFT        0x01 ///< I2C Write                                       
#define I2C0STAT_WR__NOT_SET      0x00 ///< An I2C Write operation is not in progress.      
#define I2C0STAT_WR__SET          0x02 ///< An I2C Write operation is in progress.          
                                                                                            
#define I2C0STAT_STOP__BMASK      0x04 ///< Stop                                            
#define I2C0STAT_STOP__SHIFT      0x02 ///< Stop                                            
#define I2C0STAT_STOP__NOT_SET    0x00 ///< A STOP is not pending.                          
#define I2C0STAT_STOP__SET        0x04 ///< A STOP is currently pending for the I2C0 Slave. 
                                                                                            
#define I2C0STAT_START__BMASK     0x08 ///< Start                                           
#define I2C0STAT_START__SHIFT     0x03 ///< Start                                           
#define I2C0STAT_START__NOT_SET   0x00 ///< A START was not detected.                       
#define I2C0STAT_START__SET       0x08 ///< A START is currently pending for the I2C0 Slave.
                                                                                            
#define I2C0STAT_NACK__BMASK      0x10 ///< NACK                                            
#define I2C0STAT_NACK__SHIFT      0x04 ///< NACK                                            
#define I2C0STAT_NACK__NOT_SET    0x00 ///< A NACK did not occur.                           
#define I2C0STAT_NACK__SET        0x10 ///< A NACK occurred.                                
                                                                                            
#define I2C0STAT_I2C0INT__BMASK   0x20 ///< I2C Interrupt                                   
#define I2C0STAT_I2C0INT__SHIFT   0x05 ///< I2C Interrupt                                   
#define I2C0STAT_I2C0INT__NOT_SET 0x00 ///< An interrupt did not occur.                     
#define I2C0STAT_I2C0INT__SET     0x20 ///< An interrupt occurred.                          
                                                                                            
#define I2C0STAT_ACTIVE__BMASK    0x40 ///< Bus Active                                      
#define I2C0STAT_ACTIVE__SHIFT    0x06 ///< Bus Active                                      
#define I2C0STAT_ACTIVE__NOT_SET  0x00 ///< A transfer is not in progress.                  
#define I2C0STAT_ACTIVE__SET      0x40 ///< A transfer is in progress.                      
                                                                                            
#define I2C0STAT_HSMODE__BMASK    0x80 ///< High Speed Mode                                 
#define I2C0STAT_HSMODE__SHIFT    0x07 ///< High Speed Mode                                 
#define I2C0STAT_HSMODE__NOT_SET  0x00 ///< A High Speed transfer is not in progress.       
#define I2C0STAT_HSMODE__SET      0x80 ///< A High Speed transfer is in progress.           
                                                                                            
//------------------------------------------------------------------------------
// EIE1 Enums (Extended Interrupt Enable 1 @ 0xE6)
//------------------------------------------------------------------------------
#define EIE1_ESMB0__BMASK     0x01 ///< SMBus (SMB0) Interrupt Enable                     
#define EIE1_ESMB0__SHIFT     0x00 ///< SMBus (SMB0) Interrupt Enable                     
#define EIE1_ESMB0__DISABLED  0x00 ///< Disable all SMB0 interrupts.                      
#define EIE1_ESMB0__ENABLED   0x01 ///< Enable interrupt requests generated by SMB0.      
                                                                                          
#define EIE1_EMAT__BMASK      0x02 ///< Port Match Interrupts Enable                      
#define EIE1_EMAT__SHIFT      0x01 ///< Port Match Interrupts Enable                      
#define EIE1_EMAT__DISABLED   0x00 ///< Disable all Port Match interrupts.                
#define EIE1_EMAT__ENABLED    0x02 ///< Enable interrupt requests generated by a Port     
                                   ///< Match.                                            
                                                                                          
#define EIE1_EWADC0__BMASK    0x04 ///< ADC0 Window Comparison Interrupt Enable           
#define EIE1_EWADC0__SHIFT    0x02 ///< ADC0 Window Comparison Interrupt Enable           
#define EIE1_EWADC0__DISABLED 0x00 ///< Disable ADC0 Window Comparison interrupt.         
#define EIE1_EWADC0__ENABLED  0x04 ///< Enable interrupt requests generated by ADC0 Window
                                   ///< Compare flag (ADWINT).                            
                                                                                          
#define EIE1_EADC0__BMASK     0x08 ///< ADC0 Conversion Complete Interrupt Enable         
#define EIE1_EADC0__SHIFT     0x03 ///< ADC0 Conversion Complete Interrupt Enable         
#define EIE1_EADC0__DISABLED  0x00 ///< Disable ADC0 Conversion Complete interrupt.       
#define EIE1_EADC0__ENABLED   0x08 ///< Enable interrupt requests generated by the ADINT  
                                   ///< flag.                                             
                                                                                          
#define EIE1_EPCA0__BMASK     0x10 ///< Programmable Counter Array (PCA0) Interrupt Enable
#define EIE1_EPCA0__SHIFT     0x04 ///< Programmable Counter Array (PCA0) Interrupt Enable
#define EIE1_EPCA0__DISABLED  0x00 ///< Disable all PCA0 interrupts.                      
#define EIE1_EPCA0__ENABLED   0x10 ///< Enable interrupt requests generated by PCA0.      
                                                                                          
#define EIE1_ECP0__BMASK      0x20 ///< Comparator0 (CP0) Interrupt Enable                
#define EIE1_ECP0__SHIFT      0x05 ///< Comparator0 (CP0) Interrupt Enable                
#define EIE1_ECP0__DISABLED   0x00 ///< Disable CP0 interrupts.                           
#define EIE1_ECP0__ENABLED    0x20 ///< Enable interrupt requests generated by the        
                                   ///< comparator 0 CPRIF or CPFIF flags.                
                                                                                          
#define EIE1_ECP1__BMASK      0x40 ///< Comparator1 (CP1) Interrupt Enable                
#define EIE1_ECP1__SHIFT      0x06 ///< Comparator1 (CP1) Interrupt Enable                
#define EIE1_ECP1__DISABLED   0x00 ///< Disable CP1 interrupts.                           
#define EIE1_ECP1__ENABLED    0x40 ///< Enable interrupt requests generated by the        
                                   ///< comparator 1 CPRIF or CPFIF flags.                
                                                                                          
#define EIE1_ET3__BMASK       0x80 ///< Timer 3 Interrupt Enable                          
#define EIE1_ET3__SHIFT       0x07 ///< Timer 3 Interrupt Enable                          
#define EIE1_ET3__DISABLED    0x00 ///< Disable Timer 3 interrupts.                       
#define EIE1_ET3__ENABLED     0x80 ///< Enable interrupt requests generated by the TF3L or
                                   ///< TF3H flags.                                       
                                                                                          
//------------------------------------------------------------------------------
// EIE2 Enums (Extended Interrupt Enable 2 @ 0xF3)
//------------------------------------------------------------------------------
#define EIE2_ES1__BMASK      0x01 ///< UART1 Interrupt Enable                            
#define EIE2_ES1__SHIFT      0x00 ///< UART1 Interrupt Enable                            
#define EIE2_ES1__DISABLED   0x00 ///< Disable UART1 interrupts.                         
#define EIE2_ES1__ENABLED    0x01 ///< Enable UART1 interrupts.                          
                                                                                         
#define EIE2_EI2C0__BMASK    0x02 ///< I2C0 Slave Interrupt Enable                       
#define EIE2_EI2C0__SHIFT    0x01 ///< I2C0 Slave Interrupt Enable                       
#define EIE2_EI2C0__DISABLED 0x00 ///< Disable all I2C0 slave interrupts.                
#define EIE2_EI2C0__ENABLED  0x02 ///< Enable interrupt requests generated by the I2C0   
                                  ///< slave.                                            
                                                                                         
#define EIE2_ET4__BMASK      0x04 ///< Timer 4 Interrupt Enable                          
#define EIE2_ET4__SHIFT      0x02 ///< Timer 4 Interrupt Enable                          
#define EIE2_ET4__DISABLED   0x00 ///< Disable Timer 4 interrupts.                       
#define EIE2_ET4__ENABLED    0x04 ///< Enable interrupt requests generated by the TF4L or
                                  ///< TF4H flags.                                       
                                                                                         
#define EIE2_ET5__BMASK      0x08 ///< Timer 5 Interrupt Enable                          
#define EIE2_ET5__SHIFT      0x03 ///< Timer 5 Interrupt Enable                          
#define EIE2_ET5__DISABLED   0x00 ///< Disable Timer 5 interrupts.                       
#define EIE2_ET5__ENABLED    0x08 ///< Enable interrupt requests generated by the TF5L or
                                  ///< TF5H flags.                                       
                                                                                         
#define EIE2_CL0__BMASK      0x10 ///< Configurable Logic (CL0) Interrupt Enable         
#define EIE2_CL0__SHIFT      0x04 ///< Configurable Logic (CL0) Interrupt Enable         
#define EIE2_CL0__DISABLED   0x00 ///< Disable CL0 interrupts.                           
#define EIE2_CL0__ENABLED    0x10 ///< Enable interrupt requests generated by CL0.       
                                                                                         
//------------------------------------------------------------------------------
// EIP1 Enums (Extended Interrupt Priority 1 Low @ 0xBB)
//------------------------------------------------------------------------------
#define EIP1_PSMB0__BMASK  0x01 ///< SMBus (SMB0) Interrupt Priority Control LSB                     
#define EIP1_PSMB0__SHIFT  0x00 ///< SMBus (SMB0) Interrupt Priority Control LSB                     
#define EIP1_PSMB0__LOW    0x00 ///< SMB0 interrupt priority LSB set to low.                         
#define EIP1_PSMB0__HIGH   0x01 ///< SMB0 interrupt priority LSB set to high.                        
                                                                                                     
#define EIP1_PMAT__BMASK   0x02 ///< Port Match Interrupt Priority Control LSB                       
#define EIP1_PMAT__SHIFT   0x01 ///< Port Match Interrupt Priority Control LSB                       
#define EIP1_PMAT__LOW     0x00 ///< Port Match interrupt priority LSB set to low.                   
#define EIP1_PMAT__HIGH    0x02 ///< Port Match interrupt priority LSB set to high.                  
                                                                                                     
#define EIP1_PWADC0__BMASK 0x04 ///< ADC0 Window Comparator Interrupt Priority Control LSB           
#define EIP1_PWADC0__SHIFT 0x02 ///< ADC0 Window Comparator Interrupt Priority Control LSB           
#define EIP1_PWADC0__LOW   0x00 ///< ADC0 Window interrupt priority LSB set to low.                  
#define EIP1_PWADC0__HIGH  0x04 ///< ADC0 Window interrupt priority LSB set to high.                 
                                                                                                     
#define EIP1_PADC0__BMASK  0x08 ///< ADC0 Conversion Complete Interrupt Priority Control LSB         
#define EIP1_PADC0__SHIFT  0x03 ///< ADC0 Conversion Complete Interrupt Priority Control LSB         
#define EIP1_PADC0__LOW    0x00 ///< ADC0 Conversion Complete interrupt priority LSB                 
                                ///< set to low.                                                     
#define EIP1_PADC0__HIGH   0x08 ///< ADC0 Conversion Complete interrupt priority LSB                 
                                ///< set to high.                                                    
                                                                                                     
#define EIP1_PPCA0__BMASK  0x10 ///< Programmable Counter Array (PCA0) Interrupt Priority Control LSB
#define EIP1_PPCA0__SHIFT  0x04 ///< Programmable Counter Array (PCA0) Interrupt Priority Control LSB
#define EIP1_PPCA0__LOW    0x00 ///< PCA0 interrupt priority LSB set to low.                         
#define EIP1_PPCA0__HIGH   0x10 ///< PCA0 interrupt priority LSB set to high.                        
                                                                                                     
#define EIP1_PCP0__BMASK   0x20 ///< Comparator0 (CP0) Interrupt Priority Control LSB                
#define EIP1_PCP0__SHIFT   0x05 ///< Comparator0 (CP0) Interrupt Priority Control LSB                
#define EIP1_PCP0__LOW     0x00 ///< CP0 interrupt priority LSB set to low.                          
#define EIP1_PCP0__HIGH    0x20 ///< CP0 interrupt priority LSB set to high.                         
                                                                                                     
#define EIP1_PCP1__BMASK   0x40 ///< Comparator1 (CP1) Interrupt Priority Control LSB                
#define EIP1_PCP1__SHIFT   0x06 ///< Comparator1 (CP1) Interrupt Priority Control LSB                
#define EIP1_PCP1__LOW     0x00 ///< CP1 interrupt priority LSB set to low.                          
#define EIP1_PCP1__HIGH    0x40 ///< CP1 interrupt priority LSB set to high.                         
                                                                                                     
#define EIP1_PT3__BMASK    0x80 ///< Timer 3 Interrupt Priority Control LSB                          
#define EIP1_PT3__SHIFT    0x07 ///< Timer 3 Interrupt Priority Control LSB                          
#define EIP1_PT3__LOW      0x00 ///< Timer 3 interrupt priority LSB set to low.                      
#define EIP1_PT3__HIGH     0x80 ///< Timer 3 interrupt priority LSB set to high.                     
                                                                                                     
//------------------------------------------------------------------------------
// EIP1H Enums (Extended Interrupt Priority 1 High @ 0xEE)
//------------------------------------------------------------------------------
#define EIP1H_PHSMB0__BMASK  0x01 ///< SMBus (SMB0) Interrupt Priority Control MSB                     
#define EIP1H_PHSMB0__SHIFT  0x00 ///< SMBus (SMB0) Interrupt Priority Control MSB                     
#define EIP1H_PHSMB0__LOW    0x00 ///< SMB0 interrupt priority MSB set to low.                         
#define EIP1H_PHSMB0__HIGH   0x01 ///< SMB0 interrupt priority MSB set to high.                        
                                                                                                       
#define EIP1H_PHMAT__BMASK   0x02 ///< Port Match Interrupt Priority Control MSB                       
#define EIP1H_PHMAT__SHIFT   0x01 ///< Port Match Interrupt Priority Control MSB                       
#define EIP1H_PHMAT__LOW     0x00 ///< Port Match interrupt priority MSB set to low.                   
#define EIP1H_PHMAT__HIGH    0x02 ///< Port Match interrupt priority MSB set to high.                  
                                                                                                       
#define EIP1H_PHWADC0__BMASK 0x04 ///< ADC0 Window Comparator Interrupt Priority Control MSB           
#define EIP1H_PHWADC0__SHIFT 0x02 ///< ADC0 Window Comparator Interrupt Priority Control MSB           
#define EIP1H_PHWADC0__LOW   0x00 ///< ADC0 Window interrupt priority MSB set to low.                  
#define EIP1H_PHWADC0__HIGH  0x04 ///< ADC0 Window interrupt priority MSB set to high.                 
                                                                                                       
#define EIP1H_PHADC0__BMASK  0x08 ///< ADC0 Conversion Complete Interrupt Priority Control MSB         
#define EIP1H_PHADC0__SHIFT  0x03 ///< ADC0 Conversion Complete Interrupt Priority Control MSB         
#define EIP1H_PHADC0__LOW    0x00 ///< ADC0 Conversion Complete interrupt priority MSB                 
                                  ///< set to low.                                                     
#define EIP1H_PHADC0__HIGH   0x08 ///< ADC0 Conversion Complete interrupt priority MSB                 
                                  ///< set to high.                                                    
                                                                                                       
#define EIP1H_PHPCA0__BMASK  0x10 ///< Programmable Counter Array (PCA0) Interrupt Priority Control MSB
#define EIP1H_PHPCA0__SHIFT  0x04 ///< Programmable Counter Array (PCA0) Interrupt Priority Control MSB
#define EIP1H_PHPCA0__LOW    0x00 ///< PCA0 interrupt priority MSB set to low.                         
#define EIP1H_PHPCA0__HIGH   0x10 ///< PCA0 interrupt priority MSB set to high.                        
                                                                                                       
#define EIP1H_PHCP0__BMASK   0x20 ///< Comparator0 (CP0) Interrupt Priority Control MSB                
#define EIP1H_PHCP0__SHIFT   0x05 ///< Comparator0 (CP0) Interrupt Priority Control MSB                
#define EIP1H_PHCP0__LOW     0x00 ///< CP0 interrupt priority MSB set to low.                          
#define EIP1H_PHCP0__HIGH    0x20 ///< CP0 interrupt priority MSB set to high.                         
                                                                                                       
#define EIP1H_PHCP1__BMASK   0x40 ///< Comparator1 (CP1) Interrupt Priority Control MSB                
#define EIP1H_PHCP1__SHIFT   0x06 ///< Comparator1 (CP1) Interrupt Priority Control MSB                
#define EIP1H_PHCP1__LOW     0x00 ///< CP1 interrupt priority MSB set to low.                          
#define EIP1H_PHCP1__HIGH    0x40 ///< CP1 interrupt priority MSB set to high.                         
                                                                                                       
#define EIP1H_PHT3__BMASK    0x80 ///< Timer 3 Interrupt Priority Control MSB                          
#define EIP1H_PHT3__SHIFT    0x07 ///< Timer 3 Interrupt Priority Control MSB                          
#define EIP1H_PHT3__LOW      0x00 ///< Timer 3 interrupt priority MSB set to low.                      
#define EIP1H_PHT3__HIGH     0x80 ///< Timer 3 interrupt priority MSB set to high.                     
                                                                                                       
//------------------------------------------------------------------------------
// EIP2 Enums (Extended Interrupt Priority 2 @ 0xED)
//------------------------------------------------------------------------------
#define EIP2_PS1__BMASK   0x01 ///< UART1 Interrupt Priority Control LSB                   
#define EIP2_PS1__SHIFT   0x00 ///< UART1 Interrupt Priority Control LSB                   
#define EIP2_PS1__LOW     0x00 ///< UART1 interrupt priority LSB set to low.               
#define EIP2_PS1__HIGH    0x01 ///< UART1 interrupt priority LSB set to high.              
                                                                                           
#define EIP2_PI2C0__BMASK 0x02 ///< I2C0 Slave Interrupt Priority Control LSB              
#define EIP2_PI2C0__SHIFT 0x01 ///< I2C0 Slave Interrupt Priority Control LSB              
#define EIP2_PI2C0__LOW   0x00 ///< I2C0 Slave interrupt priority LSB set to low.          
#define EIP2_PI2C0__HIGH  0x02 ///< I2C0 Slave interrupt priority LSB set to high.         
                                                                                           
#define EIP2_PT4__BMASK   0x04 ///< Timer 4 Interrupt Priority Control LSB                 
#define EIP2_PT4__SHIFT   0x02 ///< Timer 4 Interrupt Priority Control LSB                 
#define EIP2_PT4__LOW     0x00 ///< Timer 4 interrupt priority LSB set to low.             
#define EIP2_PT4__HIGH    0x04 ///< Timer 4 interrupt priority LSB set to high.            
                                                                                           
#define EIP2_PT5__BMASK   0x08 ///< Timer 5 Interrupt Priority Control LSB                 
#define EIP2_PT5__SHIFT   0x03 ///< Timer 5 Interrupt Priority Control LSB                 
#define EIP2_PT5__LOW     0x00 ///< Timer 5 interrupt priority LSB set to low.             
#define EIP2_PT5__HIGH    0x08 ///< Timer 5 interrupt priority LSB set to high.            
                                                                                           
#define EIP2_PCL0__BMASK  0x10 ///< Configurable Logic (CL0) Interrupt Priority Control LSB
#define EIP2_PCL0__SHIFT  0x04 ///< Configurable Logic (CL0) Interrupt Priority Control LSB
#define EIP2_PCL0__LOW    0x00 ///< CL0 interrupt priority LSB set to low.                 
#define EIP2_PCL0__HIGH   0x10 ///< CL0 interrupt priority LSB set to high.                
                                                                                           
//------------------------------------------------------------------------------
// EIP2H Enums (Extended Interrupt Priority 2 High @ 0xF6)
//------------------------------------------------------------------------------
#define EIP2H_PHS1__BMASK   0x01 ///< UART1 Interrupt Priority Control MSB                   
#define EIP2H_PHS1__SHIFT   0x00 ///< UART1 Interrupt Priority Control MSB                   
#define EIP2H_PHS1__LOW     0x00 ///< UART1 interrupt priority MSB set to low.               
#define EIP2H_PHS1__HIGH    0x01 ///< UART1 interrupt priority MSB set to high.              
                                                                                             
#define EIP2H_PHI2C0__BMASK 0x02 ///< I2C0 Slave Interrupt Priority Control MSB              
#define EIP2H_PHI2C0__SHIFT 0x01 ///< I2C0 Slave Interrupt Priority Control MSB              
#define EIP2H_PHI2C0__LOW   0x00 ///< I2C0 Slave interrupt priority MSB set to low.          
#define EIP2H_PHI2C0__HIGH  0x02 ///< I2C0 Slave interrupt priority MSB set to high.         
                                                                                             
#define EIP2H_PHT4__BMASK   0x04 ///< Timer 4 Interrupt Priority Control MSB                 
#define EIP2H_PHT4__SHIFT   0x02 ///< Timer 4 Interrupt Priority Control MSB                 
#define EIP2H_PHT4__LOW     0x00 ///< Timer 4 interrupt priority MSB set to low.             
#define EIP2H_PHT4__HIGH    0x04 ///< Timer 4 interrupt priority MSB set to high.            
                                                                                             
#define EIP2H_PHT5__BMASK   0x08 ///< Timer 5 Interrupt Priority Control MSB                 
#define EIP2H_PHT5__SHIFT   0x03 ///< Timer 5 Interrupt Priority Control MSB                 
#define EIP2H_PHT5__LOW     0x00 ///< Timer 5 interrupt priority MSB set to low.             
#define EIP2H_PHT5__HIGH    0x08 ///< Timer 5 interrupt priority MSB set to high.            
                                                                                             
#define EIP2H_PHCL0__BMASK  0x10 ///< Configurable Logic (CL0) Interrupt Priority Control MSB
#define EIP2H_PHCL0__SHIFT  0x04 ///< Configurable Logic (CL0) Interrupt Priority Control MSB
#define EIP2H_PHCL0__LOW    0x00 ///< CL0 interrupt priority MSB set to low.                 
#define EIP2H_PHCL0__HIGH   0x10 ///< CL0 interrupt priority MSB set to high.                
                                                                                             
//------------------------------------------------------------------------------
// IE Enums (Interrupt Enable @ 0xA8)
//------------------------------------------------------------------------------
#define IE_EX0__BMASK      0x01 ///< External Interrupt 0 Enable                       
#define IE_EX0__SHIFT      0x00 ///< External Interrupt 0 Enable                       
#define IE_EX0__DISABLED   0x00 ///< Disable external interrupt 0.                     
#define IE_EX0__ENABLED    0x01 ///< Enable interrupt requests generated by the INT0   
                                ///< input.                                            
                                                                                       
#define IE_ET0__BMASK      0x02 ///< Timer 0 Interrupt Enable                          
#define IE_ET0__SHIFT      0x01 ///< Timer 0 Interrupt Enable                          
#define IE_ET0__DISABLED   0x00 ///< Disable all Timer 0 interrupt.                    
#define IE_ET0__ENABLED    0x02 ///< Enable interrupt requests generated by the TF0    
                                ///< flag.                                             
                                                                                       
#define IE_EX1__BMASK      0x04 ///< External Interrupt 1 Enable                       
#define IE_EX1__SHIFT      0x02 ///< External Interrupt 1 Enable                       
#define IE_EX1__DISABLED   0x00 ///< Disable external interrupt 1.                     
#define IE_EX1__ENABLED    0x04 ///< Enable interrupt requests generated by the INT1   
                                ///< input.                                            
                                                                                       
#define IE_ET1__BMASK      0x08 ///< Timer 1 Interrupt Enable                          
#define IE_ET1__SHIFT      0x03 ///< Timer 1 Interrupt Enable                          
#define IE_ET1__DISABLED   0x00 ///< Disable all Timer 1 interrupt.                    
#define IE_ET1__ENABLED    0x08 ///< Enable interrupt requests generated by the TF1    
                                ///< flag.                                             
                                                                                       
#define IE_ES0__BMASK      0x10 ///< UART0 Interrupt Enable                            
#define IE_ES0__SHIFT      0x04 ///< UART0 Interrupt Enable                            
#define IE_ES0__DISABLED   0x00 ///< Disable UART0 interrupt.                          
#define IE_ES0__ENABLED    0x10 ///< Enable UART0 interrupt.                           
                                                                                       
#define IE_ET2__BMASK      0x20 ///< Timer 2 Interrupt Enable                          
#define IE_ET2__SHIFT      0x05 ///< Timer 2 Interrupt Enable                          
#define IE_ET2__DISABLED   0x00 ///< Disable Timer 2 interrupt.                        
#define IE_ET2__ENABLED    0x20 ///< Enable interrupt requests generated by the TF2L or
                                ///< TF2H flags.                                       
                                                                                       
#define IE_ESPI0__BMASK    0x40 ///< SPI0 Interrupt Enable                             
#define IE_ESPI0__SHIFT    0x06 ///< SPI0 Interrupt Enable                             
#define IE_ESPI0__DISABLED 0x00 ///< Disable all SPI0 interrupts.                      
#define IE_ESPI0__ENABLED  0x40 ///< Enable interrupt requests generated by SPI0.      
                                                                                       
#define IE_EA__BMASK       0x80 ///< All Interrupts Enable                             
#define IE_EA__SHIFT       0x07 ///< All Interrupts Enable                             
#define IE_EA__DISABLED    0x00 ///< Disable all interrupt sources.                    
#define IE_EA__ENABLED     0x80 ///< Enable each interrupt according to its individual 
                                ///< mask setting.                                     
                                                                                       
//------------------------------------------------------------------------------
// IP Enums (Interrupt Priority @ 0xB8)
//------------------------------------------------------------------------------
#define IP_PX0__BMASK   0x01 ///< External Interrupt 0 Priority Control LSB                        
#define IP_PX0__SHIFT   0x00 ///< External Interrupt 0 Priority Control LSB                        
#define IP_PX0__LOW     0x00 ///< External Interrupt 0 priority LSB set to low.                    
#define IP_PX0__HIGH    0x01 ///< External Interrupt 0 priority LSB set to high.                   
                                                                                                   
#define IP_PT0__BMASK   0x02 ///< Timer 0 Interrupt Priority Control LSB                           
#define IP_PT0__SHIFT   0x01 ///< Timer 0 Interrupt Priority Control LSB                           
#define IP_PT0__LOW     0x00 ///< Timer 0 interrupt priority LSB set to low.                       
#define IP_PT0__HIGH    0x02 ///< Timer 0 interrupt priority LSB set to high.                      
                                                                                                   
#define IP_PX1__BMASK   0x04 ///< External Interrupt 1 Priority Control LSB                        
#define IP_PX1__SHIFT   0x02 ///< External Interrupt 1 Priority Control LSB                        
#define IP_PX1__LOW     0x00 ///< External Interrupt 1 priority LSB set to low.                    
#define IP_PX1__HIGH    0x04 ///< External Interrupt 1 priority LSB set to high.                   
                                                                                                   
#define IP_PT1__BMASK   0x08 ///< Timer 1 Interrupt Priority Control LSB                           
#define IP_PT1__SHIFT   0x03 ///< Timer 1 Interrupt Priority Control LSB                           
#define IP_PT1__LOW     0x00 ///< Timer 1 interrupt priority LSB set to low.                       
#define IP_PT1__HIGH    0x08 ///< Timer 1 interrupt priority LSB set to high.                      
                                                                                                   
#define IP_PS0__BMASK   0x10 ///< UART0 Interrupt Priority Control LSB                             
#define IP_PS0__SHIFT   0x04 ///< UART0 Interrupt Priority Control LSB                             
#define IP_PS0__LOW     0x00 ///< UART0 interrupt priority LSB set to low.                         
#define IP_PS0__HIGH    0x10 ///< UART0 interrupt priority LSB set to high.                        
                                                                                                   
#define IP_PT2__BMASK   0x20 ///< Timer 2 Interrupt Priority Control LSB                           
#define IP_PT2__SHIFT   0x05 ///< Timer 2 Interrupt Priority Control LSB                           
#define IP_PT2__LOW     0x00 ///< Timer 2 interrupt priority LSB set to low.                       
#define IP_PT2__HIGH    0x20 ///< Timer 2 interrupt priority LSB set to high.                      
                                                                                                   
#define IP_PSPI0__BMASK 0x40 ///< Serial Peripheral Interface (SPI0) Interrupt Priority Control LSB
#define IP_PSPI0__SHIFT 0x06 ///< Serial Peripheral Interface (SPI0) Interrupt Priority Control LSB
#define IP_PSPI0__LOW   0x00 ///< SPI0 interrupt priority LSB set to low.                          
#define IP_PSPI0__HIGH  0x40 ///< SPI0 interrupt priority LSB set to high.                         
                                                                                                   
//------------------------------------------------------------------------------
// IPH Enums (Interrupt Priority High @ 0xF2)
//------------------------------------------------------------------------------
#define IPH_PHX0__BMASK   0x01 ///< External Interrupt 0 Priority Control MSB                        
#define IPH_PHX0__SHIFT   0x00 ///< External Interrupt 0 Priority Control MSB                        
#define IPH_PHX0__LOW     0x00 ///< External Interrupt 0 priority MSB set to low.                    
#define IPH_PHX0__HIGH    0x01 ///< External Interrupt 0 priority MSB set to high.                   
                                                                                                     
#define IPH_PHT0__BMASK   0x02 ///< Timer 0 Interrupt Priority Control MSB                           
#define IPH_PHT0__SHIFT   0x01 ///< Timer 0 Interrupt Priority Control MSB                           
#define IPH_PHT0__LOW     0x00 ///< Timer 0 interrupt priority MSB set to low.                       
#define IPH_PHT0__HIGH    0x02 ///< Timer 0 interrupt priority MSB set to high.                      
                                                                                                     
#define IPH_PHX1__BMASK   0x04 ///< External Interrupt 1 Priority Control MSB                        
#define IPH_PHX1__SHIFT   0x02 ///< External Interrupt 1 Priority Control MSB                        
#define IPH_PHX1__LOW     0x00 ///< External Interrupt 1 priority MSB set to low.                    
#define IPH_PHX1__HIGH    0x04 ///< External Interrupt 1 priority MSB set to high.                   
                                                                                                     
#define IPH_PHT1__BMASK   0x08 ///< Timer 1 Interrupt Priority Control MSB                           
#define IPH_PHT1__SHIFT   0x03 ///< Timer 1 Interrupt Priority Control MSB                           
#define IPH_PHT1__LOW     0x00 ///< Timer 1 interrupt priority MSB set to low.                       
#define IPH_PHT1__HIGH    0x08 ///< Timer 1 interrupt priority MSB set to high.                      
                                                                                                     
#define IPH_PHS0__BMASK   0x10 ///< UART0 Interrupt Priority Control MSB                             
#define IPH_PHS0__SHIFT   0x04 ///< UART0 Interrupt Priority Control MSB                             
#define IPH_PHS0__LOW     0x00 ///< UART0 interrupt priority MSB set to low.                         
#define IPH_PHS0__HIGH    0x10 ///< UART0 interrupt priority MSB set to high.                        
                                                                                                     
#define IPH_PHT2__BMASK   0x20 ///< Timer 2 Interrupt Priority Control MSB                           
#define IPH_PHT2__SHIFT   0x05 ///< Timer 2 Interrupt Priority Control MSB                           
#define IPH_PHT2__LOW     0x00 ///< Timer 2 interrupt priority MSB set to low.                       
#define IPH_PHT2__HIGH    0x20 ///< Timer 2 interrupt priority MSB set to high.                      
                                                                                                     
#define IPH_PHSPI0__BMASK 0x40 ///< Serial Peripheral Interface (SPI0) Interrupt Priority Control MSB
#define IPH_PHSPI0__SHIFT 0x06 ///< Serial Peripheral Interface (SPI0) Interrupt Priority Control MSB
#define IPH_PHSPI0__LOW   0x00 ///< SPI0 interrupt priority MSB set to low.                          
#define IPH_PHSPI0__HIGH  0x40 ///< SPI0 interrupt priority MSB set to high.                         
                                                                                                     
//------------------------------------------------------------------------------
// LFO0CN Enums (Low Frequency Oscillator Control @ 0xB1)
//------------------------------------------------------------------------------
#define LFO0CN_OSCLD__FMASK       0x03 ///< Internal L-F Oscillator Divider Select           
#define LFO0CN_OSCLD__SHIFT       0x00 ///< Internal L-F Oscillator Divider Select           
#define LFO0CN_OSCLD__DIVIDE_BY_8 0x00 ///< Divide by 8 selected.                            
#define LFO0CN_OSCLD__DIVIDE_BY_4 0x01 ///< Divide by 4 selected.                            
#define LFO0CN_OSCLD__DIVIDE_BY_2 0x02 ///< Divide by 2 selected.                            
#define LFO0CN_OSCLD__DIVIDE_BY_1 0x03 ///< Divide by 1 selected.                            
                                                                                             
#define LFO0CN_OSCLF__FMASK       0x3C ///< Internal L-F Oscillator Frequency Control        
#define LFO0CN_OSCLF__SHIFT       0x02 ///< Internal L-F Oscillator Frequency Control        
                                                                                             
#define LFO0CN_OSCLRDY__BMASK     0x40 ///< Internal L-F Oscillator Ready                    
#define LFO0CN_OSCLRDY__SHIFT     0x06 ///< Internal L-F Oscillator Ready                    
#define LFO0CN_OSCLRDY__NOT_SET   0x00 ///< Internal L-F Oscillator frequency not stabilized.
#define LFO0CN_OSCLRDY__SET       0x40 ///< Internal L-F Oscillator frequency stabilized.    
                                                                                             
#define LFO0CN_OSCLEN__BMASK      0x80 ///< Internal L-F Oscillator Enable                   
#define LFO0CN_OSCLEN__SHIFT      0x07 ///< Internal L-F Oscillator Enable                   
#define LFO0CN_OSCLEN__DISABLED   0x00 ///< Internal L-F Oscillator Disabled.                
#define LFO0CN_OSCLEN__ENABLED    0x80 ///< Internal L-F Oscillator Enabled.                 
                                                                                             
//------------------------------------------------------------------------------
// PRTDRV Enums (Port Drive Strength @ 0xF6)
//------------------------------------------------------------------------------
#define PRTDRV_P0DRV__BMASK      0x01 ///< Port 0 Drive Strength                  
#define PRTDRV_P0DRV__SHIFT      0x00 ///< Port 0 Drive Strength                  
#define PRTDRV_P0DRV__LOW_DRIVE  0x00 ///< All pins on P0 use low drive strength. 
#define PRTDRV_P0DRV__HIGH_DRIVE 0x01 ///< All pins on P0 use high drive strength.
                                                                                  
#define PRTDRV_P1DRV__BMASK      0x02 ///< Port 1 Drive Strength                  
#define PRTDRV_P1DRV__SHIFT      0x01 ///< Port 1 Drive Strength                  
#define PRTDRV_P1DRV__LOW_DRIVE  0x00 ///< All pins on P1 use low drive strength. 
#define PRTDRV_P1DRV__HIGH_DRIVE 0x02 ///< All pins on P1 use high drive strength.
                                                                                  
#define PRTDRV_P2DRV__BMASK      0x04 ///< Port 2 Drive Strength                  
#define PRTDRV_P2DRV__SHIFT      0x02 ///< Port 2 Drive Strength                  
#define PRTDRV_P2DRV__LOW_DRIVE  0x00 ///< All pins on P2 use low drive strength. 
#define PRTDRV_P2DRV__HIGH_DRIVE 0x04 ///< All pins on P2 use high drive strength.
                                                                                  
#define PRTDRV_P3DRV__BMASK      0x08 ///< Port 3 Drive Strength                  
#define PRTDRV_P3DRV__SHIFT      0x03 ///< Port 3 Drive Strength                  
#define PRTDRV_P3DRV__LOW_DRIVE  0x00 ///< All pins on P3 use low drive strength. 
#define PRTDRV_P3DRV__HIGH_DRIVE 0x08 ///< All pins on P3 use high drive strength.
                                                                                  
//------------------------------------------------------------------------------
// XBR0 Enums (Port I/O Crossbar 0 @ 0xE1)
//------------------------------------------------------------------------------
#define XBR0_URT0E__BMASK     0x01 ///< UART0 I/O Enable                                 
#define XBR0_URT0E__SHIFT     0x00 ///< UART0 I/O Enable                                 
#define XBR0_URT0E__DISABLED  0x00 ///< UART0 I/O unavailable at Port pin.               
#define XBR0_URT0E__ENABLED   0x01 ///< UART0 TX0, RX0 routed to Port pins P0.4 and P0.5.
                                                                                         
#define XBR0_SPI0E__BMASK     0x02 ///< SPI I/O Enable                                   
#define XBR0_SPI0E__SHIFT     0x01 ///< SPI I/O Enable                                   
#define XBR0_SPI0E__DISABLED  0x00 ///< SPI I/O unavailable at Port pins.                
#define XBR0_SPI0E__ENABLED   0x02 ///< SPI I/O routed to Port pins. The SPI can be      
                                   ///< assigned either 3 or 4 GPIO pins.                
                                                                                         
#define XBR0_SMB0E__BMASK     0x04 ///< SMB0 I/O Enable                                  
#define XBR0_SMB0E__SHIFT     0x02 ///< SMB0 I/O Enable                                  
#define XBR0_SMB0E__DISABLED  0x00 ///< SMBus 0 I/O unavailable at Port pins.            
#define XBR0_SMB0E__ENABLED   0x04 ///< SMBus 0 I/O routed to Port pins.                 
                                                                                         
#define XBR0_CP0E__BMASK      0x08 ///< Comparator0 Output Enable                        
#define XBR0_CP0E__SHIFT      0x03 ///< Comparator0 Output Enable                        
#define XBR0_CP0E__DISABLED   0x00 ///< CP0 unavailable at Port pin.                     
#define XBR0_CP0E__ENABLED    0x08 ///< CP0 routed to Port pin.                          
                                                                                         
#define XBR0_CP0AE__BMASK     0x10 ///< Comparator0 Asynchronous Output Enable           
#define XBR0_CP0AE__SHIFT     0x04 ///< Comparator0 Asynchronous Output Enable           
#define XBR0_CP0AE__DISABLED  0x00 ///< Asynchronous CP0 unavailable at Port pin.        
#define XBR0_CP0AE__ENABLED   0x10 ///< Asynchronous CP0 routed to Port pin.             
                                                                                         
#define XBR0_CP1E__BMASK      0x20 ///< Comparator1 Output Enable                        
#define XBR0_CP1E__SHIFT      0x05 ///< Comparator1 Output Enable                        
#define XBR0_CP1E__DISABLED   0x00 ///< CP1 unavailable at Port pin.                     
#define XBR0_CP1E__ENABLED    0x20 ///< CP1 routed to Port pin.                          
                                                                                         
#define XBR0_CP1AE__BMASK     0x40 ///< Comparator1 Asynchronous Output Enable           
#define XBR0_CP1AE__SHIFT     0x06 ///< Comparator1 Asynchronous Output Enable           
#define XBR0_CP1AE__DISABLED  0x00 ///< Asynchronous CP1 unavailable at Port pin.        
#define XBR0_CP1AE__ENABLED   0x40 ///< Asynchronous CP1 routed to Port pin.             
                                                                                         
#define XBR0_SYSCKE__BMASK    0x80 ///< SYSCLK Output Enable                             
#define XBR0_SYSCKE__SHIFT    0x07 ///< SYSCLK Output Enable                             
#define XBR0_SYSCKE__DISABLED 0x00 ///< SYSCLK unavailable at Port pin.                  
#define XBR0_SYSCKE__ENABLED  0x80 ///< SYSCLK output routed to Port pin.                
                                                                                         
//------------------------------------------------------------------------------
// XBR1 Enums (Port I/O Crossbar 1 @ 0xE2)
//------------------------------------------------------------------------------
#define XBR1_PCA0ME__FMASK        0x07 ///< PCA Module I/O Enable                            
#define XBR1_PCA0ME__SHIFT        0x00 ///< PCA Module I/O Enable                            
#define XBR1_PCA0ME__DISABLED     0x00 ///< All PCA I/O unavailable at Port pins.            
#define XBR1_PCA0ME__CEX0         0x01 ///< CEX0 routed to Port pin.                         
#define XBR1_PCA0ME__CEX0_TO_CEX1 0x02 ///< CEX0, CEX1 routed to Port pins.                  
#define XBR1_PCA0ME__CEX0_TO_CEX2 0x03 ///< CEX0, CEX1, CEX2 routed to Port pins.            
#define XBR1_PCA0ME__CEX0_TO_CEX3 0x04 ///< CEX0, CEX1, CEX2, CEX3 routed to Port pins.      
#define XBR1_PCA0ME__CEX0_TO_CEX4 0x05 ///< CEX0, CEX1, CEX2, CEX3, CEX4 routed to Port pins.
#define XBR1_PCA0ME__CEX0_TO_CEX5 0x06 ///< CEX0, CEX1, CEX2, CEX3, CEX4, CEX5 routed to Port
                                       ///< pins.                                            
                                                                                             
#define XBR1_ECIE__BMASK          0x08 ///< PCA0 External Counter Input Enable               
#define XBR1_ECIE__SHIFT          0x03 ///< PCA0 External Counter Input Enable               
#define XBR1_ECIE__DISABLED       0x00 ///< ECI unavailable at Port pin.                     
#define XBR1_ECIE__ENABLED        0x08 ///< ECI routed to Port pin.                          
                                                                                             
#define XBR1_T0E__BMASK           0x10 ///< T0 Enable                                        
#define XBR1_T0E__SHIFT           0x04 ///< T0 Enable                                        
#define XBR1_T0E__DISABLED        0x00 ///< T0 unavailable at Port pin.                      
#define XBR1_T0E__ENABLED         0x10 ///< T0 routed to Port pin.                           
                                                                                             
#define XBR1_T1E__BMASK           0x20 ///< T1 Enable                                        
#define XBR1_T1E__SHIFT           0x05 ///< T1 Enable                                        
#define XBR1_T1E__DISABLED        0x00 ///< T1 unavailable at Port pin.                      
#define XBR1_T1E__ENABLED         0x20 ///< T1 routed to Port pin.                           
                                                                                             
#define XBR1_T2E__BMASK           0x40 ///< T2 Enable                                        
#define XBR1_T2E__SHIFT           0x06 ///< T2 Enable                                        
#define XBR1_T2E__DISABLED        0x00 ///< T2 unavailable at Port pin.                      
#define XBR1_T2E__ENABLED         0x40 ///< T2 routed to Port pin.                           
                                                                                             
//------------------------------------------------------------------------------
// XBR2 Enums (Port I/O Crossbar 2 @ 0xE3)
//------------------------------------------------------------------------------
#define XBR2_URT1E__BMASK               0x01 ///< UART1 I/O Enable                                
#define XBR2_URT1E__SHIFT               0x00 ///< UART1 I/O Enable                                
#define XBR2_URT1E__DISABLED            0x00 ///< UART1 I/O unavailable at Port pin.              
#define XBR2_URT1E__ENABLED             0x01 ///< UART1 TX1 RX1 routed to Port pins.              
                                                                                                  
#define XBR2_URT1RTSE__BMASK            0x02 ///< UART1 RTS Output Enable                         
#define XBR2_URT1RTSE__SHIFT            0x01 ///< UART1 RTS Output Enable                         
#define XBR2_URT1RTSE__DISABLED         0x00 ///< UART1 RTS1 unavailable at Port pin.             
#define XBR2_URT1RTSE__ENABLED          0x02 ///< UART1 RTS1 routed to Port pin.                  
                                                                                                  
#define XBR2_URT1CTSE__BMASK            0x04 ///< UART1 CTS Input Enable                          
#define XBR2_URT1CTSE__SHIFT            0x02 ///< UART1 CTS Input Enable                          
#define XBR2_URT1CTSE__DISABLED         0x00 ///< UART1 CTS1 unavailable at Port pin.             
#define XBR2_URT1CTSE__ENABLED          0x04 ///< UART1 CTS1 routed to Port pin.                  
                                                                                                  
#define XBR2_XBARE__BMASK               0x40 ///< Crossbar Enable                                 
#define XBR2_XBARE__SHIFT               0x06 ///< Crossbar Enable                                 
#define XBR2_XBARE__DISABLED            0x00 ///< Crossbar disabled.                              
#define XBR2_XBARE__ENABLED             0x40 ///< Crossbar enabled.                               
                                                                                                  
#define XBR2_WEAKPUD__BMASK             0x80 ///< Port I/O Weak Pullup Disable                    
#define XBR2_WEAKPUD__SHIFT             0x07 ///< Port I/O Weak Pullup Disable                    
#define XBR2_WEAKPUD__PULL_UPS_ENABLED  0x00 ///< Weak Pullups enabled (except for Ports whose I/O
                                             ///< are configured for analog mode).                
#define XBR2_WEAKPUD__PULL_UPS_DISABLED 0x80 ///< Weak Pullups disabled.                          
                                                                                                  
//------------------------------------------------------------------------------
// PCA0CPH0 Enums (PCA Channel 0 Capture Module High Byte @ 0xFC)
//------------------------------------------------------------------------------
#define PCA0CPH0_PCA0CPH0__FMASK 0xFF ///< PCA Channel 0 Capture Module High Byte
#define PCA0CPH0_PCA0CPH0__SHIFT 0x00 ///< PCA Channel 0 Capture Module High Byte
                                                                                 
//------------------------------------------------------------------------------
// PCA0CPL0 Enums (PCA Channel 0 Capture Module Low Byte @ 0xFB)
//------------------------------------------------------------------------------
#define PCA0CPL0_PCA0CPL0__FMASK 0xFF ///< PCA Channel 0 Capture Module Low Byte
#define PCA0CPL0_PCA0CPL0__SHIFT 0x00 ///< PCA Channel 0 Capture Module Low Byte
                                                                                
//------------------------------------------------------------------------------
// PCA0CPM0 Enums (PCA Channel 0 Capture/Compare Mode @ 0xDA)
//------------------------------------------------------------------------------
#define PCA0CPM0_ECCF__BMASK    0x01 ///< Channel 0 Capture/Compare Flag Interrupt Enable
#define PCA0CPM0_ECCF__SHIFT    0x00 ///< Channel 0 Capture/Compare Flag Interrupt Enable
#define PCA0CPM0_ECCF__DISABLED 0x00 ///< Disable CCF0 interrupts.                       
#define PCA0CPM0_ECCF__ENABLED  0x01 ///< Enable a Capture/Compare Flag interrupt request
                                     ///< when CCF0 is set.                              
                                                                                         
#define PCA0CPM0_PWM__BMASK     0x02 ///< Channel 0 Pulse Width Modulation Mode Enable   
#define PCA0CPM0_PWM__SHIFT     0x01 ///< Channel 0 Pulse Width Modulation Mode Enable   
#define PCA0CPM0_PWM__DISABLED  0x00 ///< Disable PWM function.                          
#define PCA0CPM0_PWM__ENABLED   0x02 ///< Enable PWM function.                           
                                                                                         
#define PCA0CPM0_TOG__BMASK     0x04 ///< Channel 0 Toggle Function Enable               
#define PCA0CPM0_TOG__SHIFT     0x02 ///< Channel 0 Toggle Function Enable               
#define PCA0CPM0_TOG__DISABLED  0x00 ///< Disable toggle function.                       
#define PCA0CPM0_TOG__ENABLED   0x04 ///< Enable toggle function.                        
                                                                                         
#define PCA0CPM0_MAT__BMASK     0x08 ///< Channel 0 Match Function Enable                
#define PCA0CPM0_MAT__SHIFT     0x03 ///< Channel 0 Match Function Enable                
#define PCA0CPM0_MAT__DISABLED  0x00 ///< Disable match function.                        
#define PCA0CPM0_MAT__ENABLED   0x08 ///< Enable match function.                         
                                                                                         
#define PCA0CPM0_CAPN__BMASK    0x10 ///< Channel 0 Capture Negative Function Enable     
#define PCA0CPM0_CAPN__SHIFT    0x04 ///< Channel 0 Capture Negative Function Enable     
#define PCA0CPM0_CAPN__DISABLED 0x00 ///< Disable negative edge capture.                 
#define PCA0CPM0_CAPN__ENABLED  0x10 ///< Enable negative edge capture.                  
                                                                                         
#define PCA0CPM0_CAPP__BMASK    0x20 ///< Channel 0 Capture Positive Function Enable     
#define PCA0CPM0_CAPP__SHIFT    0x05 ///< Channel 0 Capture Positive Function Enable     
#define PCA0CPM0_CAPP__DISABLED 0x00 ///< Disable positive edge capture.                 
#define PCA0CPM0_CAPP__ENABLED  0x20 ///< Enable positive edge capture.                  
                                                                                         
#define PCA0CPM0_ECOM__BMASK    0x40 ///< Channel 0 Comparator Function Enable           
#define PCA0CPM0_ECOM__SHIFT    0x06 ///< Channel 0 Comparator Function Enable           
#define PCA0CPM0_ECOM__DISABLED 0x00 ///< Disable comparator function.                   
#define PCA0CPM0_ECOM__ENABLED  0x40 ///< Enable comparator function.                    
                                                                                         
#define PCA0CPM0_PWM16__BMASK   0x80 ///< Channel 0 16-bit Pulse Width Modulation Enable 
#define PCA0CPM0_PWM16__SHIFT   0x07 ///< Channel 0 16-bit Pulse Width Modulation Enable 
#define PCA0CPM0_PWM16__8_BIT   0x00 ///< 8 to 11-bit PWM selected.                      
#define PCA0CPM0_PWM16__16_BIT  0x80 ///< 16-bit PWM selected.                           
                                                                                         
//------------------------------------------------------------------------------
// PCA0CPH1 Enums (PCA Channel 1 Capture Module High Byte @ 0xEA)
//------------------------------------------------------------------------------
#define PCA0CPH1_PCA0CPH1__FMASK 0xFF ///< PCA Channel 1 Capture Module High Byte
#define PCA0CPH1_PCA0CPH1__SHIFT 0x00 ///< PCA Channel 1 Capture Module High Byte
                                                                                 
//------------------------------------------------------------------------------
// PCA0CPL1 Enums (PCA Channel 1 Capture Module Low Byte @ 0xE9)
//------------------------------------------------------------------------------
#define PCA0CPL1_PCA0CPL1__FMASK 0xFF ///< PCA Channel 1 Capture Module Low Byte
#define PCA0CPL1_PCA0CPL1__SHIFT 0x00 ///< PCA Channel 1 Capture Module Low Byte
                                                                                
//------------------------------------------------------------------------------
// PCA0CPM1 Enums (PCA Channel 1 Capture/Compare Mode @ 0xDB)
//------------------------------------------------------------------------------
#define PCA0CPM1_ECCF__BMASK    0x01 ///< Channel 1 Capture/Compare Flag Interrupt Enable
#define PCA0CPM1_ECCF__SHIFT    0x00 ///< Channel 1 Capture/Compare Flag Interrupt Enable
#define PCA0CPM1_ECCF__DISABLED 0x00 ///< Disable CCF1 interrupts.                       
#define PCA0CPM1_ECCF__ENABLED  0x01 ///< Enable a Capture/Compare Flag interrupt request
                                     ///< when CCF1 is set.                              
                                                                                         
#define PCA0CPM1_PWM__BMASK     0x02 ///< Channel 1 Pulse Width Modulation Mode Enable   
#define PCA0CPM1_PWM__SHIFT     0x01 ///< Channel 1 Pulse Width Modulation Mode Enable   
#define PCA0CPM1_PWM__DISABLED  0x00 ///< Disable PWM function.                          
#define PCA0CPM1_PWM__ENABLED   0x02 ///< Enable PWM function.                           
                                                                                         
#define PCA0CPM1_TOG__BMASK     0x04 ///< Channel 1 Toggle Function Enable               
#define PCA0CPM1_TOG__SHIFT     0x02 ///< Channel 1 Toggle Function Enable               
#define PCA0CPM1_TOG__DISABLED  0x00 ///< Disable toggle function.                       
#define PCA0CPM1_TOG__ENABLED   0x04 ///< Enable toggle function.                        
                                                                                         
#define PCA0CPM1_MAT__BMASK     0x08 ///< Channel 1 Match Function Enable                
#define PCA0CPM1_MAT__SHIFT     0x03 ///< Channel 1 Match Function Enable                
#define PCA0CPM1_MAT__DISABLED  0x00 ///< Disable match function.                        
#define PCA0CPM1_MAT__ENABLED   0x08 ///< Enable match function.                         
                                                                                         
#define PCA0CPM1_CAPN__BMASK    0x10 ///< Channel 1 Capture Negative Function Enable     
#define PCA0CPM1_CAPN__SHIFT    0x04 ///< Channel 1 Capture Negative Function Enable     
#define PCA0CPM1_CAPN__DISABLED 0x00 ///< Disable negative edge capture.                 
#define PCA0CPM1_CAPN__ENABLED  0x10 ///< Enable negative edge capture.                  
                                                                                         
#define PCA0CPM1_CAPP__BMASK    0x20 ///< Channel 1 Capture Positive Function Enable     
#define PCA0CPM1_CAPP__SHIFT    0x05 ///< Channel 1 Capture Positive Function Enable     
#define PCA0CPM1_CAPP__DISABLED 0x00 ///< Disable positive edge capture.                 
#define PCA0CPM1_CAPP__ENABLED  0x20 ///< Enable positive edge capture.                  
                                                                                         
#define PCA0CPM1_ECOM__BMASK    0x40 ///< Channel 1 Comparator Function Enable           
#define PCA0CPM1_ECOM__SHIFT    0x06 ///< Channel 1 Comparator Function Enable           
#define PCA0CPM1_ECOM__DISABLED 0x00 ///< Disable comparator function.                   
#define PCA0CPM1_ECOM__ENABLED  0x40 ///< Enable comparator function.                    
                                                                                         
#define PCA0CPM1_PWM16__BMASK   0x80 ///< Channel 1 16-bit Pulse Width Modulation Enable 
#define PCA0CPM1_PWM16__SHIFT   0x07 ///< Channel 1 16-bit Pulse Width Modulation Enable 
#define PCA0CPM1_PWM16__8_BIT   0x00 ///< 8 to 11-bit PWM selected.                      
#define PCA0CPM1_PWM16__16_BIT  0x80 ///< 16-bit PWM selected.                           
                                                                                         
//------------------------------------------------------------------------------
// PCA0CPH2 Enums (PCA Channel 2 Capture Module High Byte @ 0xEC)
//------------------------------------------------------------------------------
#define PCA0CPH2_PCA0CPH2__FMASK 0xFF ///< PCA Channel 2 Capture Module High Byte
#define PCA0CPH2_PCA0CPH2__SHIFT 0x00 ///< PCA Channel 2 Capture Module High Byte
                                                                                 
//------------------------------------------------------------------------------
// PCA0CPL2 Enums (PCA Channel 2 Capture Module Low Byte @ 0xEB)
//------------------------------------------------------------------------------
#define PCA0CPL2_PCA0CPL2__FMASK 0xFF ///< PCA Channel 2 Capture Module Low Byte
#define PCA0CPL2_PCA0CPL2__SHIFT 0x00 ///< PCA Channel 2 Capture Module Low Byte
                                                                                
//------------------------------------------------------------------------------
// PCA0CPM2 Enums (PCA Channel 2 Capture/Compare Mode @ 0xDC)
//------------------------------------------------------------------------------
#define PCA0CPM2_ECCF__BMASK    0x01 ///< Channel 2 Capture/Compare Flag Interrupt Enable
#define PCA0CPM2_ECCF__SHIFT    0x00 ///< Channel 2 Capture/Compare Flag Interrupt Enable
#define PCA0CPM2_ECCF__DISABLED 0x00 ///< Disable CCF2 interrupts.                       
#define PCA0CPM2_ECCF__ENABLED  0x01 ///< Enable a Capture/Compare Flag interrupt request
                                     ///< when CCF2 is set.                              
                                                                                         
#define PCA0CPM2_PWM__BMASK     0x02 ///< Channel 2 Pulse Width Modulation Mode Enable   
#define PCA0CPM2_PWM__SHIFT     0x01 ///< Channel 2 Pulse Width Modulation Mode Enable   
#define PCA0CPM2_PWM__DISABLED  0x00 ///< Disable PWM function.                          
#define PCA0CPM2_PWM__ENABLED   0x02 ///< Enable PWM function.                           
                                                                                         
#define PCA0CPM2_TOG__BMASK     0x04 ///< Channel 2 Toggle Function Enable               
#define PCA0CPM2_TOG__SHIFT     0x02 ///< Channel 2 Toggle Function Enable               
#define PCA0CPM2_TOG__DISABLED  0x00 ///< Disable toggle function.                       
#define PCA0CPM2_TOG__ENABLED   0x04 ///< Enable toggle function.                        
                                                                                         
#define PCA0CPM2_MAT__BMASK     0x08 ///< Channel 2 Match Function Enable                
#define PCA0CPM2_MAT__SHIFT     0x03 ///< Channel 2 Match Function Enable                
#define PCA0CPM2_MAT__DISABLED  0x00 ///< Disable match function.                        
#define PCA0CPM2_MAT__ENABLED   0x08 ///< Enable match function.                         
                                                                                         
#define PCA0CPM2_CAPN__BMASK    0x10 ///< Channel 2 Capture Negative Function Enable     
#define PCA0CPM2_CAPN__SHIFT    0x04 ///< Channel 2 Capture Negative Function Enable     
#define PCA0CPM2_CAPN__DISABLED 0x00 ///< Disable negative edge capture.                 
#define PCA0CPM2_CAPN__ENABLED  0x10 ///< Enable negative edge capture.                  
                                                                                         
#define PCA0CPM2_CAPP__BMASK    0x20 ///< Channel 2 Capture Positive Function Enable     
#define PCA0CPM2_CAPP__SHIFT    0x05 ///< Channel 2 Capture Positive Function Enable     
#define PCA0CPM2_CAPP__DISABLED 0x00 ///< Disable positive edge capture.                 
#define PCA0CPM2_CAPP__ENABLED  0x20 ///< Enable positive edge capture.                  
                                                                                         
#define PCA0CPM2_ECOM__BMASK    0x40 ///< Channel 2 Comparator Function Enable           
#define PCA0CPM2_ECOM__SHIFT    0x06 ///< Channel 2 Comparator Function Enable           
#define PCA0CPM2_ECOM__DISABLED 0x00 ///< Disable comparator function.                   
#define PCA0CPM2_ECOM__ENABLED  0x40 ///< Enable comparator function.                    
                                                                                         
#define PCA0CPM2_PWM16__BMASK   0x80 ///< Channel 2 16-bit Pulse Width Modulation Enable 
#define PCA0CPM2_PWM16__SHIFT   0x07 ///< Channel 2 16-bit Pulse Width Modulation Enable 
#define PCA0CPM2_PWM16__8_BIT   0x00 ///< 8 to 11-bit PWM selected.                      
#define PCA0CPM2_PWM16__16_BIT  0x80 ///< 16-bit PWM selected.                           
                                                                                         
//------------------------------------------------------------------------------
// PCA0CPH3 Enums (PCA Channel 3 Capture Module High Byte @ 0xF5)
//------------------------------------------------------------------------------
#define PCA0CPH3_PCA0CPH3__FMASK 0xFF ///< PCA Channel 3 Capture Module High Byte
#define PCA0CPH3_PCA0CPH3__SHIFT 0x00 ///< PCA Channel 3 Capture Module High Byte
                                                                                 
//------------------------------------------------------------------------------
// PCA0CPL3 Enums (PCA Channel 3 Capture Module Low Byte @ 0xF4)
//------------------------------------------------------------------------------
#define PCA0CPL3_PCA0CPL3__FMASK 0xFF ///< PCA Channel 3 Capture Module Low Byte
#define PCA0CPL3_PCA0CPL3__SHIFT 0x00 ///< PCA Channel 3 Capture Module Low Byte
                                                                                
//------------------------------------------------------------------------------
// PCA0CPM3 Enums (PCA Channel 3 Capture/Compare Mode @ 0xAE)
//------------------------------------------------------------------------------
#define PCA0CPM3_ECCF__BMASK    0x01 ///< Channel 3 Capture/Compare Flag Interrupt Enable
#define PCA0CPM3_ECCF__SHIFT    0x00 ///< Channel 3 Capture/Compare Flag Interrupt Enable
#define PCA0CPM3_ECCF__DISABLED 0x00 ///< Disable CCF3 interrupts.                       
#define PCA0CPM3_ECCF__ENABLED  0x01 ///< Enable a Capture/Compare Flag interrupt request
                                     ///< when CCF3 is set.                              
                                                                                         
#define PCA0CPM3_PWM__BMASK     0x02 ///< Channel 3 Pulse Width Modulation Mode Enable   
#define PCA0CPM3_PWM__SHIFT     0x01 ///< Channel 3 Pulse Width Modulation Mode Enable   
#define PCA0CPM3_PWM__DISABLED  0x00 ///< Disable PWM function.                          
#define PCA0CPM3_PWM__ENABLED   0x02 ///< Enable PWM function.                           
                                                                                         
#define PCA0CPM3_TOG__BMASK     0x04 ///< Channel 3 Toggle Function Enable               
#define PCA0CPM3_TOG__SHIFT     0x02 ///< Channel 3 Toggle Function Enable               
#define PCA0CPM3_TOG__DISABLED  0x00 ///< Disable toggle function.                       
#define PCA0CPM3_TOG__ENABLED   0x04 ///< Enable toggle function.                        
                                                                                         
#define PCA0CPM3_MAT__BMASK     0x08 ///< Channel 3 Match Function Enable                
#define PCA0CPM3_MAT__SHIFT     0x03 ///< Channel 3 Match Function Enable                
#define PCA0CPM3_MAT__DISABLED  0x00 ///< Disable match function.                        
#define PCA0CPM3_MAT__ENABLED   0x08 ///< Enable match function.                         
                                                                                         
#define PCA0CPM3_CAPN__BMASK    0x10 ///< Channel 3 Capture Negative Function Enable     
#define PCA0CPM3_CAPN__SHIFT    0x04 ///< Channel 3 Capture Negative Function Enable     
#define PCA0CPM3_CAPN__DISABLED 0x00 ///< Disable negative edge capture.                 
#define PCA0CPM3_CAPN__ENABLED  0x10 ///< Enable negative edge capture.                  
                                                                                         
#define PCA0CPM3_CAPP__BMASK    0x20 ///< Channel 3 Capture Positive Function Enable     
#define PCA0CPM3_CAPP__SHIFT    0x05 ///< Channel 3 Capture Positive Function Enable     
#define PCA0CPM3_CAPP__DISABLED 0x00 ///< Disable positive edge capture.                 
#define PCA0CPM3_CAPP__ENABLED  0x20 ///< Enable positive edge capture.                  
                                                                                         
#define PCA0CPM3_ECOM__BMASK    0x40 ///< Channel 3 Comparator Function Enable           
#define PCA0CPM3_ECOM__SHIFT    0x06 ///< Channel 3 Comparator Function Enable           
#define PCA0CPM3_ECOM__DISABLED 0x00 ///< Disable comparator function.                   
#define PCA0CPM3_ECOM__ENABLED  0x40 ///< Enable comparator function.                    
                                                                                         
#define PCA0CPM3_PWM16__BMASK   0x80 ///< Channel 3 16-bit Pulse Width Modulation Enable 
#define PCA0CPM3_PWM16__SHIFT   0x07 ///< Channel 3 16-bit Pulse Width Modulation Enable 
#define PCA0CPM3_PWM16__8_BIT   0x00 ///< 8 to 11-bit PWM selected.                      
#define PCA0CPM3_PWM16__16_BIT  0x80 ///< 16-bit PWM selected.                           
                                                                                         
//------------------------------------------------------------------------------
// PCA0CPH4 Enums (PCA Channel 4 Capture Module High Byte @ 0x85)
//------------------------------------------------------------------------------
#define PCA0CPH4_PCA0CPH4__FMASK 0xFF ///< PCA Channel 4 Capture Module High Byte
#define PCA0CPH4_PCA0CPH4__SHIFT 0x00 ///< PCA Channel 4 Capture Module High Byte
                                                                                 
//------------------------------------------------------------------------------
// PCA0CPL4 Enums (PCA Channel 4 Capture Module Low Byte @ 0x84)
//------------------------------------------------------------------------------
#define PCA0CPL4_PCA0CPL4__FMASK 0xFF ///< PCA Channel 4 Capture Module Low Byte
#define PCA0CPL4_PCA0CPL4__SHIFT 0x00 ///< PCA Channel 4 Capture Module Low Byte
                                                                                
//------------------------------------------------------------------------------
// PCA0CPM4 Enums (PCA Channel 4 Capture/Compare Mode @ 0xAF)
//------------------------------------------------------------------------------
#define PCA0CPM4_ECCF__BMASK    0x01 ///< Channel 4 Capture/Compare Flag Interrupt Enable
#define PCA0CPM4_ECCF__SHIFT    0x00 ///< Channel 4 Capture/Compare Flag Interrupt Enable
#define PCA0CPM4_ECCF__DISABLED 0x00 ///< Disable CCF4 interrupts.                       
#define PCA0CPM4_ECCF__ENABLED  0x01 ///< Enable a Capture/Compare Flag interrupt request
                                     ///< when CCF4 is set.                              
                                                                                         
#define PCA0CPM4_PWM__BMASK     0x02 ///< Channel 4 Pulse Width Modulation Mode Enable   
#define PCA0CPM4_PWM__SHIFT     0x01 ///< Channel 4 Pulse Width Modulation Mode Enable   
#define PCA0CPM4_PWM__DISABLED  0x00 ///< Disable PWM function.                          
#define PCA0CPM4_PWM__ENABLED   0x02 ///< Enable PWM function.                           
                                                                                         
#define PCA0CPM4_TOG__BMASK     0x04 ///< Channel 4 Toggle Function Enable               
#define PCA0CPM4_TOG__SHIFT     0x02 ///< Channel 4 Toggle Function Enable               
#define PCA0CPM4_TOG__DISABLED  0x00 ///< Disable toggle function.                       
#define PCA0CPM4_TOG__ENABLED   0x04 ///< Enable toggle function.                        
                                                                                         
#define PCA0CPM4_MAT__BMASK     0x08 ///< Channel 4 Match Function Enable                
#define PCA0CPM4_MAT__SHIFT     0x03 ///< Channel 4 Match Function Enable                
#define PCA0CPM4_MAT__DISABLED  0x00 ///< Disable match function.                        
#define PCA0CPM4_MAT__ENABLED   0x08 ///< Enable match function.                         
                                                                                         
#define PCA0CPM4_CAPN__BMASK    0x10 ///< Channel 4 Capture Negative Function Enable     
#define PCA0CPM4_CAPN__SHIFT    0x04 ///< Channel 4 Capture Negative Function Enable     
#define PCA0CPM4_CAPN__DISABLED 0x00 ///< Disable negative edge capture.                 
#define PCA0CPM4_CAPN__ENABLED  0x10 ///< Enable negative edge capture.                  
                                                                                         
#define PCA0CPM4_CAPP__BMASK    0x20 ///< Channel 4 Capture Positive Function Enable     
#define PCA0CPM4_CAPP__SHIFT    0x05 ///< Channel 4 Capture Positive Function Enable     
#define PCA0CPM4_CAPP__DISABLED 0x00 ///< Disable positive edge capture.                 
#define PCA0CPM4_CAPP__ENABLED  0x20 ///< Enable positive edge capture.                  
                                                                                         
#define PCA0CPM4_ECOM__BMASK    0x40 ///< Channel 4 Comparator Function Enable           
#define PCA0CPM4_ECOM__SHIFT    0x06 ///< Channel 4 Comparator Function Enable           
#define PCA0CPM4_ECOM__DISABLED 0x00 ///< Disable comparator function.                   
#define PCA0CPM4_ECOM__ENABLED  0x40 ///< Enable comparator function.                    
                                                                                         
#define PCA0CPM4_PWM16__BMASK   0x80 ///< Channel 4 16-bit Pulse Width Modulation Enable 
#define PCA0CPM4_PWM16__SHIFT   0x07 ///< Channel 4 16-bit Pulse Width Modulation Enable 
#define PCA0CPM4_PWM16__8_BIT   0x00 ///< 8 to 11-bit PWM selected.                      
#define PCA0CPM4_PWM16__16_BIT  0x80 ///< 16-bit PWM selected.                           
                                                                                         
//------------------------------------------------------------------------------
// PCA0CPH5 Enums (PCA Channel 5 Capture Module High Byte @ 0xDE)
//------------------------------------------------------------------------------
#define PCA0CPH5_PCA0CPH5__FMASK 0xFF ///< PCA Channel 5 Capture Module High Byte
#define PCA0CPH5_PCA0CPH5__SHIFT 0x00 ///< PCA Channel 5 Capture Module High Byte
                                                                                 
//------------------------------------------------------------------------------
// PCA0CPL5 Enums (PCA Channel 5 Capture Module Low Byte @ 0xDD)
//------------------------------------------------------------------------------
#define PCA0CPL5_PCA0CPL5__FMASK 0xFF ///< PCA Channel 5 Capture Module Low Byte
#define PCA0CPL5_PCA0CPL5__SHIFT 0x00 ///< PCA Channel 5 Capture Module Low Byte
                                                                                
//------------------------------------------------------------------------------
// PCA0CPM5 Enums (PCA Channel 5 Capture/Compare Mode @ 0xCC)
//------------------------------------------------------------------------------
#define PCA0CPM5_ECCF__BMASK    0x01 ///< Channel 5 Capture/Compare Flag Interrupt Enable
#define PCA0CPM5_ECCF__SHIFT    0x00 ///< Channel 5 Capture/Compare Flag Interrupt Enable
#define PCA0CPM5_ECCF__DISABLED 0x00 ///< Disable CCF5 interrupts.                       
#define PCA0CPM5_ECCF__ENABLED  0x01 ///< Enable a Capture/Compare Flag interrupt request
                                     ///< when CCF5 is set.                              
                                                                                         
#define PCA0CPM5_PWM__BMASK     0x02 ///< Channel 5 Pulse Width Modulation Mode Enable   
#define PCA0CPM5_PWM__SHIFT     0x01 ///< Channel 5 Pulse Width Modulation Mode Enable   
#define PCA0CPM5_PWM__DISABLED  0x00 ///< Disable PWM function.                          
#define PCA0CPM5_PWM__ENABLED   0x02 ///< Enable PWM function.                           
                                                                                         
#define PCA0CPM5_TOG__BMASK     0x04 ///< Channel 5 Toggle Function Enable               
#define PCA0CPM5_TOG__SHIFT     0x02 ///< Channel 5 Toggle Function Enable               
#define PCA0CPM5_TOG__DISABLED  0x00 ///< Disable toggle function.                       
#define PCA0CPM5_TOG__ENABLED   0x04 ///< Enable toggle function.                        
                                                                                         
#define PCA0CPM5_MAT__BMASK     0x08 ///< Channel 5 Match Function Enable                
#define PCA0CPM5_MAT__SHIFT     0x03 ///< Channel 5 Match Function Enable                
#define PCA0CPM5_MAT__DISABLED  0x00 ///< Disable match function.                        
#define PCA0CPM5_MAT__ENABLED   0x08 ///< Enable match function.                         
                                                                                         
#define PCA0CPM5_CAPN__BMASK    0x10 ///< Channel 5 Capture Negative Function Enable     
#define PCA0CPM5_CAPN__SHIFT    0x04 ///< Channel 5 Capture Negative Function Enable     
#define PCA0CPM5_CAPN__DISABLED 0x00 ///< Disable negative edge capture.                 
#define PCA0CPM5_CAPN__ENABLED  0x10 ///< Enable negative edge capture.                  
                                                                                         
#define PCA0CPM5_CAPP__BMASK    0x20 ///< Channel 5 Capture Positive Function Enable     
#define PCA0CPM5_CAPP__SHIFT    0x05 ///< Channel 5 Capture Positive Function Enable     
#define PCA0CPM5_CAPP__DISABLED 0x00 ///< Disable positive edge capture.                 
#define PCA0CPM5_CAPP__ENABLED  0x20 ///< Enable positive edge capture.                  
                                                                                         
#define PCA0CPM5_ECOM__BMASK    0x40 ///< Channel 5 Comparator Function Enable           
#define PCA0CPM5_ECOM__SHIFT    0x06 ///< Channel 5 Comparator Function Enable           
#define PCA0CPM5_ECOM__DISABLED 0x00 ///< Disable comparator function.                   
#define PCA0CPM5_ECOM__ENABLED  0x40 ///< Enable comparator function.                    
                                                                                         
#define PCA0CPM5_PWM16__BMASK   0x80 ///< Channel 5 16-bit Pulse Width Modulation Enable 
#define PCA0CPM5_PWM16__SHIFT   0x07 ///< Channel 5 16-bit Pulse Width Modulation Enable 
#define PCA0CPM5_PWM16__8_BIT   0x00 ///< 8 to 11-bit PWM selected.                      
#define PCA0CPM5_PWM16__16_BIT  0x80 ///< 16-bit PWM selected.                           
                                                                                         
//------------------------------------------------------------------------------
// PCA0CENT Enums (PCA Center Alignment Enable @ 0x9E)
//------------------------------------------------------------------------------
#define PCA0CENT_CEX0CEN__BMASK  0x01 ///< CEX0 Center Alignment Enable
#define PCA0CENT_CEX0CEN__SHIFT  0x00 ///< CEX0 Center Alignment Enable
#define PCA0CENT_CEX0CEN__EDGE   0x00 ///< Edge-aligned.               
#define PCA0CENT_CEX0CEN__CENTER 0x01 ///< Center-aligned.             
                                                                       
#define PCA0CENT_CEX1CEN__BMASK  0x02 ///< CEX1 Center Alignment Enable
#define PCA0CENT_CEX1CEN__SHIFT  0x01 ///< CEX1 Center Alignment Enable
#define PCA0CENT_CEX1CEN__EDGE   0x00 ///< Edge-aligned.               
#define PCA0CENT_CEX1CEN__CENTER 0x02 ///< Center-aligned.             
                                                                       
#define PCA0CENT_CEX2CEN__BMASK  0x04 ///< CEX2 Center Alignment Enable
#define PCA0CENT_CEX2CEN__SHIFT  0x02 ///< CEX2 Center Alignment Enable
#define PCA0CENT_CEX2CEN__EDGE   0x00 ///< Edge-aligned.               
#define PCA0CENT_CEX2CEN__CENTER 0x04 ///< Center-aligned.             
                                                                       
#define PCA0CENT_CEX3CEN__BMASK  0x08 ///< CEX3 Center Alignment Enable
#define PCA0CENT_CEX3CEN__SHIFT  0x03 ///< CEX3 Center Alignment Enable
#define PCA0CENT_CEX3CEN__EDGE   0x00 ///< Edge-aligned.               
#define PCA0CENT_CEX3CEN__CENTER 0x08 ///< Center-aligned.             
                                                                       
#define PCA0CENT_CEX4CEN__BMASK  0x10 ///< CEX4 Center Alignment Enable
#define PCA0CENT_CEX4CEN__SHIFT  0x04 ///< CEX4 Center Alignment Enable
#define PCA0CENT_CEX4CEN__EDGE   0x00 ///< Edge-aligned.               
#define PCA0CENT_CEX4CEN__CENTER 0x10 ///< Center-aligned.             
                                                                       
#define PCA0CENT_CEX5CEN__BMASK  0x20 ///< CEX5 Center Alignment Enable
#define PCA0CENT_CEX5CEN__SHIFT  0x05 ///< CEX5 Center Alignment Enable
#define PCA0CENT_CEX5CEN__EDGE   0x00 ///< Edge-aligned.               
#define PCA0CENT_CEX5CEN__CENTER 0x20 ///< Center-aligned.             
                                                                       
//------------------------------------------------------------------------------
// PCA0CLR Enums (PCA Comparator Clear Control @ 0x9C)
//------------------------------------------------------------------------------
#define PCA0CLR_CPCE0__BMASK    0x01 ///< Comparator Clear Enable for CEX0                  
#define PCA0CLR_CPCE0__SHIFT    0x00 ///< Comparator Clear Enable for CEX0                  
#define PCA0CLR_CPCE0__DISABLED 0x00 ///< Disable the comparator clear function on PCA      
                                     ///< channel 0.                                        
#define PCA0CLR_CPCE0__ENABLED  0x01 ///< Enable the comparator clear function on PCA       
                                     ///< channel 0.                                        
                                                                                            
#define PCA0CLR_CPCE1__BMASK    0x02 ///< Comparator Clear Enable for CEX1                  
#define PCA0CLR_CPCE1__SHIFT    0x01 ///< Comparator Clear Enable for CEX1                  
#define PCA0CLR_CPCE1__DISABLED 0x00 ///< Disable the comparator clear function on PCA      
                                     ///< channel 1.                                        
#define PCA0CLR_CPCE1__ENABLED  0x02 ///< Enable the comparator clear function on PCA       
                                     ///< channel 1.                                        
                                                                                            
#define PCA0CLR_CPCE2__BMASK    0x04 ///< Comparator Clear Enable for CEX2                  
#define PCA0CLR_CPCE2__SHIFT    0x02 ///< Comparator Clear Enable for CEX2                  
#define PCA0CLR_CPCE2__DISABLED 0x00 ///< Disable the comparator clear function on PCA      
                                     ///< channel 2.                                        
#define PCA0CLR_CPCE2__ENABLED  0x04 ///< Enable the comparator clear function on PCA       
                                     ///< channel 2.                                        
                                                                                            
#define PCA0CLR_CPCE3__BMASK    0x08 ///< Comparator Clear Enable for CEX3                  
#define PCA0CLR_CPCE3__SHIFT    0x03 ///< Comparator Clear Enable for CEX3                  
#define PCA0CLR_CPCE3__DISABLED 0x00 ///< Disable the comparator clear function on PCA      
                                     ///< channel 3.                                        
#define PCA0CLR_CPCE3__ENABLED  0x08 ///< Enable the comparator clear function on PCA       
                                     ///< channel 3.                                        
                                                                                            
#define PCA0CLR_CPCE4__BMASK    0x10 ///< Comparator Clear Enable for CEX4                  
#define PCA0CLR_CPCE4__SHIFT    0x04 ///< Comparator Clear Enable for CEX4                  
#define PCA0CLR_CPCE4__DISABLED 0x00 ///< Disable the comparator clear function on PCA      
                                     ///< channel 4.                                        
#define PCA0CLR_CPCE4__ENABLED  0x10 ///< Enable the comparator clear function on PCA       
                                     ///< channel 4.                                        
                                                                                            
#define PCA0CLR_CPCE5__BMASK    0x20 ///< Comparator Clear Enable for CEX5                  
#define PCA0CLR_CPCE5__SHIFT    0x05 ///< Comparator Clear Enable for CEX5                  
#define PCA0CLR_CPCE5__DISABLED 0x00 ///< Disable the comparator clear function on PCA      
                                     ///< channel 5.                                        
#define PCA0CLR_CPCE5__ENABLED  0x20 ///< Enable the comparator clear function on PCA       
                                     ///< channel 5.                                        
                                                                                            
#define PCA0CLR_CPCSEL__BMASK   0x40 ///< Comparator Clear Select                           
#define PCA0CLR_CPCSEL__SHIFT   0x06 ///< Comparator Clear Select                           
#define PCA0CLR_CPCSEL__CMP_0   0x00 ///< Comparator 0 will be used for the comparator clear
                                     ///< function.                                         
#define PCA0CLR_CPCSEL__CMP_1   0x40 ///< Comparator 1 will be used for the comparator clear
                                     ///< function.                                         
                                                                                            
#define PCA0CLR_CPCPOL__BMASK   0x80 ///< Comparator Clear Polarity                         
#define PCA0CLR_CPCPOL__SHIFT   0x07 ///< Comparator Clear Polarity                         
#define PCA0CLR_CPCPOL__LOW     0x00 ///< PCA channel(s) will be cleared when comparator    
                                     ///< result goes logic low.                            
#define PCA0CLR_CPCPOL__HIGH    0x80 ///< PCA channel(s) will be cleared when comparator    
                                     ///< result goes logic high.                           
                                                                                            
//------------------------------------------------------------------------------
// PCA0CN0 Enums (PCA Control @ 0xD8)
//------------------------------------------------------------------------------
#define PCA0CN0_CCF0__BMASK   0x01 ///< PCA Module 0 Capture/Compare Flag             
#define PCA0CN0_CCF0__SHIFT   0x00 ///< PCA Module 0 Capture/Compare Flag             
#define PCA0CN0_CCF0__NOT_SET 0x00 ///< A match or capture did not occur on channel 0.
#define PCA0CN0_CCF0__SET     0x01 ///< A match or capture occurred on channel 0.     
                                                                                      
#define PCA0CN0_CCF1__BMASK   0x02 ///< PCA Module 1 Capture/Compare Flag             
#define PCA0CN0_CCF1__SHIFT   0x01 ///< PCA Module 1 Capture/Compare Flag             
#define PCA0CN0_CCF1__NOT_SET 0x00 ///< A match or capture did not occur on channel 1.
#define PCA0CN0_CCF1__SET     0x02 ///< A match or capture occurred on channel 1.     
                                                                                      
#define PCA0CN0_CCF2__BMASK   0x04 ///< PCA Module 2 Capture/Compare Flag             
#define PCA0CN0_CCF2__SHIFT   0x02 ///< PCA Module 2 Capture/Compare Flag             
#define PCA0CN0_CCF2__NOT_SET 0x00 ///< A match or capture did not occur on channel 2.
#define PCA0CN0_CCF2__SET     0x04 ///< A match or capture occurred on channel 2.     
                                                                                      
#define PCA0CN0_CCF3__BMASK   0x08 ///< PCA Module 3 Capture/Compare Flag             
#define PCA0CN0_CCF3__SHIFT   0x03 ///< PCA Module 3 Capture/Compare Flag             
#define PCA0CN0_CCF3__NOT_SET 0x00 ///< A match or capture did not occur on channel 3.
#define PCA0CN0_CCF3__SET     0x08 ///< A match or capture occurred on channel 3.     
                                                                                      
#define PCA0CN0_CCF4__BMASK   0x10 ///< PCA Module 4 Capture/Compare Flag             
#define PCA0CN0_CCF4__SHIFT   0x04 ///< PCA Module 4 Capture/Compare Flag             
#define PCA0CN0_CCF4__NOT_SET 0x00 ///< A match or capture did not occur on channel 4.
#define PCA0CN0_CCF4__SET     0x10 ///< A match or capture occurred on channel 4.     
                                                                                      
#define PCA0CN0_CCF5__BMASK   0x20 ///< PCA Module 5 Capture/Compare Flag             
#define PCA0CN0_CCF5__SHIFT   0x05 ///< PCA Module 5 Capture/Compare Flag             
#define PCA0CN0_CCF5__NOT_SET 0x00 ///< A match or capture did not occur on channel 5.
#define PCA0CN0_CCF5__SET     0x20 ///< A match or capture occurred on channel 5.     
                                                                                      
#define PCA0CN0_CR__BMASK     0x40 ///< PCA Counter/Timer Run Control                 
#define PCA0CN0_CR__SHIFT     0x06 ///< PCA Counter/Timer Run Control                 
#define PCA0CN0_CR__STOP      0x00 ///< Stop the PCA Counter/Timer.                   
#define PCA0CN0_CR__RUN       0x40 ///< Start the PCA Counter/Timer running.          
                                                                                      
#define PCA0CN0_CF__BMASK     0x80 ///< PCA Counter/Timer Overflow Flag               
#define PCA0CN0_CF__SHIFT     0x07 ///< PCA Counter/Timer Overflow Flag               
#define PCA0CN0_CF__NOT_SET   0x00 ///< The PCA counter/timer did not overflow.       
#define PCA0CN0_CF__SET       0x80 ///< The PCA counter/timer overflowed.             
                                                                                      
//------------------------------------------------------------------------------
// PCA0H Enums (PCA Counter/Timer High Byte @ 0xFA)
//------------------------------------------------------------------------------
#define PCA0H_PCA0H__FMASK 0xFF ///< PCA Counter/Timer High Byte
#define PCA0H_PCA0H__SHIFT 0x00 ///< PCA Counter/Timer High Byte
                                                                
//------------------------------------------------------------------------------
// PCA0L Enums (PCA Counter/Timer Low Byte @ 0xF9)
//------------------------------------------------------------------------------
#define PCA0L_PCA0L__FMASK 0xFF ///< PCA Counter/Timer Low Byte
#define PCA0L_PCA0L__SHIFT 0x00 ///< PCA Counter/Timer Low Byte
                                                               
//------------------------------------------------------------------------------
// PCA0MD Enums (PCA Mode @ 0xD9)
//------------------------------------------------------------------------------
#define PCA0MD_ECF__BMASK            0x01 ///< PCA Counter/Timer Overflow Interrupt Enable       
#define PCA0MD_ECF__SHIFT            0x00 ///< PCA Counter/Timer Overflow Interrupt Enable       
#define PCA0MD_ECF__OVF_INT_DISABLED 0x00 ///< Disable the CF interrupt.                         
#define PCA0MD_ECF__OVF_INT_ENABLED  0x01 ///< Enable a PCA Counter/Timer Overflow interrupt     
                                          ///< request when CF is set.                           
                                                                                                 
#define PCA0MD_CPS__FMASK            0x0E ///< PCA Counter/Timer Pulse Select                    
#define PCA0MD_CPS__SHIFT            0x01 ///< PCA Counter/Timer Pulse Select                    
#define PCA0MD_CPS__SYSCLK_DIV_12    0x00 ///< System clock divided by 12.                       
#define PCA0MD_CPS__SYSCLK_DIV_4     0x02 ///< System clock divided by 4.                        
#define PCA0MD_CPS__T0_OVERFLOW      0x04 ///< Timer 0 overflow.                                 
#define PCA0MD_CPS__ECI              0x06 ///< High-to-low transitions on ECI (max rate = system 
                                          ///< clock divided by 4).                              
#define PCA0MD_CPS__SYSCLK           0x08 ///< System clock.                                     
#define PCA0MD_CPS__EXTOSC_DIV_8     0x0A ///< External clock divided by 8 (synchronized with the
                                          ///< system clock).                                    
#define PCA0MD_CPS__LFOSC_DIV_8      0x0C ///< Low frequency oscillator divided by 8.            
                                                                                                 
#define PCA0MD_CIDL__BMASK           0x80 ///< PCA Counter/Timer Idle Control                    
#define PCA0MD_CIDL__SHIFT           0x07 ///< PCA Counter/Timer Idle Control                    
#define PCA0MD_CIDL__NORMAL          0x00 ///< PCA continues to function normally while the      
                                          ///< system controller is in Idle Mode.                
#define PCA0MD_CIDL__SUSPEND         0x80 ///< PCA operation is suspended while the system       
                                          ///< controller is in Idle Mode.                       
                                                                                                 
//------------------------------------------------------------------------------
// PCA0POL Enums (PCA Output Polarity @ 0x96)
//------------------------------------------------------------------------------
#define PCA0POL_CEX0POL__BMASK   0x01 ///< CEX0 Output Polarity 
#define PCA0POL_CEX0POL__SHIFT   0x00 ///< CEX0 Output Polarity 
#define PCA0POL_CEX0POL__DEFAULT 0x00 ///< Use default polarity.
#define PCA0POL_CEX0POL__INVERT  0x01 ///< Invert polarity.     
                                                                
#define PCA0POL_CEX1POL__BMASK   0x02 ///< CEX1 Output Polarity 
#define PCA0POL_CEX1POL__SHIFT   0x01 ///< CEX1 Output Polarity 
#define PCA0POL_CEX1POL__DEFAULT 0x00 ///< Use default polarity.
#define PCA0POL_CEX1POL__INVERT  0x02 ///< Invert polarity.     
                                                                
#define PCA0POL_CEX2POL__BMASK   0x04 ///< CEX2 Output Polarity 
#define PCA0POL_CEX2POL__SHIFT   0x02 ///< CEX2 Output Polarity 
#define PCA0POL_CEX2POL__DEFAULT 0x00 ///< Use default polarity.
#define PCA0POL_CEX2POL__INVERT  0x04 ///< Invert polarity.     
                                                                
#define PCA0POL_CEX3POL__BMASK   0x08 ///< CEX3 Output Polarity 
#define PCA0POL_CEX3POL__SHIFT   0x03 ///< CEX3 Output Polarity 
#define PCA0POL_CEX3POL__DEFAULT 0x00 ///< Use default polarity.
#define PCA0POL_CEX3POL__INVERT  0x08 ///< Invert polarity.     
                                                                
#define PCA0POL_CEX4POL__BMASK   0x10 ///< CEX4 Output Polarity 
#define PCA0POL_CEX4POL__SHIFT   0x04 ///< CEX4 Output Polarity 
#define PCA0POL_CEX4POL__DEFAULT 0x00 ///< Use default polarity.
#define PCA0POL_CEX4POL__INVERT  0x10 ///< Invert polarity.     
                                                                
#define PCA0POL_CEX5POL__BMASK   0x20 ///< CEX5 Output Polarity 
#define PCA0POL_CEX5POL__SHIFT   0x05 ///< CEX5 Output Polarity 
#define PCA0POL_CEX5POL__DEFAULT 0x00 ///< Use default polarity.
#define PCA0POL_CEX5POL__INVERT  0x20 ///< Invert polarity.     
                                                                
//------------------------------------------------------------------------------
// PCA0PWM Enums (PCA PWM Configuration @ 0xF7)
//------------------------------------------------------------------------------
#define PCA0PWM_CLSEL__FMASK             0x07 ///< Cycle Length Select                              
#define PCA0PWM_CLSEL__SHIFT             0x00 ///< Cycle Length Select                              
#define PCA0PWM_CLSEL__8_BITS            0x00 ///< 8 bits.                                          
#define PCA0PWM_CLSEL__9_BITS            0x01 ///< 9 bits.                                          
#define PCA0PWM_CLSEL__10_BITS           0x02 ///< 10 bits.                                         
#define PCA0PWM_CLSEL__11_BITS           0x03 ///< 11 bits.                                         
                                                                                                    
#define PCA0PWM_COVF__BMASK              0x20 ///< Cycle Overflow Flag                              
#define PCA0PWM_COVF__SHIFT              0x05 ///< Cycle Overflow Flag                              
#define PCA0PWM_COVF__NO_OVERFLOW        0x00 ///< No overflow has occurred since the last time this
                                              ///< bit was cleared.                                 
#define PCA0PWM_COVF__OVERFLOW           0x20 ///< An overflow has occurred since the last time this
                                              ///< bit was cleared.                                 
                                                                                                    
#define PCA0PWM_ECOV__BMASK              0x40 ///< Cycle Overflow Interrupt Enable                  
#define PCA0PWM_ECOV__SHIFT              0x06 ///< Cycle Overflow Interrupt Enable                  
#define PCA0PWM_ECOV__COVF_MASK_DISABLED 0x00 ///< COVF will not generate PCA interrupts.           
#define PCA0PWM_ECOV__COVF_MASK_ENABLED  0x40 ///< A PCA interrupt will be generated when COVF is   
                                              ///< set.                                             
                                                                                                    
#define PCA0PWM_ARSEL__BMASK             0x80 ///< Auto-Reload Register Select                      
#define PCA0PWM_ARSEL__SHIFT             0x07 ///< Auto-Reload Register Select                      
#define PCA0PWM_ARSEL__CAPTURE_COMPARE   0x00 ///< Read/Write Capture/Compare Registers at PCA0CPHn 
                                              ///< and PCA0CPLn.                                    
#define PCA0PWM_ARSEL__AUTORELOAD        0x80 ///< Read/Write Auto-Reload Registers at PCA0CPHn and 
                                              ///< PCA0CPLn.                                        
                                                                                                    
//------------------------------------------------------------------------------
// PCON0 Enums (Power Control @ 0x87)
//------------------------------------------------------------------------------
#define PCON0_IDLE__BMASK  0x01 ///< Idle Mode Select                                
#define PCON0_IDLE__SHIFT  0x00 ///< Idle Mode Select                                
#define PCON0_IDLE__NORMAL 0x00 ///< Idle mode not activated.                        
#define PCON0_IDLE__IDLE   0x01 ///< CPU goes into Idle mode (shuts off clock to CPU,
                                ///< but clocks to enabled peripherals are still     
                                ///< active).                                        
                                                                                     
#define PCON0_STOP__BMASK  0x02 ///< Stop Mode Select                                
#define PCON0_STOP__SHIFT  0x01 ///< Stop Mode Select                                
#define PCON0_STOP__NORMAL 0x00 ///< Stop mode not activated.                        
#define PCON0_STOP__STOP   0x02 ///< CPU goes into Stop mode (internal oscillator    
                                ///< stopped).                                       
                                                                                     
#define PCON0_GF0__BMASK   0x04 ///< General Purpose Flag 0                          
#define PCON0_GF0__SHIFT   0x02 ///< General Purpose Flag 0                          
#define PCON0_GF0__NOT_SET 0x00 ///< The GF0 flag is not set. Clear the GF0 flag.    
#define PCON0_GF0__SET     0x04 ///< The GF0 flag is set. Set the GF0 flag.          
                                                                                     
#define PCON0_GF1__BMASK   0x08 ///< General Purpose Flag 1                          
#define PCON0_GF1__SHIFT   0x03 ///< General Purpose Flag 1                          
#define PCON0_GF1__NOT_SET 0x00 ///< The GF1 flag is not set. Clear the GF1 flag.    
#define PCON0_GF1__SET     0x08 ///< The GF1 flag is set. Set the GF1 flag.          
                                                                                     
#define PCON0_GF2__BMASK   0x10 ///< General Purpose Flag 2                          
#define PCON0_GF2__SHIFT   0x04 ///< General Purpose Flag 2                          
#define PCON0_GF2__NOT_SET 0x00 ///< The GF2 flag is not set. Clear the GF2 flag.    
#define PCON0_GF2__SET     0x10 ///< The GF2 flag is set. Set the GF2 flag.          
                                                                                     
#define PCON0_GF3__BMASK   0x20 ///< General Purpose Flag 3                          
#define PCON0_GF3__SHIFT   0x05 ///< General Purpose Flag 3                          
#define PCON0_GF3__NOT_SET 0x00 ///< The GF3 flag is not set. Clear the GF3 flag.    
#define PCON0_GF3__SET     0x20 ///< The GF3 flag is set. Set the GF3 flag.          
                                                                                     
#define PCON0_GF4__BMASK   0x40 ///< General Purpose Flag 4                          
#define PCON0_GF4__SHIFT   0x06 ///< General Purpose Flag 4                          
#define PCON0_GF4__NOT_SET 0x00 ///< The GF4 flag is not set. Clear the GF4 flag.    
#define PCON0_GF4__SET     0x40 ///< The GF4 flag is set. Set the GF4 flag.          
                                                                                     
#define PCON0_GF5__BMASK   0x80 ///< General Purpose Flag 5                          
#define PCON0_GF5__SHIFT   0x07 ///< General Purpose Flag 5                          
#define PCON0_GF5__NOT_SET 0x00 ///< The GF5 flag is not set. Clear the GF5 flag.    
#define PCON0_GF5__SET     0x80 ///< The GF5 flag is set. Set the GF5 flag.          
                                                                                     
//------------------------------------------------------------------------------
// PCON1 Enums (Power Control 1 @ 0xCD)
//------------------------------------------------------------------------------
#define PCON1_PINRSTMD__BMASK  0x01 ///< Pin Reset Mode                                    
#define PCON1_PINRSTMD__SHIFT  0x00 ///< Pin Reset Mode                                    
#define PCON1_PINRSTMD__RESET  0x00 ///< GPIO logic is reset when any reset event is       
                                    ///< asserted.                                         
#define PCON1_PINRSTMD__RETAIN 0x01 ///< Pins will retain state across any reset except for
                                    ///< power-on-reset events. Note that although pin     
                                    ///< configurations are maintained, the values of the  
                                    ///< pin control registers are reset. Registers PnMDIN,
                                    ///< PnMDOUT, Pn, and the XBARE bit may not reflect the
                                    ///< actual pin configuration at this time. New values 
                                    ///< written to these registers will take effect upon  
                                    ///< the write event.                                  
                                                                                           
#define PCON1_SUSPEND__BMASK   0x40 ///< Suspend Mode Select                               
#define PCON1_SUSPEND__SHIFT   0x06 ///< Suspend Mode Select                               
#define PCON1_SUSPEND__NORMAL  0x00 ///< Suspend mode not activated.                       
#define PCON1_SUSPEND__SUSPEND 0x40 ///< Device goes into suspend mode (high-frequency     
                                    ///< oscillators and SYSCLK are halted).               
                                                                                           
#define PCON1_SNOOZE__BMASK    0x80 ///< Snooze Mode Select                                
#define PCON1_SNOOZE__SHIFT    0x07 ///< Snooze Mode Select                                
#define PCON1_SNOOZE__NORMAL   0x00 ///< Snooze mode not activated.                        
#define PCON1_SNOOZE__SNOOZE   0x80 ///< Device goes into snooze mode (high-frequency      
                                    ///< oscillators and SYSCLK are halted, internal LDO in
                                    ///< low-power state).                                 
                                                                                           
//------------------------------------------------------------------------------
// PSTAT0 Enums (PMU Status 0 @ 0xAA)
//------------------------------------------------------------------------------
#define PSTAT0_CPT0WK__BMASK 0x01 ///< Comparator 0 Wake Flag      
#define PSTAT0_CPT0WK__SHIFT 0x00 ///< Comparator 0 Wake Flag      
                                                                   
#define PSTAT0_PMATWK__BMASK 0x02 ///< Port Match Wake Flag        
#define PSTAT0_PMATWK__SHIFT 0x01 ///< Port Match Wake Flag        
                                                                   
#define PSTAT0_TMR4WK__BMASK 0x04 ///< Timer 4 Wake Flag           
#define PSTAT0_TMR4WK__SHIFT 0x02 ///< Timer 4 Wake Flag           
                                                                   
#define PSTAT0_I2C0WK__BMASK 0x08 ///< I2C0 Slave Wake Flag        
#define PSTAT0_I2C0WK__SHIFT 0x03 ///< I2C0 Slave Wake Flag        
                                                                   
#define PSTAT0_SPI0WK__BMASK 0x10 ///< SPI0 Slave Wake Flag        
#define PSTAT0_SPI0WK__SHIFT 0x04 ///< SPI0 Slave Wake Flag        
                                                                   
#define PSTAT0_CL0WK__BMASK  0x20 ///< Configurable Logic Wake Flag
#define PSTAT0_CL0WK__SHIFT  0x05 ///< Configurable Logic Wake Flag
                                                                   
//------------------------------------------------------------------------------
// P0 Enums (Port 0 Pin Latch @ 0x80)
//------------------------------------------------------------------------------
#define P0_B0__BMASK 0x01 ///< Port 0 Bit 0 Latch                            
#define P0_B0__SHIFT 0x00 ///< Port 0 Bit 0 Latch                            
#define P0_B0__LOW   0x00 ///< P0.0 is low. Set P0.0 to drive low.           
#define P0_B0__HIGH  0x01 ///< P0.0 is high. Set P0.0 to drive or float high.
                                                                             
#define P0_B1__BMASK 0x02 ///< Port 0 Bit 1 Latch                            
#define P0_B1__SHIFT 0x01 ///< Port 0 Bit 1 Latch                            
#define P0_B1__LOW   0x00 ///< P0.1 is low. Set P0.1 to drive low.           
#define P0_B1__HIGH  0x02 ///< P0.1 is high. Set P0.1 to drive or float high.
                                                                             
#define P0_B2__BMASK 0x04 ///< Port 0 Bit 2 Latch                            
#define P0_B2__SHIFT 0x02 ///< Port 0 Bit 2 Latch                            
#define P0_B2__LOW   0x00 ///< P0.2 is low. Set P0.2 to drive low.           
#define P0_B2__HIGH  0x04 ///< P0.2 is high. Set P0.2 to drive or float high.
                                                                             
#define P0_B3__BMASK 0x08 ///< Port 0 Bit 3 Latch                            
#define P0_B3__SHIFT 0x03 ///< Port 0 Bit 3 Latch                            
#define P0_B3__LOW   0x00 ///< P0.3 is low. Set P0.3 to drive low.           
#define P0_B3__HIGH  0x08 ///< P0.3 is high. Set P0.3 to drive or float high.
                                                                             
#define P0_B4__BMASK 0x10 ///< Port 0 Bit 4 Latch                            
#define P0_B4__SHIFT 0x04 ///< Port 0 Bit 4 Latch                            
#define P0_B4__LOW   0x00 ///< P0.4 is low. Set P0.4 to drive low.           
#define P0_B4__HIGH  0x10 ///< P0.4 is high. Set P0.4 to drive or float high.
                                                                             
#define P0_B5__BMASK 0x20 ///< Port 0 Bit 5 Latch                            
#define P0_B5__SHIFT 0x05 ///< Port 0 Bit 5 Latch                            
#define P0_B5__LOW   0x00 ///< P0.5 is low. Set P0.5 to drive low.           
#define P0_B5__HIGH  0x20 ///< P0.5 is high. Set P0.5 to drive or float high.
                                                                             
#define P0_B6__BMASK 0x40 ///< Port 0 Bit 6 Latch                            
#define P0_B6__SHIFT 0x06 ///< Port 0 Bit 6 Latch                            
#define P0_B6__LOW   0x00 ///< P0.6 is low. Set P0.6 to drive low.           
#define P0_B6__HIGH  0x40 ///< P0.6 is high. Set P0.6 to drive or float high.
                                                                             
#define P0_B7__BMASK 0x80 ///< Port 0 Bit 7 Latch                            
#define P0_B7__SHIFT 0x07 ///< Port 0 Bit 7 Latch                            
#define P0_B7__LOW   0x00 ///< P0.7 is low. Set P0.7 to drive low.           
#define P0_B7__HIGH  0x80 ///< P0.7 is high. Set P0.7 to drive or float high.
                                                                             
//------------------------------------------------------------------------------
// P0MASK Enums (Port 0 Mask @ 0xFE)
//------------------------------------------------------------------------------
#define P0MASK_B0__BMASK    0x01 ///< Port 0 Bit 0 Mask Value                           
#define P0MASK_B0__SHIFT    0x00 ///< Port 0 Bit 0 Mask Value                           
#define P0MASK_B0__IGNORED  0x00 ///< P0.0 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P0MASK_B0__COMPARED 0x01 ///< P0.0 pin logic value is compared to P0MAT.0.      
                                                                                        
#define P0MASK_B1__BMASK    0x02 ///< Port 0 Bit 1 Mask Value                           
#define P0MASK_B1__SHIFT    0x01 ///< Port 0 Bit 1 Mask Value                           
#define P0MASK_B1__IGNORED  0x00 ///< P0.1 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P0MASK_B1__COMPARED 0x02 ///< P0.1 pin logic value is compared to P0MAT.1.      
                                                                                        
#define P0MASK_B2__BMASK    0x04 ///< Port 0 Bit 2 Mask Value                           
#define P0MASK_B2__SHIFT    0x02 ///< Port 0 Bit 2 Mask Value                           
#define P0MASK_B2__IGNORED  0x00 ///< P0.2 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P0MASK_B2__COMPARED 0x04 ///< P0.2 pin logic value is compared to P0MAT.2.      
                                                                                        
#define P0MASK_B3__BMASK    0x08 ///< Port 0 Bit 3 Mask Value                           
#define P0MASK_B3__SHIFT    0x03 ///< Port 0 Bit 3 Mask Value                           
#define P0MASK_B3__IGNORED  0x00 ///< P0.3 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P0MASK_B3__COMPARED 0x08 ///< P0.3 pin logic value is compared to P0MAT.3.      
                                                                                        
#define P0MASK_B4__BMASK    0x10 ///< Port 0 Bit 4 Mask Value                           
#define P0MASK_B4__SHIFT    0x04 ///< Port 0 Bit 4 Mask Value                           
#define P0MASK_B4__IGNORED  0x00 ///< P0.4 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P0MASK_B4__COMPARED 0x10 ///< P0.4 pin logic value is compared to P0MAT.4.      
                                                                                        
#define P0MASK_B5__BMASK    0x20 ///< Port 0 Bit 5 Mask Value                           
#define P0MASK_B5__SHIFT    0x05 ///< Port 0 Bit 5 Mask Value                           
#define P0MASK_B5__IGNORED  0x00 ///< P0.5 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P0MASK_B5__COMPARED 0x20 ///< P0.5 pin logic value is compared to P0MAT.5.      
                                                                                        
#define P0MASK_B6__BMASK    0x40 ///< Port 0 Bit 6 Mask Value                           
#define P0MASK_B6__SHIFT    0x06 ///< Port 0 Bit 6 Mask Value                           
#define P0MASK_B6__IGNORED  0x00 ///< P0.6 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P0MASK_B6__COMPARED 0x40 ///< P0.6 pin logic value is compared to P0MAT.6.      
                                                                                        
#define P0MASK_B7__BMASK    0x80 ///< Port 0 Bit 7 Mask Value                           
#define P0MASK_B7__SHIFT    0x07 ///< Port 0 Bit 7 Mask Value                           
#define P0MASK_B7__IGNORED  0x00 ///< P0.7 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P0MASK_B7__COMPARED 0x80 ///< P0.7 pin logic value is compared to P0MAT.7.      
                                                                                        
//------------------------------------------------------------------------------
// P0MAT Enums (Port 0 Match @ 0xFD)
//------------------------------------------------------------------------------
#define P0MAT_B0__BMASK 0x01 ///< Port 0 Bit 0 Match Value                         
#define P0MAT_B0__SHIFT 0x00 ///< Port 0 Bit 0 Match Value                         
#define P0MAT_B0__LOW   0x00 ///< P0.0 pin logic value is compared with logic LOW. 
#define P0MAT_B0__HIGH  0x01 ///< P0.0 pin logic value is compared with logic HIGH.
                                                                                   
#define P0MAT_B1__BMASK 0x02 ///< Port 0 Bit 1 Match Value                         
#define P0MAT_B1__SHIFT 0x01 ///< Port 0 Bit 1 Match Value                         
#define P0MAT_B1__LOW   0x00 ///< P0.1 pin logic value is compared with logic LOW. 
#define P0MAT_B1__HIGH  0x02 ///< P0.1 pin logic value is compared with logic HIGH.
                                                                                   
#define P0MAT_B2__BMASK 0x04 ///< Port 0 Bit 2 Match Value                         
#define P0MAT_B2__SHIFT 0x02 ///< Port 0 Bit 2 Match Value                         
#define P0MAT_B2__LOW   0x00 ///< P0.2 pin logic value is compared with logic LOW. 
#define P0MAT_B2__HIGH  0x04 ///< P0.2 pin logic value is compared with logic HIGH.
                                                                                   
#define P0MAT_B3__BMASK 0x08 ///< Port 0 Bit 3 Match Value                         
#define P0MAT_B3__SHIFT 0x03 ///< Port 0 Bit 3 Match Value                         
#define P0MAT_B3__LOW   0x00 ///< P0.3 pin logic value is compared with logic LOW. 
#define P0MAT_B3__HIGH  0x08 ///< P0.3 pin logic value is compared with logic HIGH.
                                                                                   
#define P0MAT_B4__BMASK 0x10 ///< Port 0 Bit 4 Match Value                         
#define P0MAT_B4__SHIFT 0x04 ///< Port 0 Bit 4 Match Value                         
#define P0MAT_B4__LOW   0x00 ///< P0.4 pin logic value is compared with logic LOW. 
#define P0MAT_B4__HIGH  0x10 ///< P0.4 pin logic value is compared with logic HIGH.
                                                                                   
#define P0MAT_B5__BMASK 0x20 ///< Port 0 Bit 5 Match Value                         
#define P0MAT_B5__SHIFT 0x05 ///< Port 0 Bit 5 Match Value                         
#define P0MAT_B5__LOW   0x00 ///< P0.5 pin logic value is compared with logic LOW. 
#define P0MAT_B5__HIGH  0x20 ///< P0.5 pin logic value is compared with logic HIGH.
                                                                                   
#define P0MAT_B6__BMASK 0x40 ///< Port 0 Bit 6 Match Value                         
#define P0MAT_B6__SHIFT 0x06 ///< Port 0 Bit 6 Match Value                         
#define P0MAT_B6__LOW   0x00 ///< P0.6 pin logic value is compared with logic LOW. 
#define P0MAT_B6__HIGH  0x40 ///< P0.6 pin logic value is compared with logic HIGH.
                                                                                   
#define P0MAT_B7__BMASK 0x80 ///< Port 0 Bit 7 Match Value                         
#define P0MAT_B7__SHIFT 0x07 ///< Port 0 Bit 7 Match Value                         
#define P0MAT_B7__LOW   0x00 ///< P0.7 pin logic value is compared with logic LOW. 
#define P0MAT_B7__HIGH  0x80 ///< P0.7 pin logic value is compared with logic HIGH.
                                                                                   
//------------------------------------------------------------------------------
// P0MDIN Enums (Port 0 Input Mode @ 0xF1)
//------------------------------------------------------------------------------
#define P0MDIN_B0__BMASK   0x01 ///< Port 0 Bit 0 Input Mode                 
#define P0MDIN_B0__SHIFT   0x00 ///< Port 0 Bit 0 Input Mode                 
#define P0MDIN_B0__ANALOG  0x00 ///< P0.0 pin is configured for analog mode. 
#define P0MDIN_B0__DIGITAL 0x01 ///< P0.0 pin is configured for digital mode.
                                                                             
#define P0MDIN_B1__BMASK   0x02 ///< Port 0 Bit 1 Input Mode                 
#define P0MDIN_B1__SHIFT   0x01 ///< Port 0 Bit 1 Input Mode                 
#define P0MDIN_B1__ANALOG  0x00 ///< P0.1 pin is configured for analog mode. 
#define P0MDIN_B1__DIGITAL 0x02 ///< P0.1 pin is configured for digital mode.
                                                                             
#define P0MDIN_B2__BMASK   0x04 ///< Port 0 Bit 2 Input Mode                 
#define P0MDIN_B2__SHIFT   0x02 ///< Port 0 Bit 2 Input Mode                 
#define P0MDIN_B2__ANALOG  0x00 ///< P0.2 pin is configured for analog mode. 
#define P0MDIN_B2__DIGITAL 0x04 ///< P0.2 pin is configured for digital mode.
                                                                             
#define P0MDIN_B3__BMASK   0x08 ///< Port 0 Bit 3 Input Mode                 
#define P0MDIN_B3__SHIFT   0x03 ///< Port 0 Bit 3 Input Mode                 
#define P0MDIN_B3__ANALOG  0x00 ///< P0.3 pin is configured for analog mode. 
#define P0MDIN_B3__DIGITAL 0x08 ///< P0.3 pin is configured for digital mode.
                                                                             
#define P0MDIN_B4__BMASK   0x10 ///< Port 0 Bit 4 Input Mode                 
#define P0MDIN_B4__SHIFT   0x04 ///< Port 0 Bit 4 Input Mode                 
#define P0MDIN_B4__ANALOG  0x00 ///< P0.4 pin is configured for analog mode. 
#define P0MDIN_B4__DIGITAL 0x10 ///< P0.4 pin is configured for digital mode.
                                                                             
#define P0MDIN_B5__BMASK   0x20 ///< Port 0 Bit 5 Input Mode                 
#define P0MDIN_B5__SHIFT   0x05 ///< Port 0 Bit 5 Input Mode                 
#define P0MDIN_B5__ANALOG  0x00 ///< P0.5 pin is configured for analog mode. 
#define P0MDIN_B5__DIGITAL 0x20 ///< P0.5 pin is configured for digital mode.
                                                                             
#define P0MDIN_B6__BMASK   0x40 ///< Port 0 Bit 6 Input Mode                 
#define P0MDIN_B6__SHIFT   0x06 ///< Port 0 Bit 6 Input Mode                 
#define P0MDIN_B6__ANALOG  0x00 ///< P0.6 pin is configured for analog mode. 
#define P0MDIN_B6__DIGITAL 0x40 ///< P0.6 pin is configured for digital mode.
                                                                             
#define P0MDIN_B7__BMASK   0x80 ///< Port 0 Bit 7 Input Mode                 
#define P0MDIN_B7__SHIFT   0x07 ///< Port 0 Bit 7 Input Mode                 
#define P0MDIN_B7__ANALOG  0x00 ///< P0.7 pin is configured for analog mode. 
#define P0MDIN_B7__DIGITAL 0x80 ///< P0.7 pin is configured for digital mode.
                                                                             
//------------------------------------------------------------------------------
// P0MDOUT Enums (Port 0 Output Mode @ 0xA4)
//------------------------------------------------------------------------------
#define P0MDOUT_B0__BMASK      0x01 ///< Port 0 Bit 0 Output Mode  
#define P0MDOUT_B0__SHIFT      0x00 ///< Port 0 Bit 0 Output Mode  
#define P0MDOUT_B0__OPEN_DRAIN 0x00 ///< P0.0 output is open-drain.
#define P0MDOUT_B0__PUSH_PULL  0x01 ///< P0.0 output is push-pull. 
                                                                   
#define P0MDOUT_B1__BMASK      0x02 ///< Port 0 Bit 1 Output Mode  
#define P0MDOUT_B1__SHIFT      0x01 ///< Port 0 Bit 1 Output Mode  
#define P0MDOUT_B1__OPEN_DRAIN 0x00 ///< P0.1 output is open-drain.
#define P0MDOUT_B1__PUSH_PULL  0x02 ///< P0.1 output is push-pull. 
                                                                   
#define P0MDOUT_B2__BMASK      0x04 ///< Port 0 Bit 2 Output Mode  
#define P0MDOUT_B2__SHIFT      0x02 ///< Port 0 Bit 2 Output Mode  
#define P0MDOUT_B2__OPEN_DRAIN 0x00 ///< P0.2 output is open-drain.
#define P0MDOUT_B2__PUSH_PULL  0x04 ///< P0.2 output is push-pull. 
                                                                   
#define P0MDOUT_B3__BMASK      0x08 ///< Port 0 Bit 3 Output Mode  
#define P0MDOUT_B3__SHIFT      0x03 ///< Port 0 Bit 3 Output Mode  
#define P0MDOUT_B3__OPEN_DRAIN 0x00 ///< P0.3 output is open-drain.
#define P0MDOUT_B3__PUSH_PULL  0x08 ///< P0.3 output is push-pull. 
                                                                   
#define P0MDOUT_B4__BMASK      0x10 ///< Port 0 Bit 4 Output Mode  
#define P0MDOUT_B4__SHIFT      0x04 ///< Port 0 Bit 4 Output Mode  
#define P0MDOUT_B4__OPEN_DRAIN 0x00 ///< P0.4 output is open-drain.
#define P0MDOUT_B4__PUSH_PULL  0x10 ///< P0.4 output is push-pull. 
                                                                   
#define P0MDOUT_B5__BMASK      0x20 ///< Port 0 Bit 5 Output Mode  
#define P0MDOUT_B5__SHIFT      0x05 ///< Port 0 Bit 5 Output Mode  
#define P0MDOUT_B5__OPEN_DRAIN 0x00 ///< P0.5 output is open-drain.
#define P0MDOUT_B5__PUSH_PULL  0x20 ///< P0.5 output is push-pull. 
                                                                   
#define P0MDOUT_B6__BMASK      0x40 ///< Port 0 Bit 6 Output Mode  
#define P0MDOUT_B6__SHIFT      0x06 ///< Port 0 Bit 6 Output Mode  
#define P0MDOUT_B6__OPEN_DRAIN 0x00 ///< P0.6 output is open-drain.
#define P0MDOUT_B6__PUSH_PULL  0x40 ///< P0.6 output is push-pull. 
                                                                   
#define P0MDOUT_B7__BMASK      0x80 ///< Port 0 Bit 7 Output Mode  
#define P0MDOUT_B7__SHIFT      0x07 ///< Port 0 Bit 7 Output Mode  
#define P0MDOUT_B7__OPEN_DRAIN 0x00 ///< P0.7 output is open-drain.
#define P0MDOUT_B7__PUSH_PULL  0x80 ///< P0.7 output is push-pull. 
                                                                   
//------------------------------------------------------------------------------
// P0SKIP Enums (Port 0 Skip @ 0xD4)
//------------------------------------------------------------------------------
#define P0SKIP_B0__BMASK       0x01 ///< Port 0 Bit 0 Skip                       
#define P0SKIP_B0__SHIFT       0x00 ///< Port 0 Bit 0 Skip                       
#define P0SKIP_B0__NOT_SKIPPED 0x00 ///< P0.0 pin is not skipped by the crossbar.
#define P0SKIP_B0__SKIPPED     0x01 ///< P0.0 pin is skipped by the crossbar.    
                                                                                 
#define P0SKIP_B1__BMASK       0x02 ///< Port 0 Bit 1 Skip                       
#define P0SKIP_B1__SHIFT       0x01 ///< Port 0 Bit 1 Skip                       
#define P0SKIP_B1__NOT_SKIPPED 0x00 ///< P0.1 pin is not skipped by the crossbar.
#define P0SKIP_B1__SKIPPED     0x02 ///< P0.1 pin is skipped by the crossbar.    
                                                                                 
#define P0SKIP_B2__BMASK       0x04 ///< Port 0 Bit 2 Skip                       
#define P0SKIP_B2__SHIFT       0x02 ///< Port 0 Bit 2 Skip                       
#define P0SKIP_B2__NOT_SKIPPED 0x00 ///< P0.2 pin is not skipped by the crossbar.
#define P0SKIP_B2__SKIPPED     0x04 ///< P0.2 pin is skipped by the crossbar.    
                                                                                 
#define P0SKIP_B3__BMASK       0x08 ///< Port 0 Bit 3 Skip                       
#define P0SKIP_B3__SHIFT       0x03 ///< Port 0 Bit 3 Skip                       
#define P0SKIP_B3__NOT_SKIPPED 0x00 ///< P0.3 pin is not skipped by the crossbar.
#define P0SKIP_B3__SKIPPED     0x08 ///< P0.3 pin is skipped by the crossbar.    
                                                                                 
#define P0SKIP_B4__BMASK       0x10 ///< Port 0 Bit 4 Skip                       
#define P0SKIP_B4__SHIFT       0x04 ///< Port 0 Bit 4 Skip                       
#define P0SKIP_B4__NOT_SKIPPED 0x00 ///< P0.4 pin is not skipped by the crossbar.
#define P0SKIP_B4__SKIPPED     0x10 ///< P0.4 pin is skipped by the crossbar.    
                                                                                 
#define P0SKIP_B5__BMASK       0x20 ///< Port 0 Bit 5 Skip                       
#define P0SKIP_B5__SHIFT       0x05 ///< Port 0 Bit 5 Skip                       
#define P0SKIP_B5__NOT_SKIPPED 0x00 ///< P0.5 pin is not skipped by the crossbar.
#define P0SKIP_B5__SKIPPED     0x20 ///< P0.5 pin is skipped by the crossbar.    
                                                                                 
#define P0SKIP_B6__BMASK       0x40 ///< Port 0 Bit 6 Skip                       
#define P0SKIP_B6__SHIFT       0x06 ///< Port 0 Bit 6 Skip                       
#define P0SKIP_B6__NOT_SKIPPED 0x00 ///< P0.6 pin is not skipped by the crossbar.
#define P0SKIP_B6__SKIPPED     0x40 ///< P0.6 pin is skipped by the crossbar.    
                                                                                 
#define P0SKIP_B7__BMASK       0x80 ///< Port 0 Bit 7 Skip                       
#define P0SKIP_B7__SHIFT       0x07 ///< Port 0 Bit 7 Skip                       
#define P0SKIP_B7__NOT_SKIPPED 0x00 ///< P0.7 pin is not skipped by the crossbar.
#define P0SKIP_B7__SKIPPED     0x80 ///< P0.7 pin is skipped by the crossbar.    
                                                                                 
//------------------------------------------------------------------------------
// P1 Enums (Port 1 Pin Latch @ 0x90)
//------------------------------------------------------------------------------
#define P1_B0__BMASK 0x01 ///< Port 1 Bit 0 Latch                            
#define P1_B0__SHIFT 0x00 ///< Port 1 Bit 0 Latch                            
#define P1_B0__LOW   0x00 ///< P1.0 is low. Set P1.0 to drive low.           
#define P1_B0__HIGH  0x01 ///< P1.0 is high. Set P1.0 to drive or float high.
                                                                             
#define P1_B1__BMASK 0x02 ///< Port 1 Bit 1 Latch                            
#define P1_B1__SHIFT 0x01 ///< Port 1 Bit 1 Latch                            
#define P1_B1__LOW   0x00 ///< P1.1 is low. Set P1.1 to drive low.           
#define P1_B1__HIGH  0x02 ///< P1.1 is high. Set P1.1 to drive or float high.
                                                                             
#define P1_B2__BMASK 0x04 ///< Port 1 Bit 2 Latch                            
#define P1_B2__SHIFT 0x02 ///< Port 1 Bit 2 Latch                            
#define P1_B2__LOW   0x00 ///< P1.2 is low. Set P1.2 to drive low.           
#define P1_B2__HIGH  0x04 ///< P1.2 is high. Set P1.2 to drive or float high.
                                                                             
#define P1_B3__BMASK 0x08 ///< Port 1 Bit 3 Latch                            
#define P1_B3__SHIFT 0x03 ///< Port 1 Bit 3 Latch                            
#define P1_B3__LOW   0x00 ///< P1.3 is low. Set P1.3 to drive low.           
#define P1_B3__HIGH  0x08 ///< P1.3 is high. Set P1.3 to drive or float high.
                                                                             
#define P1_B4__BMASK 0x10 ///< Port 1 Bit 4 Latch                            
#define P1_B4__SHIFT 0x04 ///< Port 1 Bit 4 Latch                            
#define P1_B4__LOW   0x00 ///< P1.4 is low. Set P1.4 to drive low.           
#define P1_B4__HIGH  0x10 ///< P1.4 is high. Set P1.4 to drive or float high.
                                                                             
#define P1_B5__BMASK 0x20 ///< Port 1 Bit 5 Latch                            
#define P1_B5__SHIFT 0x05 ///< Port 1 Bit 5 Latch                            
#define P1_B5__LOW   0x00 ///< P1.5 is low. Set P1.5 to drive low.           
#define P1_B5__HIGH  0x20 ///< P1.5 is high. Set P1.5 to drive or float high.
                                                                             
#define P1_B6__BMASK 0x40 ///< Port 1 Bit 6 Latch                            
#define P1_B6__SHIFT 0x06 ///< Port 1 Bit 6 Latch                            
#define P1_B6__LOW   0x00 ///< P1.6 is low. Set P1.6 to drive low.           
#define P1_B6__HIGH  0x40 ///< P1.6 is high. Set P1.6 to drive or float high.
                                                                             
#define P1_B7__BMASK 0x80 ///< Port 1 Bit 7 Latch                            
#define P1_B7__SHIFT 0x07 ///< Port 1 Bit 7 Latch                            
#define P1_B7__LOW   0x00 ///< P1.7 is low. Set P1.7 to drive low.           
#define P1_B7__HIGH  0x80 ///< P1.7 is high. Set P1.7 to drive or float high.
                                                                             
//------------------------------------------------------------------------------
// P1MASK Enums (Port 1 Mask @ 0xEE)
//------------------------------------------------------------------------------
#define P1MASK_B0__BMASK    0x01 ///< Port 1 Bit 0 Mask Value                           
#define P1MASK_B0__SHIFT    0x00 ///< Port 1 Bit 0 Mask Value                           
#define P1MASK_B0__IGNORED  0x00 ///< P1.0 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P1MASK_B0__COMPARED 0x01 ///< P1.0 pin logic value is compared to P1MAT.0.      
                                                                                        
#define P1MASK_B1__BMASK    0x02 ///< Port 1 Bit 1 Mask Value                           
#define P1MASK_B1__SHIFT    0x01 ///< Port 1 Bit 1 Mask Value                           
#define P1MASK_B1__IGNORED  0x00 ///< P1.1 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P1MASK_B1__COMPARED 0x02 ///< P1.1 pin logic value is compared to P1MAT.1.      
                                                                                        
#define P1MASK_B2__BMASK    0x04 ///< Port 1 Bit 2 Mask Value                           
#define P1MASK_B2__SHIFT    0x02 ///< Port 1 Bit 2 Mask Value                           
#define P1MASK_B2__IGNORED  0x00 ///< P1.2 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P1MASK_B2__COMPARED 0x04 ///< P1.2 pin logic value is compared to P1MAT.2.      
                                                                                        
#define P1MASK_B3__BMASK    0x08 ///< Port 1 Bit 3 Mask Value                           
#define P1MASK_B3__SHIFT    0x03 ///< Port 1 Bit 3 Mask Value                           
#define P1MASK_B3__IGNORED  0x00 ///< P1.3 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P1MASK_B3__COMPARED 0x08 ///< P1.3 pin logic value is compared to P1MAT.3.      
                                                                                        
#define P1MASK_B4__BMASK    0x10 ///< Port 1 Bit 4 Mask Value                           
#define P1MASK_B4__SHIFT    0x04 ///< Port 1 Bit 4 Mask Value                           
#define P1MASK_B4__IGNORED  0x00 ///< P1.4 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P1MASK_B4__COMPARED 0x10 ///< P1.4 pin logic value is compared to P1MAT.4.      
                                                                                        
#define P1MASK_B5__BMASK    0x20 ///< Port 1 Bit 5 Mask Value                           
#define P1MASK_B5__SHIFT    0x05 ///< Port 1 Bit 5 Mask Value                           
#define P1MASK_B5__IGNORED  0x00 ///< P1.5 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P1MASK_B5__COMPARED 0x20 ///< P1.5 pin logic value is compared to P1MAT.5.      
                                                                                        
#define P1MASK_B6__BMASK    0x40 ///< Port 1 Bit 6 Mask Value                           
#define P1MASK_B6__SHIFT    0x06 ///< Port 1 Bit 6 Mask Value                           
#define P1MASK_B6__IGNORED  0x00 ///< P1.6 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P1MASK_B6__COMPARED 0x40 ///< P1.6 pin logic value is compared to P1MAT.6.      
                                                                                        
#define P1MASK_B7__BMASK    0x80 ///< Port 1 Bit 7 Mask Value                           
#define P1MASK_B7__SHIFT    0x07 ///< Port 1 Bit 7 Mask Value                           
#define P1MASK_B7__IGNORED  0x00 ///< P1.7 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P1MASK_B7__COMPARED 0x80 ///< P1.7 pin logic value is compared to P1MAT.7.      
                                                                                        
//------------------------------------------------------------------------------
// P1MAT Enums (Port 1 Match @ 0xED)
//------------------------------------------------------------------------------
#define P1MAT_B0__BMASK 0x01 ///< Port 1 Bit 0 Match Value                         
#define P1MAT_B0__SHIFT 0x00 ///< Port 1 Bit 0 Match Value                         
#define P1MAT_B0__LOW   0x00 ///< P1.0 pin logic value is compared with logic LOW. 
#define P1MAT_B0__HIGH  0x01 ///< P1.0 pin logic value is compared with logic HIGH.
                                                                                   
#define P1MAT_B1__BMASK 0x02 ///< Port 1 Bit 1 Match Value                         
#define P1MAT_B1__SHIFT 0x01 ///< Port 1 Bit 1 Match Value                         
#define P1MAT_B1__LOW   0x00 ///< P1.1 pin logic value is compared with logic LOW. 
#define P1MAT_B1__HIGH  0x02 ///< P1.1 pin logic value is compared with logic HIGH.
                                                                                   
#define P1MAT_B2__BMASK 0x04 ///< Port 1 Bit 2 Match Value                         
#define P1MAT_B2__SHIFT 0x02 ///< Port 1 Bit 2 Match Value                         
#define P1MAT_B2__LOW   0x00 ///< P1.2 pin logic value is compared with logic LOW. 
#define P1MAT_B2__HIGH  0x04 ///< P1.2 pin logic value is compared with logic HIGH.
                                                                                   
#define P1MAT_B3__BMASK 0x08 ///< Port 1 Bit 3 Match Value                         
#define P1MAT_B3__SHIFT 0x03 ///< Port 1 Bit 3 Match Value                         
#define P1MAT_B3__LOW   0x00 ///< P1.3 pin logic value is compared with logic LOW. 
#define P1MAT_B3__HIGH  0x08 ///< P1.3 pin logic value is compared with logic HIGH.
                                                                                   
#define P1MAT_B4__BMASK 0x10 ///< Port 1 Bit 4 Match Value                         
#define P1MAT_B4__SHIFT 0x04 ///< Port 1 Bit 4 Match Value                         
#define P1MAT_B4__LOW   0x00 ///< P1.4 pin logic value is compared with logic LOW. 
#define P1MAT_B4__HIGH  0x10 ///< P1.4 pin logic value is compared with logic HIGH.
                                                                                   
#define P1MAT_B5__BMASK 0x20 ///< Port 1 Bit 5 Match Value                         
#define P1MAT_B5__SHIFT 0x05 ///< Port 1 Bit 5 Match Value                         
#define P1MAT_B5__LOW   0x00 ///< P1.5 pin logic value is compared with logic LOW. 
#define P1MAT_B5__HIGH  0x20 ///< P1.5 pin logic value is compared with logic HIGH.
                                                                                   
#define P1MAT_B6__BMASK 0x40 ///< Port 1 Bit 6 Match Value                         
#define P1MAT_B6__SHIFT 0x06 ///< Port 1 Bit 6 Match Value                         
#define P1MAT_B6__LOW   0x00 ///< P1.6 pin logic value is compared with logic LOW. 
#define P1MAT_B6__HIGH  0x40 ///< P1.6 pin logic value is compared with logic HIGH.
                                                                                   
#define P1MAT_B7__BMASK 0x80 ///< Port 1 Bit 7 Match Value                         
#define P1MAT_B7__SHIFT 0x07 ///< Port 1 Bit 7 Match Value                         
#define P1MAT_B7__LOW   0x00 ///< P1.7 pin logic value is compared with logic LOW. 
#define P1MAT_B7__HIGH  0x80 ///< P1.7 pin logic value is compared with logic HIGH.
                                                                                   
//------------------------------------------------------------------------------
// P1MDIN Enums (Port 1 Input Mode @ 0xF2)
//------------------------------------------------------------------------------
#define P1MDIN_B0__BMASK   0x01 ///< Port 1 Bit 0 Input Mode                 
#define P1MDIN_B0__SHIFT   0x00 ///< Port 1 Bit 0 Input Mode                 
#define P1MDIN_B0__ANALOG  0x00 ///< P1.0 pin is configured for analog mode. 
#define P1MDIN_B0__DIGITAL 0x01 ///< P1.0 pin is configured for digital mode.
                                                                             
#define P1MDIN_B1__BMASK   0x02 ///< Port 1 Bit 1 Input Mode                 
#define P1MDIN_B1__SHIFT   0x01 ///< Port 1 Bit 1 Input Mode                 
#define P1MDIN_B1__ANALOG  0x00 ///< P1.1 pin is configured for analog mode. 
#define P1MDIN_B1__DIGITAL 0x02 ///< P1.1 pin is configured for digital mode.
                                                                             
#define P1MDIN_B2__BMASK   0x04 ///< Port 1 Bit 2 Input Mode                 
#define P1MDIN_B2__SHIFT   0x02 ///< Port 1 Bit 2 Input Mode                 
#define P1MDIN_B2__ANALOG  0x00 ///< P1.2 pin is configured for analog mode. 
#define P1MDIN_B2__DIGITAL 0x04 ///< P1.2 pin is configured for digital mode.
                                                                             
#define P1MDIN_B3__BMASK   0x08 ///< Port 1 Bit 3 Input Mode                 
#define P1MDIN_B3__SHIFT   0x03 ///< Port 1 Bit 3 Input Mode                 
#define P1MDIN_B3__ANALOG  0x00 ///< P1.3 pin is configured for analog mode. 
#define P1MDIN_B3__DIGITAL 0x08 ///< P1.3 pin is configured for digital mode.
                                                                             
#define P1MDIN_B4__BMASK   0x10 ///< Port 1 Bit 4 Input Mode                 
#define P1MDIN_B4__SHIFT   0x04 ///< Port 1 Bit 4 Input Mode                 
#define P1MDIN_B4__ANALOG  0x00 ///< P1.4 pin is configured for analog mode. 
#define P1MDIN_B4__DIGITAL 0x10 ///< P1.4 pin is configured for digital mode.
                                                                             
#define P1MDIN_B5__BMASK   0x20 ///< Port 1 Bit 5 Input Mode                 
#define P1MDIN_B5__SHIFT   0x05 ///< Port 1 Bit 5 Input Mode                 
#define P1MDIN_B5__ANALOG  0x00 ///< P1.5 pin is configured for analog mode. 
#define P1MDIN_B5__DIGITAL 0x20 ///< P1.5 pin is configured for digital mode.
                                                                             
#define P1MDIN_B6__BMASK   0x40 ///< Port 1 Bit 6 Input Mode                 
#define P1MDIN_B6__SHIFT   0x06 ///< Port 1 Bit 6 Input Mode                 
#define P1MDIN_B6__ANALOG  0x00 ///< P1.6 pin is configured for analog mode. 
#define P1MDIN_B6__DIGITAL 0x40 ///< P1.6 pin is configured for digital mode.
                                                                             
#define P1MDIN_B7__BMASK   0x80 ///< Port 1 Bit 7 Input Mode                 
#define P1MDIN_B7__SHIFT   0x07 ///< Port 1 Bit 7 Input Mode                 
#define P1MDIN_B7__ANALOG  0x00 ///< P1.7 pin is configured for analog mode. 
#define P1MDIN_B7__DIGITAL 0x80 ///< P1.7 pin is configured for digital mode.
                                                                             
//------------------------------------------------------------------------------
// P1MDOUT Enums (Port 1 Output Mode @ 0xA5)
//------------------------------------------------------------------------------
#define P1MDOUT_B0__BMASK      0x01 ///< Port 1 Bit 0 Output Mode  
#define P1MDOUT_B0__SHIFT      0x00 ///< Port 1 Bit 0 Output Mode  
#define P1MDOUT_B0__OPEN_DRAIN 0x00 ///< P1.0 output is open-drain.
#define P1MDOUT_B0__PUSH_PULL  0x01 ///< P1.0 output is push-pull. 
                                                                   
#define P1MDOUT_B1__BMASK      0x02 ///< Port 1 Bit 1 Output Mode  
#define P1MDOUT_B1__SHIFT      0x01 ///< Port 1 Bit 1 Output Mode  
#define P1MDOUT_B1__OPEN_DRAIN 0x00 ///< P1.1 output is open-drain.
#define P1MDOUT_B1__PUSH_PULL  0x02 ///< P1.1 output is push-pull. 
                                                                   
#define P1MDOUT_B2__BMASK      0x04 ///< Port 1 Bit 2 Output Mode  
#define P1MDOUT_B2__SHIFT      0x02 ///< Port 1 Bit 2 Output Mode  
#define P1MDOUT_B2__OPEN_DRAIN 0x00 ///< P1.2 output is open-drain.
#define P1MDOUT_B2__PUSH_PULL  0x04 ///< P1.2 output is push-pull. 
                                                                   
#define P1MDOUT_B3__BMASK      0x08 ///< Port 1 Bit 3 Output Mode  
#define P1MDOUT_B3__SHIFT      0x03 ///< Port 1 Bit 3 Output Mode  
#define P1MDOUT_B3__OPEN_DRAIN 0x00 ///< P1.3 output is open-drain.
#define P1MDOUT_B3__PUSH_PULL  0x08 ///< P1.3 output is push-pull. 
                                                                   
#define P1MDOUT_B4__BMASK      0x10 ///< Port 1 Bit 4 Output Mode  
#define P1MDOUT_B4__SHIFT      0x04 ///< Port 1 Bit 4 Output Mode  
#define P1MDOUT_B4__OPEN_DRAIN 0x00 ///< P1.4 output is open-drain.
#define P1MDOUT_B4__PUSH_PULL  0x10 ///< P1.4 output is push-pull. 
                                                                   
#define P1MDOUT_B5__BMASK      0x20 ///< Port 1 Bit 5 Output Mode  
#define P1MDOUT_B5__SHIFT      0x05 ///< Port 1 Bit 5 Output Mode  
#define P1MDOUT_B5__OPEN_DRAIN 0x00 ///< P1.5 output is open-drain.
#define P1MDOUT_B5__PUSH_PULL  0x20 ///< P1.5 output is push-pull. 
                                                                   
#define P1MDOUT_B6__BMASK      0x40 ///< Port 1 Bit 6 Output Mode  
#define P1MDOUT_B6__SHIFT      0x06 ///< Port 1 Bit 6 Output Mode  
#define P1MDOUT_B6__OPEN_DRAIN 0x00 ///< P1.6 output is open-drain.
#define P1MDOUT_B6__PUSH_PULL  0x40 ///< P1.6 output is push-pull. 
                                                                   
#define P1MDOUT_B7__BMASK      0x80 ///< Port 1 Bit 7 Output Mode  
#define P1MDOUT_B7__SHIFT      0x07 ///< Port 1 Bit 7 Output Mode  
#define P1MDOUT_B7__OPEN_DRAIN 0x00 ///< P1.7 output is open-drain.
#define P1MDOUT_B7__PUSH_PULL  0x80 ///< P1.7 output is push-pull. 
                                                                   
//------------------------------------------------------------------------------
// P1SKIP Enums (Port 1 Skip @ 0xD5)
//------------------------------------------------------------------------------
#define P1SKIP_B0__BMASK       0x01 ///< Port 1 Bit 0 Skip                       
#define P1SKIP_B0__SHIFT       0x00 ///< Port 1 Bit 0 Skip                       
#define P1SKIP_B0__NOT_SKIPPED 0x00 ///< P1.0 pin is not skipped by the crossbar.
#define P1SKIP_B0__SKIPPED     0x01 ///< P1.0 pin is skipped by the crossbar.    
                                                                                 
#define P1SKIP_B1__BMASK       0x02 ///< Port 1 Bit 1 Skip                       
#define P1SKIP_B1__SHIFT       0x01 ///< Port 1 Bit 1 Skip                       
#define P1SKIP_B1__NOT_SKIPPED 0x00 ///< P1.1 pin is not skipped by the crossbar.
#define P1SKIP_B1__SKIPPED     0x02 ///< P1.1 pin is skipped by the crossbar.    
                                                                                 
#define P1SKIP_B2__BMASK       0x04 ///< Port 1 Bit 2 Skip                       
#define P1SKIP_B2__SHIFT       0x02 ///< Port 1 Bit 2 Skip                       
#define P1SKIP_B2__NOT_SKIPPED 0x00 ///< P1.2 pin is not skipped by the crossbar.
#define P1SKIP_B2__SKIPPED     0x04 ///< P1.2 pin is skipped by the crossbar.    
                                                                                 
#define P1SKIP_B3__BMASK       0x08 ///< Port 1 Bit 3 Skip                       
#define P1SKIP_B3__SHIFT       0x03 ///< Port 1 Bit 3 Skip                       
#define P1SKIP_B3__NOT_SKIPPED 0x00 ///< P1.3 pin is not skipped by the crossbar.
#define P1SKIP_B3__SKIPPED     0x08 ///< P1.3 pin is skipped by the crossbar.    
                                                                                 
#define P1SKIP_B4__BMASK       0x10 ///< Port 1 Bit 4 Skip                       
#define P1SKIP_B4__SHIFT       0x04 ///< Port 1 Bit 4 Skip                       
#define P1SKIP_B4__NOT_SKIPPED 0x00 ///< P1.4 pin is not skipped by the crossbar.
#define P1SKIP_B4__SKIPPED     0x10 ///< P1.4 pin is skipped by the crossbar.    
                                                                                 
#define P1SKIP_B5__BMASK       0x20 ///< Port 1 Bit 5 Skip                       
#define P1SKIP_B5__SHIFT       0x05 ///< Port 1 Bit 5 Skip                       
#define P1SKIP_B5__NOT_SKIPPED 0x00 ///< P1.5 pin is not skipped by the crossbar.
#define P1SKIP_B5__SKIPPED     0x20 ///< P1.5 pin is skipped by the crossbar.    
                                                                                 
#define P1SKIP_B6__BMASK       0x40 ///< Port 1 Bit 6 Skip                       
#define P1SKIP_B6__SHIFT       0x06 ///< Port 1 Bit 6 Skip                       
#define P1SKIP_B6__NOT_SKIPPED 0x00 ///< P1.6 pin is not skipped by the crossbar.
#define P1SKIP_B6__SKIPPED     0x40 ///< P1.6 pin is skipped by the crossbar.    
                                                                                 
#define P1SKIP_B7__BMASK       0x80 ///< Port 1 Bit 7 Skip                       
#define P1SKIP_B7__SHIFT       0x07 ///< Port 1 Bit 7 Skip                       
#define P1SKIP_B7__NOT_SKIPPED 0x00 ///< P1.7 pin is not skipped by the crossbar.
#define P1SKIP_B7__SKIPPED     0x80 ///< P1.7 pin is skipped by the crossbar.    
                                                                                 
//------------------------------------------------------------------------------
// P2 Enums (Port 2 Pin Latch @ 0xA0)
//------------------------------------------------------------------------------
#define P2_B0__BMASK 0x01 ///< Port 2 Bit 0 Latch                            
#define P2_B0__SHIFT 0x00 ///< Port 2 Bit 0 Latch                            
#define P2_B0__LOW   0x00 ///< P2.0 is low. Set P2.0 to drive low.           
#define P2_B0__HIGH  0x01 ///< P2.0 is high. Set P2.0 to drive or float high.
                                                                             
#define P2_B1__BMASK 0x02 ///< Port 2 Bit 1 Latch                            
#define P2_B1__SHIFT 0x01 ///< Port 2 Bit 1 Latch                            
#define P2_B1__LOW   0x00 ///< P2.1 is low. Set P2.1 to drive low.           
#define P2_B1__HIGH  0x02 ///< P2.1 is high. Set P2.1 to drive or float high.
                                                                             
#define P2_B2__BMASK 0x04 ///< Port 2 Bit 2 Latch                            
#define P2_B2__SHIFT 0x02 ///< Port 2 Bit 2 Latch                            
#define P2_B2__LOW   0x00 ///< P2.2 is low. Set P2.2 to drive low.           
#define P2_B2__HIGH  0x04 ///< P2.2 is high. Set P2.2 to drive or float high.
                                                                             
#define P2_B3__BMASK 0x08 ///< Port 2 Bit 3 Latch                            
#define P2_B3__SHIFT 0x03 ///< Port 2 Bit 3 Latch                            
#define P2_B3__LOW   0x00 ///< P2.3 is low. Set P2.3 to drive low.           
#define P2_B3__HIGH  0x08 ///< P2.3 is high. Set P2.3 to drive or float high.
                                                                             
#define P2_B4__BMASK 0x10 ///< Port 2 Bit 4 Latch                            
#define P2_B4__SHIFT 0x04 ///< Port 2 Bit 4 Latch                            
#define P2_B4__LOW   0x00 ///< P2.4 is low. Set P2.4 to drive low.           
#define P2_B4__HIGH  0x10 ///< P2.4 is high. Set P2.4 to drive or float high.
                                                                             
#define P2_B5__BMASK 0x20 ///< Port 2 Bit 5 Latch                            
#define P2_B5__SHIFT 0x05 ///< Port 2 Bit 5 Latch                            
#define P2_B5__LOW   0x00 ///< P2.5 is low. Set P2.5 to drive low.           
#define P2_B5__HIGH  0x20 ///< P2.5 is high. Set P2.5 to drive or float high.
                                                                             
#define P2_B6__BMASK 0x40 ///< Port 2 Bit 6 Latch                            
#define P2_B6__SHIFT 0x06 ///< Port 2 Bit 6 Latch                            
#define P2_B6__LOW   0x00 ///< P2.6 is low. Set P2.6 to drive low.           
#define P2_B6__HIGH  0x40 ///< P2.6 is high. Set P2.6 to drive or float high.
                                                                             
//------------------------------------------------------------------------------
// P2MASK Enums (Port 2 Mask @ 0xFC)
//------------------------------------------------------------------------------
#define P2MASK_B0__BMASK    0x01 ///< Port 2 Bit 0 Mask Value                           
#define P2MASK_B0__SHIFT    0x00 ///< Port 2 Bit 0 Mask Value                           
#define P2MASK_B0__IGNORED  0x00 ///< P2.0 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P2MASK_B0__COMPARED 0x01 ///< P2.0 pin logic value is compared to P2MAT.0.      
                                                                                        
#define P2MASK_B1__BMASK    0x02 ///< Port 2 Bit 1 Mask Value                           
#define P2MASK_B1__SHIFT    0x01 ///< Port 2 Bit 1 Mask Value                           
#define P2MASK_B1__IGNORED  0x00 ///< P2.1 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P2MASK_B1__COMPARED 0x02 ///< P2.1 pin logic value is compared to P2MAT.1.      
                                                                                        
#define P2MASK_B2__BMASK    0x04 ///< Port 2 Bit 2 Mask Value                           
#define P2MASK_B2__SHIFT    0x02 ///< Port 2 Bit 2 Mask Value                           
#define P2MASK_B2__IGNORED  0x00 ///< P2.2 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P2MASK_B2__COMPARED 0x04 ///< P2.2 pin logic value is compared to P2MAT.2.      
                                                                                        
#define P2MASK_B3__BMASK    0x08 ///< Port 2 Bit 3 Mask Value                           
#define P2MASK_B3__SHIFT    0x03 ///< Port 2 Bit 3 Mask Value                           
#define P2MASK_B3__IGNORED  0x00 ///< P2.3 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P2MASK_B3__COMPARED 0x08 ///< P2.3 pin logic value is compared to P2MAT.3.      
                                                                                        
#define P2MASK_B4__BMASK    0x10 ///< Port 2 Bit 4 Mask Value                           
#define P2MASK_B4__SHIFT    0x04 ///< Port 2 Bit 4 Mask Value                           
#define P2MASK_B4__IGNORED  0x00 ///< P2.4 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P2MASK_B4__COMPARED 0x10 ///< P2.4 pin logic value is compared to P2MAT.4.      
                                                                                        
#define P2MASK_B5__BMASK    0x20 ///< Port 2 Bit 5 Mask Value                           
#define P2MASK_B5__SHIFT    0x05 ///< Port 2 Bit 5 Mask Value                           
#define P2MASK_B5__IGNORED  0x00 ///< P2.5 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P2MASK_B5__COMPARED 0x20 ///< P2.5 pin logic value is compared to P2MAT.5.      
                                                                                        
#define P2MASK_B6__BMASK    0x40 ///< Port 2 Bit 6 Mask Value                           
#define P2MASK_B6__SHIFT    0x06 ///< Port 2 Bit 6 Mask Value                           
#define P2MASK_B6__IGNORED  0x00 ///< P2.6 pin logic value is ignored and will not cause
                                 ///< a port mismatch event.                            
#define P2MASK_B6__COMPARED 0x40 ///< P2.6 pin logic value is compared to P2MAT.6.      
                                                                                        
//------------------------------------------------------------------------------
// P2MAT Enums (Port 2 Match @ 0xFB)
//------------------------------------------------------------------------------
#define P2MAT_B0__BMASK 0x01 ///< Port 2 Bit 0 Match Value                         
#define P2MAT_B0__SHIFT 0x00 ///< Port 2 Bit 0 Match Value                         
#define P2MAT_B0__LOW   0x00 ///< P2.0 pin logic value is compared with logic LOW. 
#define P2MAT_B0__HIGH  0x01 ///< P2.0 pin logic value is compared with logic HIGH.
                                                                                   
#define P2MAT_B1__BMASK 0x02 ///< Port 2 Bit 1 Match Value                         
#define P2MAT_B1__SHIFT 0x01 ///< Port 2 Bit 1 Match Value                         
#define P2MAT_B1__LOW   0x00 ///< P2.1 pin logic value is compared with logic LOW. 
#define P2MAT_B1__HIGH  0x02 ///< P2.1 pin logic value is compared with logic HIGH.
                                                                                   
#define P2MAT_B2__BMASK 0x04 ///< Port 2 Bit 2 Match Value                         
#define P2MAT_B2__SHIFT 0x02 ///< Port 2 Bit 2 Match Value                         
#define P2MAT_B2__LOW   0x00 ///< P2.2 pin logic value is compared with logic LOW. 
#define P2MAT_B2__HIGH  0x04 ///< P2.2 pin logic value is compared with logic HIGH.
                                                                                   
#define P2MAT_B3__BMASK 0x08 ///< Port 2 Bit 3 Match Value                         
#define P2MAT_B3__SHIFT 0x03 ///< Port 2 Bit 3 Match Value                         
#define P2MAT_B3__LOW   0x00 ///< P2.3 pin logic value is compared with logic LOW. 
#define P2MAT_B3__HIGH  0x08 ///< P2.3 pin logic value is compared with logic HIGH.
                                                                                   
#define P2MAT_B4__BMASK 0x10 ///< Port 2 Bit 4 Match Value                         
#define P2MAT_B4__SHIFT 0x04 ///< Port 2 Bit 4 Match Value                         
#define P2MAT_B4__LOW   0x00 ///< P2.4 pin logic value is compared with logic LOW. 
#define P2MAT_B4__HIGH  0x10 ///< P2.4 pin logic value is compared with logic HIGH.
                                                                                   
#define P2MAT_B5__BMASK 0x20 ///< Port 2 Bit 5 Match Value                         
#define P2MAT_B5__SHIFT 0x05 ///< Port 2 Bit 5 Match Value                         
#define P2MAT_B5__LOW   0x00 ///< P2.5 pin logic value is compared with logic LOW. 
#define P2MAT_B5__HIGH  0x20 ///< P2.5 pin logic value is compared with logic HIGH.
                                                                                   
#define P2MAT_B6__BMASK 0x40 ///< Port 2 Bit 6 Match Value                         
#define P2MAT_B6__SHIFT 0x06 ///< Port 2 Bit 6 Match Value                         
#define P2MAT_B6__LOW   0x00 ///< P2.6 pin logic value is compared with logic LOW. 
#define P2MAT_B6__HIGH  0x40 ///< P2.6 pin logic value is compared with logic HIGH.
                                                                                   
//------------------------------------------------------------------------------
// P2MDIN Enums (Port 2 Input Mode @ 0xF3)
//------------------------------------------------------------------------------
#define P2MDIN_B0__BMASK   0x01 ///< Port 2 Bit 0 Input Mode                 
#define P2MDIN_B0__SHIFT   0x00 ///< Port 2 Bit 0 Input Mode                 
#define P2MDIN_B0__ANALOG  0x00 ///< P2.0 pin is configured for analog mode. 
#define P2MDIN_B0__DIGITAL 0x01 ///< P2.0 pin is configured for digital mode.
                                                                             
#define P2MDIN_B1__BMASK   0x02 ///< Port 2 Bit 1 Input Mode                 
#define P2MDIN_B1__SHIFT   0x01 ///< Port 2 Bit 1 Input Mode                 
#define P2MDIN_B1__ANALOG  0x00 ///< P2.1 pin is configured for analog mode. 
#define P2MDIN_B1__DIGITAL 0x02 ///< P2.1 pin is configured for digital mode.
                                                                             
#define P2MDIN_B2__BMASK   0x04 ///< Port 2 Bit 2 Input Mode                 
#define P2MDIN_B2__SHIFT   0x02 ///< Port 2 Bit 2 Input Mode                 
#define P2MDIN_B2__ANALOG  0x00 ///< P2.2 pin is configured for analog mode. 
#define P2MDIN_B2__DIGITAL 0x04 ///< P2.2 pin is configured for digital mode.
                                                                             
#define P2MDIN_B3__BMASK   0x08 ///< Port 2 Bit 3 Input Mode                 
#define P2MDIN_B3__SHIFT   0x03 ///< Port 2 Bit 3 Input Mode                 
#define P2MDIN_B3__ANALOG  0x00 ///< P2.3 pin is configured for analog mode. 
#define P2MDIN_B3__DIGITAL 0x08 ///< P2.3 pin is configured for digital mode.
                                                                             
#define P2MDIN_B4__BMASK   0x10 ///< Port 2 Bit 4 Input Mode                 
#define P2MDIN_B4__SHIFT   0x04 ///< Port 2 Bit 4 Input Mode                 
#define P2MDIN_B4__ANALOG  0x00 ///< P2.4 pin is configured for analog mode. 
#define P2MDIN_B4__DIGITAL 0x10 ///< P2.4 pin is configured for digital mode.
                                                                             
#define P2MDIN_B5__BMASK   0x20 ///< Port 2 Bit 5 Input Mode                 
#define P2MDIN_B5__SHIFT   0x05 ///< Port 2 Bit 5 Input Mode                 
#define P2MDIN_B5__ANALOG  0x00 ///< P2.5 pin is configured for analog mode. 
#define P2MDIN_B5__DIGITAL 0x20 ///< P2.5 pin is configured for digital mode.
                                                                             
#define P2MDIN_B6__BMASK   0x40 ///< Port 2 Bit 6 Input Mode                 
#define P2MDIN_B6__SHIFT   0x06 ///< Port 2 Bit 6 Input Mode                 
#define P2MDIN_B6__ANALOG  0x00 ///< P2.6 pin is configured for analog mode. 
#define P2MDIN_B6__DIGITAL 0x40 ///< P2.6 pin is configured for digital mode.
                                                                             
//------------------------------------------------------------------------------
// P2MDOUT Enums (Port 2 Output Mode @ 0xA6)
//------------------------------------------------------------------------------
#define P2MDOUT_B0__BMASK      0x01 ///< Port 2 Bit 0 Output Mode  
#define P2MDOUT_B0__SHIFT      0x00 ///< Port 2 Bit 0 Output Mode  
#define P2MDOUT_B0__OPEN_DRAIN 0x00 ///< P2.0 output is open-drain.
#define P2MDOUT_B0__PUSH_PULL  0x01 ///< P2.0 output is push-pull. 
                                                                   
#define P2MDOUT_B1__BMASK      0x02 ///< Port 2 Bit 1 Output Mode  
#define P2MDOUT_B1__SHIFT      0x01 ///< Port 2 Bit 1 Output Mode  
#define P2MDOUT_B1__OPEN_DRAIN 0x00 ///< P2.1 output is open-drain.
#define P2MDOUT_B1__PUSH_PULL  0x02 ///< P2.1 output is push-pull. 
                                                                   
#define P2MDOUT_B2__BMASK      0x04 ///< Port 2 Bit 2 Output Mode  
#define P2MDOUT_B2__SHIFT      0x02 ///< Port 2 Bit 2 Output Mode  
#define P2MDOUT_B2__OPEN_DRAIN 0x00 ///< P2.2 output is open-drain.
#define P2MDOUT_B2__PUSH_PULL  0x04 ///< P2.2 output is push-pull. 
                                                                   
#define P2MDOUT_B3__BMASK      0x08 ///< Port 2 Bit 3 Output Mode  
#define P2MDOUT_B3__SHIFT      0x03 ///< Port 2 Bit 3 Output Mode  
#define P2MDOUT_B3__OPEN_DRAIN 0x00 ///< P2.3 output is open-drain.
#define P2MDOUT_B3__PUSH_PULL  0x08 ///< P2.3 output is push-pull. 
                                                                   
#define P2MDOUT_B4__BMASK      0x10 ///< Port 2 Bit 4 Output Mode  
#define P2MDOUT_B4__SHIFT      0x04 ///< Port 2 Bit 4 Output Mode  
#define P2MDOUT_B4__OPEN_DRAIN 0x00 ///< P2.4 output is open-drain.
#define P2MDOUT_B4__PUSH_PULL  0x10 ///< P2.4 output is push-pull. 
                                                                   
#define P2MDOUT_B5__BMASK      0x20 ///< Port 2 Bit 5 Output Mode  
#define P2MDOUT_B5__SHIFT      0x05 ///< Port 2 Bit 5 Output Mode  
#define P2MDOUT_B5__OPEN_DRAIN 0x00 ///< P2.5 output is open-drain.
#define P2MDOUT_B5__PUSH_PULL  0x20 ///< P2.5 output is push-pull. 
                                                                   
#define P2MDOUT_B6__BMASK      0x40 ///< Port 2 Bit 6 Output Mode  
#define P2MDOUT_B6__SHIFT      0x06 ///< Port 2 Bit 6 Output Mode  
#define P2MDOUT_B6__OPEN_DRAIN 0x00 ///< P2.6 output is open-drain.
#define P2MDOUT_B6__PUSH_PULL  0x40 ///< P2.6 output is push-pull. 
                                                                   
//------------------------------------------------------------------------------
// P2SKIP Enums (Port 2 Skip @ 0xCC)
//------------------------------------------------------------------------------
#define P2SKIP_B0__BMASK       0x01 ///< Port 2 Bit 0 Skip                       
#define P2SKIP_B0__SHIFT       0x00 ///< Port 2 Bit 0 Skip                       
#define P2SKIP_B0__NOT_SKIPPED 0x00 ///< P2.0 pin is not skipped by the crossbar.
#define P2SKIP_B0__SKIPPED     0x01 ///< P2.0 pin is skipped by the crossbar.    
                                                                                 
#define P2SKIP_B1__BMASK       0x02 ///< Port 2 Bit 1 Skip                       
#define P2SKIP_B1__SHIFT       0x01 ///< Port 2 Bit 1 Skip                       
#define P2SKIP_B1__NOT_SKIPPED 0x00 ///< P2.1 pin is not skipped by the crossbar.
#define P2SKIP_B1__SKIPPED     0x02 ///< P2.1 pin is skipped by the crossbar.    
                                                                                 
#define P2SKIP_B2__BMASK       0x04 ///< Port 2 Bit 2 Skip                       
#define P2SKIP_B2__SHIFT       0x02 ///< Port 2 Bit 2 Skip                       
#define P2SKIP_B2__NOT_SKIPPED 0x00 ///< P2.2 pin is not skipped by the crossbar.
#define P2SKIP_B2__SKIPPED     0x04 ///< P2.2 pin is skipped by the crossbar.    
                                                                                 
#define P2SKIP_B3__BMASK       0x08 ///< Port 2 Bit 3 Skip                       
#define P2SKIP_B3__SHIFT       0x03 ///< Port 2 Bit 3 Skip                       
#define P2SKIP_B3__NOT_SKIPPED 0x00 ///< P2.3 pin is not skipped by the crossbar.
#define P2SKIP_B3__SKIPPED     0x08 ///< P2.3 pin is skipped by the crossbar.    
                                                                                 
//------------------------------------------------------------------------------
// P3 Enums (Port 3 Pin Latch @ 0xB0)
//------------------------------------------------------------------------------
#define P3_B0__BMASK 0x01 ///< Port 3 Bit 0 Latch                            
#define P3_B0__SHIFT 0x00 ///< Port 3 Bit 0 Latch                            
#define P3_B0__LOW   0x00 ///< P3.0 is low. Set P3.0 to drive low.           
#define P3_B0__HIGH  0x01 ///< P3.0 is high. Set P3.0 to drive or float high.
                                                                             
#define P3_B1__BMASK 0x02 ///< Port 3 Bit 1 Latch                            
#define P3_B1__SHIFT 0x01 ///< Port 3 Bit 1 Latch                            
#define P3_B1__LOW   0x00 ///< P3.1 is low. Set P3.1 to drive low.           
#define P3_B1__HIGH  0x02 ///< P3.1 is high. Set P3.1 to drive or float high.
                                                                             
#define P3_B2__BMASK 0x04 ///< Port 3 Bit 2 Latch                            
#define P3_B2__SHIFT 0x02 ///< Port 3 Bit 2 Latch                            
#define P3_B2__LOW   0x00 ///< P3.2 is low. Set P3.2 to drive low.           
#define P3_B2__HIGH  0x04 ///< P3.2 is high. Set P3.2 to drive or float high.
                                                                             
#define P3_B3__BMASK 0x08 ///< Port 3 Bit 3 Latch                            
#define P3_B3__SHIFT 0x03 ///< Port 3 Bit 3 Latch                            
#define P3_B3__LOW   0x00 ///< P3.3 is low. Set P3.3 to drive low.           
#define P3_B3__HIGH  0x08 ///< P3.3 is high. Set P3.3 to drive or float high.
                                                                             
#define P3_B4__BMASK 0x10 ///< Port 3 Bit 4 Latch                            
#define P3_B4__SHIFT 0x04 ///< Port 3 Bit 4 Latch                            
#define P3_B4__LOW   0x00 ///< P3.4 is low. Set P3.4 to drive low.           
#define P3_B4__HIGH  0x10 ///< P3.4 is high. Set P3.4 to drive or float high.
                                                                             
#define P3_B7__BMASK 0x80 ///< Port 3 Bit 7 Latch                            
#define P3_B7__SHIFT 0x07 ///< Port 3 Bit 7 Latch                            
#define P3_B7__LOW   0x00 ///< P3.7 is low. Set P3.7 to drive low.           
#define P3_B7__HIGH  0x80 ///< P3.7 is high. Set P3.7 to drive or float high.
                                                                             
//------------------------------------------------------------------------------
// P3MDIN Enums (Port 3 Input Mode @ 0xF4)
//------------------------------------------------------------------------------
#define P3MDIN_B0__BMASK   0x01 ///< Port 3 Bit 0 Input Mode                 
#define P3MDIN_B0__SHIFT   0x00 ///< Port 3 Bit 0 Input Mode                 
#define P3MDIN_B0__ANALOG  0x00 ///< P3.0 pin is configured for analog mode. 
#define P3MDIN_B0__DIGITAL 0x01 ///< P3.0 pin is configured for digital mode.
                                                                             
#define P3MDIN_B1__BMASK   0x02 ///< Port 3 Bit 1 Input Mode                 
#define P3MDIN_B1__SHIFT   0x01 ///< Port 3 Bit 1 Input Mode                 
#define P3MDIN_B1__ANALOG  0x00 ///< P3.1 pin is configured for analog mode. 
#define P3MDIN_B1__DIGITAL 0x02 ///< P3.1 pin is configured for digital mode.
                                                                             
#define P3MDIN_B2__BMASK   0x04 ///< Port 3 Bit 2 Input Mode                 
#define P3MDIN_B2__SHIFT   0x02 ///< Port 3 Bit 2 Input Mode                 
#define P3MDIN_B2__ANALOG  0x00 ///< P3.2 pin is configured for analog mode. 
#define P3MDIN_B2__DIGITAL 0x04 ///< P3.2 pin is configured for digital mode.
                                                                             
#define P3MDIN_B3__BMASK   0x08 ///< Port 3 Bit 3 Input Mode                 
#define P3MDIN_B3__SHIFT   0x03 ///< Port 3 Bit 3 Input Mode                 
#define P3MDIN_B3__ANALOG  0x00 ///< P3.3 pin is configured for analog mode. 
#define P3MDIN_B3__DIGITAL 0x08 ///< P3.3 pin is configured for digital mode.
                                                                             
#define P3MDIN_B4__BMASK   0x10 ///< Port 3 Bit 4 Input Mode                 
#define P3MDIN_B4__SHIFT   0x04 ///< Port 3 Bit 4 Input Mode                 
#define P3MDIN_B4__ANALOG  0x00 ///< P3.4 pin is configured for analog mode. 
#define P3MDIN_B4__DIGITAL 0x10 ///< P3.4 pin is configured for digital mode.
                                                                             
#define P3MDIN_B7__BMASK   0x80 ///< Port 3 Bit 7 Input Mode                 
#define P3MDIN_B7__SHIFT   0x07 ///< Port 3 Bit 7 Input Mode                 
#define P3MDIN_B7__ANALOG  0x00 ///< P3.7 pin is configured for analog mode. 
#define P3MDIN_B7__DIGITAL 0x80 ///< P3.7 pin is configured for digital mode.
                                                                             
//------------------------------------------------------------------------------
// P3MDOUT Enums (Port 3 Output Mode @ 0x9C)
//------------------------------------------------------------------------------
#define P3MDOUT_B0__BMASK      0x01 ///< Port 3 Bit 0 Output Mode  
#define P3MDOUT_B0__SHIFT      0x00 ///< Port 3 Bit 0 Output Mode  
#define P3MDOUT_B0__OPEN_DRAIN 0x00 ///< P3.0 output is open-drain.
#define P3MDOUT_B0__PUSH_PULL  0x01 ///< P3.0 output is push-pull. 
                                                                   
#define P3MDOUT_B1__BMASK      0x02 ///< Port 3 Bit 1 Output Mode  
#define P3MDOUT_B1__SHIFT      0x01 ///< Port 3 Bit 1 Output Mode  
#define P3MDOUT_B1__OPEN_DRAIN 0x00 ///< P3.1 output is open-drain.
#define P3MDOUT_B1__PUSH_PULL  0x02 ///< P3.1 output is push-pull. 
                                                                   
#define P3MDOUT_B2__BMASK      0x04 ///< Port 3 Bit 2 Output Mode  
#define P3MDOUT_B2__SHIFT      0x02 ///< Port 3 Bit 2 Output Mode  
#define P3MDOUT_B2__OPEN_DRAIN 0x00 ///< P3.2 output is open-drain.
#define P3MDOUT_B2__PUSH_PULL  0x04 ///< P3.2 output is push-pull. 
                                                                   
#define P3MDOUT_B3__BMASK      0x08 ///< Port 3 Bit 3 Output Mode  
#define P3MDOUT_B3__SHIFT      0x03 ///< Port 3 Bit 3 Output Mode  
#define P3MDOUT_B3__OPEN_DRAIN 0x00 ///< P3.3 output is open-drain.
#define P3MDOUT_B3__PUSH_PULL  0x08 ///< P3.3 output is push-pull. 
                                                                   
#define P3MDOUT_B4__BMASK      0x10 ///< Port 3 Bit 4 Output Mode  
#define P3MDOUT_B4__SHIFT      0x04 ///< Port 3 Bit 4 Output Mode  
#define P3MDOUT_B4__OPEN_DRAIN 0x00 ///< P3.4 output is open-drain.
#define P3MDOUT_B4__PUSH_PULL  0x10 ///< P3.4 output is push-pull. 
                                                                   
#define P3MDOUT_B7__BMASK      0x80 ///< Port 3 Bit 7 Output Mode  
#define P3MDOUT_B7__SHIFT      0x07 ///< Port 3 Bit 7 Output Mode  
#define P3MDOUT_B7__OPEN_DRAIN 0x00 ///< P3.7 output is open-drain.
#define P3MDOUT_B7__PUSH_PULL  0x80 ///< P3.7 output is push-pull. 
                                                                   
//------------------------------------------------------------------------------
// RSTSRC Enums (Reset Source @ 0xEF)
//------------------------------------------------------------------------------
#define RSTSRC_PINRSF__BMASK   0x01 ///< HW Pin Reset Flag                                                    
#define RSTSRC_PINRSF__SHIFT   0x00 ///< HW Pin Reset Flag                                                    
#define RSTSRC_PINRSF__NOT_SET 0x00 ///< The RSTb pin did not cause the last reset.                           
#define RSTSRC_PINRSF__SET     0x01 ///< The RSTb pin caused the last reset.                                  
                                                                                                              
#define RSTSRC_PORSF__BMASK    0x02 ///< Power-On / Supply Monitor Reset Flag, and Supply Monitor Reset Enable
#define RSTSRC_PORSF__SHIFT    0x01 ///< Power-On / Supply Monitor Reset Flag, and Supply Monitor Reset Enable
#define RSTSRC_PORSF__NOT_SET  0x00 ///< A power-on or supply monitor reset did not occur.                    
#define RSTSRC_PORSF__SET      0x02 ///< A power-on or supply monitor reset occurred.                         
                                                                                                              
#define RSTSRC_MCDRSF__BMASK   0x04 ///< Missing Clock Detector Enable and Flag                               
#define RSTSRC_MCDRSF__SHIFT   0x02 ///< Missing Clock Detector Enable and Flag                               
#define RSTSRC_MCDRSF__NOT_SET 0x00 ///< A missing clock detector reset did not occur.                        
#define RSTSRC_MCDRSF__SET     0x04 ///< A missing clock detector reset occurred.                             
                                                                                                              
#define RSTSRC_WDTRSF__BMASK   0x08 ///< Watchdog Timer Reset Flag                                            
#define RSTSRC_WDTRSF__SHIFT   0x03 ///< Watchdog Timer Reset Flag                                            
#define RSTSRC_WDTRSF__NOT_SET 0x00 ///< A watchdog timer overflow reset did not occur.                       
#define RSTSRC_WDTRSF__SET     0x08 ///< A watchdog timer overflow reset occurred.                            
                                                                                                              
#define RSTSRC_SWRSF__BMASK    0x10 ///< Software Reset Force and Flag                                        
#define RSTSRC_SWRSF__SHIFT    0x04 ///< Software Reset Force and Flag                                        
#define RSTSRC_SWRSF__NOT_SET  0x00 ///< A software reset did not occur.                                      
#define RSTSRC_SWRSF__SET      0x10 ///< A software reset occurred.                                           
                                                                                                              
#define RSTSRC_C0RSEF__BMASK   0x20 ///< Comparator0 Reset Enable and Flag                                    
#define RSTSRC_C0RSEF__SHIFT   0x05 ///< Comparator0 Reset Enable and Flag                                    
#define RSTSRC_C0RSEF__NOT_SET 0x00 ///< A Comparator 0 reset did not occur.                                  
#define RSTSRC_C0RSEF__SET     0x20 ///< A Comparator 0 reset occurred.                                       
                                                                                                              
#define RSTSRC_FERROR__BMASK   0x40 ///< Flash Error Reset Flag                                               
#define RSTSRC_FERROR__SHIFT   0x06 ///< Flash Error Reset Flag                                               
#define RSTSRC_FERROR__NOT_SET 0x00 ///< A flash error reset did not occur.                                   
#define RSTSRC_FERROR__SET     0x40 ///< A flash error reset occurred.                                        
                                                                                                              
//------------------------------------------------------------------------------
// SFRPAGE Enums (SFR Page @ 0xA7)
//------------------------------------------------------------------------------
#define SFRPAGE_SFRPAGE__FMASK 0xFF ///< SFR Page
#define SFRPAGE_SFRPAGE__SHIFT 0x00 ///< SFR Page
                                                 
//------------------------------------------------------------------------------
// SFRPGCN Enums (SFR Page Control @ 0xBC)
//------------------------------------------------------------------------------
#define SFRPGCN_SFRPGEN__BMASK        0x01 ///< SFR Automatic Page Control Enable                
#define SFRPGCN_SFRPGEN__SHIFT        0x00 ///< SFR Automatic Page Control Enable                
#define SFRPGCN_SFRPGEN__DISABLED     0x00 ///< Disable automatic SFR paging.                    
#define SFRPGCN_SFRPGEN__ENABLED      0x01 ///< Enable automatic SFR paging.                     
                                                                                                 
#define SFRPGCN_SFRPGIDX__FMASK       0x70 ///< SFR Page Stack Index                             
#define SFRPGCN_SFRPGIDX__SHIFT       0x04 ///< SFR Page Stack Index                             
#define SFRPGCN_SFRPGIDX__FIRST_BYTE  0x00 ///< SFRSTACK contains the value of SFRPAGE, the      
                                           ///< first/top byte of the SFR page stack.            
#define SFRPGCN_SFRPGIDX__SECOND_BYTE 0x10 ///< SFRSTACK contains the value of the second byte of
                                           ///< the SFR page stack.                              
#define SFRPGCN_SFRPGIDX__THIRD_BYTE  0x20 ///< SFRSTACK contains the value of the third byte of 
                                           ///< the SFR page stack.                              
#define SFRPGCN_SFRPGIDX__FOURTH_BYTE 0x30 ///< SFRSTACK contains the value of the fourth byte of
                                           ///< the SFR page stack.                              
#define SFRPGCN_SFRPGIDX__FIFTH_BYTE  0x40 ///< SFRSTACK contains the value of the fifth byte of 
                                           ///< the SFR page stack.                              
                                                                                                 
//------------------------------------------------------------------------------
// SFRSTACK Enums (SFR Page Stack @ 0xD7)
//------------------------------------------------------------------------------
#define SFRSTACK_SFRSTACK__FMASK 0xFF ///< SFR Page Stack
#define SFRSTACK_SFRSTACK__SHIFT 0x00 ///< SFR Page Stack
                                                         
//------------------------------------------------------------------------------
// SMB0ADM Enums (SMBus 0 Slave Address Mask @ 0xD6)
//------------------------------------------------------------------------------
#define SMB0ADM_EHACK__BMASK             0x01 ///< Hardware Acknowledge Enable                     
#define SMB0ADM_EHACK__SHIFT             0x00 ///< Hardware Acknowledge Enable                     
#define SMB0ADM_EHACK__ADR_ACK_MANUAL    0x00 ///< Firmware must manually acknowledge all incoming 
                                              ///< address and data bytes.                         
#define SMB0ADM_EHACK__ADR_ACK_AUTOMATIC 0x01 ///< Automatic slave address recognition and hardware
                                              ///< acknowledge is enabled.                         
                                                                                                   
#define SMB0ADM_SLVM__FMASK              0xFE ///< SMBus Slave Address Mask                        
#define SMB0ADM_SLVM__SHIFT              0x01 ///< SMBus Slave Address Mask                        
                                                                                                   
//------------------------------------------------------------------------------
// SMB0ADR Enums (SMBus 0 Slave Address @ 0xD7)
//------------------------------------------------------------------------------
#define SMB0ADR_GC__BMASK      0x01 ///< General Call Address Enable        
#define SMB0ADR_GC__SHIFT      0x00 ///< General Call Address Enable        
#define SMB0ADR_GC__IGNORED    0x00 ///< General Call Address is ignored.   
#define SMB0ADR_GC__RECOGNIZED 0x01 ///< General Call Address is recognized.
                                                                            
#define SMB0ADR_SLV__FMASK     0xFE ///< SMBus Hardware Slave Address       
#define SMB0ADR_SLV__SHIFT     0x01 ///< SMBus Hardware Slave Address       
                                                                            
//------------------------------------------------------------------------------
// SMB0CF Enums (SMBus 0 Configuration @ 0xC1)
//------------------------------------------------------------------------------
#define SMB0CF_SMBCS__FMASK             0x03 ///< SMBus Clock Source Selection                     
#define SMB0CF_SMBCS__SHIFT             0x00 ///< SMBus Clock Source Selection                     
#define SMB0CF_SMBCS__TIMER0            0x00 ///< Timer 0 Overflow.                                
#define SMB0CF_SMBCS__TIMER1            0x01 ///< Timer 1 Overflow.                                
#define SMB0CF_SMBCS__TIMER2_HIGH       0x02 ///< Timer 2 High Byte Overflow.                      
#define SMB0CF_SMBCS__TIMER2_LOW        0x03 ///< Timer 2 Low Byte Overflow.                       
                                                                                                   
#define SMB0CF_SMBFTE__BMASK            0x04 ///< SMBus Free Timeout Detection Enable              
#define SMB0CF_SMBFTE__SHIFT            0x02 ///< SMBus Free Timeout Detection Enable              
#define SMB0CF_SMBFTE__FREE_TO_DISABLED 0x00 ///< Disable bus free timeouts.                       
#define SMB0CF_SMBFTE__FREE_TO_ENABLED  0x04 ///< Enable bus free timeouts. The bus the bus will be
                                             ///< considered free if SCL and SDA remain high for   
                                             ///< more than 10 SMBus clock source periods.         
                                                                                                   
#define SMB0CF_SMBTOE__BMASK            0x08 ///< SMBus SCL Timeout Detection Enable               
#define SMB0CF_SMBTOE__SHIFT            0x03 ///< SMBus SCL Timeout Detection Enable               
#define SMB0CF_SMBTOE__SCL_TO_DISABLED  0x00 ///< Disable SCL low timeouts.                        
#define SMB0CF_SMBTOE__SCL_TO_ENABLED   0x08 ///< Enable SCL low timeouts if Timer 3 RLFSEL is set 
                                             ///< appropriately.                                   
                                                                                                   
#define SMB0CF_EXTHOLD__BMASK           0x10 ///< SMBus Setup and Hold Time Extension Enable       
#define SMB0CF_EXTHOLD__SHIFT           0x04 ///< SMBus Setup and Hold Time Extension Enable       
#define SMB0CF_EXTHOLD__DISABLED        0x00 ///< Disable SDA extended setup and hold times.       
#define SMB0CF_EXTHOLD__ENABLED         0x10 ///< Enable SDA extended setup and hold times.        
                                                                                                   
#define SMB0CF_BUSY__BMASK              0x20 ///< SMBus Busy Indicator                             
#define SMB0CF_BUSY__SHIFT              0x05 ///< SMBus Busy Indicator                             
#define SMB0CF_BUSY__NOT_SET            0x00 ///< The bus is not busy.                             
#define SMB0CF_BUSY__SET                0x20 ///< The bus is busy and a transfer is currently in   
                                             ///< progress.                                        
                                                                                                   
#define SMB0CF_INH__BMASK               0x40 ///< SMBus Slave Inhibit                              
#define SMB0CF_INH__SHIFT               0x06 ///< SMBus Slave Inhibit                              
#define SMB0CF_INH__SLAVE_ENABLED       0x00 ///< Slave states are enabled.                        
#define SMB0CF_INH__SLAVE_DISABLED      0x40 ///< Slave states are inhibited.                      
                                                                                                   
#define SMB0CF_ENSMB__BMASK             0x80 ///< SMBus Enable                                     
#define SMB0CF_ENSMB__SHIFT             0x07 ///< SMBus Enable                                     
#define SMB0CF_ENSMB__DISABLED          0x00 ///< Disable the SMBus module.                        
#define SMB0CF_ENSMB__ENABLED           0x80 ///< Enable the SMBus module.                         
                                                                                                   
//------------------------------------------------------------------------------
// SMB0CN0 Enums (SMBus 0 Control @ 0xC0)
//------------------------------------------------------------------------------
#define SMB0CN0_SI__BMASK           0x01 ///< SMBus Interrupt Flag                           
#define SMB0CN0_SI__SHIFT           0x00 ///< SMBus Interrupt Flag                           
#define SMB0CN0_SI__NOT_SET         0x00 ///<                                                
#define SMB0CN0_SI__SET             0x01 ///<                                                
                                                                                             
#define SMB0CN0_ACK__BMASK          0x02 ///< SMBus Acknowledge                              
#define SMB0CN0_ACK__SHIFT          0x01 ///< SMBus Acknowledge                              
#define SMB0CN0_ACK__NOT_SET        0x00 ///< Generate a NACK, or the response was a NACK.   
#define SMB0CN0_ACK__SET            0x02 ///< Generate an ACK, or the response was an ACK.   
                                                                                             
#define SMB0CN0_ARBLOST__BMASK      0x04 ///< SMBus Arbitration Lost Indicator               
#define SMB0CN0_ARBLOST__SHIFT      0x02 ///< SMBus Arbitration Lost Indicator               
#define SMB0CN0_ARBLOST__NOT_SET    0x00 ///< No arbitration error.                          
#define SMB0CN0_ARBLOST__ERROR      0x04 ///< Arbitration error occurred.                    
                                                                                             
#define SMB0CN0_ACKRQ__BMASK        0x08 ///< SMBus Acknowledge Request                      
#define SMB0CN0_ACKRQ__SHIFT        0x03 ///< SMBus Acknowledge Request                      
#define SMB0CN0_ACKRQ__NOT_SET      0x00 ///< No ACK requested.                              
#define SMB0CN0_ACKRQ__REQUESTED    0x08 ///< ACK requested.                                 
                                                                                             
#define SMB0CN0_STO__BMASK          0x10 ///< SMBus Stop Flag                                
#define SMB0CN0_STO__SHIFT          0x04 ///< SMBus Stop Flag                                
#define SMB0CN0_STO__NOT_SET        0x00 ///< A STOP is not pending.                         
#define SMB0CN0_STO__SET            0x10 ///< Generate a STOP or a STOP is currently pending.
                                                                                             
#define SMB0CN0_STA__BMASK          0x20 ///< SMBus Start Flag                               
#define SMB0CN0_STA__SHIFT          0x05 ///< SMBus Start Flag                               
#define SMB0CN0_STA__NOT_SET        0x00 ///< A START was not detected.                      
#define SMB0CN0_STA__SET            0x20 ///< Generate a START, repeated START, or a START is
                                         ///< currently pending.                             
                                                                                             
#define SMB0CN0_TXMODE__BMASK       0x40 ///< SMBus Transmit Mode Indicator                  
#define SMB0CN0_TXMODE__SHIFT       0x06 ///< SMBus Transmit Mode Indicator                  
#define SMB0CN0_TXMODE__RECEIVER    0x00 ///< SMBus in Receiver Mode.                        
#define SMB0CN0_TXMODE__TRANSMITTER 0x40 ///< SMBus in Transmitter Mode.                     
                                                                                             
#define SMB0CN0_MASTER__BMASK       0x80 ///< SMBus Master/Slave Indicator                   
#define SMB0CN0_MASTER__SHIFT       0x07 ///< SMBus Master/Slave Indicator                   
#define SMB0CN0_MASTER__SLAVE       0x00 ///< SMBus operating in slave mode.                 
#define SMB0CN0_MASTER__MASTER      0x80 ///< SMBus operating in master mode.                
                                                                                             
//------------------------------------------------------------------------------
// SMB0DAT Enums (SMBus 0 Data @ 0xC2)
//------------------------------------------------------------------------------
#define SMB0DAT_SMB0DAT__FMASK 0xFF ///< SMBus 0 Data
#define SMB0DAT_SMB0DAT__SHIFT 0x00 ///< SMBus 0 Data
                                                     
//------------------------------------------------------------------------------
// SMB0FCN0 Enums (SMBus 0 FIFO Control 0 @ 0xC3)
//------------------------------------------------------------------------------
#define SMB0FCN0_RXTH__FMASK     0x03 ///< RX FIFO Threshold                                 
#define SMB0FCN0_RXTH__SHIFT     0x00 ///< RX FIFO Threshold                                 
#define SMB0FCN0_RXTH__ZERO      0x00 ///< RFRQ will be set anytime new data arrives in the  
                                      ///< RX FIFO (when the RX FIFO is not empty).          
                                                                                             
#define SMB0FCN0_RFLSH__BMASK    0x04 ///< RX FIFO Flush                                     
#define SMB0FCN0_RFLSH__SHIFT    0x02 ///< RX FIFO Flush                                     
#define SMB0FCN0_RFLSH__FLUSH    0x04 ///< Initiate an RX FIFO flush.                        
                                                                                             
#define SMB0FCN0_RFRQE__BMASK    0x08 ///< Read Request Interrupt Enable                     
#define SMB0FCN0_RFRQE__SHIFT    0x03 ///< Read Request Interrupt Enable                     
#define SMB0FCN0_RFRQE__DISABLED 0x00 ///< SMBus 0 interrupts will not be generated when RFRQ
                                      ///< is set.                                           
#define SMB0FCN0_RFRQE__ENABLED  0x08 ///< SMBus 0 interrupts will be generated if RFRQ is   
                                      ///< set.                                              
                                                                                             
#define SMB0FCN0_TXTH__FMASK     0x30 ///< TX FIFO Threshold                                 
#define SMB0FCN0_TXTH__SHIFT     0x04 ///< TX FIFO Threshold                                 
#define SMB0FCN0_TXTH__ZERO      0x00 ///< TFRQ will be set when the TX FIFO is empty.       
                                                                                             
#define SMB0FCN0_TFLSH__BMASK    0x40 ///< TX FIFO Flush                                     
#define SMB0FCN0_TFLSH__SHIFT    0x06 ///< TX FIFO Flush                                     
#define SMB0FCN0_TFLSH__FLUSH    0x40 ///< Initiate a TX FIFO flush.                         
                                                                                             
#define SMB0FCN0_TFRQE__BMASK    0x80 ///< Write Request Interrupt Enable                    
#define SMB0FCN0_TFRQE__SHIFT    0x07 ///< Write Request Interrupt Enable                    
#define SMB0FCN0_TFRQE__DISABLED 0x00 ///< SMBus 0 interrupts will not be generated when TFRQ
                                      ///< is set.                                           
#define SMB0FCN0_TFRQE__ENABLED  0x80 ///< SMBus 0 interrupts will be generated if TFRQ is   
                                      ///< set.                                              
                                                                                             
//------------------------------------------------------------------------------
// SMB0FCN1 Enums (SMBus 0 FIFO Control 1 @ 0xC4)
//------------------------------------------------------------------------------
#define SMB0FCN1_RXE__BMASK     0x04 ///< RX FIFO Empty                                     
#define SMB0FCN1_RXE__SHIFT     0x02 ///< RX FIFO Empty                                     
#define SMB0FCN1_RXE__NOT_EMPTY 0x00 ///< The RX FIFO contains data.                        
#define SMB0FCN1_RXE__EMPTY     0x04 ///< The RX FIFO is empty.                             
                                                                                            
#define SMB0FCN1_RFRQ__BMASK    0x08 ///< Receive FIFO Request                              
#define SMB0FCN1_RFRQ__SHIFT    0x03 ///< Receive FIFO Request                              
#define SMB0FCN1_RFRQ__NOT_SET  0x00 ///< The number of bytes in the RX FIFO is less than or
                                     ///< equal to RXTH.                                    
#define SMB0FCN1_RFRQ__SET      0x08 ///< The number of bytes in the RX FIFO is greater than
                                     ///< RXTH.                                             
                                                                                            
#define SMB0FCN1_TXNF__BMASK    0x40 ///< TX FIFO Not Full                                  
#define SMB0FCN1_TXNF__SHIFT    0x06 ///< TX FIFO Not Full                                  
#define SMB0FCN1_TXNF__FULL     0x00 ///< The TX FIFO is full.                              
#define SMB0FCN1_TXNF__NOT_FULL 0x40 ///< The TX FIFO has room for more data.               
                                                                                            
#define SMB0FCN1_TFRQ__BMASK    0x80 ///< Transmit FIFO Request                             
#define SMB0FCN1_TFRQ__SHIFT    0x07 ///< Transmit FIFO Request                             
#define SMB0FCN1_TFRQ__NOT_SET  0x00 ///< The number of bytes in the TX FIFO is greater than
                                     ///< TXTH.                                             
#define SMB0FCN1_TFRQ__SET      0x80 ///< The number of bytes in the TX FIFO is less than or
                                     ///< equal to TXTH.                                    
                                                                                            
//------------------------------------------------------------------------------
// SMB0FCT Enums (SMBus 0 FIFO Count @ 0xEF)
//------------------------------------------------------------------------------
#define SMB0FCT_RXCNT__BMASK 0x01 ///< RX FIFO Count
#define SMB0FCT_RXCNT__SHIFT 0x00 ///< RX FIFO Count
                                                    
#define SMB0FCT_TXCNT__BMASK 0x10 ///< TX FIFO Count
#define SMB0FCT_TXCNT__SHIFT 0x04 ///< TX FIFO Count
                                                    
//------------------------------------------------------------------------------
// SMB0RXLN Enums (SMBus 0 Receive Length Counter @ 0xC5)
//------------------------------------------------------------------------------
#define SMB0RXLN_RXLN__FMASK 0xFF ///< SMBus Receive Length Counter
#define SMB0RXLN_RXLN__SHIFT 0x00 ///< SMBus Receive Length Counter
                                                                   
//------------------------------------------------------------------------------
// SMB0TC Enums (SMBus 0 Timing and Pin Control @ 0xAC)
//------------------------------------------------------------------------------
#define SMB0TC_SDD__FMASK         0x03 ///< SMBus Start Detection Window                     
#define SMB0TC_SDD__SHIFT         0x00 ///< SMBus Start Detection Window                     
#define SMB0TC_SDD__NONE          0x00 ///< No additional SDA falling edge recognition delay 
                                       ///< (0-1 SYSCLK).                                    
#define SMB0TC_SDD__ADD_2_SYSCLKS 0x01 ///< Increase SDA falling edge recognition time window
                                       ///< to 2-3 SYSCLKs after the SCL falling edge.       
#define SMB0TC_SDD__ADD_4_SYSCLKS 0x02 ///< Increase SDA falling edge recognition window to  
                                       ///< 4-5 SYSCLKs after the SCL falling edge.          
#define SMB0TC_SDD__ADD_8_SYSCLKS 0x03 ///< Increase SDA falling edge recognition window to  
                                       ///< 8-9 SYSCLKs after the SCL falling edge.          
                                                                                             
#define SMB0TC_DLYEXT__BMASK      0x10 ///< Setup and Hold Delay Extension                   
#define SMB0TC_DLYEXT__SHIFT      0x04 ///< Setup and Hold Delay Extension                   
#define SMB0TC_DLYEXT__STANDARD   0x00 ///< SDA setup time is 11 SYSCLKs and SDA hold time is
                                       ///< 12 SYSCLKs.                                      
#define SMB0TC_DLYEXT__EXTENDED   0x10 ///< SDA setup time is 31 SYSCLKs and SDA hold time is
                                       ///< 31 SYSCLKs.                                      
                                                                                             
#define SMB0TC_SWAP__BMASK        0x80 ///< SMBus Swap Pins                                  
#define SMB0TC_SWAP__SHIFT        0x07 ///< SMBus Swap Pins                                  
#define SMB0TC_SWAP__SDA_LOW_PIN  0x00 ///< SDA is mapped to the lower-numbered port pin, and
                                       ///< SCL is mapped to the higher-numbered port pin.   
#define SMB0TC_SWAP__SDA_HIGH_PIN 0x80 ///< SCL is mapped to the lower-numbered port pin, and
                                       ///< SDA is mapped to the higher-numbered port pin.   
                                                                                             
//------------------------------------------------------------------------------
// SPI0CFG Enums (SPI0 Configuration @ 0xA1)
//------------------------------------------------------------------------------
#define SPI0CFG_RXE__BMASK                  0x01 ///< RX FIFO Empty                              
#define SPI0CFG_RXE__SHIFT                  0x00 ///< RX FIFO Empty                              
#define SPI0CFG_RXE__NOT_EMPTY              0x00 ///< The RX FIFO contains data.                 
#define SPI0CFG_RXE__EMPTY                  0x01 ///< The RX FIFO is empty.                      
                                                                                                 
#define SPI0CFG_SRMT__BMASK                 0x02 ///< Shift Register Empty                       
#define SPI0CFG_SRMT__SHIFT                 0x01 ///< Shift Register Empty                       
#define SPI0CFG_SRMT__NOT_SET               0x00 ///< The shift register is not empty.           
#define SPI0CFG_SRMT__SET                   0x02 ///< The shift register is empty.               
                                                                                                 
#define SPI0CFG_NSSIN__BMASK                0x04 ///< NSS Instantaneous Pin Input                
#define SPI0CFG_NSSIN__SHIFT                0x02 ///< NSS Instantaneous Pin Input                
#define SPI0CFG_NSSIN__LOW                  0x00 ///< The NSS pin is low.                        
#define SPI0CFG_NSSIN__HIGH                 0x04 ///< The NSS pin is high.                       
                                                                                                 
#define SPI0CFG_SLVSEL__BMASK               0x08 ///< Slave Selected Flag                        
#define SPI0CFG_SLVSEL__SHIFT               0x03 ///< Slave Selected Flag                        
#define SPI0CFG_SLVSEL__NOT_SELECTED        0x00 ///< The Slave is not selected (NSS is high).   
#define SPI0CFG_SLVSEL__SELECTED            0x08 ///< The Slave is selected (NSS is low).        
                                                                                                 
#define SPI0CFG_CKPOL__BMASK                0x10 ///< SPI0 Clock Polarity                        
#define SPI0CFG_CKPOL__SHIFT                0x04 ///< SPI0 Clock Polarity                        
#define SPI0CFG_CKPOL__IDLE_LOW             0x00 ///< SCK line low in idle state.                
#define SPI0CFG_CKPOL__IDLE_HIGH            0x10 ///< SCK line high in idle state.               
                                                                                                 
#define SPI0CFG_CKPHA__BMASK                0x20 ///< SPI0 Clock Phase                           
#define SPI0CFG_CKPHA__SHIFT                0x05 ///< SPI0 Clock Phase                           
#define SPI0CFG_CKPHA__DATA_CENTERED_FIRST  0x00 ///< Data centered on first edge of SCK period. 
#define SPI0CFG_CKPHA__DATA_CENTERED_SECOND 0x20 ///< Data centered on second edge of SCK period.
                                                                                                 
#define SPI0CFG_MSTEN__BMASK                0x40 ///< Master Mode Enable                         
#define SPI0CFG_MSTEN__SHIFT                0x06 ///< Master Mode Enable                         
#define SPI0CFG_MSTEN__MASTER_DISABLED      0x00 ///< Disable master mode. Operate in slave mode.
#define SPI0CFG_MSTEN__MASTER_ENABLED       0x40 ///< Enable master mode. Operate as a master.   
                                                                                                 
#define SPI0CFG_SPIBSY__BMASK               0x80 ///< SPI Busy                                   
#define SPI0CFG_SPIBSY__SHIFT               0x07 ///< SPI Busy                                   
#define SPI0CFG_SPIBSY__NOT_SET             0x00 ///< A SPI transfer is not in progress.         
#define SPI0CFG_SPIBSY__SET                 0x80 ///< A SPI transfer is in progress.             
                                                                                                 
//------------------------------------------------------------------------------
// SPI0CKR Enums (SPI0 Clock Rate @ 0xA2)
//------------------------------------------------------------------------------
#define SPI0CKR_SPI0CKR__FMASK 0xFF ///< SPI0 Clock Rate
#define SPI0CKR_SPI0CKR__SHIFT 0x00 ///< SPI0 Clock Rate
                                                        
//------------------------------------------------------------------------------
// SPI0CN0 Enums (SPI0 Control @ 0xF8)
//------------------------------------------------------------------------------
#define SPI0CN0_SPIEN__BMASK                  0x01 ///< SPI0 Enable                                       
#define SPI0CN0_SPIEN__SHIFT                  0x00 ///< SPI0 Enable                                       
#define SPI0CN0_SPIEN__DISABLED               0x00 ///< Disable the SPI module.                           
#define SPI0CN0_SPIEN__ENABLED                0x01 ///< Enable the SPI module.                            
                                                                                                          
#define SPI0CN0_TXNF__BMASK                   0x02 ///< TX FIFO Not Full                                  
#define SPI0CN0_TXNF__SHIFT                   0x01 ///< TX FIFO Not Full                                  
#define SPI0CN0_TXNF__FULL                    0x00 ///< The TX FIFO is full.                              
#define SPI0CN0_TXNF__NOT_FULL                0x02 ///< The TX FIFO has room for more data.               
                                                                                                          
#define SPI0CN0_NSSMD__FMASK                  0x0C ///< Slave Select Mode                                 
#define SPI0CN0_NSSMD__SHIFT                  0x02 ///< Slave Select Mode                                 
#define SPI0CN0_NSSMD__3_WIRE                 0x00 ///< 3-Wire Slave or 3-Wire Master Mode. NSS signal is 
                                                   ///< not routed to a port pin.                         
#define SPI0CN0_NSSMD__4_WIRE_SLAVE           0x04 ///< 4-Wire Slave or Multi-Master Mode. NSS is an input
                                                   ///< to the device.                                    
#define SPI0CN0_NSSMD__4_WIRE_MASTER_NSS_LOW  0x08 ///< 4-Wire Single-Master Mode. NSS is an output and   
                                                   ///< logic low.                                        
#define SPI0CN0_NSSMD__4_WIRE_MASTER_NSS_HIGH 0x0C ///< 4-Wire Single-Master Mode. NSS is an output and   
                                                   ///< logic high.                                       
                                                                                                          
#define SPI0CN0_RXOVRN__BMASK                 0x10 ///< Receive Overrun Flag                              
#define SPI0CN0_RXOVRN__SHIFT                 0x04 ///< Receive Overrun Flag                              
#define SPI0CN0_RXOVRN__NOT_SET               0x00 ///< A receive overrun did not occur.                  
#define SPI0CN0_RXOVRN__SET                   0x10 ///< A receive overrun occurred.                       
                                                                                                          
#define SPI0CN0_MODF__BMASK                   0x20 ///< Mode Fault Flag                                   
#define SPI0CN0_MODF__SHIFT                   0x05 ///< Mode Fault Flag                                   
#define SPI0CN0_MODF__NOT_SET                 0x00 ///< A master collision did not occur.                 
#define SPI0CN0_MODF__SET                     0x20 ///< A master collision occurred.                      
                                                                                                          
#define SPI0CN0_WCOL__BMASK                   0x40 ///< Write Collision Flag                              
#define SPI0CN0_WCOL__SHIFT                   0x06 ///< Write Collision Flag                              
#define SPI0CN0_WCOL__NOT_SET                 0x00 ///< A write collision did not occur.                  
#define SPI0CN0_WCOL__SET                     0x40 ///< A write collision occurred.                       
                                                                                                          
#define SPI0CN0_SPIF__BMASK                   0x80 ///< SPI0 Interrupt Flag                               
#define SPI0CN0_SPIF__SHIFT                   0x07 ///< SPI0 Interrupt Flag                               
#define SPI0CN0_SPIF__NOT_SET                 0x00 ///< A data transfer has not completed since the last  
                                                   ///< time SPIF was cleared.                            
#define SPI0CN0_SPIF__SET                     0x80 ///< A data transfer completed.                        
                                                                                                          
//------------------------------------------------------------------------------
// SPI0DAT Enums (SPI0 Data @ 0xA3)
//------------------------------------------------------------------------------
#define SPI0DAT_SPI0DAT__FMASK 0xFF ///< SPI0 Transmit and Receive Data
#define SPI0DAT_SPI0DAT__SHIFT 0x00 ///< SPI0 Transmit and Receive Data
                                                                       
//------------------------------------------------------------------------------
// SPI0FCN0 Enums (SPI0 FIFO Control 0 @ 0x9A)
//------------------------------------------------------------------------------
#define SPI0FCN0_RXTH__FMASK     0x03 ///< RX FIFO Threshold                                 
#define SPI0FCN0_RXTH__SHIFT     0x00 ///< RX FIFO Threshold                                 
#define SPI0FCN0_RXTH__ZERO      0x00 ///< RFRQ will be set anytime new data arrives in the  
                                      ///< RX FIFO (when the RX FIFO is not empty).          
#define SPI0FCN0_RXTH__ONE       0x01 ///< RFRQ will be set if the RX FIFO contains more than
                                      ///< one byte.                                         
                                                                                             
#define SPI0FCN0_RFLSH__BMASK    0x04 ///< RX FIFO Flush                                     
#define SPI0FCN0_RFLSH__SHIFT    0x02 ///< RX FIFO Flush                                     
#define SPI0FCN0_RFLSH__FLUSH    0x04 ///< Initiate an RX FIFO flush.                        
                                                                                             
#define SPI0FCN0_RFRQE__BMASK    0x08 ///< Read Request Interrupt Enable                     
#define SPI0FCN0_RFRQE__SHIFT    0x03 ///< Read Request Interrupt Enable                     
#define SPI0FCN0_RFRQE__DISABLED 0x00 ///< SPI0 interrupts will not be generated when RFRQ is
                                      ///< set.                                              
#define SPI0FCN0_RFRQE__ENABLED  0x08 ///< SPI0 interrupts will be generated if RFRQ is set. 
                                                                                             
#define SPI0FCN0_TXTH__FMASK     0x30 ///< TX FIFO Threshold                                 
#define SPI0FCN0_TXTH__SHIFT     0x04 ///< TX FIFO Threshold                                 
#define SPI0FCN0_TXTH__ZERO      0x00 ///< TFRQ will be set when the TX FIFO is empty.       
#define SPI0FCN0_TXTH__ONE       0x10 ///< TFRQ will be set when the TX FIFO contains one or 
                                      ///< fewer bytes.                                      
                                                                                             
#define SPI0FCN0_TFLSH__BMASK    0x40 ///< TX FIFO Flush                                     
#define SPI0FCN0_TFLSH__SHIFT    0x06 ///< TX FIFO Flush                                     
#define SPI0FCN0_TFLSH__FLUSH    0x40 ///< Initiate a TX FIFO flush.                         
                                                                                             
#define SPI0FCN0_TFRQE__BMASK    0x80 ///< Write Request Interrupt Enable                    
#define SPI0FCN0_TFRQE__SHIFT    0x07 ///< Write Request Interrupt Enable                    
#define SPI0FCN0_TFRQE__DISABLED 0x00 ///< SPI0 interrupts will not be generated when TFRQ is
                                      ///< set.                                              
#define SPI0FCN0_TFRQE__ENABLED  0x80 ///< SPI0 interrupts will be generated if TFRQ is set. 
                                                                                             
//------------------------------------------------------------------------------
// SPI0FCN1 Enums (SPI0 FIFO Control 1 @ 0x9B)
//------------------------------------------------------------------------------
#define SPI0FCN1_RXFIFOE__BMASK    0x01 ///< Receive FIFO Enable                               
#define SPI0FCN1_RXFIFOE__SHIFT    0x00 ///< Receive FIFO Enable                               
#define SPI0FCN1_RXFIFOE__DISABLED 0x00 ///< Received bytes will be discarded.                 
#define SPI0FCN1_RXFIFOE__ENABLED  0x01 ///< Received bytes will be placed in the RX FIFO.     
                                                                                               
#define SPI0FCN1_RXTOE__BMASK      0x02 ///< Receive Timeout Enable                            
#define SPI0FCN1_RXTOE__SHIFT      0x01 ///< Receive Timeout Enable                            
#define SPI0FCN1_RXTOE__DISABLED   0x00 ///< Lingering bytes in the RX FIFO will not generate  
                                        ///< an interrupt.                                     
#define SPI0FCN1_RXTOE__ENABLED    0x02 ///< Lingering bytes in the RX FIFO will generate an   
                                        ///< interrupt after timeout.                          
                                                                                               
#define SPI0FCN1_RFRQ__BMASK       0x08 ///< Receive FIFO Request                              
#define SPI0FCN1_RFRQ__SHIFT       0x03 ///< Receive FIFO Request                              
#define SPI0FCN1_RFRQ__NOT_SET     0x00 ///< The number of bytes in the RX FIFO is less than or
                                        ///< equal to RXTH.                                    
#define SPI0FCN1_RFRQ__SET         0x08 ///< The number of bytes in the RX FIFO is greater than
                                        ///< RXTH.                                             
                                                                                               
#define SPI0FCN1_SPIFEN__BMASK     0x10 ///< SPIF Interrupt Enable                             
#define SPI0FCN1_SPIFEN__SHIFT     0x04 ///< SPIF Interrupt Enable                             
#define SPI0FCN1_SPIFEN__DISABLED  0x00 ///< SPI0 interrupts will not be generated when SPIF is
                                        ///< set.                                              
#define SPI0FCN1_SPIFEN__ENABLED   0x10 ///< SPI0 interrupts will be generated if SPIF is set. 
                                                                                               
#define SPI0FCN1_TXHOLD__BMASK     0x20 ///< Transmit Hold                                     
#define SPI0FCN1_TXHOLD__SHIFT     0x05 ///< Transmit Hold                                     
#define SPI0FCN1_TXHOLD__CONTINUE  0x00 ///< The UART will continue to transmit any available  
                                        ///< data in the TX FIFO.                              
#define SPI0FCN1_TXHOLD__HOLD      0x20 ///< The UART will not transmit any new data from the  
                                        ///< TX FIFO.                                          
                                                                                               
#define SPI0FCN1_THPOL__BMASK      0x40 ///< Transmit Hold Polarity                            
#define SPI0FCN1_THPOL__SHIFT      0x06 ///< Transmit Hold Polarity                            
#define SPI0FCN1_THPOL__HOLD_0     0x00 ///< Data output will be held at logic low when TXHOLD 
                                        ///< is set.                                           
#define SPI0FCN1_THPOL__HOLD_1     0x40 ///< Data output will be held at logic high when TXHOLD
                                        ///< is set.                                           
                                                                                               
#define SPI0FCN1_TFRQ__BMASK       0x80 ///< Transmit FIFO Request                             
#define SPI0FCN1_TFRQ__SHIFT       0x07 ///< Transmit FIFO Request                             
#define SPI0FCN1_TFRQ__NOT_SET     0x00 ///< The number of bytes in the TX FIFO is greater than
                                        ///< TXTH.                                             
#define SPI0FCN1_TFRQ__SET         0x80 ///< The number of bytes in the TX FIFO is less than or
                                        ///< equal to TXTH.                                    
                                                                                               
//------------------------------------------------------------------------------
// SPI0FCT Enums (SPI0 FIFO Count @ 0xF7)
//------------------------------------------------------------------------------
#define SPI0FCT_RXCNT__FMASK 0x07 ///< RX FIFO Count
#define SPI0FCT_RXCNT__SHIFT 0x00 ///< RX FIFO Count
                                                    
#define SPI0FCT_TXCNT__FMASK 0x70 ///< TX FIFO Count
#define SPI0FCT_TXCNT__SHIFT 0x04 ///< TX FIFO Count
                                                    
//------------------------------------------------------------------------------
// SPI0PCF Enums (SPI0 Pin Configuration @ 0xDF)
//------------------------------------------------------------------------------
#define SPI0PCF_SISEL__BMASK     0x01 ///< Slave Data Input Select                           
#define SPI0PCF_SISEL__SHIFT     0x00 ///< Slave Data Input Select                           
#define SPI0PCF_SISEL__CROSSBAR  0x00 ///< MOSI (slave mode data input) is connected to the  
                                      ///< pin assigned by the crossbar.                     
#define SPI0PCF_SISEL__CLU3      0x01 ///< MOSI (slave mode data input) is connected to the  
                                      ///< CLU3 output.                                      
                                                                                             
#define SPI0PCF_MISEL__BMASK     0x02 ///< Master Data Input Select                          
#define SPI0PCF_MISEL__SHIFT     0x01 ///< Master Data Input Select                          
#define SPI0PCF_MISEL__CROSSBAR  0x00 ///< MISO (master mode data input) is connected to the 
                                      ///< pin assigned by the crossbar.                     
#define SPI0PCF_MISEL__CLU2      0x02 ///< MISO (master mode data input)  is connected to the
                                      ///< CLU2 output.                                      
                                                                                             
#define SPI0PCF_SCKSEL__BMASK    0x04 ///< Slave Clock Input Select                          
#define SPI0PCF_SCKSEL__SHIFT    0x02 ///< Slave Clock Input Select                          
#define SPI0PCF_SCKSEL__CROSSBAR 0x00 ///< SCK (slave mode clock input) is connected to the  
                                      ///< pin assigned by the crossbar.                     
#define SPI0PCF_SCKSEL__CLU1     0x04 ///< SCK (slave mode clock input) is connected to the  
                                      ///< CLU1 output.                                      
                                                                                             
//------------------------------------------------------------------------------
// TH0 Enums (Timer 0 High Byte @ 0x8C)
//------------------------------------------------------------------------------
#define TH0_TH0__FMASK 0xFF ///< Timer 0 High Byte
#define TH0_TH0__SHIFT 0x00 ///< Timer 0 High Byte
                                                  
//------------------------------------------------------------------------------
// TH1 Enums (Timer 1 High Byte @ 0x8D)
//------------------------------------------------------------------------------
#define TH1_TH1__FMASK 0xFF ///< Timer 1 High Byte
#define TH1_TH1__SHIFT 0x00 ///< Timer 1 High Byte
                                                  
//------------------------------------------------------------------------------
// TL0 Enums (Timer 0 Low Byte @ 0x8A)
//------------------------------------------------------------------------------
#define TL0_TL0__FMASK 0xFF ///< Timer 0 Low Byte
#define TL0_TL0__SHIFT 0x00 ///< Timer 0 Low Byte
                                                 
//------------------------------------------------------------------------------
// TL1 Enums (Timer 1 Low Byte @ 0x8B)
//------------------------------------------------------------------------------
#define TL1_TL1__FMASK 0xFF ///< Timer 1 Low Byte
#define TL1_TL1__SHIFT 0x00 ///< Timer 1 Low Byte
                                                 
//------------------------------------------------------------------------------
// TMR2CN0 Enums (Timer 2 Control 0 @ 0xC8)
//------------------------------------------------------------------------------
#define TMR2CN0_T2XCLK__FMASK          0x03 ///< Timer 2 External Clock Select                     
#define TMR2CN0_T2XCLK__SHIFT          0x00 ///< Timer 2 External Clock Select                     
#define TMR2CN0_T2XCLK__SYSCLK_DIV_12  0x00 ///< Timer 2 clock is the system clock divided by 12.  
#define TMR2CN0_T2XCLK__EXTOSC_DIV_8   0x01 ///< Timer 2 clock is the external oscillator divided  
                                            ///< by 8 (synchronized with SYSCLK when not in suspend
                                            ///< or snooze mode).                                  
                                                                                                   
#define TMR2CN0_TR2__BMASK             0x04 ///< Timer 2 Run Control                               
#define TMR2CN0_TR2__SHIFT             0x02 ///< Timer 2 Run Control                               
#define TMR2CN0_TR2__STOP              0x00 ///< Stop Timer 2.                                     
#define TMR2CN0_TR2__RUN               0x04 ///< Start Timer 2 running.                            
                                                                                                   
#define TMR2CN0_T2SPLIT__BMASK         0x08 ///< Timer 2 Split Mode Enable                         
#define TMR2CN0_T2SPLIT__SHIFT         0x03 ///< Timer 2 Split Mode Enable                         
#define TMR2CN0_T2SPLIT__16_BIT_RELOAD 0x00 ///< Timer 2 operates in 16-bit auto-reload mode.      
#define TMR2CN0_T2SPLIT__8_BIT_RELOAD  0x08 ///< Timer 2 operates as two 8-bit auto-reload timers. 
                                                                                                   
#define TMR2CN0_TF2CEN__BMASK          0x10 ///< Timer 2 Capture Enable                            
#define TMR2CN0_TF2CEN__SHIFT          0x04 ///< Timer 2 Capture Enable                            
#define TMR2CN0_TF2CEN__DISABLED       0x00 ///< Disable capture mode.                             
#define TMR2CN0_TF2CEN__ENABLED        0x10 ///< Enable capture mode.                              
                                                                                                   
#define TMR2CN0_TF2LEN__BMASK          0x20 ///< Timer 2 Low Byte Interrupt Enable                 
#define TMR2CN0_TF2LEN__SHIFT          0x05 ///< Timer 2 Low Byte Interrupt Enable                 
#define TMR2CN0_TF2LEN__DISABLED       0x00 ///< Disable low byte interrupts.                      
#define TMR2CN0_TF2LEN__ENABLED        0x20 ///< Enable low byte interrupts.                       
                                                                                                   
#define TMR2CN0_TF2L__BMASK            0x40 ///< Timer 2 Low Byte Overflow Flag                    
#define TMR2CN0_TF2L__SHIFT            0x06 ///< Timer 2 Low Byte Overflow Flag                    
#define TMR2CN0_TF2L__NOT_SET          0x00 ///< Timer 2 low byte did not overflow.                
#define TMR2CN0_TF2L__SET              0x40 ///< Timer 2 low byte overflowed.                      
                                                                                                   
#define TMR2CN0_TF2H__BMASK            0x80 ///< Timer 2 High Byte Overflow Flag                   
#define TMR2CN0_TF2H__SHIFT            0x07 ///< Timer 2 High Byte Overflow Flag                   
#define TMR2CN0_TF2H__NOT_SET          0x00 ///< Timer 2 8-bit high byte or 16-bit value did not   
                                            ///< overflow.                                         
#define TMR2CN0_TF2H__SET              0x80 ///< Timer 2 8-bit high byte or 16-bit value           
                                            ///< overflowed.                                       
                                                                                                   
//------------------------------------------------------------------------------
// TMR2CN1 Enums (Timer 2 Control 1 @ 0xFD)
//------------------------------------------------------------------------------
#define TMR2CN1_T2CSEL__FMASK       0x07 ///< Timer 2 Capture Select                           
#define TMR2CN1_T2CSEL__SHIFT       0x00 ///< Timer 2 Capture Select                           
#define TMR2CN1_T2CSEL__PIN         0x00 ///< Capture high-to-low transitions on the T2 input  
                                         ///< pin.                                             
#define TMR2CN1_T2CSEL__LFOSC       0x01 ///< Capture high-to-low transitions of the LFO       
                                         ///< oscillator.                                      
#define TMR2CN1_T2CSEL__COMPARATOR0 0x02 ///< Capture high-to-low transitions of the Comparator
                                         ///< 0 output.                                        
#define TMR2CN1_T2CSEL__CLU0_OUT    0x04 ///< Capture high-to-low transitions on the           
                                         ///< configurable logic unit 0 synchronous output.    
#define TMR2CN1_T2CSEL__CLU1_OUT    0x05 ///< Capture high-to-low transitions on the           
                                         ///< configurable logic unit 1 synchronous output.    
#define TMR2CN1_T2CSEL__CLU2_OUT    0x06 ///< Capture high-to-low transitions on the           
                                         ///< configurable logic unit 2 synchronous output.    
#define TMR2CN1_T2CSEL__CLU3_OUT    0x07 ///< Capture high-to-low transitions on the           
                                         ///< configurable logic unit 3 synchronous output.    
                                                                                               
#define TMR2CN1_RLFSEL__FMASK       0xE0 ///< Force Reload Select                              
#define TMR2CN1_RLFSEL__SHIFT       0x05 ///< Force Reload Select                              
#define TMR2CN1_RLFSEL__NONE        0x00 ///< Timer will only reload on overflow events.       
#define TMR2CN1_RLFSEL__CLU0_OUT    0x20 ///< Timer will reload on overflow events and CLU0    
                                         ///< synchronous output high.                         
#define TMR2CN1_RLFSEL__CLU2_OUT    0x40 ///< Timer will reload on overflow events and CLU2    
                                         ///< synchronous output high.                         
                                                                                               
//------------------------------------------------------------------------------
// TMR2H Enums (Timer 2 High Byte @ 0xCF)
//------------------------------------------------------------------------------
#define TMR2H_TMR2H__FMASK 0xFF ///< Timer 2 High Byte
#define TMR2H_TMR2H__SHIFT 0x00 ///< Timer 2 High Byte
                                                      
//------------------------------------------------------------------------------
// TMR2L Enums (Timer 2 Low Byte @ 0xCE)
//------------------------------------------------------------------------------
#define TMR2L_TMR2L__FMASK 0xFF ///< Timer 2 Low Byte
#define TMR2L_TMR2L__SHIFT 0x00 ///< Timer 2 Low Byte
                                                     
//------------------------------------------------------------------------------
// TMR2RLH Enums (Timer 2 Reload High Byte @ 0xCB)
//------------------------------------------------------------------------------
#define TMR2RLH_TMR2RLH__FMASK 0xFF ///< Timer 2 Reload High Byte
#define TMR2RLH_TMR2RLH__SHIFT 0x00 ///< Timer 2 Reload High Byte
                                                                 
//------------------------------------------------------------------------------
// TMR2RLL Enums (Timer 2 Reload Low Byte @ 0xCA)
//------------------------------------------------------------------------------
#define TMR2RLL_TMR2RLL__FMASK 0xFF ///< Timer 2 Reload Low Byte
#define TMR2RLL_TMR2RLL__SHIFT 0x00 ///< Timer 2 Reload Low Byte
                                                                
//------------------------------------------------------------------------------
// TMR3CN0 Enums (Timer 3 Control 0 @ 0x91)
//------------------------------------------------------------------------------
#define TMR3CN0_T3XCLK__FMASK          0x03 ///< Timer 3 External Clock Select                     
#define TMR3CN0_T3XCLK__SHIFT          0x00 ///< Timer 3 External Clock Select                     
#define TMR3CN0_T3XCLK__SYSCLK_DIV_12  0x00 ///< Timer 3 clock is the system clock divided by 12.  
#define TMR3CN0_T3XCLK__EXTOSC_DIV_8   0x01 ///< Timer 3 clock is the external oscillator divided  
                                            ///< by 8 (synchronized with SYSCLK when not in suspend
                                            ///< or snooze mode).                                  
#define TMR3CN0_T3XCLK__LFOSC_DIV_8    0x03 ///< Timer 3 clock is the low-frequency oscillator     
                                            ///< divided by 8 (synchronized with SYSCLK when not in
                                            ///< suspend or snooze mode).                          
                                                                                                   
#define TMR3CN0_TR3__BMASK             0x04 ///< Timer 3 Run Control                               
#define TMR3CN0_TR3__SHIFT             0x02 ///< Timer 3 Run Control                               
#define TMR3CN0_TR3__STOP              0x00 ///< Stop Timer 3.                                     
#define TMR3CN0_TR3__RUN               0x04 ///< Start Timer 3 running.                            
                                                                                                   
#define TMR3CN0_T3SPLIT__BMASK         0x08 ///< Timer 3 Split Mode Enable                         
#define TMR3CN0_T3SPLIT__SHIFT         0x03 ///< Timer 3 Split Mode Enable                         
#define TMR3CN0_T3SPLIT__16_BIT_RELOAD 0x00 ///< Timer 3 operates in 16-bit auto-reload mode.      
#define TMR3CN0_T3SPLIT__8_BIT_RELOAD  0x08 ///< Timer 3 operates as two 8-bit auto-reload timers. 
                                                                                                   
#define TMR3CN0_TF3CEN__BMASK          0x10 ///< Timer 3 Capture Enable                            
#define TMR3CN0_TF3CEN__SHIFT          0x04 ///< Timer 3 Capture Enable                            
#define TMR3CN0_TF3CEN__DISABLED       0x00 ///< Disable capture mode.                             
#define TMR3CN0_TF3CEN__ENABLED        0x10 ///< Enable capture mode.                              
                                                                                                   
#define TMR3CN0_TF3LEN__BMASK          0x20 ///< Timer 3 Low Byte Interrupt Enable                 
#define TMR3CN0_TF3LEN__SHIFT          0x05 ///< Timer 3 Low Byte Interrupt Enable                 
#define TMR3CN0_TF3LEN__DISABLED       0x00 ///< Disable low byte interrupts.                      
#define TMR3CN0_TF3LEN__ENABLED        0x20 ///< Enable low byte interrupts.                       
                                                                                                   
#define TMR3CN0_TF3L__BMASK            0x40 ///< Timer 3 Low Byte Overflow Flag                    
#define TMR3CN0_TF3L__SHIFT            0x06 ///< Timer 3 Low Byte Overflow Flag                    
#define TMR3CN0_TF3L__NOT_SET          0x00 ///< Timer 3 low byte did not overflow.                
#define TMR3CN0_TF3L__SET              0x40 ///< Timer 3 low byte overflowed.                      
                                                                                                   
#define TMR3CN0_TF3H__BMASK            0x80 ///< Timer 3 High Byte Overflow Flag                   
#define TMR3CN0_TF3H__SHIFT            0x07 ///< Timer 3 High Byte Overflow Flag                   
#define TMR3CN0_TF3H__NOT_SET          0x00 ///< Timer 3 8-bit high byte or 16-bit value did not   
                                            ///< overflow.                                         
#define TMR3CN0_TF3H__SET              0x80 ///< Timer 3 8-bit high byte or 16-bit value           
                                            ///< overflowed.                                       
                                                                                                   
//------------------------------------------------------------------------------
// TMR3CN1 Enums (Timer 3 Control 1 @ 0xFE)
//------------------------------------------------------------------------------
#define TMR3CN1_T3CSEL__FMASK       0x07 ///< Timer 3 Capture Select                           
#define TMR3CN1_T3CSEL__SHIFT       0x00 ///< Timer 3 Capture Select                           
#define TMR3CN1_T3CSEL__PIN         0x00 ///< Capture high-to-low transitions on the T2 input  
                                         ///< pin.                                             
#define TMR3CN1_T3CSEL__LFOSC       0x01 ///< Capture high-to-low transitions of the LFO       
                                         ///< oscillator.                                      
#define TMR3CN1_T3CSEL__COMPARATOR0 0x02 ///< Capture high-to-low transitions of the Comparator
                                         ///< 0 output.                                        
#define TMR3CN1_T3CSEL__CLU0_OUT    0x04 ///< Capture high-to-low transitions on the           
                                         ///< configurable logic unit 0 synchronous output.    
#define TMR3CN1_T3CSEL__CLU1_OUT    0x05 ///< Capture high-to-low transitions on the           
                                         ///< configurable logic unit 1 synchronous output.    
#define TMR3CN1_T3CSEL__CLU2_OUT    0x06 ///< Capture high-to-low transitions on the           
                                         ///< configurable logic unit 2 synchronous output.    
#define TMR3CN1_T3CSEL__CLU3_OUT    0x07 ///< Capture high-to-low transitions on the           
                                         ///< configurable logic unit 3 synchronous output.    
                                                                                               
#define TMR3CN1_STSYNC__BMASK       0x10 ///< Suspend Timer Synchronization Status             
#define TMR3CN1_STSYNC__SHIFT       0x04 ///< Suspend Timer Synchronization Status             
                                                                                               
#define TMR3CN1_RLFSEL__FMASK       0xE0 ///< Force Reload Select                              
#define TMR3CN1_RLFSEL__SHIFT       0x05 ///< Force Reload Select                              
#define TMR3CN1_RLFSEL__SMB0_SCL    0x00 ///< If the SMBTOE bit in the SMB0CF register is 0,   
                                         ///< then the timer will only reload on overflow      
                                         ///< events. If SMBTOE is 1, the timer will reload on 
                                         ///< overflow events and when the SMB0 SCL signal is  
                                         ///< high.                                            
#define TMR3CN1_RLFSEL__CLU1_OUT    0x20 ///< Timer will reload on overflow events and CLU1    
                                         ///< synchronous output high.                         
#define TMR3CN1_RLFSEL__CLU3_OUT    0x40 ///< Timer will reload on overflow events and CLU3    
                                         ///< synchronous output high.                         
#define TMR3CN1_RLFSEL__NONE        0x60 ///< Timer will only reload on overflow events.       
                                                                                               
//------------------------------------------------------------------------------
// TMR3H Enums (Timer 3 High Byte @ 0x95)
//------------------------------------------------------------------------------
#define TMR3H_TMR3H__FMASK 0xFF ///< Timer 3 High Byte
#define TMR3H_TMR3H__SHIFT 0x00 ///< Timer 3 High Byte
                                                      
//------------------------------------------------------------------------------
// TMR3L Enums (Timer 3 Low Byte @ 0x94)
//------------------------------------------------------------------------------
#define TMR3L_TMR3L__FMASK 0xFF ///< Timer 3 Low Byte
#define TMR3L_TMR3L__SHIFT 0x00 ///< Timer 3 Low Byte
                                                     
//------------------------------------------------------------------------------
// TMR3RLH Enums (Timer 3 Reload High Byte @ 0x93)
//------------------------------------------------------------------------------
#define TMR3RLH_TMR3RLH__FMASK 0xFF ///< Timer 3 Reload High Byte
#define TMR3RLH_TMR3RLH__SHIFT 0x00 ///< Timer 3 Reload High Byte
                                                                 
//------------------------------------------------------------------------------
// TMR3RLL Enums (Timer 3 Reload Low Byte @ 0x92)
//------------------------------------------------------------------------------
#define TMR3RLL_TMR3RLL__FMASK 0xFF ///< Timer 3 Reload Low Byte
#define TMR3RLL_TMR3RLL__SHIFT 0x00 ///< Timer 3 Reload Low Byte
                                                                
//------------------------------------------------------------------------------
// TMR4CN0 Enums (Timer 4 Control 0 @ 0x98)
//------------------------------------------------------------------------------
#define TMR4CN0_T4XCLK__FMASK          0x03 ///< Timer 4 External Clock Select                     
#define TMR4CN0_T4XCLK__SHIFT          0x00 ///< Timer 4 External Clock Select                     
#define TMR4CN0_T4XCLK__SYSCLK_DIV_12  0x00 ///< Timer 4 clock is the system clock divided by 12.  
#define TMR4CN0_T4XCLK__EXTOSC_DIV_8   0x01 ///< Timer 4 clock is the external oscillator divided  
                                            ///< by 8 (synchronized with SYSCLK when not in suspend
                                            ///< or snooze mode).                                  
#define TMR4CN0_T4XCLK__TIMER3         0x02 ///< Timer 4 is clocked by Timer 3 overflows.          
#define TMR4CN0_T4XCLK__LFOSC_DIV_8    0x03 ///< Timer 4 clock is the low-frequency oscillator     
                                            ///< divided by 8 (synchronized with SYSCLK when not in
                                            ///< suspend or snooze mode).                          
                                                                                                   
#define TMR4CN0_TR4__BMASK             0x04 ///< Timer 4 Run Control                               
#define TMR4CN0_TR4__SHIFT             0x02 ///< Timer 4 Run Control                               
#define TMR4CN0_TR4__STOP              0x00 ///< Stop Timer 4.                                     
#define TMR4CN0_TR4__RUN               0x04 ///< Start Timer 4 running.                            
                                                                                                   
#define TMR4CN0_T4SPLIT__BMASK         0x08 ///< Timer 4 Split Mode Enable                         
#define TMR4CN0_T4SPLIT__SHIFT         0x03 ///< Timer 4 Split Mode Enable                         
#define TMR4CN0_T4SPLIT__16_BIT_RELOAD 0x00 ///< Timer 4 operates in 16-bit auto-reload mode.      
#define TMR4CN0_T4SPLIT__8_BIT_RELOAD  0x08 ///< Timer 4 operates as two 8-bit auto-reload timers. 
                                                                                                   
#define TMR4CN0_TF4CEN__BMASK          0x10 ///< Timer 4 Capture Enable                            
#define TMR4CN0_TF4CEN__SHIFT          0x04 ///< Timer 4 Capture Enable                            
#define TMR4CN0_TF4CEN__DISABLED       0x00 ///< Disable capture mode.                             
#define TMR4CN0_TF4CEN__ENABLED        0x10 ///< Enable capture mode.                              
                                                                                                   
#define TMR4CN0_TF4LEN__BMASK          0x20 ///< Timer 4 Low Byte Interrupt Enable                 
#define TMR4CN0_TF4LEN__SHIFT          0x05 ///< Timer 4 Low Byte Interrupt Enable                 
#define TMR4CN0_TF4LEN__DISABLED       0x00 ///< Disable low byte interrupts.                      
#define TMR4CN0_TF4LEN__ENABLED        0x20 ///< Enable low byte interrupts.                       
                                                                                                   
#define TMR4CN0_TF4L__BMASK            0x40 ///< Timer 4 Low Byte Overflow Flag                    
#define TMR4CN0_TF4L__SHIFT            0x06 ///< Timer 4 Low Byte Overflow Flag                    
#define TMR4CN0_TF4L__NOT_SET          0x00 ///< Timer 4 low byte did not overflow.                
#define TMR4CN0_TF4L__SET              0x40 ///< Timer 4 low byte overflowed.                      
                                                                                                   
#define TMR4CN0_TF4H__BMASK            0x80 ///< Timer 4 High Byte Overflow Flag                   
#define TMR4CN0_TF4H__SHIFT            0x07 ///< Timer 4 High Byte Overflow Flag                   
#define TMR4CN0_TF4H__NOT_SET          0x00 ///< Timer 4 8-bit high byte or 16-bit value did not   
                                            ///< overflow.                                         
#define TMR4CN0_TF4H__SET              0x80 ///< Timer 4 8-bit high byte or 16-bit value           
                                            ///< overflowed.                                       
                                                                                                   
//------------------------------------------------------------------------------
// TMR4CN1 Enums (Timer 4 Control 1 @ 0xFF)
//------------------------------------------------------------------------------
#define TMR4CN1_T4CSEL__FMASK         0x07 ///< Timer 4 Capture Select                            
#define TMR4CN1_T4CSEL__SHIFT         0x00 ///< Timer 4 Capture Select                            
#define TMR4CN1_T4CSEL__PIN           0x00 ///< Capture high-to-low transitions on the T2 input   
                                           ///< pin.                                              
#define TMR4CN1_T4CSEL__LFOSC         0x01 ///< Capture high-to-low transitions of the LFO        
                                           ///< oscillator.                                       
#define TMR4CN1_T4CSEL__COMPARATOR0   0x02 ///< Capture high-to-low transitions of the Comparator 
                                           ///< 0 output.                                         
#define TMR4CN1_T4CSEL__CLU0_OUT      0x04 ///< Capture high-to-low transitions on the            
                                           ///< configurable logic unit 0 synchronous output.     
#define TMR4CN1_T4CSEL__CLU1_OUT      0x05 ///< Capture high-to-low transitions on the            
                                           ///< configurable logic unit 1 synchronous output.     
#define TMR4CN1_T4CSEL__CLU2_OUT      0x06 ///< Capture high-to-low transitions on the            
                                           ///< configurable logic unit 2 synchronous output.     
#define TMR4CN1_T4CSEL__CLU3_OUT      0x07 ///< Capture high-to-low transitions on the            
                                           ///< configurable logic unit 3 synchronous output.     
                                                                                                  
#define TMR4CN1_STSYNC__BMASK         0x10 ///< Suspend Timer Synchronization Status              
#define TMR4CN1_STSYNC__SHIFT         0x04 ///< Suspend Timer Synchronization Status              
                                                                                                  
#define TMR4CN1_RLFSEL__FMASK         0xE0 ///< Force Reload Select                               
#define TMR4CN1_RLFSEL__SHIFT         0x05 ///< Force Reload Select                               
#define TMR4CN1_RLFSEL__I2CSLAVE0_SCL 0x00 ///< If the TIMEOUT bit in I2C0CN0 is 0, then the timer
                                           ///< will only reload on overflow events. If TIMEOUT is
                                           ///< 1, the timer will reload on overflow events and   
                                           ///< when the I2C0 SCL signal is high.                 
#define TMR4CN1_RLFSEL__CLU0_OUT      0x20 ///< Timer will reload on overflow events and CLU0     
                                           ///< synchronous output high.                          
#define TMR4CN1_RLFSEL__CLU2_OUT      0x40 ///< Timer will reload on overflow events and CLU2     
                                           ///< synchronous output high.                          
#define TMR4CN1_RLFSEL__NONE          0x60 ///< Timer will only reload on overflow events.        
                                                                                                  
//------------------------------------------------------------------------------
// TMR4H Enums (Timer 4 High Byte @ 0xA5)
//------------------------------------------------------------------------------
#define TMR4H_TMR4H__FMASK 0xFF ///< Timer 4 High Byte
#define TMR4H_TMR4H__SHIFT 0x00 ///< Timer 4 High Byte
                                                      
//------------------------------------------------------------------------------
// TMR4L Enums (Timer 4 Low Byte @ 0xA4)
//------------------------------------------------------------------------------
#define TMR4L_TMR4L__FMASK 0xFF ///< Timer 4 Low Byte
#define TMR4L_TMR4L__SHIFT 0x00 ///< Timer 4 Low Byte
                                                     
//------------------------------------------------------------------------------
// TMR4RLH Enums (Timer 4 Reload High Byte @ 0xA3)
//------------------------------------------------------------------------------
#define TMR4RLH_TMR4RLH__FMASK 0xFF ///< Timer 4 Reload High Byte
#define TMR4RLH_TMR4RLH__SHIFT 0x00 ///< Timer 4 Reload High Byte
                                                                 
//------------------------------------------------------------------------------
// TMR4RLL Enums (Timer 4 Reload Low Byte @ 0xA2)
//------------------------------------------------------------------------------
#define TMR4RLL_TMR4RLL__FMASK 0xFF ///< Timer 4 Reload Low Byte
#define TMR4RLL_TMR4RLL__SHIFT 0x00 ///< Timer 4 Reload Low Byte
                                                                
//------------------------------------------------------------------------------
// TMR5CN0 Enums (Timer 5 Control 0 @ 0xC0)
//------------------------------------------------------------------------------
#define TMR5CN0_T5XCLK__FMASK          0x03 ///< Timer 5 External Clock Select                     
#define TMR5CN0_T5XCLK__SHIFT          0x00 ///< Timer 5 External Clock Select                     
#define TMR5CN0_T5XCLK__SYSCLK_DIV_12  0x00 ///< Timer 5 clock is the system clock divided by 12.  
#define TMR5CN0_T5XCLK__EXTOSC_DIV_8   0x01 ///< Timer 5 clock is the external oscillator divided  
                                            ///< by 8 (synchronized with SYSCLK when not in suspend
                                            ///< or snooze mode).                                  
                                                                                                   
#define TMR5CN0_TR5__BMASK             0x04 ///< Timer 5 Run Control                               
#define TMR5CN0_TR5__SHIFT             0x02 ///< Timer 5 Run Control                               
#define TMR5CN0_TR5__STOP              0x00 ///< Stop Timer 5.                                     
#define TMR5CN0_TR5__RUN               0x04 ///< Start Timer 5 running.                            
                                                                                                   
#define TMR5CN0_T5SPLIT__BMASK         0x08 ///< Timer 5 Split Mode Enable                         
#define TMR5CN0_T5SPLIT__SHIFT         0x03 ///< Timer 5 Split Mode Enable                         
#define TMR5CN0_T5SPLIT__16_BIT_RELOAD 0x00 ///< Timer 5 operates in 16-bit auto-reload mode.      
#define TMR5CN0_T5SPLIT__8_BIT_RELOAD  0x08 ///< Timer 5 operates as two 8-bit auto-reload timers. 
                                                                                                   
#define TMR5CN0_TF5CEN__BMASK          0x10 ///< Timer 5 Capture Enable                            
#define TMR5CN0_TF5CEN__SHIFT          0x04 ///< Timer 5 Capture Enable                            
#define TMR5CN0_TF5CEN__DISABLED       0x00 ///< Disable capture mode.                             
#define TMR5CN0_TF5CEN__ENABLED        0x10 ///< Enable capture mode.                              
                                                                                                   
#define TMR5CN0_TF5LEN__BMASK          0x20 ///< Timer 5 Low Byte Interrupt Enable                 
#define TMR5CN0_TF5LEN__SHIFT          0x05 ///< Timer 5 Low Byte Interrupt Enable                 
#define TMR5CN0_TF5LEN__DISABLED       0x00 ///< Disable low byte interrupts.                      
#define TMR5CN0_TF5LEN__ENABLED        0x20 ///< Enable low byte interrupts.                       
                                                                                                   
#define TMR5CN0_TF5L__BMASK            0x40 ///< Timer 5 Low Byte Overflow Flag                    
#define TMR5CN0_TF5L__SHIFT            0x06 ///< Timer 5 Low Byte Overflow Flag                    
#define TMR5CN0_TF5L__NOT_SET          0x00 ///< Timer 5 low byte did not overflow.                
#define TMR5CN0_TF5L__SET              0x40 ///< Timer 5 low byte overflowed.                      
                                                                                                   
#define TMR5CN0_TF5H__BMASK            0x80 ///< Timer 5 High Byte Overflow Flag                   
#define TMR5CN0_TF5H__SHIFT            0x07 ///< Timer 5 High Byte Overflow Flag                   
#define TMR5CN0_TF5H__NOT_SET          0x00 ///< Timer 5 8-bit high byte or 16-bit value did not   
                                            ///< overflow.                                         
#define TMR5CN0_TF5H__SET              0x80 ///< Timer 5 8-bit high byte or 16-bit value           
                                            ///< overflowed.                                       
                                                                                                   
//------------------------------------------------------------------------------
// TMR5CN1 Enums (Timer 5 Control 1 @ 0xF1)
//------------------------------------------------------------------------------
#define TMR5CN1_T5CSEL__FMASK       0x07 ///< Timer 5 Capture Select                           
#define TMR5CN1_T5CSEL__SHIFT       0x00 ///< Timer 5 Capture Select                           
#define TMR5CN1_T5CSEL__PIN         0x00 ///< Capture high-to-low transitions on the T2 input  
                                         ///< pin.                                             
#define TMR5CN1_T5CSEL__LFOSC       0x01 ///< Capture high-to-low transitions of the LFO       
                                         ///< oscillator.                                      
#define TMR5CN1_T5CSEL__COMPARATOR0 0x02 ///< Capture high-to-low transitions of the Comparator
                                         ///< 0 output.                                        
#define TMR5CN1_T5CSEL__CLU0_OUT    0x04 ///< Capture high-to-low transitions on the           
                                         ///< configurable logic unit 0 synchronous output.    
#define TMR5CN1_T5CSEL__CLU1_OUT    0x05 ///< Capture high-to-low transitions on the           
                                         ///< configurable logic unit 1 synchronous output.    
#define TMR5CN1_T5CSEL__CLU2_OUT    0x06 ///< Capture high-to-low transitions on the           
                                         ///< configurable logic unit 2 synchronous output.    
#define TMR5CN1_T5CSEL__CLU3_OUT    0x07 ///< Capture high-to-low transitions on the           
                                         ///< configurable logic unit 3 synchronous output.    
                                                                                               
#define TMR5CN1_RLFSEL__FMASK       0xE0 ///< Force Reload Select                              
#define TMR5CN1_RLFSEL__SHIFT       0x05 ///< Force Reload Select                              
#define TMR5CN1_RLFSEL__NONE        0x00 ///< Timer will only reload on overflow events.       
#define TMR5CN1_RLFSEL__CLU1_OUT    0x20 ///< Timer will reload on overflow events and CLU1    
                                         ///< synchronous output high.                         
#define TMR5CN1_RLFSEL__CLU3_OUT    0x40 ///< Timer will reload on overflow events and CLU3    
                                         ///< synchronous output high.                         
                                                                                               
//------------------------------------------------------------------------------
// TMR5H Enums (Timer 5 High Byte @ 0xD5)
//------------------------------------------------------------------------------
#define TMR5H_TMR5H__FMASK 0xFF ///< Timer 5 High Byte
#define TMR5H_TMR5H__SHIFT 0x00 ///< Timer 5 High Byte
                                                      
//------------------------------------------------------------------------------
// TMR5L Enums (Timer 5 Low Byte @ 0xD4)
//------------------------------------------------------------------------------
#define TMR5L_TMR5L__FMASK 0xFF ///< Timer 5 Low Byte
#define TMR5L_TMR5L__SHIFT 0x00 ///< Timer 5 Low Byte
                                                     
//------------------------------------------------------------------------------
// TMR5RLH Enums (Timer 5 Reload High Byte @ 0xD3)
//------------------------------------------------------------------------------
#define TMR5RLH_TMR5RLH__FMASK 0xFF ///< Timer 5 Reload High Byte
#define TMR5RLH_TMR5RLH__SHIFT 0x00 ///< Timer 5 Reload High Byte
                                                                 
//------------------------------------------------------------------------------
// TMR5RLL Enums (Timer 5 Reload Low Byte @ 0xD2)
//------------------------------------------------------------------------------
#define TMR5RLL_TMR5RLL__FMASK 0xFF ///< Timer 5 Reload Low Byte
#define TMR5RLL_TMR5RLL__SHIFT 0x00 ///< Timer 5 Reload Low Byte
                                                                
//------------------------------------------------------------------------------
// CKCON0 Enums (Clock Control 0 @ 0x8E)
//------------------------------------------------------------------------------
#define CKCON0_SCA__FMASK           0x03 ///< Timer 0/1 Prescale                                
#define CKCON0_SCA__SHIFT           0x00 ///< Timer 0/1 Prescale                                
#define CKCON0_SCA__SYSCLK_DIV_12   0x00 ///< System clock divided by 12.                       
#define CKCON0_SCA__SYSCLK_DIV_4    0x01 ///< System clock divided by 4.                        
#define CKCON0_SCA__SYSCLK_DIV_48   0x02 ///< System clock divided by 48.                       
#define CKCON0_SCA__EXTOSC_DIV_8    0x03 ///< External oscillator divided by 8 (synchronized    
                                         ///< with the system clock).                           
                                                                                                
#define CKCON0_T0M__BMASK           0x04 ///< Timer 0 Clock Select                              
#define CKCON0_T0M__SHIFT           0x02 ///< Timer 0 Clock Select                              
#define CKCON0_T0M__PRESCALE        0x00 ///< Counter/Timer 0 uses the clock defined by the     
                                         ///< prescale field, SCA.                              
#define CKCON0_T0M__SYSCLK          0x04 ///< Counter/Timer 0 uses the system clock.            
                                                                                                
#define CKCON0_T1M__BMASK           0x08 ///< Timer 1 Clock Select                              
#define CKCON0_T1M__SHIFT           0x03 ///< Timer 1 Clock Select                              
#define CKCON0_T1M__PRESCALE        0x00 ///< Timer 1 uses the clock defined by the prescale    
                                         ///< field, SCA.                                       
#define CKCON0_T1M__SYSCLK          0x08 ///< Timer 1 uses the system clock.                    
                                                                                                
#define CKCON0_T2ML__BMASK          0x10 ///< Timer 2 Low Byte Clock Select                     
#define CKCON0_T2ML__SHIFT          0x04 ///< Timer 2 Low Byte Clock Select                     
#define CKCON0_T2ML__EXTERNAL_CLOCK 0x00 ///< Timer 2 low byte uses the clock defined by T2XCLK 
                                         ///< in TMR2CN0.                                       
#define CKCON0_T2ML__SYSCLK         0x10 ///< Timer 2 low byte uses the system clock.           
                                                                                                
#define CKCON0_T2MH__BMASK          0x20 ///< Timer 2 High Byte Clock Select                    
#define CKCON0_T2MH__SHIFT          0x05 ///< Timer 2 High Byte Clock Select                    
#define CKCON0_T2MH__EXTERNAL_CLOCK 0x00 ///< Timer 2 high byte uses the clock defined by T2XCLK
                                         ///< in TMR2CN0.                                       
#define CKCON0_T2MH__SYSCLK         0x20 ///< Timer 2 high byte uses the system clock.          
                                                                                                
#define CKCON0_T3ML__BMASK          0x40 ///< Timer 3 Low Byte Clock Select                     
#define CKCON0_T3ML__SHIFT          0x06 ///< Timer 3 Low Byte Clock Select                     
#define CKCON0_T3ML__EXTERNAL_CLOCK 0x00 ///< Timer 3 low byte uses the clock defined by T3XCLK 
                                         ///< in TMR3CN0.                                       
#define CKCON0_T3ML__SYSCLK         0x40 ///< Timer 3 low byte uses the system clock.           
                                                                                                
#define CKCON0_T3MH__BMASK          0x80 ///< Timer 3 High Byte Clock Select                    
#define CKCON0_T3MH__SHIFT          0x07 ///< Timer 3 High Byte Clock Select                    
#define CKCON0_T3MH__EXTERNAL_CLOCK 0x00 ///< Timer 3 high byte uses the clock defined by T3XCLK
                                         ///< in TMR3CN0.                                       
#define CKCON0_T3MH__SYSCLK         0x80 ///< Timer 3 high byte uses the system clock.          
                                                                                                
//------------------------------------------------------------------------------
// CKCON1 Enums (Clock Control 1 @ 0xA6)
//------------------------------------------------------------------------------
#define CKCON1_T4ML__BMASK          0x01 ///< Timer 4 Low Byte Clock Select                     
#define CKCON1_T4ML__SHIFT          0x00 ///< Timer 4 Low Byte Clock Select                     
#define CKCON1_T4ML__EXTERNAL_CLOCK 0x00 ///< Timer 4 low byte uses the clock defined by T4XCLK 
                                         ///< in TMR4CN0.                                       
#define CKCON1_T4ML__SYSCLK         0x01 ///< Timer 4 low byte uses the system clock.           
                                                                                                
#define CKCON1_T4MH__BMASK          0x02 ///< Timer 4 High Byte Clock Select                    
#define CKCON1_T4MH__SHIFT          0x01 ///< Timer 4 High Byte Clock Select                    
#define CKCON1_T4MH__EXTERNAL_CLOCK 0x00 ///< Timer 4 high byte uses the clock defined by T4XCLK
                                         ///< in TMR4CN0.                                       
#define CKCON1_T4MH__SYSCLK         0x02 ///< Timer 4 high byte uses the system clock.          
                                                                                                
#define CKCON1_T5ML__BMASK          0x04 ///< Timer 5 Low Byte Clock Select                     
#define CKCON1_T5ML__SHIFT          0x02 ///< Timer 5 Low Byte Clock Select                     
#define CKCON1_T5ML__EXTERNAL_CLOCK 0x00 ///< Timer 5 low byte uses the clock defined by T5XCLK 
                                         ///< in TMR5CN0.                                       
#define CKCON1_T5ML__SYSCLK         0x04 ///< Timer 5 low byte uses the system clock.           
                                                                                                
#define CKCON1_T5MH__BMASK          0x08 ///< Timer 5 High Byte Clock Select                    
#define CKCON1_T5MH__SHIFT          0x03 ///< Timer 5 High Byte Clock Select                    
#define CKCON1_T5MH__EXTERNAL_CLOCK 0x00 ///< Timer 5 high byte uses the clock defined by T5XCLK
                                         ///< in TMR5CN0.                                       
#define CKCON1_T5MH__SYSCLK         0x08 ///< Timer 5 high byte uses the system clock.          
                                                                                                
//------------------------------------------------------------------------------
// TCON Enums (Timer 0/1 Control @ 0x88)
//------------------------------------------------------------------------------
#define TCON_IT0__BMASK   0x01 ///< Interrupt 0 Type Select  
#define TCON_IT0__SHIFT   0x00 ///< Interrupt 0 Type Select  
#define TCON_IT0__LEVEL   0x00 ///< INT0 is level triggered. 
#define TCON_IT0__EDGE    0x01 ///< INT0 is edge triggered.  
                                                             
#define TCON_IE0__BMASK   0x02 ///< External Interrupt 0     
#define TCON_IE0__SHIFT   0x01 ///< External Interrupt 0     
#define TCON_IE0__NOT_SET 0x00 ///< Edge/level not detected. 
#define TCON_IE0__SET     0x02 ///< Edge/level detected      
                                                             
#define TCON_IT1__BMASK   0x04 ///< Interrupt 1 Type Select  
#define TCON_IT1__SHIFT   0x02 ///< Interrupt 1 Type Select  
#define TCON_IT1__LEVEL   0x00 ///< INT1 is level triggered. 
#define TCON_IT1__EDGE    0x04 ///< INT1 is edge triggered.  
                                                             
#define TCON_IE1__BMASK   0x08 ///< External Interrupt 1     
#define TCON_IE1__SHIFT   0x03 ///< External Interrupt 1     
#define TCON_IE1__NOT_SET 0x00 ///< Edge/level not detected. 
#define TCON_IE1__SET     0x08 ///< Edge/level detected      
                                                             
#define TCON_TR0__BMASK   0x10 ///< Timer 0 Run Control      
#define TCON_TR0__SHIFT   0x04 ///< Timer 0 Run Control      
#define TCON_TR0__STOP    0x00 ///< Stop Timer 0.            
#define TCON_TR0__RUN     0x10 ///< Start Timer 0 running.   
                                                             
#define TCON_TF0__BMASK   0x20 ///< Timer 0 Overflow Flag    
#define TCON_TF0__SHIFT   0x05 ///< Timer 0 Overflow Flag    
#define TCON_TF0__NOT_SET 0x00 ///< Timer 0 did not overflow.
#define TCON_TF0__SET     0x20 ///< Timer 0 overflowed.      
                                                             
#define TCON_TR1__BMASK   0x40 ///< Timer 1 Run Control      
#define TCON_TR1__SHIFT   0x06 ///< Timer 1 Run Control      
#define TCON_TR1__STOP    0x00 ///< Stop Timer 1.            
#define TCON_TR1__RUN     0x40 ///< Start Timer 1 running.   
                                                             
#define TCON_TF1__BMASK   0x80 ///< Timer 1 Overflow Flag    
#define TCON_TF1__SHIFT   0x07 ///< Timer 1 Overflow Flag    
#define TCON_TF1__NOT_SET 0x00 ///< Timer 1 did not overflow.
#define TCON_TF1__SET     0x80 ///< Timer 1 overflowed.      
                                                             
//------------------------------------------------------------------------------
// TMOD Enums (Timer 0/1 Mode @ 0x89)
//------------------------------------------------------------------------------
#define TMOD_T0M__FMASK      0x03 ///< Timer 0 Mode Select                               
#define TMOD_T0M__SHIFT      0x00 ///< Timer 0 Mode Select                               
#define TMOD_T0M__MODE0      0x00 ///< Mode 0, 13-bit Counter/Timer                      
#define TMOD_T0M__MODE1      0x01 ///< Mode 1, 16-bit Counter/Timer                      
#define TMOD_T0M__MODE2      0x02 ///< Mode 2, 8-bit Counter/Timer with Auto-Reload      
#define TMOD_T0M__MODE3      0x03 ///< Mode 3, Two 8-bit Counter/Timers                  
                                                                                         
#define TMOD_CT0__BMASK      0x04 ///< Counter/Timer 0 Select                            
#define TMOD_CT0__SHIFT      0x02 ///< Counter/Timer 0 Select                            
#define TMOD_CT0__TIMER      0x00 ///< Timer Mode. Timer 0 increments on the clock       
                                  ///< defined by T0M in the CKCON0 register.            
#define TMOD_CT0__COUNTER    0x04 ///< Counter Mode. Timer 0 increments on high-to-low   
                                  ///< transitions of an external pin (T0).              
                                                                                         
#define TMOD_GATE0__BMASK    0x08 ///< Timer 0 Gate Control                              
#define TMOD_GATE0__SHIFT    0x03 ///< Timer 0 Gate Control                              
#define TMOD_GATE0__DISABLED 0x00 ///< Timer 0 enabled when TR0 = 1 irrespective of INT0 
                                  ///< logic level.                                      
#define TMOD_GATE0__ENABLED  0x08 ///< Timer 0 enabled only when TR0 = 1 and INT0 is     
                                  ///< active as defined by bit IN0PL in register IT01CF.
                                                                                         
#define TMOD_T1M__FMASK      0x30 ///< Timer 1 Mode Select                               
#define TMOD_T1M__SHIFT      0x04 ///< Timer 1 Mode Select                               
#define TMOD_T1M__MODE0      0x00 ///< Mode 0, 13-bit Counter/Timer                      
#define TMOD_T1M__MODE1      0x10 ///< Mode 1, 16-bit Counter/Timer                      
#define TMOD_T1M__MODE2      0x20 ///< Mode 2, 8-bit Counter/Timer with Auto-Reload      
#define TMOD_T1M__MODE3      0x30 ///< Mode 3, Timer 1 Inactive                          
                                                                                         
#define TMOD_CT1__BMASK      0x40 ///< Counter/Timer 1 Select                            
#define TMOD_CT1__SHIFT      0x06 ///< Counter/Timer 1 Select                            
#define TMOD_CT1__TIMER      0x00 ///< Timer Mode. Timer 1 increments on the clock       
                                  ///< defined by T1M in the CKCON0 register.            
#define TMOD_CT1__COUNTER    0x40 ///< Counter Mode. Timer 1 increments on high-to-low   
                                  ///< transitions of an external pin (T1).              
                                                                                         
#define TMOD_GATE1__BMASK    0x80 ///< Timer 1 Gate Control                              
#define TMOD_GATE1__SHIFT    0x07 ///< Timer 1 Gate Control                              
#define TMOD_GATE1__DISABLED 0x00 ///< Timer 1 enabled when TR1 = 1 irrespective of INT1 
                                  ///< logic level.                                      
#define TMOD_GATE1__ENABLED  0x80 ///< Timer 1 enabled only when TR1 = 1 and INT1 is     
                                  ///< active as defined by bit IN1PL in register IT01CF.
                                                                                         
//------------------------------------------------------------------------------
// SBCON1 Enums (UART1 Baud Rate Generator Control @ 0x94)
//------------------------------------------------------------------------------
#define SBCON1_BPS__FMASK     0x07 ///< Baud Rate Prescaler Select                     
#define SBCON1_BPS__SHIFT     0x00 ///< Baud Rate Prescaler Select                     
#define SBCON1_BPS__DIV_BY_12 0x00 ///< Prescaler = 12.                                
#define SBCON1_BPS__DIV_BY_4  0x01 ///< Prescaler = 4.                                 
#define SBCON1_BPS__DIV_BY_48 0x02 ///< Prescaler = 48.                                
#define SBCON1_BPS__DIV_BY_1  0x03 ///< Prescaler = 1.                                 
#define SBCON1_BPS__DIV_BY_8  0x04 ///< Prescaler = 8.                                 
#define SBCON1_BPS__DIV_BY_16 0x05 ///< Prescaler = 16.                                
#define SBCON1_BPS__DIV_BY_24 0x06 ///< Prescaler = 24.                                
#define SBCON1_BPS__DIV_BY_32 0x07 ///< Prescaler = 32.                                
                                                                                       
#define SBCON1_BREN__BMASK    0x40 ///< Baud Rate Generator Enable                     
#define SBCON1_BREN__SHIFT    0x06 ///< Baud Rate Generator Enable                     
#define SBCON1_BREN__DISABLED 0x00 ///< Disable the baud rate generator. UART1 will not
                                   ///< function.                                      
#define SBCON1_BREN__ENABLED  0x40 ///< Enable the baud rate generator.                
                                                                                       
//------------------------------------------------------------------------------
// SBRLH1 Enums (UART1 Baud Rate Generator High Byte @ 0x96)
//------------------------------------------------------------------------------
#define SBRLH1_BRH__FMASK 0xFF ///< UART1 Baud Rate Reload High
#define SBRLH1_BRH__SHIFT 0x00 ///< UART1 Baud Rate Reload High
                                                               
//------------------------------------------------------------------------------
// SBRLL1 Enums (UART1 Baud Rate Generator Low Byte @ 0x95)
//------------------------------------------------------------------------------
#define SBRLL1_BRL__FMASK 0xFF ///< UART1 Baud Rate Reload Low
#define SBRLL1_BRL__SHIFT 0x00 ///< UART1 Baud Rate Reload Low
                                                              
//------------------------------------------------------------------------------
// SBUF1 Enums (UART1 Serial Port Data Buffer @ 0x92)
//------------------------------------------------------------------------------
#define SBUF1_SBUF1__FMASK 0xFF ///< Serial Port Data Buffer
#define SBUF1_SBUF1__SHIFT 0x00 ///< Serial Port Data Buffer
                                                            
//------------------------------------------------------------------------------
// SCON1 Enums (UART1 Serial Port Control @ 0xC8)
//------------------------------------------------------------------------------
#define SCON1_RI__BMASK             0x01 ///< Receive Interrupt Flag                           
#define SCON1_RI__SHIFT             0x00 ///< Receive Interrupt Flag                           
#define SCON1_RI__NOT_SET           0x00 ///< New data has not been received by UART1.         
#define SCON1_RI__SET               0x01 ///< UART1 received one or more data bytes.           
                                                                                               
#define SCON1_TI__BMASK             0x02 ///< Transmit Interrupt Flag                          
#define SCON1_TI__SHIFT             0x01 ///< Transmit Interrupt Flag                          
#define SCON1_TI__NOT_SET           0x00 ///< A byte of data has not been transmitted by UART1.
#define SCON1_TI__SET               0x02 ///< UART1 transmitted a byte of data.                
                                                                                               
#define SCON1_RBX__BMASK            0x04 ///< Extra Receive Bit                                
#define SCON1_RBX__SHIFT            0x02 ///< Extra Receive Bit                                
#define SCON1_RBX__LOW              0x00 ///< The extra bit or the first stop bit is 0.        
#define SCON1_RBX__HIGH             0x04 ///< The extra bit or the first stop bit is 1.        
                                                                                               
#define SCON1_TBX__BMASK            0x08 ///< Extra Transmission Bit                           
#define SCON1_TBX__SHIFT            0x03 ///< Extra Transmission Bit                           
#define SCON1_TBX__LOW              0x00 ///< Set extra bit to 0 (low).                        
#define SCON1_TBX__HIGH             0x08 ///< Set extra bit to 1 (high).                       
                                                                                               
#define SCON1_REN__BMASK            0x10 ///< Receive Enable                                   
#define SCON1_REN__SHIFT            0x04 ///< Receive Enable                                   
#define SCON1_REN__RECEIVE_DISABLED 0x00 ///< UART1 reception disabled.                        
#define SCON1_REN__RECEIVE_ENABLED  0x10 ///< UART1 reception enabled.                         
                                                                                               
#define SCON1_PERR__BMASK           0x40 ///< Parity Error Flag                                
#define SCON1_PERR__SHIFT           0x06 ///< Parity Error Flag                                
#define SCON1_PERR__NOT_SET         0x00 ///< Parity error has not occurred.                   
#define SCON1_PERR__SET             0x40 ///< Parity error has occurred.                       
                                                                                               
#define SCON1_OVR__BMASK            0x80 ///< Receive FIFO Overrun Flag                        
#define SCON1_OVR__SHIFT            0x07 ///< Receive FIFO Overrun Flag                        
#define SCON1_OVR__NOT_SET          0x00 ///< Receive FIFO overrun has not occurred.           
#define SCON1_OVR__SET              0x80 ///< Receive FIFO overrun has occurred.               
                                                                                               
//------------------------------------------------------------------------------
// SMOD1 Enums (UART1 Mode @ 0x93)
//------------------------------------------------------------------------------
#define SMOD1_SBL__BMASK          0x01 ///< Stop Bit Length                                 
#define SMOD1_SBL__SHIFT          0x00 ///< Stop Bit Length                                 
#define SMOD1_SBL__SHORT          0x00 ///< Short: Stop bit is active for one bit time.     
#define SMOD1_SBL__LONG           0x01 ///< Long: Stop bit is active for two bit times (data
                                       ///< length = 6, 7, or 8 bits) or 1.5 bit times (data
                                       ///< length = 5 bits).                               
                                                                                            
#define SMOD1_XBE__BMASK          0x02 ///< Extra Bit Enable                                
#define SMOD1_XBE__SHIFT          0x01 ///< Extra Bit Enable                                
#define SMOD1_XBE__DISABLED       0x00 ///< Disable the extra bit.                          
#define SMOD1_XBE__ENABLED        0x02 ///< Enable the extra bit.                           
                                                                                            
#define SMOD1_SDL__FMASK          0x0C ///< Data Length                                     
#define SMOD1_SDL__SHIFT          0x02 ///< Data Length                                     
#define SMOD1_SDL__5_BITS         0x00 ///< 5 bits.                                         
#define SMOD1_SDL__6_BITS         0x04 ///< 6 bits.                                         
#define SMOD1_SDL__7_BITS         0x08 ///< 7 bits.                                         
#define SMOD1_SDL__8_BITS         0x0C ///< 8 bits.                                         
                                                                                            
#define SMOD1_PE__BMASK           0x10 ///< Parity Enable                                   
#define SMOD1_PE__SHIFT           0x04 ///< Parity Enable                                   
#define SMOD1_PE__PARITY_DISABLED 0x00 ///< Disable hardware parity.                        
#define SMOD1_PE__PARITY_ENABLED  0x10 ///< Enable hardware parity.                         
                                                                                            
#define SMOD1_SPT__FMASK          0x60 ///< Parity Type                                     
#define SMOD1_SPT__SHIFT          0x05 ///< Parity Type                                     
#define SMOD1_SPT__ODD_PARITY     0x00 ///< Odd.                                            
#define SMOD1_SPT__EVEN_PARITY    0x20 ///< Even.                                           
#define SMOD1_SPT__MARK_PARITY    0x40 ///< Mark.                                           
#define SMOD1_SPT__SPACE_PARITY   0x60 ///< Space.                                          
                                                                                            
#define SMOD1_MCE__BMASK          0x80 ///< Multiprocessor Communication Enable             
#define SMOD1_MCE__SHIFT          0x07 ///< Multiprocessor Communication Enable             
#define SMOD1_MCE__MULTI_DISABLED 0x00 ///< RI will be activated after the data is received.
#define SMOD1_MCE__MULTI_ENABLED  0x80 ///< RI will be activated if the stop bits (and extra
                                       ///< bit if enabled) are 1. The extra bit must be    
                                       ///< enabled using XBE.                              
                                                                                            
//------------------------------------------------------------------------------
// UART1FCN0 Enums (UART1 FIFO Control 0 @ 0x9D)
//------------------------------------------------------------------------------
#define UART1FCN0_RXTH__FMASK     0x03 ///< RX FIFO Threshold                                 
#define UART1FCN0_RXTH__SHIFT     0x00 ///< RX FIFO Threshold                                 
#define UART1FCN0_RXTH__ZERO      0x00 ///< RFRQ will be set anytime new data arrives in the  
                                       ///< RX FIFO (when the RX FIFO is not empty).          
                                                                                              
#define UART1FCN0_RFLSH__BMASK    0x04 ///< RX FIFO Flush                                     
#define UART1FCN0_RFLSH__SHIFT    0x02 ///< RX FIFO Flush                                     
#define UART1FCN0_RFLSH__FLUSH    0x04 ///< Initiate an RX FIFO flush.                        
                                                                                              
#define UART1FCN0_RFRQE__BMASK    0x08 ///< Read Request Interrupt Enable                     
#define UART1FCN0_RFRQE__SHIFT    0x03 ///< Read Request Interrupt Enable                     
#define UART1FCN0_RFRQE__DISABLED 0x00 ///< UART1 interrupts will not be generated when RFRQ  
                                       ///< is set.                                           
#define UART1FCN0_RFRQE__ENABLED  0x08 ///< UART1 interrupts will be generated if RFRQ is set.
                                                                                              
#define UART1FCN0_TXTH__FMASK     0x30 ///< TX FIFO Threshold                                 
#define UART1FCN0_TXTH__SHIFT     0x04 ///< TX FIFO Threshold                                 
#define UART1FCN0_TXTH__ZERO      0x00 ///< TFRQ will be set when the TX FIFO is empty.       
                                                                                              
#define UART1FCN0_TFLSH__BMASK    0x40 ///< TX FIFO Flush                                     
#define UART1FCN0_TFLSH__SHIFT    0x06 ///< TX FIFO Flush                                     
#define UART1FCN0_TFLSH__FLUSH    0x40 ///< Initiate a TX FIFO flush.                         
                                                                                              
#define UART1FCN0_TFRQE__BMASK    0x80 ///< Write Request Interrupt Enable                    
#define UART1FCN0_TFRQE__SHIFT    0x07 ///< Write Request Interrupt Enable                    
#define UART1FCN0_TFRQE__DISABLED 0x00 ///< UART1 interrupts will not be generated when TFRQ  
                                       ///< is set.                                           
#define UART1FCN0_TFRQE__ENABLED  0x80 ///< UART1 interrupts will be generated if TFRQ is set.
                                                                                              
//------------------------------------------------------------------------------
// UART1FCN1 Enums (UART1 FIFO Control 1 @ 0xD8)
//------------------------------------------------------------------------------
#define UART1FCN1_RIE__BMASK       0x01 ///< Receive Interrupt Enable                          
#define UART1FCN1_RIE__SHIFT       0x00 ///< Receive Interrupt Enable                          
#define UART1FCN1_RIE__DISABLED    0x00 ///< The RI flag will not generate UART1 interrupts.   
#define UART1FCN1_RIE__ENABLED     0x01 ///< The RI flag will generate UART1 interrupts when it
                                        ///< is set.                                           
                                                                                               
#define UART1FCN1_RXTO__FMASK      0x06 ///< Receive Timeout                                   
#define UART1FCN1_RXTO__SHIFT      0x01 ///< Receive Timeout                                   
#define UART1FCN1_RXTO__DISABLED   0x00 ///< The receive timeout feature is disabled.          
#define UART1FCN1_RXTO__TIMEOUT_2  0x02 ///< A receive timeout will occur after 2 idle periods 
                                        ///< on the UART RX line.                              
#define UART1FCN1_RXTO__TIMEOUT_4  0x04 ///< A receive timeout will occur after 4 idle periods 
                                        ///< on the UART RX line.                              
#define UART1FCN1_RXTO__TIMEOUT_16 0x06 ///< A receive timeout will occur after 16 idle periods
                                        ///< on the UART RX line.                              
                                                                                               
#define UART1FCN1_RFRQ__BMASK      0x08 ///< Receive FIFO Request                              
#define UART1FCN1_RFRQ__SHIFT      0x03 ///< Receive FIFO Request                              
#define UART1FCN1_RFRQ__NOT_SET    0x00 ///< The number of bytes in the RX FIFO is less than or
                                        ///< equal to RXTH.                                    
#define UART1FCN1_RFRQ__SET        0x08 ///< The number of bytes in the RX FIFO is greater than
                                        ///< RXTH.                                             
                                                                                               
#define UART1FCN1_TIE__BMASK       0x10 ///< Transmit Interrupt Enable                         
#define UART1FCN1_TIE__SHIFT       0x04 ///< Transmit Interrupt Enable                         
#define UART1FCN1_TIE__DISABLED    0x00 ///< The TI flag will not generate UART1 interrupts.   
#define UART1FCN1_TIE__ENABLED     0x10 ///< The TI flag will generate UART1 interrupts when it
                                        ///< is set.                                           
                                                                                               
#define UART1FCN1_TXHOLD__BMASK    0x20 ///< Transmit Hold                                     
#define UART1FCN1_TXHOLD__SHIFT    0x05 ///< Transmit Hold                                     
#define UART1FCN1_TXHOLD__CONTINUE 0x00 ///< The UART will continue to transmit any available  
                                        ///< data in the TX FIFO.                              
#define UART1FCN1_TXHOLD__HOLD     0x20 ///< The UART will not transmit any new data from the  
                                        ///< TX FIFO.                                          
                                                                                               
#define UART1FCN1_TXNF__BMASK      0x40 ///< TX FIFO Not Full                                  
#define UART1FCN1_TXNF__SHIFT      0x06 ///< TX FIFO Not Full                                  
#define UART1FCN1_TXNF__FULL       0x00 ///< The TX FIFO is full.                              
#define UART1FCN1_TXNF__NOT_FULL   0x40 ///< The TX FIFO has room for more data.               
                                                                                               
#define UART1FCN1_TFRQ__BMASK      0x80 ///< Transmit FIFO Request                             
#define UART1FCN1_TFRQ__SHIFT      0x07 ///< Transmit FIFO Request                             
#define UART1FCN1_TFRQ__NOT_SET    0x00 ///< The number of bytes in the TX FIFO is greater than
                                        ///< TXTH.                                             
#define UART1FCN1_TFRQ__SET        0x80 ///< The number of bytes in the TX FIFO is less than or
                                        ///< equal to TXTH.                                    
                                                                                               
//------------------------------------------------------------------------------
// UART1FCT Enums (UART1 FIFO Count @ 0xFA)
//------------------------------------------------------------------------------
#define UART1FCT_RXCNT__FMASK 0x07 ///< RX FIFO Count
#define UART1FCT_RXCNT__SHIFT 0x00 ///< RX FIFO Count
                                                     
#define UART1FCT_TXCNT__FMASK 0x70 ///< TX FIFO Count
#define UART1FCT_TXCNT__SHIFT 0x04 ///< TX FIFO Count
                                                     
//------------------------------------------------------------------------------
// UART1LIN Enums (UART1 LIN Configuration @ 0x9E)
//------------------------------------------------------------------------------
#define UART1LIN_SYNCDIE__BMASK      0x01 ///< LIN Sync Detect Interrupt Enable                  
#define UART1LIN_SYNCDIE__SHIFT      0x00 ///< LIN Sync Detect Interrupt Enable                  
#define UART1LIN_SYNCDIE__DISABLED   0x00 ///< The SYNCD flag will not generate UART1 interrupts.
#define UART1LIN_SYNCDIE__ENABLED    0x01 ///< The SYNCD flag will generate UART1 interrupts when
                                          ///< it is set.                                        
                                                                                                 
#define UART1LIN_SYNCTOIE__BMASK     0x02 ///< LIN Sync Detect Timeout Interrupt Enable          
#define UART1LIN_SYNCTOIE__SHIFT     0x01 ///< LIN Sync Detect Timeout Interrupt Enable          
#define UART1LIN_SYNCTOIE__DISABLED  0x00 ///< The SYNCTO flag will not generate UART1           
                                          ///< interrupts.                                       
#define UART1LIN_SYNCTOIE__ENABLED   0x02 ///< The SYNCTO flag will generate UART1 interrupts    
                                          ///< when it is set.                                   
                                                                                                 
#define UART1LIN_BREAKDNIE__BMASK    0x04 ///< LIN Break Done Interrupt Enable                   
#define UART1LIN_BREAKDNIE__SHIFT    0x02 ///< LIN Break Done Interrupt Enable                   
#define UART1LIN_BREAKDNIE__DISABLED 0x00 ///< The BREAKDN flag will not generate UART1          
                                          ///< interrupts.                                       
#define UART1LIN_BREAKDNIE__ENABLED  0x04 ///< The BREAKDN flag will generate UART1 interrupts   
                                          ///< when it is set.                                   
                                                                                                 
#define UART1LIN_LINMDE__BMASK       0x08 ///< LIN Mode Enable                                   
#define UART1LIN_LINMDE__SHIFT       0x03 ///< LIN Mode Enable                                   
#define UART1LIN_LINMDE__DISABLED    0x00 ///< If AUTOBDE is set to 1, sync detection and        
                                          ///< autobaud will begin on the first falling edge of  
                                          ///< RX.                                               
#define UART1LIN_LINMDE__ENABLED     0x08 ///< A valid LIN break field and delimiter must be     
                                          ///< detected prior to the hardware state machine      
                                          ///< recognizing a sync word and performing autobaud.  
                                                                                                 
#define UART1LIN_SYNCD__BMASK        0x10 ///< LIN Sync Detect Flag                              
#define UART1LIN_SYNCD__SHIFT        0x04 ///< LIN Sync Detect Flag                              
#define UART1LIN_SYNCD__NOT_SET      0x00 ///< A sync has not been detected or is not yet        
                                          ///< complete.                                         
#define UART1LIN_SYNCD__SYNC_DONE    0x10 ///< A valid sync word was detected.                   
                                                                                                 
#define UART1LIN_SYNCTO__BMASK       0x20 ///< LIN Sync Timeout Flag                             
#define UART1LIN_SYNCTO__SHIFT       0x05 ///< LIN Sync Timeout Flag                             
#define UART1LIN_SYNCTO__NOT_SET     0x00 ///< A sync timeout has not occured.                   
#define UART1LIN_SYNCTO__TIMEOUT     0x20 ///< A sync timeout occured.                           
                                                                                                 
#define UART1LIN_BREAKDN__BMASK      0x40 ///< LIN Break Done Flag                               
#define UART1LIN_BREAKDN__SHIFT      0x06 ///< LIN Break Done Flag                               
#define UART1LIN_BREAKDN__NOT_SET    0x00 ///< A LIN break has not been detected.                
#define UART1LIN_BREAKDN__BREAK      0x40 ///< A LIN break was detected since the flag was last  
                                          ///< cleared.                                          
                                                                                                 
#define UART1LIN_AUTOBDE__BMASK      0x80 ///< Auto Baud Detection Enable                        
#define UART1LIN_AUTOBDE__SHIFT      0x07 ///< Auto Baud Detection Enable                        
#define UART1LIN_AUTOBDE__DISABLED   0x00 ///< Autobaud is not enabled.                          
#define UART1LIN_AUTOBDE__ENABLED    0x80 ///< Autobaud is enabled.                              
                                                                                                 
//------------------------------------------------------------------------------
// UART1PCF Enums (UART1 Pin Configuration @ 0xDA)
//------------------------------------------------------------------------------
#define UART1PCF_RXSEL__FMASK    0x03 ///< RX Input Select                           
#define UART1PCF_RXSEL__SHIFT    0x00 ///< RX Input Select                           
#define UART1PCF_RXSEL__CROSSBAR 0x00 ///< RX is connected to the pin assigned by the
                                      ///< crossbar.                                 
#define UART1PCF_RXSEL__CLU0     0x01 ///< RX is connected to the CLU0 output signal.
#define UART1PCF_RXSEL__CLU1     0x02 ///< RX is connected to the CLU1 output signal.
#define UART1PCF_RXSEL__CLU2     0x03 ///< RX is connected to the CLU2 output signal.
                                                                                     
//------------------------------------------------------------------------------
// SBUF0 Enums (UART0 Serial Port Data Buffer @ 0x99)
//------------------------------------------------------------------------------
#define SBUF0_SBUF0__FMASK 0xFF ///< Serial Data Buffer
#define SBUF0_SBUF0__SHIFT 0x00 ///< Serial Data Buffer
                                                       
//------------------------------------------------------------------------------
// SCON0 Enums (UART0 Serial Port Control @ 0x98)
//------------------------------------------------------------------------------
#define SCON0_RI__BMASK             0x01 ///< Receive Interrupt Flag                           
#define SCON0_RI__SHIFT             0x00 ///< Receive Interrupt Flag                           
#define SCON0_RI__NOT_SET           0x00 ///< New data has not been received by UART0.         
#define SCON0_RI__SET               0x01 ///< UART0 received one or more data bytes.           
                                                                                               
#define SCON0_TI__BMASK             0x02 ///< Transmit Interrupt Flag                          
#define SCON0_TI__SHIFT             0x01 ///< Transmit Interrupt Flag                          
#define SCON0_TI__NOT_SET           0x00 ///< A byte of data has not been transmitted by UART0.
#define SCON0_TI__SET               0x02 ///< UART0 transmitted a byte of data.                
                                                                                               
#define SCON0_RB8__BMASK            0x04 ///< Ninth Receive Bit                                
#define SCON0_RB8__SHIFT            0x02 ///< Ninth Receive Bit                                
#define SCON0_RB8__NOT_SET          0x00 ///< In Mode 0, the STOP bit was 0. In Mode 1, the 9th
                                         ///< bit was 0.                                       
#define SCON0_RB8__SET              0x04 ///< In Mode 0, the STOP bit was 1. In Mode 1, the 9th
                                         ///< bit was 1.                                       
                                                                                               
#define SCON0_TB8__BMASK            0x08 ///< Ninth Transmission Bit                           
#define SCON0_TB8__SHIFT            0x03 ///< Ninth Transmission Bit                           
#define SCON0_TB8__NOT_SET          0x00 ///< In Mode 1, set the 9th transmission bit to 0.    
#define SCON0_TB8__SET              0x08 ///< In Mode 1, set the 9th transmission bit to 1.    
                                                                                               
#define SCON0_REN__BMASK            0x10 ///< Receive Enable                                   
#define SCON0_REN__SHIFT            0x04 ///< Receive Enable                                   
#define SCON0_REN__RECEIVE_DISABLED 0x00 ///< UART0 reception disabled.                        
#define SCON0_REN__RECEIVE_ENABLED  0x10 ///< UART0 reception enabled.                         
                                                                                               
#define SCON0_MCE__BMASK            0x20 ///< Multiprocessor Communication Enable              
#define SCON0_MCE__SHIFT            0x05 ///< Multiprocessor Communication Enable              
#define SCON0_MCE__MULTI_DISABLED   0x00 ///< Ignore level of 9th bit / Stop bit.              
#define SCON0_MCE__MULTI_ENABLED    0x20 ///< RI is set and an interrupt is generated only when
                                         ///< the stop bit is logic 1 (Mode 0) or when the 9th 
                                         ///< bit is logic 1 (Mode 1).                         
                                                                                               
#define SCON0_SMODE__BMASK          0x80 ///< Serial Port 0 Operation Mode                     
#define SCON0_SMODE__SHIFT          0x07 ///< Serial Port 0 Operation Mode                     
#define SCON0_SMODE__8_BIT          0x00 ///< 8-bit UART with Variable Baud Rate (Mode 0).     
#define SCON0_SMODE__9_BIT          0x80 ///< 9-bit UART with Variable Baud Rate (Mode 1).     
                                                                                               
//------------------------------------------------------------------------------
// UART0PCF Enums (UART0 Pin Configuration @ 0xD9)
//------------------------------------------------------------------------------
#define UART0PCF_RXSEL__FMASK    0x03 ///< RX Input Select                           
#define UART0PCF_RXSEL__SHIFT    0x00 ///< RX Input Select                           
#define UART0PCF_RXSEL__CROSSBAR 0x00 ///< RX is connected to the pin assigned by the
                                      ///< crossbar.                                 
#define UART0PCF_RXSEL__CLU0     0x01 ///< RX is connected to the CLU0 output signal.
#define UART0PCF_RXSEL__CLU1     0x02 ///< RX is connected to the CLU1 output signal.
#define UART0PCF_RXSEL__CLU2     0x03 ///< RX is connected to the CLU2 output signal.
                                                                                     
//------------------------------------------------------------------------------
// VDM0CN Enums (Supply Monitor Control @ 0xFF)
//------------------------------------------------------------------------------
#define VDM0CN_VDDSTAT__BMASK  0x40 ///< Supply Status                                   
#define VDM0CN_VDDSTAT__SHIFT  0x06 ///< Supply Status                                   
#define VDM0CN_VDDSTAT__BELOW  0x00 ///< VDD is at or below the supply monitor threshold.
#define VDM0CN_VDDSTAT__ABOVE  0x40 ///< VDD is above the supply monitor threshold.      
                                                                                         
#define VDM0CN_VDMEN__BMASK    0x80 ///< Supply Monitor Enable                           
#define VDM0CN_VDMEN__SHIFT    0x07 ///< Supply Monitor Enable                           
#define VDM0CN_VDMEN__DISABLED 0x00 ///< Supply Monitor Disabled.                        
#define VDM0CN_VDMEN__ENABLED  0x80 ///< Supply Monitor Enabled.                         
                                                                                         
//------------------------------------------------------------------------------
// REF0CN Enums (Voltage Reference Control @ 0xD1)
//------------------------------------------------------------------------------
#define REF0CN_VREFSL__FMASK    0xC0 ///< Voltage Reference Output Select             
#define REF0CN_VREFSL__SHIFT    0x06 ///< Voltage Reference Output Select             
#define REF0CN_VREFSL__NONE     0x00 ///< The VREF pin is not driven by an on-chip    
                                     ///< reference. It may be driven with an external
                                     ///< reference.                                  
#define REF0CN_VREFSL__VREF_1P2 0x40 ///< 1.2 V reference output to VREF pin.         
#define REF0CN_VREFSL__VREF_2P4 0x80 ///< 2.4 V reference output to VREF pin.         
                                                                                      
//------------------------------------------------------------------------------
// REG0CN Enums (Voltage Regulator 0 Control @ 0xC9)
//------------------------------------------------------------------------------
#define REG0CN_STOPCF__BMASK    0x08 ///< Stop and Shutdown Mode Configuration              
#define REG0CN_STOPCF__SHIFT    0x03 ///< Stop and Shutdown Mode Configuration              
#define REG0CN_STOPCF__ACTIVE   0x00 ///< Regulator is still active in stop mode. Any       
                                     ///< enabled reset source will reset the device.       
#define REG0CN_STOPCF__SHUTDOWN 0x08 ///< Regulator is shut down in stop mode (device enters
                                     ///< Shutdown mode). Only the RSTb pin or power cycle  
                                     ///< can reset the device.                             
                                                                                            
//------------------------------------------------------------------------------
// WDTCN Enums (Watchdog Timer Control @ 0x97)
//------------------------------------------------------------------------------
#define WDTCN_WDTCN__FMASK 0xFF ///< WDT Control
#define WDTCN_WDTCN__SHIFT 0x00 ///< WDT Control
                                                
//------------------------------------------------------------------------------
// EMI0CN Enums (External Memory Interface Control @ 0xE7)
//------------------------------------------------------------------------------
#define EMI0CN_PGSEL__FMASK 0x0F ///< XRAM Page Select
#define EMI0CN_PGSEL__SHIFT 0x00 ///< XRAM Page Select
                                                      
#endif // SI_EFM8BB3_REGISTER_ENUMS_H
//-eof--------------------------------------------------------------------------

